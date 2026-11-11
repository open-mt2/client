#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonIME.h>
#include <Client/UI/PythonApplication.h>

PyObject* imeEnable(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().Initialize(CPythonApplication::Instance().GetWindowHandle());
	return Py_BuildNone();
}

PyObject* imeDisable(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().Uninitialize();
	return Py_BuildNone();
}

PyObject* imeEnableCaptureInput(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().EnableCaptureInput();
	return Py_BuildNone();
}

PyObject* imeDisableCaptureInput(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().DisableCaptureInput();
	return Py_BuildNone();
}

PyObject* imeSetMax(PyObject* poSelf, PyObject* poArgs)
{
	int iMax;
	if (!PyTuple_GetInteger(poArgs, 0, &iMax))
		return Py_BuildException();

	CPythonIME::Instance().SetMax(iMax);
	return Py_BuildNone();
}

PyObject* imeSetUserMax(PyObject* poSelf, PyObject* poArgs)
{
	int iMax;
	if (!PyTuple_GetInteger(poArgs, 0, &iMax))
		return Py_BuildException();

	CPythonIME::Instance().SetUserMax(iMax);
	return Py_BuildNone();
}

PyObject* imeSetText(PyObject* poSelf, PyObject* poArgs)
{
	char* szText;
	if (!PyTuple_GetString(poArgs, 0, &szText))
		return Py_BuildException();

	CPythonIME::Instance().SetText(szText, strlen(szText));
	return Py_BuildNone();
}

PyObject* imeGetText(PyObject* poSelf, PyObject* poArgs)
{
	int bCodePage;
	if (!PyTuple_GetInteger(poArgs, 0, &bCodePage))
		bCodePage = 0;

	std::string strText;
	CPythonIME::Instance().GetText(strText, bCodePage ? true : false);
	return Py_BuildValue("s", strText.c_str());
}

PyObject* imeGetCodePage(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonIME::Instance().GetCodePage());
}

PyObject* imeGetCandidateCount(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonIME::Instance().GetCandidatePageCount());
}

PyObject* imeGetCandidate(PyObject* poSelf, PyObject* poArgs)
{
	int index;
	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	std::string strText;
	int iLength = CPythonIME::Instance().GetCandidate(index, strText);
	return Py_BuildValue("si", strText.c_str(), iLength);
}

PyObject* imeGetCandidateSelection(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonIME::Instance().GetCandidateSelection());
}

PyObject* imeGetReading(PyObject* poSelf, PyObject* poArgs)
{
	std::string strText;
	CPythonIME::Instance().GetReading(strText);
	return Py_BuildValue("s", strText.c_str());
}

PyObject* imeGetReadingError(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonIME::Instance().GetReadingError());
}

PyObject* imeEnableIME(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().EnableIME();
	return Py_BuildNone();
}

PyObject* imeDisableIME(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().DisableIME();
	return Py_BuildNone();
}

PyObject* imeSetInputMode(PyObject* poSelf, PyObject* poArgs)
{
	int mode;
	if (!PyTuple_GetInteger(poArgs, 0, &mode))
		return Py_BuildException();

	CPythonIME::Instance().SetInputMode(mode);
	return Py_BuildNone();
}

PyObject* imeSetNumberMode(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().SetNumberMode();
	return Py_BuildNone();
}

PyObject* imeAddExceptKey(PyObject* poSelf, PyObject* poArgs)
{
	int key;
	if (!PyTuple_GetInteger(poArgs, 0, &key))
		return Py_BuildException();

	CPythonIME::Instance().AddExceptKey(key);
	return Py_BuildNone();
}

PyObject* imeClearExceptKey(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().ClearExceptKey();
	return Py_BuildNone();
}

PyObject* imeSetStringMode(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().SetStringMode();
	return Py_BuildNone();
}

PyObject* imeGetInputMode(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonIME::Instance().GetInputMode());
}

PyObject* imeMoveLeft(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().MoveLeft();

	return Py_BuildNone();
}
PyObject* imeMoveRight(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().MoveRight();

	return Py_BuildNone();
}

PyObject* imeMoveHome(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().MoveHome();

	return Py_BuildNone();
}
PyObject* imeMoveEnd(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().MoveEnd();

	return Py_BuildNone();
}

