#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonMiniMap.h>

ScriptObject* miniMapSetScale(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fScale;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fScale))
		return Script_BuildException();

	CPythonMiniMap::Instance().SetScale(fScale);
	return Script_BuildNone();
}

ScriptObject* miniMapSetCenterPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fCenterX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fCenterX))
		return Script_BuildException();

	float fCenterY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fCenterY))
		return Script_BuildException();

	CPythonMiniMap::Instance().SetCenterPosition(fCenterX, fCenterY);
	return Script_BuildNone();
}

ScriptObject* miniMapSetMiniMapSize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fWidth;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fWidth))
		return Script_BuildException();

	float fHeight;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fHeight))
		return Script_BuildException();

	CPythonMiniMap::Instance().SetMiniMapSize(fWidth, fHeight);
	return Script_BuildNone();
}

ScriptObject* miniMapDestroy(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().Destroy();
	return Script_BuildNone();
}

ScriptObject* miniMapCreate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().Create();
	return Script_BuildNone();
}

ScriptObject* miniMapUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fCenterX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fCenterX))
		return Script_BuildException();

	float fCenterY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fCenterY))
		return Script_BuildException();

	CPythonMiniMap::Instance().Update(fCenterX, fCenterY);
	return Script_BuildNone();
}

ScriptObject* miniMapRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fScrrenX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Script_BuildException();

	float fScrrenY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Script_BuildException();

	CPythonMiniMap::Instance().Render(fScrrenX, fScrrenY);
	return Script_BuildNone();
}

ScriptObject* miniMapShow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().Show();
	return Script_BuildNone();
}

ScriptObject* miniMapHide(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().Hide();
	return Script_BuildNone();
}

ScriptObject* miniMapisShow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	bool bShow = CPythonMiniMap::Instance().CanShow();
	return Script_BuildValue("b", bShow);
}

ScriptObject* miniMapScaleUp(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().ScaleUp();
	return Script_BuildNone();
}

ScriptObject* miniMapScaleDown(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().ScaleDown();
	return Script_BuildNone();
}

ScriptObject* miniMapGetInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fScrrenX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Script_BuildException();

	float fScrrenY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Script_BuildException();

	std::string aString;
	float fPosX, fPosY;
	DWORD dwTextColor;
	bool bFind = CPythonMiniMap::Instance().GetPickedInstanceInfo(fScrrenX, fScrrenY, aString, &fPosX, &fPosY, &dwTextColor);
	int iPosX, iPosY;
	PR_FLOAT_TO_INT(fPosX, iPosX);
	PR_FLOAT_TO_INT(fPosY, iPosY);
	iPosX /= 100;
	iPosY /= 100;
	return Script_BuildValue("isiil", (int)bFind, aString.c_str(), iPosX, iPosY, (signed)dwTextColor);
}

//////////////////////////////////////////////////////////////////////////
// Atlas
ScriptObject* miniMapLoadAtlas(ScriptObject* poSelf, ScriptObject* poArgs)
{
	if (!CPythonMiniMap::Instance().LoadAtlas())
		TraceError("CPythonMiniMap::Instance().LoadAtlas() Failed");
	return Script_BuildNone();
}

ScriptObject* miniMapUpdateAtlas(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().UpdateAtlas();
	return Script_BuildNone();
}

ScriptObject* miniMapRenderAtlas(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fScrrenX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Script_BuildException();

	float fScrrenY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Script_BuildException();

	CPythonMiniMap::Instance().RenderAtlas(fScrrenX, fScrrenY);
	return Script_BuildNone();
}

ScriptObject* miniMapShowAtlas(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().ShowAtlas();
	return Script_BuildNone();
}

ScriptObject* miniMapHideAtlas(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().HideAtlas();
	return Script_BuildNone();
}

ScriptObject* miniMapisShowAtlas(ScriptObject* poSelf, ScriptObject* poArgs)
{
	bool bShow = CPythonMiniMap::Instance().CanShowAtlas();
	return Script_BuildValue("b", bShow);
}

ScriptObject* miniMapIsAtlas(ScriptObject* poSelf, ScriptObject* poArgs)
{
	bool isData = CPythonMiniMap::Instance().IsAtlas();
	return Script_BuildValue("b", isData);
}

ScriptObject* miniMapGetAtlasInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fScrrenX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fScrrenX))
		return Script_BuildException();
	float fScrrenY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fScrrenY))
		return Script_BuildException();

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
	return Script_BuildValue("isiili", (int)bFind, aString.c_str(), iPosX, iPosY, (signed)dwTextColor, dwGuildID);
}

ScriptObject* miniMapGetAtlasSize(ScriptObject* poSelf, ScriptObject* poArgs)
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

	return Script_BuildValue("bii", (int)bGet, iSizeX, iSizeY);
}

ScriptObject* miniMapAddWayPoint(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	float fX;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fX))
		return Script_BuildException();

	float fY;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fY))
		return Script_BuildException();

	char* buf;
	if (!ScriptTuple_GetString(poArgs, 3, &buf))
		return Script_BuildException();

	CPythonMiniMap::Instance().AddWayPoint(CPythonMiniMap::TYPE_WAYPOINT, (DWORD)iID, fX, fY, buf);

	return Script_BuildNone();
}

