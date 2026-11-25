#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonNonPlayer.h>

#include <Client/UI/InstanceBase.h>
#include <Client/UI/PythonCharacterManager.h>

ScriptObject* nonplayerGetEventType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualNumber;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualNumber))
		return Script_BuildException();

	BYTE iType = CPythonNonPlayer::Instance().GetEventType(iVirtualNumber);

	return Script_BuildValue("i", iType);
}

ScriptObject* nonplayerGetEventTypeByVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	BYTE iType = CPythonNonPlayer::Instance().GetEventTypeByVID(iVirtualID);

	return Script_BuildValue("i", iType);
}

ScriptObject* nonplayerGetLevelByVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pInstance)
		return Script_BuildValue("i", -1);

	const CPythonNonPlayer::TMobTable* pMobTable = CPythonNonPlayer::Instance().GetTable(pInstance->GetVirtualNumber());

	if (!pMobTable)
		return Script_BuildValue("i", -1);

	float fAverageLevel = pMobTable->bLevel; //(float(pMobTable->abLevelRange[0]) + float(pMobTable->abLevelRange[1])) / 2.0f;
	fAverageLevel = floor(fAverageLevel + 0.5f);
	return Script_BuildValue("i", int(fAverageLevel));
}

ScriptObject* nonplayerGetGradeByVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pInstance)
		return Script_BuildValue("i", -1);

	const CPythonNonPlayer::TMobTable* pMobTable = CPythonNonPlayer::Instance().GetTable(pInstance->GetVirtualNumber());

	if (!pMobTable)
		return Script_BuildValue("i", -1);

	return Script_BuildValue("i", pMobTable->bRank);
}

ScriptObject* nonplayerGetMonsterName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVNum;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVNum))
		return Script_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	return Script_BuildValue("s", rkNonPlayer.GetMonsterName(iVNum));
}

ScriptObject* nonplayerLoadNonPlayerData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CPythonNonPlayer::Instance().LoadNonPlayerData(szFileName);
	return Script_BuildNone();
}

void initNonPlayer()
{
	static ScriptMethodDef s_methods[] =
		{
			{"GetEventType", nonplayerGetEventType, SCRIPT_VARARGS, NULL},
			{"GetEventTypeByVID", nonplayerGetEventTypeByVID, SCRIPT_VARARGS, NULL},
			{"GetLevelByVID", nonplayerGetLevelByVID, SCRIPT_VARARGS, NULL},
			{"GetGradeByVID", nonplayerGetGradeByVID, SCRIPT_VARARGS, NULL},
			{"GetMonsterName", nonplayerGetMonsterName, SCRIPT_VARARGS, NULL},

			{"LoadNonPlayerData", nonplayerLoadNonPlayerData, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("nonplayer", s_methods);

	ScriptModule_AddIntConstant(poModule, "ON_CLICK_EVENT_NONE", CPythonNonPlayer::ON_CLICK_EVENT_NONE);
	ScriptModule_AddIntConstant(poModule, "ON_CLICK_EVENT_BATTLE", CPythonNonPlayer::ON_CLICK_EVENT_BATTLE);
	ScriptModule_AddIntConstant(poModule, "ON_CLICK_EVENT_SHOP", CPythonNonPlayer::ON_CLICK_EVENT_SHOP);
	ScriptModule_AddIntConstant(poModule, "ON_CLICK_EVENT_TALK", CPythonNonPlayer::ON_CLICK_EVENT_TALK);
	ScriptModule_AddIntConstant(poModule, "ON_CLICK_EVENT_VEHICLE", CPythonNonPlayer::ON_CLICK_EVENT_VEHICLE);

	ScriptModule_AddIntConstant(poModule, "PAWN", 0);
	ScriptModule_AddIntConstant(poModule, "S_PAWN", 1);
	ScriptModule_AddIntConstant(poModule, "KNIGHT", 2);
	ScriptModule_AddIntConstant(poModule, "S_KNIGHT", 3);
	ScriptModule_AddIntConstant(poModule, "BOSS", 4);
	ScriptModule_AddIntConstant(poModule, "KING", 5);
}