#include <Client/UI/StdAfx.h>
#include <Client/UI/QuitRequest.h>
#include <Neoeter/Render/SdlPlatform.h>

void Client_RequestQuit()
{
	Platform_GetQuitRequested().store(true);
}