PyObject* imeSetCursorPosition(PyObject* poSelf, PyObject* poArgs)
{
	int iPosition;
	if (!PyTuple_GetInteger(poArgs, 0, &iPosition))
		return Py_BuildException();

	CPythonIME::Instance().SetCursorPosition(iPosition);

	return Py_BuildNone();
}

PyObject* imeDelete(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().Delete();

	return Py_BuildNone();
}

PyObject* imePasteTextFromClipBoard(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().PasteTextFromClipBoard();
	return Py_BuildNone();
}

PyObject* imeEnablePaste(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonIME::Instance().EnablePaste(iFlag ? true : false);
	return Py_BuildNone();
}

PyObject* imePasteString(PyObject* poSelf, PyObject* poArgs)
{
	char* szText;
	if (!PyTuple_GetString(poArgs, 0, &szText))
		return Py_BuildException();
	CPythonIME::Instance().PasteString(szText);
	return Py_BuildNone();
}

PyObject* imePasteBackspace(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().WMChar(NULL, WM_CHAR, 0x08, 0);
	return Py_BuildNone();
}

PyObject* imePasteReturn(PyObject* poSelf, PyObject* poArgs)
{
	CPythonIME::Instance().WMChar(NULL, WM_CHAR, 0x0D, 0);
	return Py_BuildNone();
}

void initime()
{
	static PyMethodDef s_methods[] =
		{
			{"Enable", imeEnable, METH_VARARGS, NULL},
			{"Disable", imeDisable, METH_VARARGS, NULL},

			{"EnableCaptureInput", imeEnableCaptureInput, METH_VARARGS, NULL},
			{"DisableCaptureInput", imeDisableCaptureInput, METH_VARARGS, NULL},
			{"SetMax", imeSetMax, METH_VARARGS, NULL},
			{"SetUserMax", imeSetUserMax, METH_VARARGS, NULL},
			{"SetText", imeSetText, METH_VARARGS, NULL},
			{"GetText", imeGetText, METH_VARARGS, NULL},
			{"GetCodePage", imeGetCodePage, METH_VARARGS, NULL},
			{"GetCandidateCount", imeGetCandidateCount, METH_VARARGS, NULL},
			{"GetCandidate", imeGetCandidate, METH_VARARGS, NULL},
			{"GetCandidateSelection", imeGetCandidateSelection, METH_VARARGS, NULL},
			{"GetReading", imeGetReading, METH_VARARGS, NULL},
			{"GetReadingError", imeGetReadingError, METH_VARARGS, NULL},
			{"EnableIME", imeEnableIME, METH_VARARGS, NULL},
			{"DisableIME", imeDisableIME, METH_VARARGS, NULL},
			{"GetInputMode", imeGetInputMode, METH_VARARGS, NULL},
			{"SetInputMode", imeSetInputMode, METH_VARARGS, NULL},

			{"SetNumberMode", imeSetNumberMode, METH_VARARGS, NULL},
			{"SetStringMode", imeSetStringMode, METH_VARARGS, NULL},
			{"AddExceptKey", imeAddExceptKey, METH_VARARGS, NULL},
			{"ClearExceptKey", imeClearExceptKey, METH_VARARGS, NULL},

			{"MoveLeft", imeMoveLeft, METH_VARARGS, NULL},
			{"MoveRight", imeMoveRight, METH_VARARGS, NULL},
			{"MoveHome", imeMoveHome, METH_VARARGS, NULL},
			{"MoveEnd", imeMoveEnd, METH_VARARGS, NULL},
			{"SetCursorPosition", imeSetCursorPosition, METH_VARARGS, NULL},
			{"Delete", imeDelete, METH_VARARGS, NULL},
			{"PasteString", imePasteString, METH_VARARGS, NULL},
			{"PasteBackspace", imePasteBackspace, METH_VARARGS, NULL},
			{"PasteReturn", imePasteReturn, METH_VARARGS, NULL},
			{"PasteTextFromClipBoard", imePasteTextFromClipBoard, METH_VARARGS, NULL},
			{"EnablePaste", imeEnablePaste, METH_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	Py_InitModule("ime", s_methods);
}