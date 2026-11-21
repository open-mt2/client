#include <Client/Game/StdAfx.h>
#include <Neoeter/Effect/EffectManager.h>
#include <Neoeter/Audio/SoundManager.h>

#include <Client/Game/ActorInstance.h>
#include <Client/Game/CActorInstanceBattle.h>
#include <Client/Game/RaceData.h>

CActorInstanceBattle::CActorInstanceBattle(CActorInstance& rkOwner) : m_rkOwner(rkOwner)
{
}

void CActorInstanceBattle::SetBattleHitEffect(uint32_t dwID)
{
	m_rkOwner.m_dwBattleHitEffectID = dwID;
}

void CActorInstanceBattle::SetBattleAttachEffect(uint32_t dwID)
{
	m_rkOwner.m_dwBattleAttachEffectID = dwID;
}

bool CActorInstanceBattle::CanAct()
{
	if (m_rkOwner.IsDead())
		return false;

	if (m_rkOwner.IsStun())
		return false;

	if (m_rkOwner.IsParalysis())
		return false;

	if (m_rkOwner.IsFaint())
		return false;

	if (m_rkOwner.IsSleep())
		return false;

	return true;
}

bool CActorInstanceBattle::CanUseSkill()
{
	if (!CanAct())
		return false;

	uint32_t dwCurMotionIndex = m_rkOwner.GetMotion().__GetCurrentMotionIndex();

	// Locked during attack
	switch (dwCurMotionIndex)
	{
		case CRaceMotionData::NAME_FISHING_THROW:
		case CRaceMotionData::NAME_FISHING_WAIT:
		case CRaceMotionData::NAME_FISHING_STOP:
		case CRaceMotionData::NAME_FISHING_REACT:
		case CRaceMotionData::NAME_FISHING_CATCH:
		case CRaceMotionData::NAME_FISHING_FAIL:
			return TRUE;
			break;
	}

	// Locked during using skill
	if (m_rkOwner.GetMotion().IsUsingSkill())
	{
		if (m_rkOwner.m_pkCurRaceMotionData->IsCancelEnableSkill())
			return TRUE;

		return FALSE;
	}

	return true;
}

bool CActorInstanceBattle::CanMove()
{
	if (!CanAct())
		return false;

	if (m_rkOwner.GetMotion().isLock())
		return false;

	return true;
}

bool CActorInstanceBattle::CanAttack()
{
	if (!CanAct())
		return false;

	if (m_rkOwner.GetMotion().IsUsingSkill())
	{
		if (!m_rkOwner.GetMotion().CanCancelSkill())
			return false;
	}

	return true;
}

bool CActorInstanceBattle::CanFishing()
{
	if (!CanAct())
		return false;

	if (m_rkOwner.GetMotion().IsUsingSkill())
		return false;

	switch (m_rkOwner.GetMotion().__GetCurrentMotionIndex())
	{
		case CRaceMotionData::NAME_WAIT:
		case CRaceMotionData::NAME_WALK:
		case CRaceMotionData::NAME_RUN:
			break;
		default:
			return false;
			break;
	}

	return true;
}

int32_t CActorInstanceBattle::IsClickableDistanceDestInstance(CActorInstance& rkInstDst, float fDistance)
{
	TPixelPosition kPPosSrc;
	m_rkOwner.GetPixelPosition(&kPPosSrc);

	D3DXVECTOR3 kD3DVct3Src(kPPosSrc);

	CActorInstance::TCollisionPointInstanceList& rkLstkDefPtInst = rkInstDst.m_DefendingPointInstanceList;
	CActorInstance::TCollisionPointInstanceList::iterator i;

	for (i = rkLstkDefPtInst.begin(); i != rkLstkDefPtInst.end(); ++i)
	{
		CDynamicSphereInstanceVector& rkVctkDefSphere = (*i).SphereInstanceVector;

		CDynamicSphereInstanceVector::iterator j;
		for (j = rkVctkDefSphere.begin(); j != rkVctkDefSphere.end(); ++j)
		{
			CDynamicSphereInstance& rkSphere = (*j);

			float fMovDistance = D3DXVec3Length(&D3DXVECTOR3(rkSphere.v3Position - kD3DVct3Src));
			float fAtkDistance = rkSphere.fRadius + fDistance;

			if (fAtkDistance > fMovDistance)
				return TRUE;
		}
	}

	return FALSE;
}

