#include <Client/UI/StdAfx.h>
#include <Client/Python/Player/PythonPlayer.h>
#include <Client/Python/Player/PythonPlayerInput.h>
#include <Client/Python/Player/PythonPlayerEventHandler.h>
#include <Client/Python/Application/PythonApplication.h>
#include <Neoeter/Render/Camera.h>
#include <Neoeter/Platform/Base/Timer.h>

const int c_iFastestSendingCount = 3;
const int c_iSlowestSendingCount = 3;
const float c_fFastestSendingDelay = 1.0f / float(c_iFastestSendingCount);
const float c_fSlowestSendingDelay = 1.0f / float(c_iSlowestSendingCount);
const float c_fRotatingStepTime = 0.5f;

const float c_fComboDistance = 250.0f;
const float c_fClickDistance = 300.0f;

CPythonPlayerInput::CPythonPlayerInput(CPythonPlayer& rkOwner) : m_rkOwner(rkOwner)
{
}

uint32_t CPythonPlayerInput::__GetPickableDistance()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (pkInstMain)
		if (pkInstMain->IsMountingHorse())
			return 500;

	return 300;
}

void CPythonPlayerInput::PickCloseMoney()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	TPixelPosition kPPosMain;
	pkInstMain->GetTransform().NEW_GetPixelPosition(&kPPosMain);

	uint32_t dwItemID;
	CPythonItem& rkItem = CPythonItem::Instance();
	if (!rkItem.GetCloseMoney(kPPosMain, &dwItemID, __GetPickableDistance()))
		return;

	m_rkOwner.SendClickItemPacket(dwItemID);
}

void CPythonPlayerInput::PickCloseItem()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	TPixelPosition kPPosMain;
	pkInstMain->GetTransform().NEW_GetPixelPosition(&kPPosMain);

	uint32_t dwItemID;
	CPythonItem& rkItem = CPythonItem::Instance();
	if (!rkItem.GetCloseItem(kPPosMain, &dwItemID, __GetPickableDistance()))
		return;

	m_rkOwner.SendClickItemPacket(dwItemID);
}

bool CPythonPlayerInput::__IsTarget()
{
	return 0 != __GetTargetVID();
}

bool CPythonPlayerInput::__IsSameTargetVID(uint32_t dwVID)
{
	return dwVID == __GetTargetVID();
}

uint32_t CPythonPlayerInput::__GetTargetVID()
{
	return m_rkOwner.m_dwTargetVID;
}

uint32_t CPythonPlayerInput::GetTargetVID()
{
	return __GetTargetVID();
}

CInstanceBase* CPythonPlayerInput::__GetTargetActorPtr()
{
	return m_rkOwner.NEW_FindActorPtr(__GetTargetVID());
}

void CPythonPlayerInput::__SetTargetVID(uint32_t dwVID)
{
	m_rkOwner.m_dwTargetVID = dwVID;
}

void CPythonPlayerInput::__ClearTarget()
{
	if (!__IsTarget())
		return;

	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	pkInstMain->GetBattle().ClearFlyTargetInstance();

	CInstanceBase* pTargetedInstance = __GetTargetActorPtr();
	if (pTargetedInstance)
		pTargetedInstance->OnUntargeted();

	__SetTargetVID(0);

	CPythonNetworkStream::Instance().SendTargetPacket(0);
}

