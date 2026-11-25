#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonIME.h>
#include <Client/UI/PythonApplication.h>

ScriptObject* imeEnable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().Initialize(CPythonApplication::Instance().GetWindowHandle());
	return Script_BuildNone();
}

ScriptObject* imeDisable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().Uninitialize();
	return Script_BuildNone();
}

ScriptObject* imeEnableCaptureInput(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().EnableCaptureInput();
	return Script_BuildNone();
}

ScriptObject* imeDisableCaptureInput(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().DisableCaptureInput();
	return Script_BuildNone();
}

ScriptObject* imeSetMax(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMax;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMax))
		return Script_BuildException();

	CPythonIME::Instance().SetMax(iMax);
	return Script_BuildNone();
}

ScriptObject* imeSetUserMax(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMax;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMax))
		return Script_BuildException();

	CPythonIME::Instance().SetUserMax(iMax);
	return Script_BuildNone();
}

ScriptObject* imeSetText(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szText;
	if (!ScriptTuple_GetString(poArgs, 0, &szText))
		return Script_BuildException();

	CPythonIME::Instance().SetText(szText, strlen(szText));
	return Script_BuildNone();
}

ScriptObject* imeGetText(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int bCodePage;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bCodePage))
		bCodePage = 0;

	std::string strText;
	CPythonIME::Instance().GetText(strText, bCodePage ? true : false);
	return Script_BuildValue("s", strText.c_str());
}

ScriptObject* imeGetCodePage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonIME::Instance().GetCodePage());
}

ScriptObject* imeGetCandidateCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonIME::Instance().GetCandidatePageCount());
}

ScriptObject* imeGetCandidate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index;
	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BuildException();

	std::string strText;
	int iLength = CPythonIME::Instance().GetCandidate(index, strText);
	return Script_BuildValue("si", strText.c_str(), iLength);
}

ScriptObject* imeGetCandidateSelection(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonIME::Instance().GetCandidateSelection());
}

ScriptObject* imeGetReading(ScriptObject* poSelf, ScriptObject* poArgs)
{
	std::string strText;
	CPythonIME::Instance().GetReading(strText);
	return Script_BuildValue("s", strText.c_str());
}

ScriptObject* imeGetReadingError(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonIME::Instance().GetReadingError());
}

ScriptObject* imeEnableIME(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().EnableIME();
	return Script_BuildNone();
}

ScriptObject* imeDisableIME(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().DisableIME();
	return Script_BuildNone();
}

ScriptObject* imeSetInputMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int mode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &mode))
		return Script_BuildException();

	CPythonIME::Instance().SetInputMode(mode);
	return Script_BuildNone();
}

ScriptObject* imeSetNumberMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().SetNumberMode();
	return Script_BuildNone();
}

ScriptObject* imeAddExceptKey(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int key;
	if (!ScriptTuple_GetInteger(poArgs, 0, &key))
		return Script_BuildException();

	CPythonIME::Instance().AddExceptKey(key);
	return Script_BuildNone();
}

ScriptObject* imeClearExceptKey(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().ClearExceptKey();
	return Script_BuildNone();
}

ScriptObject* imeSetStringMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().SetStringMode();
	return Script_BuildNone();
}

ScriptObject* imeGetInputMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonIME::Instance().GetInputMode());
}

ScriptObject* imeMoveLeft(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().MoveLeft();

	return Script_BuildNone();
}
ScriptObject* imeMoveRight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().MoveRight();

	return Script_BuildNone();
}

ScriptObject* imeMoveHome(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().MoveHome();

	return Script_BuildNone();
}
ScriptObject* imeMoveEnd(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().MoveEnd();

	return Script_BuildNone();
}

ScriptObject* imeSetCursorPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPosition;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPosition))
		return Script_BuildException();

	CPythonIME::Instance().SetCursorPosition(iPosition);

	return Script_BuildNone();
}

ScriptObject* imeDelete(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().Delete();

	return Script_BuildNone();
}