ScriptObject* miniMapRemoveWayPoint(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	CPythonMiniMap::Instance().RemoveWayPoint((DWORD)iID);

	return Script_BuildNone();
}

ScriptObject* miniMapRegisterAtlasWindow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poHandler;
	if (!ScriptTuple_GetObject(poArgs, 0, &poHandler))
		return Script_BuildException();
	CPythonMiniMap::Instance().RegisterAtlasWindow(poHandler);
	return Script_BuildNone();
}

ScriptObject* miniMapUnregisterAtlasWindow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMiniMap::Instance().UnregisterAtlasWindow();
	return Script_BuildNone();
}

ScriptObject* miniMapGetGuildAreaID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fx;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fx))
		return Script_BuildException();
	float fy;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fy))
		return Script_BuildException();

	DWORD dwGuildID = CPythonMiniMap::Instance().GetGuildAreaID(fx, fy);
	return Script_BuildValue("i", dwGuildID);
}

void initMiniMap()
{
	static ScriptMethodDef s_methods[] =
		{
			{"SetScale", miniMapSetScale, SCRIPT_VARARGS, NULL},
			{"ScaleUp", miniMapScaleUp, SCRIPT_VARARGS, NULL},
			{"ScaleDown", miniMapScaleDown, SCRIPT_VARARGS, NULL},
			{"SetMiniMapSize", miniMapSetMiniMapSize, SCRIPT_VARARGS, NULL},

			{"SetCenterPosition", miniMapSetCenterPosition, SCRIPT_VARARGS, NULL},

			{"Destroy", miniMapDestroy, SCRIPT_VARARGS, NULL},
			{"Create", miniMapCreate, SCRIPT_VARARGS, NULL},
			{"Update", miniMapUpdate, SCRIPT_VARARGS, NULL},
			{"Render", miniMapRender, SCRIPT_VARARGS, NULL},

			{"Show", miniMapShow, SCRIPT_VARARGS, NULL},
			{"Hide", miniMapHide, SCRIPT_VARARGS, NULL},

			{"isShow", miniMapisShow, SCRIPT_VARARGS, NULL},

			{"GetInfo", miniMapGetInfo, SCRIPT_VARARGS, NULL},

			{"LoadAtlas", miniMapLoadAtlas, SCRIPT_VARARGS, NULL},
			{"UpdateAtlas", miniMapUpdateAtlas, SCRIPT_VARARGS, NULL},
			{"RenderAtlas", miniMapRenderAtlas, SCRIPT_VARARGS, NULL},
			{"ShowAtlas", miniMapShowAtlas, SCRIPT_VARARGS, NULL},
			{"HideAtlas", miniMapHideAtlas, SCRIPT_VARARGS, NULL},
			{"isShowAtlas", miniMapisShowAtlas, SCRIPT_VARARGS, NULL},
			{"IsAtlas", miniMapIsAtlas, SCRIPT_VARARGS, NULL},
			{"GetAtlasInfo", miniMapGetAtlasInfo, SCRIPT_VARARGS, NULL},
			{"GetAtlasSize", miniMapGetAtlasSize, SCRIPT_VARARGS, NULL},

			{"AddWayPoint", miniMapAddWayPoint, SCRIPT_VARARGS, NULL},
			{"RemoveWayPoint", miniMapRemoveWayPoint, SCRIPT_VARARGS, NULL},

			{"RegisterAtlasWindow", miniMapRegisterAtlasWindow, SCRIPT_VARARGS, NULL},
			{"UnregisterAtlasWindow", miniMapUnregisterAtlasWindow, SCRIPT_VARARGS, NULL},

			{"GetGuildAreaID", miniMapGetGuildAreaID, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("miniMap", s_methods);

	ScriptModule_AddIntConstant(poModule, "TYPE_OPC", CPythonMiniMap::TYPE_OPC);
	ScriptModule_AddIntConstant(poModule, "TYPE_OPCPVP", CPythonMiniMap::TYPE_OPCPVP);
	ScriptModule_AddIntConstant(poModule, "TYPE_OPCPVPSELF", CPythonMiniMap::TYPE_OPCPVPSELF);
	ScriptModule_AddIntConstant(poModule, "TYPE_NPC", CPythonMiniMap::TYPE_NPC);
	ScriptModule_AddIntConstant(poModule, "TYPE_MONSTER", CPythonMiniMap::TYPE_MONSTER);
	ScriptModule_AddIntConstant(poModule, "TYPE_WARP", CPythonMiniMap::TYPE_WARP);
	ScriptModule_AddIntConstant(poModule, "TYPE_WAYPOINT", CPythonMiniMap::TYPE_WAYPOINT);
	ScriptModule_AddIntConstant(poModule, "TYPE_PARTY", CPythonMiniMap::TYPE_PARTY);
	ScriptModule_AddIntConstant(poModule, "TYPE_EMPIRE", CPythonMiniMap::TYPE_EMPIRE);
}