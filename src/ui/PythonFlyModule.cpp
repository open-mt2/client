#include <Client/UI/StdAfx.h>
#include <Client/Game/FlyingObjectManager.h>
ScriptObject* flyUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CFlyingManager::Instance().Update();
	return Script_BuildNone();
}

ScriptObject* flyRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CFlyingManager::Instance().Render();
	return Script_BuildNone();
}

void initfly()
{
	static ScriptMethodDef s_methods[] =
		{
			{"Update", flyUpdate, SCRIPT_VARARGS, NULL},
			{"Render", flyRender, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptModule_Create("fly", s_methods);
}