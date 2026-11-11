#pragma once

#include <memory>
#include <string>

class CPythonLauncher;
class CPythonExceptionSender;

namespace Neoeter
{
	class PythonRuntime
	{
		public:
			enum class Result
			{
				Success,
				RuntimeFailure,
				ScriptFailure,
			};

			PythonRuntime();
			~PythonRuntime();

			PythonRuntime(const PythonRuntime&) = delete;
			PythonRuntime& operator=(const PythonRuntime&) = delete;

			Result Start(const std::string& commandLine);
			void Stop();

		private:
			void RegisterModules();
			bool RegisterDebugFlag();
			bool RegisterCommandLine(const std::string& commandLine);
			bool RunSystemScript();

			std::unique_ptr<CPythonLauncher> m_launcher;
			std::unique_ptr<CPythonExceptionSender> m_exceptionSender;
	};
}
