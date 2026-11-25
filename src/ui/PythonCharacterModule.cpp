#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonCharacterManager.h>
#include <Client/UI/PythonNonPlayer.h>

ScriptObject* chrRaceToJob(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int race;
	if (!ScriptTuple_GetInteger(poArgs, 0, &race))
		return Script_BuildException();

	return Script_BuildValue("i", RaceToJob(race));
}

ScriptObject* chrRaceToSex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int race;
	if (!ScriptTuple_GetInteger(poArgs, 0, &race))
		return Script_BuildException();

	return Script_BuildValue("i", RaceToSex(race));
}

ScriptObject* chrDestroy(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonCharacterManager::Instance().Destroy();
	return Script_BuildNone();
}

ScriptObject* chrUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonCharacterManager::Instance().Update();
	return Script_BuildNone();
}

ScriptObject* chrDeform(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonCharacterManager::Instance().Deform();
	return Script_BuildNone();
}

ScriptObject* chrRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonCharacterManager::Instance().Render();
	return Script_BuildNone();
}

ScriptObject* chrRenderCollision(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonCharacterManager::Instance().RenderCollision();
	return Script_BuildNone();
}

// Functions For Python Test Code
ScriptObject* chrCreateInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	if (ScriptTuple_Size(poArgs) == 2)
	{
		ScriptObject* poDict = ScriptTuple_GetItem(poArgs, 1);
		if (!ScriptDict_Check(poDict))
			return Script_BuildException();

		CInstanceBase::SCreateData kCreateData;
		kCreateData.m_bType = CActorInstance::TYPE_PC;
		kCreateData.m_dwLevel = 0;
		kCreateData.m_dwGuildID = 0;
		kCreateData.m_dwEmpireID = 0;
		kCreateData.m_dwVID = iVirtualID;
		kCreateData.m_dwMountVnum = 0;
		kCreateData.m_dwRace = 0;
		kCreateData.m_fRot = CInstanceBase::DIR_NORTH;
		kCreateData.m_lPosX = 0;
		kCreateData.m_lPosY = 0;
		kCreateData.m_stName = "NONAME";
		kCreateData.m_dwStateFlags = 0;
		kCreateData.m_dwMovSpd = 100;
		kCreateData.m_dwAtkSpd = 100;
		kCreateData.m_sAlignment = 0;
		kCreateData.m_byPKMode = 0;
		kCreateData.m_kAffectFlags.Clear();
		kCreateData.m_dwArmor = 8;
		kCreateData.m_dwWeapon = 0;
		kCreateData.m_dwHair = 0;
		kCreateData.m_isMain = false;

		ScriptObject* poHorse = ScriptDict_GetItemString(poDict, "horse");
		if (poHorse)
			kCreateData.m_dwMountVnum = Script_AsLongLong(poHorse);

		ScriptObject* poX = ScriptDict_GetItemString(poDict, "x");
		if (poX)
			kCreateData.m_lPosX = Script_AsLongLong(poX);

		ScriptObject* poY = ScriptDict_GetItemString(poDict, "y");
		if (poY)
			kCreateData.m_lPosX = Script_AsLongLong(poY);

		CPythonCharacterManager::Instance().CreateInstance(kCreateData);
	}
	else
	{
		CPythonCharacterManager::Instance().RegisterInstance(iVirtualID);
	}

	return Script_BuildNone();
}

ScriptObject* chrDeleteInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CPythonCharacterManager::Instance().DeleteInstance(iVirtualID);
	return Script_BuildNone();
}

ScriptObject* chrDeleteInstanceByFade(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CPythonCharacterManager::Instance().DeleteInstanceByFade(iVirtualID);
	return Script_BuildNone();
}

ScriptObject* chrSelectInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CPythonCharacterManager::Instance().SelectInstance(iVirtualID);
	return Script_BuildNone();
}

ScriptObject* chrHasInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	bool bFlag = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID) ? TRUE : FALSE;
	return Script_BuildValue("i", bFlag);
}

ScriptObject* chrIsEnemy(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (!pInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pInstance->IsEnemy());
}

ScriptObject* chrIsNPC(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (!pInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pInstance->IsNPC());
}

ScriptObject* chrIsGameMaster(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (!pInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pInstance->IsGameMaster());
}

ScriptObject* chrIsPartyMember(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (!pInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pInstance->IsPartyMember());
}

ScriptObject* chrSelect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pkInst)
		return Script_BuildNone();

	switch (CPythonNonPlayer::Instance().GetEventType(pkInst->GetVirtualNumber()))
	{
		case CPythonNonPlayer::ON_CLICK_EVENT_SHOP:
			pkInst->SetAddRenderMode();
			pkInst->SetAddColor(D3DXCOLOR(0.0f, 0.3f, 0.0f, 1.0f));
			break;

		default:
			// NOTE: 빨간색으로 나오게 하면 스샷 찍을 때 보기가 안좋아서 코멘트 하였습니다 [cronan 040226]
			// pkInst->SetAddColor(D3DXCOLOR(0.3f, 0.0f, 0.0f, 1.0f));
			break;
	}

	return Script_BuildNone();
}

ScriptObject* chrSetAddRenderMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	float fr;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fr))
		return Script_BuildException();
	float fg;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fg))
		return Script_BuildException();
	float fb;
	if (!ScriptTuple_GetFloat(poArgs, 3, &fb))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pkInst)
		return Script_BuildNone();

	pkInst->SetAddRenderMode();
	pkInst->SetAddColor(D3DXCOLOR(fr, fg, fb, 1.0f));

	return Script_BuildNone();
}

ScriptObject* chrSetBlendRenderMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	float fAlpha;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fAlpha))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pkInst)
		return Script_BuildNone();

	pkInst->SetAlpha(fAlpha);

	return Script_BuildNone();
}

ScriptObject* chrUnselect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (!pkInst)
		return Script_BuildNone();

	pkInst->RestoreRenderMode();
	return Script_BuildNone();
}

ScriptObject* chrPick(ScriptObject* poSelf, ScriptObject* poArgs)
{
	DWORD VirtualID = 0;
	if (CPythonCharacterManager::Instance().OLD_GetPickedInstanceVID(&VirtualID))
		return Script_BuildValue("i", VirtualID);
	else
		return Script_BuildValue("i", -1);
}

