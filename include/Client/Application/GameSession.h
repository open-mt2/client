#pragma once

#include <Client/Application/PlatformRuntime.h>
#include <Client/Script/ScriptHost.h>

namespace OMT2Client::Application
{
	class CommandLine;
	class Context;
}

class CPythonApplication;

namespace OMT2Client::Application
{
	class GameSession
	{
		public:
			enum class Result
			{
				Success,
				Failure,
				Aborted,
			};

			GameSession(Context& context, CPythonApplication& application);
			~GameSession();

			GameSession(const GameSession&) = delete;
			GameSession& operator=(const GameSession&) = delete;

			Result Run(const CommandLine& arguments);

		private:
			bool Prepare();
			void Cleanup();

			Context& m_context;
			CPythonApplication& m_application;
			PlatformRuntime m_platform;
			OMT2Client::Script::ScriptHost m_script;
	};
}
