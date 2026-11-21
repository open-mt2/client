#include <Client/UI/StdAfx.h>
#include <Client/UI/InstanceBase.h>
#include <Client/UI/InstanceBaseMovement.h>
#include <Client/UI/PythonBackground.h>
#include <Neoeter/Render/GrpMath.h>

CInstanceBaseMovement::CInstanceBaseMovement(CInstanceBase& rkOwner) : m_rkOwner(rkOwner)
{
}

void CInstanceBaseMovement::SetAttackSpeed(uint32_t uAtkSpd)
{
	if (uAtkSpd > 1100)
		uAtkSpd = 0;

	m_rkOwner.m_GraphicThingInstance.SetAttackSpeed(uAtkSpd / 100.0f);
	m_rkOwner.m_kHorse.SetAttackSpeed(uAtkSpd);
}

void CInstanceBaseMovement::SetMoveSpeed(uint32_t uMovSpd)
{
	if (uMovSpd > 1100)
		uMovSpd = 0;

	m_rkOwner.m_GraphicThingInstance.SetMoveSpeed(uMovSpd / 100.0f);
	m_rkOwner.m_kHorse.SetMoveSpeed(uMovSpd);
}

void CInstanceBaseMovement::SetRotationSpeed(float fRotSpd)
{
	m_rkOwner.m_fMaxRotSpd = fRotSpd;
}

void CInstanceBaseMovement::NEW_Stop()
{
	if (m_rkOwner.__IsSyncing())
		return;

	if (m_rkOwner.GetMotion().isLock())
		return;

	if (m_rkOwner.GetBattle().IsUsingSkill())
		return;

	if (!IsWaiting())
		EndWalking();

	m_rkOwner.m_GraphicThingInstance.__OnStop();
}

void CInstanceBaseMovement::NEW_SyncPixelPosition(long nPPosX, long nPPosY)
{
	m_rkOwner.m_GraphicThingInstance.TEMP_Push(nPPosX, nPPosY);
}

bool CInstanceBaseMovement::NEW_CanMoveToDestPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	TPixelPosition kPPosCur;
	m_rkOwner.GetTransform().NEW_GetPixelPosition(&kPPosCur);

	if (kPPosCur.x == c_rkPPosDst.x && kPPosCur.y == c_rkPPosDst.y)
		return false;

	return true;
}

float CInstanceBase_GetDegreeFromPosition(float x, float y)
{
	D3DXVECTOR3 vtDir(floor(x), floor(y), 0.0f);
	D3DXVec3Normalize(&vtDir, &vtDir);

	D3DXVECTOR3 vtStan(0, -1, 0);
	float ret = D3DXToDegree(acosf(D3DXVec3Dot(&vtDir, &vtStan)));

	if (vtDir.x < 0.0f)
		ret = 360.0f - ret;

	return ret;
}

float CInstanceBaseMovement::NEW_GetAdvancingRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir)
{
	float fDirRot = CInstanceBase_GetDegreeFromPosition(c_rkPPosDir.x, -c_rkPPosDir.y);
	float fClampDirRot = ClampDegree(fDirRot);

	return fClampDirRot;
}

float CInstanceBaseMovement::NEW_GetAdvancingRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	TPixelPosition kPPosCur;
	m_rkOwner.GetTransform().NEW_GetPixelPosition(&kPPosCur);
	return NEW_GetAdvancingRotationFromPixelPosition(kPPosCur, c_rkPPosDst);
}

float CInstanceBaseMovement::NEW_GetAdvancingRotationFromPixelPosition(const TPixelPosition& c_rkPPosSrc, const TPixelPosition& c_rkPPosDst)
{
	TPixelPosition kPPosDelta;
	kPPosDelta = c_rkPPosDst - c_rkPPosSrc;
	return NEW_GetAdvancingRotationFromDirPixelPosition(kPPosDelta);
}

void CInstanceBaseMovement::NEW_SetAdvancingRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir)
{
	float fClampDirRot = NEW_GetAdvancingRotationFromDirPixelPosition(c_rkPPosDir);
	m_rkOwner.m_GraphicThingInstance.SetAdvancingRotation(fClampDirRot);

	float fCurRot = m_rkOwner.m_GraphicThingInstance.GetRotation();
	float fAdvRot = m_rkOwner.m_GraphicThingInstance.GetAdvancingRotation();

	m_rkOwner.m_iRotatingDirection = GetRotatingDirection(fCurRot, fAdvRot);
}

