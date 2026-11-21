#include <Client/UI/StdAfx.h>
#include <Client/Player/PythonPlayer.h>
#include <Client/Player/PythonPlayerInput.h>
#include <Client/UI/PythonApplication.h>

#include <Neoeter/Render/Camera.h>

void CPythonPlayerInput::NEW_SetMouseMoveState(int eMBS)
{
	if (CPythonPlayer::MBS_PRESS == eMBS)
	{
		NEW_MoveToMouseScreenDirection();

		m_rkOwner.m_isDirMov = true;
	}
	else if (CPythonPlayer::MBS_CLICK == eMBS)
	{
		NEW_Stop();

		m_rkOwner.m_isDirMov = false;
	}
}

bool CPythonPlayerInput::NEW_MoveToMouseScreenDirection()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();

	long lMouseX;
	long lMouseY;
	rkWndMgr.GetMousePosition(lMouseX, lMouseY);

	long lScrWidth = rkWndMgr.GetScreenWidth();
	long lScrHeight = rkWndMgr.GetScreenHeight();
	float fMouseX = lMouseX / float(lScrWidth);
	float fMouseY = lMouseY / float(lScrHeight);

	float fDirRot;
	NEW_GetMouseDirRotation(fMouseX, fMouseY, &fDirRot);

	return NEW_MoveToDirection(fDirRot);
}

void CPythonPlayerInput::NEW_SetMouseCameraState(int eMBS)
{
	CPythonApplication& rkApp = CPythonApplication::Instance();
	CPythonBackground& rkBG = CPythonBackground::Instance();
	CCamera* pkCmrCur = CCameraManager::Instance().GetCurrentCamera();

	if (pkCmrCur)
	{
		if (CPythonPlayer::MBS_PRESS == eMBS)
		{
			POINT ptMouse;
			rkApp.GetMousePosition(&ptMouse);

			pkCmrCur->BeginDrag(ptMouse.x, ptMouse.y);

			if (!rkBG.IsMapReady())
				return;

			rkApp.Cursor().SetNum(CApplicationCursor::CAMERA_ROTATE);
			if (CApplicationCursor::MODE_HARDWARE == rkApp.Cursor().GetMode())
				rkApp.Cursor().SetVisible(FALSE, true);
		}
		else if (CPythonPlayer::MBS_CLICK == eMBS)
		{
			bool isCameraDrag = pkCmrCur->EndDrag();

			if (!rkBG.IsMapReady())
				return;

			rkApp.Cursor().SetNum(CApplicationCursor::NORMAL);
			if (CApplicationCursor::MODE_HARDWARE == rkApp.Cursor().GetMode())
				rkApp.Cursor().SetVisible(TRUE);

			if (!isCameraDrag)
			{
				__ChangeTargetToPickedInstance();

				CInstanceBase* pkInstPicked;
				if (m_rkOwner.__GetPickedActorPtr(&pkInstPicked))
				{
					OpenCharacterMenu(pkInstPicked->GetVirtualID());
				}
			}
		}
	}
}

void CPythonPlayerInput::NEW_SetMouseSmartState(int eMBS, bool isAuto)
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain) return;

	// PrivateShop
	if (m_rkOwner.IsOpenPrivateShop())
	{
		// ShopPacket 이 오기전 잠깐 동안
		m_rkOwner.m_isSmtMov = false;
		return;
	}

	// Emotion
	if (m_rkOwner.__IsProcessingEmotion())
	{
		return;
	}

	// AFFECT_STUN 처리
	if (pkInstMain->IsSleep())
	{
		return;
	}

	if (CPythonPlayer::MBS_PRESS == eMBS)
	{
		m_rkOwner.m_isSmtMov = true;

		__OnPressSmart(*pkInstMain, isAuto);
	}
	else if (CPythonPlayer::MBS_CLICK == eMBS)
	{
		m_rkOwner.m_isSmtMov = false;

		__OnClickSmart(*pkInstMain, isAuto);
	}
}

