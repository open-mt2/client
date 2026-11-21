#include <Client/UI/StdAfx.h>
#include <Client/Player/PythonPlayer.h>
#include <Client/Player/PythonPlayerInput.h>

#include <Client/UI/Instance/InstanceBase.h>

void CPythonPlayerInput::SetAttackKeyState(bool isPress)
{
	if (isPress)
	{
		CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
		if (pkInstMain)
			if (pkInstMain->IsFishingMode())
			{
				NEW_Fishing();
				return;
			}
	}

	m_rkOwner.m_isAtkKey = isPress;
}

void CPythonPlayerInput::NEW_SetSingleDIKKeyState(int eDIKKey, bool isPress)
{
	if (NEW_CancelFishing())
		return;

	switch (eDIKKey)
	{
		case DIK_UP:
			NEW_SetSingleDirKeyState(CPythonPlayer::DIR_UP, isPress);
			break;
		case DIK_DOWN:
			NEW_SetSingleDirKeyState(CPythonPlayer::DIR_DOWN, isPress);
			break;
		case DIK_LEFT:
			NEW_SetSingleDirKeyState(CPythonPlayer::DIR_LEFT, isPress);
			break;
		case DIK_RIGHT:
			NEW_SetSingleDirKeyState(CPythonPlayer::DIR_RIGHT, isPress);
			break;
	}
}

void CPythonPlayerInput::NEW_SetSingleDirKeyState(int eDirKey, bool isPress)
{
	switch (eDirKey)
	{
		case CPythonPlayer::DIR_UP:
			m_rkOwner.m_isUp = isPress;
			break;
		case CPythonPlayer::DIR_DOWN:
			m_rkOwner.m_isDown = isPress;
			break;
		case CPythonPlayer::DIR_LEFT:
			m_rkOwner.m_isLeft = isPress;
			break;
		case CPythonPlayer::DIR_RIGHT:
			m_rkOwner.m_isRight = isPress;
			break;
	}

	m_rkOwner.m_isDirKey = (m_rkOwner.m_isUp || m_rkOwner.m_isDown || m_rkOwner.m_isLeft || m_rkOwner.m_isRight);

	NEW_SetMultiDirKeyState(m_rkOwner.m_isLeft, m_rkOwner.m_isRight, m_rkOwner.m_isUp, m_rkOwner.m_isDown);
}

void CPythonPlayerInput::NEW_SetMultiDirKeyState(bool isLeft, bool isRight, bool isUp, bool isDown)
{
	if (!__CanMove())
		return;

	bool isAny = (isLeft || isRight || isUp || isDown);

	if (isAny)
	{
		float fDirRot = 0.0f;
		NEW_GetMultiKeyDirRotation(isLeft, isRight, isUp, isDown, &fDirRot);

		if (!NEW_MoveToDirection(fDirRot))
		{
			Tracen("CPythonPlayerInput::NEW_SetMultiKeyState - NEW_Move -> ERROR");
			return;
		}
	}
	else
	{
		NEW_Stop();
	}
}

float CPythonPlayerInput::GetDegreeFromDirection(int iUD, int iLR)
{
	switch (iUD)
	{
		case CPythonPlayer::KEYBOARD_UD_UP:
			if (CPythonPlayer::KEYBOARD_LR_LEFT == iLR)
			{
				return +45.0f;
			}
			else if (CPythonPlayer::KEYBOARD_LR_RIGHT == iLR)
			{
				return -45.0f;
			}

			return 0.0f;
			break;

		case CPythonPlayer::KEYBOARD_UD_DOWN:
			if (CPythonPlayer::KEYBOARD_LR_LEFT == iLR)
			{
				return +135.0f;
			}
			else if (CPythonPlayer::KEYBOARD_LR_RIGHT == iLR)
			{
				return -135.0f;
			}

			return +180.0f;
			break;

		case CPythonPlayer::KEYBOARD_UD_NONE:
			if (CPythonPlayer::KEYBOARD_LR_LEFT == iLR)
			{
				return +90.0f;
			}
			else if (CPythonPlayer::KEYBOARD_LR_RIGHT == iLR)
			{
				return -90.0f;
			}
			break;
	}

	return 0.0f;
}
