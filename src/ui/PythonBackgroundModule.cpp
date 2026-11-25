#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonSystem.h>
#include <Client/UI/PythonBackground.h>
#include <Neoeter/Render/StateManager.h>
#include <Client/Game/MapOutdoor.h>

ScriptObject* backgroundIsSoftwareTiling(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return Script_BuildValue("i", rkBG.IsSoftwareTilingEnable());
}

ScriptObject* backgroundEnableSoftwareTiling(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIsEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nIsEnable))
		return Script_BadArgument();

	bool isEnable = nIsEnable ? true : false;

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.ReserveSoftwareTilingEnable(isEnable);

	CPythonSystem& rkSystem = CPythonSystem::Instance();
	rkSystem.SetSoftwareTiling(isEnable);
	return Script_BuildNone();
}

ScriptObject* backgroundEnableSnow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIsEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nIsEnable))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	if (nIsEnable)
		rkBG.EnableSnowEnvironment();
	else
		rkBG.DisableSnowEnvironment();

	return Script_BuildNone();
}

ScriptObject* backgroundLoadMap(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* pszMapPathName;

	if (!ScriptTuple_GetString(poArgs, 0, &pszMapPathName))
		return Script_BadArgument();

	float x, y, z;

	if (!ScriptTuple_GetFloat(poArgs, 1, &x))
		return Script_BadArgument();

	if (!ScriptTuple_GetFloat(poArgs, 2, &y))
		return Script_BadArgument();

	if (!ScriptTuple_GetFloat(poArgs, 3, &z))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.LoadMap(pszMapPathName, x, y, z);

	// #ifdef _DEBUG
	//	CMapOutdoor& rkMap=rkBG.GetMapOutdoorRef();
	//	rkMap.EnablePortal(TRUE);
	//	rkBG.EnableTerrainOnlyForHeight();
	// #endif

	return Script_BuildNone();
}

ScriptObject* backgroundDestroy(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetShadowLevel(CPythonBackground::SHADOW_NONE);
	rkBG.Destroy();
	return Script_BuildNone();
}

ScriptObject* backgroundRegisterEnvironmentData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BadArgument();

	char* pszEnvironmentFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &pszEnvironmentFileName))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	if (!rkBG.RegisterEnvironmentData(iIndex, pszEnvironmentFileName))
	{
		TraceError("background.RegisterEnvironmentData(iIndex=%d, szEnvironmentFileName=%s)", iIndex, pszEnvironmentFileName);

		// TODO:
		// 디폴트 환경 설정 작업을 해주자
	}

	return Script_BuildNone();
}

ScriptObject* backgroundSetEnvironmentData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BadArgument();

	const TEnvironmentData* c_pEnvironmenData;

	CPythonBackground& rkBG = CPythonBackground::Instance();
	if (rkBG.GetEnvironmentData(iIndex, &c_pEnvironmenData))
		rkBG.ResetEnvironmentDataPtr(c_pEnvironmenData);

	return Script_BuildNone();
}

ScriptObject* backgroundGetCurrentMapName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return Script_BuildValue("s", rkBG.GetWarpMapName());
}

ScriptObject* backgroundGetPickingPoint(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	TPixelPosition kPPosPicked(0.0f, 0.0f, 0.0f);
	if (rkBG.GetPickingPoint(&kPPosPicked))
	{
		kPPosPicked.y = -kPPosPicked.y;
	}
	return Script_BuildValue("fff", kPPosPicked.x, kPPosPicked.y, kPPosPicked.z);
}

ScriptObject* backgroundBeginEnvironment(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.BeginEnvironment();
	return Script_BuildNone();
}

ScriptObject* backgroundEndEnvironemt(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.EndEnvironment();
	return Script_BuildNone();
}

ScriptObject* backgroundSetCharacterDirLight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetCharacterDirLight();
	return Script_BuildNone();
}