void CPythonPlayerInput::SetTarget(uint32_t dwVID, int32_t bForceChange)
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	// 2004. 07. 07. [levites] - 스킬 사용중 타겟이 바뀌는 문제 해결을 위한 코드
	if (!pkInstMain->CanChangeTarget())
	{
		return;
	}

	uint32_t dwCurrentTime = CTimer::Instance().GetCurrentMillisecond();

	if (__IsSameTargetVID(dwVID))
	{
		if (dwVID == pkInstMain->GetVirtualID())
		{
			__SetTargetVID(0);

			pkInstMain->OnUntargeted();
			pkInstMain->GetBattle().ClearFlyTargetInstance();
			CPythonNetworkStream::Instance().SendTargetPacket(0);
			return;
		}
		m_rkOwner.m_dwTargetEndTime = dwCurrentTime + 1000;
		return;
	}

	if (bForceChange)
	{
		m_rkOwner.m_dwTargetEndTime = dwCurrentTime + 2000;
	}
	else
	{
		if (m_rkOwner.m_dwTargetEndTime > dwCurrentTime)
		{
			return;
		}

		m_rkOwner.m_dwTargetEndTime = dwCurrentTime + 1000;
	}

	if (__IsTarget())
	{
		CInstanceBase* pTargetedInstance = __GetTargetActorPtr();
		if (pTargetedInstance)
			pTargetedInstance->OnUntargeted();
	}

	CInstanceBase* pkInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(dwVID);
	if (pkInstTarget)
	{
		if (pkInstMain->IsTargetableInstance(*pkInstTarget))
		{
			__SetTargetVID(dwVID);

			pkInstTarget->OnTargeted();
			pkInstMain->GetBattle().SetFlyTargetInstance(*pkInstTarget);
			pkInstMain->GetGraphicThingInstanceRef().SetFlyEventHandler(CPythonPlayerEventHandler::GetSingleton().GetNormalBowAttackFlyEventHandler(pkInstMain, pkInstTarget));
			CPythonNetworkStream::Instance().SendTargetPacket(dwVID);

			return;
		}
	}

	__SetTargetVID(0);

	pkInstMain->GetBattle().ClearFlyTargetInstance();
	CPythonNetworkStream::Instance().SendTargetPacket(0);
}

bool CPythonPlayerInput::__ChangeTargetToPickedInstance()
{
	uint32_t dwVID;
	if (!CPythonCharacterManager::Instance().OLD_GetPickedInstanceVID(&dwVID))
		return false;

	SetTarget(dwVID);
	return true;
}

CInstanceBase* CPythonPlayerInput::__GetSkillTargetInstancePtr(CPythonSkill::TSkillData& rkSkillData)
{
	if (rkSkillData.IsNeedCorpse())
		return __GetDeadTargetInstancePtr();

	return __GetAliveTargetInstancePtr();
}

CInstanceBase* CPythonPlayerInput::__GetDeadTargetInstancePtr()
{
	if (!__IsTarget())
		return NULL;

	CInstanceBase* pkInstTarget = __GetTargetActorPtr();

	if (!pkInstTarget)
		return NULL;

	if (!pkInstTarget->IsDead())
		return NULL;

	return pkInstTarget;
}

CInstanceBase* CPythonPlayerInput::__GetAliveTargetInstancePtr()
{
	if (!__IsTarget())
		return NULL;

	CInstanceBase* pkInstTarget = __GetTargetActorPtr();

	if (!pkInstTarget)
		return NULL;

	if (pkInstTarget->IsDead())
		return NULL;

	return pkInstTarget;
}

void CPythonPlayerInput::OpenCharacterMenu(uint32_t dwVictimActorID)
{
	CInstanceBase* pkInstMain = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	CInstanceBase* pkInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(dwVictimActorID);
	if (!pkInstTarget)
		return;

	if (!pkInstTarget->IsPC() && !pkInstTarget->IsBuilding())
		return;

	//	if (pkInstMain == pkInstTarget)
	//		return;

	PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "SetPCTargetBoard", Py_BuildValue("(is)", pkInstTarget->GetVirtualID(), pkInstTarget->GetNameString()));
}

void CPythonPlayerInput::__OnClickItem(CInstanceBase& rkInstMain, uint32_t dwItemID)
{
}

