#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonMiniMap.h>

PyObject* miniMapSetScale(PyObject* poSelf, PyObject* poArgs)
{
	float fScale;
	if (!PyTuple_GetFloat(poArgs, 0, &fScale))
		return Py_BuildException();

	CPythonMiniMap::Instance().SetScale(fScale);
	return Py_BuildNone();
}

PyObject* miniMapSetCenterPosition(PyObject* poSelf, PyObject* poArgs)
{
	float fCenterX;
	if (!PyTuple_GetFloat(poArgs, 0, &fCenterX))
		return Py_BuildException();

	float fCenterY;
	if (!PyTuple_GetFloat(poArgs, 1, &fCenterY))
		return Py_BuildException();

	CPythonMiniMap::Instance().SetCenterPosition(fCenterX, fCenterY);
	return Py_BuildNone();
}

PyObject* miniMapSetMiniMapSize(PyObject* poSelf, PyObject* poArgs)
{
	float fWidth;
	if (!PyTuple_GetFloat(poArgs, 0, &fWidth))
		return Py_BuildException();

	float fHeight;
	if (!PyTuple_GetFloat(poArgs, 1, &fHeight))
		return Py_BuildException();

	CPythonMiniMap::Instance().SetMiniMapSize(fWidth, fHeight);
	return Py_BuildNone();
}

PyObject* miniMapDestroy(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().Destroy();
	return Py_BuildNone();
}

PyObject* miniMapCreate(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().Create();
	return Py_BuildNone();
}

PyObject* miniMapUpdate(PyObject* poSelf, PyObject* poArgs)
{
	float fCenterX;
	if (!PyTuple_GetFloat(poArgs, 0, &fCenterX))
		return Py_BuildException();

	float fCenterY;
	if (!PyTuple_GetFloat(poArgs, 1, &fCenterY))
		return Py_BuildException();

	CPythonMiniMap::Instance().Update(fCenterX, fCenterY);
	return Py_BuildNone();
}

