#pragma once

#include <Client/Application/PythonRuntime.h>

#include <string>

namespace Neoeter
{
	// Starts the game scripts on one of the scripting runtimes:
	//   python - system.py on the embedded Python 2.7 interpreter (legacy);
	//   dotnet - the entry assembly (scripts/OpenMT2.Scripts.dll) on .NET 10;
	//   auto   - dotnet when the entry assembly is in the packs, else python.
	// Both see the same native modules, so scripts can migrate one at a time.
	class ScriptRuntime
	{
		public:
			enum class Kind
			{
				Auto,
				Python,
				DotNet,
			};

			using Result = PythonRuntime::Result;

			static constexpr const char* DotNetEntryAssembly = "scripts/OpenMT2.Scripts.dll";

			ScriptRuntime();
			~ScriptRuntime();

			ScriptRuntime(const ScriptRuntime&) = delete;
			ScriptRuntime& operator=(const ScriptRuntime&) = delete;

			// Accepts "auto", "python" or "dotnet" (case-sensitive).
			static bool ParseKind(const std::string& text, Kind* kind);
			static const char* KindName(Kind kind);
			static bool IsDotNetAvailable();

			Result Start(Kind kind, const std::string& commandLine);
			void Stop();

			// The runtime that was started (Auto before Start).
			Kind Active() const { return m_active; }

		private:
			Kind Resolve(Kind requested) const;
			Result StartDotNet(const std::string& commandLine);

			PythonRuntime m_python;
			Kind m_active;
			bool m_dotnetStarted;
	};

	// Registers the engine's native modules (app, chr, net, ...) and the
	// scriptHost module (NAME, DEBUG, COMMAND_LINE) in the neutral registry.
	void RegisterNativeModules(const char* runtimeName, const std::string& commandLine);
}
