#pragma once

#include <Client/Game/GameType.h>
#include <Client/Map/MapType.h>

#include <cstdint>
#include <vector>

class CInstanceBase;
class IFlyEventHandler;

class CInstanceBaseBattle
{
	public:
		explicit CInstanceBaseBattle(CInstanceBase& rkOwner);

		void AddFlyTargetInstance(CInstanceBase& rkInstDst);
		void AddFlyTargetPosition(const TPixelPosition& c_rkPPosDst);
		int32_t CanAttackHorseLevel();
		int32_t CanCancelSkill();
		int32_t CheckAdvancing();
		int32_t CheckAttacking(CInstanceBase& rkInstVictim);
		void ClearFlyTargetInstance();
		void Die();
		float GetAttackingElapsedTime();
		void GetBlendingPosition(TPixelPosition* pPixelPosition);
		uint32_t GetComboIndex();
		float GetFlyTargetDistance();
		MOTION_KEY GetNormalAttackIndex();
		void Hide();
		void InputComboAttack(float fAtkDirRot);
		void InputNormalAttack(float fAtkDirRot);
		bool IsAffect(uint32_t uAffect);
		bool IsFlyTargetObject();
		int32_t IsUsingMovingSkill();
		int32_t IsUsingSkill();
		void NEW_Attack();
		void NEW_Attack(float fDirRot);
		bool NEW_AttackToDestInstanceDirection(CInstanceBase& rkInstDst, IFlyEventHandler* pkFlyHandler);
		bool NEW_AttackToDestInstanceDirection(CInstanceBase& rkInstDst);
		void NEW_AttackToDestPixelPositionDirection(const TPixelPosition& c_rkPPosDst);
		float NEW_GetDistanceFromDestInstance(CInstanceBase& rkInstDst);
		float NEW_GetDistanceFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		float NEW_GetDistanceFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		bool NEW_GetFrontInstance(CInstanceBase** ppoutTargetInstance, float fDistance);
		bool NEW_GetInstanceVectorInCircleRange(float fSkillDistance, std::vector<CInstanceBase*>* pkVct_pkInst);
		bool NEW_GetInstanceVectorInFanRange(float fSkillDistance, CInstanceBase& rkInstTarget, std::vector<CInstanceBase*>* pkVct_pkInst);
		void NEW_GetRandomPositionInFanRange(CInstanceBase& rkInstTarget, TPixelPosition* pkPPosDst);
		float NEW_GetRotation();
		float NEW_GetRotationFromDestInstance(CInstanceBase& rkInstDst);
		float NEW_GetRotationFromDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		float NEW_GetRotationFromDirPixelPosition(const TPixelPosition& c_rkPPosDir);
		int32_t NEW_IsClickableDistanceDestInstance(CInstanceBase& rkInstDst);
		int32_t NEW_IsClickableDistanceDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		bool NEW_UseSkill(uint32_t uSkill, uint32_t uMot, uint32_t uMotLoopCount, bool isMovingSkill);
		void ProcessHitting(uint32_t dwMotionKey, CInstanceBase* pVictimInstance);
		void ProcessHitting(uint32_t dwMotionKey, uint8_t byEventIndex, CInstanceBase* pVictimInstance);
		void Revive();
		void RunComboAttack(float fAtkDirRot, uint32_t wMotionIndex);
		void RunNormalAttack(float fAtkDirRot);
		void SetBlendingPosition(const TPixelPosition& c_rPixelPosition);
		void AttackProcess();
		void SetFlyTargetInstance(CInstanceBase& rkInstDst);
		void Show();
		void Stun();
		int32_t isComboAttacking();
		int32_t isNormalAttacking();

	private:
		CInstanceBase& m_rkOwner;
};
