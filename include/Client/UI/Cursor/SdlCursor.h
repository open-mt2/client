#pragma once

// SDL hardware cursors.
//
// No resource script for the Win32 cursor loader ships with the sources, so
// that call would always fail on the Windows target. Both targets use SDL for
// the window and input, so use SDL system cursors instead.
//
// The helpers keep the Windows-only SetCursor/ShowCursor/DestroyCursor calls in
// one place so the call sites stay platform-neutral.

#include <Neoeter/Platform/PortTypes.h>

#include <SDL2/SDL.h>

inline PlatformHandle Platform_CreateSystemCursor(SDL_SystemCursor cursor)
{
	return reinterpret_cast<PlatformHandle>(SDL_CreateSystemCursor(cursor));
}

inline void Platform_DestroyCursor(PlatformHandle handle)
{
	if (handle)
		SDL_FreeCursor(reinterpret_cast<SDL_Cursor*>(handle));
}

inline void Platform_SetCursorHandle(PlatformHandle handle)
{
	SDL_SetCursor(reinterpret_cast<SDL_Cursor*>(handle));
}

// SDL_ShowCursor returns the previous 0/1 state, not the Win32 reference
// counter; callers must not drive a do/while loop with it.
inline int Platform_ShowCursorFlag(int32_t show)
{
	return SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}