ScriptObject* imePasteTextFromClipBoard(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().PasteTextFromClipBoard();
	return Script_BuildNone();
}

ScriptObject* imeEnablePaste(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();

	CPythonIME::Instance().EnablePaste(iFlag ? true : false);
	return Script_BuildNone();
}

ScriptObject* imePasteString(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szText;
	if (!ScriptTuple_GetString(poArgs, 0, &szText))
		return Script_BuildException();
	CPythonIME::Instance().PasteString(szText);
	return Script_BuildNone();
}

ScriptObject* imePasteBackspace(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().WMChar(NULL, WM_CHAR, 0x08, 0);
	return Script_BuildNone();
}

ScriptObject* imePasteReturn(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonIME::Instance().WMChar(NULL, WM_CHAR, 0x0D, 0);
	return Script_BuildNone();
}

void initime()
{
	static ScriptMethodDef s_methods[] =
		{
			{"Enable", imeEnable, SCRIPT_VARARGS, NULL},
			{"Disable", imeDisable, SCRIPT_VARARGS, NULL},

			{"EnableCaptureInput", imeEnableCaptureInput, SCRIPT_VARARGS, NULL},
			{"DisableCaptureInput", imeDisableCaptureInput, SCRIPT_VARARGS, NULL},
			{"SetMax", imeSetMax, SCRIPT_VARARGS, NULL},
			{"SetUserMax", imeSetUserMax, SCRIPT_VARARGS, NULL},
			{"SetText", imeSetText, SCRIPT_VARARGS, NULL},
			{"GetText", imeGetText, SCRIPT_VARARGS, NULL},
			{"GetCodePage", imeGetCodePage, SCRIPT_VARARGS, NULL},
			{"GetCandidateCount", imeGetCandidateCount, SCRIPT_VARARGS, NULL},
			{"GetCandidate", imeGetCandidate, SCRIPT_VARARGS, NULL},
			{"GetCandidateSelection", imeGetCandidateSelection, SCRIPT_VARARGS, NULL},
			{"GetReading", imeGetReading, SCRIPT_VARARGS, NULL},
			{"GetReadingError", imeGetReadingError, SCRIPT_VARARGS, NULL},
			{"EnableIME", imeEnableIME, SCRIPT_VARARGS, NULL},
			{"DisableIME", imeDisableIME, SCRIPT_VARARGS, NULL},
			{"GetInputMode", imeGetInputMode, SCRIPT_VARARGS, NULL},
			{"SetInputMode", imeSetInputMode, SCRIPT_VARARGS, NULL},

			{"SetNumberMode", imeSetNumberMode, SCRIPT_VARARGS, NULL},
			{"SetStringMode", imeSetStringMode, SCRIPT_VARARGS, NULL},
			{"AddExceptKey", imeAddExceptKey, SCRIPT_VARARGS, NULL},
			{"ClearExceptKey", imeClearExceptKey, SCRIPT_VARARGS, NULL},

			{"MoveLeft", imeMoveLeft, SCRIPT_VARARGS, NULL},
			{"MoveRight", imeMoveRight, SCRIPT_VARARGS, NULL},
			{"MoveHome", imeMoveHome, SCRIPT_VARARGS, NULL},
			{"MoveEnd", imeMoveEnd, SCRIPT_VARARGS, NULL},
			{"SetCursorPosition", imeSetCursorPosition, SCRIPT_VARARGS, NULL},
			{"Delete", imeDelete, SCRIPT_VARARGS, NULL},
			{"PasteString", imePasteString, SCRIPT_VARARGS, NULL},
			{"PasteBackspace", imePasteBackspace, SCRIPT_VARARGS, NULL},
			{"PasteReturn", imePasteReturn, SCRIPT_VARARGS, NULL},
			{"PasteTextFromClipBoard", imePasteTextFromClipBoard, SCRIPT_VARARGS, NULL},
			{"EnablePaste", imeEnablePaste, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptModule_Create("ime", s_methods);
}