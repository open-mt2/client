#pragma once

// class CActorInstance

//		Note : 캐릭터의 Lighting, Local Point Light, Weapon Trace 등의 효과와 콤보 데이타, 사운드 데이타,
//             모션 데이타 들을 추상적, 총체적으로 관리하는 개별 매니저가 될 것이다.

#include <Client/Game/FlyTarget.h>
#include <Client/Game/RaceData.h>
#include <Client/Game/RaceMotionData.h>
#include <Client/Map/PhysicsObject.h>
#include <Client/Game/ActorInstanceInterface.h>
#include <Client/Game/Interface.h>
#include <Client/Game/CActorInstanceMotion.h>
#include <Client/Game/CActorInstanceBattle.h>
#include <Client/Game/CActorInstanceAttach.h>
#include <Client/Game/CActorInstanceRender.h>
#include <Client/Game/CActorInstanceCollisionDetection.h>
// #include <Neoeter/Model/Grn/ThingInstance.h>

class CItemData;
class CWeaponTrace;
class IFlyEventHandler;
class CSpeedTreeWrapper;

class IMobProto : public CSingleton<IMobProto>
{
	public:
		IMobProto() {}
		virtual ~IMobProto() {}

		virtual bool FindRaceType(uint32_t eRace, uint32_t* puType);
};

class CActorInstance : public IActorInstance, public IFlyTargetableObject
{
	public:
		class IEventHandler
		{
			public:
				static IEventHandler* GetEmptyPtr();

			public:
				struct SState
				{
						TPixelPosition kPPosSelf;
						float fAdvRotSelf;
				};

			public:
				IEventHandler() {}
				virtual ~IEventHandler() {}

				virtual void OnSyncing(const SState& c_rkState) = 0;
				virtual void OnWaiting(const SState& c_rkState) = 0;
				virtual void OnMoving(const SState& c_rkState) = 0;
				virtual void OnMove(const SState& c_rkState) = 0;
				virtual void OnStop(const SState& c_rkState) = 0;
				virtual void OnWarp(const SState& c_rkState) = 0;
				virtual void OnSetAffect(uint32_t uAffect) = 0;
				virtual void OnResetAffect(uint32_t uAffect) = 0;
				virtual void OnClearAffects() = 0;

				virtual void OnAttack(const SState& c_rkState, uint16_t wMotionIndex) = 0;
				virtual void OnUseSkill(const SState& c_rkState, uint32_t uMotSkill, uint32_t uMotLoopCount) = 0;

				virtual void OnHit(uint32_t uSkill, CActorInstance& rkActorVictim, int32_t isSendPacket) = 0;

				virtual void OnChangeShape() = 0;
		};

		// 2004.07.05.myevan.궁신탄영 맵에 끼이는 문제해결
	private:
		static IBackground& GetBackground();

	public:
	public:
		enum EType
		{
			TYPE_ENEMY,
			TYPE_NPC,
			TYPE_STONE,
			TYPE_WARP,
			TYPE_DOOR,
			TYPE_BUILDING,
			TYPE_PC,
			TYPE_POLY,
			TYPE_HORSE,
			TYPE_GOTO,

			TYPE_OBJECT, // Only For Client
		};

		enum ERenderMode
		{
			RENDER_MODE_NORMAL,
			RENDER_MODE_BLEND,
			RENDER_MODE_ADD,
			RENDER_MODE_MODULATE,
		};

		/////////////////////////////////////////////////////////////////////////////////////
		typedef struct SReservingMotionNode
		{
				EMotionPushType iMotionType;

				float fStartTime;
				float fBlendTime;
				float fDuration;
				float fSpeedRatio;

				uint32_t dwMotionKey;
		} TReservingMotionNode;

		struct SCurrentMotionNode
		{
				EMotionPushType iMotionType;
				uint32_t dwMotionKey;

				uint32_t dwcurFrame;
				uint32_t dwFrameCount;

				float fStartTime;
				float fEndTime;
				float fSpeedRatio;

				int iLoopCount;
				uint32_t uSkill;
		};

		typedef std::deque<TReservingMotionNode> TMotionDeque;
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// Motion Event
		typedef struct SMotionEventInstance
		{
				int iType;
				int iMotionEventIndex;
				float fStartingTime;

				const CRaceMotionData::TMotionEventData* c_pMotionData;
		} TMotionEventInstance;

