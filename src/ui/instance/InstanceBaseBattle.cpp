#include <Client/UI/StdAfx.h>
#include <Client/UI/Instance/InstanceBase.h>
#include <Client/Feature/Background/PythonBackground.h>
#include <Client/Feature/Character/PythonCharacterManager.h>
#include <Neoeter/Terrain/Terrain.h>

float NEW_UnsignedDegreeToSignedDegree(float fUD)
{
	float fSD;
	if (fUD > 180.0f)
		fSD = -(360.0f - fUD);
	else if (fUD < -180.0f)
		fSD = +(360.0f + fUD);
	else
		fSD = fUD;

	return fSD;
}

float NEW_GetSignedDegreeFromDirPixelPosition(const TPixelPosition& kPPosDir)
{
	D3DXVECTOR3 vtDir(kPPosDir.x, -kPPosDir.y, kPPosDir.z);
	D3DXVECTOR3 vtDirNormal;
	D3DXVec3Normalize(&vtDirNormal, &vtDir);

	D3DXVECTOR3 vtDirNormalStan(0, -1, 0);
	float fDirRot = D3DXToDegree(acosf(D3DXVec3Dot(&vtDirNormal, &vtDirNormalStan)));

	if (vtDirNormal.x < 0.0f)
		fDirRot = -fDirRot;

	return fDirRot;
}

CInstanceBaseBattle::CInstanceBaseBattle(CInstanceBase& rkOwner) : m_rkOwner(rkOwner)
{
}

bool CInstanceBaseBattle::IsFlyTargetObject()
{
	return m_rkOwner.m_GraphicThingInstance.IsFlyTargetObject();
}

float CInstanceBaseBattle::GetFlyTargetDistance()
{
	return m_rkOwner.m_GraphicThingInstance.GetFlyTargetDistance();
}

void CInstanceBaseBattle::ClearFlyTargetInstance()
{
	m_rkOwner.m_GraphicThingInstance.ClearFlyTarget();
}

void CInstanceBaseBattle::SetFlyTargetInstance(CInstanceBase& rkInstDst)
{
	// NOTE : NEW_Attack 때 Target을 바꿀때 여기서 리턴 되어버림 - [levites]
	//	if (GetMotion().isLock())
	//		return;

	m_rkOwner.m_GraphicThingInstance.SetFlyTarget(rkInstDst.GetGraphicThingInstancePtr());
}

void CInstanceBaseBattle::AddFlyTargetPosition(const TPixelPosition& c_rkPPosDst)
{
	m_rkOwner.m_GraphicThingInstance.AddFlyTarget(c_rkPPosDst);
}

void CInstanceBaseBattle::AddFlyTargetInstance(CInstanceBase& rkInstDst)
{
	m_rkOwner.m_GraphicThingInstance.AddFlyTarget(rkInstDst.GetGraphicThingInstancePtr());
}

float CInstanceBaseBattle::NEW_GetDistanceFromDestInstance(CInstanceBase& rkInstDst)
{
	TPixelPosition kPPosDst;
	rkInstDst.GetTransform().NEW_GetPixelPosition(&kPPosDst);

	return NEW_GetDistanceFromDestPixelPosition(kPPosDst);
}

float CInstanceBaseBattle::NEW_GetDistanceFromDestPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	TPixelPosition kPPosCur;
	m_rkOwner.GetTransform().NEW_GetPixelPosition(&kPPosCur);

	TPixelPosition kPPosDir;
	kPPosDir = c_rkPPosDst - kPPosCur;

	return NEW_GetDistanceFromDirPixelPosition(kPPosDir);
}

float CInstanceBaseBattle::NEW_GetDistanceFromDirPixelPosition(const TPixelPosition& c_rkPPosDir)
{
	return sqrtf(c_rkPPosDir.x * c_rkPPosDir.x + c_rkPPosDir.y * c_rkPPosDir.y);
}

float CInstanceBaseBattle::NEW_GetRotation()
{
	float fCurRot = m_rkOwner.GetTransform().GetRotation();
	return NEW_UnsignedDegreeToSignedDegree(fCurRot);
}

