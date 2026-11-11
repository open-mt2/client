#include <Client/Application/Application.h>

#include <Client/UI/StdAfx.h>

#include <Client/UI/ClientStartup.h>
#include <Client/UI/LoadingScreen.h>
#include <Client/UI/Locale.h>
#include <Client/UI/PythonApplication.h>
#include <Client/UI/Test.h>
#include <Client/UI/Version.h>
#include <Client/UI/resource.h>
#include <Client/WebBrowser/CWebBrowser.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/CPostIt.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Platform/Base/Utils.h>
#include <Neoeter/Platform/Base/lzo.h>
#include <Neoeter/Render/MSWindow.h>
#include <Neoeter/Render/Util.h>

#include <SDL2/SDL.h>

#include <cstdio>
#include <cstring>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#ifndef OPENMT2_CLIENT_VERSION
#define OPENMT2_CLIENT_VERSION "unknown"
#endif

namespace
{
	const char* const kPythonLibraries[] =
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
		"UserDict.pyc",
		"__future__.pyc",
		"copy_reg.pyc",
		"linecache.pyc",
		"ntpath.pyc",
		"os.pyc",
		"site.pyc",
		"stat.pyc",
		"string.pyc",
		"traceback.pyc",
		"types.pyc",
#endif
		"\n",
	};

	bool CheckPythonLibraryFilenames()
	{
		for (int i = 0; *kPythonLibraries[i] != '\n'; ++i)
		{
			std::string filename = "lib/";
			filename += kPythonLibraries[i];

			for (char& character : filename)
			{
				if (character == '\\')
					character = '/';
			}

			if (_access(filename.c_str(), 0) != 0)
				return false;

			MoveFile(filename.c_str(), filename.c_str());
		}

		return true;
	}

	void ShowStartupError(const char* message)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenMT2", message, CMSWindow::GetSDLWindow());
	}

	bool IsRevisionOption(const char* argument)
	{
		return strcmp(argument, "--perforce-revision") == 0;
	}

	bool IsLocaleOption(const char* argument)
	{
		return strcmp(argument, "--locale") == 0;
	}

	bool IsTimeStampOption(const char* argument)
	{
		const char* option = "/timestamp";
		return strncmp(argument, option, strlen(option)) == 0;
	}

	void PrintTimeStamp()
	{
#ifndef _MSC_VER
#define _MSC_VER 1900
#endif
#ifdef _DEBUG
		if (__IS_TEST_SERVER_MODE__)
		{
			LogBoxf("OpenMT2 BINARY TEST DEBUG VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
		}
		else
		{
			LogBoxf("OpenMT2 BINARY DEBUG VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
		}
#else
		if (__IS_TEST_SERVER_MODE__)
		{
			LogBoxf("OpenMT2 BINARY TEST VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
		}
		else
		{
			LogBoxf("OpenMT2 BINARY DISTRIBUTE VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
		}
#endif
	}

	class CommandLine
	{
		public:
			explicit CommandLine(const std::string& commandLine)
				: m_commandLine(commandLine), m_argv(nullptr), m_count(0)
			{
				m_argv = CommandLineToArgv(m_commandLine.empty() ? const_cast<PCHAR>("") : &m_commandLine[0], &m_count);
			}

			~CommandLine()
			{
				SAFE_FREE_GLOBAL(m_argv);
			}

			CommandLine(const CommandLine&) = delete;
			CommandLine& operator=(const CommandLine&) = delete;

			int Count() const
			{
				return m_count;
			}

			const char* At(int index) const
			{
				return m_argv[index];
			}

		private:
			std::string m_commandLine;
			PCHAR* m_argv;
			int m_count;
	};
}

namespace Neoeter
{
	Application::Application()
		: m_hInstance(nullptr), m_pythonApplication()
	{
	}

	Application::~Application() = default;

	int Application::Run(int argc, char** argv)
	{
		std::string commandLine;
		for (int i = 1; i < argc; ++i)
		{
			if (i > 1)
				commandLine += ' ';

			commandLine += argv[i];
		}

		return Run(commandLine);
	}

	int Application::Run(const std::string& commandLine)
	{
		if (commandLine.find("--hackshield") != std::string::npos)
			return 0;

		Initialize();

		const CommandLineResult commandLineResult = ProcessCommandLine(commandLine);
		if (commandLineResult == CommandLineResult::ExitSuccess)
			return 0;

		if (commandLineResult == CommandLineResult::ExitFailure)
			return 1;

		if (!ValidateLaunch(commandLine))
			return 1;

		const int result = RunGame(commandLine);

		Shutdown();

		return result;
	}

	void Application::Initialize()
	{
		m_hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));

#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

		ApplicationStringTable_Initialize(m_hInstance);
		LocaleService_LoadConfig("locale.cfg");
		SetDefaultCodePage(LocaleService_GetCodePage());

#if defined(_DEBUG) || defined(OPENMT2_PLATFORM_LINUX)
		OpenConsoleWindow();
#endif
		OpenLogFile(true);
	}

	Application::CommandLineResult Application::ProcessCommandLine(const std::string& commandLine)
	{
		CommandLine arguments(commandLine);
		bool quit = false;

		for (int i = 0; i < arguments.Count(); ++i)
		{
			const char* argument = arguments.At(i);
			if (argument == nullptr)
				continue;

			if (IsRevisionOption(argument))
			{
				char moduleName[MAX_PATH];
				char versionPath[MAX_PATH + 16];
				GetModuleFileName(NULL, moduleName, sizeof(moduleName));
				snprintf(versionPath, sizeof(versionPath), "%s.version", moduleName);

				FILE* file = fopen(versionPath, "wt");
				if (file)
				{
					fprintf(file, "r%d\n", OPENMT2_GET_VERSION());
					fclose(file);
				}
				quit = true;
			}
			else if (IsLocaleOption(argument))
			{
				FILE* file = fopen("locale.txt", "wt");
				if (file)
				{
					fprintf(file, "service[%s] code_page[%d]", LocaleService_GetName(), LocaleService_GetCodePage());
					fclose(file);
				}
				quit = true;
			}
			else if (IsTimeStampOption(argument))
			{
				PrintTimeStamp();
				quit = true;
			}
			else if (strcmp(argument, "--force-set-locale") == 0)
			{
				if (arguments.Count() <= i + 2)
				{
					ShowStartupError("Invalid arguments");
					return CommandLineResult::ExitFailure;
				}

				const char* localeName = arguments.At(++i);
				const char* localePath = arguments.At(++i);
				LocaleService_ForceSetLocale(localeName, localePath);
			}
		}

		return quit ? CommandLineResult::ExitSuccess : CommandLineResult::Launch;
	}

	bool Application::ValidateLaunch(const std::string& commandLine) const
	{
#if defined(NEEDED_COMMAND_ARGUMENT)
		if (strstr(commandLine.c_str(), NEEDED_COMMAND_ARGUMENT) == 0)
		{
			ShowStartupError(ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"));
			return false;
		}
#endif

#if defined(NEEDED_COMMAND_CLIPBOARD)
		CHAR securityKey[256];
		CPostIt clipboard("VOLUME1");

		if (clipboard.Get("SEC_KEY", securityKey, sizeof(securityKey)) == FALSE)
		{
			ShowStartupError(ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"));
			return false;
		}

		if (strstr(securityKey, NEEDED_COMMAND_CLIPBOARD) == 0)
		{
			ShowStartupError(ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"));
			return false;
		}

		clipboard.Empty();
#endif

		return true;
	}

	int Application::RunGame(const std::string& commandLine)
	{
		WebBrowser_Startup(m_hInstance);

		static CLZO s_lzo;
		static CEterPackManager s_eterPackManager;

		m_pythonApplication.reset(new CPythonApplication);
		m_pythonApplication->Initialize(m_hInstance);

		if (!m_pythonApplication->CreateStartupWindow())
		{
			ShowStartupError("Could not create the game window.");
			return 1;
		}

		LoadingScreen_SetVersion((std::string("v") + OPENMT2_CLIENT_VERSION).c_str());
		LoadingScreen_FadeIn(0.8f);

		if (!CheckPythonLibraryFilenames())
		{
			LoadingScreen_ShowError(
				"The CPython 2.7 standard library is missing.\n\n"
				"Expected the Python files in the 'lib' directory next to the "
				"client.");
			LogBoxf("FATAL ERROR!! Python Library file not exist!");
			m_pythonApplication->CMSWindow::Destroy();
			return 1;
		}

		return RunClientGame(*m_pythonApplication, m_hInstance, commandLine.c_str()) ? 0 : 1;
	}

	void Application::Shutdown()
	{
		WebBrowser_Cleanup();
		::CoUninitialize();
		m_pythonApplication.reset();
	}
}
