#include <Client/UI/StdAfx.h>
#include <Client/Application/QuitRequest.h>
#include <Neoeter/Platform/PlatformWindow.h>

void Client_RequestQuit()
{
	Platform_GetQuitRequested().store(true);
}
