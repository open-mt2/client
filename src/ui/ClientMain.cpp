#include <Client/UI/StdAfx.h>

#include <Client/UI/resource.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include <Client/UI/PythonApplication.h>
#include <Client/UI/Test.h>
#include <Client/UI/Version.h>
#include <Client/WebBrowser/CWebBrowser.h>
#include <Neoeter/Platform/Base/CPostIt.h>
#include <Neoeter/Platform/Base/Utils.h>
#include <Neoeter/Render/MSWindow.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/lzo.h>

#include <SDL2/SDL.h>
#include <cstring>

#include <Client/UI/ClientMain.h>
#include <Client/UI/ClientStartup.h>
#include <Client/UI/LoadingScreen.h>

#ifndef OPENMT2_CLIENT_VERSION
#define OPENMT2_CLIENT_VERSION "unknown"
#endif

static const char *sc_apszPythonLibraryFilenames[] =

    {

#if PY_MINOR_VERSION == 7
        "abc.pyc",
        "bisect.pyc",
        "codecs.pyc",
        "collections.pyc",
        "copy.pyc",
        "copy_reg.pyc",
        "fnmatch.pyc",
        "functools.pyc",
        "genericpath.pyc",
        "heapq.pyc",
        "hashlib.pyc",
        "keyword.pyc",
        "linecache.pyc",
        "locale.pyc",
        "ntpath.pyc",
        "os.pyc",
        "random.pyc",
        "re.pyc",
        "shutil.pyc",
        "site.pyc",
        "sre_compile.pyc",
        "sre_constants.pyc",
        "sre_parse.pyc",
        "stat.pyc",
        "string.pyc",
        "sysconfig.pyc",
        "traceback.pyc",
        "types.pyc",
        "UserDict.pyc",
        "warnings.pyc",
        "weakref.pyc",
        "_abcoll.pyc",
        "_weakrefset.pyc",
        "__future__.pyc",
        "encodings/aliases.pyc",
        "encodings/ascii.pyc",
        "encodings/cp1250.pyc",
        "encodings/cp1251.pyc",
        "encodings/cp1252.pyc",
        "encodings/cp949.pyc",
        "encodings/utf_8.pyc",
        "encodings/cp850.pyc",
        "encodings/__init__.pyc",
#else
        "UserDict.pyc", "__future__.pyc", "copy_reg.pyc", "linecache.pyc",
        "ntpath.pyc",   "os.pyc",         "site.pyc",     "stat.pyc",
        "string.pyc",   "traceback.pyc",  "types.pyc",
#endif

        "\n",

};

char gs_szErrorString[512] = "";

void ApplicationSetErrorString(const char *szErrorString) {
  strcpy(gs_szErrorString, szErrorString);
}

static bool CheckPythonLibraryFilenames() {
  for (int i = 0; *sc_apszPythonLibraryFilenames[i] != '\n'; ++i) {
    std::string stFilename = "lib/";
    stFilename += sc_apszPythonLibraryFilenames[i];
    for (char &c : stFilename) {
      if (c == '\\')
        c = '/';
    }

    if (_access(stFilename.c_str(), 0) != 0)
      return false;

    MoveFile(stFilename.c_str(), stFilename.c_str());
  }

  return true;
}

static void ShowStartupError(const char *message) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenMT2", message,
                           CMSWindow::GetSDLWindow());
}

static void __ErrorPythonLibraryIsNotExist() {
  LogBoxf("FATAL ERROR!! Python Library file not exist!");
}

static bool __IsTimeStampOption(LPSTR lpCmdLine) {
  const char *TIMESTAMP = "/timestamp";
  return (strncmp(lpCmdLine, TIMESTAMP, strlen(TIMESTAMP)) == 0);
}

static void __PrintTimeStamp() {
#ifndef _MSC_VER
#define _MSC_VER 1900
#endif
#ifdef _DEBUG
  if (__IS_TEST_SERVER_MODE__)
    LogBoxf("OpenMT2 BINARY TEST DEBUG VERSION %s  ( MS C++ %d Compiled )",
            __TIMESTAMP__, _MSC_VER);
  else
    LogBoxf("OpenMT2 BINARY DEBUG VERSION %s ( MS C++ %d Compiled )",
            __TIMESTAMP__, _MSC_VER);
#else
  if (__IS_TEST_SERVER_MODE__)
    LogBoxf("OpenMT2 BINARY TEST VERSION %s  ( MS C++ %d Compiled )",
            __TIMESTAMP__, _MSC_VER);
  else
    LogBoxf("OpenMT2 BINARY DISTRIBUTE VERSION %s ( MS C++ %d Compiled )",
            __TIMESTAMP__, _MSC_VER);
#endif
}

static bool __IsLocaleOption(LPSTR lpCmdLine) {
  return (strcmp(lpCmdLine, "--locale") == 0);
}

