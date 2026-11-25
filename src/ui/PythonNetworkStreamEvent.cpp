#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonNetworkStream.h>

void CPythonNetworkStream::OnRemoteDisconnect()
{
	Script_CallMethod(m_poHandler, "SetLoginPhase", Script_BuildValue("()"));
}

void CPythonNetworkStream::OnDisconnect()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Game
void CPythonNetworkStream::OnScriptEventStart(int iSkin, int iIndex)
{
	Script_CallMethod(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenQuestWindow", Script_BuildValue("(ii)", iSkin, iIndex));
}