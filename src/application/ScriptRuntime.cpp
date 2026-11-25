#include <Client/Application/ScriptRuntime.h>

#include <Client/Application/Context.h>

#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonExceptionSender.h>

#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Platform/Base/Utils.h>
#include <Neoeter/Script/ScriptBackend.h>

#if OPENMT2_SCRIPT_DOTNET
#include <Neoeter/DotNet/DotNetRuntime.h>
#endif

#include <filesystem>
#include <memory>
#include <vector>

namespace Neoeter
{
	namespace
	{
#if OPENMT2_SCRIPT_DOTNET
		std::unique_ptr<CPythonExceptionSender> g_dotnetExceptionSender;
#endif
	}

	void RegisterNativeModules(const char* runtimeName, const std::string& commandLine)
	{
		initpack();
		initdbg();
		initime();
		initgrp();
		initgrpImage();
		initgrpText();
		initwndMgr();

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

		static const ScriptMethodDef s_noMethods[] = {{nullptr, nullptr, 0, nullptr}};
		ScriptObject* host = ScriptModule_Create("scriptHost", s_noMethods);
		ScriptModule_AddStringConstant(host, "NAME", runtimeName);
		ScriptModule_AddIntConstant(host, "DEBUG", Context::IsDistribute() ? 0 : 1);
		ScriptModule_AddStringConstant(host, "COMMAND_LINE", commandLine.c_str());
	}

	ScriptRuntime::ScriptRuntime() : m_python(), m_active(Kind::Auto), m_dotnetStarted(false)
	{
	}

	ScriptRuntime::~ScriptRuntime()
	{
		Stop();
	}

	bool ScriptRuntime::ParseKind(const std::string& text, Kind* kind)
	{
		if (text == "auto")
			*kind = Kind::Auto;
		else if (text == "python")
			*kind = Kind::Python;
		else if (text == "dotnet")
			*kind = Kind::DotNet;
		else
			return false;
		return true;
	}

	const char* ScriptRuntime::KindName(Kind kind)
	{
		switch (kind)
		{
			case Kind::Python:
				return "python";
			case Kind::DotNet:
				return "dotnet";
			default:
				return "auto";
		}
	}

	bool ScriptRuntime::IsDotNetAvailable()
	{
#if OPENMT2_SCRIPT_DOTNET
		return true;
#else
		return false;
#endif
	}

	ScriptRuntime::Kind ScriptRuntime::Resolve(Kind requested) const
	{
		if (requested != Kind::Auto)
			return requested;
		if (IsDotNetAvailable() && CEterPackManager::Instance().isExist(DotNetEntryAssembly))
			return Kind::DotNet;
		return Kind::Python;
	}

	ScriptRuntime::Result ScriptRuntime::Start(Kind kind, const std::string& commandLine)
	{
		m_active = Resolve(kind);
		Tracef("Script runtime: %s (requested %s)\n", KindName(m_active), KindName(kind));

		if (m_active == Kind::DotNet)
			return StartDotNet(commandLine);
		return m_python.Start(commandLine);
	}

	ScriptRuntime::Result ScriptRuntime::StartDotNet(const std::string& commandLine)
	{
#if OPENMT2_SCRIPT_DOTNET
		g_dotnetExceptionSender.reset(new CPythonExceptionSender);
		SetExceptionSender(g_dotnetExceptionSender.get());

		// A runtime bundled next to the client wins over an installed one.
		DotNetRuntimeOptions options;
		std::error_code ec;
		const std::filesystem::path bundled = std::filesystem::path(CDotNetRuntime::DefaultHostDirectory()) / "runtime";
		if (std::filesystem::is_directory(bundled / "host" / "fxr", ec))
			options.dotnetRoot = bundled.string();

		CDotNetRuntime& runtime = CDotNetRuntime::Instance();
		std::string error;
		if (!runtime.Initialize(options, &error))
		{
			TraceError("Failed to initialize the .NET runtime: %s", error.c_str());
			return Result::RuntimeFailure;
		}
		m_dotnetStarted = true;

		RegisterNativeModules("dotnet", commandLine);
		Script_SetActiveBackend(&runtime);

		if (!CEterPackManager::Instance().isExist(DotNetEntryAssembly))
		{
			TraceError("%s not found in the packs", DotNetEntryAssembly);
			return Result::ScriptFailure;
		}

		std::vector<std::string> args;
		SplitLine(commandLine.c_str(), " ", &args);
		if (!runtime.RunEntry(DotNetEntryAssembly, args))
		{
			TraceError("%s failed with a .NET exception", DotNetEntryAssembly);
			return Result::ScriptFailure;
		}
		return Result::Success;
#else
		(void)commandLine;
		TraceError("This client was built without the .NET script runtime (OPENMT2_SCRIPT_DOTNET=OFF)");
		return Result::RuntimeFailure;
#endif
	}

	void ScriptRuntime::Stop()
	{
#if OPENMT2_SCRIPT_DOTNET
		if (m_dotnetStarted)
		{
			CDotNetRuntime& runtime = CDotNetRuntime::Instance();
			if (Script_GetActiveBackend() == &runtime)
				Script_SetActiveBackend(nullptr);
			runtime.Shutdown();
			SetExceptionSender(nullptr);
			g_dotnetExceptionSender.reset();
			m_dotnetStarted = false;
		}
#endif
		m_python.Stop();
	}
}