		typedef std::list<TMotionEventInstance> TMotionEventInstanceList;
		typedef TMotionEventInstanceList::iterator TMotionEventInstanceListIterator;
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// For Collision Detection
		typedef struct SCollisionPointInstance
		{
				const NRaceData::TCollisionData* c_pCollisionData;
				int32_t isAttached;
				uint32_t dwModelIndex;
				uint32_t dwBoneIndex;
				CDynamicSphereInstanceVector SphereInstanceVector;
		} TCollisionPointInstance;
		typedef std::list<TCollisionPointInstance> TCollisionPointInstanceList;
		typedef TCollisionPointInstanceList::iterator TCollisionPointInstanceListIterator;

		typedef std::map<CActorInstance*, float> THittedInstanceMap;
		typedef std::map<const NRaceData::THitData*, THittedInstanceMap> THitDataMap;
		struct SSplashArea
		{
				int32_t isEnableHitProcess;
				uint32_t uSkill;
				MOTION_KEY MotionKey;
				float fDisappearingTime;
				const CRaceMotionData::TMotionAttackingEventData* c_pAttackingEvent;
				CDynamicSphereInstanceVector SphereInstanceVector;

				THittedInstanceMap HittedInstanceMap;
		};

		typedef struct SHittingData
		{
				uint8_t byAttackingType;
				uint32_t dwMotionKey;
				uint8_t byEventIndex;
		} THittingData;
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// For Attaching
		enum EAttachEffect
		{
			EFFECT_LIFE_NORMAL,
			EFFECT_LIFE_INFINITE,
			EFFECT_LIFE_WITH_MOTION,
		};

		struct TAttachingEffect
		{
				uint32_t dwEffectIndex;
				int iBoneIndex;
				uint32_t dwModelIndex;
				D3DXMATRIX matTranslation;
				D3DXMATRIX matAttachTransform;
				int32_t isAttaching;
				int32_t bUseAttachTransform;

				int iLifeType;
				uint32_t dwEndTime;
		};
		/////////////////////////////////////////////////////////////////////////////////////

	public:
		static void DestroySystem();

	public:
		CActorInstance();
		virtual ~CActorInstance();

		CActorInstanceMotion& GetMotion();
		CActorInstanceBattle& GetBattle();
		CActorInstanceAttach& GetAttach();
		CActorInstanceRender& GetRender();
		CActorInstanceCollisionDetection& GetCollisionDetection();

		virtual void OnShootDamage();
		virtual bool TestCollisionWithDynamicSphere(const CDynamicSphereInstance& dsi);
		bool CreateCollisionInstancePiece(uint32_t dwAttachingModelIndex, const NRaceData::TAttachingData* c_pAttachingData, TCollisionPointInstance* pPointInstance);
		void UpdatePointInstance(TCollisionPointInstance* pPointInstance);

		// 20041201.myevan.인스턴스베이스용 함수
		void INSTANCEBASE_Transform();
		void INSTANCEBASE_Deform();

		void Destroy();

		void Move();
		void Stop(float fBlendingTime = 0.15f);

		void SetMainInstance();

		void SetParalysis(bool isParalysis);
		void SetFaint(bool isFaint);
		void SetSleep(bool isSleep);
		void SetResistFallen(bool isResistFallen);

		void SetAttackSpeed(float fAtkSpd);
		void SetMoveSpeed(float fMovSpd);

		void SetEventHandler(IEventHandler* pkEventHandler);

		bool SetRace(uint32_t eRace);
		void SetHair(uint32_t eHair);
		void SetVirtualID(uint32_t dwVID);

		void SetShape(uint32_t eShape, float fSpecular = 0.0f);
		void ChangeMaterial(const char* c_szFileName);

	public:
		void SetComboType(uint16_t wComboType);

		uint32_t GetRace();
		uint32_t GetVirtualID();

		uint32_t GetActorType() const;
		void SetActorType(uint32_t eType);

		bool IsPC();
		bool IsNPC();
		bool IsEnemy();
		bool IsStone();
		bool IsWarp();
		bool IsGoto();
		bool IsObject();
		bool IsDoor();
		bool IsPoly();

		bool IsBuilding();

		bool IsHandMode();
		bool IsBowMode();
		bool IsTwoHandMode();

		uint32_t GetPartItemID(uint32_t dwPartIndex);