void CActorInstanceBattle::InputNormalAttackCommand(float fDirRot)
{
	if (!__CanInputNormalAttackCommand())
		return;

	m_rkOwner.m_fAtkDirRot = fDirRot;
	NormalAttack(m_rkOwner.m_fAtkDirRot);
}

bool CActorInstanceBattle::InputComboAttackCommand(float fDirRot)
{
	m_rkOwner.m_fAtkDirRot = fDirRot;

	if (m_rkOwner.m_isPreInput)
		return false;

	/////////////////////////////////////////////////////////////////////////////////

	// Process Input
	if (0 == m_rkOwner.m_dwcurComboIndex)
	{
		__RunNextCombo();
		return true;
	}
	else if (m_rkOwner.m_pkCurRaceMotionData->IsComboInputTimeData())
	{
		// 동작 경과 시간
		float fElapsedTime = GetAttackingElapsedTime();

		// 이미 입력 한계 시간이 지났다면..
		if (fElapsedTime > m_rkOwner.m_pkCurRaceMotionData->GetComboInputEndTime())
		{
			// Tracen("Input Timeout");
			if (m_rkOwner.IsBowMode())
				m_rkOwner.m_isNextPreInput = TRUE;
			return false;
		}

		if (fElapsedTime > m_rkOwner.m_pkCurRaceMotionData->GetNextComboTime()) // 콤보 발동 시간 이 후라면
		{
			// Tracen("Next combo action");
			//  args : BlendingTime
			__RunNextCombo();
			return true;
		}
		else if (fElapsedTime > m_rkOwner.m_pkCurRaceMotionData->GetComboInputStartTime()) // 선 입력 시간 범위 라면..
		{
			// Tracen("Setting up line input");
			m_rkOwner.m_isPreInput = TRUE;
			return false;
		}
	}
	else
	{
		float fElapsedTime = GetAttackingElapsedTime();
		if (fElapsedTime > m_rkOwner.m_pkCurRaceMotionData->GetMotionDuration() * 0.9f) // 콤보 발동 시간 이 후라면
		{
			// Tracen("Next combo action");
			//  args : BlendingTime
			__RunNextCombo();
			return true;
		}
	}
	// Process Input

	return false;
}

void CActorInstanceBattle::ComboProcess()
{
	// If combo is on action
	if (0 != m_rkOwner.m_dwcurComboIndex)
	{
		if (!m_rkOwner.m_pkCurRaceMotionData)
		{
			Tracef("Attacking motion data is NULL! : %d\n", m_rkOwner.m_dwcurComboIndex);
			__ClearCombo();
			return;
		}

		float fElapsedTime = GetAttackingElapsedTime();

		// Process PreInput
		if (m_rkOwner.m_isPreInput)
		{
			// Tracenf("Prefix %f NextComboTime %f", fElapsedTime, m_rkOwner.m_pkCurRaceMotionData->GetNextComboTime());
			if (fElapsedTime > m_rkOwner.m_pkCurRaceMotionData->GetNextComboTime())
			{
				__RunNextCombo();
				m_rkOwner.m_isPreInput = FALSE;

				return;
			}
		}
	}
	else
	{
		m_rkOwner.m_isPreInput = FALSE;

		if (!m_rkOwner.GetMotion().IsUsingSkill()) // m_rkOwner.m_isNextPreInput는 활모드 일때만 사용하는 변수
			if (m_rkOwner.m_isNextPreInput)		   // 활일때만 스킬이 캔슬 되는건 이곳 때문임
			{
				__RunNextCombo();
				m_rkOwner.m_isNextPreInput = FALSE;
			}
	}
}

