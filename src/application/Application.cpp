#include <Client/Application/Application.h>

#include <Client/Application/CLI.h>
#include <Client/Application/Context.h>
#include <Client/Application/GameSession.h>
#include <Client/Application/PackFileSource.h>

#include <Client/GameData/FileSource.h>

#include <Client/Game/MapActorFilter.h>
#include <Client/Map/Map.h>

#include <Client/UI/StdAfx.h>

#include <Client/UI/Widget/LoadingScreen.h>
#include <Client/Locale/Locale.h>
#include <Client/Python/Application/PythonApplication.h>
#include <Client/Python/Utils/PythonUtils.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Platform/Base/lzo.h>
#include <Neoeter/Render/MSWindow.h>
#include <Neoeter/Render/Util.h>

#include <cstddef>
#include <cstdint>
#include <vector>

extern bool Platform_GetImageInfo(const void* data, size_t size, uint32_t* outW, uint32_t* outH, uint32_t* outFormat);
extern bool Platform_DecodeImage(const void* data, size_t size, uint32_t* outW, uint32_t* outH, std::vector<uint8_t>& outPixels);
void* g_pForceLinkImageLoader[] = {
	(void*)&Platform_GetImageInfo,
	(void*)&Platform_DecodeImage};

namespace Neoeter
{
	Application::Application() : m_context(), m_pythonApplication()
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

	/**
	 * OpenMT2 Client Application Entry point
	 * Initialize components
	 * Parses CLI arguments
	 */
	int Application::Run(const std::string& commandLine)
	{
		Initialize();

		CommandLine arguments(commandLine);
		const CommandLine::Result commandLineResult = arguments.Process();
		if (commandLineResult == CommandLine::Result::ExitSuccess)
			return 0;

		if (commandLineResult == CommandLine::Result::InvalidArguments)
		{
			TraceError("Invalid arguments");
			return 1;
		}

		const int result = RunGame(arguments);

		Shutdown();

		if (Context::ErrorString()[0] != '\0')
			LoadingScreen_ShowError(Context::ErrorString());

		return result;
	}

	void Application::Initialize()
	{
		Context::EnableDebugHeapChecks();

		SetDefaultCodePage(LocaleService_GetCodePage());

		if (Context::IsDebug() || Context::IsLinux())
			OpenConsoleWindow();

		OpenLogFile(true);
	}

	int Application::RunGame(const CommandLine& arguments)
	{
		static CLZO s_lzo;
		static CEterPackManager s_eterPackManager;
		static CPackFileSource s_packFileSource;

		SetGameDataFileSource(&s_packFileSource);
		Map::SetActorBlockerPredicate(&IsMapBuildingActor);

		m_pythonApplication.reset(new CPythonApplication);
		m_pythonApplication->Initialize(m_context.Instance());

		if (!m_pythonApplication->CreateStartupWindow())
		{
			LoadingScreen_ShowError("Could not create the game window.");
			return 1;
		}

		LoadingScreen_SetVersion((std::string("v") + Context::ClientVersion()).c_str());
		LoadingScreen_FadeIn(0.8f);

		if (!Utils::Python::CheckPythonLibraryFilenames())
		{
			LoadingScreen_ShowError("The CPython 2.7 standard library is missing.\n\nExpected the Python files in the 'lib' directory next to the client.");
			LogBoxf("Python Library file not exist.");
			m_pythonApplication->CMSWindow::Destroy();
			return 1;
		}

		GameSession session(m_context, *m_pythonApplication);
		const GameSession::Result result = session.Run(arguments);

		if (result == GameSession::Result::Aborted)
		{
			m_pythonApplication->CMSWindow::Destroy();
			return 1;
		}

		m_pythonApplication->Destroy();
		return result == GameSession::Result::Success ? 0 : 1;
	}

	void Application::Shutdown()
	{
		SetGameDataFileSource(NULL);
		m_pythonApplication.reset();
	}
}
