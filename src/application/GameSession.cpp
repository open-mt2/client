#include <Client/Application/GameSession.h>

#include <Client/UI/StdAfx.h>

#include <Client/Application/CLI.h>
#include <Client/Application/Context.h>
#include <Client/Application/Packs.h>
#include <Client/UI/LoadingScreen.h>
#include <Client/UI/Locale.h>
#include <Client/UI/PythonApplication.h>
#include <Client/UI/PythonBackground.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Render/Util.h>
#include <Platform/Platform.h>

#include <cstdint>
#include <cstdlib>
#include <ctime>

namespace Neoeter
{
	GameSession::GameSession(Context& context, CPythonApplication& application)
		: m_context(context), m_application(application), m_platform(), m_python()
	{
	}

	GameSession::~GameSession() = default;

	GameSession::Result GameSession::Run(const CommandLine& arguments)
	{
		if (!Prepare())
			return Result::Aborted;

		if (!m_platform.Initialize())
		{
			LoadingScreen_ShowError("Could not initialize the client environment.");
			return Result::Aborted;
		}

		if (!Packs::Mount("pack"))
		{
			LoadingScreen_ShowError("Pack initialization failed. Check log.txt.");
			return Result::Aborted;
		}

		LoadingScreen_Show();

		if (LocaleService_LoadGlobal(m_context.Instance()))
			SetDefaultCodePage(LocaleService_GetCodePage());

		CPythonBackground::Instance().Initialize();

		LoadingScreen_Show();

		if (arguments.PauseBeforeScript())
			system("pause");

		const PythonRuntime::Result pythonResult = m_python.Start(arguments.PythonCommandLine());
		if (pythonResult == PythonRuntime::Result::RuntimeFailure)
			LoadingScreen_ShowError("Could not initialize the Python runtime.");

		Cleanup();

		return pythonResult == PythonRuntime::Result::Success ? Result::Success : Result::Failure;
	}

	bool GameSession::Prepare()
	{
		const DWORD randomSeed = time(NULL) + DWORD((uintptr_t)GetCurrentProcess());
		srandom(randomSeed);
		srand(random());

		SetLogLevel(1);

#ifdef LOCALE_SERVICE_YMIR
		extern bool g_isScreenShotKey;
		g_isScreenShotKey = true;
#endif
#ifdef LOCALE_SERVICE_VIETNAM_MILD
		extern BOOL USE_VIETNAM_CONVERT_WEAPON_VNUM;
		USE_VIETNAM_CONVERT_WEAPON_VNUM = true;
#endif

		if (_access("perf_game_update.txt", 0) == 0)
			DeleteFile("perf_game_update.txt");

		if (_access("newpatch.exe", 0) == 0)
		{
			system("patchupdater.exe");
			return false;
		}

		return true;
	}

	void GameSession::Cleanup()
	{
		m_application.Clear();
		m_platform.Shutdown();
		m_python.Stop();
	}
}
