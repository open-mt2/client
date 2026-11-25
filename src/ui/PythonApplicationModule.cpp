#include <Client/UI/StdAfx.h>
#include <Client/UI/ImageFile.h>
#include <Client/UI/PythonApplication.h>
#include <Neoeter/Script/ScriptBackend.h>
#include <Neoeter/Render/Camera.h>

extern bool PERF_CHECKER_RENDER_GAME;
extern D3DXCOLOR g_fSpecularColor;
BOOL bVisibleNotice = true;
BOOL bTestServerFlag = FALSE;
int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE = 0;

ScriptObject* appShowWebPage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szWebPage;
	if (!ScriptTuple_GetString(poArgs, 0, &szWebPage))
		return Script_BuildException();

	ScriptObject* poRect = ScriptTuple_GetItem(poArgs, 1);
	if (!ScriptTuple_Check(poRect))
		return Script_BuildException();

	RECT rcWebPage;
	rcWebPage.left = Script_AsLongLong(ScriptTuple_GetItem(poRect, 0));
	rcWebPage.top = Script_AsLongLong(ScriptTuple_GetItem(poRect, 1));
	rcWebPage.right = Script_AsLongLong(ScriptTuple_GetItem(poRect, 2));
	rcWebPage.bottom = Script_AsLongLong(ScriptTuple_GetItem(poRect, 3));

	CPythonApplication::Instance().ShowWebPage(
		szWebPage,
		rcWebPage);
	return Script_BuildNone();
}

ScriptObject* appMoveWebPage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poRect = ScriptTuple_GetItem(poArgs, 0);
	if (!ScriptTuple_Check(poRect))
		return Script_BuildException();

	RECT rcWebPage;
	rcWebPage.left = Script_AsLongLong(ScriptTuple_GetItem(poRect, 0));
	rcWebPage.top = Script_AsLongLong(ScriptTuple_GetItem(poRect, 1));
	rcWebPage.right = Script_AsLongLong(ScriptTuple_GetItem(poRect, 2));
	rcWebPage.bottom = Script_AsLongLong(ScriptTuple_GetItem(poRect, 3));

	CPythonApplication::Instance().MoveWebPage(rcWebPage);
	return Script_BuildNone();
}

ScriptObject* appHideWebPage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().HideWebPage();
	return Script_BuildNone();
}

ScriptObject* appIsWebPageMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().IsWebPageMode());
}

ScriptObject* appEnablePerformanceTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szMode;
	if (!ScriptTuple_GetString(poArgs, 0, &szMode))
		return Script_BuildException();

	int nEnable;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nEnable))
		return Script_BuildException();

	bool isEnable = nEnable ? true : false;

	if (strcmp(szMode, "RENDER_GAME") == 0)
		PERF_CHECKER_RENDER_GAME = isEnable;

	return Script_BuildNone();
}

/////////////////////////////////////////////////////

extern BOOL HAIR_COLOR_ENABLE;
extern BOOL USE_ARMOR_SPECULAR;
extern BOOL USE_WEAPON_SPECULAR;
extern BOOL SKILL_EFFECT_UPGRADE_ENABLE;
extern BOOL RIDE_HORSE_ENABLE;
extern double g_specularSpd;

// TEXTTAIL_LIVINGTIME_CONTROL
extern void TextTail_SetLivingTime(long livingTime);

ScriptObject* appSetTextTailLivingTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float livingTime;
	if (!ScriptTuple_GetFloat(poArgs, 0, &livingTime))
		return Script_BuildException();

	TextTail_SetLivingTime(livingTime * 1000);

	return Script_BuildNone();
}
// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

ScriptObject* appSetHairColorEnable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nEnable))
		return Script_BuildException();

	HAIR_COLOR_ENABLE = nEnable;

	return Script_BuildNone();
}

ScriptObject* appSetArmorSpecularEnable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nEnable))
		return Script_BuildException();

	USE_ARMOR_SPECULAR = nEnable;

	return Script_BuildNone();
}

ScriptObject* appSetWeaponSpecularEnable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nEnable))
		return Script_BuildException();

	USE_WEAPON_SPECULAR = nEnable;

	return Script_BuildNone();
}

ScriptObject* appSetSkillEffectUpgradeEnable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nEnable))
		return Script_BuildException();

	SKILL_EFFECT_UPGRADE_ENABLE = nEnable;

	return Script_BuildNone();
}

ScriptObject* SetTwoHandedWeaponAttSpeedDecreaseValue(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iValue;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iValue))
		return Script_BuildException();

	TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE = iValue;

	return Script_BuildNone();
}

ScriptObject* appSetRideHorseEnable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nEnable))
		return Script_BuildException();

	RIDE_HORSE_ENABLE = nEnable;

	return Script_BuildNone();
}

ScriptObject* appSetCameraMaxDistance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fMax;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fMax))
		return Script_BuildException();

	CCamera::SetCameraMaxDistance(fMax);
	return Script_BuildNone();
}

ScriptObject* appSetControlFP(ScriptObject* poSelf, ScriptObject* poArgs)
{
#ifndef OPENMT2_PLATFORM_LINUX
	_controlfp(_PC_24, _MCW_PC);
#endif
	return Script_BuildNone();
}

ScriptObject* appSetSpecularSpeed(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fSpeed;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fSpeed))
		return Script_BuildException();

	g_specularSpd = fSpeed;

	return Script_BuildNone();
}

ScriptObject* appSetMinFog(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fMinFog;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fMinFog))
		return Script_BuildException();

	CPythonApplication::Instance().SetMinFog(fMinFog);
	return Script_BuildNone();
}

ScriptObject* appSetFrameSkip(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nFrameSkip;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nFrameSkip))
		return Script_BuildException();

	CPythonApplication::Instance().SetFrameSkip(nFrameSkip ? true : false);
	return Script_BuildNone();
}

// LOCALE

ScriptObject* appForceSetLocale(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	char* szLocalePath;
	if (!ScriptTuple_GetString(poArgs, 1, &szLocalePath))
		return Script_BuildException();

	Tracenf("ForceSetLocale(%s, %s) ignored: the client locale is fixed to English", szName, szLocalePath);

	return Script_BuildNone();
}

ScriptObject* appGetLocaleServiceName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("s", LocaleService_GetName());
}

//
bool LoadLocaleData(const char* localePath);

ScriptObject* appSetCHEONMA(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int enable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &enable))
		return Script_BuildException();

	LocaleService_SetCHEONMA(enable ? true : false);
	return Script_BuildNone();
}

ScriptObject* appIsCHEONMA(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", LocaleService_IsCHEONMA());
}

#include <Neoeter/Platform/Base/tea.h>

