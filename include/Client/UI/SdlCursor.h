#pragma once

// SDL hardware cursors (T3.2).
//
// The original client loaded its cursor bitmaps from Win32 resources
// (`LoadImage(..., IMAGE_CURSOR, ...)`). No resource script ships with the
// sources, so on the Windows target that call always fails and the client stops
// with a "CREATE_CURSOR" message box before the login screen. Since T1.2/T1.3
// both targets use SDL for the window and input, use SDL system cursors instead.
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
