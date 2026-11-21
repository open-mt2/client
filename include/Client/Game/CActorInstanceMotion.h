#pragma once

#include <Neoeter/Model/Grn/ThingInstance.h>
#include <Client/Game/GameType.h>
#include <Client/Map/MapType.h>

#include <cstdint>

class CActorInstance;

enum EMotionPushType
{
	MOTION_TYPE_NONE,
	MOTION_TYPE_ONCE,
	MOTION_TYPE_LOOP,
};

struct SSetMotionData
{
		MOTION_KEY dwMotKey;
		float fSpeedRatio;
		float fBlendTime;
		int iLoopCount;
		uint32_t uSkill;

		SSetMotionData()
		{
			iLoopCount = 0;
			dwMotKey = 0;
			fSpeedRatio = 1.0f;
			fBlendTime = 0.0f;
			uSkill = 0;
		}
};

class CActorInstanceMotion
{
	public:
		explicit CActorInstanceMotion(CActorInstance& rkOwner);

		int32_t CanCancelSkill();
		void CurrentMotionProcess();
		float GetLastMotionTime(float fBlendTime); // NOTE : 자동으로 BlendTime만큼을 앞당긴 시간을 리턴
		float GetMotionDuration(uint32_t dwMotionKey);
		int GetMotionMode();
		MOTION_KEY GetRandomMotionKey(MOTION_KEY dwMotionKey);
		void HORSE_MotionProcess(int32_t isPC);
		bool InterceptLoopMotion(uint32_t dwMotion, float fBlendTime = 0.1f);
		bool InterceptOnceMotion(uint32_t dwMotion, float fBlendTime = 0.1f, uint32_t uSkill = 0, float fSpeedRatio = 1.0f);
		int32_t IsFishing();
		int32_t IsUsingSkill();
		void MotionProcess(int32_t isPC);
		void PreAttack();
		bool PushLoopMotion(uint32_t dwMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f); // FIXME : 모드의 시간차 적용이 가능하게끔 한다.
		bool PushOnceMotion(uint32_t dwMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f); // FIXME : 모드의 시간차 적용이 가능하게끔 한다.
		void ReservingMotionProcess();
		void SetLoopMotion(uint32_t dwMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f);
		void SetMotionLoopCount(int iCount);
		void PushMotion(EMotionPushType iMotionType, uint32_t dwMotionKey, float fBlendTime, float fSpeedRatio = 1.0f);
		bool InterceptMotion(EMotionPushType iMotionType, uint16_t wMotion, float fBlendTime = 0.1f, uint32_t uSkill = 0, float fSpeedRatio = 1.0f);
		uint32_t __SetMotion(const SSetMotionData& c_rkSetMotData, uint32_t dwRandMotKey = 0);
		void SetMotionMode(int iMotionMode); // FIXME : 모드의 시간차 적용이 가능하게끔 한다.
		bool __BindMotionData(uint32_t dwMotionKey); // 모션 데이터를 바인딩
		bool __CanAttack();			 // 공격 할수 있는가?
		bool __CanNextComboAttack(); // 다음 콤보 어택이 가능한가?
		void __ClearMotion();
		uint16_t __GetCurrentMotionIndex();
		uint32_t __GetCurrentMotionKey();
		int __GetLoopCount();
		uint32_t __GetMotionType(); // 모션 타입 얻기
		bool __HasMotionFlyEvent();		// 무언가를 쏘는가?
		bool __IsAttackMotion();		// 공격 모션 인가?
		bool __IsComboAttackMotion();	// 콤보 공격 모션 인가?
		bool __IsComboAttacking();	// 콤보 공격중인가?
		bool __IsDamageMotion();		// 데미지 모션인가?
		bool __IsDieMotion();			// 사망 모션 인가?
		bool __IsKnockDownMotion();		// 넉다운 모션인가?
		bool __IsMoveMotion();			// 이동 모션 인가?
		bool __IsNeedFlyTargetMotion(); // FlyTarget 이 필요한 모션인가?
		bool __IsStandUpMotion();		// 일어서기 모션인가?
		bool __IsWaitMotion();			// 대기 모션 인가?
		void __MotionEventProcess(int32_t isPC);
		int32_t isLock();

	private:
		CActorInstance& m_rkOwner;
};