void CPythonPlayerInput::__OnPressSmart(CInstanceBase& rkInstMain, bool isAuto)
{
	uint32_t dwPickedItemID;
	uint32_t dwPickedActorID;
	TPixelPosition kPPosPickedGround;

	bool isPickedItemID = m_rkOwner.__GetPickedItemID(&dwPickedItemID);
	bool isPickedActorID = m_rkOwner.__GetPickedActorID(&dwPickedActorID);
	bool isPickedGroundPos = m_rkOwner.__GetPickedGroundPos(&kPPosPickedGround);

	if (isPickedItemID)
	{
		__OnPressItem(rkInstMain, dwPickedItemID);
	}
	else if (isPickedActorID && dwPickedActorID != m_rkOwner.GetMainCharacterIndex())
	{
		__OnPressActor(rkInstMain, dwPickedActorID, isAuto);
	}
	else if (isPickedGroundPos)
	{
		__OnPressGround(rkInstMain, kPPosPickedGround);
	}
	else
	{
		__OnPressScreen(rkInstMain);
	}
}

void CPythonPlayerInput::__OnClickSmart(CInstanceBase& rkInstMain, bool isAuto)
{
	uint32_t dwPickedItemID;
	uint32_t dwPickedActorID;
	TPixelPosition kPPosPickedGround;
	if (m_rkOwner.__GetPickedItemID(&dwPickedItemID))
	{
		__OnClickItem(rkInstMain, dwPickedItemID);
	}
	else if (m_rkOwner.__GetPickedActorID(&dwPickedActorID))
	{
		__OnClickActor(rkInstMain, dwPickedActorID, isAuto);
	}
	else if (m_rkOwner.__GetPickedGroundPos(&kPPosPickedGround))
	{
		__OnClickGround(rkInstMain, kPPosPickedGround);
	}
	else
	{
		rkInstMain.GetMovement().NEW_Stop();
	}
}

void CPythonPlayerInput::__ShowPickedEffect(const TPixelPosition& c_rkPPosPickedGround)
{
#ifdef __MOVIE_MODE__
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain) return;

	if (pkInstMain->IsMovieMode())
		return;
#endif
	m_rkOwner.NEW_ShowEffect(CPythonPlayer::EFFECT_PICK, c_rkPPosPickedGround);
}

bool CPythonPlayerInput::NEW_SetMouseFunc(int eMBT, int eMBF)
{
	if (eMBT >= CPythonPlayer::MBT_NUM)
		return false;

	m_rkOwner.m_aeMBFButton[eMBT] = eMBF;

	return true;
}

int CPythonPlayerInput::NEW_GetMouseFunc(int eMBT)
{
	if (eMBT >= CPythonPlayer::MBT_NUM)
		return false;

	return m_rkOwner.m_aeMBFButton[eMBT];
}

void CPythonPlayerInput::SetQuickCameraMode(bool isEnable)
{
	if (isEnable)
	{
	}
	else
	{
		NEW_SetMouseCameraState(CPythonPlayer::MBS_CLICK);
	}
}

bool CPythonPlayerInput::NEW_SetMouseState(int eMBT, int eMBS)
{
	if (eMBT >= CPythonPlayer::MBT_NUM)
		return false;

	int eMBF = m_rkOwner.m_aeMBFButton[eMBT];
	switch (eMBF)
	{
		case CPythonPlayer::MBF_MOVE:
			if (__CanMove())
				NEW_SetMouseMoveState(eMBS);
			break;
		case CPythonPlayer::MBF_SMART:
			if (CPythonApplication::Instance().IsPressed(DIK_LCONTROL) || CPythonApplication::Instance().IsPressed(DIK_RCONTROL))
			{
				NEW_Attack();
			}
			else
			{
				NEW_SetMouseSmartState(eMBS, false);
			}
			break;
		case CPythonPlayer::MBF_CAMERA:
			NEW_SetMouseCameraState(eMBS);
			break;
		case CPythonPlayer::MBF_AUTO:
			NEW_SetMouseSmartState(eMBS, true);
			break;
		case CPythonPlayer::MBF_ATTACK:
			NEW_Attack();
			break;
		case CPythonPlayer::MBF_SKILL:
			if (CPythonApplication::Instance().IsPressed(DIK_LCONTROL))
			{
				NEW_SetMouseCameraState(eMBS);
			}
			else
			{
				if (CPythonPlayer::MBS_PRESS == eMBS)
				{
					__ChangeTargetToPickedInstance();
					m_rkOwner.GetSkill().__UseCurrentSkill();
				}
			}
			break;
	}

	return true;
}