void CActorInstanceBattle::__RunNextCombo()
{
	++m_rkOwner.m_dwcurComboIndex;
	///////////////////////////

	uint16_t wComboIndex = m_rkOwner.m_dwcurComboIndex;
	uint16_t wComboType = m_rkOwner.__GetCurrentComboType();

	if (wComboIndex == 0)
	{
		TraceError("CActorInstanceBattle::__RunNextCombo(wComboType=%d, wComboIndex=%d)", wComboType, wComboIndex);
		return;
	}

	uint32_t dwComboArrayIndex = wComboIndex - 1;

	CRaceData::TComboData* pComboData;

	if (!m_rkOwner.m_pkCurRaceData->GetComboDataPointer(m_rkOwner.m_wcurMotionMode, wComboType, &pComboData))
	{
		TraceError("CActorInstanceBattle::__RunNextCombo(wComboType=%d, wComboIndex=%d) - m_rkOwner.m_pkCurRaceData->GetComboDataPointer(m_rkOwner.m_wcurMotionMode=%d, &pComboData) == NULL",
				   wComboType, wComboIndex, m_rkOwner.m_wcurMotionMode);
		return;
	}

	if (dwComboArrayIndex >= pComboData->ComboIndexVector.size())
	{
		TraceError("CActorInstanceBattle::__RunNextCombo(wComboType=%d, wComboIndex=%d) - (dwComboArrayIndex=%d) >= (pComboData->ComboIndexVector.size()=%d)",
				   wComboType, wComboIndex, dwComboArrayIndex, pComboData->ComboIndexVector.size());
		return;
	}

	uint16_t wcurComboMotionIndex = pComboData->ComboIndexVector[dwComboArrayIndex];
	ComboAttack(wcurComboMotionIndex, m_rkOwner.m_fAtkDirRot, 0.1f);

	////////////////////////////////
	// 콤보가 끝났다면
	if (m_rkOwner.m_dwcurComboIndex == pComboData->ComboIndexVector.size())
	{
		__OnEndCombo();
	}
}

void CActorInstanceBattle::__OnEndCombo()
{
	if (m_rkOwner.__IsMountingHorse())
	{
		m_rkOwner.m_dwcurComboIndex = 1;
	}

	// 여기서 콤보를 초기화 해선 안된다.
	// 콤보가 초기화 되는 곳은 마지막 콤보가 끝나고 Motion 이 자동으로 Wait 으로 돌아가는 시점이다.
}

