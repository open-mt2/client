#pragma once

#include <Neoeter/Platform/Platform.h>

namespace OMT2Client::Application
{
	class Context
	{
		public:
			Context();
			~Context();

			Context(const Context&) = delete;
			Context& operator=(const Context&) = delete;

			PlatformInstance Instance() const;

			static bool IsDebug();
			static bool IsLinux();
			static bool IsDistribute();

			static bool IsTestServer();
			static void SetTestServer(bool enabled);

			static void EnableDebugHeapChecks();
			static const char* ClientVersion();
			static int CompilerVersion();

			static void SetErrorString(const char* error);
			static const char* ErrorString();

		private:
			PlatformInstance m_instance;
	};
}