		// Attach Effect

		/////////////////////////////////////////////////////////////////////////////////////
		// Motion Queueing System

		bool IsPushing();

		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// Collison Detection

		// Collision Detection Checking

		/////////////////////////////////////////////////////////////////////////////////////

	protected:
	public:
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// Battle
		// Input
		// 하위로 옮길 가능성이 있는 코드들
		// 네트웍 연동시 전투 관련은 플레이어를 제외하곤 단순히 Showing Type이기 때문에
		// 조건 검사가 필요 없다.

		// Command

		void Revive();

		int32_t IsSleep();
		int32_t IsParalysis();
		int32_t IsFaint();
		int32_t IsResistFallen();
		int32_t IsWaiting();
		int32_t IsMoving();
		int32_t IsDead();
		int32_t IsStun();
		int32_t IsAttacked();
		int32_t IsDamage();
		int32_t IsKnockDown();
		void SetWalkMode();
		void SetRunMode();
		void Stun();
		void Die();
		void DieEnd();

		MOTION_KEY GetNormalAttackIndex();
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// Position
		const D3DXVECTOR3& GetMovementVectorRef();
		const D3DXVECTOR3& GetPositionVectorRef();

		void SetCurPixelPosition(const TPixelPosition& c_rkPPosCur);
		void NEW_SetAtkPixelPosition(const TPixelPosition& c_rkPPosAtk);
		void NEW_SetSrcPixelPosition(const TPixelPosition& c_rkPPosSrc);
		void NEW_SetDstPixelPosition(const TPixelPosition& c_rkPPosDst);
		void NEW_SetDstPixelPositionZ(float z);

		const TPixelPosition& NEW_GetAtkPixelPositionRef();
		const TPixelPosition& NEW_GetCurPixelPositionRef();
		const TPixelPosition& NEW_GetSrcPixelPositionRef();
		const TPixelPosition& NEW_GetDstPixelPositionRef();

		const TPixelPosition& NEW_GetLastPixelPositionRef();

		void GetPixelPosition(TPixelPosition* pPixelPosition);
		void SetPixelPosition(const TPixelPosition& c_rPixelPos);

		// Rotation Command
		void LookAt(float fDirRot);
		void LookAt(float fx, float fy);
		void LookAt(CActorInstance* pInstance);
		void LookWith(CActorInstance* pInstance);
		void LookAtFromXY(float x, float y, CActorInstance* pDestInstance);

		void SetReachScale(float fScale);
		void SetOwner(uint32_t dwOwnerVID);

		float GetRotation();
		float GetTargetRotation();

		float GetAdvancingRotation();

		float GetRotatingTime();
		void SetRotation(float fRot);
		void SetXYRotation(float fRotX, float fRotY);
		void BlendRotation(float fRot, float fBlendTime);
		void SetAdvancingRotation(float fRot);
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		void MotionEventProcess();
		void MotionEventProcess(uint32_t dwcurTime, int iIndex, const CRaceMotionData::TMotionEventData* c_pData);
		void SoundEventProcess(int32_t bCheckFrequency);
		/////////////////////////////////////////////////////////////////////////////////////

		////
		// Rendering Functions - Temporary Place
		int32_t IsMovement();

		void SetBlendRenderMode();
		void SetAlphaValue(float fAlpha);
		float GetAlphaValue();
		void BlendAlphaValue(float fDstAlpha, float fDuration);
		void SetSpecularInfo(int32_t bEnable, int iPart, float fAlpha);
		void SetSpecularInfoForce(int32_t bEnable, int iPart, float fAlpha);

		void RenderTrace();

	protected:
		void __AdjustCollisionMovement(const CGraphicObjectInstance* c_pGraphicObjectInstance);

	public:
		void AdjustDynamicCollisionMovement(const CActorInstance* c_pActorInstance);

		// Weapon Trace

		// ETC
		void UpdateAttribute();
		bool IntersectDefendingSphere();
		float GetHeight();
		void ClearAttachingEffect();

		// Fishing
		void SetFishingPosition(D3DXVECTOR3& rv3Position);

		// Flying Methods
		// As a Flying Target
	public:
		virtual D3DXVECTOR3 OnGetFlyTargetPosition();

