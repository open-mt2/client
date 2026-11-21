#include <cstdint>

extern "C"
{
#if defined(_MSC_VER)
	extern int32_t _fltused;
	volatile int32_t _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;
#endif
	__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int32_t AmdPowerXpressRequestHighPerformance = 1;
};