ScriptObject* chrHide(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->Hide();
	return Script_BuildNone();
}

ScriptObject* chrShow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->Show();
	return Script_BuildNone();
}

ScriptObject* chrPickAll(ScriptObject* poSelf, ScriptObject* poArgs)
{
	DWORD VirtualID = CPythonCharacterManager::Instance().PickAll();
	return Script_BuildValue("i", VirtualID);
}

ScriptObject* chrSetRace(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iRace;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iRace))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->SetRace(iRace);

	return Script_BuildNone();
}

ScriptObject* chrSetHair(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iRace;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iRace))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->SetHair(iRace);

	return Script_BuildNone();
}

ScriptObject* chrChangeHair(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iHair;

	if (!ScriptTuple_GetInteger(poArgs, 0, &iHair))
		return Script_BuildException();
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->ChangeHair(iHair);
	return Script_BuildNone();
}

ScriptObject* chrSetArmor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iForm;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iForm))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->SetArmor(iForm);

	pkInst->RegisterBoundingSphere();

	return Script_BuildNone();
}

ScriptObject* chrChangeShape(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iForm;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iForm))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->ChangeArmor(iForm);

	return Script_BuildNone();
}

ScriptObject* chrSetWeapon(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iForm;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iForm))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->SetWeapon(iForm);

	return Script_BuildNone();
}

ScriptObject* chrSetVirtualID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->SetVirtualID(iVID);
	return Script_BuildNone();
}

ScriptObject* chrSetNameString(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* c_szName;
	if (!ScriptTuple_GetString(poArgs, 0, &c_szName))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->SetNameString(c_szName, strlen(c_szName));
	return Script_BuildNone();
}

ScriptObject* chrSetInstanceType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iInstanceType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iInstanceType))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->SetInstanceType(iInstanceType);
	return Script_BuildNone();
}

ScriptObject* chrAttachEffectByName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iParentPartIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iParentPartIndex))
		return Script_BuildException();

	char* pszBoneName;
	if (!ScriptTuple_GetString(poArgs, 1, &pszBoneName))
	{
		pszBoneName = NULL;
		// return Script_BuildException();
	}

	char* pszEffectName;
	if (!ScriptTuple_GetString(poArgs, 2, &pszEffectName))
		return Script_BuildException();

	int iLife = CActorInstance::EFFECT_LIFE_INFINITE;

	if (ScriptTuple_Size(poArgs) == 4)
	{
		if (!ScriptTuple_GetInteger(poArgs, 3, &iLife))
			return Script_BuildException();
	}

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Script_BuildNone();

	// pkInst->AttachEffectByName(iParentPartIndex, pszBoneName, pszEffectName, iLife);
	return Script_BuildNone();
}

ScriptObject* chrAttachEffectByID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iParentPartIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iParentPartIndex))
		return Script_BuildException();

	char* pszBoneName;
	if (!ScriptTuple_GetString(poArgs, 1, &pszBoneName))
	{
		pszBoneName = NULL;
	}
	//	return Script_BuildException();

	// FIXME : bug or error on getting unsigned value
	int iEffectID;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iEffectID))
		return Script_BuildException();

	int iLife = CActorInstance::EFFECT_LIFE_INFINITE;

	if (ScriptTuple_Size(poArgs) == 4)
	{
		if (!ScriptTuple_GetInteger(poArgs, 3, &iLife))
			return Script_BuildException();
	}

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Script_BuildNone();

	// pkInst->AttachEffectByID(iParentPartIndex, pszBoneName, iEffectID, iLife);
	return Script_BuildNone();
}

ScriptObject* chrRefresh(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Script_BuildNone();

	// Select 화면에서는 WAIT 모션이 준비 되지 않은 상태이기 때문에 문제가 생긴다.
	// pkInst->Refresh(CRaceMotionData::NAME_WAIT, true);
	return Script_BuildNone();
}

ScriptObject* chrMountHorse(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->MountHorse(20030);
	return Script_BuildNone();
}

ScriptObject* chrDismountHorse(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->DismountHorse();
	return Script_BuildNone();
}

ScriptObject* chrRevive(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->Revive();
	return Script_BuildNone();
}

ScriptObject* chrDie(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->Die();
	return Script_BuildNone();
}

ScriptObject* chrLookAt(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iCellX;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iCellX))
		return Script_BuildException();

	int iCellY;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iCellY))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	// 	pkInst->LookAt(TPixelPosition(iCellX * c_Section_xAttributeCellSize, iCellY * c_Section_xAttributeCellSize));
	return Script_BuildNone();
}

ScriptObject* chrSetMotionMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionMode))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->SetMotionMode(iMotionMode);
	return Script_BuildNone();
}

ScriptObject* chrSetLoopMotion(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionIndex))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->SetLoopMotion(WORD(iMotionIndex));

	return Script_BuildNone();
}

ScriptObject* chrBlendLoopMotion(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionIndex))
		return Script_BuildException();
	float fBlendTime;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fBlendTime))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->SetLoopMotion(WORD(iMotionIndex), fBlendTime);

	return Script_BuildNone();
}

ScriptObject* chrPushOnceMotion(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionIndex))
		return Script_BuildException();

	float fBlendTime;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fBlendTime))
	{
		fBlendTime = 0.1f;
	}

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->PushOnceMotion(WORD(iMotionIndex), fBlendTime);

	return Script_BuildNone();
}

ScriptObject* chrPushLoopMotion(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionIndex))
		return Script_BuildException();

	float fBlendTime;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fBlendTime))
	{
		fBlendTime = 0.1f;
	}

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();
	pkInst->PushLoopMotion(WORD(iMotionIndex), fBlendTime);

	return Script_BuildNone();
}

ScriptObject* chrSetPixelPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iX;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iX))
		return Script_BuildException();
	int iY;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iY))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	int iZ;
	if (ScriptTuple_GetInteger(poArgs, 2, &iZ))
	{
		pkInst->NEW_SetPixelPosition(TPixelPosition(iX, iY, iZ));
	}
	else
	{
		pkInst->SCRIPT_SetPixelPosition(iX, iY);
	}
	return Script_BuildNone();
}