void CPythonPlayerInput::NEW_SetMouseMiddleButtonState(int eMBState)
{
	NEW_SetMouseCameraState(eMBState);
}

void CPythonPlayerInput::NEW_RefreshMouseWalkingDirection()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain) return;

	switch (m_rkOwner.m_eReservedMode)
	{
		case CPythonPlayer::MODE_CLICK_ITEM:
		{
			CPythonItem& rkIT = CPythonItem::Instance();

			TPixelPosition kPPosPickedItem;
			if (rkIT.GetGroundItemPosition(m_rkOwner.m_dwIIDReserved, &kPPosPickedItem))
			{
				if (pkInstMain->GetBattle().NEW_GetDistanceFromDestPixelPosition(kPPosPickedItem) < 20.0f)
				{
					CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();

					TPixelPosition kPPosCur;
					pkInstMain->GetTransform().NEW_GetPixelPosition(&kPPosCur);

					float fCurRot = pkInstMain->GetTransform().GetRotation();
					rkNetStream.SendCharacterStatePacket(kPPosCur, fCurRot, CInstanceBase::FUNC_WAIT, 0);
					m_rkOwner.SendClickItemPacket(m_rkOwner.m_dwIIDReserved);

					pkInstMain->GetMovement().NEW_Stop();

					__ClearReservedAction();
				}
				else
				{
					pkInstMain->GetMovement().NEW_MoveToDestPixelPositionDirection(kPPosPickedItem);
				}
			}
			else
			{
				__ClearReservedAction();
			}

			break;
		}

		case CPythonPlayer::MODE_CLICK_ACTOR:
		{
			__ReserveProcess_ClickActor();
			break;
		}

		case CPythonPlayer::MODE_CLICK_POSITION:
		{
			if (!pkInstMain->GetMotion().isLock())
				if (NEW_IsEmptyReservedDelayTime(CPythonApplication::Instance().GetGlobalElapsedTime()))
				{
					pkInstMain->GetMovement().NEW_MoveToDestPixelPositionDirection(m_rkOwner.m_kPPosReserved);
					__ClearReservedAction();
				}
			break;
		}

		case CPythonPlayer::MODE_USE_SKILL:
		{
			CInstanceBase* pkInstReserved = m_rkOwner.NEW_FindActorPtr(m_rkOwner.m_dwVIDReserved);
			if (pkInstReserved)
			{
				float fDistance = pkInstMain->GetDistance(pkInstReserved);

				// #0000806: [M2EU] 수룡에게 무사(나한군) 탄환격 스킬 사용 안됨
				extern bool IS_HUGE_RACE(unsigned int vnum);
				if (IS_HUGE_RACE(pkInstReserved->GetRace())) // 거대 종족은 근접을 못함
				{
					fDistance -= 200.0f; // TEMP: 일단 하드 코딩 처리. 정석적으로는 바운드 스피어를 고려해야함
				}

				if (fDistance < float(m_rkOwner.m_dwSkillRangeReserved))
				{
					// 접근 도중 Target 이 바뀌어 있을 수도 있다 - [levites]
					SetTarget(m_rkOwner.m_dwVIDReserved);
					if (m_rkOwner.GetSkill().__UseSkill(m_rkOwner.m_dwSkillSlotIndexReserved))
						__ClearReservedAction();
				}
				else
				{
					pkInstMain->GetMovement().NEW_MoveToDestInstanceDirection(*pkInstReserved);
				}
			}
			else
			{
				__ClearReservedAction();
			}
			break;
		}

		default:
			break;
	}

	if (m_rkOwner.m_isSmtMov)
		__OnPressSmart(*pkInstMain, false);

	if (m_rkOwner.m_isDirMov)
		NEW_MoveToMouseScreenDirection();

	if (m_rkOwner.m_isDirKey)
		NEW_SetMultiDirKeyState(m_rkOwner.m_isLeft, m_rkOwner.m_isRight, m_rkOwner.m_isUp, m_rkOwner.m_isDown);

	if (m_rkOwner.m_isAtkKey)
		NEW_Attack();

	m_rkOwner.m_iComboOld = pkInstMain->GetBattle().GetComboIndex();
}

int32_t CPythonPlayerInput::__IsRightButtonSkillMode()
{
	return CPythonPlayer::MBF_SKILL == m_rkOwner.m_aeMBFButton[CPythonPlayer::MBT_RIGHT];
}
