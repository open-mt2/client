#pragma once

#include <Client/Game/GameType.h>
#include <Client/Map/MapType.h>

#include <cstdint>
#include <Client/Actor/InstanceBase.h>
#include <Client/Python/Skill/PythonSkill.h>

class CPythonPlayer;

class CPythonPlayerInput
{
	public:
		explicit CPythonPlayerInput(CPythonPlayer& rkOwner);

		float GetDegreeFromDirection(int iUD, int iLR);
		float GetDegreeFromPosition(int ix, int iy, int iHalfWidth, int iHalfHeight);
		uint32_t GetTargetVID();
		void NEW_Attack();
		bool NEW_CancelFishing();
		void NEW_Fishing();
		void NEW_GetMouseDirRotation(float fScrX, float fScrY, float* pfDirRot);
		int NEW_GetMouseFunc(int eMBT);
		void NEW_GetMultiKeyDirRotation(bool isLeft, bool isRight, bool isUp, bool isDown, float* pfDirRot);
		bool NEW_IsEmptyReservedDelayTime(float fElapsedtime); // 네이밍 교정 논의 필요 - [levites]
		bool NEW_MoveToDirection(float fDirRot);
		bool NEW_MoveToMouseScreenDirection();
		void NEW_RefreshMouseWalkingDirection();
		void NEW_ResetCameraRotation();
		void NEW_SetAutoCameraRotationSpeed(float fRotSpd);
		void NEW_SetMouseCameraState(int eMBS);
		bool NEW_SetMouseFunc(int eMBType, int eMBFunc);
		void NEW_SetMouseMiddleButtonState(int eMBState);
		void NEW_SetMouseMoveState(int eMBS);
		void NEW_SetMouseSmartState(int eMBS, bool isAuto);
		bool NEW_SetMouseState(int eMBType, int eMBState);
		void NEW_SetMultiDirKeyState(bool isLeft, bool isRight, bool isUp, bool isDown);
		void NEW_SetSingleDIKKeyState(int eDIKKey, bool isPress);
		void NEW_SetSingleDirKeyState(int eDirKey, bool isPress);
		void NEW_Stop();
		void OpenCharacterMenu(uint32_t dwVictimActorID);
		void PickCloseItem();
		void PickCloseMoney();
		void SetAttackKeyState(bool isPress);
		void SetMovableGroundDistance(float fDistance);
		void SetQuickCameraMode(bool isEnable);
		void SetTarget(uint32_t dwVID, int32_t bForceChange = TRUE);
		bool __CanAttack();
		bool __CanChangeTarget();
		bool __CanMove();
		bool __CanShot(CInstanceBase& rkInstMain, CInstanceBase& rkInstTarget);
		bool __ChangeTargetToPickedInstance();
		void __ClearAutoAttackTargetActorID();
		void __ClearReservedAction();
		void __ClearTarget();
		CInstanceBase* __GetAliveTargetInstancePtr();
		CInstanceBase* __GetDeadTargetInstancePtr();
		uint32_t __GetPickableDistance();
		CInstanceBase* __GetSkillTargetInstancePtr(CPythonSkill::TSkillData& rkSkillData);
		CInstanceBase* __GetTargetActorPtr();
		uint32_t __GetTargetVID();
		bool __IsMovableGroundDistance(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround);
		bool __IsReservedUseSkill(uint32_t dwSkillSlotIndex);
		int32_t __IsRightButtonSkillMode();
		bool __IsSameTargetVID(uint32_t dwVID);
		bool __IsTarget();
		void __OnClickActor(CInstanceBase& rkInstMain, uint32_t dwPickedActorID, bool isAuto);
		void __OnClickGround(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround);
		void __OnClickItem(CInstanceBase& rkInstMain, uint32_t dwPickedItemID);
		void __OnClickSmart(CInstanceBase& rkInstMain, bool isAuto);
		void __OnPressActor(CInstanceBase& rkInstMain, uint32_t dwPickedActorID, bool isAuto);
		void __OnPressGround(CInstanceBase& rkInstMain, const TPixelPosition& c_rkPPosPickedGround);
		void __OnPressItem(CInstanceBase& rkInstMain, uint32_t dwPickedItemID);
		void __OnPressScreen(CInstanceBase& rkInstMain);
		void __OnPressSmart(CInstanceBase& rkInstMain, bool isAuto);
		void __ReserveClickActor(uint32_t dwActorID);
		void __ReserveClickGround(const TPixelPosition& c_rkPPosPickedGround);
		void __ReserveClickItem(uint32_t dwItemID);
		void __ReserveProcess_ClickActor();
		void __ReserveUseSkill(uint32_t dwActorID, uint32_t dwSkillSlotIndex, uint32_t dwRange);
		void __SetAutoAttackTargetActorID(uint32_t dwActorID);
		void __SetTargetVID(uint32_t dwVID);
		void __ShowPickedEffect(const TPixelPosition& c_rkPPosPickedGround);

	private:
		CPythonPlayer& m_rkOwner;
};