ScriptObject* chrSetDirection(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDirection;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDirection))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	pkInst->SetDirection(iDirection);
	return Script_BuildNone();
}

ScriptObject* chrGetPixelPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pkInst)
		return Script_BuildException();

	TPixelPosition PixelPosition;
	pkInst->NEW_GetPixelPosition(&PixelPosition);

	return Script_BuildValue("fff", PixelPosition.x, PixelPosition.y, PixelPosition.z);
}

ScriptObject* chrSetRotation(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fRotation;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fRotation))
		return Script_BuildException();

	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pCharacterInstance)
		return Script_BuildNone();

	fRotation = fmod(fRotation + 180.0f, 360.0f);

	pCharacterInstance->SetRotation(fRotation);

	return Script_BuildNone();
}

ScriptObject* chrSetRotationAll(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fRotX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fRotX))
		return Script_BuildException();
	float fRotY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fRotY))
		return Script_BuildException();
	float fRotZ;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fRotZ))
		return Script_BuildException();

	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pCharacterInstance)
		return Script_BuildNone();

	pCharacterInstance->GetGraphicThingInstanceRef().SetXYRotation(fRotX, fRotY);
	pCharacterInstance->GetGraphicThingInstanceRef().SetRotation(fRotZ);
	return Script_BuildNone();
}

ScriptObject* chrBlendRotation(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fRotation;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fRotation))
		return Script_BuildException();
	float fBlendTime;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fBlendTime))
		return Script_BuildException();

	fRotation = fmod(720.0f - fRotation, 360.0f);

	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pCharacterInstance)
		return Script_BuildNone();
	pCharacterInstance->BlendRotation(fRotation, fBlendTime);

	return Script_BuildNone();
}

ScriptObject* chrGetRotation(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pCharacterInstance)
		return Script_BuildValue("f", 0.0f);

	float fRotation = pCharacterInstance->GetRotation();

	return Script_BuildValue("f", 360.0f - fRotation);
}

ScriptObject* chrGetRace(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pCharacterInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pCharacterInstance->GetRace());
}

ScriptObject* chrGetName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pCharacterInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("s", pCharacterInstance->GetNameString());
}

ScriptObject* chrGetNameByVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pInstance)
		return Script_BuildValue("s", "None");

	return Script_BuildValue("s", pInstance->GetNameString());
}

ScriptObject* chrGetGuildID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pInstance->GetGuildID());
}

ScriptObject* chrGetProjectPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	int iHeight;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iHeight))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pInstance)
		return Script_BuildValue("ii", -100, -100);

	TPixelPosition PixelPosition;
	pInstance->NEW_GetPixelPosition(&PixelPosition);

	CPythonGraphic& rpyGraphic = CPythonGraphic::Instance();

	float fx, fy, fz;
	rpyGraphic.ProjectPosition(PixelPosition.x,
							   -PixelPosition.y,
							   PixelPosition.z + float(iHeight),
							   &fx, &fy, &fz);

	const long uiWidth = UI::CWindowManager::Instance().GetScreenWidth();
	const long uiHeight = UI::CWindowManager::Instance().GetScreenHeight();
	if (uiWidth > 0 && uiHeight > 0)
	{
		fx *= float(uiWidth) / float(rpyGraphic.GetBackBufferWidth());
		fy *= float(uiHeight) / float(rpyGraphic.GetBackBufferHeight());
	}

	if (1 == int(fz))
		return Script_BuildValue("ii", -100, -100);

	return Script_BuildValue("ii", int(fx), int(fy));
}

ScriptObject* chrGetVirtualNumber(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (NULL != pkInst)
		return Script_BuildValue("i", pkInst->GetVirtualNumber());

	return Script_BuildValue("i", CActorInstance::TYPE_PC);
}

ScriptObject* chrGetInstanceType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (NULL != pkInst)
		return Script_BuildValue("i", pkInst->GetInstanceType());

	return Script_BuildValue("i", CActorInstance::TYPE_PC);
}

ScriptObject* chrGetBoundBoxOnlyXY(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Script_BuildValue("ffff", 0.0f, 0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 v3Min, v3Max;
	pkInst->GetBoundBox(&v3Min, &v3Max);

	return Script_BuildValue("ffff", v3Min.x, v3Min.y, v3Max.x, v3Max.y);
}

ScriptObject* chrtestGetPKData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Script_BuildValue("ii", 0, 4);

	return Script_BuildValue("ii", pkInst->GetAlignment(), pkInst->GetAlignmentGrade());
}

ScriptObject* chrtestSetComboType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iComboType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iComboType))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (!pkInst)
		return Script_BuildNone();

	pkInst->GetGraphicThingInstanceRef().SetComboType(iComboType);

	return Script_BuildNone();
}

ScriptObject* chrtestSetAddRenderMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	int iColor;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iColor))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (pkInst)
	{
		pkInst->SetAddRenderMode();
		pkInst->SetAddColor(0xff000000 | iColor);
	}

	return Script_BuildNone();
}

ScriptObject* chrtestSetModulateRenderMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	int iColor;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iColor))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (pkInst)
	{
		pkInst->SetModulateRenderMode();
		pkInst->SetAddColor(0xff000000 | iColor);
	}

	return Script_BuildNone();
}

ScriptObject* chrtestSetAddRenderModeRGB(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	float fr;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fr))
		return Script_BuildException();
	float fg;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fg))
		return Script_BuildException();
	float fb;
	if (!ScriptTuple_GetFloat(poArgs, 3, &fb))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (pkInst)
	{
		pkInst->SetAddRenderMode();
		pkInst->SetAddColor(D3DXCOLOR(fr, fg, fb, 1.0f));
	}

	return Script_BuildNone();
}

ScriptObject* chrtestSetModulateRenderModeRGB(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	float fr;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fr))
		return Script_BuildException();
	float fg;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fg))
		return Script_BuildException();
	float fb;
	if (!ScriptTuple_GetFloat(poArgs, 3, &fb))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (pkInst)
	{
		pkInst->SetModulateRenderMode();
		pkInst->SetAddColor(D3DXCOLOR(fr, fg, fb, 1.0f));
	}

	return Script_BuildNone();
}

ScriptObject* chrtestSetSpecularRenderMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	int iPart;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPart))
		return Script_BuildException();
	float fAlpha;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fAlpha))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (pkInst)
	{
		pkInst->GetGraphicThingInstanceRef().SetSpecularInfo(TRUE, iPart, fAlpha);
	}

	return Script_BuildNone();
}

ScriptObject* chrtestSetSpecularRenderMode2(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	int iPart;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPart))
		return Script_BuildException();
	float fAlpha;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fAlpha))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (pkInst)
	{
		pkInst->GetGraphicThingInstanceRef().SetSpecularInfoForce(TRUE, iPart, fAlpha);
	}

	return Script_BuildNone();
}

ScriptObject* chrtestRestoreRenderMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (pkInst)
	{
		pkInst->RestoreRenderMode();
	}

	return Script_BuildNone();
}

ScriptObject* chrFaintTest(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	if (pCharacterInstance)
	{
		if (pCharacterInstance->GetGraphicThingInstanceRef().IsFaint())
		{
			pCharacterInstance->GetGraphicThingInstanceRef().SetFaint(false);
		}
		else
		{
			pCharacterInstance->GetGraphicThingInstanceRef().InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING);
			pCharacterInstance->GetGraphicThingInstanceRef().PushOnceMotion(CRaceMotionData::NAME_STAND_UP);
			pCharacterInstance->GetGraphicThingInstanceRef().PushLoopMotion(CRaceMotionData::NAME_WAIT);
			pCharacterInstance->GetGraphicThingInstanceRef().SetFaint(true);
		}
	}

	return Script_BuildNone();
}

ScriptObject* chrSetMoveSpeed(ScriptObject* poSelf, ScriptObject* poArgs)
{
	// int iSpeed;
	// if (!ScriptTuple_GetInteger(poArgs, 0, &iSpeed))
	//	return Script_BadArgument();

	// CInstanceBase * pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	// if (!pCharacterInstance)
	//	return Script_BuildValue("i", 0);

	// pCharacterInstance->SetMoveSpeed(iSpeed);

	return Script_BuildNone();
}

ScriptObject* chrSetAttackSpeed(ScriptObject* poSelf, ScriptObject* poArgs)
{
	// int iSpeed;
	// if (!ScriptTuple_GetInteger(poArgs, 0, &iSpeed))
	//	return Script_BadArgument();

	// CInstanceBase * pCharacterInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	// if (!pCharacterInstance)
	//	return Script_BuildValue("i", 0);

	// pCharacterInstance->SetAttackSpeed(iSpeed);

	return Script_BuildNone();
}

ScriptObject* chrWeaponTraceSetTexture(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szPathName;
	if (!ScriptTuple_GetString(poArgs, 0, &szPathName))
		return Script_BadArgument();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (pInstance)
	{
		pInstance->GetGraphicThingInstanceRef().SetWeaponTraceTexture(szPathName);
	}
	return Script_BuildNone();
}

ScriptObject* chrWeaponTraceUseTexture(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (pInstance)
	{
		pInstance->GetGraphicThingInstanceRef().UseTextureWeaponTrace();
	}
	return Script_BuildNone();
}

ScriptObject* chrWeaponTraceUseAlpha(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (pInstance)
	{
		pInstance->GetGraphicThingInstanceRef().UseAlphaWeaponTrace();
	}
	return Script_BuildNone();
}

ScriptObject* chrMoveToDestPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BadArgument();

	int ix;
	if (!ScriptTuple_GetInteger(poArgs, 1, &ix))
		return Script_BadArgument();
	int iy;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iy))
		return Script_BadArgument();

	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pCharacterInstance)
		return Script_BuildNone();

	pCharacterInstance->NEW_MoveToDestPixelPositionDirection(TPixelPosition(ix, iy, 0.0f));

	return Script_BuildNone();
}

ScriptObject* chrtestSetRideMan(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ix;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ix))
		return Script_BadArgument();
	int iy;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iy))
		return Script_BadArgument();
	int imount = 20030;
	ScriptTuple_GetInteger(poArgs, 2, &imount);

	CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().RegisterInstance(1);
	CInstanceBase::SCreateData kCreateData = {};
	kCreateData.m_bType = CActorInstance::TYPE_PC;
	kCreateData.m_dwRace = 0;
	kCreateData.m_dwArmor = 0;
	kCreateData.m_dwHair = 100;
	kCreateData.m_dwMovSpd = 100;
	kCreateData.m_dwAtkSpd = 100;
	kCreateData.m_dwMountVnum = imount;
	kCreateData.m_lPosX = ix;
	kCreateData.m_lPosY = iy;
	pCharacterInstance->Create(kCreateData);

	return Script_BuildNone();
}

