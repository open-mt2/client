#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonApplication.h>
#include <Client/UI/PythonBackground.h>
#include <Client/UI/PythonExceptionSender.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Render/Util.h>

// Force link ImageLoaderCompat.cpp from libEterBase.a
extern bool Platform_GetImageInfo(const void* data, size_t size, uint32_t* outW, uint32_t* outH, uint32_t* outFormat);
extern bool Platform_DecodeImage(const void* data, size_t size, uint32_t* outW, uint32_t* outH, std::vector<uint8_t>& outPixels);
void* g_pForceLinkImageLoader[] = {
    (void*)&Platform_GetImageInfo,
    (void*)&Platform_DecodeImage
};

#ifndef OPENMT2_PLATFORM_LINUX
// T4.1: the DirectX SDK, Miles (mss32) and the SpeedTree import library are
// gone. The Windows libraries the client actually needs (opengl32, winmm,
// imagehlp, ws2_32) are linked by CMake, not by #pragma.
extern "C" {
#if defined(_MSC_VER)
	// _fltused is provided by the MSVC CRT; MinGW has no such symbol (T1.9).
	extern int32_t _fltused;
	volatile int32_t _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;
#endif
	__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int32_t AmdPowerXpressRequestHighPerformance = 1;
};
#endif
#include <stdlib.h>

#include <Client/UI/ClientStartup.h>
#include <Client/UI/LoadingScreen.h>
bool __IS_TEST_SERVER_MODE__=false;

extern bool SetDefaultCodePage(DWORD codePage);


int Setup();

bool PackInitialize(const std::string& packFolder)
{
	if (_access(packFolder.c_str(), 0) != 0)
		return true;

	// Initialize variables
	bool bPackFirst = true;
	std::string stFolder = packFolder + "/";
	std::string stFileName;

#ifdef _DISTRIBUTE
	Tracef("Info: Pack search mode set to archive-first.\n");

	// Set Index file name to production value
	stFileName = stFolder + "Index";
#else
	if (_access((stFolder + "Index.dev").c_str(), 0) == 0)
	{
		bPackFirst = false;
		Tracef("Info: Pack search mode set to file-first.\n");

		// Set Index file name to development value
		stFileName = stFolder + "Index.dev";
	}
	else
	{
		bPackFirst = true;
		Tracef("Info: Pack search mode set to archive-first (fallback).\n");

		// Set Index file name to production value
		stFileName = stFolder + "Index";
	}
#endif

	// Set up file reader
	CMappedFile file;
	LPCVOID pvData;

	if (!file.Create(stFileName.c_str(), &pvData, 0, 0))
	{
		LogBoxf("FATAL ERROR! File not exist: %s", stFileName.c_str());
		TraceError("FATAL ERROR! File not exist: %s", stFileName.c_str());
		return true;
	}

	CMemoryTextFileLoader TextLoader;
	TextLoader.Bind(file.Size(), pvData);

	// Set up archive manager
	CTextFileLoader::SetCacheMode();
	CEterPackManager::Instance().SetSearchMode(bPackFirst);

	CSoundData::SetPackMode(); // Miles 파일 콜백을 셋팅

	// Read lines and parse tokens
	std::string strPackName, strTexCachePackName;
	CTokenVector tokens = CTokenVector();

	for (DWORD i = 0; i < TextLoader.GetLineCount() - 1; i++)
	{
		// Split line into tokens
		TextLoader.SplitLineByTab(i, &tokens);

		// Check if the read number of tokens is valid
		if (tokens.size() != 2)
		{
			TraceError("Invalid number of tokens on line %d: expected %d, got %d!", i, 2, tokens.size());
			return false;
		}

		// Assign tokens into variables
		const std::string& c_rstName = tokens[0];
		const std::string& c_rstContainer = tokens[1];

		// Load pack
		strPackName = stFolder + c_rstName;
		CEterPackManager::Instance().RegisterPack(strPackName, c_rstContainer);

		// Try to load texture cache pack if it exists
		strTexCachePackName = strPackName + "_texcache";
		if (CEterPackManager::Instance().PackExists(strTexCachePackName, c_rstContainer)) {
			CEterPackManager::Instance().RegisterPack(strTexCachePackName, c_rstContainer);
		}
	}

	return true;
}