float CInstanceBaseBattle::NEW_GetRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir)
{
	return NEW_GetSignedDegreeFromDirPixelPosition(c_rkPPosDir);
}

float CInstanceBaseBattle::NEW_GetRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	TPixelPosition kPPosCur;
	m_rkOwner.GetTransform().NEW_GetPixelPosition(&kPPosCur);

	TPixelPosition kPPosDir;
	kPPosDir = c_rkPPosDst - kPPosCur;

	return NEW_GetRotationFromDirPixelPosition(kPPosDir);
}

float CInstanceBaseBattle::NEW_GetRotationFromDestInstance(CInstanceBase& rkInstDst)
{
	TPixelPosition kPPosDst;
	rkInstDst.GetTransform().NEW_GetPixelPosition(&kPPosDst);

	return NEW_GetRotationFromDestPixelPosition(kPPosDst);
}

void CInstanceBaseBattle::NEW_GetRandomPositionInFanRange(CInstanceBase& rkInstTarget, TPixelPosition* pkPPosDst)
{
	float fDstDirRot = NEW_GetRotationFromDestInstance(rkInstTarget);

	float fRot = frandom(fDstDirRot - 10.0f, fDstDirRot + 10.0f);

	D3DXMATRIX kMatRot;
	D3DXMatrixRotationZ(&kMatRot, D3DXToRadian(-fRot));

	D3DXVECTOR3 v3Src(0.0f, 8000.0f, 0.0f);
	D3DXVECTOR3 v3Pos;
	D3DXVec3TransformCoord(&v3Pos, &v3Src, &kMatRot);

	const TPixelPosition& c_rkPPosCur = m_rkOwner.NEW_GetCurPixelPositionRef();
	// const TPixelPosition& c_rkPPosFront=rkInstTarget.m_rkOwner.NEW_GetCurPixelPositionRef();

	pkPPosDst->x = c_rkPPosCur.x + v3Pos.x;
	pkPPosDst->y = c_rkPPosCur.y + v3Pos.y;
	pkPPosDst->z = m_rkOwner.__GetBackgroundHeight(c_rkPPosCur.x, c_rkPPosCur.y);
}

bool CInstanceBaseBattle::NEW_GetFrontInstance(CInstanceBase** ppoutTargetInstance, float fDistance)
{
	const float HALF_FAN_ROT_MIN = 10.0f;
	const float HALF_FAN_ROT_MAX = 50.0f;
	const float HALF_FAN_ROT_MIN_DISTANCE = 1000.0f;
	const float RPM = (HALF_FAN_ROT_MAX - HALF_FAN_ROT_MIN) / HALF_FAN_ROT_MIN_DISTANCE;

	float fDstRot = NEW_GetRotation();

	std::multimap<float, CInstanceBase*> kMap_pkInstNear;
	{
		CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
		CPythonCharacterManager::CharacterIterator i;
		for (i = rkChrMgr.CharacterInstanceBegin(); i != rkChrMgr.CharacterInstanceEnd(); ++i)
		{
			CInstanceBase* pkInstEach = *i;
			if (pkInstEach == &m_rkOwner)
				continue;

			if (!m_rkOwner.IsAttackableInstance(*pkInstEach))
				continue;

			if (NEW_GetDistanceFromDestInstance(*pkInstEach) > fDistance)
				continue;

			float fEachInstDistance = min(NEW_GetDistanceFromDestInstance(*pkInstEach), HALF_FAN_ROT_MIN_DISTANCE);
			float fEachInstDirRot = NEW_GetRotationFromDestInstance(*pkInstEach);

			float fHalfFanRot = (HALF_FAN_ROT_MAX - HALF_FAN_ROT_MIN) - RPM * fEachInstDistance + HALF_FAN_ROT_MIN;

			float fMinDstDirRot = fDstRot - fHalfFanRot;
			float fMaxDstDirRot = fDstRot + fHalfFanRot;

			if (fEachInstDirRot >= fMinDstDirRot && fEachInstDirRot <= fMaxDstDirRot)
				kMap_pkInstNear.insert(std::multimap<float, CInstanceBase*>::value_type(fEachInstDistance, pkInstEach));
		}
	}

	if (kMap_pkInstNear.empty())
		return false;

	*ppoutTargetInstance = kMap_pkInstNear.begin()->second;

	return true;
}