ScriptObject* appLoadLocaleAddr(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* addrPath;
	if (!ScriptTuple_GetString(poArgs, 0, &addrPath))
		return Script_BuildException();

	FILE* fp = fopen(addrPath, "rb");
	if (!fp)
		return Script_BuildException();

	fseek(fp, 0, SEEK_END);

	int size = ftell(fp);
	char* enc = (char*)_alloca(size);
	fseek(fp, 0, SEEK_SET);
	fread(enc, size, 1, fp);
	fclose(fp);

	static const unsigned char key[16] = {
		0x82, 0x1b, 0x34, 0xae,
		0x12, 0x3b, 0xfb, 0x17,
		0xd7, 0x2c, 0x39, 0xae,
		0x41, 0x98, 0xf1, 0x63};

	char* buf = (char*)_alloca(size);
	// int decSize =
	tea_decrypt((uint32_t*)buf, (const uint32_t*)enc, (const uint32_t*)key, size);
	unsigned int retSize = *(unsigned int*)buf;
	char* ret = buf + sizeof(unsigned int);
	return Script_BuildValue("s#", ret, retSize);
}

ScriptObject* appLoadLocaleData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* localePath;
	if (!ScriptTuple_GetString(poArgs, 0, &localePath))
		return Script_BuildException();

	return Script_BuildValue("i", LoadLocaleData(localePath));
}

ScriptObject* appGetLocaleName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("s", LocaleService_GetLocaleName());
}

ScriptObject* appGetLocalePath(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("s", LocaleService_GetLocalePath());
}
// END_OF_LOCALE

ScriptObject* appGetDefaultCodePage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", LocaleService_GetCodePage());
}

ScriptObject* appGetImageInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	uint32_t uWidth = 0;
	uint32_t uHeight = 0;
	uint32_t uFormat = 0;

	BOOL canLoad = GetImageFileInfo(szFileName, &uWidth, &uHeight, &uFormat) ? TRUE : FALSE;

	return Script_BuildValue("iii", canLoad, uWidth, uHeight);
}

#include <Neoeter/IO/EterPackManager.h>

ScriptObject* appIsExistFile(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	bool isExist = CEterPackManager::Instance().isExist(szFileName);

	return Script_BuildValue("i", isExist);
}

ScriptObject* appGetFileList(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFilter;
	if (!ScriptTuple_GetString(poArgs, 0, &szFilter))
		return Script_BuildException();

	ScriptObject* poList = ScriptList_New(0);

	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(wfd));

	HANDLE hFind = FindFirstFile(szFilter, &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			ScriptObject* poFileName = ScriptString_New(wfd.cFileName);
			ScriptList_Append(poList, poFileName);
		} while (FindNextFile(hFind, &wfd));

		FindClose(hFind);
	}

	return poList;
}

ScriptObject* appUpdateGame(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().UpdateGame();
	return Script_BuildNone();
}

ScriptObject* appRenderGame(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().RenderGame();
	return Script_BuildNone();
}

ScriptObject* appSetMouseHandler(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poHandler;
	if (!ScriptTuple_GetObject(poArgs, 0, &poHandler))
		return Script_BuildException();

	CPythonApplication::Instance().SetMouseHandler(poHandler);
	return Script_BuildNone();
}

ScriptObject* appCreate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	int width;
	if (!ScriptTuple_GetInteger(poArgs, 1, &width))
		return Script_BuildException();

	int height;
	if (!ScriptTuple_GetInteger(poArgs, 2, &height))
		return Script_BuildException();

	int Windowed;
	if (!ScriptTuple_GetInteger(poArgs, 3, &Windowed))
		return Script_BuildException();

	CPythonApplication& rkApp = CPythonApplication::Instance();
	if (!rkApp.Create(poSelf, szName, width, height, Windowed))
	{
		// return Script_BuildNone();
		return NULL;
	}

	return Script_BuildNone();
}

ScriptObject* appLoop(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().Loop();

	return Script_BuildNone();
}

ScriptObject* appGetInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nInfo;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nInfo))
		return Script_BuildException();

	std::string stInfo;
	CPythonApplication::Instance().GetInfo(nInfo, &stInfo);
	return Script_BuildValue("s", stInfo.c_str());
}

ScriptObject* appProcess(ScriptObject* poSelf, ScriptObject* poArgs)
{
	if (CPythonApplication::Instance().Process())
		return Script_BuildValue("i", 1);

	return Script_BuildValue("i", 0);
}

ScriptObject* appAbort(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().Abort();
	return Script_BuildNone();
}

ScriptObject* appExit(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().Exit();
	return Script_BuildNone();
}

ScriptObject* appSetCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float Distance;
	if (!ScriptTuple_GetFloat(poArgs, 0, &Distance))
		return Script_BuildException();

	float Pitch;
	if (!ScriptTuple_GetFloat(poArgs, 1, &Pitch))
		return Script_BuildException();

	float Rotation;
	if (!ScriptTuple_GetFloat(poArgs, 2, &Rotation))
		return Script_BuildException();

	float fDestinationHeight;
	if (!ScriptTuple_GetFloat(poArgs, 3, &fDestinationHeight))
		return Script_BuildException();

	CPythonApplication::Instance().SetCamera(Distance, Pitch, Rotation, fDestinationHeight);
	return Script_BuildNone();
}

ScriptObject* appGetCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float Distance, Pitch, Rotation, DestinationHeight;
	CPythonApplication::Instance().GetCamera(&Distance, &Pitch, &Rotation, &DestinationHeight);

	return Script_BuildValue("ffff", Distance, Pitch, Rotation, DestinationHeight);
}

ScriptObject* appGetCameraPitch(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("f", CPythonApplication::Instance().GetPitch());
}

ScriptObject* appGetCameraRotation(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("f", CPythonApplication::Instance().GetRotation());
}

ScriptObject* appGetTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("f", CPythonApplication::Instance().GetGlobalTime());
}

ScriptObject* appGetGlobalTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetServerTime());
}

ScriptObject* appGetGlobalTimeStamp(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetServerTimeStamp());
}

ScriptObject* appGetUpdateFPS(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetUpdateFPS());
}

ScriptObject* appGetRenderFPS(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetRenderFPS());
}

ScriptObject* appRotateCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDirection;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDirection))
		return Script_BuildException();
	CPythonApplication::Instance().RotateCamera(iDirection);
	return Script_BuildNone();
}

ScriptObject* appPitchCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDirection;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDirection))
		return Script_BuildException();
	CPythonApplication::Instance().PitchCamera(iDirection);
	return Script_BuildNone();
}

ScriptObject* appZoomCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDirection;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDirection))
		return Script_BuildException();
	CPythonApplication::Instance().ZoomCamera(iDirection);
	return Script_BuildNone();
}