PyObject* miniMapRender(PyObject* poSelf, PyObject* poArgs)
{
	float fScrrenX;
	if (!PyTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Py_BuildException();

	float fScrrenY;
	if (!PyTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Py_BuildException();

	CPythonMiniMap::Instance().Render(fScrrenX, fScrrenY);
	return Py_BuildNone();
}

PyObject* miniMapShow(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().Show();
	return Py_BuildNone();
}

PyObject* miniMapHide(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().Hide();
	return Py_BuildNone();
}

PyObject* miniMapisShow(PyObject* poSelf, PyObject* poArgs)
{
	bool bShow = CPythonMiniMap::Instance().CanShow();
	return Py_BuildValue("b", bShow);
}

PyObject* miniMapScaleUp(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().ScaleUp();
	return Py_BuildNone();
}

PyObject* miniMapScaleDown(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().ScaleDown();
	return Py_BuildNone();
}

PyObject* miniMapGetInfo(PyObject* poSelf, PyObject* poArgs)
{
	float fScrrenX;
	if (!PyTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Py_BuildException();

	float fScrrenY;
	if (!PyTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Py_BuildException();

	std::string aString;
	float fPosX, fPosY;
	DWORD dwTextColor;
	bool bFind = CPythonMiniMap::Instance().GetPickedInstanceInfo(fScrrenX, fScrrenY, aString, &fPosX, &fPosY, &dwTextColor);
	int iPosX, iPosY;
	PR_FLOAT_TO_INT(fPosX, iPosX);
	PR_FLOAT_TO_INT(fPosY, iPosY);
	iPosX /= 100;
	iPosY /= 100;
	return Py_BuildValue("isiil", (int)bFind, aString.c_str(), iPosX, iPosY, (signed)dwTextColor);
}

//////////////////////////////////////////////////////////////////////////
// Atlas
PyObject* miniMapLoadAtlas(PyObject* poSelf, PyObject* poArgs)
{
	if (!CPythonMiniMap::Instance().LoadAtlas())
		TraceError("CPythonMiniMap::Instance().LoadAtlas() Failed");
	return Py_BuildNone();
}

PyObject* miniMapUpdateAtlas(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().UpdateAtlas();
	return Py_BuildNone();
}

PyObject* miniMapRenderAtlas(PyObject* poSelf, PyObject* poArgs)
{
	float fScrrenX;
	if (!PyTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Py_BuildException();

	float fScrrenY;
	if (!PyTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Py_BuildException();

	CPythonMiniMap::Instance().RenderAtlas(fScrrenX, fScrrenY);
	return Py_BuildNone();
}

PyObject* miniMapShowAtlas(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().ShowAtlas();
	return Py_BuildNone();
}

PyObject* miniMapHideAtlas(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().HideAtlas();
	return Py_BuildNone();
}

PyObject* miniMapisShowAtlas(PyObject* poSelf, PyObject* poArgs)
{
	bool bShow = CPythonMiniMap::Instance().CanShowAtlas();
	return Py_BuildValue("b", bShow);
}

PyObject* miniMapIsAtlas(PyObject* poSelf, PyObject* poArgs)
{
	bool isData = CPythonMiniMap::Instance().IsAtlas();
	return Py_BuildValue("b", isData);
}

PyObject* miniMapGetAtlasInfo(PyObject* poSelf, PyObject* poArgs)
{
	float fScrrenX;
	if (!PyTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Py_BuildException();
	float fScrrenY;
	if (!PyTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Py_BuildException();

	std::string aString = "";
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	DWORD dwTextColor = 0;
	DWORD dwGuildID = 0;
	bool bFind = CPythonMiniMap::Instance().GetAtlasInfo(fScrrenX, fScrrenY, aString, &fPosX, &fPosY, &dwTextColor, &dwGuildID);
	int iPosX, iPosY;
	PR_FLOAT_TO_INT(fPosX, iPosX);
	PR_FLOAT_TO_INT(fPosY, iPosY);
	iPosX /= 100;
	iPosY /= 100;
	return Py_BuildValue("isiili", (int)bFind, aString.c_str(), iPosX, iPosY, (signed)dwTextColor, dwGuildID);
}

PyObject* miniMapGetAtlasSize(PyObject* poSelf, PyObject* poArgs)
{
	float fSizeX, fSizeY;
	bool bGet = CPythonMiniMap::Instance().GetAtlasSize(&fSizeX, &fSizeY);

	/*
		float fSizeXoo256 = fSizeX / 256.0f;
		float fSizeYoo256 = fSizeY / 256.0f;

		if (fSizeXoo256 >= fSizeYoo256)
		{
			fSizeX /= fSizeYoo256;
			fSizeY = 256.0f;
		}
		else
		{
			fSizeX = 256.0f;
			fSizeY /= fSizeXoo256;
		}
	*/

	int iSizeX, iSizeY;
	PR_FLOAT_TO_INT(fSizeX, iSizeX);
	PR_FLOAT_TO_INT(fSizeY, iSizeY);

	return Py_BuildValue("bii", (int)bGet, iSizeX, iSizeY);
}

PyObject* miniMapAddWayPoint(PyObject* poSelf, PyObject* poArgs)
{
	int iID;
	if (!PyTuple_GetInteger(poArgs, 0, &iID))
		return Py_BuildException();

	float fX;
	if (!PyTuple_GetFloat(poArgs, 1, &fX))
		return Py_BuildException();

	float fY;
	if (!PyTuple_GetFloat(poArgs, 2, &fY))
		return Py_BuildException();

	char* buf;
	if (!PyTuple_GetString(poArgs, 3, &buf))
		return Py_BuildException();

	CPythonMiniMap::Instance().AddWayPoint(CPythonMiniMap::TYPE_WAYPOINT, (DWORD)iID, fX, fY, buf);

	return Py_BuildNone();
}

PyObject* miniMapRemoveWayPoint(PyObject* poSelf, PyObject* poArgs)
{
	int iID;
	if (!PyTuple_GetInteger(poArgs, 0, &iID))
		return Py_BuildException();

	CPythonMiniMap::Instance().RemoveWayPoint((DWORD)iID);

	return Py_BuildNone();
}

PyObject* miniMapRegisterAtlasWindow(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler;
	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BuildException();
	CPythonMiniMap::Instance().RegisterAtlasWindow(poHandler);
	return Py_BuildNone();
}

PyObject* miniMapUnregisterAtlasWindow(PyObject* poSelf, PyObject* poArgs)
{
	CPythonMiniMap::Instance().UnregisterAtlasWindow();
	return Py_BuildNone();
}

PyObject* miniMapGetGuildAreaID(PyObject* poSelf, PyObject* poArgs)
{
	float fx;
	if (!PyTuple_GetFloat(poArgs, 0, &fx))
		return Py_BuildException();
	float fy;
	if (!PyTuple_GetFloat(poArgs, 1, &fy))
		return Py_BuildException();

	DWORD dwGuildID = CPythonMiniMap::Instance().GetGuildAreaID(fx, fy);
	return Py_BuildValue("i", dwGuildID);
}

void initMiniMap()
{
	static PyMethodDef s_methods[] =
		{
			{"SetScale", miniMapSetScale, METH_VARARGS, NULL},
			{"ScaleUp", miniMapScaleUp, METH_VARARGS, NULL},
			{"ScaleDown", miniMapScaleDown, METH_VARARGS, NULL},
			{"SetMiniMapSize", miniMapSetMiniMapSize, METH_VARARGS, NULL},

			{"SetCenterPosition", miniMapSetCenterPosition, METH_VARARGS, NULL},

			{"Destroy", miniMapDestroy, METH_VARARGS, NULL},
			{"Create", miniMapCreate, METH_VARARGS, NULL},
			{"Update", miniMapUpdate, METH_VARARGS, NULL},
			{"Render", miniMapRender, METH_VARARGS, NULL},

			{"Show", miniMapShow, METH_VARARGS, NULL},
			{"Hide", miniMapHide, METH_VARARGS, NULL},

			{"isShow", miniMapisShow, METH_VARARGS, NULL},

			{"GetInfo", miniMapGetInfo, METH_VARARGS, NULL},

			{"LoadAtlas", miniMapLoadAtlas, METH_VARARGS, NULL},
			{"UpdateAtlas", miniMapUpdateAtlas, METH_VARARGS, NULL},
			{"RenderAtlas", miniMapRenderAtlas, METH_VARARGS, NULL},
			{"ShowAtlas", miniMapShowAtlas, METH_VARARGS, NULL},
			{"HideAtlas", miniMapHideAtlas, METH_VARARGS, NULL},
			{"isShowAtlas", miniMapisShowAtlas, METH_VARARGS, NULL},
			{"IsAtlas", miniMapIsAtlas, METH_VARARGS, NULL},
			{"GetAtlasInfo", miniMapGetAtlasInfo, METH_VARARGS, NULL},
			{"GetAtlasSize", miniMapGetAtlasSize, METH_VARARGS, NULL},

			{"AddWayPoint", miniMapAddWayPoint, METH_VARARGS, NULL},
			{"RemoveWayPoint", miniMapRemoveWayPoint, METH_VARARGS, NULL},

			{"RegisterAtlasWindow", miniMapRegisterAtlasWindow, METH_VARARGS, NULL},
			{"UnregisterAtlasWindow", miniMapUnregisterAtlasWindow, METH_VARARGS, NULL},

			{"GetGuildAreaID", miniMapGetGuildAreaID, METH_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	PyObject* poModule = Py_InitModule("miniMap", s_methods);

	PyModule_AddIntConstant(poModule, "TYPE_OPC", CPythonMiniMap::TYPE_OPC);
	PyModule_AddIntConstant(poModule, "TYPE_OPCPVP", CPythonMiniMap::TYPE_OPCPVP);
	PyModule_AddIntConstant(poModule, "TYPE_OPCPVPSELF", CPythonMiniMap::TYPE_OPCPVPSELF);
	PyModule_AddIntConstant(poModule, "TYPE_NPC", CPythonMiniMap::TYPE_NPC);
	PyModule_AddIntConstant(poModule, "TYPE_MONSTER", CPythonMiniMap::TYPE_MONSTER);
	PyModule_AddIntConstant(poModule, "TYPE_WARP", CPythonMiniMap::TYPE_WARP);
	PyModule_AddIntConstant(poModule, "TYPE_WAYPOINT", CPythonMiniMap::TYPE_WAYPOINT);
	PyModule_AddIntConstant(poModule, "TYPE_PARTY", CPythonMiniMap::TYPE_PARTY);
	PyModule_AddIntConstant(poModule, "TYPE_EMPIRE", CPythonMiniMap::TYPE_EMPIRE);
}