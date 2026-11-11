#include <Client/Application/PythonRuntime.h>

#include <Client/Application/Context.h>

#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonExceptionSender.h>

#include <Neoeter/Python/Host/PythonLauncher.h>
#include <Neoeter/Python/Host/StdAfx.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/Debug.h>

namespace Neoeter
{
	PythonRuntime::PythonRuntime() : m_launcher(), m_exceptionSender()
	{
	}

	PythonRuntime::~PythonRuntime()
	{
		Stop();
	}

	PythonRuntime::Result PythonRuntime::Start(const std::string& commandLine)
	{
		m_exceptionSender.reset(new CPythonExceptionSender);
		SetExceptionSender(m_exceptionSender.get());

		m_launcher.reset(new CPythonLauncher);
		if (!m_launcher->Create())
		{
			TraceError("Failed to initialize the Python launcher.");
			return Result::RuntimeFailure;
		}

		RegisterModules();

		if (!RegisterDebugFlag())
		{
			TraceError("RegisterDebugFlag Error");
			return Result::ScriptFailure;
		}

		if (!RegisterCommandLine(commandLine))
		{
			TraceError("RegisterCommandLine Error");
			return Result::ScriptFailure;
		}

		if (!RunSystemScript())
			return Result::ScriptFailure;

		return Result::Success;
	}

	void PythonRuntime::Stop()
	{
		if (m_launcher)
			m_launcher->Clear();

		SetExceptionSender(nullptr);
		m_launcher.reset();
		m_exceptionSender.reset();
	}

	void PythonRuntime::RegisterModules()
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
	}

	bool PythonRuntime::RegisterDebugFlag()
	{
		return m_launcher->RunLine(Context::IsDistribute() ? "__DEBUG__ = 0" : "__DEBUG__ = 1");
	}

	bool PythonRuntime::RegisterCommandLine(const std::string& commandLine)
	{
		std::string line = "__COMMAND_LINE__ = \"";
		line += commandLine;
		line += '"';
		return m_launcher->RunLine(line.c_str());
	}

	bool PythonRuntime::RunSystemScript()
	{
		if (!CEterPackManager::Instance().isExist("system.py"))
		{
			TraceError("system.py not found; check that the pack folder with Index and the archive files is next to the client");
			return false;
		}

		if (!m_launcher->RunFile("system.py"))
		{
			TraceError("system.py failed with a Python exception");
			return false;
		}

		return true;
	}
}