void CInstanceBaseMovement::NEW_SetAdvancingRotationFromPixelPosition(const TPixelPosition& c_rkPPosSrc, const TPixelPosition& c_rkPPosDst)
{
	TPixelPosition kPPosDelta;
	kPPosDelta = c_rkPPosDst - c_rkPPosSrc;

	NEW_SetAdvancingRotationFromDirPixelPosition(kPPosDelta);
}

bool CInstanceBaseMovement::NEW_SetAdvancingRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	if (!NEW_CanMoveToDestPixelPosition(c_rkPPosDst))
	{
		Tracenf("Failed to move next position (%f,%f, %f)", c_rkPPosDst.x, c_rkPPosDst.y, c_rkPPosDst.z);
		return false;
	}

	TPixelPosition kPPosSrc;
	m_rkOwner.GetTransform().NEW_GetPixelPosition(&kPPosSrc);
	NEW_SetAdvancingRotationFromPixelPosition(kPPosSrc, c_rkPPosDst);
	return true;
}

void CInstanceBaseMovement::SetAdvancingRotation(float fRotation)
{
	float frotDifference = GetDegreeDifference(m_rkOwner.GetTransform().GetRotation(), fRotation);

	if (frotDifference > 45.0f)
		m_rkOwner.m_fRotSpd = m_rkOwner.m_fMaxRotSpd;
	else
		m_rkOwner.m_fRotSpd = m_rkOwner.m_fMaxRotSpd * 5 / 12;

	m_rkOwner.m_GraphicThingInstance.SetAdvancingRotation(ClampDegree(fRotation));
	m_rkOwner.m_iRotatingDirection = GetRotatingDirection(m_rkOwner.m_GraphicThingInstance.GetRotation(),
												m_rkOwner.m_GraphicThingInstance.GetAdvancingRotation());
}

void CInstanceBaseMovement::StartWalking()
{
	m_rkOwner.m_GraphicThingInstance.Move();

	if (m_rkOwner.GetBattle().IsAffect(CInstanceBase::AFFECT_GYEONGGONG))
	{
		m_rkOwner.m_adwCRCAffectEffect[CInstanceBase::AFFECT_GYEONGGONG] = m_rkOwner.GetEffect().__EffectContainer_AttachEffect(CInstanceBase::EFFECT_AFFECT_GYEONGGONG);
	}
	else if (m_rkOwner.GetBattle().IsAffect(CInstanceBase::AFFECT_KWAESOK))
	{
		m_rkOwner.m_adwCRCAffectEffect[CInstanceBase::AFFECT_KWAESOK] = m_rkOwner.GetEffect().__EffectContainer_AttachEffect(CInstanceBase::EFFECT_AFFECT_KWAESOK);
	}
}

void CInstanceBaseMovement::EndWalking(float fBlendingTime)
{
	assert(!IsWaiting() && "CInstanceBaseMovement::EndWalking");

	m_rkOwner.m_isGoing = FALSE;

	// 걷고 있을때는 무조건 멈추게 해야 한다
	if (IsWalking() || !IsAttacked())
	{
		m_rkOwner.m_GraphicThingInstance.Stop(fBlendingTime);
		if (m_rkOwner.GetBattle().IsAffect(CInstanceBase::AFFECT_GYEONGGONG))
		{
			m_rkOwner.GetEffect().__EffectContainer_DetachEffect(CInstanceBase::EFFECT_AFFECT_GYEONGGONG);
		}
		else if (m_rkOwner.GetBattle().IsAffect(CInstanceBase::AFFECT_KWAESOK))
		{
			m_rkOwner.GetEffect().__EffectContainer_DetachEffect(CInstanceBase::EFFECT_AFFECT_KWAESOK);
		}
	}
}

void CInstanceBaseMovement::EndWalkingWithoutBlending()
{
	EndWalking(0.0f);
}

int32_t CInstanceBaseMovement::IsWaiting()
{
	return m_rkOwner.m_GraphicThingInstance.IsWaiting();
}

int32_t CInstanceBaseMovement::IsWalking()
{
	return m_rkOwner.m_GraphicThingInstance.IsMoving();
}

int32_t CInstanceBaseMovement::IsPushing()
{
	return m_rkOwner.m_GraphicThingInstance.IsPushing();
}

int32_t CInstanceBaseMovement::IsAttacked()
{
	return m_rkOwner.m_GraphicThingInstance.IsAttacked();
}

int32_t CInstanceBaseMovement::IsKnockDown()
{
	if (!m_rkOwner.m_GraphicThingInstance.IsKnockDown())
		return FALSE;

	return TRUE;
}

int32_t CInstanceBaseMovement::IsAttacking()
{
	return m_rkOwner.m_GraphicThingInstance.isAttacking();
}

