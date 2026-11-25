#include <Client/UI/StdAfx.h>
#include <Client/Game/GameEventManager.h>

ScriptObject* eventMgrUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fx;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fx))
		return Script_BuildException();
	float fy;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fy))
		return Script_BuildException();
	float fz;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fz))
		return Script_BuildException();

	CGameEventManager::Instance().SetCenterPosition(fx, fy, fz);
	CGameEventManager::Instance().Update();
	return Script_BuildNone();
}

void initeventmgr()
{
	static ScriptMethodDef s_methods[] =
		{
			{"Update", eventMgrUpdate, SCRIPT_VARARGS, NULL},
			{NULL, NULL, 0, NULL},
		};

	ScriptModule_Create("eventMgr", s_methods);
}