void CPythonPlayerInput::__OnClickActor(CInstanceBase& rkInstMain, uint32_t dwPickedActorID, bool isAuto)
{
	// 만약 스킬을 써서 접근중이라면..
	if (CPythonPlayer::MODE_USE_SKILL == m_rkOwner.m_eReservedMode)
	{
		// 같은 캐릭터를 클릭 했으면 리턴
		if (__GetTargetVID() == dwPickedActorID)
			return;

		// 2005.03.25.levites
		// 탄환격을 쓰고 달려가는중 공격할 수 있는 다른 타겟을 클릭하면
		if (m_rkOwner.GetSkill().__CheckDashAffect(rkInstMain))
		{
			m_rkOwner.m_dwVIDReserved = dwPickedActorID;
			return;
		}
	}

	__ClearReservedAction();

	CInstanceBase* pkInstVictim = m_rkOwner.NEW_FindActorPtr(dwPickedActorID);
	CInstanceBase& rkInstVictim = *pkInstVictim;
	if (!pkInstVictim)
		return;

	// 2005.01.28.myevan
	// 초급말 상태에서는 공격이 안되나 NPC 클릭이되어야함
	if (rkInstMain.IsAttackableInstance(*pkInstVictim))
		if (!__CanAttack())
			return;

	if (!rkInstMain.GetBattle().NEW_IsClickableDistanceDestInstance(rkInstVictim))
	{
		__ReserveClickActor(dwPickedActorID);
		return;
	}

	if (rkInstVictim.IsNPC())
	{
		m_rkOwner.__SendClickActorPacket(rkInstVictim);
	}

	rkInstMain.GetMovement().NEW_Stop();
	return;
}

void CPythonPlayerInput::__OnPressActor(CInstanceBase& rkInstMain, uint32_t dwPickedActorID, bool isAuto)
{
	// 만약 스킬을 써서 접근중이라면..
	if (CPythonPlayer::MODE_USE_SKILL == m_rkOwner.m_eReservedMode)
	{
		// 같은 캐릭터를 클릭 했으면 리턴
		if (__GetTargetVID() == dwPickedActorID)
			return;

		// 2005.03.25.levites
		// 탄환격을 쓰고 달려가는중 공격할 수 있는 다른 타겟을 클릭하면
		if (m_rkOwner.GetSkill().__CheckDashAffect(rkInstMain))
		{
			m_rkOwner.m_dwVIDReserved = dwPickedActorID;
			return;
		}
	}

	__ChangeTargetToPickedInstance();
	__ClearReservedAction();

	if (!__CanAttack())
		return;

	CInstanceBase* pkInstVictim = m_rkOwner.NEW_FindActorPtr(dwPickedActorID);
	if (!pkInstVictim)
		return;

	CInstanceBase& rkInstVictim = *pkInstVictim;

	if (isAuto)
	{
		// 2004.10.21.myevan.공격가능한 상대만 자동 공격
		if (rkInstMain.IsAttackableInstance(rkInstVictim))
			__SetAutoAttackTargetActorID(rkInstVictim.GetVirtualID());
	}

	if (rkInstMain.IsBowMode())
	{
		if (rkInstMain.IsAttackableInstance(rkInstVictim))
			if (!__CanShot(rkInstMain, rkInstVictim))
				return;
	}

	if (!rkInstMain.GetBattle().NEW_IsClickableDistanceDestInstance(rkInstVictim))
	{
		__ReserveClickActor(dwPickedActorID);
		return;
	}

	if (!rkInstMain.IsAttackableInstance(rkInstVictim))
	{
		return;
	}

	CPythonPlayerEventHandler& rkPlayerEventHandler = CPythonPlayerEventHandler::GetSingleton();
	rkInstMain.GetBattle().NEW_AttackToDestInstanceDirection(rkInstVictim, rkPlayerEventHandler.GetNormalBowAttackFlyEventHandler(&rkInstMain, &rkInstVictim));
}