void CActorInstanceBattle::__ClearCombo()
{
	m_rkOwner.m_dwcurComboIndex = 0;
	m_rkOwner.m_isPreInput = FALSE;
	m_rkOwner.m_pkCurRaceMotionData = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

int32_t CActorInstanceBattle::isAttacking()
{
	if (isNormalAttacking())
		return TRUE;

	if (isComboAttacking())
		return TRUE;

	if (IsSplashAttacking())
		return TRUE;

	return FALSE;
}

int32_t CActorInstanceBattle::isValidAttacking()
{
	if (!m_rkOwner.m_pkCurRaceMotionData)
		return FALSE;

	if (!m_rkOwner.m_pkCurRaceMotionData->isAttackingMotion())
		return FALSE;

	const NRaceData::TMotionAttackData* c_pData = m_rkOwner.m_pkCurRaceMotionData->GetMotionAttackDataPointer();
	float fElapsedTime = GetAttackingElapsedTime();
	NRaceData::THitDataContainer::const_iterator itor = c_pData->HitDataContainer.begin();
	for (; itor != c_pData->HitDataContainer.end(); ++itor)
	{
		const NRaceData::THitData& c_rHitData = *itor;
		if (fElapsedTime > c_rHitData.fAttackStartTime &&
			fElapsedTime < c_rHitData.fAttackEndTime)
			return TRUE;
	}

	return TRUE;
}

int32_t CActorInstanceBattle::CanCheckAttacking()
{
	if (isAttacking())
		return true;

	return false;
}

bool CActorInstanceBattle::__IsInSplashTime()
{
	if (m_rkOwner.m_kSplashArea.fDisappearingTime > m_rkOwner.GetLocalTime())
		return true;

	return false;
}

int32_t CActorInstanceBattle::isNormalAttacking()
{
	if (!m_rkOwner.m_pkCurRaceMotionData)
		return FALSE;

	if (!m_rkOwner.m_pkCurRaceMotionData->isAttackingMotion())
		return FALSE;

	const NRaceData::TMotionAttackData* c_pData = m_rkOwner.m_pkCurRaceMotionData->GetMotionAttackDataPointer();
	if (NRaceData::MOTION_TYPE_NORMAL != c_pData->iMotionType)
		return FALSE;

	return TRUE;
}

int32_t CActorInstanceBattle::isComboAttacking()
{
	if (!m_rkOwner.m_pkCurRaceMotionData)
		return FALSE;

	if (!m_rkOwner.m_pkCurRaceMotionData->isAttackingMotion())
		return FALSE;

	const NRaceData::TMotionAttackData* c_pData = m_rkOwner.m_pkCurRaceMotionData->GetMotionAttackDataPointer();
	if (NRaceData::MOTION_TYPE_COMBO != c_pData->iMotionType)
		return FALSE;

	return TRUE;
}

int32_t CActorInstanceBattle::IsSplashAttacking()
{
	if (!m_rkOwner.m_pkCurRaceMotionData)
		return FALSE;

	if (m_rkOwner.m_pkCurRaceMotionData->HasSplashMotionEvent())
		return TRUE;

	return FALSE;
}

int32_t CActorInstanceBattle::__IsMovingSkill(uint16_t wSkillNumber)
{
	enum
	{
		HORSE_DASH_SKILL_NUMBER = 137,
	};

	return HORSE_DASH_SKILL_NUMBER == wSkillNumber;
}

int32_t CActorInstanceBattle::IsActEmotion()
{
	uint32_t dwCurMotionIndex = m_rkOwner.GetMotion().__GetCurrentMotionIndex();
	switch (dwCurMotionIndex)
	{
		case CRaceMotionData::NAME_FRENCH_KISS_START + 0:
		case CRaceMotionData::NAME_FRENCH_KISS_START + 1:
		case CRaceMotionData::NAME_FRENCH_KISS_START + 2:
		case CRaceMotionData::NAME_FRENCH_KISS_START + 3:
		case CRaceMotionData::NAME_KISS_START + 0:
		case CRaceMotionData::NAME_KISS_START + 1:
		case CRaceMotionData::NAME_KISS_START + 2:
		case CRaceMotionData::NAME_KISS_START + 3:
			return TRUE;
			break;
	}

	return FALSE;
}

int32_t CActorInstanceBattle::IsUsingMovingSkill()
{
	return __IsMovingSkill(m_rkOwner.m_kCurMotNode.uSkill);
}

uint32_t CActorInstanceBattle::GetComboIndex()
{
	return m_rkOwner.m_dwcurComboIndex;
}

float CActorInstanceBattle::GetAttackingElapsedTime()
{
	return (m_rkOwner.GetLocalTime() - m_rkOwner.m_kCurMotNode.fStartTime) * m_rkOwner.m_kCurMotNode.fSpeedRatio;
	//	return (m_rkOwner.GetLocalTime() - m_rkOwner.m_kCurMotNode.fStartTime) * m_rkOwner.__GetAttackSpeed();
}

bool CActorInstanceBattle::__CanInputNormalAttackCommand()
{
	if (m_rkOwner.IsWaiting())
		return true;

	if (isNormalAttacking())
	{
		float fElapsedTime = GetAttackingElapsedTime();

		if (fElapsedTime > m_rkOwner.m_pkCurRaceMotionData->GetMotionDuration() * 0.9f)
			return true;
	}

	return false;
}

int32_t CActorInstanceBattle::NormalAttack(float fDirRot, float fBlendTime)
{
	uint16_t wMotionIndex;
	if (!m_rkOwner.m_pkCurRaceData->GetNormalAttackIndex(m_rkOwner.m_wcurMotionMode, &wMotionIndex))
		return FALSE;

	m_rkOwner.BlendRotation(fDirRot, fBlendTime);
	m_rkOwner.SetAdvancingRotation(fDirRot);
	m_rkOwner.GetMotion().InterceptOnceMotion(wMotionIndex, 0.1f, 0, m_rkOwner.__GetAttackSpeed());

	m_rkOwner.__OnAttack(wMotionIndex);

	m_rkOwner.NEW_SetAtkPixelPosition(m_rkOwner.NEW_GetCurPixelPositionRef());

	return TRUE;
}

int32_t CActorInstanceBattle::ComboAttack(uint32_t dwMotionIndex, float fDirRot, float fBlendTime)
{
	m_rkOwner.BlendRotation(fDirRot, fBlendTime);
	m_rkOwner.SetAdvancingRotation(fDirRot);

	m_rkOwner.GetMotion().InterceptOnceMotion(dwMotionIndex, fBlendTime, 0, m_rkOwner.__GetAttackSpeed());

	m_rkOwner.__OnAttack(dwMotionIndex);

	m_rkOwner.NEW_SetAtkPixelPosition(m_rkOwner.NEW_GetCurPixelPositionRef());

	return TRUE;
}

void CActorInstanceBattle::__ProcessMotionEventAttackSuccess(uint32_t dwMotionKey, uint8_t byEventIndex, CActorInstance& rVictim)
{
	CRaceMotionData* pMotionData;

	if (!m_rkOwner.m_pkCurRaceData->GetMotionDataPointer(dwMotionKey, &pMotionData))
		return;

	if (byEventIndex >= pMotionData->GetMotionEventDataCount())
		return;

	const CRaceMotionData::TMotionAttackingEventData* pMotionEventData;
	if (!pMotionData->GetMotionAttackingEventDataPointer(byEventIndex, &pMotionEventData))
		return;

	const D3DXVECTOR3& c_rv3VictimPos = rVictim.GetPositionVectorRef();
	__ProcessDataAttackSuccess(pMotionEventData->AttackData, rVictim, c_rv3VictimPos);
}

void CActorInstanceBattle::__ProcessMotionAttackSuccess(uint32_t dwMotionKey, CActorInstance& rVictim)
{
	CRaceMotionData* c_pMotionData;

	if (!m_rkOwner.m_pkCurRaceData->GetMotionDataPointer(dwMotionKey, &c_pMotionData))
		return;

	const D3DXVECTOR3& c_rv3VictimPos = rVictim.GetPositionVectorRef();
	__ProcessDataAttackSuccess(c_pMotionData->GetMotionAttackDataReference(), rVictim, c_rv3VictimPos);
}

uint32_t CActorInstanceBattle::__GetOwnerVID()
{
	return m_rkOwner.m_dwOwnerVID;
}

float CActorInstanceBattle::__GetOwnerTime()
{
	return m_rkOwner.GetLocalTime() - m_rkOwner.m_fOwnerBaseTime;
}

bool IS_HUGE_RACE(unsigned int vnum)
{
	switch (vnum)
	{
		case 2493:
			return true;
	}
	return false;
}

bool CActorInstanceBattle::__CanPushDestActor(CActorInstance& rkActorDst)
{
	if (rkActorDst.IsBuilding())
		return false;

	if (rkActorDst.IsDoor())
		return false;

	if (rkActorDst.IsStone())
		return false;

	if (rkActorDst.IsNPC())
		return false;

	// 거대 몬스터 밀림 제외
	extern bool IS_HUGE_RACE(unsigned int vnum);
	if (IS_HUGE_RACE(rkActorDst.GetRace()))
		return false;

	if (rkActorDst.IsStun())
		return true;

	if (rkActorDst.GetBattle().__GetOwnerVID() != m_rkOwner.GetVirtualID())
		return false;

	if (rkActorDst.GetBattle().__GetOwnerTime() > 3.0f)
		return false;

	return true;
}

bool IS_PARTY_HUNTING_RACE(unsigned int vnum)
{
	return true;

	// 모든 몬스터 파티 사냥 적용
	/*
	if (vnum < 8) // 플레이어
		return true;

	if (vnum >= 8000 && vnum <= 8112) // 메틴석
		return true;

	if (vnum >= 2400 && vnum <  5000) // 천의 동굴 이후 몬스터
		return true;

	return false;
	*/
}

void CActorInstanceBattle::__ProcessDataAttackSuccess(const NRaceData::TAttackData& c_rAttackData, CActorInstance& rVictim, const D3DXVECTOR3& c_rv3Position, uint32_t uiSkill, int32_t isSendPacket)
{
	if (NRaceData::HIT_TYPE_NONE == c_rAttackData.iHittingType)
		return;

	m_rkOwner.InsertDelay(c_rAttackData.fStiffenTime);

	if (__CanPushDestActor(rVictim) && c_rAttackData.fExternalForce > 0.0f)
	{
		__PushCircle(rVictim);

		// VICTIM_COLLISION_TEST
		const D3DXVECTOR3& kVictimPos = rVictim.GetPosition();
		rVictim.m_PhysicsObject.IncreaseExternalForce(kVictimPos, c_rAttackData.fExternalForce); //*nForceRatio/100.0f);

		// VICTIM_COLLISION_TEST_END
	}

	// Invisible Time
	if (IS_PARTY_HUNTING_RACE(rVictim.GetRace()))
	{
		if (uiSkill) // 파티 사냥 몬스터라도 스킬이면 무적시간 적용
			rVictim.m_fInvisibleTime = CTimer::Instance().GetCurrentSecond() + c_rAttackData.fInvisibleTime;

		if (m_rkOwner.m_isMain) // #0000794: [M2KR] 폴리모프 - 밸런싱 문제 타인 공격에 의한 무적 타임은 고려하지 않고 자신 공격에 의한것만 체크한다
			rVictim.m_fInvisibleTime = CTimer::Instance().GetCurrentSecond() + c_rAttackData.fInvisibleTime;
	}
	else // 파티 사냥 몬스터가 아닐 경우만 적용
	{
		rVictim.m_fInvisibleTime = CTimer::Instance().GetCurrentSecond() + c_rAttackData.fInvisibleTime;
	}

	// Stiffen Time
	rVictim.InsertDelay(c_rAttackData.fStiffenTime);

	// Hit Effect
	D3DXVECTOR3 vec3Effect(rVictim.m_x, rVictim.m_y, rVictim.m_z);

	// #0000780: [M2KR] 수룡 타격구 문제
	extern bool IS_HUGE_RACE(unsigned int vnum);
	if (IS_HUGE_RACE(rVictim.GetRace()))
	{
		vec3Effect = c_rv3Position;
	}

	const D3DXVECTOR3& v3Pos = m_rkOwner.GetPosition();

	float fHeight = D3DXToDegree(atan2(-vec3Effect.x + v3Pos.x, +vec3Effect.y - v3Pos.y));

	// 2004.08.03.myevan.빌딩이나 문의 경우 타격 효과가 보이지 않는다
	if (rVictim.IsBuilding() || rVictim.IsDoor())
	{
		D3DXVECTOR3 vec3Delta = vec3Effect - v3Pos;
		D3DXVec3Normalize(&vec3Delta, &vec3Delta);
		vec3Delta *= 30.0f;

		CEffectManager& rkEftMgr = CEffectManager::Instance();
		if (m_rkOwner.m_dwBattleHitEffectID)
			rkEftMgr.CreateEffect(m_rkOwner.m_dwBattleHitEffectID, v3Pos + vec3Delta, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		CEffectManager& rkEftMgr = CEffectManager::Instance();
		if (m_rkOwner.m_dwBattleHitEffectID)
			rkEftMgr.CreateEffect(m_rkOwner.m_dwBattleHitEffectID, vec3Effect, D3DXVECTOR3(0.0f, 0.0f, fHeight));
		if (m_rkOwner.m_dwBattleAttachEffectID)
			rVictim.GetAttach().AttachEffectByID(0, NULL, m_rkOwner.m_dwBattleAttachEffectID);
	}

	if (rVictim.IsBuilding())
	{
		// 2004.08.03.빌딩의 경우 흔들리면 이상하다
	}
	else if (rVictim.IsStone() || rVictim.IsDoor())
	{
		__HitStone(rVictim);
	}
	else
	{
		///////////
		// Motion
		if (NRaceData::HIT_TYPE_GOOD == c_rAttackData.iHittingType || rVictim.IsResistFallen())
		{
			__HitGood(rVictim);
		}
		else if (NRaceData::HIT_TYPE_GREAT == c_rAttackData.iHittingType)
		{
			__HitGreate(rVictim);
		}
		else
		{
			TraceError("ProcessSucceedingAttacking: Unknown AttackingData.iHittingType %d", c_rAttackData.iHittingType);
		}
	}

	m_rkOwner.__OnHit(uiSkill, rVictim, isSendPacket);
}

void CActorInstance::OnShootDamage()
{
	if (IsStun())
	{
		Die();
	}
	else
	{
		GetBattle().__Shake(100);

		if (!GetMotion().isLock() && !GetMotion().__IsKnockDownMotion() && !GetMotion().__IsStandUpMotion())
		{
			if (GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE))
				GetMotion().PushLoopMotion(CRaceMotionData::NAME_WAIT);
		}
	}
}

void CActorInstanceBattle::__Shake(uint32_t dwDuration)
{
	uint32_t dwCurTime = ELTimer_GetMSec();
	m_rkOwner.m_dwShakeTime = dwCurTime + dwDuration;
}

void CActorInstanceBattle::ShakeProcess()
{
	if (m_rkOwner.m_dwShakeTime)
	{
		D3DXVECTOR3 v3Pos(0.0f, 0.0f, 0.0f);

		uint32_t dwCurTime = ELTimer_GetMSec();

		if (m_rkOwner.m_dwShakeTime < dwCurTime)
		{
			m_rkOwner.m_dwShakeTime = 0;
		}
		else
		{
			int nShakeSize = 10;

			switch (rand() % 2)
			{
				case 0:
					v3Pos.x += rand() % nShakeSize;
					break;
				case 1:
					v3Pos.x -= rand() % nShakeSize;
					break;
			}

			switch (rand() % 2)
			{
				case 0:
					v3Pos.y += rand() % nShakeSize;
					break;
				case 1:
					v3Pos.y -= rand() % nShakeSize;
					break;
			}

			switch (rand() % 2)
			{
				case 0:
					v3Pos.z += rand() % nShakeSize;
					break;
				case 1:
					v3Pos.z -= rand() % nShakeSize;
					break;
			}
		}

		m_rkOwner.m_worldMatrix._41 += v3Pos.x;
		m_rkOwner.m_worldMatrix._42 += v3Pos.y;
		m_rkOwner.m_worldMatrix._43 += v3Pos.z;
	}
}

void CActorInstanceBattle::__HitStone(CActorInstance& rVictim)
{
	if (rVictim.IsStun())
	{
		rVictim.Die();
	}
	else
	{
		rVictim.GetBattle().__Shake(100);
	}
}

void CActorInstanceBattle::__HitGood(CActorInstance& rVictim)
{
	if (rVictim.IsKnockDown())
		return;

	if (rVictim.IsStun())
	{
		rVictim.Die();
	}
	else
	{
		rVictim.GetBattle().__Shake(100);

		if (!rVictim.GetMotion().isLock())
		{
			float fRotRad = D3DXToRadian(m_rkOwner.GetRotation());
			float fVictimRotRad = D3DXToRadian(rVictim.GetRotation());

			D3DXVECTOR2 v2Normal(sin(fRotRad), cos(fRotRad));
			D3DXVECTOR2 v2VictimNormal(sin(fVictimRotRad), cos(fVictimRotRad));

			D3DXVec2Normalize(&v2Normal, &v2Normal);
			D3DXVec2Normalize(&v2VictimNormal, &v2VictimNormal);

			float fScalar = D3DXVec2Dot(&v2Normal, &v2VictimNormal);

			if (fScalar < 0.0f)
			{
				if (rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE))
					rVictim.GetMotion().PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
			else
			{
				if (rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_BACK))
					rVictim.GetMotion().PushLoopMotion(CRaceMotionData::NAME_WAIT);
				else if (rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE))
					rVictim.GetMotion().PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
		}
	}
}