ScriptObject* appMovieRotateCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDirection;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDirection))
		return Script_BuildException();
	CPythonApplication::Instance().MovieRotateCamera(iDirection);
	return Script_BuildNone();
}

ScriptObject* appMoviePitchCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDirection;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDirection))
		return Script_BuildException();
	CPythonApplication::Instance().MoviePitchCamera(iDirection);
	return Script_BuildNone();
}

ScriptObject* appMovieZoomCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDirection;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDirection))
		return Script_BuildException();
	CPythonApplication::Instance().MovieZoomCamera(iDirection);
	return Script_BuildNone();
}

ScriptObject* appMovieResetCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().MovieResetCamera();
	return Script_BuildNone();
}

ScriptObject* appGetFaceSpeed(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("f", CPythonApplication::Instance().GetFaceSpeed());
}

ScriptObject* appGetRenderTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("fi",
							 CPythonApplication::Instance().GetAveRenderTime(),
							 CPythonApplication::Instance().GetCurRenderTime());
}

ScriptObject* appGetUpdateTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetCurUpdateTime());
}

ScriptObject* appGetLoad(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetLoad());
}
ScriptObject* appGetFaceCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetFaceCount());
}

ScriptObject* appGetAvaiableTextureMememory(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CGraphicBase::GetAvailableTextureMemory());
}

ScriptObject* appSetFPS(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFPS;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFPS))
		return Script_BuildException();

	CPythonApplication::Instance().SetFPS(iFPS);

	return Script_BuildNone();
}

ScriptObject* appSetGlobalCenterPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int x;
	if (!ScriptTuple_GetInteger(poArgs, 0, &x))
		return Script_BuildException();

	int y;
	if (!ScriptTuple_GetInteger(poArgs, 1, &y))
		return Script_BuildException();

	CPythonApplication::Instance().SetGlobalCenterPosition(x, y);
	return Script_BuildNone();
}

ScriptObject* appSetCenterPosition(ScriptObject* poSelf, ScriptObject* poArgs)
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

	CPythonApplication::Instance().SetCenterPosition(fx, -fy, fz);
	return Script_BuildNone();
}

ScriptObject* appGetCursorPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	long lx, ly;
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.GetMousePosition(lx, ly);

	return Script_BuildValue("ii", lx, ly);
}

ScriptObject* appRunPythonFile(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	bool ret = Script_RunFile(szFileName);
	return Script_BuildValue("i", ret);
}

ScriptObject* appIsPressed(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iKey;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iKey))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonApplication::Instance().IsPressed(iKey));
}

ScriptObject* appSetCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	/*
		char * szName;
		if (!ScriptTuple_GetString(poArgs, 0, &szName))
			return Script_BuildException();

		if (!CPythonApplication::Instance().SetHardwareCursor(szName))
			return Script_BuildException("Wrong Cursor Name [%s]", szName);
	*/
	int iCursorNum;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iCursorNum))
		return Script_BuildException();

	if (!CPythonApplication::Instance().SetCursorNum(iCursorNum))
		return Script_BuildException("Wrong Cursor Name [%d]", iCursorNum);

	return Script_BuildNone();
}

ScriptObject* appGetCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonApplication::Instance().GetCursorNum());
}

ScriptObject* appShowCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().SetCursorVisible(TRUE);

	return Script_BuildNone();
}

ScriptObject* appHideCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().SetCursorVisible(FALSE);

	return Script_BuildNone();
}

ScriptObject* appIsShowCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", TRUE == CPythonApplication::Instance().GetCursorVisible());
}

ScriptObject* appIsLiarCursorOn(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", TRUE == CPythonApplication::Instance().GetLiarCursorOn());
}

ScriptObject* appSetSoftwareCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_SOFTWARE);
	return Script_BuildNone();
}

ScriptObject* appSetHardwareCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_HARDWARE);
	return Script_BuildNone();
}

ScriptObject* appSetConnectData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szIP;
	if (!ScriptTuple_GetString(poArgs, 0, &szIP))
		return Script_BuildException();

	int iPort;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPort))
		return Script_BuildException();

	CPythonApplication::Instance().SetConnectData(szIP, iPort);

	return Script_BuildNone();
}

ScriptObject* appGetConnectData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	std::string strIP;
	int iPort;

	CPythonApplication::Instance().GetConnectData(strIP, iPort);

	return Script_BuildValue("si", strIP.c_str(), iPort);
}

ScriptObject* appGetRandom(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int from;
	if (!ScriptTuple_GetInteger(poArgs, 0, &from))
		return Script_BuildException();

	int to;
	if (!ScriptTuple_GetInteger(poArgs, 1, &to))
		return Script_BuildException();

	if (from > to)
	{
		int tmp = from;
		from = to;
		to = tmp;
	}

	return Script_BuildValue("i", random_range(from, to));
}

ScriptObject* appGetRotatingDirection(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fSource;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fSource))
		return Script_BuildException();
	float fTarget;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fTarget))
		return Script_BuildException();

	return Script_BuildValue("i", GetRotatingDirection(fSource, fTarget));
}

ScriptObject* appGetDegreeDifference(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fSource;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fSource))
		return Script_BuildException();
	float fTarget;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fTarget))
		return Script_BuildException();

	return Script_BuildValue("f", GetDegreeDifference(fSource, fTarget));
}

ScriptObject* appSleep(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iTime;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iTime))
		return Script_BuildException();

	Sleep(iTime);

	return Script_BuildNone();
}

ScriptObject* appSetDefaultFontName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFontName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFontName))
		return Script_BuildException();

	// DEFAULT_FONT
	DefaultFont_SetName(szFontName);
	// END_OF_DEFAULT_FONT

	return Script_BuildNone();
}

ScriptObject* appSetGuildSymbolPath(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szPathName;
	if (!ScriptTuple_GetString(poArgs, 0, &szPathName))
		return Script_BuildException();

	SetGuildSymbolPath(szPathName);

	return Script_BuildNone();
}

ScriptObject* appEnableSpecialCameraMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().EnableSpecialCameraMode();
	return Script_BuildNone();
}

ScriptObject* appSetCameraSpeed(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPercentage;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPercentage))
		return Script_BuildException();

	CPythonApplication::Instance().SetCameraSpeed(iPercentage);

	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (pCamera)
		pCamera->SetResistance(float(iPercentage) / 100.0f);
	return Script_BuildNone();
}

ScriptObject* appIsFileExist(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	return Script_BuildValue("i", -1 != _access(szFileName, 0));
}