void CPythonPlayerInput::__OnPressItem(CInstanceBase& rkInstMain, uint32_t dwPickedItemID)
{
	static uint32_t s_dwLastPickItemID = 0;

	if (s_dwLastPickItemID == dwPickedItemID)
	{
		Logn(1, "CPythonPlayerInput::__OnPressItem - ALREADY PICKED ITEM");
		return;
	}

	__ClearReservedAction();
	__ClearAutoAttackTargetActorID();

	CPythonItem& rkItem = CPythonItem::Instance();

	TPixelPosition kPPosPickedItem;
	if (!rkItem.GetGroundItemPosition(dwPickedItemID, &kPPosPickedItem))
		return;

	if (!rkInstMain.GetBattle().NEW_IsClickableDistanceDestPixelPosition(kPPosPickedItem))
	{
		__ReserveClickItem(dwPickedItemID);
		return;
	}

	rkInstMain.GetMovement().NEW_Stop();
	m_rkOwner.SendClickItemPacket(dwPickedItemID);
}

void CPythonPlayerInput::__OnClickGround(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround)
{
	if (!__IsMovableGroundDistance(rkInstMain, c_rkPPosPickedGround))
		return;

	if (rkInstMain.GetMovement().NEW_MoveToDestPixelPositionDirection(c_rkPPosPickedGround))
		__ShowPickedEffect(c_rkPPosPickedGround);
}

void CPythonPlayerInput::SetMovableGroundDistance(float fDistance)
{
	m_rkOwner.MOVABLE_GROUND_DISTANCE = fDistance;
}

bool CPythonPlayerInput::__IsMovableGroundDistance(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround)
{
	float fDistance = rkInstMain.GetBattle().NEW_GetDistanceFromDestPixelPosition(c_rkPPosPickedGround);

	if (fDistance < m_rkOwner.MOVABLE_GROUND_DISTANCE)
		return false;

	// Tracef("IsMovableGroundDistance(%f)\n", fDistance);
	return true;
}

void CPythonPlayerInput::__OnPressGround(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround)
{
	__ClearReservedAction();
	__ClearAutoAttackTargetActorID();

	if (NEW_CancelFishing())
		return;

	if (!__IsMovableGroundDistance(rkInstMain, c_rkPPosPickedGround))
		return;

	if (!rkInstMain.GetMovement().NEW_MoveToDestPixelPositionDirection(c_rkPPosPickedGround))
	{
		__ReserveClickGround(c_rkPPosPickedGround);
		return;
	}
}

void CPythonPlayerInput::__OnPressScreen(CInstanceBase& rkInstMain)
{
	__ClearReservedAction();

	NEW_MoveToMouseScreenDirection();
}

bool CPythonPlayerInput::NEW_MoveToDirection(float fDirRot)
{
	// PrivateShop
	if (m_rkOwner.IsOpenPrivateShop())
		return true;

	__ClearReservedAction();

	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	if (pkInstMain->GetMotion().isLock())
		if (!pkInstMain->GetBattle().IsUsingMovingSkill())
			return true;

	CCamera* pkCmrCur = CCameraManager::Instance().GetCurrentCamera();
	if (pkCmrCur)
	{
		float fCmrCurRot = CameraRotationToCharacterRotation(pkCmrCur->GetRoll());

		// 현재
		if (m_rkOwner.m_isCmrRot)
		{
			float fSigDirRot = fDirRot;
			if (fSigDirRot > 180.0f)
				fSigDirRot = fSigDirRot - 360.0f;

			float fRotRat = fSigDirRot;
			if (fRotRat > 90.0f)
				fRotRat = (180.0f - fRotRat);
			else if (fRotRat < -90.0f)
				fRotRat = (-180.0f - fRotRat);

			float fElapsedTime = CPythonApplication::Instance().GetGlobalElapsedTime();

			float fRotDeg = -m_rkOwner.m_fCmrRotSpd * fElapsedTime * fRotRat / 90.0f;
			pkCmrCur->Roll(fRotDeg);
		}

		fDirRot = fmod(360.0f + fCmrCurRot + fDirRot, 360.0f);
	}

	pkInstMain->GetMovement().NEW_MoveToDirection(fDirRot);

	return true;
}