		// As a Shooter
		// NOTE : target and target position are exclusive
	public:
		void ClearFlyTarget();
		bool IsFlyTargetObject();
		void AddFlyTarget(const CFlyTarget& cr_FlyTarget);
		void SetFlyTarget(const CFlyTarget& cr_FlyTarget);
		void LookAtFlyTarget();

		float GetFlyTargetDistance();

		void ClearFlyEventHandler();
		void SetFlyEventHandler(IFlyEventHandler* pHandler);

		// 2004. 07. 07. [levites] - 스킬 사용중 타겟이 바뀌는 문제 해결을 위한 코드
		bool CanChangeTarget();

	protected:
		IFlyEventHandler* m_pFlyEventHandler;

	public:
		void MountHorse(CActorInstance* pkHorse);
		void RotationProcess();
		void PhysicsProcess();
		void TransformProcess();
		void AccumulationMovement();
		void TraceProcess();
		void __AccumulationMovement(float fRot);

	private:
	protected:
		CFlyTarget m_kFlyTarget;
		CFlyTarget m_kBackupFlyTarget;
		std::deque<CFlyTarget> m_kQue_kFlyTarget;

	protected:
		void OnUpdate();
		void OnRender();

		void ProcessMotionEventEffectEvent(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventEffectToTargetEvent(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventSpecialAttacking(int iMotionEventIndex, const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventSound(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventFly(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventWarp(const CRaceMotionData::TMotionEventData* c_pData);

		void AddMovement(float fx, float fy, float fz);

	protected:
		void __InitializeMovement();

	protected:
		void __Initialize();

	protected:
		float __GetAttackSpeed();
		void __ClearHittedActorInstanceMap(); // 때려진 액터 인스턴스 맵을 지운다

		bool __IsMountingHorse();

		void __CancelComboAttack(); // 콤보 공격 취소

		uint16_t __GetCurrentComboType();

		void __ShowEvent();
		void __HideEvent();
		int32_t __IsHiding();

		float __GetReachScale();

		void __CreateAttributeInstance(CAttributeData* pData);

		bool __IsFlyTargetPC();
		bool __IsSameFlyTarget(CActorInstance* pInstance);
		D3DXVECTOR3 __GetFlyTargetPosition();

	protected:
		void __DestroyWeaponTrace(); // 무기 잔상을 제거한다
		void __ShowWeaponTrace();	 // 무기 잔상을 보인다
		void __HideWeaponTrace();	 // 무기 잔상을 감춘다

	protected:
		// collision data
		void OnUpdateCollisionData(const CStaticCollisionDataVector* pscdVector);
		void OnUpdateHeighInstance(CAttributeInstance* pAttributeInstance);
		bool OnGetObjectHeight(float fX, float fY, float* pfHeight);

	protected:
		/////////////////////////////////////////////////////////////////////////////////////
		// Motion Queueing System
		TMotionDeque m_MotionDeque;
		SCurrentMotionNode m_kCurMotNode;
		uint16_t m_wcurMotionMode;
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// For Collision Detection
		TCollisionPointInstanceList m_BodyPointInstanceList;
		TCollisionPointInstanceList m_DefendingPointInstanceList;
		SSplashArea m_kSplashArea; // TODO : 복수에 대한 고려를 해야한다 - [levites]
		CAttributeInstance* m_pAttributeInstance;
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// For Battle System
		std::vector<CWeaponTrace*> m_WeaponTraceVector;
		CPhysicsObject m_PhysicsObject;

		uint32_t m_dwcurComboIndex;

		uint32_t m_eActorType;

		uint32_t m_eRace;
		uint32_t m_eShape;
		uint32_t m_eHair;
		int32_t m_isPreInput;
		int32_t m_isNextPreInput;
		uint32_t m_dwcurComboBackMotionIndex;

		uint16_t m_wcurComboType;

		float m_fAtkDirRot;

		CRaceData* m_pkCurRaceData;
		CRaceMotionData* m_pkCurRaceMotionData;

		// Defender
		float m_fInvisibleTime;
		int32_t m_isHiding;

		// TODO : State로 통합 시킬 수 있는지 고려해 볼것
		int32_t m_isResistFallen;
		int32_t m_isSleep;
		int32_t m_isFaint;
		int32_t m_isParalysis;
		int32_t m_isStun;
		int32_t m_isRealDead;
		int32_t m_isWalking;
		int32_t m_isMain;

		// Effect
		uint32_t m_dwBattleHitEffectID;
		uint32_t m_dwBattleAttachEffectID;
		/////////////////////////////////////////////////////////////////////////////////////

		// Fishing
		D3DXVECTOR3 m_v3FishingPosition;
		int m_iFishingEffectID;

		// Position
		float m_x;
		float m_y;
		float m_z;
		D3DXVECTOR3 m_v3Pos;
		D3DXVECTOR3 m_v3Movement;
		int32_t m_bNeedUpdateCollision;

		uint32_t m_dwShakeTime;

		float m_fReachScale;
		float m_fMovSpd;
		float m_fAtkSpd;

		// Rotation
		float m_fcurRotation;
		float m_rotBegin;
		float m_rotEnd;
		float m_rotEndTime;
		float m_rotBeginTime;
		float m_rotBlendTime;
		float m_fAdvancingRotation;
		float m_rotX;
		float m_rotY;

		float m_fOwnerBaseTime;

		// Rendering
		int m_iRenderMode;
		D3DXCOLOR m_AddColor;
		float m_fAlphaValue;

		// Part
		uint32_t m_adwPartItemID[CRaceData::PART_MAX_NUM];

		// Attached Effect
		std::list<TAttachingEffect> m_AttachingEffectList;
		bool m_bEffectInitialized;

		// material color
		uint32_t m_dwMtrlColor;
		uint32_t m_dwMtrlAlpha;

		TPixelPosition m_kPPosCur;
		TPixelPosition m_kPPosSrc;
		TPixelPosition m_kPPosDst;
		TPixelPosition m_kPPosAtk;

		TPixelPosition m_kPPosLast;

		THitDataMap m_HitDataMap;

		CActorInstance* m_pkHorse;
		CSpeedTreeWrapper* m_pkTree;

	protected:
		uint32_t m_dwSelfVID;
		uint32_t m_dwOwnerVID;

	protected:
		void __InitializeStateData();
		void __InitializeMotionData();
		void __InitializeRotationData();
		void __InitializePositionData();

	public: // InstanceBase 통합전 임시로 public
		IEventHandler* __GetEventHandlerPtr();
		IEventHandler& __GetEventHandlerRef();

		void __OnSyncing();
		void __OnWaiting();
		void __OnMoving();
		void __OnMove();
		void __OnStop();
		void __OnWarp();
		void __OnClearAffects();
		void __OnSetAffect(uint32_t uAffect);
		void __OnResetAffect(uint32_t uAffect);
		void __OnAttack(uint16_t wMotionIndex);
		void __OnUseSkill(uint32_t uMotSkill, uint32_t uLoopCount, bool isMoving);

	protected:
		void __OnHit(uint32_t uSkill, CActorInstance& rkInstVictm, int32_t isSendPacket);

	public:
	protected:
		bool m_canSkipCollision;

	protected:
		struct SBlendAlpha
		{
				float m_fBaseTime;
				float m_fBaseAlpha;
				float m_fDuration;
				float m_fDstAlpha;

				uint32_t m_iOldRenderMode;
				bool m_isBlending;
		} m_kBlendAlpha;

		void __BlendAlpha_Initialize();
		void __BlendAlpha_Apply(float fDstAlpha, float fDuration);
		void __BlendAlpha_Update();
		void __BlendAlpha_UpdateFadeIn();
		void __BlendAlpha_UpdateFadeOut();
		void __BlendAlpha_UpdateComplete();
		float __BlendAlpha_GetElapsedTime();

		void __Push(int x, int y);

	public:
		void TEMP_Push(int x, int y);
		bool __IsSyncing();

		void __CreateTree(const char* c_szFileName);
		void __DestroyTree();
		void __SetTreePosition(float fx, float fy, float fz);

	protected:
		IEventHandler* m_pkEventHandler;

	protected:
		static bool ms_isDirLine;

	protected:
		friend class CActorInstanceMotion;
		friend class CActorInstanceBattle;
		friend class CActorInstanceAttach;
		friend class CActorInstanceRender;
		friend class CActorInstanceCollisionDetection;

		CActorInstanceMotion m_kMotion;
		CActorInstanceBattle m_kBattle;
		CActorInstanceAttach m_kAttach;
		CActorInstanceRender m_kRender;
		CActorInstanceCollisionDetection m_kCollisionDetection;
};
