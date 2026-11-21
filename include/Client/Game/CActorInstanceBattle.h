#pragma once

#include <Neoeter/Model/Grn/ThingInstance.h>
#include <Client/Map/MapType.h>

#include <cstdint>
#include <Client/Game/RaceData.h>

class CActorInstance;

class CActorInstanceBattle
{
	public:
		explicit CActorInstanceBattle(CActorInstance& rkOwner);

		bool CanAct();
		bool CanAttack();
		int32_t CanCheckAttacking();
		bool CanFishing();
		bool CanMove();
		bool CanUseSkill();
		int32_t ComboAttack(uint32_t wMotionIndex, float fDirRot, float fBlendTime = 0.1f);
		void ComboProcess();
		float GetAttackingElapsedTime();
		void GetBlendingPosition(TPixelPosition* pPosition);
		uint32_t GetComboIndex();
		bool InputComboAttackCommand(float fDirRot);  // Process input - Only used by player's character
		void InputNormalAttackCommand(float fDirRot); // Process input - Only used by player's character
		int32_t IsActEmotion();
		int32_t IsClickableDistanceDestInstance(CActorInstance& rkInstDst, float fDistance);
		int32_t IsSplashAttacking();
		int32_t IsUsingMovingSkill();
		int32_t NormalAttack(float fDirRot, float fBlendTime = 0.1f);
		void ResetBlendingPosition();
		void SetBattleAttachEffect(uint32_t dwID);
		void SetBattleHitEffect(uint32_t dwID);
		void SetBlendingPosition(const TPixelPosition& c_rPosition, float fBlendingTime = 1.0f);
		void ShakeProcess();
		bool __CanInputNormalAttackCommand();
		bool __CanPushDestActor(CActorInstance& rkActorDst);
		void __ClearCombo();
		float __GetOwnerTime();
		uint32_t __GetOwnerVID();
		void __HitGood(CActorInstance& rVictim);
		void __HitGreate(CActorInstance& rVictim);
		void __HitStone(CActorInstance& rVictim);
		bool __IsInSplashTime();
		int32_t __IsMovingSkill(uint16_t wSkillNumber);
		void __OnEndCombo();
		void __ProcessDataAttackSuccess(const NRaceData::TAttackData& c_rAttackData, CActorInstance& rVictim, const D3DXVECTOR3& c_rv3Position, uint32_t uiSkill = 0, int32_t isSendPacket = TRUE);
		void __ProcessMotionAttackSuccess(uint32_t dwMotionKey, CActorInstance& rVictim);
		void __ProcessMotionEventAttackSuccess(uint32_t dwMotionKey, uint8_t byEventIndex, CActorInstance& rVictim);
		void __PushCircle(CActorInstance& rVictim);
		void __PushDirect(CActorInstance& rVictim);
		void __RunNextCombo();
		void __SetFallingDirection(float fx, float fy);
		void __Shake(uint32_t dwDuration);
		bool __isInvisible();
		int32_t isAttacking();
		int32_t isComboAttacking();
		int32_t isNormalAttacking();
		int32_t isValidAttacking();

	private:
		CActorInstance& m_rkOwner;
};
