#include <Client/Application/Application.h>

#include <Client/Application/CLI.h>
#include <Client/Application/Context.h>
#include <Client/Application/GameSession.h>

#include <Client/UI/StdAfx.h>

#include <Client/UI/LoadingScreen.h>
#include <Client/UI/Locale.h>
#include <Client/UI/PythonApplication.h>
#include <Client/Utils/PythonUtils.h>
#include <Client/WebBrowser/CWebBrowser.h>
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

#ifndef OPENMT2_PLATFORM_LINUX
extern "C"
{
#if defined(_MSC_VER)
	extern int32_t _fltused;
	volatile int32_t _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;
#endif
	__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int32_t AmdPowerXpressRequestHighPerformance = 1;
};
#endif

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

	int Application::Run(const std::string& commandLine)
	{
		if (commandLine.find("--hackshield") != std::string::npos)
			return 0;

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

		ApplicationStringTable_Initialize(m_context.Instance());
		LocaleService_LoadConfig("locale.cfg");
		SetDefaultCodePage(LocaleService_GetCodePage());

		if (Context::IsDebug() || Context::IsLinux())
			OpenConsoleWindow();

		OpenLogFile(true);
	}

	int Application::RunGame(const CommandLine& arguments)
	{
		WebBrowser_Startup(m_context.Instance());

		static CLZO s_lzo;
		static CEterPackManager s_eterPackManager;

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
		WebBrowser_Cleanup();
		::CoUninitialize();
		m_pythonApplication.reset();
	}
}