void initchr()
{
	static ScriptMethodDef s_methods[] =
		{
			{"DismountHorse", chrDismountHorse, SCRIPT_VARARGS, NULL},
			{"MountHorse", chrMountHorse, SCRIPT_VARARGS, NULL},

			{"Destroy", chrDestroy, SCRIPT_VARARGS, NULL},
			{"Update", chrUpdate, SCRIPT_VARARGS, NULL},
			{"Deform", chrDeform, SCRIPT_VARARGS, NULL},
			{"Render", chrRender, SCRIPT_VARARGS, NULL},
			{"RenderCollision", chrRenderCollision, SCRIPT_VARARGS, NULL},

			// Functions For Python Code
			{"CreateInstance", chrCreateInstance, SCRIPT_VARARGS, NULL},
			{"DeleteInstance", chrDeleteInstance, SCRIPT_VARARGS, NULL},
			{"DeleteInstanceByFade", chrDeleteInstanceByFade, SCRIPT_VARARGS, NULL},
			{"SelectInstance", chrSelectInstance, SCRIPT_VARARGS, NULL},

			{"HasInstance", chrHasInstance, SCRIPT_VARARGS, NULL},
			{"IsEnemy", chrIsEnemy, SCRIPT_VARARGS, NULL},
			{"IsNPC", chrIsNPC, SCRIPT_VARARGS, NULL},
			{"IsGameMaster", chrIsGameMaster, SCRIPT_VARARGS, NULL},
			{"IsPartyMember", chrIsPartyMember, SCRIPT_VARARGS, NULL},

			{"Select", chrSelect, SCRIPT_VARARGS, NULL},
			{"SetAddRenderMode", chrSetAddRenderMode, SCRIPT_VARARGS, NULL},
			{"SetBlendRenderMode", chrSetBlendRenderMode, SCRIPT_VARARGS, NULL},
			{"Unselect", chrUnselect, SCRIPT_VARARGS, NULL},

			{"Hide", chrHide, SCRIPT_VARARGS, NULL},
			{"Show", chrShow, SCRIPT_VARARGS, NULL},
			{"Pick", chrPick, SCRIPT_VARARGS, NULL},
			{"PickAll", chrPickAll, SCRIPT_VARARGS, NULL},

			{"SetArmor", chrSetArmor, SCRIPT_VARARGS, NULL},
			{"SetWeapon", chrSetWeapon, SCRIPT_VARARGS, NULL},
			{"ChangeShape", chrChangeShape, SCRIPT_VARARGS, NULL},
			{"SetRace", chrSetRace, SCRIPT_VARARGS, NULL},
			{"SetHair", chrSetHair, SCRIPT_VARARGS, NULL},
			{"ChangeHair", chrChangeHair, SCRIPT_VARARGS, NULL},
			{"SetVirtualID", chrSetVirtualID, SCRIPT_VARARGS, NULL},
			{"SetNameString", chrSetNameString, SCRIPT_VARARGS, NULL},
			{"SetInstanceType", chrSetInstanceType, SCRIPT_VARARGS, NULL},

			{"SetPixelPosition", chrSetPixelPosition, SCRIPT_VARARGS, NULL},
			{"SetDirection", chrSetDirection, SCRIPT_VARARGS, NULL},
			{"Refresh", chrRefresh, SCRIPT_VARARGS, NULL},
			{"Revive", chrRevive, SCRIPT_VARARGS, NULL},
			{"Die", chrDie, SCRIPT_VARARGS, NULL},

			{"AttachEffectByID", chrAttachEffectByID, SCRIPT_VARARGS, NULL},
			{"AttachEffectByName", chrAttachEffectByName, SCRIPT_VARARGS, NULL},

			{"LookAt", chrLookAt, SCRIPT_VARARGS, NULL},
			{"SetMotionMode", chrSetMotionMode, SCRIPT_VARARGS, NULL},
			{"SetLoopMotion", chrSetLoopMotion, SCRIPT_VARARGS, NULL},
			{"BlendLoopMotion", chrBlendLoopMotion, SCRIPT_VARARGS, NULL},
			{"PushOnceMotion", chrPushOnceMotion, SCRIPT_VARARGS, NULL},
			{"PushLoopMotion", chrPushLoopMotion, SCRIPT_VARARGS, NULL},
			{"GetPixelPosition", chrGetPixelPosition, SCRIPT_VARARGS, NULL},
			{"SetRotation", chrSetRotation, SCRIPT_VARARGS, NULL},
			{"SetRotationAll", chrSetRotationAll, SCRIPT_VARARGS, NULL},
			{"BlendRotation", chrBlendRotation, SCRIPT_VARARGS, NULL},
			{"GetRotation", chrGetRotation, SCRIPT_VARARGS, NULL},
			{"GetRace", chrGetRace, SCRIPT_VARARGS, NULL},
			{"GetName", chrGetName, SCRIPT_VARARGS, NULL},
			{"GetNameByVID", chrGetNameByVID, SCRIPT_VARARGS, NULL},
			{"GetGuildID", chrGetGuildID, SCRIPT_VARARGS, NULL},
			{"GetProjectPosition", chrGetProjectPosition, SCRIPT_VARARGS, NULL},

			{"GetVirtualNumber", chrGetVirtualNumber, SCRIPT_VARARGS, NULL},
			{"GetInstanceType", chrGetInstanceType, SCRIPT_VARARGS, NULL},

			{"GetBoundBoxOnlyXY", chrGetBoundBoxOnlyXY, SCRIPT_VARARGS, NULL},

			{"RaceToJob", chrRaceToJob, SCRIPT_VARARGS, NULL},
			{"RaceToSex", chrRaceToSex, SCRIPT_VARARGS, NULL},

			// For Test
			{"testGetPKData", chrtestGetPKData, SCRIPT_VARARGS, NULL},
			{"FaintTest", chrFaintTest, SCRIPT_VARARGS, NULL},
			{"SetMoveSpeed", chrSetMoveSpeed, SCRIPT_VARARGS, NULL},
			{"SetAttackSpeed", chrSetAttackSpeed, SCRIPT_VARARGS, NULL},
			{"WeaponTraceSetTexture", chrWeaponTraceSetTexture, SCRIPT_VARARGS, NULL},
			{"WeaponTraceUseAlpha", chrWeaponTraceUseAlpha, SCRIPT_VARARGS, NULL},
			{"WeaponTraceUseTexture", chrWeaponTraceUseTexture, SCRIPT_VARARGS, NULL},
			{"MoveToDestPosition", chrMoveToDestPosition, SCRIPT_VARARGS, NULL},
			{"testSetComboType", chrtestSetComboType, SCRIPT_VARARGS, NULL},
			{"testSetAddRenderMode", chrtestSetAddRenderMode, SCRIPT_VARARGS, NULL},
			{"testSetModulateRenderMode", chrtestSetModulateRenderMode, SCRIPT_VARARGS, NULL},
			{"testSetAddRenderModeRGB", chrtestSetAddRenderModeRGB, SCRIPT_VARARGS, NULL},
			{"testSetModulateRenderModeRGB", chrtestSetModulateRenderModeRGB, SCRIPT_VARARGS, NULL},
			{"testSetSpecularRenderMode", chrtestSetSpecularRenderMode, SCRIPT_VARARGS, NULL},
			{"testSetSpecularRenderMode2", chrtestSetSpecularRenderMode2, SCRIPT_VARARGS, NULL},
			{"testRestoreRenderMode", chrtestRestoreRenderMode, SCRIPT_VARARGS, NULL},
			{"testSetRideMan", chrtestSetRideMan, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("chr", s_methods);

	// Length
	ScriptModule_AddIntConstant(poModule, "PLAYER_NAME_MAX_LEN", PLAYER_NAME_MAX_LEN);

	// General
	ScriptModule_AddIntConstant(poModule, "MOTION_NONE", CRaceMotionData::NAME_NONE);

	ScriptModule_AddIntConstant(poModule, "MOTION_SPAWN", CRaceMotionData::NAME_SPAWN);
	ScriptModule_AddIntConstant(poModule, "MOTION_WAIT", CRaceMotionData::NAME_WAIT);
	ScriptModule_AddIntConstant(poModule, "MOTION_WALK", CRaceMotionData::NAME_WALK);
	ScriptModule_AddIntConstant(poModule, "MOTION_RUN", CRaceMotionData::NAME_RUN);
	ScriptModule_AddIntConstant(poModule, "MOTION_CHANGE_WEAPON", CRaceMotionData::NAME_CHANGE_WEAPON);
	ScriptModule_AddIntConstant(poModule, "MOTION_DAMAGE", CRaceMotionData::NAME_DAMAGE);
	ScriptModule_AddIntConstant(poModule, "MOTION_DAMAGE_FLYING", CRaceMotionData::NAME_DAMAGE_FLYING);
	ScriptModule_AddIntConstant(poModule, "MOTION_STAND_UP", CRaceMotionData::NAME_STAND_UP);
	ScriptModule_AddIntConstant(poModule, "MOTION_DAMAGE_BACK", CRaceMotionData::NAME_DAMAGE_BACK);
	ScriptModule_AddIntConstant(poModule, "MOTION_DAMAGE_FLYING_BACK", CRaceMotionData::NAME_DAMAGE_FLYING_BACK);
	ScriptModule_AddIntConstant(poModule, "MOTION_STAND_UP_BACK", CRaceMotionData::NAME_STAND_UP_BACK);
	ScriptModule_AddIntConstant(poModule, "MOTION_DEAD", CRaceMotionData::NAME_DEAD);
	ScriptModule_AddIntConstant(poModule, "MOTION_DEAD_BACK", CRaceMotionData::NAME_DEAD_BACK);
	ScriptModule_AddIntConstant(poModule, "MOTION_NORMAL_ATTACK", CRaceMotionData::NAME_NORMAL_ATTACK);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_1", CRaceMotionData::NAME_COMBO_ATTACK_1);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_2", CRaceMotionData::NAME_COMBO_ATTACK_2);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_3", CRaceMotionData::NAME_COMBO_ATTACK_3);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_4", CRaceMotionData::NAME_COMBO_ATTACK_4);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_5", CRaceMotionData::NAME_COMBO_ATTACK_5);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_6", CRaceMotionData::NAME_COMBO_ATTACK_6);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_7", CRaceMotionData::NAME_COMBO_ATTACK_7);
	ScriptModule_AddIntConstant(poModule, "MOTION_COMBO_ATTACK_8", CRaceMotionData::NAME_COMBO_ATTACK_8);
	ScriptModule_AddIntConstant(poModule, "MOTION_INTRO_WAIT", CRaceMotionData::NAME_INTRO_WAIT);
	ScriptModule_AddIntConstant(poModule, "MOTION_INTRO_SELECTED", CRaceMotionData::NAME_INTRO_SELECTED);
	ScriptModule_AddIntConstant(poModule, "MOTION_INTRO_NOT_SELECTED", CRaceMotionData::NAME_INTRO_NOT_SELECTED);
	ScriptModule_AddIntConstant(poModule, "MOTION_FISHING_THROW", CRaceMotionData::NAME_FISHING_THROW);
	ScriptModule_AddIntConstant(poModule, "MOTION_FISHING_WAIT", CRaceMotionData::NAME_FISHING_WAIT);
	ScriptModule_AddIntConstant(poModule, "MOTION_FISHING_STOP", CRaceMotionData::NAME_FISHING_STOP);
	ScriptModule_AddIntConstant(poModule, "MOTION_FISHING_REACT", CRaceMotionData::NAME_FISHING_REACT);
	ScriptModule_AddIntConstant(poModule, "MOTION_FISHING_CATCH", CRaceMotionData::NAME_FISHING_CATCH);
	ScriptModule_AddIntConstant(poModule, "MOTION_FISHING_FAIL", CRaceMotionData::NAME_FISHING_FAIL);
	ScriptModule_AddIntConstant(poModule, "MOTION_STOP", CRaceMotionData::NAME_STOP);
	ScriptModule_AddIntConstant(poModule, "MOTION_SKILL", CRaceMotionData::NAME_SKILL);
	ScriptModule_AddIntConstant(poModule, "MOTION_CLAP", CRaceMotionData::NAME_CLAP);
	ScriptModule_AddIntConstant(poModule, "MOTION_DANCE_1", CRaceMotionData::NAME_DANCE_1);
	ScriptModule_AddIntConstant(poModule, "MOTION_DANCE_2", CRaceMotionData::NAME_DANCE_2);

	ScriptModule_AddIntConstant(poModule, "MOTION_DANCE_3", CRaceMotionData::NAME_DANCE_3);
	ScriptModule_AddIntConstant(poModule, "MOTION_DANCE_4", CRaceMotionData::NAME_DANCE_4);
	ScriptModule_AddIntConstant(poModule, "MOTION_DANCE_5", CRaceMotionData::NAME_DANCE_5);
	ScriptModule_AddIntConstant(poModule, "MOTION_DANCE_6", CRaceMotionData::NAME_DANCE_6);
	ScriptModule_AddIntConstant(poModule, "MOTION_CONGRATULATION", CRaceMotionData::NAME_CONGRATULATION);
	ScriptModule_AddIntConstant(poModule, "MOTION_FORGIVE", CRaceMotionData::NAME_FORGIVE);
	ScriptModule_AddIntConstant(poModule, "MOTION_ANGRY", CRaceMotionData::NAME_ANGRY);
	ScriptModule_AddIntConstant(poModule, "MOTION_ATTRACTIVE", CRaceMotionData::NAME_ATTRACTIVE);
	ScriptModule_AddIntConstant(poModule, "MOTION_SAD", CRaceMotionData::NAME_SAD);
	ScriptModule_AddIntConstant(poModule, "MOTION_SHY", CRaceMotionData::NAME_SHY);
	ScriptModule_AddIntConstant(poModule, "MOTION_CHEERUP", CRaceMotionData::NAME_CHEERUP);
	ScriptModule_AddIntConstant(poModule, "MOTION_BANTER", CRaceMotionData::NAME_BANTER);
	ScriptModule_AddIntConstant(poModule, "MOTION_JOY", CRaceMotionData::NAME_JOY);

	ScriptModule_AddIntConstant(poModule, "MOTION_CHEERS_1", CRaceMotionData::NAME_CHEERS_1);
	ScriptModule_AddIntConstant(poModule, "MOTION_CHEERS_2", CRaceMotionData::NAME_CHEERS_2);
	ScriptModule_AddIntConstant(poModule, "MOTION_KISS_WITH_WARRIOR", CRaceMotionData::NAME_KISS_WITH_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "MOTION_KISS_WITH_ASSASSIN", CRaceMotionData::NAME_KISS_WITH_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "MOTION_KISS_WITH_SURA", CRaceMotionData::NAME_KISS_WITH_SURA);
	ScriptModule_AddIntConstant(poModule, "MOTION_KISS_WITH_SHAMAN", CRaceMotionData::NAME_KISS_WITH_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "MOTION_FRENCH_KISS_WITH_WARRIOR", CRaceMotionData::NAME_FRENCH_KISS_WITH_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "MOTION_FRENCH_KISS_WITH_ASSASSIN", CRaceMotionData::NAME_FRENCH_KISS_WITH_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "MOTION_FRENCH_KISS_WITH_SURA", CRaceMotionData::NAME_FRENCH_KISS_WITH_SURA);
	ScriptModule_AddIntConstant(poModule, "MOTION_FRENCH_KISS_WITH_SHAMAN", CRaceMotionData::NAME_FRENCH_KISS_WITH_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HIT_WITH_WARRIOR", CRaceMotionData::NAME_SLAP_HIT_WITH_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HIT_WITH_ASSASSIN", CRaceMotionData::NAME_SLAP_HIT_WITH_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HIT_WITH_SURA", CRaceMotionData::NAME_SLAP_HIT_WITH_SURA);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HIT_WITH_SHAMAN", CRaceMotionData::NAME_SLAP_HIT_WITH_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HURT_WITH_WARRIOR", CRaceMotionData::NAME_SLAP_HURT_WITH_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HURT_WITH_ASSASSIN", CRaceMotionData::NAME_SLAP_HURT_WITH_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HURT_WITH_SURA", CRaceMotionData::NAME_SLAP_HURT_WITH_SURA);
	ScriptModule_AddIntConstant(poModule, "MOTION_SLAP_HURT_WITH_SHAMAN", CRaceMotionData::NAME_SLAP_HURT_WITH_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "MOTION_DIG", CRaceMotionData::NAME_DIG);

	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_RESERVED", CRaceMotionData::MODE_RESERVED);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_GENERAL", CRaceMotionData::MODE_GENERAL);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_ONEHAND_SWORD", CRaceMotionData::MODE_ONEHAND_SWORD);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_TWOHAND_SWORD", CRaceMotionData::MODE_TWOHAND_SWORD);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_DUALHAND_SWORD", CRaceMotionData::MODE_DUALHAND_SWORD);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_BOW", CRaceMotionData::MODE_BOW);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_FAN", CRaceMotionData::MODE_FAN);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_BELL", CRaceMotionData::MODE_BELL);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_FISHING", CRaceMotionData::MODE_FISHING);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_HORSE", CRaceMotionData::MODE_HORSE);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_HORSE_ONEHAND_SWORD", CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_HORSE_TWOHAND_SWORD", CRaceMotionData::MODE_HORSE_TWOHAND_SWORD);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_HORSE_DUALHAND_SWORD", CRaceMotionData::MODE_HORSE_DUALHAND_SWORD);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_HORSE_BOW", CRaceMotionData::MODE_HORSE_BOW);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_HORSE_FAN", CRaceMotionData::MODE_HORSE_FAN);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_HORSE_BELL", CRaceMotionData::MODE_HORSE_BELL);
	ScriptModule_AddIntConstant(poModule, "MOTION_MODE_WEDDING_DRESS", CRaceMotionData::MODE_WEDDING_DRESS);

	ScriptModule_AddIntConstant(poModule, "DIR_NORTH", CInstanceBase::DIR_NORTH);
	ScriptModule_AddIntConstant(poModule, "DIR_NORTHEAST", CInstanceBase::DIR_NORTHEAST);
	ScriptModule_AddIntConstant(poModule, "DIR_EAST", CInstanceBase::DIR_EAST);
	ScriptModule_AddIntConstant(poModule, "DIR_SOUTHEAST", CInstanceBase::DIR_SOUTHEAST);
	ScriptModule_AddIntConstant(poModule, "DIR_SOUTH", CInstanceBase::DIR_SOUTH);
	ScriptModule_AddIntConstant(poModule, "DIR_SOUTHWEST", CInstanceBase::DIR_SOUTHWEST);
	ScriptModule_AddIntConstant(poModule, "DIR_WEST", CInstanceBase::DIR_WEST);
	ScriptModule_AddIntConstant(poModule, "DIR_NORTHWEST", CInstanceBase::DIR_NORTHWEST);

	ScriptModule_AddIntConstant(poModule, "INSTANCE_TYPE_PLAYER", CActorInstance::TYPE_PC);
	ScriptModule_AddIntConstant(poModule, "INSTANCE_TYPE_NPC", CActorInstance::TYPE_NPC);
	ScriptModule_AddIntConstant(poModule, "INSTANCE_TYPE_ENEMY", CActorInstance::TYPE_ENEMY);
	ScriptModule_AddIntConstant(poModule, "INSTANCE_TYPE_BUILDING", CActorInstance::TYPE_BUILDING);
	ScriptModule_AddIntConstant(poModule, "INSTANCE_TYPE_OBJECT", CActorInstance::TYPE_OBJECT);

	ScriptModule_AddIntConstant(poModule, "PART_WEAPON", CRaceData::PART_WEAPON);
	ScriptModule_AddIntConstant(poModule, "PART_HEAD", CRaceData::PART_HEAD);
	ScriptModule_AddIntConstant(poModule, "PART_WEAPON_LEFT", CRaceData::PART_WEAPON_LEFT);

	/////

	ScriptModule_AddIntConstant(poModule, "AFFECT_POISON", CInstanceBase::AFFECT_POISON);
	ScriptModule_AddIntConstant(poModule, "AFFECT_SLOW", CInstanceBase::AFFECT_SLOW);
	ScriptModule_AddIntConstant(poModule, "AFFECT_STUN", CInstanceBase::AFFECT_STUN);
	ScriptModule_AddIntConstant(poModule, "AFFECT_MOV_SPEED_POTION", CInstanceBase::AFFECT_MOV_SPEED_POTION);
	ScriptModule_AddIntConstant(poModule, "AFFECT_ATT_SPEED_POTION", CInstanceBase::AFFECT_ATT_SPEED_POTION);
	ScriptModule_AddIntConstant(poModule, "AFFECT_FISH_MIND", CInstanceBase::AFFECT_FISH_MIND);

	ScriptModule_AddIntConstant(poModule, "AFFECT_JEONGWI", CInstanceBase::AFFECT_JEONGWI);
	ScriptModule_AddIntConstant(poModule, "AFFECT_GEOMGYEONG", CInstanceBase::AFFECT_GEOMGYEONG);
	ScriptModule_AddIntConstant(poModule, "AFFECT_CHEONGEUN", CInstanceBase::AFFECT_CHEONGEUN);
	ScriptModule_AddIntConstant(poModule, "AFFECT_GYEONGGONG", CInstanceBase::AFFECT_GYEONGGONG);
	ScriptModule_AddIntConstant(poModule, "AFFECT_EUNHYEONG", CInstanceBase::AFFECT_EUNHYEONG);
	ScriptModule_AddIntConstant(poModule, "AFFECT_GWIGEOM", CInstanceBase::AFFECT_GWIGEOM);
	ScriptModule_AddIntConstant(poModule, "AFFECT_GONGPO", CInstanceBase::AFFECT_GONGPO);
	ScriptModule_AddIntConstant(poModule, "AFFECT_JUMAGAP", CInstanceBase::AFFECT_JUMAGAP);
	ScriptModule_AddIntConstant(poModule, "AFFECT_HOSIN", CInstanceBase::AFFECT_HOSIN);
	ScriptModule_AddIntConstant(poModule, "AFFECT_BOHO", CInstanceBase::AFFECT_BOHO);
	ScriptModule_AddIntConstant(poModule, "AFFECT_KWAESOK", CInstanceBase::AFFECT_KWAESOK);
	ScriptModule_AddIntConstant(poModule, "AFFECT_HEUKSIN", CInstanceBase::AFFECT_HEUKSIN);
	ScriptModule_AddIntConstant(poModule, "AFFECT_MUYEONG", CInstanceBase::AFFECT_MUYEONG);
	ScriptModule_AddIntConstant(poModule, "AFFECT_GICHEON", CInstanceBase::AFFECT_GICHEON);
	ScriptModule_AddIntConstant(poModule, "AFFECT_JEUNGRYEOK", CInstanceBase::AFFECT_JEUNGRYEOK);
	ScriptModule_AddIntConstant(poModule, "AFFECT_PABEOP", CInstanceBase::AFFECT_PABEOP);
	ScriptModule_AddIntConstant(poModule, "AFFECT_FALLEN_CHEONGEUN", CInstanceBase::AFFECT_FALLEN_CHEONGEUN);
	ScriptModule_AddIntConstant(poModule, "AFFECT_CHINA_FIREWORK", CInstanceBase::AFFECT_CHINA_FIREWORK);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_MALL", CInstanceBase::NEW_AFFECT_MALL);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_NO_DEATH_PENALTY", CInstanceBase::NEW_AFFECT_NO_DEATH_PENALTY);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_SKILL_BOOK_BONUS", CInstanceBase::NEW_AFFECT_SKILL_BOOK_BONUS);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_SKILL_BOOK_NO_DELAY", CInstanceBase::NEW_AFFECT_SKILL_BOOK_NO_DELAY);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_EXP_BONUS", CInstanceBase::NEW_AFFECT_EXP_BONUS);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_EXP_BONUS_EURO_FREE", CInstanceBase::NEW_AFFECT_EXP_BONUS_EURO_FREE);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_EXP_BONUS_EURO_FREE_UNDER_15", CInstanceBase::NEW_AFFECT_EXP_BONUS_EURO_FREE_UNDER_15);

	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_ITEM_BONUS", CInstanceBase::NEW_AFFECT_ITEM_BONUS);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_SAFEBOX", CInstanceBase::NEW_AFFECT_SAFEBOX);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_AUTOLOOT", CInstanceBase::NEW_AFFECT_AUTOLOOT);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_FISH_MIND", CInstanceBase::NEW_AFFECT_FISH_MIND);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_MARRIAGE_FAST", CInstanceBase::NEW_AFFECT_MARRIAGE_FAST);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_GOLD_BONUS", CInstanceBase::NEW_AFFECT_GOLD_BONUS);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_AUTO_HP_RECOVERY", CInstanceBase::NEW_AFFECT_AUTO_HP_RECOVERY);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_AUTO_SP_RECOVERY", CInstanceBase::NEW_AFFECT_AUTO_SP_RECOVERY);

	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_DRAGON_SOUL_QUALIFIED", CInstanceBase::NEW_AFFECT_DRAGON_SOUL_QUALIFIED);

	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_DRAGON_SOUL_DECK1", CInstanceBase::NEW_AFFECT_DRAGON_SOUL_DECK1);
	ScriptModule_AddIntConstant(poModule, "NEW_AFFECT_DRAGON_SOUL_DECK2", CInstanceBase::NEW_AFFECT_DRAGON_SOUL_DECK2);
}
