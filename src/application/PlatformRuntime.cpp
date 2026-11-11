#include <Client/Application/PlatformRuntime.h>

#include <Neoeter/Model/Grn/GrannyCompat.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Platform/Base/Utils.h>
#include <Platform/Platform.h>

namespace
{
	void GrannyError(granny_log_message_type type, granny_log_message_origin origin, char const* error, void* userData)
	{
		TraceError("GRANNY: %s", error);
	}
}

namespace Neoeter
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
		TIMECAPS timeCaps;
		if (timeGetDevCaps(&timeCaps, sizeof(TIMECAPS)) != TIMERR_NOERROR)
			return false;

		m_timerResolution = MINMAX(timeCaps.wPeriodMin, 1, timeCaps.wPeriodMax);
		timeBeginPeriod(m_timerResolution);
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

		timeEndPeriod(m_timerResolution);
		m_timerRaised = false;
	}
}