void CPythonPlayerInput::NEW_Stop()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	pkInstMain->GetMovement().NEW_Stop();
	m_rkOwner.m_isLeft = FALSE;
	m_rkOwner.m_isRight = FALSE;
	m_rkOwner.m_isUp = FALSE;
	m_rkOwner.m_isDown = FALSE;
}

bool CPythonPlayerInput::NEW_CancelFishing()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	if (pkInstMain->IsFishing())
	{
		static uint32_t s_dwLastCancelTime = 0;
		if (CTimer::Instance().GetCurrentMillisecond() < s_dwLastCancelTime + 500)
			return false;

		CPythonNetworkStream::Instance().SendFishingPacket(0);
		s_dwLastCancelTime = CTimer::Instance().GetCurrentMillisecond();
		return true;
	}

	return false;
}

void CPythonPlayerInput::NEW_Fishing()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	if (pkInstMain->IsFishing())
	{
		CPythonNetworkStream::Instance().SendFishingPacket(0);
	}
	else
	{
		if (pkInstMain->CanFishing())
		{
			int irot;
			if (pkInstMain->GetMotion().GetFishingRot(&irot))
				CPythonNetworkStream::Instance().SendFishingPacket(irot);
			else
			{
				PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnFishingWrongPlace", Py_BuildValue("()"));
			}
		}
	}
}

void CPythonPlayerInput::NEW_Attack()
{
	// PrivateShop
	if (m_rkOwner.IsOpenPrivateShop())
		return;

	if (!__CanAttack())
		return;

	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	if (pkInstMain->IsBowMode())
	{
		// CPythonPlayerEventHandler& rkPlayerEventHandler=CPythonPlayerEventHandler::GetSingleton();

		CInstanceBase* pkInstTarget = __GetAliveTargetInstancePtr();
		if (!pkInstTarget)
		{
			__ChangeTargetToPickedInstance();
			pkInstTarget = __GetAliveTargetInstancePtr();
		}

		if (pkInstTarget)
		{
			if (!__CanShot(*pkInstMain, *pkInstTarget))
				return;

			if (!pkInstMain->GetBattle().NEW_IsClickableDistanceDestInstance(*pkInstTarget))
			{
				__ReserveClickActor(pkInstTarget->GetVirtualID());
				return;
			}

			if (pkInstMain->IsAttackableInstance(*pkInstTarget))
			{
				pkInstMain->GetTransform().NEW_LookAtDestInstance(*pkInstTarget);
				pkInstMain->GetBattle().NEW_AttackToDestInstanceDirection(*pkInstTarget);
			}
		}
		else
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NEED_TARGET"));
			return;
		}
	}
	else if (m_rkOwner.m_isDirKey)
	{
		float fDirRot = 0.0f;
		NEW_GetMultiKeyDirRotation(m_rkOwner.m_isLeft, m_rkOwner.m_isRight, m_rkOwner.m_isUp, m_rkOwner.m_isDown, &fDirRot);

		CCamera* pkCmrCur = CCameraManager::Instance().GetCurrentCamera();
		if (pkCmrCur)
		{
			float fCmrCurRot = CameraRotationToCharacterRotation(pkCmrCur->GetRoll());

			fDirRot = fmod(360.0f + fCmrCurRot + fDirRot, 360.0f);
		}

		pkInstMain->GetBattle().NEW_Attack(fDirRot);
	}
	else
	{
		//!@# 말에 탄 상태에서 맨손 공격은 지원되지 않는다 - [levites]
		if (pkInstMain->IsMountingHorse())
		{
			if (pkInstMain->IsHandMode())
				return;
		}

		pkInstMain->GetBattle().NEW_Attack();
	}
}

bool CPythonPlayerInput::NEW_IsEmptyReservedDelayTime(float fElapsedTime)
{
	m_rkOwner.m_fReservedDelayTime -= fElapsedTime;

	if (m_rkOwner.m_fReservedDelayTime <= 0.0f)
	{
		m_rkOwner.m_fReservedDelayTime = 0.0f;
		return true;
	}

	return false;
}