// 2004.07.21.levites - 비파부 다중 타겟 지원
bool CInstanceBaseBattle::NEW_GetInstanceVectorInFanRange(float fSkillDistance, CInstanceBase& rkInstTarget, std::vector<CInstanceBase*>* pkVct_pkInst)
{
	const float HALF_FAN_ROT_MIN = 20.0f;
	const float HALF_FAN_ROT_MAX = 40.0f;
	const float HALF_FAN_ROT_MIN_DISTANCE = 1000.0f;
	const float RPM = (HALF_FAN_ROT_MAX - HALF_FAN_ROT_MIN) / HALF_FAN_ROT_MIN_DISTANCE;

	float fDstDirRot = NEW_GetRotationFromDestInstance(rkInstTarget);

	// 2004.07.24.myevan - 비파부 가까이 있는 적부터 공격
	std::multimap<float, CInstanceBase*> kMap_pkInstNear;
	{
		CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
		CPythonCharacterManager::CharacterIterator i;
		for (i = rkChrMgr.CharacterInstanceBegin(); i != rkChrMgr.CharacterInstanceEnd(); ++i)
		{
			CInstanceBase* pkInstEach = *i;
			if (pkInstEach == &m_rkOwner)
				continue;

			// 2004.07.25.myevan - 적인 경우만 추가한다
			if (!m_rkOwner.IsAttackableInstance(*pkInstEach))
				continue;

			// 2004.07.21.levites - 비파부 다중 타겟 지원
			if (m_rkOwner.m_GraphicThingInstance.GetBattle().IsClickableDistanceDestInstance(pkInstEach->m_GraphicThingInstance, fSkillDistance))
			{
				float fEachInstDistance = min(NEW_GetDistanceFromDestInstance(*pkInstEach), HALF_FAN_ROT_MIN_DISTANCE);
				float fEachInstDirRot = NEW_GetRotationFromDestInstance(*pkInstEach);

				float fHalfFanRot = (HALF_FAN_ROT_MAX - HALF_FAN_ROT_MIN) - RPM * fEachInstDistance + HALF_FAN_ROT_MIN;

				float fMinDstDirRot = fDstDirRot - fHalfFanRot;
				float fMaxDstDirRot = fDstDirRot + fHalfFanRot;

				if (fEachInstDirRot >= fMinDstDirRot && fEachInstDirRot <= fMaxDstDirRot)
					kMap_pkInstNear.insert(std::multimap<float, CInstanceBase*>::value_type(fEachInstDistance, pkInstEach));
			}
		}
	}

	{
		std::multimap<float, CInstanceBase*>::iterator i = kMap_pkInstNear.begin();
		for (i = kMap_pkInstNear.begin(); i != kMap_pkInstNear.end(); ++i)
			pkVct_pkInst->push_back(i->second);
	}

	if (pkVct_pkInst->empty())
		return false;

	return true;
}

bool CInstanceBaseBattle::NEW_GetInstanceVectorInCircleRange(float fSkillDistance, std::vector<CInstanceBase*>* pkVct_pkInst)
{
	std::multimap<float, CInstanceBase*> kMap_pkInstNear;

	{
		CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
		CPythonCharacterManager::CharacterIterator i;
		for (i = rkChrMgr.CharacterInstanceBegin(); i != rkChrMgr.CharacterInstanceEnd(); ++i)
		{
			CInstanceBase* pkInstEach = *i;

			// 자신인 경우 추가하지 않는다
			if (pkInstEach == &m_rkOwner)
				continue;

			// 적인 경우만 추가한다
			if (!m_rkOwner.IsAttackableInstance(*pkInstEach))
				continue;

			if (m_rkOwner.m_GraphicThingInstance.GetBattle().IsClickableDistanceDestInstance(pkInstEach->m_GraphicThingInstance, fSkillDistance))
			{
				float fEachInstDistance = NEW_GetDistanceFromDestInstance(*pkInstEach);
				kMap_pkInstNear.insert(std::make_pair(fEachInstDistance, pkInstEach));
			}
		}
	}

	{
		std::multimap<float, CInstanceBase*>::iterator i = kMap_pkInstNear.begin();
		for (i = kMap_pkInstNear.begin(); i != kMap_pkInstNear.end(); ++i)
			pkVct_pkInst->push_back(i->second);
	}

	if (pkVct_pkInst->empty())
		return false;

	return true;
}