ScriptObject* backgroundSetBackgroundDirLight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetBackgroundDirLight();
	return Script_BuildNone();
}

ScriptObject* backgroundSetLightingMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* pszMode;
	if (!ScriptTuple_GetString(poArgs, 0, &pszMode))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	return Script_BuildValue("i", rkBG.SetLightingMode(pszMode) ? 1 : 0);
}

ScriptObject* backgroundGetLightingMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return Script_BuildValue("s", rkBG.GetLightingMode());
}

ScriptObject* backgroundSetDayNightEnabled(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int bEnabled;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bEnabled))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetDayNightEnabled(bEnabled != 0);
	return Script_BuildNone();
}

ScriptObject* backgroundIsDayNightEnabled(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return Script_BuildValue("i", rkBG.IsDayNightEnabled() ? 1 : 0);
}

ScriptObject* backgroundSetTimeOfDay(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fHour;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fHour))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetTimeOfDay(fHour);
	return Script_BuildNone();
}

ScriptObject* backgroundGetTimeOfDay(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return Script_BuildValue("f", rkBG.GetTimeOfDay());
}

ScriptObject* backgroundChangeToDay(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.ChangeToDay();
	return Script_BuildNone();
}

ScriptObject* backgroundChangeToNight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.ChangeToNight();
	return Script_BuildNone();
}

ScriptObject* backgroundInitialize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.Create();
	return Script_BuildNone();
}

ScriptObject* backgroundUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fCameraX;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fCameraX))
		return Script_BadArgument();

	float fCameraY;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fCameraY))
		return Script_BadArgument();

	float fCameraZ;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fCameraZ))
		return Script_BadArgument();

	CPythonBackground::Instance().Update(fCameraX, fCameraY, fCameraZ);
	return Script_BuildNone();
}

ScriptObject* backgroundRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().Render();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderPCBlocker(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderPCBlocker();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderCollision(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderCollision();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderSky(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderSky();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderCloud(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderCloud();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderWater(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderWater();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderEffect();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderBeforeLensFlare(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderBeforeLensFlare();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderAfterLensFlare(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderAfterLensFlare();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderCharacterShadowToTexture(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().RenderCharacterShadowToTexture();
	return Script_BuildNone();
}

ScriptObject* backgroundRenderDungeon(ScriptObject* poSelf, ScriptObject* poArgs)
{
	assert(!"background.RenderDungeon() - \xBB\xE7\xBF\xEB\xC7\xCF\xC1\xF6 \xBE\xCA\xB4\xC2 \xC7\xD4\xBC\xF6\xC0\xD4\xB4\xCF\xB4\xD9 - [levites]");
	return Script_BuildNone();
}

ScriptObject* backgroundGetHeight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fx;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fx))
		return Script_BadArgument();

	float fy;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fy))
		return Script_BadArgument();

	float fz = CPythonBackground::Instance().GetHeight(fx, fy);
	return Script_BuildValue("f", fz);
}

ScriptObject* backgroundGetRenderedSplatNum(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPatch;
	int iSplat;
	float fSplatRatio;

	std::vector<int>& aTextureNumVector = CPythonBackground::Instance().GetRenderedSplatNum(&iPatch, &iSplat, &fSplatRatio);

	char szOutput[MAX_PATH] = "";
	int iOutput = 0;
	for (std::vector<int>::iterator it = aTextureNumVector.begin(); it != aTextureNumVector.end(); it++)
	{
		iOutput += snprintf(szOutput + iOutput, sizeof(szOutput) - iOutput, "%d ", *it);
	}
	// std::copy(aTextureNumVector.begin(),aTextureNumVector.end(),std::ostream_iterator<int>(ostr," "));

	return Script_BuildValue("iifs", iPatch, iSplat, fSplatRatio, szOutput);
}

ScriptObject* backgroundGetRenderedGTINum(ScriptObject* poSelf, ScriptObject* poArgs)
{
	DWORD dwGraphicThingInstanceNum;
	DWORD dwCRCNum;

	/*CArea::TCRCWithNumberVector & rCRCWithNumberVector = */
	CPythonBackground::Instance().GetRenderedGraphicThingInstanceNum(&dwGraphicThingInstanceNum, &dwCRCNum);

	/*
		std::ostringstream ostr;

		std::for_each(rCRCWithNumberVector.begin(),rCRCWithNumberVector.end(),std::ostream_iterator<CArea::TCRCWithNumberVector>(ostr," "));
	*/

	return Script_BuildValue("ii", dwGraphicThingInstanceNum, dwCRCNum);
}

ScriptObject* backgroundGetRenderShadowTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return Script_BuildValue("i", rkBG.GetRenderShadowTime());
}

ScriptObject* backgroundGetShadowMapcolor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fx;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fx))
		return Script_BadArgument();

	float fy;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fy))
		return Script_BadArgument();

	DWORD dwColor = CPythonBackground::Instance().GetShadowMapColor(fx, fy);
	return Script_BuildValue("i", dwColor);
}

