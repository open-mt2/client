#pragma once

#include <Platform/Platform.h>

#include <string>

namespace Neoeter
{
	class Context
	{
		public:
			Context();
			~Context();

			Context(const Context&) = delete;
			Context& operator=(const Context&) = delete;

			HINSTANCE Instance() const;

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
			HINSTANCE m_instance;
	};
}