void CActorInstanceBattle::__HitGreate(CActorInstance& rVictim)
{
	// DISABLE_KNOCKDOWN_ATTACK
	if (rVictim.IsKnockDown())
		return;
	if (rVictim.GetMotion().__IsStandUpMotion())
		return;
	// END_OF_DISABLE_KNOCKDOWN_ATTACK

	float fRotRad = D3DXToRadian(m_rkOwner.GetRotation());
	float fVictimRotRad = D3DXToRadian(rVictim.GetRotation());

	D3DXVECTOR2 v2Normal(sin(fRotRad), cos(fRotRad));
	D3DXVECTOR2 v2VictimNormal(sin(fVictimRotRad), cos(fVictimRotRad));

	D3DXVec2Normalize(&v2Normal, &v2Normal);
	D3DXVec2Normalize(&v2VictimNormal, &v2VictimNormal);

	float fScalar = D3DXVec2Dot(&v2Normal, &v2VictimNormal);

	rVictim.GetBattle().__Shake(100);

	if (rVictim.GetMotion().IsUsingSkill())
		return;

	if (rVictim.IsStun())
	{
		if (fScalar < 0.0f)
			rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING);
		else
		{
			if (!rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING_BACK))
				rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING);
		}

		rVictim.m_isRealDead = true;
	}
	else
	{
		if (fScalar < 0.0f)
		{
			if (rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING))
			{
				rVictim.GetMotion().PushOnceMotion(CRaceMotionData::NAME_STAND_UP);
				rVictim.GetMotion().PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
		}
		else
		{
			if (!rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING_BACK))
			{
				if (rVictim.GetMotion().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING))
				{
					rVictim.GetMotion().PushOnceMotion(CRaceMotionData::NAME_STAND_UP);
					rVictim.GetMotion().PushLoopMotion(CRaceMotionData::NAME_WAIT);
				}
			}
			else
			{
				rVictim.GetMotion().PushOnceMotion(CRaceMotionData::NAME_STAND_UP_BACK);
				rVictim.GetMotion().PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
		}
	}
}

