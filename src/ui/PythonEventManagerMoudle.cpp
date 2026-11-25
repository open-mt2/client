#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonEventManager.h>
#include <Client/UI/PythonNetworkStream.h>

ScriptObject* eventRegisterEventSet(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	int iEventIndex = CPythonEventManager::Instance().RegisterEventSet(szFileName);
	return Script_BuildValue("i", iEventIndex);
}

ScriptObject* eventRegisterEventSetFromString(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szEventString;
	if (!ScriptTuple_GetString(poArgs, 0, &szEventString))
		return Script_BuildException();

	int iEventIndex = CPythonEventManager::Instance().RegisterEventSetFromString(szEventString);
	return Script_BuildValue("i", iEventIndex);
}

ScriptObject* eventClearEventSet(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonEventManager::Instance().ClearEventSeti(iIndex);
	return Script_BuildNone();
}

ScriptObject* eventSetRestrictedCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	int iCount;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iCount))
		return Script_BuildException();

	CPythonEventManager::Instance().SetRestrictedCount(iIndex, iCount);
	return Script_BuildNone();
}

ScriptObject* eventGetEventSetLocalYPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonEventManager::Instance().GetEventSetLocalYPosition(iIndex));
}

ScriptObject* eventAddEventSetLocalYPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	int iPos;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPos))
		return Script_BuildException();

	CPythonEventManager::Instance().AddEventSetLocalYPosition(iIndex, iPos);
	return Script_BuildNone();
}

ScriptObject* eventInsertText(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	char* szText;
	if (!ScriptTuple_GetString(poArgs, 1, &szText))
		return Script_BuildException();

	CPythonEventManager::Instance().InsertText(iIndex, szText);
	return Script_BuildNone();
}

ScriptObject* eventInsertTextInline(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	char* szText;
	if (!ScriptTuple_GetString(poArgs, 1, &szText))
		return Script_BuildException();
	int iXIndex;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iXIndex))
		return Script_BuildException();

	CPythonEventManager::Instance().InsertText(iIndex, szText, iXIndex);
	return Script_BuildNone();
}

ScriptObject* eventUpdateEventSet(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	int ix;
	if (!ScriptTuple_GetInteger(poArgs, 1, &ix))
		return Script_BuildException();

	int iy;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iy))
		return Script_BuildException();

	CPythonEventManager::Instance().UpdateEventSet(iIndex, ix, -iy);
	return Script_BuildNone();
}

ScriptObject* eventRenderEventSet(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonEventManager::Instance().RenderEventSet(iIndex);
	return Script_BuildNone();
}

ScriptObject* eventSetEventSetWidth(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	int iWidth;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iWidth))
		return Script_BuildException();

	CPythonEventManager::Instance().SetEventSetWidth(iIndex, iWidth);
	return Script_BuildNone();
}

ScriptObject* eventSkip(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonEventManager::Instance().Skip(iIndex);
	return Script_BuildNone();
}

ScriptObject* eventIsWait(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonEventManager::Instance().IsWait(iIndex) == true ? 1 : 0);
}

ScriptObject* eventEndEventProcess(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonEventManager::Instance().EndEventProcess(iIndex);

	return Script_BuildNone();
}

ScriptObject* eventSetEventHandler(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	ScriptObject* poEventHandler;
	if (!ScriptTuple_GetObject(poArgs, 1, &poEventHandler))
		return Script_BuildException();

	CPythonEventManager::Instance().SetEventHandler(iIndex, poEventHandler);
	return Script_BuildNone();
}

ScriptObject* eventSelectAnswer(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	int iAnswer;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAnswer))
		return Script_BuildException();

	CPythonEventManager::Instance().SelectAnswer(iIndex, iAnswer);
	return Script_BuildNone();
}

ScriptObject* eventGetLineCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	int iLineCount = CPythonEventManager::Instance().GetLineCount(iIndex);
	return Script_BuildValue("i", iLineCount);
}

ScriptObject* eventSetVisibleStartLine(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	int iStartLine;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iStartLine))
		return Script_BuildException();

	CPythonEventManager::Instance().SetVisibleStartLine(iIndex, iStartLine);
	return Script_BuildNone();
}