ScriptObject* appSetCameraSetting(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ix;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ix))
		return Script_BuildException();
	int iy;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iy))
		return Script_BuildException();
	int iz;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iz))
		return Script_BuildException();

	int iZoom;
	if (!ScriptTuple_GetInteger(poArgs, 3, &iZoom))
		return Script_BuildException();
	int iRotation;
	if (!ScriptTuple_GetInteger(poArgs, 4, &iRotation))
		return Script_BuildException();
	int iPitch;
	if (!ScriptTuple_GetInteger(poArgs, 5, &iPitch))
		return Script_BuildException();

	CPythonApplication::SCameraSetting CameraSetting = {};
	CameraSetting.v3CenterPosition.x = float(ix);
	CameraSetting.v3CenterPosition.y = float(iy);
	CameraSetting.v3CenterPosition.z = float(iz);
	CameraSetting.fZoom = float(iZoom);
	CameraSetting.fRotation = float(iRotation);
	CameraSetting.fPitch = float(iPitch);
	CPythonApplication::Instance().SetEventCamera(CameraSetting);
	return Script_BuildNone();
}

ScriptObject* appSaveCameraSetting(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CPythonApplication::Instance().SaveCameraSetting(szFileName);
	return Script_BuildNone();
}

ScriptObject* appLoadCameraSetting(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	bool bResult = CPythonApplication::Instance().LoadCameraSetting(szFileName);
	return Script_BuildValue("i", bResult);
}

ScriptObject* appSetDefaultCamera(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonApplication::Instance().SetDefaultCamera();
	return Script_BuildNone();
}

ScriptObject* appSetSightRange(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iRange;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iRange))
		return Script_BuildException();

	CPythonApplication::Instance().SetForceSightRange(iRange);
	return Script_BuildNone();
}

extern int g_iAccumulationTime;

ScriptObject* apptestGetAccumulationTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", g_iAccumulationTime);
}

ScriptObject* apptestResetAccumulationTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	g_iAccumulationTime = 0;
	return Script_BuildNone();
}

ScriptObject* apptestSetSpecularColor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fr;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fr))
		return Script_BuildException();
	float fg;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fg))
		return Script_BuildException();
	float fb;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fb))
		return Script_BuildException();
	g_fSpecularColor = D3DXCOLOR(fr, fg, fb, 1.0f);
	return Script_BuildNone();
}

ScriptObject* appSetVisibleNotice(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();
	bVisibleNotice = iFlag;
	return Script_BuildNone();
}

ScriptObject* appIsVisibleNotice(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", bVisibleNotice);
}

ScriptObject* appEnableTestServerFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	bTestServerFlag = TRUE;
	return Script_BuildNone();
}

ScriptObject* appIsEnableTestServerFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", bTestServerFlag);
}

class CTextLineLoader
{
	public:
		CTextLineLoader(const char* c_szFileName)
		{
			CEterPackManager::TPackDataPtr data;
			if (!CEterPackManager::Instance().Get(c_szFileName, data))
				return;

			m_kTextFileLoader.Bind(data->size(), data->data());
		}

		DWORD GetLineCount()
		{
			return m_kTextFileLoader.GetLineCount();
		}

		const char* GetLine(DWORD dwIndex)
		{
			if (dwIndex >= GetLineCount())
				return "";

			return m_kTextFileLoader.GetLineString(dwIndex).c_str();
		}

	protected:
		CMemoryTextFileLoader m_kTextFileLoader;
};

ScriptObject* appOpenTextFile(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CTextLineLoader* pTextLineLoader = new CTextLineLoader(szFileName);

	return Script_BuildHandle(pTextLineLoader);
}

ScriptObject* appCloseTextFile(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CTextLineLoader* pTextFileLoader;
	if (!ScriptTuple_GetHandle(poArgs, 0, &pTextFileLoader))
		return Script_BuildException();

	delete pTextFileLoader;

	return Script_BuildNone();
}

ScriptObject* appGetTextFileLineCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CTextLineLoader* pTextFileLoader;
	if (!ScriptTuple_GetHandle(poArgs, 0, &pTextFileLoader))
		return Script_BuildException();

	return Script_BuildValue("i", pTextFileLoader->GetLineCount());
}

ScriptObject* appGetTextFileLine(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CTextLineLoader* pTextFileLoader;
	if (!ScriptTuple_GetHandle(poArgs, 0, &pTextFileLoader))
		return Script_BuildException();
	int iLineIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iLineIndex))
		return Script_BuildException();

	return Script_BuildValue("s", pTextFileLoader->GetLine(iLineIndex));
}

ScriptObject* appSetGuildMarkPath(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* path;
	if (!ScriptTuple_GetString(poArgs, 0, &path))
		return Script_BuildException();

	char newPath[256];
	char* ext = strstr(path, ".tga");

	if (ext)
	{
		int extPos = ext - path;
		strncpy(newPath, path, extPos);
		newPath[extPos] = '\0';
	}
	else
		strncpy(newPath, path, sizeof(newPath) - 1);

	CGuildMarkManager::Instance().SetMarkPathPrefix(newPath);
	return Script_BuildNone();
}

ScriptObject* appIsDevStage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIsDevelopmentStage = 0;
#if defined(LOCALE_SERVICE_STAGE_DEVELOPMENT)
	nIsDevelopmentStage = 1;
#endif
	return Script_BuildValue("i", nIsDevelopmentStage);
}

ScriptObject* appIsTestStage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIsTestStage = 0;
#if defined(LOCALE_SERVICE_STAGE_TEST)
	nIsTestStage = 1;
#endif
	return Script_BuildValue("i", nIsTestStage);
}

ScriptObject* appIsLiveStage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIsLiveStage = 0;
#if !defined(LOCALE_SERVICE_STAGE_TEST) && !defined(LOCALE_SERVICE_STAGE_DEVELOPMENT)
	nIsLiveStage = 1;
#endif
	return Script_BuildValue("i", nIsLiveStage);
}

ScriptObject* appLogoOpen(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	int nIsSuccess = 1; // CPythonApplication::Instance().OnLogoOpen(szName);
	CMovieMan::Instance().PlayLogo(szName);

	return Script_BuildValue("i", nIsSuccess);
}

ScriptObject* appLogoUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIsRun = 0; // CPythonApplication::Instance().OnLogoUpdate();
	return Script_BuildValue("i", nIsRun);
}

ScriptObject* appLogoRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	// CPythonApplication::Instance().OnLogoRender();
	return Script_BuildNone();
}

ScriptObject* appLogoClose(ScriptObject* poSelf, ScriptObject* poArgs)
{
	// CPythonApplication::Instance().OnLogoClose();
	return Script_BuildNone();
}

