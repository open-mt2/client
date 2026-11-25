#pragma once

#include <Client/Application/ScriptRuntime.h>
#include <Platform/Platform.h>

#include <string>

namespace Neoeter
{
	class CommandLine
	{
		public:
			enum class Result
			{
				Continue,
				ExitSuccess,
				InvalidArguments,
			};

			explicit CommandLine(const std::string& commandLine);
			~CommandLine();

			CommandLine(const CommandLine&) = delete;
			CommandLine& operator=(const CommandLine&) = delete;

			Result Process();

			const std::string& PythonCommandLine() const;
			bool PauseBeforeScript() const;
			// --script-runtime=auto|python|dotnet, else $OPENMT2_SCRIPT_RUNTIME.
			ScriptRuntime::Kind ScriptRuntimeKind() const;

		private:
			std::string NormalizePythonCommandLine() const;
			void ExtractScriptRuntime();
			void WriteRevisionFile() const;
			void WriteLocaleFile() const;
			void PrintVersionBanner() const;

			std::string m_commandLine;
			std::string m_pythonCommandLine;
			PCHAR* m_argv;
			int m_count;
			bool m_pauseBeforeScript;
			ScriptRuntime::Kind m_scriptRuntime;
			std::string m_invalidScriptRuntime;
	};
}