void CActorInstanceBattle::SetBlendingPosition(const TPixelPosition& c_rPosition, float fBlendingTime)
{
	// return;
	TPixelPosition Position;

	Position.x = c_rPosition.x - m_rkOwner.m_x;
	Position.y = c_rPosition.y - m_rkOwner.m_y;
	Position.z = 0;

	m_rkOwner.m_PhysicsObject.SetLastPosition(Position, fBlendingTime);
}

void CActorInstanceBattle::ResetBlendingPosition()
{
	m_rkOwner.m_PhysicsObject.Initialize();
}

void CActorInstanceBattle::GetBlendingPosition(TPixelPosition* pPosition)
{
	if (m_rkOwner.m_PhysicsObject.isBlending())
	{
		m_rkOwner.m_PhysicsObject.GetLastPosition(pPosition);
		pPosition->x += m_rkOwner.m_x;
		pPosition->y += m_rkOwner.m_y;
		pPosition->z += m_rkOwner.m_z;
	}
	else
	{
		pPosition->x = m_rkOwner.m_x;
		pPosition->y = m_rkOwner.m_y;
		pPosition->z = m_rkOwner.m_z;
	}
}

void CActorInstanceBattle::__PushCircle(CActorInstance& rVictim)
{
	const TPixelPosition& c_rkPPosAtk = m_rkOwner.NEW_GetAtkPixelPositionRef();

	D3DXVECTOR3 v3SrcPos(c_rkPPosAtk.x, -c_rkPPosAtk.y, c_rkPPosAtk.z);

	const D3DXVECTOR3& c_rv3SrcPos = v3SrcPos;
	const D3DXVECTOR3& c_rv3DstPos = rVictim.GetPosition();

	D3DXVECTOR3 v3Direction;
	v3Direction.x = c_rv3DstPos.x - c_rv3SrcPos.x;
	v3Direction.y = c_rv3DstPos.y - c_rv3SrcPos.y;
	v3Direction.z = 0.0f;
	D3DXVec3Normalize(&v3Direction, &v3Direction);

	rVictim.GetBattle().__SetFallingDirection(v3Direction.x, v3Direction.y);
}

void CActorInstanceBattle::__PushDirect(CActorInstance& rVictim)
{
	D3DXVECTOR3 v3Direction;
	v3Direction.x = cosf(D3DXToRadian(m_rkOwner.m_fcurRotation + 270.0f));
	v3Direction.y = sinf(D3DXToRadian(m_rkOwner.m_fcurRotation + 270.0f));
	v3Direction.z = 0.0f;

	rVictim.GetBattle().__SetFallingDirection(v3Direction.x, v3Direction.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CActorInstanceBattle::__isInvisible()
{
	if (m_rkOwner.IsDead())
		return true;

	if (CTimer::Instance().GetCurrentSecond() >= m_rkOwner.m_fInvisibleTime)
		return false;

	return true;
}

void CActorInstanceBattle::__SetFallingDirection(float fx, float fy)
{
	m_rkOwner.m_PhysicsObject.SetDirection(D3DXVECTOR3(fx, fy, 0.0f));
}