void initapp()
{
	static ScriptMethodDef s_methods[] =
		{
			{"IsDevStage", appIsDevStage, SCRIPT_VARARGS, NULL},
			{"IsTestStage", appIsTestStage, SCRIPT_VARARGS, NULL},
			{"IsLiveStage", appIsLiveStage, SCRIPT_VARARGS, NULL},

			// TEXTTAIL_LIVINGTIME_CONTROL
			{"SetTextTailLivingTime", appSetTextTailLivingTime, SCRIPT_VARARGS, NULL},
			// END_OF_TEXTTAIL_LIVINGTIME_CONTROL

			{"EnablePerformanceTime", appEnablePerformanceTime, SCRIPT_VARARGS, NULL},
			{"SetHairColorEnable", appSetHairColorEnable, SCRIPT_VARARGS, NULL},

			{"SetArmorSpecularEnable", appSetArmorSpecularEnable, SCRIPT_VARARGS, NULL},
			{"SetWeaponSpecularEnable", appSetWeaponSpecularEnable, SCRIPT_VARARGS, NULL},
			{"SetSkillEffectUpgradeEnable", appSetSkillEffectUpgradeEnable, SCRIPT_VARARGS, NULL},
			{"SetTwoHandedWeaponAttSpeedDecreaseValue", SetTwoHandedWeaponAttSpeedDecreaseValue, SCRIPT_VARARGS, NULL},
			{"SetRideHorseEnable", appSetRideHorseEnable, SCRIPT_VARARGS, NULL},

			{"SetCameraMaxDistance", appSetCameraMaxDistance, SCRIPT_VARARGS, NULL},
			{"SetMinFog", appSetMinFog, SCRIPT_VARARGS, NULL},
			{"SetFrameSkip", appSetFrameSkip, SCRIPT_VARARGS, NULL},
			{"GetImageInfo", appGetImageInfo, SCRIPT_VARARGS, NULL},
			{"GetInfo", appGetInfo, SCRIPT_VARARGS, NULL},
			{"UpdateGame", appUpdateGame, SCRIPT_VARARGS, NULL},
			{"RenderGame", appRenderGame, SCRIPT_VARARGS, NULL},
			{"Loop", appLoop, SCRIPT_VARARGS, NULL},
			{"Create", appCreate, SCRIPT_VARARGS, NULL},
			{"Process", appProcess, SCRIPT_VARARGS, NULL},
			{"Exit", appExit, SCRIPT_VARARGS, NULL},
			{"Abort", appAbort, SCRIPT_VARARGS, NULL},
			{"SetMouseHandler", appSetMouseHandler, SCRIPT_VARARGS, NULL},
			{"IsExistFile", appIsExistFile, SCRIPT_VARARGS, NULL},
			{"GetFileList", appGetFileList, SCRIPT_VARARGS, NULL},

			{"SetCamera", appSetCamera, SCRIPT_VARARGS, NULL},
			{"GetCamera", appGetCamera, SCRIPT_VARARGS, NULL},
			{"GetCameraPitch", appGetCameraPitch, SCRIPT_VARARGS, NULL},
			{"GetCameraRotation", appGetCameraRotation, SCRIPT_VARARGS, NULL},
			{"GetTime", appGetTime, SCRIPT_VARARGS, NULL},
			{"GetGlobalTime", appGetGlobalTime, SCRIPT_VARARGS, NULL},
			{"GetGlobalTimeStamp", appGetGlobalTimeStamp, SCRIPT_VARARGS, NULL},
			{"GetUpdateFPS", appGetUpdateFPS, SCRIPT_VARARGS, NULL},
			{"GetRenderFPS", appGetRenderFPS, SCRIPT_VARARGS, NULL},
			{"RotateCamera", appRotateCamera, SCRIPT_VARARGS, NULL},
			{"PitchCamera", appPitchCamera, SCRIPT_VARARGS, NULL},
			{"ZoomCamera", appZoomCamera, SCRIPT_VARARGS, NULL},
			{"MovieRotateCamera", appMovieRotateCamera, SCRIPT_VARARGS, NULL},
			{"MoviePitchCamera", appMoviePitchCamera, SCRIPT_VARARGS, NULL},
			{"MovieZoomCamera", appMovieZoomCamera, SCRIPT_VARARGS, NULL},
			{"MovieResetCamera", appMovieResetCamera, SCRIPT_VARARGS, NULL},

			{"GetAvailableTextureMemory", appGetAvaiableTextureMememory, SCRIPT_VARARGS, NULL},
			{"GetRenderTime", appGetRenderTime, SCRIPT_VARARGS, NULL},
			{"GetUpdateTime", appGetUpdateTime, SCRIPT_VARARGS, NULL},
			{"GetLoad", appGetLoad, SCRIPT_VARARGS, NULL},
			{"GetFaceSpeed", appGetFaceSpeed, SCRIPT_VARARGS, NULL},
			{"GetFaceCount", appGetFaceCount, SCRIPT_VARARGS, NULL},
			{"SetFPS", appSetFPS, SCRIPT_VARARGS, NULL},
			{"SetGlobalCenterPosition", appSetGlobalCenterPosition, SCRIPT_VARARGS, NULL},
			{"SetCenterPosition", appSetCenterPosition, SCRIPT_VARARGS, NULL},
			{"GetCursorPosition", appGetCursorPosition, SCRIPT_VARARGS, NULL},

			{"GetRandom", appGetRandom, SCRIPT_VARARGS, NULL},
			{"RunPythonFile", appRunPythonFile, SCRIPT_VARARGS, NULL},
			{"IsWebPageMode", appIsWebPageMode, SCRIPT_VARARGS, NULL},
			{"ShowWebPage", appShowWebPage, SCRIPT_VARARGS, NULL},
			{"MoveWebPage", appMoveWebPage, SCRIPT_VARARGS, NULL},
			{"HideWebPage", appHideWebPage, SCRIPT_VARARGS, NULL},
			{"IsPressed", appIsPressed, SCRIPT_VARARGS, NULL},
			{"SetCursor", appSetCursor, SCRIPT_VARARGS, NULL},
			{"GetCursor", appGetCursor, SCRIPT_VARARGS, NULL},
			{"ShowCursor", appShowCursor, SCRIPT_VARARGS, NULL},
			{"HideCursor", appHideCursor, SCRIPT_VARARGS, NULL},
			{"IsShowCursor", appIsShowCursor, SCRIPT_VARARGS, NULL},
			{"IsLiarCursorOn", appIsLiarCursorOn, SCRIPT_VARARGS, NULL},
			{"SetSoftwareCursor", appSetSoftwareCursor, SCRIPT_VARARGS, NULL},
			{"SetHardwareCursor", appSetHardwareCursor, SCRIPT_VARARGS, NULL},

			{"SetConnectData", appSetConnectData, SCRIPT_VARARGS, NULL},
			{"GetConnectData", appGetConnectData, SCRIPT_VARARGS, NULL},

			{"GetRotatingDirection", appGetRotatingDirection, SCRIPT_VARARGS, NULL},
			{"GetDegreeDifference", appGetDegreeDifference, SCRIPT_VARARGS, NULL},
			{"Sleep", appSleep, SCRIPT_VARARGS, NULL},
			{"SetDefaultFontName", appSetDefaultFontName, SCRIPT_VARARGS, NULL},
			{"SetGuildSymbolPath", appSetGuildSymbolPath, SCRIPT_VARARGS, NULL},

			{"EnableSpecialCameraMode", appEnableSpecialCameraMode, SCRIPT_VARARGS, NULL},
			{"SetCameraSpeed", appSetCameraSpeed, SCRIPT_VARARGS, NULL},

			{"SaveCameraSetting", appSaveCameraSetting, SCRIPT_VARARGS, NULL},
			{"LoadCameraSetting", appLoadCameraSetting, SCRIPT_VARARGS, NULL},
			{"SetDefaultCamera", appSetDefaultCamera, SCRIPT_VARARGS, NULL},
			{"SetCameraSetting", appSetCameraSetting, SCRIPT_VARARGS, NULL},

			{"SetSightRange", appSetSightRange, SCRIPT_VARARGS, NULL},

			{"IsFileExist", appIsFileExist, SCRIPT_VARARGS, NULL},
			{"OpenTextFile", appOpenTextFile, SCRIPT_VARARGS, NULL},
			{"CloseTextFile", appCloseTextFile, SCRIPT_VARARGS, NULL},
			{"GetTextFileLineCount", appGetTextFileLineCount, SCRIPT_VARARGS, NULL},
			{"GetTextFileLine", appGetTextFileLine, SCRIPT_VARARGS, NULL},

			// LOCALE
			{"GetLocaleServiceName", appGetLocaleServiceName, SCRIPT_VARARGS, NULL},
			{"GetLocaleName", appGetLocaleName, SCRIPT_VARARGS, NULL},
			{"GetLocalePath", appGetLocalePath, SCRIPT_VARARGS, NULL},
			{"ForceSetLocale", appForceSetLocale, SCRIPT_VARARGS, NULL},
			// END_OF_LOCALE

			// CHEONMA
			{"LoadLocaleAddr", appLoadLocaleAddr, SCRIPT_VARARGS, NULL},
			{"LoadLocaleData", appLoadLocaleData, SCRIPT_VARARGS, NULL},
			{"SetCHEONMA", appSetCHEONMA, SCRIPT_VARARGS, NULL},
			{"IsCHEONMA", appIsCHEONMA, SCRIPT_VARARGS, NULL},
			// END_OF_CHEONMA

			{"GetDefaultCodePage", appGetDefaultCodePage, SCRIPT_VARARGS, NULL},
			{"SetControlFP", appSetControlFP, SCRIPT_VARARGS, NULL},
			{"SetSpecularSpeed", appSetSpecularSpeed, SCRIPT_VARARGS, NULL},

			{"testGetAccumulationTime", apptestGetAccumulationTime, SCRIPT_VARARGS, NULL},
			{"testResetAccumulationTime", apptestResetAccumulationTime, SCRIPT_VARARGS, NULL},
			{"testSetSpecularColor", apptestSetSpecularColor, SCRIPT_VARARGS, NULL},

			{"SetVisibleNotice", appSetVisibleNotice, SCRIPT_VARARGS, NULL},
			{"IsVisibleNotice", appIsVisibleNotice, SCRIPT_VARARGS, NULL},
			{"EnableTestServerFlag", appEnableTestServerFlag, SCRIPT_VARARGS, NULL},
			{"IsEnableTestServerFlag", appIsEnableTestServerFlag, SCRIPT_VARARGS, NULL},

			{"SetGuildMarkPath", appSetGuildMarkPath, SCRIPT_VARARGS, NULL},

			{"OnLogoUpdate", appLogoUpdate, SCRIPT_VARARGS, NULL},
			{"OnLogoRender", appLogoRender, SCRIPT_VARARGS, NULL},
			{"OnLogoOpen", appLogoOpen, SCRIPT_VARARGS, NULL},
			{"OnLogoClose", appLogoClose, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("app", s_methods);

	ScriptModule_AddIntConstant(poModule, "INFO_ITEM", CPythonApplication::INFO_ITEM);
	ScriptModule_AddIntConstant(poModule, "INFO_ACTOR", CPythonApplication::INFO_ACTOR);
	ScriptModule_AddIntConstant(poModule, "INFO_EFFECT", CPythonApplication::INFO_EFFECT);
	ScriptModule_AddIntConstant(poModule, "INFO_TEXTTAIL", CPythonApplication::INFO_TEXTTAIL);

	ScriptModule_AddIntConstant(poModule, "DEGREE_DIRECTION_SAME", DEGREE_DIRECTION_SAME);
	ScriptModule_AddIntConstant(poModule, "DEGREE_DIRECTION_RIGHT", DEGREE_DIRECTION_RIGHT);
	ScriptModule_AddIntConstant(poModule, "DEGREE_DIRECTION_LEFT", DEGREE_DIRECTION_LEFT);

	ScriptModule_AddIntConstant(poModule, "VK_LEFT", VK_LEFT);
	ScriptModule_AddIntConstant(poModule, "VK_RIGHT", VK_RIGHT);
	ScriptModule_AddIntConstant(poModule, "VK_UP", VK_UP);
	ScriptModule_AddIntConstant(poModule, "VK_DOWN", VK_DOWN);
	ScriptModule_AddIntConstant(poModule, "VK_HOME", VK_HOME);
	ScriptModule_AddIntConstant(poModule, "VK_END", VK_END);
	ScriptModule_AddIntConstant(poModule, "VK_DELETE", VK_DELETE);

	ScriptModule_AddIntConstant(poModule, "DIK_ESCAPE", DIK_ESCAPE);
	ScriptModule_AddIntConstant(poModule, "DIK_ESC", DIK_ESCAPE); // 편의를 위해
	ScriptModule_AddIntConstant(poModule, "DIK_1", DIK_1);
	ScriptModule_AddIntConstant(poModule, "DIK_2", DIK_2);
	ScriptModule_AddIntConstant(poModule, "DIK_3", DIK_3);
	ScriptModule_AddIntConstant(poModule, "DIK_4", DIK_4);
	ScriptModule_AddIntConstant(poModule, "DIK_5", DIK_5);
	ScriptModule_AddIntConstant(poModule, "DIK_6", DIK_6);
	ScriptModule_AddIntConstant(poModule, "DIK_7", DIK_7);
	ScriptModule_AddIntConstant(poModule, "DIK_8", DIK_8);
	ScriptModule_AddIntConstant(poModule, "DIK_9", DIK_9);
	ScriptModule_AddIntConstant(poModule, "DIK_0", DIK_0);
	ScriptModule_AddIntConstant(poModule, "DIK_MINUS", DIK_MINUS); /* - on main keyboard */
	ScriptModule_AddIntConstant(poModule, "DIK_EQUALS", DIK_EQUALS);
	ScriptModule_AddIntConstant(poModule, "DIK_BACK", DIK_BACK); /* backspace */
	ScriptModule_AddIntConstant(poModule, "DIK_TAB", DIK_TAB);
	ScriptModule_AddIntConstant(poModule, "DIK_Q", DIK_Q);
	ScriptModule_AddIntConstant(poModule, "DIK_W", DIK_W);
	ScriptModule_AddIntConstant(poModule, "DIK_E", DIK_E);
	ScriptModule_AddIntConstant(poModule, "DIK_R", DIK_R);
	ScriptModule_AddIntConstant(poModule, "DIK_T", DIK_T);
	ScriptModule_AddIntConstant(poModule, "DIK_Y", DIK_Y);
	ScriptModule_AddIntConstant(poModule, "DIK_U", DIK_U);
	ScriptModule_AddIntConstant(poModule, "DIK_I", DIK_I);
	ScriptModule_AddIntConstant(poModule, "DIK_O", DIK_O);
	ScriptModule_AddIntConstant(poModule, "DIK_P", DIK_P);
	ScriptModule_AddIntConstant(poModule, "DIK_LBRACKET", DIK_LBRACKET);
	ScriptModule_AddIntConstant(poModule, "DIK_RBRACKET", DIK_RBRACKET);
	ScriptModule_AddIntConstant(poModule, "DIK_RETURN", DIK_RETURN); /* Enter on main keyboard */
	ScriptModule_AddIntConstant(poModule, "DIK_LCONTROL", DIK_LCONTROL);
	ScriptModule_AddIntConstant(poModule, "DIK_A", DIK_A);
	ScriptModule_AddIntConstant(poModule, "DIK_S", DIK_S);
	ScriptModule_AddIntConstant(poModule, "DIK_D", DIK_D);
	ScriptModule_AddIntConstant(poModule, "DIK_F", DIK_F);
	ScriptModule_AddIntConstant(poModule, "DIK_G", DIK_G);
	ScriptModule_AddIntConstant(poModule, "DIK_H", DIK_H);
	ScriptModule_AddIntConstant(poModule, "DIK_J", DIK_J);
	ScriptModule_AddIntConstant(poModule, "DIK_K", DIK_K);
	ScriptModule_AddIntConstant(poModule, "DIK_L", DIK_L);
	ScriptModule_AddIntConstant(poModule, "DIK_SEMICOLON", DIK_SEMICOLON);
	ScriptModule_AddIntConstant(poModule, "DIK_APOSTROPHE", DIK_APOSTROPHE);
	ScriptModule_AddIntConstant(poModule, "DIK_GRAVE", DIK_GRAVE); /* accent grave */
	ScriptModule_AddIntConstant(poModule, "DIK_LSHIFT", DIK_LSHIFT);
	ScriptModule_AddIntConstant(poModule, "DIK_BACKSLASH", DIK_BACKSLASH);
	ScriptModule_AddIntConstant(poModule, "DIK_Z", DIK_Z);
	ScriptModule_AddIntConstant(poModule, "DIK_X", DIK_X);
	ScriptModule_AddIntConstant(poModule, "DIK_C", DIK_C);
	ScriptModule_AddIntConstant(poModule, "DIK_V", DIK_V);
	ScriptModule_AddIntConstant(poModule, "DIK_B", DIK_B);
	ScriptModule_AddIntConstant(poModule, "DIK_N", DIK_N);
	ScriptModule_AddIntConstant(poModule, "DIK_M", DIK_M);
	ScriptModule_AddIntConstant(poModule, "DIK_COMMA", DIK_COMMA);
	ScriptModule_AddIntConstant(poModule, "DIK_PERIOD", DIK_PERIOD); /* . on main keyboard */
	ScriptModule_AddIntConstant(poModule, "DIK_SLASH", DIK_SLASH);	 /* / on main keyboard */
	ScriptModule_AddIntConstant(poModule, "DIK_RSHIFT", DIK_RSHIFT);
	ScriptModule_AddIntConstant(poModule, "DIK_MULTIPLY", DIK_MULTIPLY); /* * on numeric keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_LALT", DIK_LMENU);		 /* left Alt */
	ScriptModule_AddIntConstant(poModule, "DIK_SPACE", DIK_SPACE);
	ScriptModule_AddIntConstant(poModule, "DIK_CAPITAL", DIK_CAPITAL);
	ScriptModule_AddIntConstant(poModule, "DIK_F1", DIK_F1);
	ScriptModule_AddIntConstant(poModule, "DIK_F2", DIK_F2);
	ScriptModule_AddIntConstant(poModule, "DIK_F3", DIK_F3);
	ScriptModule_AddIntConstant(poModule, "DIK_F4", DIK_F4);
	ScriptModule_AddIntConstant(poModule, "DIK_F5", DIK_F5);
	ScriptModule_AddIntConstant(poModule, "DIK_F6", DIK_F6);
	ScriptModule_AddIntConstant(poModule, "DIK_F7", DIK_F7);
	ScriptModule_AddIntConstant(poModule, "DIK_F8", DIK_F8);
	ScriptModule_AddIntConstant(poModule, "DIK_F9", DIK_F9);
	ScriptModule_AddIntConstant(poModule, "DIK_F10", DIK_F10);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMLOCK", DIK_NUMLOCK);
	ScriptModule_AddIntConstant(poModule, "DIK_SCROLL", DIK_SCROLL); /* Scroll Lock */
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD7", DIK_NUMPAD7);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD8", DIK_NUMPAD8);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD9", DIK_NUMPAD9);
	ScriptModule_AddIntConstant(poModule, "DIK_SUBTRACT", DIK_SUBTRACT); /* - on numeric keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD4", DIK_NUMPAD4);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD5", DIK_NUMPAD5);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD6", DIK_NUMPAD6);
	ScriptModule_AddIntConstant(poModule, "DIK_ADD", DIK_ADD); /* + on numeric keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD1", DIK_NUMPAD1);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD2", DIK_NUMPAD2);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD3", DIK_NUMPAD3);
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPAD0", DIK_NUMPAD0);
	ScriptModule_AddIntConstant(poModule, "DIK_DECIMAL", DIK_DECIMAL); /* . on numeric keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_F11", DIK_F11);
	ScriptModule_AddIntConstant(poModule, "DIK_F12", DIK_F12);
	ScriptModule_AddIntConstant(poModule, "DIK_NEXTTRACK", DIK_NEXTTRACK);	   /* Next Track */
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPADENTER", DIK_NUMPADENTER); /* Enter on numeric keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_RCONTROL", DIK_RCONTROL);
	ScriptModule_AddIntConstant(poModule, "DIK_MUTE", DIK_MUTE);			   /* Mute */
	ScriptModule_AddIntConstant(poModule, "DIK_CALCULATOR", DIK_CALCULATOR);   /* Calculator */
	ScriptModule_AddIntConstant(poModule, "DIK_PLAYPAUSE", DIK_PLAYPAUSE);	   /* Play / Pause */
	ScriptModule_AddIntConstant(poModule, "DIK_MEDIASTOP", DIK_MEDIASTOP);	   /* Media Stop */
	ScriptModule_AddIntConstant(poModule, "DIK_VOLUMEDOWN", DIK_VOLUMEDOWN);   /* Volume - */
	ScriptModule_AddIntConstant(poModule, "DIK_VOLUMEUP", DIK_VOLUMEUP);	   /* Volume + */
	ScriptModule_AddIntConstant(poModule, "DIK_WEBHOME", DIK_WEBHOME);		   /* Web home */
	ScriptModule_AddIntConstant(poModule, "DIK_NUMPADCOMMA", DIK_NUMPADCOMMA); /* , on numeric keypad (NEC PC98) */
	ScriptModule_AddIntConstant(poModule, "DIK_DIVIDE", DIK_DIVIDE);		   /* / on numeric keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_SYSRQ", DIK_SYSRQ);
	ScriptModule_AddIntConstant(poModule, "DIK_RALT", DIK_RMENU);	 /* right Alt */
	ScriptModule_AddIntConstant(poModule, "DIK_PAUSE", DIK_PAUSE);	 /* Pause */
	ScriptModule_AddIntConstant(poModule, "DIK_HOME", DIK_HOME);	 /* Home on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_UP", DIK_UP);		 /* UpArrow on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_PGUP", DIK_PRIOR);	 /* PgUp on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_LEFT", DIK_LEFT);	 /* LeftArrow on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_RIGHT", DIK_RIGHT);	 /* RightArrow on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_END", DIK_END);		 /* End on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_DOWN", DIK_DOWN);	 /* DownArrow on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_PGDN", DIK_NEXT);	 /* PgDn on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_INSERT", DIK_INSERT); /* Insert on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_DELETE", DIK_DELETE); /* Delete on arrow keypad */
	ScriptModule_AddIntConstant(poModule, "DIK_LWIN", DIK_LWIN);	 /* Left Windows key */
	ScriptModule_AddIntConstant(poModule, "DIK_RWIN", DIK_RWIN);	 /* Right Windows key */
	ScriptModule_AddIntConstant(poModule, "DIK_APPS", DIK_APPS);	 /* AppMenu key */

	// Cursor
	ScriptModule_AddIntConstant(poModule, "NORMAL", CPythonApplication::CURSOR_SHAPE_NORMAL);
	ScriptModule_AddIntConstant(poModule, "ATTACK", CPythonApplication::CURSOR_SHAPE_ATTACK);
	ScriptModule_AddIntConstant(poModule, "TARGET", CPythonApplication::CURSOR_SHAPE_TARGET);
	ScriptModule_AddIntConstant(poModule, "TALK", CPythonApplication::CURSOR_SHAPE_TALK);
	ScriptModule_AddIntConstant(poModule, "CANT_GO", CPythonApplication::CURSOR_SHAPE_CANT_GO);
	ScriptModule_AddIntConstant(poModule, "PICK", CPythonApplication::CURSOR_SHAPE_PICK);

	ScriptModule_AddIntConstant(poModule, "DOOR", CPythonApplication::CURSOR_SHAPE_DOOR);
	ScriptModule_AddIntConstant(poModule, "CHAIR", CPythonApplication::CURSOR_SHAPE_CHAIR);
	ScriptModule_AddIntConstant(poModule, "MAGIC", CPythonApplication::CURSOR_SHAPE_MAGIC);
	ScriptModule_AddIntConstant(poModule, "BUY", CPythonApplication::CURSOR_SHAPE_BUY);
	ScriptModule_AddIntConstant(poModule, "SELL", CPythonApplication::CURSOR_SHAPE_SELL);

	ScriptModule_AddIntConstant(poModule, "CAMERA_ROTATE", CPythonApplication::CURSOR_SHAPE_CAMERA_ROTATE);
	ScriptModule_AddIntConstant(poModule, "HSIZE", CPythonApplication::CURSOR_SHAPE_HSIZE);
	ScriptModule_AddIntConstant(poModule, "VSIZE", CPythonApplication::CURSOR_SHAPE_VSIZE);
	ScriptModule_AddIntConstant(poModule, "HVSIZE", CPythonApplication::CURSOR_SHAPE_HVSIZE);

	ScriptModule_AddIntConstant(poModule, "CAMERA_TO_POSITIVE", CPythonApplication::CAMERA_TO_POSITIVE);
	ScriptModule_AddIntConstant(poModule, "CAMERA_TO_NEGATIVE", CPythonApplication::CAMERA_TO_NEGITIVE);
	ScriptModule_AddIntConstant(poModule, "CAMERA_STOP", CPythonApplication::CAMERA_STOP);

#ifdef ENABLE_COSTUME_SYSTEM
	ScriptModule_AddIntConstant(poModule, "ENABLE_COSTUME_SYSTEM", 1);
#else
	ScriptModule_AddIntConstant(poModule, "ENABLE_COSTUME_SYSTEM", 0);
#endif

#ifdef ENABLE_ENERGY_SYSTEM
	ScriptModule_AddIntConstant(poModule, "ENABLE_ENERGY_SYSTEM", 1);
#else
	ScriptModule_AddIntConstant(poModule, "ENABLE_ENERGY_SYSTEM", 0);
#endif

#ifdef ENABLE_DRAGON_SOUL_SYSTEM
	ScriptModule_AddIntConstant(poModule, "ENABLE_DRAGON_SOUL_SYSTEM", 1);
#else
	ScriptModule_AddIntConstant(poModule, "ENABLE_DRAGON_SOUL_SYSTEM", 0);
#endif

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	ScriptModule_AddIntConstant(poModule, "ENABLE_NEW_EQUIPMENT_SYSTEM", 1);
#else
	ScriptModule_AddIntConstant(poModule, "ENABLE_NEW_EQUIPMENT_SYSTEM", 0);
#endif

	ScriptModule_AddIntConstant(poModule, "USE_OPENID", 0);
	ScriptModule_AddIntConstant(poModule, "OPENID_TEST", 0);
}
