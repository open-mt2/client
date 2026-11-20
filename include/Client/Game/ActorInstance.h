#pragma once

// class CActorInstance

//		Note : 캐릭터의 Lighting, Local Point Light, Weapon Trace 등의 효과와 콤보 데이타, 사운드 데이타,
//             모션 데이타 들을 추상적, 총체적으로 관리하는 개별 매니저가 될 것이다.

#include <Client/Game/FlyTarget.h>
#include <Client/Game/RaceData.h>
#include <Client/Game/RaceMotionData.h>
#include <Client/Game/PhysicsObject.h>
#include <Client/Game/ActorInstanceInterface.h>
#include <Client/Game/Interface.h>
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
		static bool IsDirLine();

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
		// Motion Queueing System
		enum EMotionPushType
		{
			MOTION_TYPE_NONE,
			MOTION_TYPE_ONCE,
			MOTION_TYPE_LOOP,
		};

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
		static void ShowDirectionLine(bool isVisible);
		static void DestroySystem();

	public:
		CActorInstance();
		virtual ~CActorInstance();

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

		void SetMaterialAlpha(uint32_t dwAlpha);
		void SetMaterialColor(uint32_t dwColor);

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

		bool CanAct();
		bool CanMove();
		bool CanAttack();
		bool CanUseSkill();

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

		void AttachWeapon(uint32_t dwItemIndex, uint32_t dwParentPartIndex = CRaceData::PART_MAIN, uint32_t dwPartIndex = CRaceData::PART_WEAPON);
		void AttachWeapon(uint32_t dwParentPartIndex, uint32_t dwPartIndex, CItemData* pItemData);

		void RefreshActorInstance();
		uint32_t GetPartItemID(uint32_t dwPartIndex);

		// Attach Effect
		int32_t GetAttachingBoneName(uint32_t dwPartIndex, const char** c_szBoneName);
		void UpdateAttachingInstances();
		void DettachEffect(uint32_t dwEID);
		uint32_t AttachEffectByName(uint32_t dwParentPartIndex, const char* c_pszBoneName, const char* c_pszEffectFileName);
		uint32_t AttachEffectByID(uint32_t dwParentPartIndex, const char* c_pszBoneName, uint32_t dwEffectID, const D3DXVECTOR3* c_pv3Position = NULL);
		uint32_t AttachSmokeEffect(uint32_t eSmoke);

		/////////////////////////////////////////////////////////////////////////////////////
		// Motion Queueing System
		void SetMotionMode(int iMotionMode); // FIXME : 모드의 시간차 적용이 가능하게끔 한다.
		int GetMotionMode();
		void SetLoopMotion(uint32_t dwMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f);
		bool InterceptOnceMotion(uint32_t dwMotion, float fBlendTime = 0.1f, uint32_t uSkill = 0, float fSpeedRatio = 1.0f);
		bool InterceptLoopMotion(uint32_t dwMotion, float fBlendTime = 0.1f);
		bool PushOnceMotion(uint32_t dwMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f); // FIXME : 모드의 시간차 적용이 가능하게끔 한다.
		bool PushLoopMotion(uint32_t dwMotion, float fBlendTime = 0.1f, float fSpeedRatio = 1.0f); // FIXME : 모드의 시간차 적용이 가능하게끔 한다.
		void SetMotionLoopCount(int iCount);

		bool IsPushing();

		int32_t isLock();
		int32_t IsUsingSkill();
		int32_t CanCheckAttacking();
		int32_t CanCancelSkill();
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// Collison Detection
		bool CreateCollisionInstancePiece(uint32_t dwAttachingModelIndex, const NRaceData::TAttachingData* c_pAttachingData, TCollisionPointInstance* pPointInstance);

		void UpdatePointInstance();
		void UpdatePointInstance(TCollisionPointInstance* pPointInstance);
		bool CheckCollisionDetection(const CDynamicSphereInstanceVector* c_pAttackingSphereVector, D3DXVECTOR3* pv3Position);

		// Collision Detection Checking
		virtual bool TestCollisionWithDynamicSphere(const CDynamicSphereInstance& dsi);

		void UpdateAdvancingPointInstance();

		int32_t IsClickableDistanceDestInstance(CActorInstance& rkInstDst, float fDistance);

		bool AvoidObject(const CGraphicObjectInstance& c_rkBGObj);
		bool IsBlockObject(const CGraphicObjectInstance& c_rkBGObj);
		void BlockMovement();
		/////////////////////////////////////////////////////////////////////////////////////

	protected:
		int32_t __TestObjectCollision(const CGraphicObjectInstance* c_pObjectInstance);

	public:
		int32_t TestActorCollision(CActorInstance& rVictim);
		int32_t TestPhysicsBlendingCollision(CActorInstance& rVictim);

		int32_t AttackingProcess(CActorInstance& rVictim);

		void PreAttack();
		/////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////
		// Battle
		// Input
		// 하위로 옮길 가능성이 있는 코드들
		// 네트웍 연동시 전투 관련은 플레이어를 제외하곤 단순히 Showing Type이기 때문에
		// 조건 검사가 필요 없다.
		void InputNormalAttackCommand(float fDirRot); // Process input - Only used by player's character
		bool InputComboAttackCommand(float fDirRot);  // Process input - Only used by player's character

		// Command
		int32_t isAttacking();
		int32_t isNormalAttacking();
		int32_t isComboAttacking();
		int32_t IsSplashAttacking();
		int32_t IsUsingMovingSkill();
		int32_t IsActEmotion();
		uint32_t GetComboIndex();
		float GetAttackingElapsedTime();
		void SetBlendingPosition(const TPixelPosition& c_rPosition, float fBlendingTime = 1.0f);
		void ResetBlendingPosition();
		void GetBlendingPosition(TPixelPosition* pPosition);

		int32_t NormalAttack(float fDirRot, float fBlendTime = 0.1f);
		int32_t ComboAttack(uint32_t wMotionIndex, float fDirRot, float fBlendTime = 0.1f);

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

		void SetBattleHitEffect(uint32_t dwID);
		void SetBattleAttachEffect(uint32_t dwID);

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

		void RestoreRenderMode();

		void BeginDiffuseRender();
		void EndDiffuseRender();
		void BeginOpacityRender();
		void EndOpacityRender();

		void BeginBlendRender();
		void EndBlendRender();
		void SetBlendRenderMode();
		void SetAlphaValue(float fAlpha);
		float GetAlphaValue();
		void BlendAlphaValue(float fDstAlpha, float fDuration);
		void SetSpecularInfo(int32_t bEnable, int iPart, float fAlpha);
		void SetSpecularInfoForce(int32_t bEnable, int iPart, float fAlpha);

		void BeginAddRender();
		void EndAddRender();
		void SetAddRenderMode();
		void SetAddColor(const D3DXCOLOR& c_rColor);

		void BeginModulateRender();
		void EndModulateRender();
		void SetModulateRenderMode();

		void SetRenderMode(int iRenderMode);

		void RenderTrace();
		void RenderCollisionData();
		void RenderToShadowMap();

	protected:
		void __AdjustCollisionMovement(const CGraphicObjectInstance* c_pGraphicObjectInstance);

	public:
		void AdjustDynamicCollisionMovement(const CActorInstance* c_pActorInstance);

		// Weapon Trace
		void SetWeaponTraceTexture(const char* szTextureName);
		void UseTextureWeaponTrace();
		void UseAlphaWeaponTrace();

		// ETC
		void UpdateAttribute();
		bool IntersectDefendingSphere();
		float GetHeight();
		void ShowAllAttachingEffect();
		void HideAllAttachingEffect();
		void ClearAttachingEffect();

		// Fishing
		bool CanFishing();
		int32_t IsFishing();
		void SetFishingPosition(D3DXVECTOR3& rv3Position);

		// Flying Methods
		// As a Flying Target
	public:
		virtual D3DXVECTOR3 OnGetFlyTargetPosition();

		void OnShootDamage();

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
		void HORSE_MotionProcess(int32_t isPC);
		void MotionProcess(int32_t isPC);
		void RotationProcess();
		void PhysicsProcess();
		void ComboProcess();
		void TransformProcess();
		void AccumulationMovement();
		void ShakeProcess();
		void TraceProcess();
		void __MotionEventProcess(int32_t isPC);
		void __AccumulationMovement(float fRot);
		int32_t __SplashAttackProcess(CActorInstance& rVictim);
		int32_t __NormalAttackProcess(CActorInstance& rVictim);
		bool __CanInputNormalAttackCommand();

	private:
		void __Shake(uint32_t dwDuration);

	protected:
		CFlyTarget m_kFlyTarget;
		CFlyTarget m_kBackupFlyTarget;
		std::deque<CFlyTarget> m_kQue_kFlyTarget;

	protected:
		bool __IsInSplashTime();

		void OnUpdate();
		void OnRender();

		int32_t isValidAttacking();

		void ReservingMotionProcess();
		void CurrentMotionProcess();
		MOTION_KEY GetRandomMotionKey(MOTION_KEY dwMotionKey);

		float GetLastMotionTime(float fBlendTime); // NOTE : 자동으로 BlendTime만큼을 앞당긴 시간을 리턴
		float GetMotionDuration(uint32_t dwMotionKey);

		bool InterceptMotion(EMotionPushType iMotionType, uint16_t wMotion, float fBlendTime = 0.1f, uint32_t uSkill = 0, float fSpeedRatio = 1.0f);
		void PushMotion(EMotionPushType iMotionType, uint32_t dwMotionKey, float fBlendTime, float fSpeedRatio = 1.0f);
		void ProcessMotionEventEffectEvent(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventEffectToTargetEvent(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventSpecialAttacking(int iMotionEventIndex, const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventSound(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventFly(const CRaceMotionData::TMotionEventData* c_pData);
		void ProcessMotionEventWarp(const CRaceMotionData::TMotionEventData* c_pData);

		void AddMovement(float fx, float fy, float fz);

		bool __IsLeftHandWeapon(uint32_t type);
		bool __IsRightHandWeapon(uint32_t type);
		bool __IsWeaponTrace(uint32_t weaponType);
		bool __GetWeaponAttachTransform(uint32_t dwPartIndex, D3DXMATRIX* pMat);

	protected:
		void __InitializeMovement();

	protected:
		void __Initialize();

		void __ClearAttachingEffect();

		float __GetOwnerTime();
		uint32_t __GetOwnerVID();
		bool __CanPushDestActor(CActorInstance& rkActorDst);

	protected:
		void __RunNextCombo();
		void __ClearCombo();
		void __OnEndCombo();

		void __ProcessDataAttackSuccess(const NRaceData::TAttackData& c_rAttackData, CActorInstance& rVictim, const D3DXVECTOR3& c_rv3Position, uint32_t uiSkill = 0, int32_t isSendPacket = TRUE);
		void __ProcessMotionEventAttackSuccess(uint32_t dwMotionKey, uint8_t byEventIndex, CActorInstance& rVictim);
		void __ProcessMotionAttackSuccess(uint32_t dwMotionKey, CActorInstance& rVictim);

		void __HitStone(CActorInstance& rVictim);
		void __HitGood(CActorInstance& rVictim);
		void __HitGreate(CActorInstance& rVictim);

		void __PushDirect(CActorInstance& rVictim);
		void __PushCircle(CActorInstance& rVictim);
		bool __isInvisible();
		void __SetFallingDirection(float fx, float fy);

	protected:
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

	protected:
		float __GetAttackSpeed();
		uint32_t __SetMotion(const SSetMotionData& c_rkSetMotData, uint32_t dwRandMotKey = 0); // 모션 데이터 설정
		void __ClearMotion();

		bool __BindMotionData(uint32_t dwMotionKey); // 모션 데이터를 바인딩
		void __ClearHittedActorInstanceMap();		 // 때려진 액터 인스턴스 맵을 지운다

		uint32_t __GetMotionType(); // 모션 타입 얻기

		bool __IsNeedFlyTargetMotion(); // FlyTarget 이 필요한 모션인가?
		bool __HasMotionFlyEvent();		// 무언가를 쏘는가?
		bool __IsWaitMotion();			// 대기 모션 인가?
		bool __IsMoveMotion();			// 이동 모션 인가?
		bool __IsAttackMotion();		// 공격 모션 인가?
		bool __IsComboAttackMotion();	// 콤보 공격 모션 인가?
		bool __IsDamageMotion();		// 데미지 모션인가?
		bool __IsKnockDownMotion();		// 넉다운 모션인가?
		bool __IsDieMotion();			// 사망 모션 인가?
		bool __IsStandUpMotion();		// 일어서기 모션인가?
		bool __IsMountingHorse();

		bool __CanAttack();			 // 공격 할수 있는가?
		bool __CanNextComboAttack(); // 다음 콤보 어택이 가능한가?

		bool __IsComboAttacking();	// 콤보 공격중인가?
		void __CancelComboAttack(); // 콤보 공격 취소

		uint16_t __GetCurrentMotionIndex();
		uint32_t __GetCurrentMotionKey();

		int __GetLoopCount();
		uint16_t __GetCurrentComboType();

		void __ShowEvent();
		void __HideEvent();
		int32_t __IsHiding();
		int32_t __IsMovingSkill(uint16_t wSkillNumber);

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
		void EnableSkipCollision();
		void DisableSkipCollision();
		bool CanSkipCollision();

	protected:
		void __InitializeCollisionData();

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
};
