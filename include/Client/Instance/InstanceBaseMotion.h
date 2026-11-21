#pragma once

#include <Client/Map/MapType.h>

class CInstanceBase;

class CInstanceBaseMotion
{
	public:
		explicit CInstanceBaseMotion(CInstanceBase& rkOwner);

		void SetMotionMode(int iMotionMode);
		int GetMotionMode(uint32_t dwMotionIndex);

		void SetLoopMotion(uint16_t wMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f);
		void PushOnceMotion(uint16_t wMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f);
		void PushLoopMotion(uint16_t wMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f);

		void ResetLocalTime();
		void SetEndStopMotion();
		int32_t isLock();

		void StartFishing(float frot);
		void StopFishing();
		void ReactFishing();
		void CatchSuccess();
		void CatchFail();
		int32_t GetFishingRot(int* pirot);

		void EnableChangingTCPState();
		void DisableChangingTCPState();

		void ActDualEmotion(CInstanceBase& rkDstInst, uint16_t wMotionNumber1, uint16_t wMotionNumber2);
		void ActEmotion(uint32_t dwMotionNumber);

	private:
		CInstanceBase& m_rkOwner;
};
