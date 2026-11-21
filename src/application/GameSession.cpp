#include <Client/Application/GameSession.h>

#include <Client/UI/StdAfx.h>

#include <Client/Application/CLI.h>
#include <Client/Application/Context.h>
#include <Client/Application/Packs.h>
#include <Client/Python/Host/PythonHostAdapter.h>
#include <Client/UI/Widget/LoadingScreen.h>
#include <Client/Locale/Locale.h>
#include <Client/Python/Application/PythonApplication.h>
#include <Client/Python/Background/PythonBackground.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Render/Util.h>
#include <Neoeter/Platform/Platform.h>

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>

namespace Neoeter
{
	GameSession::GameSession(Context& context, CPythonApplication& application)
		: m_context(context), m_application(application), m_platform(), m_script()
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

		CPythonBackground::Instance().Initialize();

		LoadingScreen_Show();

		if (arguments.PauseBeforeScript())
			system("pause");

		CEterPackManager::TPackDataPtr manifestData;
		if (!CEterPackManager::Instance().Get("manifest", manifestData) || !manifestData || manifestData->empty())
		{
			LoadingScreen_ShowError("The pack manifest is missing. Check that the pack folder contains 'manifest'.");
			Cleanup();
			return Result::Aborted;
		}

		const std::string manifestText(manifestData->begin(), manifestData->end());

		m_script.RegisterHost("python", &CreatePythonHost);

		ScriptContext scriptContext;
		scriptContext.commandLine = arguments.PythonCommandLine();
		scriptContext.debug = !Context::IsDistribute();

		const ScriptHost::Result scriptResult = m_script.Start(manifestText, scriptContext);
		if (scriptResult != ScriptHost::Result::Success)
			TraceError("%s", m_script.LastError().c_str());

		if (scriptResult == ScriptHost::Result::LoadError)
			LoadingScreen_ShowError("The pack script failed. Check log.txt.");
		else if (scriptResult != ScriptHost::Result::Success)
			LoadingScreen_ShowError(m_script.LastError().c_str());

		Cleanup();

		return scriptResult == ScriptHost::Result::Success ? Result::Success : Result::Failure;
	}

	bool GameSession::Prepare()
	{
		const uint32_t randomSeed = time(NULL) + uint32_t((uintptr_t)GetCurrentProcess());
		srandom(randomSeed);
		srand(random());

		SetLogLevel(1);

#ifdef LOCALE_SERVICE_YMIR
		extern bool g_isScreenShotKey;
		g_isScreenShotKey = true;
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
		m_script.Stop();
	}
}