bool RunMainScript(CPythonLauncher& pyLauncher, const char* lpCmdLine)
{
	initpack();
	initdbg();
	initime();
	initgrp();
	initgrpImage();
	initgrpText();
	initwndMgr();
	/////////////////////////////////////////////
	initapp();
	initsystem();
	initchr();
	initchrmgr();
	initPlayer();
	initItem();
	initNonPlayer();
	initTrade();
	initChat();
	initTextTail();
	initnet();
	initMiniMap();
	initProfiler();
	initEvent();
	initeffect();
	initfly();
	initsnd();
	initeventmgr();
	initshop();
	initskill();
	initquest();
	initBackground();
	initMessenger();
	initsafebox();
	initguild();
	initServerStateChecker();

	// RegisterDebugFlag
	{
		std::string stRegisterDebugFlag;

#ifdef _DISTRIBUTE 
		stRegisterDebugFlag ="__DEBUG__ = 0";
#else
		stRegisterDebugFlag ="__DEBUG__ = 1"; 
#endif

		if (!pyLauncher.RunLine(stRegisterDebugFlag.c_str()))
		{
			TraceError("RegisterDebugFlag Error");
			return false;
		}
	}

	// RegisterCommandLine
	{
		std::string stRegisterCmdLine;

		const char * loginMark = "-cs";
		const char * loginMark_NonEncode = "-ncs";
		const char * seperator = " ";

		std::string stCmdLine;
		const int CmdSize = 3;
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine,seperator,&stVec);
		if (CmdSize == stVec.size() && stVec[0]==loginMark)
		{
			char buf[MAX_PATH];	//TODO 아래 함수 string 형태로 수정
			base64_decode(stVec[2].c_str(),buf);
			stVec[2] = buf;
			string_join(seperator,stVec,&stCmdLine);
		}
		else if (CmdSize <= stVec.size() && stVec[0]==loginMark_NonEncode)
		{
			stVec[0] = loginMark;
			string_join(" ",stVec,&stCmdLine);
		}
		else
			stCmdLine = lpCmdLine;

		stRegisterCmdLine ="__COMMAND_LINE__ = ";
		stRegisterCmdLine+='"';
		stRegisterCmdLine+=stCmdLine;
		stRegisterCmdLine+='"';

		const CHAR* c_szRegisterCmdLine=stRegisterCmdLine.c_str();
		if (!pyLauncher.RunLine(c_szRegisterCmdLine))
		{
			TraceError("RegisterCommandLine Error");
			return false;
		}
	}
	{
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine," " ,&stVec);

		if (stVec.size() != 0 && "--pause-before-create-window" == stVec[0])
		{
			system("pause");
		}
		if (!pyLauncher.RunFile("system.py"))
		{
			TraceError("RunMain Error");
			return false;
		}
	}

	return true;
}

bool RunClientGame(CPythonApplication &app, HINSTANCE hInstance,
                   const char *lpCmdLine) {
#ifdef LOCALE_SERVICE_YMIR
  extern bool g_isScreenShotKey;
  g_isScreenShotKey = true;
#endif

  DWORD dwRandSeed = time(NULL) + DWORD((uintptr_t)GetCurrentProcess());
  srandom(dwRandSeed);
  srand(random());

  SetLogLevel(1);

#ifdef LOCALE_SERVICE_VIETNAM_MILD
  extern BOOL USE_VIETNAM_CONVERT_WEAPON_VNUM;
  USE_VIETNAM_CONVERT_WEAPON_VNUM = true;
#endif

  if (_access("perf_game_update.txt", 0) == 0) {
    DeleteFile("perf_game_update.txt");
  }

  if (_access("newpatch.exe", 0) == 0) {
    system("patchupdater.exe");
    app.CMSWindow::Destroy();
    return false;
  }


  Tracef("[UserInterface] Setting up environment...\n");
  if (!Setup()) {
    LoadingScreen_ShowError("Could not initialize the client environment.");
    app.CMSWindow::Destroy();
    return false;
  }


  Tracef("[UserInterface] Initializing pack system from 'pack'...\n");

  if (!PackInitialize("pack")) {
    LoadingScreen_ShowError("Pack initialization failed. Check log.txt.");
    app.CMSWindow::Destroy();
    return false;
  }
  Tracef("[UserInterface] Pack system initialized successfully.\n");

  LoadingScreen_Show();


  Tracef("[UserInterface] Loading global locale configuration...\n");
  if (LocaleService_LoadGlobal(hInstance))
    SetDefaultCodePage(LocaleService_GetCodePage());

  Tracef("[UserInterface] Reloading map atlas from packs...\n");
  CPythonBackground::Instance().Initialize();

  LoadingScreen_Show();

  bool ret = false;
  {
    Tracef("[UserInterface] Creating CPythonLauncher...\n");
    CPythonLauncher pyLauncher;
    CPythonExceptionSender pyExceptionSender;
    SetExceptionSender(&pyExceptionSender);

    if (pyLauncher.Create()) {
      Tracef("[UserInterface] Python runtime created. Launching main script "
             "(system.py)...\n");
      ret = RunMainScript(pyLauncher, lpCmdLine);
      Tracef("[UserInterface] RunMainScript exited with code %d.\n", ret);
    } else {
      Tracef("[UserInterface] Failed to initialize Python Launcher!\n");
      LoadingScreen_ShowError("Could not initialize the Python runtime.");
    }

    Tracef("[UserInterface] Cleaning up application...\n");
    app.Clear();

    timeEndPeriod(1);
    pyLauncher.Clear();
  }

  app.Destroy();
  Tracef("[UserInterface] Application cleanly terminated.\n");

  return ret;
}

HANDLE CreateOpenMT2GameMutex()
{
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength				= sizeof(sa);
	sa.lpSecurityDescriptor	= NULL;
	sa.bInheritHandle		= FALSE;

	return CreateMutex(&sa, FALSE, "OpenMT2GameMutex");
}

void DestroyOpenMT2GameMutex(HANDLE hMutex)
{
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		hMutex = NULL;
	}
}


static void GrannyError(granny_log_message_type Type,
						granny_log_message_origin Origin,
						char const *Error,
						void *UserData)
{
    TraceError("GRANNY: %s", Error);
}

int Setup()
{
	/* 
	 *	타이머 정밀도를 올린다.
	 */
	TIMECAPS tc; 
	UINT wTimerRes; 

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		return 0;

	wTimerRes = MINMAX(tc.wPeriodMin, 1, tc.wPeriodMax); 
	timeBeginPeriod(wTimerRes); 

	/*
	 *	그래니 에러 핸들링
	 */

	granny_log_callback Callback;
    Callback.Function = GrannyError;
    Callback.UserData = 0;
    GrannySetLogCallback(&Callback);
	return 1;
}