void CPythonPlayerInput::NEW_SetAutoCameraRotationSpeed(float fRotSpd)
{
	m_rkOwner.m_fCmrRotSpd = fRotSpd;
}

void CPythonPlayerInput::NEW_ResetCameraRotation()
{
	CCamera* pkCmrCur = CCameraManager::Instance().GetCurrentCamera();
	CPythonApplication& rkApp = CPythonApplication::Instance();

	pkCmrCur->EndDrag();

	rkApp.Cursor().SetNum(CApplicationCursor::NORMAL);
	if (CApplicationCursor::MODE_HARDWARE == rkApp.Cursor().GetMode())
		rkApp.Cursor().SetVisible(TRUE);
}

bool CPythonPlayerInput::__CanShot(CInstanceBase& rkInstMain, CInstanceBase& rkInstTarget)
{
	if (!m_rkOwner.GetSkill().__HasEnoughArrow())
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotShot", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "EMPTY_ARROW"));
		return false;
	}

	if (rkInstMain.IsInSafe())
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotShot", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "IN_SAFE"));
		return false;
	}

	if (rkInstTarget.IsInSafe())
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotShot", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "DEST_IN_SAFE"));
		return false;
	}

	return true;
}

bool CPythonPlayerInput::__CanChangeTarget()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	return pkInstMain->CanChangeTarget();
}

bool CPythonPlayerInput::__CanMove()
{
	if (m_rkOwner.__IsProcessingEmotion())
	{
		return false;
	}

	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	if (!pkInstMain->CanMove())
	{
		if (!pkInstMain->GetBattle().IsUsingMovingSkill())
			return false;
	}

	return true;
}
/*
bool CPythonPlayerInput::__OLD_CanMove()
{
	if (m_rkOwner.__IsProcessingEmotion())
	{
		return false;
	}

	CInstanceBase* pkInstMain=m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	if (pkInstMain->IsDead())
		return false;

	if (pkInstMain->IsStun())
		return false;

	if (pkInstMain->GetMotion().isLock())
		return false;

	if (pkInstMain->IsParalysis())
		return false;

	return true;
}
*/

bool CPythonPlayerInput::__CanAttack()
{
	if (m_rkOwner.__IsProcessingEmotion())
	{
		return false;
	}

	if (m_rkOwner.IsOpenPrivateShop())
		return false;

	if (m_rkOwner.IsObserverMode())
		return false;

	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	// Fix me
	// 뉴마운트 25레벨 이상 35레벨 미만인 경우 중급 마운트를 타고 공격못하도록 하드 코딩...
	// 나중에 시간 나면 can attack 체크를 서버에서 해주자...
	// ㅡ_ㅡ unique 슬롯에 차는 탈것은 이 조건이랑 관계없이 공격할 수 있어야 한다 ㅡ_ㅡ
	// ㅡ_ㅡ 뉴마운트만 이 체크를 하게 함... ㅡ_ㅡ_ㅡ_ㅡ_ㅡ
	if (pkInstMain->IsMountingHorse() && pkInstMain->IsNewMount() && (m_rkOwner.GetSkillGrade(109) < 1 && m_rkOwner.GetSkillLevel(109) < 11))
	{
		return false;
	}

	return pkInstMain->CanAttack();
}

void CPythonPlayerInput::NEW_GetMultiKeyDirRotation(bool isLeft, bool isRight, bool isUp, bool isDown, float* pfDirRot)
{
	float fScrX = 0.0f;
	float fScrY = 0.0f;

	if (isLeft)
		fScrX = 0.0f;
	else if (isRight)
		fScrX = 1.0f;
	else
		fScrX = 0.5f;

	if (isUp)
		fScrY = 0.0f;
	else if (isDown)
		fScrY = 1.0f;
	else
		fScrY = 0.5f;

	NEW_GetMouseDirRotation(fScrX, fScrY, pfDirRot);
}

