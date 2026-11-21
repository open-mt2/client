#pragma once

#include <Client/Map/MapType.h>

class CInstanceBase;

class CInstanceBaseMovement
{
	public:
		explicit CInstanceBaseMovement(CInstanceBase& rkOwner);

		void EndGoing();
		void EndWalking(float fBlendingTime = 0.15f);
		void EndWalkingWithoutBlending();

		int32_t IsActingEmotion();
		int32_t IsAttacked();
		int32_t IsAttacking();
		int32_t IsGoing();
		int32_t IsKnockDown();
		int32_t IsPushing();
		int32_t IsWaiting();
		int32_t IsWalking();

		bool NEW_CanMoveToDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		float NEW_GetAdvancingRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		float NEW_GetAdvancingRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		float NEW_GetAdvancingRotationFromPixelPosition(const TPixelPosition& c_rkPPosSrc, const TPixelPosition& c_rkPPosDst);
		bool NEW_Goto(const TPixelPosition& c_rkPPosDst, float fDstRot);
		void NEW_MoveToDestInstanceDirection(CInstanceBase& rkInstDst);
		bool NEW_MoveToDestPixelPositionDirection(const TPixelPosition& c_rkPPosDst);
		void NEW_MoveToDirection(float fDirRot);
		bool NEW_SetAdvancingRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		void NEW_SetAdvancingRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		void NEW_SetAdvancingRotationFromPixelPosition(const TPixelPosition& c_rkPPosSrc, const TPixelPosition& c_rkPPosDst);
		void NEW_Stop();
		void NEW_SyncPixelPosition(long nPPosX, long nPPosY);
		void SetAdvancingRotation(float fRotation);
		void SetAttackSpeed(uint32_t uAtkSpd);
		void SetMoveSpeed(uint32_t uMovSpd);
		void SetRotationSpeed(float fRotSpd);
		void SetRunMode();
		void SetWalkMode();
		void StartWalking();

	private:
		CInstanceBase& m_rkOwner;
};