int32_t CInstanceBaseBattle::NEW_IsClickableDistanceDestPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	float fDistance = NEW_GetDistanceFromDestPixelPosition(c_rkPPosDst);

	if (fDistance > 150.0f)
		return FALSE;

	return TRUE;
}

int32_t CInstanceBaseBattle::NEW_IsClickableDistanceDestInstance(CInstanceBase& rkInstDst)
{
	float fDistance = 150.0f;

	if (m_rkOwner.IsBowMode())
		fDistance = m_rkOwner.__GetBowRange();

	if (rkInstDst.IsNPC())
		fDistance = 500.0f;

	if (rkInstDst.IsResource())
		fDistance = 100.0f;

	return m_rkOwner.m_GraphicThingInstance.GetBattle().IsClickableDistanceDestInstance(rkInstDst.m_GraphicThingInstance, fDistance);
}

bool CInstanceBaseBattle::NEW_UseSkill(uint32_t uSkill, uint32_t uMot, uint32_t uMotLoopCount, bool isMovingSkill)
{
	if (m_rkOwner.IsDead())
		return false;

	if (m_rkOwner.IsStun())
		return false;

	if (m_rkOwner.GetMovement().IsKnockDown())
		return false;

	if (isMovingSkill)
	{
		if (!m_rkOwner.GetMovement().IsWalking())
			m_rkOwner.GetMovement().StartWalking();

		m_rkOwner.m_isGoing = TRUE;
	}
	else
	{
		if (m_rkOwner.GetMovement().IsWalking())
			m_rkOwner.GetMovement().EndWalking();

		m_rkOwner.m_isGoing = FALSE;
	}

	float fCurRot = m_rkOwner.m_GraphicThingInstance.GetTargetRotation();
	m_rkOwner.GetMovement().SetAdvancingRotation(fCurRot);

	m_rkOwner.m_GraphicThingInstance.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_SKILL + uMot, 0.1f, uSkill, 1.0f);

	m_rkOwner.m_GraphicThingInstance.__OnUseSkill(uMot, uMotLoopCount, isMovingSkill);

	if (uMotLoopCount > 0)
		m_rkOwner.m_GraphicThingInstance.GetMotion().SetMotionLoopCount(uMotLoopCount);

	return true;
}

void CInstanceBaseBattle::NEW_Attack()
{
	float fDirRot = m_rkOwner.GetTransform().GetRotation();
	NEW_Attack(fDirRot);
}

void CInstanceBaseBattle::NEW_Attack(float fDirRot)
{
	if (m_rkOwner.IsDead())
		return;

	if (m_rkOwner.IsStun())
		return;

	if (m_rkOwner.GetMovement().IsKnockDown())
		return;

	if (IsUsingSkill())
		return;

	if (m_rkOwner.GetMovement().IsWalking())
		m_rkOwner.GetMovement().EndWalking();

	m_rkOwner.m_isGoing = FALSE;

	if (m_rkOwner.IsPoly())
	{
		InputNormalAttack(fDirRot);
	}
	else
	{
		if (m_rkOwner.m_kHorse.IsMounting())
		{
			InputComboAttack(fDirRot);
		}
		else
		{
			InputComboAttack(fDirRot);
		}
	}
}

void CInstanceBaseBattle::NEW_AttackToDestPixelPositionDirection(const TPixelPosition& c_rkPPosDst)
{
	float fDirRot = NEW_GetRotationFromDestPixelPosition(c_rkPPosDst);

	NEW_Attack(fDirRot);
}