ScriptObject* backgroundSetShadowLevel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iLevel;

	if (!ScriptTuple_GetInteger(poArgs, 0, &iLevel))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetShadowLevel(iLevel);
	return Script_BuildNone();
}

ScriptObject* backgroundSetVisiblePart(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ePart;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ePart))
		return Script_BadArgument();

	int isVisible;
	if (!ScriptTuple_GetInteger(poArgs, 1, &isVisible))
		return Script_BadArgument();

	if (ePart >= CMapOutdoor::PART_NUM)
		return Script_BuildException("ePart(%d)<background.PART_NUM(%d)", ePart, CMapOutdoor::PART_NUM);

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetVisiblePart(ePart, isVisible ? true : false);

	return Script_BuildNone();
}

ScriptObject* backgroundSetSpaltLimit(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSplatNum;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSplatNum))
		return Script_BadArgument();

	if (iSplatNum < 0)
		return Script_BuildException("background.SetSplatLimit(iSplatNum(%d)>=0)", iSplatNum);

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetSplatLimit(iSplatNum);

	return Script_BuildNone();
}

ScriptObject* backgroundSelectViewDistanceNum(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iNum;

	if (!ScriptTuple_GetInteger(poArgs, 0, &iNum))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SelectViewDistanceNum(iNum);

	return Script_BuildNone();
}

ScriptObject* backgroundSetViewDistanceSet(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iNum;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iNum))
		return Script_BadArgument();

	float fFarClip;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fFarClip))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.SetViewDistanceSet(iNum, fFarClip);
	return Script_BuildNone();
}

ScriptObject* backgroundGetFarClip(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fFarClip = CPythonBackground::Instance().GetFarClip();
	return Script_BuildValue("f", fFarClip);
}

ScriptObject* backgroundGetDistanceSetInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iNum;
	float fStart, fEnd, fFarClip;
	CPythonBackground::Instance().GetDistanceSetInfo(&iNum, &fStart, &fEnd, &fFarClip);
	return Script_BuildValue("ifff", iNum, fStart, fEnd, fFarClip);
}

ScriptObject* backgroundSetBGLoading(ScriptObject* poSelf, ScriptObject* poArgs)
{
	bool bBGLoading;
	if (!ScriptTuple_GetBoolean(poArgs, 0, &bBGLoading))
		return Script_BadArgument();

	// CPythonBackground::Instance().BGLoadingEnable(bBGLoading);
	return Script_BuildNone();
}

ScriptObject* backgroundSetRenderSort(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eSort;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eSort))
		return Script_BadArgument();

	CPythonBackground::Instance().SetTerrainRenderSort((CMapOutdoor::ETerrainRenderSort)eSort);
	return Script_BuildNone();
}

ScriptObject* backgroundSetTransparentTree(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int bTransparent;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bTransparent))
		return Script_BadArgument();

	CPythonBackground::Instance().SetTransparentTree(bTransparent ? true : false);
	return Script_BuildNone();
}

ScriptObject* backgroundGlobalPositionToLocalPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iX;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iX))
		return Script_BadArgument();

	int iY;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iY))
		return Script_BadArgument();

	LONG lX = iX;
	LONG lY = iY;
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.GlobalPositionToLocalPosition(lX, lY);

	return Script_BuildValue("ii", lX, lY);
}

ScriptObject* backgroundGlobalPositionToMapInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iX;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iX))
		return Script_BadArgument();

	int iY;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iY))
		return Script_BadArgument();

	CPythonBackground& rkBG = CPythonBackground::Instance();
	CPythonBackground::TMapInfo* pkMapInfo = rkBG.GlobalPositionToMapInfo(iX, iY);

	if (pkMapInfo)
		return Script_BuildValue("sii", pkMapInfo->m_strName.c_str(), pkMapInfo->m_dwBaseX, pkMapInfo->m_dwBaseY);
	else
		return Script_BuildValue("sii", "", 0, 0);
}

ScriptObject* backgroundWarpTest(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iX;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iX))
		return Script_BadArgument();

	int iY;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iY))
		return Script_BadArgument();

	CPythonBackground::Instance().Warp((DWORD)iX * 100, (DWORD)iY * 100);
	return Script_BuildNone();
}

ScriptObject* backgroundSetXMasTree(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGrade;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGrade))
		return Script_BadArgument();

	CPythonBackground::Instance().SetXMaxTree(iGrade);
	return Script_BuildNone();
}

ScriptObject* backgroundRegisterDungeonMapName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BadArgument();

	CPythonBackground::Instance().RegisterDungeonMapName(szName);
	return Script_BuildNone();
}

ScriptObject* backgroundVisibleGuildArea(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().VisibleGuildArea();
	return Script_BuildNone();
}

ScriptObject* backgroundDisableGuildArea(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonBackground::Instance().DisableGuildArea();
	return Script_BuildNone();
}

