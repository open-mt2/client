#include <Client/UI/StdAfx.h>
#include <Client/UI/InstanceBase.h>
#include <Client/UI/InstanceBaseMotion.h>
#include <Client/UI/AbstractPlayer.h>
#include <Client/Game/ActorInstance.h>

namespace
{
	const int c_iFishingRotStep = 8;
	const float c_fFishingDistance = 600.0f;
}

CInstanceBaseMotion::CInstanceBaseMotion(CInstanceBase& rkOwner) : m_rkOwner(rkOwner)
{
}

void CInstanceBaseMotion::SetMotionMode(int iMotionMode)
{
	m_rkOwner.m_GraphicThingInstance.SetMotionMode(iMotionMode);
}

int CInstanceBaseMotion::GetMotionMode(uint32_t dwMotionIndex)
{
	return m_rkOwner.m_GraphicThingInstance.GetMotionMode();
}

void CInstanceBaseMotion::SetLoopMotion(uint16_t wMotion, float fBlendTime, float fSpeedRatio)
{
	m_rkOwner.m_GraphicThingInstance.SetLoopMotion(wMotion, fBlendTime, fSpeedRatio);
}

void CInstanceBaseMotion::PushOnceMotion(uint16_t wMotion, float fBlendTime, float fSpeedRatio)
{
	m_rkOwner.m_GraphicThingInstance.PushOnceMotion(wMotion, fBlendTime, fSpeedRatio);
}

void CInstanceBaseMotion::PushLoopMotion(uint16_t wMotion, float fBlendTime, float fSpeedRatio)
{
	m_rkOwner.m_GraphicThingInstance.PushLoopMotion(wMotion, fBlendTime, fSpeedRatio);
}

void CInstanceBaseMotion::ResetLocalTime()
{
	m_rkOwner.m_GraphicThingInstance.ResetLocalTime();
}

void CInstanceBaseMotion::SetEndStopMotion()
{
	m_rkOwner.m_GraphicThingInstance.SetEndStopMotion();
}

int32_t CInstanceBaseMotion::isLock()
{
	return m_rkOwner.m_GraphicThingInstance.isLock();
}

void CInstanceBaseMotion::StartFishing(float frot)
{
	m_rkOwner.m_GraphicThingInstance.BlendRotation(frot, 0.1f);

	const TPixelPosition& c_rkPPosCur = m_rkOwner.m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	float fRot = m_rkOwner.m_GraphicThingInstance.GetTargetRotation();

	TPixelPosition kPPosFishing;
	ELPlainCoord_GetRotatedPixelPosition(c_rkPPosCur.x, c_rkPPosCur.y, c_fFishingDistance, fRot, &kPPosFishing.x, &kPPosFishing.y);
	if (!m_rkOwner.__Background_GetWaterHeight(kPPosFishing, &kPPosFishing.z))
		kPPosFishing.z = c_rkPPosCur.z;

	D3DXVECTOR3 v3Fishing;
	PixelPositionToD3DXVECTOR3(kPPosFishing, &v3Fishing);
	m_rkOwner.m_GraphicThingInstance.SetFishingPosition(v3Fishing);

	PushOnceMotion(CRaceMotionData::NAME_FISHING_THROW);
	PushLoopMotion(CRaceMotionData::NAME_FISHING_WAIT);
}

void CInstanceBaseMotion::StopFishing()
{
	m_rkOwner.m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_FISHING_STOP);
	PushLoopMotion(CRaceMotionData::NAME_WAIT);
}

void CInstanceBaseMotion::ReactFishing()
{
	PushOnceMotion(CRaceMotionData::NAME_FISHING_REACT);
	PushLoopMotion(CRaceMotionData::NAME_FISHING_WAIT);
}

void CInstanceBaseMotion::CatchSuccess()
{
	m_rkOwner.m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_FISHING_CATCH);
	PushLoopMotion(CRaceMotionData::NAME_WAIT);
}

