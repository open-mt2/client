#include <Client/Application/Context.h>
#include <Neoeter/Platform/Port.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include <cstring>

bool __IS_TEST_SERVER_MODE__ = false;

namespace
{
	char gs_errorString[512] = "";
}

namespace Neoeter
{
	Context::Context() : m_instance(Neoeter::Platform::ModuleHandle())
	{
	}

	Context::~Context() = default;

	PlatformInstance Context::Instance() const
	{
		return m_instance;
	}

	bool Context::IsDebug()
	{
#ifdef _DEBUG
		return true;
#else
		return false;
#endif
	}

	bool Context::IsLinux()
	{
		return Neoeter::Platform::IsLinux();
	}

	bool Context::IsDistribute()
	{
#ifdef _DISTRIBUTE
		return true;
#else
		return false;
#endif
	}

	bool Context::IsTestServer()
	{
		return __IS_TEST_SERVER_MODE__;
	}

	void Context::SetTestServer(bool enabled)
	{
		__IS_TEST_SERVER_MODE__ = enabled;
	}

	void Context::EnableDebugHeapChecks()
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}

	const char* Context::ClientVersion()
	{
#ifdef OPENMT2_CLIENT_VERSION
		return OPENMT2_CLIENT_VERSION;
#else
		return "unknown";
#endif
	}

	int Context::CompilerVersion()
	{
		return Neoeter::Platform::CompilerVersion();
	}

	void Context::SetErrorString(const char* error)
	{
		if (error == nullptr)
		{
			gs_errorString[0] = '\0';
			return;
		}

		strncpy(gs_errorString, error, sizeof(gs_errorString) - 1);
		gs_errorString[sizeof(gs_errorString) - 1] = '\0';
	}

	const char* Context::ErrorString()
	{
		return gs_errorString;
	}
}

void ApplicationSetErrorString(const char* szErrorString)
{
	Neoeter::Context::SetErrorString(szErrorString);
}