void initBackground()
{
	static ScriptMethodDef s_methods[] =
		{
			{"IsSoftwareTiling", backgroundIsSoftwareTiling, SCRIPT_VARARGS, NULL},
			{"EnableSoftwareTiling", backgroundEnableSoftwareTiling, SCRIPT_VARARGS, NULL},
			{"EnableSnow", backgroundEnableSnow, SCRIPT_VARARGS, NULL},
			{"GlobalPositionToLocalPosition", backgroundGlobalPositionToLocalPosition, SCRIPT_VARARGS, NULL},
			{"GlobalPositionToMapInfo", backgroundGlobalPositionToMapInfo, SCRIPT_VARARGS, NULL},
			{"GetRenderShadowTime", backgroundGetRenderShadowTime, SCRIPT_VARARGS, NULL},
			{"LoadMap", backgroundLoadMap, SCRIPT_VARARGS, NULL},
			{"Destroy", backgroundDestroy, SCRIPT_VARARGS, NULL},
			{"RegisterEnvironmentData", backgroundRegisterEnvironmentData, SCRIPT_VARARGS, NULL},
			{"SetEnvironmentData", backgroundSetEnvironmentData, SCRIPT_VARARGS, NULL},
			{"GetCurrentMapName", backgroundGetCurrentMapName, SCRIPT_VARARGS, NULL},
			{"GetPickingPoint", backgroundGetPickingPoint, SCRIPT_VARARGS, NULL},

			{"BeginEnvironment", backgroundBeginEnvironment, SCRIPT_VARARGS, NULL},
			{"EndEnvironment", backgroundEndEnvironemt, SCRIPT_VARARGS, NULL},
			{"SetCharacterDirLight", backgroundSetCharacterDirLight, SCRIPT_VARARGS, NULL},
			{"SetBackgroundDirLight", backgroundSetBackgroundDirLight, SCRIPT_VARARGS, NULL},
			{"SetLightingMode", backgroundSetLightingMode, SCRIPT_VARARGS, NULL},
			{"GetLightingMode", backgroundGetLightingMode, SCRIPT_VARARGS, NULL},
			{"SetDayNightEnabled", backgroundSetDayNightEnabled, SCRIPT_VARARGS, NULL},
			{"IsDayNightEnabled", backgroundIsDayNightEnabled, SCRIPT_VARARGS, NULL},
			{"SetTimeOfDay", backgroundSetTimeOfDay, SCRIPT_VARARGS, NULL},
			{"GetTimeOfDay", backgroundGetTimeOfDay, SCRIPT_VARARGS, NULL},
			{"ChangeToDay", backgroundChangeToDay, SCRIPT_VARARGS, NULL},
			{"ChangeToNight", backgroundChangeToNight, SCRIPT_VARARGS, NULL},

			{"Initialize", backgroundInitialize, SCRIPT_VARARGS, NULL},
			{"Update", backgroundUpdate, SCRIPT_VARARGS, NULL},
			{"Render", backgroundRender, SCRIPT_VARARGS, NULL},
			{"RenderPCBlocker", backgroundRenderPCBlocker, SCRIPT_VARARGS, NULL},
			{"RenderCollision", backgroundRenderCollision, SCRIPT_VARARGS, NULL},
			{"RenderSky", backgroundRenderSky, SCRIPT_VARARGS, NULL},
			{"RenderCloud", backgroundRenderCloud, SCRIPT_VARARGS, NULL},
			{"RenderWater", backgroundRenderWater, SCRIPT_VARARGS, NULL},
			{"RenderEffect", backgroundRenderEffect, SCRIPT_VARARGS, NULL},
			{"RenderBeforeLensFlare", backgroundRenderBeforeLensFlare, SCRIPT_VARARGS, NULL},
			{"RenderAfterLensFlare", backgroundRenderAfterLensFlare, SCRIPT_VARARGS, NULL},
			{"RenderCharacterShadowToTexture", backgroundRenderCharacterShadowToTexture, SCRIPT_VARARGS, NULL},
			{"RenderDungeon", backgroundRenderDungeon, SCRIPT_VARARGS, NULL},
			{"GetHeight", backgroundGetHeight, SCRIPT_VARARGS, NULL},

			{"SetShadowLevel", backgroundSetShadowLevel, SCRIPT_VARARGS, NULL},

			{"SetVisiblePart", backgroundSetVisiblePart, SCRIPT_VARARGS, NULL},
			{"GetShadowMapColor", backgroundGetShadowMapcolor, SCRIPT_VARARGS, NULL},
			{"SetSplatLimit", backgroundSetSpaltLimit, SCRIPT_VARARGS, NULL},
			{"GetRenderedSplatNum", backgroundGetRenderedSplatNum, SCRIPT_VARARGS, NULL},
			{"GetRenderedGraphicThingInstanceNum", backgroundGetRenderedGTINum, SCRIPT_VARARGS, NULL},
			{"SelectViewDistanceNum", backgroundSelectViewDistanceNum, SCRIPT_VARARGS, NULL},
			{"SetViewDistanceSet", backgroundSetViewDistanceSet, SCRIPT_VARARGS, NULL},
			{"GetFarClip", backgroundGetFarClip, SCRIPT_VARARGS, NULL},
			{"GetDistanceSetInfo", backgroundGetDistanceSetInfo, SCRIPT_VARARGS, NULL},
			{"SetBGLoading", backgroundSetBGLoading, SCRIPT_VARARGS, NULL},
			{"SetRenderSort", backgroundSetRenderSort, SCRIPT_VARARGS, NULL},
			{"SetTransparentTree", backgroundSetTransparentTree, SCRIPT_VARARGS, NULL},
			{"SetXMasTree", backgroundSetXMasTree, SCRIPT_VARARGS, NULL},
			{"RegisterDungeonMapName", backgroundRegisterDungeonMapName, SCRIPT_VARARGS, NULL},

			{"VisibleGuildArea", backgroundVisibleGuildArea, SCRIPT_VARARGS, NULL},
			{"DisableGuildArea", backgroundDisableGuildArea, SCRIPT_VARARGS, NULL},

			{"WarpTest", backgroundWarpTest, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("background", s_methods);

	ScriptModule_AddIntConstant(poModule, "PART_SKY", CMapOutdoor::PART_SKY);
	ScriptModule_AddIntConstant(poModule, "PART_TREE", CMapOutdoor::PART_TREE);
	ScriptModule_AddIntConstant(poModule, "PART_CLOUD", CMapOutdoor::PART_CLOUD);
	ScriptModule_AddIntConstant(poModule, "PART_WATER", CMapOutdoor::PART_WATER);
	ScriptModule_AddIntConstant(poModule, "PART_OBJECT", CMapOutdoor::PART_OBJECT);
	ScriptModule_AddIntConstant(poModule, "PART_TERRAIN", CMapOutdoor::PART_TERRAIN);

	ScriptModule_AddIntConstant(poModule, "SKY_RENDER_MODE_DEFAULT", CSkyObject::SKY_RENDER_MODE_DEFAULT);
	ScriptModule_AddIntConstant(poModule, "SKY_RENDER_MODE_DIFFUSE", CSkyObject::SKY_RENDER_MODE_DIFFUSE);
	ScriptModule_AddIntConstant(poModule, "SKY_RENDER_MODE_TEXTURE", CSkyObject::SKY_RENDER_MODE_TEXTURE);
	ScriptModule_AddIntConstant(poModule, "SKY_RENDER_MODE_MODULATE", CSkyObject::SKY_RENDER_MODE_MODULATE);
	ScriptModule_AddIntConstant(poModule, "SKY_RENDER_MODE_MODULATE2X", CSkyObject::SKY_RENDER_MODE_MODULATE2X);
	ScriptModule_AddIntConstant(poModule, "SKY_RENDER_MODE_MODULATE4X", CSkyObject::SKY_RENDER_MODE_MODULATE4X);

	ScriptModule_AddIntConstant(poModule, "SHADOW_NONE", CPythonBackground::SHADOW_NONE);
	ScriptModule_AddIntConstant(poModule, "SHADOW_GROUND", CPythonBackground::SHADOW_GROUND);
	ScriptModule_AddIntConstant(poModule, "SHADOW_GROUND_AND_SOLO", CPythonBackground::SHADOW_GROUND_AND_SOLO);
	ScriptModule_AddIntConstant(poModule, "SHADOW_ALL", CPythonBackground::SHADOW_ALL);
	ScriptModule_AddIntConstant(poModule, "SHADOW_ALL_HIGH", CPythonBackground::SHADOW_ALL_HIGH);
	ScriptModule_AddIntConstant(poModule, "SHADOW_ALL_MAX", CPythonBackground::SHADOW_ALL_MAX);

	ScriptModule_AddIntConstant(poModule, "DISTANCE0", CPythonBackground::DISTANCE0);
	ScriptModule_AddIntConstant(poModule, "DISTANCE1", CPythonBackground::DISTANCE1);
	ScriptModule_AddIntConstant(poModule, "DISTANCE2", CPythonBackground::DISTANCE2);
	ScriptModule_AddIntConstant(poModule, "DISTANCE3", CPythonBackground::DISTANCE3);
	ScriptModule_AddIntConstant(poModule, "DISTANCE4", CPythonBackground::DISTANCE4);

	ScriptModule_AddIntConstant(poModule, "DISTANCE_SORT", CMapOutdoor::DISTANCE_SORT);
	ScriptModule_AddIntConstant(poModule, "TEXTURE_SORT", CMapOutdoor::TEXTURE_SORT);
}
