#pragma once

#include <Platform/Platform.h>

#include <memory>
#include <string>

class CPythonApplication;

namespace Neoeter
{
	class Application
	{
		public:
			Application();
			~Application();

			Application(const Application&) = delete;
			Application& operator=(const Application&) = delete;

			int Run(int argc, char** argv);
			int Run(const std::string& commandLine);

		private:
			enum class CommandLineResult
			{
				Launch,
				ExitSuccess,
				ExitFailure,
			};

			void Initialize();
			CommandLineResult ProcessCommandLine(const std::string& commandLine);
			bool ValidateLaunch(const std::string& commandLine) const;
			int RunGame(const std::string& commandLine);
			void Shutdown();

			HINSTANCE m_hInstance;
			std::unique_ptr<CPythonApplication> m_pythonApplication;
	};
}