static bool __IsLocaleVersion(LPSTR lpCmdLine) {
  return (strcmp(lpCmdLine, "--perforce-revision") == 0);
}

int ClientMain_Run(const std::string &commandLine) {
  HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
  if (commandLine.find("--hackshield") != std::string::npos)
    return 0;

#ifdef _DEBUG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF |
                 _CRTDBG_LEAK_CHECK_DF);
#endif

  ApplicationStringTable_Initialize(hInstance);
  LocaleService_LoadConfig("locale.cfg");
  SetDefaultCodePage(LocaleService_GetCodePage());

#if defined(_DEBUG) || defined(OPENMT2_PLATFORM_LINUX)
  OpenConsoleWindow();
#endif
  OpenLogFile(true);

  bool bQuit = false;
  int nArgc = 0;
  std::string mutableCommandLine = commandLine;
  PCHAR *szArgv =
      CommandLineToArgv(mutableCommandLine.empty() ? const_cast<PCHAR>("")
                                                   : &mutableCommandLine[0],
                        &nArgc);

  for (int i = 0; i < nArgc; i++) {
    if (szArgv[i] == 0)
      continue;

    if (__IsLocaleVersion(szArgv[i])) {
      char szModuleName[MAX_PATH];
      char szVersionPath[MAX_PATH + 16];
      GetModuleFileName(NULL, szModuleName, sizeof(szModuleName));
      snprintf(szVersionPath, sizeof(szVersionPath), "%s.version",
               szModuleName);
      FILE *fp = fopen(szVersionPath, "wt");
      if (fp) {
        fprintf(fp, "r%d\n", OPENMT2_GET_VERSION());
        fclose(fp);
      }
      bQuit = true;
    } else if (__IsLocaleOption(szArgv[i])) {
      FILE *fp = fopen("locale.txt", "wt");
      fprintf(fp, "service[%s] code_page[%d]", LocaleService_GetName(),
              LocaleService_GetCodePage());
      fclose(fp);
      bQuit = true;
    } else if (__IsTimeStampOption(szArgv[i])) {
      __PrintTimeStamp();
      bQuit = true;
    } else if ((strcmp(szArgv[i], "--force-set-locale") == 0)) {
      if (nArgc <= i + 2) {
        ShowStartupError("Invalid arguments");
        SAFE_FREE_GLOBAL(szArgv);
        return 1;
      }

      const char *localeName = szArgv[++i];
      const char *localePath = szArgv[++i];
      LocaleService_ForceSetLocale(localeName, localePath);
    }
  }

  if (bQuit) {
    SAFE_FREE_GLOBAL(szArgv);
    return 0;
  }

#if defined(NEEDED_COMMAND_ARGUMENT)
  if (strstr(mutableCommandLine.c_str(), NEEDED_COMMAND_ARGUMENT) == 0) {
    ShowStartupError(ApplicationStringTable_GetStringz(
        IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"));
    SAFE_FREE_GLOBAL(szArgv);
    return 1;
  }
#endif

#if defined(NEEDED_COMMAND_CLIPBOARD)
  {
    CHAR szSecKey[256];
    CPostIt cPostIt("VOLUME1");

    if (cPostIt.Get("SEC_KEY", szSecKey, sizeof(szSecKey)) == FALSE) {
      ShowStartupError(ApplicationStringTable_GetStringz(
          IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"));
      SAFE_FREE_GLOBAL(szArgv);
      return 1;
    }
    if (strstr(szSecKey, NEEDED_COMMAND_CLIPBOARD) == 0) {
      ShowStartupError(ApplicationStringTable_GetStringz(
          IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"));
      SAFE_FREE_GLOBAL(szArgv);
      return 1;
    }
    cPostIt.Empty();
  }
#endif

  SAFE_FREE_GLOBAL(szArgv);

  WebBrowser_Startup(hInstance);

  static CLZO s_lzo;
  static CEterPackManager s_eterPackManager;

  CPythonApplication *app = new CPythonApplication;
  app->Initialize(hInstance);

  int result = 1;

  if (!app->CreateStartupWindow()) {
    ShowStartupError("Could not create the game window.");
  } else {
    LoadingScreen_SetVersion((std::string("v") + OPENMT2_CLIENT_VERSION).c_str());
    LoadingScreen_FadeIn(0.8f);

    if (!CheckPythonLibraryFilenames()) {
      LoadingScreen_ShowError(
          "The CPython 2.7 standard library is missing.\n\n"
          "Expected the Python files in the 'lib' directory next to the "
          "client.");
      __ErrorPythonLibraryIsNotExist();
      app->CMSWindow::Destroy();
    } else {
      result = RunClientGame(*app, hInstance, commandLine.c_str()) ? 0 : 1;
    }
  }

  WebBrowser_Cleanup();
  ::CoUninitialize();

  if (gs_szErrorString[0])
    ShowStartupError(gs_szErrorString);

  delete app;
  return result;
}