bool CInstanceBaseBattle::NEW_AttackToDestInstanceDirection(CInstanceBase& rkInstDst, IFlyEventHandler* pkFlyHandler)
{
	return NEW_AttackToDestInstanceDirection(rkInstDst);
}

bool CInstanceBaseBattle::NEW_AttackToDestInstanceDirection(CInstanceBase& rkInstDst)
{
	TPixelPosition kPPosDst;
	rkInstDst.GetTransform().NEW_GetPixelPosition(&kPPosDst);
	NEW_AttackToDestPixelPositionDirection(kPPosDst);

	return true;
}

void CInstanceBaseBattle::AttackProcess()
{
	if (!m_rkOwner.m_GraphicThingInstance.GetBattle().CanCheckAttacking())
		return;

	CInstanceBase* pkInstLast = NULL;
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CPythonCharacterManager::CharacterIterator i = rkChrMgr.CharacterInstanceBegin();
	while (rkChrMgr.CharacterInstanceEnd() != i)
	{
		CInstanceBase* pkInstEach = *i;
		++i;

		// 서로간의 InstanceType 비교
		if (!m_rkOwner.IsAttackableInstance(*pkInstEach))
			continue;

		if (pkInstEach != &m_rkOwner)
		{
			if (CheckAttacking(*pkInstEach))
			{
				pkInstLast = pkInstEach;
			}
		}
	}

	if (pkInstLast)
	{
		m_rkOwner.m_dwLastDmgActorVID = pkInstLast->GetVirtualID();
	}
}

void CInstanceBaseBattle::InputNormalAttack(float fAtkDirRot)
{
	m_rkOwner.m_GraphicThingInstance.GetBattle().InputNormalAttackCommand(fAtkDirRot);
}

void CInstanceBaseBattle::InputComboAttack(float fAtkDirRot)
{
	m_rkOwner.m_GraphicThingInstance.GetBattle().InputComboAttackCommand(fAtkDirRot);
	m_rkOwner.GetEffect().__ComboProcess();
}

void CInstanceBaseBattle::RunNormalAttack(float fAtkDirRot)
{
	m_rkOwner.GetMovement().EndGoing();
	m_rkOwner.m_GraphicThingInstance.GetBattle().NormalAttack(fAtkDirRot);
}

void CInstanceBaseBattle::RunComboAttack(float fAtkDirRot, uint32_t wMotionIndex)
{
	m_rkOwner.GetMovement().EndGoing();
	m_rkOwner.m_GraphicThingInstance.GetBattle().ComboAttack(wMotionIndex, fAtkDirRot);
}

