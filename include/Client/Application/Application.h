#pragma once

#include <Client/Application/Context.h>

#include <memory>
#include <string>

class CPythonApplication;

namespace Neoeter
{
	class CommandLine;

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
			void Initialize();
			int RunGame(const CommandLine& arguments);
			void Shutdown();

			Context m_context;
			std::unique_ptr<CPythonApplication> m_pythonApplication;
	};
}
