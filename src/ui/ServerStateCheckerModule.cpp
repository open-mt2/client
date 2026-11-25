#include <Client/UI/StdAfx.h>
#include <Client/UI/ServerStateChecker.h>

ScriptObject* ServerStateCheckerCreate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poWnd;
	if (!ScriptTuple_GetObject(poArgs, 0, &poWnd))
		return Script_BadArgument();

	CServerStateChecker::Instance().Create(poWnd);
	return Script_BuildNone();
}

ScriptObject* ServerStateCheckerUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CServerStateChecker::Instance().Update();
	return Script_BuildNone();
}

ScriptObject* ServerStateCheckerRequest(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CServerStateChecker::Instance().Request();
	return Script_BuildNone();
}

ScriptObject* ServerStateCheckerAddChannel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nServerIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nServerIndex))
		return Script_BuildException();

	char* szAddr;
	if (!ScriptTuple_GetString(poArgs, 1, &szAddr))
		return Script_BuildException();

	int nPort;
	if (!ScriptTuple_GetInteger(poArgs, 2, &nPort))
		return Script_BuildException();

	CServerStateChecker::Instance().AddChannel(nServerIndex, szAddr, nPort);
	return Script_BuildNone();
}

ScriptObject* ServerStateCheckerInitialize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CServerStateChecker::Instance().Initialize();
	return Script_BuildNone();
}

void initServerStateChecker()
{
	static ScriptMethodDef s_methods[] =
		{
			{"Create", ServerStateCheckerCreate, SCRIPT_VARARGS, NULL},
			{"Update", ServerStateCheckerUpdate, SCRIPT_VARARGS, NULL},
			{"Request", ServerStateCheckerRequest, SCRIPT_VARARGS, NULL},
			{"AddChannel", ServerStateCheckerAddChannel, SCRIPT_VARARGS, NULL},
			{"Initialize", ServerStateCheckerInitialize, SCRIPT_VARARGS, NULL},
			{NULL, NULL, 0, NULL},
		};

	// ScriptObject * poModule = ScriptModule_Create("ServerStateChecker", s_methods);
	ScriptModule_Create("ServerStateChecker", s_methods);
}