// 리턴값 TRUE가 무엇인가가 있다
int32_t CInstanceBaseBattle::CheckAdvancing()
{
#ifdef __MOVIE_MODE__
	if (IsMovieMode())
		return FALSE;
#endif
	if (!m_rkOwner.__IsMainInstance() && !m_rkOwner.GetMovement().IsAttacking())
	{
		if (m_rkOwner.IsPC() && m_rkOwner.GetMovement().IsWalking())
		{
			CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
			for (CPythonCharacterManager::CharacterIterator i = rkChrMgr.CharacterInstanceBegin(); i != rkChrMgr.CharacterInstanceEnd(); ++i)
			{
				CInstanceBase* pkInstEach = *i;
				if (pkInstEach == &m_rkOwner)
					continue;
				if (!pkInstEach->IsDoor())
					continue;

				if (m_rkOwner.m_GraphicThingInstance.GetCollisionDetection().TestActorCollision(pkInstEach->GetGraphicThingInstanceRef()))
				{
					m_rkOwner.BlockMovement();
					return true;
				}
			}
		}
		return FALSE;
	}

	if (m_rkOwner.m_GraphicThingInstance.GetCollisionDetection().CanSkipCollision())
	{
		// Tracenf("%x VID %d Skip collisions", ELTimer_GetMSec(), GetVirtualID());
		return FALSE;
	}

	int32_t bUsingSkill = m_rkOwner.m_GraphicThingInstance.GetMotion().IsUsingSkill();

	m_rkOwner.m_dwAdvActorVID = 0;
	uint32_t uCollisionCount = 0;

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	for (CPythonCharacterManager::CharacterIterator i = rkChrMgr.CharacterInstanceBegin(); i != rkChrMgr.CharacterInstanceEnd(); ++i)
	{
		CInstanceBase* pkInstEach = *i;
		if (pkInstEach == &m_rkOwner)
			continue;

		CActorInstance& rkActorSelf = m_rkOwner.m_GraphicThingInstance;
		CActorInstance& rkActorEach = pkInstEach->GetGraphicThingInstanceRef();

		// NOTE : Skil을 쓰더라도 Door Type과는 Collision체크 한다.
		if (bUsingSkill && !rkActorEach.IsDoor())
			continue;

		// 앞으로 전진할수 있는가?
		if (rkActorSelf.GetCollisionDetection().TestActorCollision(rkActorEach))
		{
			uCollisionCount++;
			if (uCollisionCount == 2)
			{
				rkActorSelf.GetCollisionDetection().BlockMovement();
				return TRUE;
			}
			rkActorSelf.AdjustDynamicCollisionMovement(&rkActorEach);

			if (rkActorSelf.GetCollisionDetection().TestActorCollision(rkActorEach))
			{
				rkActorSelf.GetCollisionDetection().BlockMovement();
				return TRUE;
			}
			else
			{
				m_rkOwner.GetMovement().NEW_MoveToDestPixelPositionDirection(m_rkOwner.NEW_GetDstPixelPositionRef());
			}
		}
	}

	// 맵속성 체크
	CPythonBackground& rkBG = CPythonBackground::Instance();
	const D3DXVECTOR3& rv3Position = m_rkOwner.m_GraphicThingInstance.GetPosition();
	const D3DXVECTOR3& rv3MoveDirection = m_rkOwner.m_GraphicThingInstance.GetMovementVectorRef();

	// NOTE : 만약 이동 거리가 크다면 쪼개서 구간 별로 속성을 체크해 본다
	//        현재 설정해 놓은 10.0f는 임의의 거리 - [levites]
	int iStep = int(D3DXVec3Length(&rv3MoveDirection) / 10.0f);
	D3DXVECTOR3 v3CheckStep = rv3MoveDirection / float(iStep);
	D3DXVECTOR3 v3CheckPosition = rv3Position;
	for (int j = 0; j < iStep; ++j)
	{
		v3CheckPosition += v3CheckStep;

		// Check
		if (rkBG.isAttrOn(v3CheckPosition.x, -v3CheckPosition.y, CTerrainImpl::ATTRIBUTE_BLOCK))
		{
			m_rkOwner.BlockMovement();
			// return TRUE;
		}
	}

	// Check
	D3DXVECTOR3 v3NextPosition = rv3Position + rv3MoveDirection;
	if (rkBG.isAttrOn(v3NextPosition.x, -v3NextPosition.y, CTerrainImpl::ATTRIBUTE_BLOCK))
	{
		m_rkOwner.BlockMovement();
		return TRUE;
	}

	return FALSE;
}

int32_t CInstanceBaseBattle::CheckAttacking(CInstanceBase& rkInstVictim)
{
	if (m_rkOwner.IsInSafe())
		return FALSE;

	if (rkInstVictim.IsInSafe())
		return FALSE;

#ifdef __MOVIE_MODE__
	return FALSE;
#endif

	if (!m_rkOwner.m_GraphicThingInstance.GetCollisionDetection().AttackingProcess(rkInstVictim.m_GraphicThingInstance))
		return FALSE;

	return TRUE;
}

int32_t CInstanceBaseBattle::isNormalAttacking()
{
	return m_rkOwner.m_GraphicThingInstance.GetBattle().isNormalAttacking();
}

int32_t CInstanceBaseBattle::isComboAttacking()
{
	return m_rkOwner.m_GraphicThingInstance.GetBattle().isComboAttacking();
}

int32_t CInstanceBaseBattle::IsUsingSkill()
{
	return m_rkOwner.m_GraphicThingInstance.GetMotion().IsUsingSkill();
}