void CPythonPlayerInput::NEW_GetMouseDirRotation(float fScrX, float fScrY, float* pfDirRot)
{
	long lWidth = UI::CWindowManager::Instance().GetScreenWidth();
	long lHeight = UI::CWindowManager::Instance().GetScreenHeight();
	int nScrPosX = lWidth * fScrX;
	int nScrPosY = lHeight * fScrY;
	int nScrWidth = lWidth;
	int nScrHeight = lHeight;
	int nScrCenterX = nScrWidth / 2;
	int nScrCenterY = nScrHeight / 2;

	float finputRotation = GetDegreeFromPosition(nScrPosX, nScrPosY, nScrCenterX, nScrCenterY);
	*pfDirRot = finputRotation;
}

float CPythonPlayerInput::GetDegreeFromPosition(int ix, int iy, int iHalfWidth, int iHalfHeight)
{
	D3DXVECTOR3 vtDir(float(ix - iHalfWidth), float(iy - iHalfHeight), 0.0f);
	D3DXVec3Normalize(&vtDir, &vtDir);

	D3DXVECTOR3 vtStan(0, -1, 0);
	float ret = D3DXToDegree(acosf(D3DXVec3Dot(&vtDir, &vtStan)));

	if (vtDir.x < 0.0f)
		ret = 360.0f - ret;

	return 360.0f - ret;
}

void CPythonPlayerInput::__ClearReservedAction()
{
	m_rkOwner.m_eReservedMode = CPythonPlayer::MODE_NONE;
}

void CPythonPlayerInput::__ReserveClickItem(uint32_t dwItemID)
{
	m_rkOwner.m_eReservedMode = CPythonPlayer::MODE_CLICK_ITEM;
	m_rkOwner.m_dwIIDReserved = dwItemID;
}

void CPythonPlayerInput::__ReserveClickActor(uint32_t dwActorID)
{
	m_rkOwner.m_eReservedMode = CPythonPlayer::MODE_CLICK_ACTOR;
	m_rkOwner.m_dwVIDReserved = dwActorID;
}

void CPythonPlayerInput::__ReserveClickGround(const TPixelPosition& c_rkPPosPickedGround)
{
	m_rkOwner.m_eReservedMode = CPythonPlayer::MODE_CLICK_POSITION;
	m_rkOwner.m_kPPosReserved = c_rkPPosPickedGround;
	m_rkOwner.m_fReservedDelayTime = 0.1f;
}

bool CPythonPlayerInput::__IsReservedUseSkill(uint32_t dwSkillSlotIndex)
{
	if (CPythonPlayer::MODE_USE_SKILL != m_rkOwner.m_eReservedMode)
		return false;

	if (m_rkOwner.m_dwSkillSlotIndexReserved != dwSkillSlotIndex)
		return false;

	return true;
}

void CPythonPlayerInput::__ReserveUseSkill(uint32_t dwActorID, uint32_t dwSkillSlotIndex, uint32_t dwRange)
{
	m_rkOwner.m_eReservedMode = CPythonPlayer::MODE_USE_SKILL;
	m_rkOwner.m_dwVIDReserved = dwActorID;
	m_rkOwner.m_dwSkillSlotIndexReserved = dwSkillSlotIndex;
	m_rkOwner.m_dwSkillRangeReserved = dwRange;

	// NOTE : 아슬아슬하게 거리가 꼬이는 문제가 있어서 약간 느슨하게..
	if (m_rkOwner.m_dwSkillRangeReserved > 100)
		m_rkOwner.m_dwSkillRangeReserved -= 10;
}

void CPythonPlayerInput::__ClearAutoAttackTargetActorID()
{
	__SetAutoAttackTargetActorID(0);
}

void CPythonPlayerInput::__SetAutoAttackTargetActorID(uint32_t dwVID)
{
	m_rkOwner.m_dwAutoAttackTargetVID = dwVID;
}

