#include <Client/Application/CLI.h>

#include <Client/Application/Context.h>
#include <Client/UI/Locale.h>
#include <Client/UI/Version.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Platform/Base/Utils.h>
#include <Neoeter/Render/Util.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace Neoeter
{
	CommandLine::CommandLine(const std::string& commandLine)
		: m_commandLine(commandLine), m_pythonCommandLine(), m_argv(nullptr), m_count(0), m_pauseBeforeScript(false), m_scriptRuntime(ScriptRuntime::Kind::Auto)
	{
		ExtractScriptRuntime();
		m_pythonCommandLine = NormalizePythonCommandLine();
		m_argv = CommandLineToArgv(m_commandLine.empty() ? const_cast<PCHAR>("") : &m_commandLine[0], &m_count);

		std::vector<std::string> tokens;
		SplitLine(m_commandLine.c_str(), " ", &tokens);
		m_pauseBeforeScript = !tokens.empty() && tokens[0] == "--pause-before-create-window";
	}

	CommandLine::~CommandLine()
	{
		SAFE_FREE_GLOBAL(m_argv);
	}

	void CommandLine::ExtractScriptRuntime()
	{
		static const std::string option = "--script-runtime";

		std::string value;
		if (const char* env = getenv("OPENMT2_SCRIPT_RUNTIME"))
			value = env;

		std::vector<std::string> tokens;
		SplitLine(m_commandLine.c_str(), " ", &tokens);

		std::vector<std::string> kept;
		for (std::size_t i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i].compare(0, option.size() + 1, option + "=") == 0)
				value = tokens[i].substr(option.size() + 1);
			else if (tokens[i] == option && i + 1 < tokens.size())
				value = tokens[++i];
			else
				kept.push_back(tokens[i]);
		}

		if (kept.size() != tokens.size())
		{
			// The scripts parse the command line; they never see this option.
			m_commandLine.clear();
			if (!kept.empty())
				string_join(" ", kept, &m_commandLine);
		}

		if (!value.empty() && !ScriptRuntime::ParseKind(value, &m_scriptRuntime))
			m_invalidScriptRuntime = value;
	}

	ScriptRuntime::Kind CommandLine::ScriptRuntimeKind() const
	{
		return m_scriptRuntime;
	}

	CommandLine::Result CommandLine::Process()
	{
		if (!m_invalidScriptRuntime.empty())
		{
			TraceError("Unknown script runtime '%s' (expected auto, python or dotnet)", m_invalidScriptRuntime.c_str());
			return Result::InvalidArguments;
		}

		bool quit = false;

		for (int i = 0; i < m_count; ++i)
		{
			const char* argument = m_argv[i];
			if (argument == nullptr)
				continue;

			if (strcmp(argument, "--perforce-revision") == 0)
			{
				WriteRevisionFile();
				quit = true;
			}
			else if (strcmp(argument, "--locale") == 0)
			{
				WriteLocaleFile();
				quit = true;
			}
			else if (strncmp(argument, "/timestamp", strlen("/timestamp")) == 0)
			{
				PrintVersionBanner();
				quit = true;
			}
		}

		return quit ? Result::ExitSuccess : Result::Continue;
	}

	const std::string& CommandLine::PythonCommandLine() const
	{
		return m_pythonCommandLine;
	}

	bool CommandLine::PauseBeforeScript() const
	{
		return m_pauseBeforeScript;
	}

	std::string CommandLine::NormalizePythonCommandLine() const
	{
		const char* loginMark = "-cs";
		const char* plainLoginMark = "-ncs";
		const char* separator = " ";
		const int encodedArgumentCount = 3;

		std::vector<std::string> tokens;
		SplitLine(m_commandLine.c_str(), separator, &tokens);

		if (static_cast<int>(tokens.size()) == encodedArgumentCount && tokens[0] == loginMark)
		{
			char buffer[MAX_PATH];
			base64_decode(tokens[2].c_str(), buffer);
			tokens[2] = buffer;

			std::string result;
			string_join(separator, tokens, &result);
			return result;
		}

		if (static_cast<int>(tokens.size()) >= encodedArgumentCount && tokens[0] == plainLoginMark)
		{
			tokens[0] = loginMark;

			std::string result;
			string_join(separator, tokens, &result);
			return result;
		}

		return m_commandLine;
	}

	void CommandLine::WriteRevisionFile() const
	{
		char moduleName[MAX_PATH];
		char versionPath[MAX_PATH + 16];
		GetModuleFileName(NULL, moduleName, sizeof(moduleName));
		snprintf(versionPath, sizeof(versionPath), "%s.version", moduleName);

		FILE* file = fopen(versionPath, "wt");
		if (file == nullptr)
			return;

		fprintf(file, "r%d\n", OPENMT2_GET_VERSION());
		fclose(file);
	}

	void CommandLine::WriteLocaleFile() const
	{
		FILE* file = fopen("locale.txt", "wt");
		if (file == nullptr)
			return;

		fprintf(file, "service[%s] code_page[%d]", LocaleService_GetName(), LocaleService_GetCodePage());
		fclose(file);
	}

	void CommandLine::PrintVersionBanner() const
	{
		if (Context::IsDebug())
		{
			if (Context::IsTestServer())
				LogBoxf("OpenMT2 BINARY TEST DEBUG VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, Context::CompilerVersion());
			else
				LogBoxf("OpenMT2 BINARY DEBUG VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, Context::CompilerVersion());
		}
		else
		{
			if (Context::IsTestServer())
				LogBoxf("OpenMT2 BINARY TEST VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, Context::CompilerVersion());
			else
				LogBoxf("OpenMT2 BINARY DISTRIBUTE VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, Context::CompilerVersion());
		}
	}
}