ScriptObject* eventGetVisibleStartLine(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonEventManager::Instance().GetVisibleStartLine(iIndex));
}

ScriptObject* eventQuestButtonClick(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonNetworkStream::Instance().SendScriptButtonPacket(iIndex);

	return Script_BuildNone();
}

ScriptObject* eventSetInterfaceWindow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* pyHandle;
	if (!ScriptTuple_GetObject(poArgs, 0, &pyHandle))
		return Script_BadArgument();

	CPythonEventManager& rpem = CPythonEventManager::Instance();
	rpem.SetInterfaceWindow(pyHandle);
	return Script_BuildNone();
}

ScriptObject* eventSetLeftTimeString(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szText;
	if (!ScriptTuple_GetString(poArgs, 0, &szText))
		return Script_BuildException();

	CPythonEventManager& rpem = CPythonEventManager::Instance();
	rpem.SetLeftTimeString(szText);
	return Script_BuildNone();
}

ScriptObject* eventDestroy(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonEventManager& rpem = CPythonEventManager::Instance();
	rpem.Destroy();
	return Script_BuildNone();
}

void initEvent()
{
	static ScriptMethodDef s_methods[] =
		{
			{"RegisterEventSet", eventRegisterEventSet, SCRIPT_VARARGS, NULL},
			{"RegisterEventSetFromString", eventRegisterEventSetFromString, SCRIPT_VARARGS, NULL},
			{"ClearEventSet", eventClearEventSet, SCRIPT_VARARGS, NULL},

			{"SetRestrictedCount", eventSetRestrictedCount, SCRIPT_VARARGS, NULL},

			{"GetEventSetLocalYPosition", eventGetEventSetLocalYPosition, SCRIPT_VARARGS, NULL},
			{"AddEventSetLocalYPosition", eventAddEventSetLocalYPosition, SCRIPT_VARARGS, NULL},
			{"InsertText", eventInsertText, SCRIPT_VARARGS, NULL},
			{"InsertTextInline", eventInsertTextInline, SCRIPT_VARARGS, NULL},

			{"UpdateEventSet", eventUpdateEventSet, SCRIPT_VARARGS, NULL},
			{"RenderEventSet", eventRenderEventSet, SCRIPT_VARARGS, NULL},
			{"SetEventSetWidth", eventSetEventSetWidth, SCRIPT_VARARGS, NULL},

			{"Skip", eventSkip, SCRIPT_VARARGS, NULL},
			{"IsWait", eventIsWait, SCRIPT_VARARGS, NULL},
			{"EndEventProcess", eventEndEventProcess, SCRIPT_VARARGS, NULL},

			{"SelectAnswer", eventSelectAnswer, SCRIPT_VARARGS, NULL},
			{"GetLineCount", eventGetLineCount, SCRIPT_VARARGS, NULL},
			{"SetVisibleStartLine", eventSetVisibleStartLine, SCRIPT_VARARGS, NULL},
			{"GetVisibleStartLine", eventGetVisibleStartLine, SCRIPT_VARARGS, NULL},

			{"SetEventHandler", eventSetEventHandler, SCRIPT_VARARGS, NULL},
			{"SetInterfaceWindow", eventSetInterfaceWindow, SCRIPT_VARARGS, NULL},
			{"SetLeftTimeString", eventSetLeftTimeString, SCRIPT_VARARGS, NULL},

			{"QuestButtonClick", eventQuestButtonClick, SCRIPT_VARARGS, NULL},
			{"Destroy", eventDestroy, SCRIPT_VARARGS, NULL},
			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("event", s_methods);

	ScriptModule_AddIntConstant(poModule, "BOX_VISIBLE_LINE_COUNT", CPythonEventManager::BOX_VISIBLE_LINE_COUNT);
	ScriptModule_AddIntConstant(poModule, "BUTTON_TYPE_NEXT", CPythonEventManager::BUTTON_TYPE_NEXT);
	ScriptModule_AddIntConstant(poModule, "BUTTON_TYPE_DONE", CPythonEventManager::BUTTON_TYPE_DONE);
	ScriptModule_AddIntConstant(poModule, "BUTTON_TYPE_CANCEL", CPythonEventManager::BUTTON_TYPE_CANCEL);
}