int32_t CInstanceBaseBattle::IsUsingMovingSkill()
{
	return m_rkOwner.m_GraphicThingInstance.GetBattle().IsUsingMovingSkill();
}

int32_t CInstanceBaseBattle::CanCancelSkill()
{
	return m_rkOwner.m_GraphicThingInstance.GetMotion().CanCancelSkill();
}

int32_t CInstanceBaseBattle::CanAttackHorseLevel()
{
	if (!m_rkOwner.IsMountingHorse())
		return FALSE;

	return m_rkOwner.m_kHorse.CanAttack();
}

bool CInstanceBaseBattle::IsAffect(uint32_t uAffect)
{
	return m_rkOwner.m_kAffectFlagContainer.IsSet(uAffect);
}

MOTION_KEY CInstanceBaseBattle::GetNormalAttackIndex()
{
	return m_rkOwner.m_GraphicThingInstance.GetNormalAttackIndex();
}

uint32_t CInstanceBaseBattle::GetComboIndex()
{
	return m_rkOwner.m_GraphicThingInstance.GetBattle().GetComboIndex();
}

float CInstanceBaseBattle::GetAttackingElapsedTime()
{
	return m_rkOwner.m_GraphicThingInstance.GetBattle().GetAttackingElapsedTime();
}

void CInstanceBaseBattle::ProcessHitting(uint32_t dwMotionKey, CInstanceBase* pVictimInstance)
{
	assert(!"-_-" && "CInstanceBaseBattle::ProcessHitting");
	// m_rkOwner.m_GraphicThingInstance.ProcessSucceedingAttacking(dwMotionKey, pVictimInstance->m_GraphicThingInstance);
}

void CInstanceBaseBattle::ProcessHitting(uint32_t dwMotionKey, uint8_t byEventIndex, CInstanceBase* pVictimInstance)
{
	assert(!"-_-" && "CInstanceBaseBattle::ProcessHitting");
	// m_rkOwner.m_GraphicThingInstance.ProcessSucceedingAttacking(dwMotionKey, byEventIndex, pVictimInstance->m_GraphicThingInstance);
}

void CInstanceBaseBattle::GetBlendingPosition(TPixelPosition* pPixelPosition)
{
	m_rkOwner.m_GraphicThingInstance.GetBattle().GetBlendingPosition(pPixelPosition);
}

void CInstanceBaseBattle::SetBlendingPosition(const TPixelPosition& c_rPixelPosition)
{
	m_rkOwner.m_GraphicThingInstance.GetBattle().SetBlendingPosition(c_rPixelPosition);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void CInstanceBaseBattle::Revive()
{
	m_rkOwner.m_isGoing = FALSE;
	m_rkOwner.m_GraphicThingInstance.Revive();

	m_rkOwner.__AttachHorseSaddle();
}

void CInstanceBaseBattle::Stun()
{
	m_rkOwner.GetMovement().NEW_Stop();
	m_rkOwner.m_GraphicThingInstance.Stun();

	m_rkOwner.GetEffect().__AttachEffect(CInstanceBase::EFFECT_STUN);
}

void CInstanceBaseBattle::Die()
{
	m_rkOwner.__DetachHorseSaddle();

	if (IsAffect(CInstanceBase::AFFECT_SPAWN))
		m_rkOwner.GetEffect().__AttachEffect(CInstanceBase::EFFECT_SPAWN_DISAPPEAR);

	// 2004.07.25.이펙트 안붙는 문제해결
	////////////////////////////////////////
	m_rkOwner.GetEffect().__ClearAffects();
	////////////////////////////////////////

	m_rkOwner.OnUnselected();
	m_rkOwner.OnUntargeted();

	m_rkOwner.m_GraphicThingInstance.Die();
}

void CInstanceBaseBattle::Hide()
{
	m_rkOwner.m_GraphicThingInstance.SetAlphaValue(0.0f);
	m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(0.0f, 0.1f);
}

void CInstanceBaseBattle::Show()
{
	m_rkOwner.m_GraphicThingInstance.SetAlphaValue(1.0f);
	m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(1.0f, 0.1f);
}