int32_t CInstanceBaseMovement::IsActingEmotion()
{
	return m_rkOwner.m_GraphicThingInstance.IsActEmotion();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int32_t CInstanceBaseMovement::IsGoing()
{
	return m_rkOwner.m_isGoing;
}

void CInstanceBaseMovement::NEW_MoveToDestInstanceDirection(CInstanceBase& rkInstDst)
{
	TPixelPosition kPPosDst;
	rkInstDst.GetTransform().NEW_GetPixelPosition(&kPPosDst);

	NEW_MoveToDestPixelPositionDirection(kPPosDst);
}

bool CInstanceBaseMovement::NEW_MoveToDestPixelPositionDirection(const TPixelPosition& c_rkPPosDst)
{
	TPixelPosition kPPosCur;
	m_rkOwner.GetTransform().NEW_GetPixelPosition(&kPPosCur);
	float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(kPPosCur, c_rkPPosDst);

	return NEW_Goto(c_rkPPosDst, fDstRot);
}

bool CInstanceBaseMovement::NEW_Goto(const TPixelPosition& c_rkPPosDst, float fDstRot)
{
	if (m_rkOwner.__IsSyncing())
	{
		return false;
	}

	if (m_rkOwner.m_GraphicThingInstance.IsUsingMovingSkill())
	{
		SetAdvancingRotation(fDstRot);
		return true;
	}

	if (m_rkOwner.GetMotion().isLock())
	{
		return false;
	}

	if (!NEW_CanMoveToDestPixelPosition(c_rkPPosDst))
	{
		if (!IsWaiting())
			EndWalking();

		return true;
	}

	m_rkOwner.NEW_SetSrcPixelPosition(m_rkOwner.NEW_GetCurPixelPositionRef());
	m_rkOwner.NEW_SetDstPixelPosition(c_rkPPosDst);
	m_rkOwner.NEW_SetDstPixelPositionZ(m_rkOwner.NEW_GetSrcPixelPositionRef().z);
	m_rkOwner.m_fDstRot = fDstRot;
	m_rkOwner.m_isGoing = TRUE;

	if (!IsWalking())
	{
		StartWalking();
	}

	NEW_SetAdvancingRotationFromPixelPosition(m_rkOwner.NEW_GetSrcPixelPositionRef(), m_rkOwner.NEW_GetDstPixelPositionRef());

	return true;
}

void CInstanceBaseMovement::NEW_MoveToDirection(float fDirRot)
{
	if (m_rkOwner.__IsSyncing())
		return;

	if (m_rkOwner.m_GraphicThingInstance.IsUsingMovingSkill())
	{
		SetAdvancingRotation(fDirRot);
		return;
	}

	if (m_rkOwner.GetMotion().isLock())
		return;

	m_rkOwner.m_isGoing = FALSE;

	SetAdvancingRotation(fDirRot);

	if (!IsWalking())
	{
		StartWalking();
	}

	TPixelPosition kPPosCur;
	m_rkOwner.GetTransform().NEW_GetPixelPosition(&kPPosCur);

	D3DXVECTOR3 kD3DVt3Cur(kPPosCur.x, -kPPosCur.y, kPPosCur.z);
	D3DXVECTOR3 kD3DVt3Dst;

	D3DXVECTOR3 kD3DVt3AdvDir(0.0f, -1.0f, 0.0f);
	D3DXMATRIX kD3DMatAdv;
	D3DXMatrixRotationZ(&kD3DMatAdv, D3DXToRadian(fDirRot));
	D3DXVec3TransformCoord(&kD3DVt3AdvDir, &kD3DVt3AdvDir, &kD3DMatAdv);
	D3DXVec3Scale(&kD3DVt3AdvDir, &kD3DVt3AdvDir, 300.0f);
	D3DXVec3Add(&kD3DVt3Dst, &kD3DVt3AdvDir, &kD3DVt3Cur);

	TPixelPosition kPPosDst;
	kPPosDst.x = +kD3DVt3Dst.x;
	kPPosDst.y = -kD3DVt3Dst.y;
	kPPosDst.z = +kD3DVt3Dst.z;

	m_rkOwner.NEW_SetSrcPixelPosition(kPPosCur);
	m_rkOwner.NEW_SetDstPixelPosition(kPPosDst);
}

void CInstanceBaseMovement::EndGoing()
{
	if (!IsWaiting())
		EndWalking();

	// Tracen("EndGoing");

	m_rkOwner.m_isGoing = FALSE;
}

void CInstanceBaseMovement::SetRunMode()
{
	m_rkOwner.m_GraphicThingInstance.SetRunMode();
}

void CInstanceBaseMovement::SetWalkMode()
{
	m_rkOwner.m_GraphicThingInstance.SetWalkMode();
}