void CPythonPlayerInput::__ReserveProcess_ClickActor()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	CInstanceBase* pkInstReserved = m_rkOwner.NEW_FindActorPtr(m_rkOwner.m_dwVIDReserved);
	if (!pkInstMain || !pkInstReserved)
	{
		__ClearReservedAction();
		return;
	}

	if (!pkInstMain->GetBattle().NEW_IsClickableDistanceDestInstance(*pkInstReserved))
	{
		pkInstMain->GetMovement().NEW_MoveToDestInstanceDirection(*pkInstReserved);
		return;
	}

	if (!pkInstMain->IsAttackableInstance(*pkInstReserved))
	{
		pkInstMain->GetMovement().NEW_Stop();
		m_rkOwner.__SendClickActorPacket(*pkInstReserved);
		__ClearReservedAction();
		return;
	}

	if (pkInstReserved->IsDead())
	{
		__ClearReservedAction();
		return;
	}

	if (pkInstMain->IsInSafe())
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotAttack", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "IN_SAFE"));
		pkInstMain->GetMovement().NEW_Stop();
		__ClearReservedAction();
		return;
	}

	if (pkInstReserved->IsInSafe())
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotAttack", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "DEST_IN_SAFE"));
		pkInstMain->GetMovement().NEW_Stop();
		__ClearReservedAction();
		return;
	}

	// 탄환격 쓰고 달려가는 도중에는 공격하지 않는다.
	if (m_rkOwner.GetSkill().__CheckDashAffect(*pkInstMain))
	{
		return;
	}

	/////

	if (pkInstMain->IsBowMode())
	{
		if (!m_rkOwner.GetSkill().__HasEnoughArrow())
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotShot", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "EMPTY_ARROW"));
			pkInstMain->GetMovement().NEW_Stop();
			__ClearReservedAction();
			return;
		}
	}

	if (pkInstReserved->GetVirtualID() != GetTargetVID())
	{
		SetTarget(pkInstReserved->GetVirtualID());
	}

	pkInstMain->GetBattle().NEW_AttackToDestInstanceDirection(*pkInstReserved);
	__ClearReservedAction();
}
/*
CInstanceBase* pkInstReserved=m_rkOwner.NEW_FindActorPtr(m_rkOwner.m_dwVIDReserved);
if (pkInstReserved)
{
	if (pkInstMain->GetBattle().NEW_IsClickableDistanceDestInstance(*pkInstReserved))
	{
		if (pkInstMain->IsAttackableInstance(*pkInstReserved) )
		{
			if (!pkInstReserved->IsDead())
			{
				if (pkInstMain->IsInSafe())
				{
					PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotAttack", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "IN_SAFE"));
					pkInstMain->GetMovement().NEW_Stop();
				}
				else if (pkInstReserved->IsInSafe())
				{
					PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotAttack", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "DEST_IN_SAFE"));
					pkInstMain->GetMovement().NEW_Stop();
				}
				else
				{
					if (pkInstMain->IsBowMode())
					{
						if (!m_rkOwner.GetSkill().__HasEnoughArrow())
						{
							PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotShot", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "EMPTY_ARROW"));
							pkInstMain->GetMovement().NEW_Stop();
							__ClearReservedAction();
							break;
						}
					}

					if (pkInstReserved->GetVirtualID() != GetTargetVID())
					{
						SetTarget(pkInstReserved->GetVirtualID());
					}

					pkInstMain->GetBattle().NEW_AttackToDestInstanceDirection(*pkInstReserved);
				}
			}
		}
		else
		{
			m_rkOwner.__SendClickActorPacket(*pkInstReserved);

			pkInstMain->GetMovement().NEW_Stop();
		}

		__ClearReservedAction();
	}
	else
	{
		//Tracen("ReservedMode: MOVE");
		pkInstMain->GetMovement().NEW_MoveToDestInstanceDirection(*pkInstReserved);
	}
}
else
{
	__ClearReservedAction();
}
*/
