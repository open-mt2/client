#include <Client/Application/PlatformRuntime.h>

#include <Neoeter/Model/Grn/GrannyCompat.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Platform/Base/Utils.h>
#include <Neoeter/Platform/Port.h>

namespace
{
	void GrannyError(granny_log_message_type type, granny_log_message_origin origin, char const* error, void* userData)
	{
		TraceError("GRANNY: %s", error);
	}
}

namespace OMT2Client::Application
{
	PlatformRuntime::PlatformRuntime() : m_timerRaised(false), m_timerResolution(0)
	{
	}

	PlatformRuntime::~PlatformRuntime()
	{
		Shutdown();
	}

	bool PlatformRuntime::Initialize()
	{
		uint32_t period = 0;
		if (!Neoeter::Platform::RaiseTimerResolution(&period))
			return false;

		m_timerResolution = period;
		m_timerRaised = true;

		granny_log_callback callback;
		callback.Function = GrannyError;
		callback.UserData = 0;
		GrannySetLogCallback(&callback);

		return true;
	}

	void PlatformRuntime::Shutdown()
	{
		if (!m_timerRaised)
			return;

		Neoeter::Platform::RestoreTimerResolution(m_timerResolution);
		m_timerRaised = false;
	}
}
