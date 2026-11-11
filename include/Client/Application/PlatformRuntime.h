#pragma once

namespace Neoeter
{
	class PlatformRuntime
	{
		public:
			PlatformRuntime();
			~PlatformRuntime();

			PlatformRuntime(const PlatformRuntime&) = delete;
			PlatformRuntime& operator=(const PlatformRuntime&) = delete;

			bool Initialize();
			void Shutdown();

		private:
			bool m_timerRaised;
			unsigned int m_timerResolution;
	};
}