void CInstanceBaseMotion::CatchFail()
{
	m_rkOwner.m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_FISHING_FAIL);
	PushLoopMotion(CRaceMotionData::NAME_WAIT);
}

int32_t CInstanceBaseMotion::GetFishingRot(int* pirot)
{
	const TPixelPosition& c_rkPPosCur = m_rkOwner.m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	float fCharacterRot = m_rkOwner.m_GraphicThingInstance.GetRotation();

	for (float fRot = 0.0f; fRot <= 180.0f; fRot += 10.0f)
	{
		TPixelPosition kPPosFishingRight;
		ELPlainCoord_GetRotatedPixelPosition(c_rkPPosCur.x, c_rkPPosCur.y, c_fFishingDistance, fCharacterRot + fRot, &kPPosFishingRight.x, &kPPosFishingRight.y);
		if (m_rkOwner.__Background_IsWaterPixelPosition(kPPosFishingRight))
		{
			*pirot = fCharacterRot + fRot;
			return TRUE;
		}

		TPixelPosition kPPosFishingLeft;
		ELPlainCoord_GetRotatedPixelPosition(c_rkPPosCur.x, c_rkPPosCur.y, c_fFishingDistance, fCharacterRot - fRot, &kPPosFishingLeft.x, &kPPosFishingLeft.y);
		if (m_rkOwner.__Background_IsWaterPixelPosition(kPPosFishingLeft))
		{
			*pirot = fCharacterRot - fRot;
			return TRUE;
		}
	}

	return FALSE;
}

void CInstanceBaseMotion::EnableChangingTCPState()
{
	m_rkOwner.m_bEnableTCPState = TRUE;
}

void CInstanceBaseMotion::DisableChangingTCPState()
{
	m_rkOwner.m_bEnableTCPState = FALSE;
}

void CInstanceBaseMotion::ActDualEmotion(CInstanceBase& rkDstInst, uint16_t wMotionNumber1, uint16_t wMotionNumber2)
{
	if (!m_rkOwner.IsWaiting())
	{
		m_rkOwner.m_GraphicThingInstance.SetLoopMotion(CRaceMotionData::NAME_WAIT, 0.05f);
	}
	if (!rkDstInst.IsWaiting())
	{
		rkDstInst.m_GraphicThingInstance.SetLoopMotion(CRaceMotionData::NAME_WAIT, 0.05f);
	}

	const float c_fEmotionDistance = 100.0f;
	const TPixelPosition& c_rMainPosition = m_rkOwner.NEW_GetCurPixelPositionRef();
	const TPixelPosition& c_rTargetPosition = rkDstInst.NEW_GetCurPixelPositionRef();
	TPixelPosition kDirection = c_rMainPosition - c_rTargetPosition;
	float fDistance = sqrtf((kDirection.x * kDirection.x) + (kDirection.y * kDirection.y));
	TPixelPosition kDstPosition;
	kDstPosition.x = c_rTargetPosition.x + (kDirection.x / fDistance) * c_fEmotionDistance;
	kDstPosition.y = c_rTargetPosition.y + (kDirection.y / fDistance) * c_fEmotionDistance;

	uint32_t dwCurTime = ELTimer_GetServerMSec() + 500;
	m_rkOwner.PushTCPStateExpanded(dwCurTime, kDstPosition, 0.0f, CInstanceBase::FUNC_EMOTION, MAKELONG(wMotionNumber1, wMotionNumber2), rkDstInst.GetVirtualID());

	DisableChangingTCPState();
	rkDstInst.GetMotion().DisableChangingTCPState();

	if (m_rkOwner.__IsMainInstance() || rkDstInst.__IsMainInstance())
	{
		IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
		rPlayer.StartEmotionProcess();
	}
}

void CInstanceBaseMotion::ActEmotion(uint32_t dwMotionNumber)
{
	PushOnceMotion(dwMotionNumber);
}
