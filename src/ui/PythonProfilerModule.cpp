#include <Client/UI/StdAfx.h>
#include <Neoeter/Render/Profiler.h>

ScriptObject* profilerPush(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	return Script_BuildNone();
}

ScriptObject* profilerPop(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	return Script_BuildNone();
}

void initProfiler()
{
	static ScriptMethodDef s_methods[] =
		{
			{"Push", profilerPush, SCRIPT_VARARGS, NULL},
			{"Pop", profilerPop, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptModule_Create("profiler", s_methods);
}