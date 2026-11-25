#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonNetworkStream.h>
// #include "PythonNetworkDatagram.h"
#include <Client/UI/AccountConnector.h>
#include <Client/UI/PythonGuild.h>
#include <Client/UI/Test.h>

#include <Client/UI/AbstractPlayer.h>

static std::string gs_stServerInfo;
extern BOOL gs_bEmpireLanuageEnable;
std::list<std::string> g_kList_strCommand;

ScriptObject* netGetBettingGuildWarValue(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.EXPORT_GetBettingGuildWarValue(szName));
}

ScriptObject* netSetServerInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	gs_stServerInfo = szFileName;
	return Script_BuildNone();
}

ScriptObject* netGetServerInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("s", gs_stServerInfo.c_str());
}

ScriptObject* netPreserveServerCommand(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szLine;
	if (!ScriptTuple_GetString(poArgs, 0, &szLine))
		return Script_BuildException();

	g_kList_strCommand.push_back(szLine);

	return Script_BuildNone();
}

ScriptObject* netGetPreservedServerCommand(ScriptObject* poSelf, ScriptObject* poArgs)
{
	if (g_kList_strCommand.empty())
		return Script_BuildValue("s", "");

	std::string strCommand = g_kList_strCommand.front();
	g_kList_strCommand.pop_front();

	return Script_BuildValue("s", strCommand.c_str());
}

ScriptObject* netStartGame(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.StartGame();

	return Script_BuildNone();
}

ScriptObject* netIsTest(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", __IS_TEST_SERVER_MODE__);
}

ScriptObject* netWarp(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nX;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nX))
		return Script_BuildException();

	int nY;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nY))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.Warp(nX, nY);

	return Script_BuildNone();
}

ScriptObject* netLoadInsultList(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.LoadInsultList(szFileName));
}

ScriptObject* netUploadMark(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.UploadMark(szFileName));
}

ScriptObject* netUploadSymbol(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.UploadSymbol(szFileName));
}

ScriptObject* netGetGuildID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.GetGuildID());
}

ScriptObject* netGetEmpireID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.GetEmpireID());
}

ScriptObject* netGetMainActorVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.GetMainActorVID());
}

ScriptObject* netGetMainActorRace(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.GetMainActorRace());
}

ScriptObject* netGetMainActorEmpire(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.GetMainActorEmpire());
}

ScriptObject* netGetMainActorSkillGroup(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.GetMainActorSkillGroup());
}

ScriptObject* netIsSelectedEmpire(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.IsSelectedEmpire());
}

ScriptObject* netGetAccountCharacterSlotDataInteger(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nIndex))
		return Script_BuildException();

	int nType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nType))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	UINT uResult = rkNetStream.GetAccountCharacterSlotDatau(nIndex, nType);
	return Script_BuildValue("i", uResult);
}

ScriptObject* netGetAccountCharacterSlotDataString(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nIndex))
		return Script_BuildException();

	int nType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nType))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("s", rkNetStream.GetAccountCharacterSlotDataz(nIndex, nType));
}

// SUPPORT_BGM
ScriptObject* netGetFieldMusicFileName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("s", rkNetStream.GetFieldMusicFileName());
}

ScriptObject* netGetFieldMusicVolume(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("f", rkNetStream.GetFieldMusicVolume());
}
// END_OF_SUPPORT_BGM

ScriptObject* netSetPhaseWindow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ePhaseWnd;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ePhaseWnd))
		return Script_BuildException();

	ScriptObject* poPhaseWnd;
	if (!ScriptTuple_GetObject(poArgs, 1, &poPhaseWnd))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetPhaseWindow(ePhaseWnd, poPhaseWnd);
	return Script_BuildNone();
}

ScriptObject* netClearPhaseWindow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ePhaseWnd;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ePhaseWnd))
		return Script_BuildException();

	ScriptObject* poPhaseWnd;
	if (!ScriptTuple_GetObject(poArgs, 1, &poPhaseWnd))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ClearPhaseWindow(ePhaseWnd, poPhaseWnd);
	return Script_BuildNone();
}

ScriptObject* netSetServerCommandParserWindow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poPhaseWnd;
	if (!ScriptTuple_GetObject(poArgs, 0, &poPhaseWnd))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetServerCommandParserWindow(poPhaseWnd);
	return Script_BuildNone();
}

ScriptObject* netSetAccountConnectorHandler(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poPhaseWnd;
	if (!ScriptTuple_GetObject(poArgs, 0, &poPhaseWnd))
		return Script_BuildException();

	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkAccountConnector.SetHandler(poPhaseWnd);
	return Script_BuildNone();
}

ScriptObject* netSetHandler(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poHandler;

	if (!ScriptTuple_GetObject(poArgs, 0, &poHandler))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetHandler(poHandler);
	return Script_BuildNone();
}

ScriptObject* netSetTCPRecvBufferSize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int bufSize;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bufSize))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetRecvBufferSize(bufSize);
	return Script_BuildNone();
}

ScriptObject* netSetTCPSendBufferSize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int bufSize;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bufSize))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetSendBufferSize(bufSize);
	return Script_BuildNone();
}

ScriptObject* netSetMarkServer(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szAddr;
	if (!ScriptTuple_GetString(poArgs, 0, &szAddr))
		return Script_BuildException();

	int port;
	if (!ScriptTuple_GetInteger(poArgs, 1, &port))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetMarkServer(szAddr, port);
	return Script_BuildNone();
}

ScriptObject* netConnectTCP(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szAddr;
	if (!ScriptTuple_GetString(poArgs, 0, &szAddr))
		return Script_BuildException();

	int port;
	if (!ScriptTuple_GetInteger(poArgs, 1, &port))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ConnectLoginServer(szAddr, port);
	return Script_BuildNone();
}

ScriptObject* netConnectToAccountServer(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* addr;
	if (!ScriptTuple_GetString(poArgs, 0, &addr))
		return Script_BuildException();

	int port;
	if (!ScriptTuple_GetInteger(poArgs, 1, &port))
		return Script_BuildException();

	char* account_addr;
	if (!ScriptTuple_GetString(poArgs, 2, &account_addr))
		return Script_BuildException();

	int account_port;
	if (!ScriptTuple_GetInteger(poArgs, 3, &account_port))
		return Script_BuildException();

	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkAccountConnector.Connect(addr, port, account_addr, account_port);
	return Script_BuildNone();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptObject* netSetLoginInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	char* szPwd;
	if (!ScriptTuple_GetString(poArgs, 1, &szPwd))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkNetStream.SetLoginInfo(szName, szPwd);
	rkAccountConnector.SetLoginInfo(szName, szPwd);
	return Script_BuildNone();
}

ScriptObject* netSetOfflinePhase(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetOffLinePhase();
	return Script_BuildNone();
}

ScriptObject* netSendSelectEmpirePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iEmpireIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iEmpireIndex))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendSelectEmpirePacket(iEmpireIndex);
	return Script_BuildNone();
}

ScriptObject* netSendLoginPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	char* szPwd;
	if (!ScriptTuple_GetString(poArgs, 1, &szPwd))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendLoginPacket(szName, szPwd);
	return Script_BuildNone();
}

ScriptObject* netSendChinaMatrixCardPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szMatrixCardString;
	if (!ScriptTuple_GetString(poArgs, 0, &szMatrixCardString))
		return Script_BuildException();

	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkAccountConnector.SendChinaMatrixCardPacket(szMatrixCardString);
	return Script_BuildNone();
}

ScriptObject* netSendRunupMatrixCardPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szMatrixCardString;
	if (!ScriptTuple_GetString(poArgs, 0, &szMatrixCardString))
		return Script_BuildException();

	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkAccountConnector.SendRunupMatrixCardPacket(szMatrixCardString);
	return Script_BuildNone();
}

ScriptObject* netSendNEWCIBNPasspodAnswerPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szMatrixCardString;
	if (!ScriptTuple_GetString(poArgs, 0, &szMatrixCardString))
		return Script_BuildException();

	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkAccountConnector.SendNEWCIBNPasspodAnswerPacket(szMatrixCardString);
	return Script_BuildNone();
}

ScriptObject* netDirectEnter(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nChrSlot;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nChrSlot))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ConnectGameServer(nChrSlot);
	return Script_BuildNone();
}

ScriptObject* netSendSelectCharacterPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int Index;
	if (!ScriptTuple_GetInteger(poArgs, 0, &Index))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendSelectCharacterPacket((BYTE)Index);
	return Script_BuildNone();
}

ScriptObject* netSendChangeNamePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 1, &szName))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendChangeNamePacket((BYTE)iIndex, szName);
	return Script_BuildNone();
}

ScriptObject* netEnableChatInsultFilter(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nEnable))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.EnableChatInsultFilter(nEnable ? true : false);
	return Script_BuildNone();
}

ScriptObject* netIsChatInsultIn(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szMsg;
	if (!ScriptTuple_GetString(poArgs, 0, &szMsg))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.IsChatInsultIn(szMsg));
}

ScriptObject* netIsInsultIn(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szMsg;
	if (!ScriptTuple_GetString(poArgs, 0, &szMsg))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.IsInsultIn(szMsg));
}

ScriptObject* netSendWhisperPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	char* szLine;

	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	if (!ScriptTuple_GetString(poArgs, 1, &szLine))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendWhisperPacket(szName, szLine);
	return Script_BuildNone();
}

ScriptObject* netSendCharacterPositionPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPosition;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPosition))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendCharacterPositionPacket((BYTE)iPosition);
	return Script_BuildNone();
}

ScriptObject* netSendChatPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szLine;
	if (!ScriptTuple_GetString(poArgs, 0, &szLine))
		return Script_BuildException();
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iType))
	{
		iType = CHAT_TYPE_TALKING;
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendChatPacket(szLine, iType);
	return Script_BuildNone();
}

ScriptObject* netSendEmoticon(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eEmoticon;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eEmoticon))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendEmoticon(eEmoticon);
	return Script_BuildNone();
}

ScriptObject* netSendCreateCharacterPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index;
	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BuildException();

	char* name;
	if (!ScriptTuple_GetString(poArgs, 1, &name))
		return Script_BuildException();

	int job;
	if (!ScriptTuple_GetInteger(poArgs, 2, &job))
		return Script_BuildException();

	int shape;
	if (!ScriptTuple_GetInteger(poArgs, 3, &shape))
		return Script_BuildException();

	int stat1;
	if (!ScriptTuple_GetInteger(poArgs, 4, &stat1))
		return Script_BuildException();
	int stat2;
	if (!ScriptTuple_GetInteger(poArgs, 5, &stat2))
		return Script_BuildException();
	int stat3;
	if (!ScriptTuple_GetInteger(poArgs, 6, &stat3))
		return Script_BuildException();
	int stat4;
	if (!ScriptTuple_GetInteger(poArgs, 7, &stat4))
		return Script_BuildException();

	if (index < 0 && index > 3)
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendCreateCharacterPacket((BYTE)index, name, (BYTE)job, (BYTE)shape, stat1, stat2, stat3, stat4);
	return Script_BuildNone();
}

ScriptObject* netSendDestroyCharacterPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index;
	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BuildException();

	char* szPrivateCode;
	if (!ScriptTuple_GetString(poArgs, 1, &szPrivateCode))
		return Script_BuildException();

	if (index < 0 && index > 3)
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendDestroyCharacterPacket((BYTE)index, szPrivateCode);
	return Script_BuildNone();
}

ScriptObject* netSendEnterGamePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendEnterGame();
	return Script_BuildNone();
}

ScriptObject* netOnClickPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index;
	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendOnClickPacket(index);

	return Script_BuildNone();
}

ScriptObject* netSendItemUsePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;
	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();
			break;
		case 2:
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemUsePacket(Cell);
	return Script_BuildNone();
}

ScriptObject* netSendItemUseToItemPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos SourceCell;
	TItemPos TargetCell;
	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &SourceCell.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetCell.cell))
				return Script_BuildException();
			break;
		case 4:
			if (!ScriptTuple_GetByte(poArgs, 0, &SourceCell.window_type))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &SourceCell.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetByte(poArgs, 2, &TargetCell.window_type))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 3, &TargetCell.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemUseToItemPacket(SourceCell, TargetCell);
	return Script_BuildNone();
}

ScriptObject* netSendItemDropPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;
	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();
			break;
		case 2:
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemDropPacket(Cell, 0);
	return Script_BuildNone();
}

ScriptObject* netSendItemDropPacketNew(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;
	int count;
	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &count))
				return Script_BuildException();

			break;
		case 3:
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &count))
				return Script_BuildException();

			break;
		default:
			return Script_BuildException();
	}
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemDropPacketNew(Cell, 0, count);
	return Script_BuildNone();
}

ScriptObject* netSendElkDropPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iElk;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iElk))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemDropPacket(TItemPos(RESERVED_WINDOW, 0), (DWORD)iElk);
	return Script_BuildNone();
}

ScriptObject* netSendGoldDropPacketNew(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iElk;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iElk))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemDropPacketNew(TItemPos(RESERVED_WINDOW, 0), (DWORD)iElk, 0);
	return Script_BuildNone();
}

ScriptObject* netSendItemMovePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;
	TItemPos ChangeCell;
	int num;

	switch (ScriptTuple_Size(poArgs))
	{
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &ChangeCell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &num))
				return Script_BuildException();
			break;
		case 5:
		{
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetByte(poArgs, 2, &ChangeCell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 3, &ChangeCell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 4, &num))
				return Script_BuildException();
		}
		break;
		default:
			return Script_BuildException();
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemMovePacket(Cell, ChangeCell, (BYTE)num);
	return Script_BuildNone();
}

ScriptObject* netSendItemPickUpPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int vid;
	if (!ScriptTuple_GetInteger(poArgs, 0, &vid))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemPickUpPacket(vid);
	return Script_BuildNone();
}

ScriptObject* netSendGiveItemPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iTargetVID;
	TItemPos Cell;
	int iItemCount;
	switch (ScriptTuple_Size(poArgs))
	{
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iTargetVID))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &iItemCount))
				return Script_BuildException();
			break;
		case 4:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iTargetVID))
				return Script_BuildException();
			if (!ScriptTuple_GetByte(poArgs, 1, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &Cell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 3, &iItemCount))
				return Script_BuildException();
			break;
		default:
			break;
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendGiveItemPacket(iTargetVID, Cell, iItemCount);
	return Script_BuildNone();
}

ScriptObject* netSendShopEndPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopEndPacket();
	return Script_BuildNone();
}

ScriptObject* netSendShopBuyPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iCount;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iCount))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopBuyPacket(iCount);
	return Script_BuildNone();
}

ScriptObject* netSendShopSellPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotNumber;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotNumber))
		return Script_BuildException();
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopSellPacket(iSlotNumber);
	return Script_BuildNone();
}

ScriptObject* netSendShopSellPacketNew(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotNumber;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotNumber))
		return Script_BuildException();
	int iCount;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iCount))
		return Script_BuildException();
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopSellPacketNew(iSlotNumber, iCount);
	return Script_BuildNone();
}

ScriptObject* netSendExchangeStartPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int vid;
	if (!ScriptTuple_GetInteger(poArgs, 0, &vid))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeStartPacket(vid);
	return Script_BuildNone();
}

ScriptObject* netSendExchangeElkAddPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iElk;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iElk))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeElkAddPacket(iElk);
	return Script_BuildNone();
}

ScriptObject* netSendExchangeItemAddPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bWindowType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bWindowType))
		return Script_BuildException();
	WORD wSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &wSlotIndex))
		return Script_BuildException();
	int iDisplaySlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iDisplaySlotIndex))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeItemAddPacket(TItemPos(bWindowType, wSlotIndex), iDisplaySlotIndex);
	return Script_BuildNone();
}

ScriptObject* netSendExchangeItemDelPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int pos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeItemDelPacket((BYTE)pos);
	return Script_BuildNone();
}

ScriptObject* netSendExchangeAcceptPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeAcceptPacket();
	return Script_BuildNone();
}

ScriptObject* netSendExchangeExitPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeExitPacket();
	return Script_BuildNone();
}

ScriptObject* netExitApplication(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ExitApplication();
	return Script_BuildNone();
}

ScriptObject* netExitGame(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ExitGame();
	return Script_BuildNone();
}

ScriptObject* netLogOutGame(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.LogOutGame();
	return Script_BuildNone();
}

ScriptObject* netDisconnect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetOffLinePhase();
	rkNetStream.Disconnect();

	return Script_BuildNone();
}

ScriptObject* netIsConnect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Script_BuildValue("i", rkNetStream.IsOnline());
}

ScriptObject* netToggleGameDebugInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ToggleGameDebugInfo();
	return Script_BuildNone();
}

ScriptObject* netRegisterEmoticonString(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* pcEmoticonString;
	if (!ScriptTuple_GetString(poArgs, 0, &pcEmoticonString))
		return Script_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.RegisterEmoticonString(pcEmoticonString);
	return Script_BuildNone();
}

ScriptObject* netSendMessengerAddByVIDPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int vid;
	if (!ScriptTuple_GetInteger(poArgs, 0, &vid))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerAddByVIDPacket(vid);

	return Script_BuildNone();
}

ScriptObject* netSendMessengerAddByNamePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerAddByNamePacket(szName);

	return Script_BuildNone();
}

ScriptObject* netSendMessengerRemovePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szKey;
	if (!ScriptTuple_GetString(poArgs, 0, &szKey))
		return Script_BuildException();
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 1, &szName))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerRemovePacket(szKey, szName);

	return Script_BuildNone();
}

ScriptObject* netSendPartyInvitePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int vid;
	if (!ScriptTuple_GetInteger(poArgs, 0, &vid))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyInvitePacket(vid);

	return Script_BuildNone();
}

ScriptObject* netSendPartyInviteAnswerPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int vid;
	if (!ScriptTuple_GetInteger(poArgs, 0, &vid))
		return Script_BuildException();
	int answer;
	if (!ScriptTuple_GetInteger(poArgs, 1, &answer))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyInviteAnswerPacket(vid, answer);

	return Script_BuildNone();
}

ScriptObject* netSendPartyExitPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();

	DWORD dwVID = rPlayer.GetMainCharacterIndex();
	DWORD dwPID;
	if (rPlayer.PartyMemberVIDToPID(dwVID, &dwPID))
		rns.SendPartyRemovePacket(dwPID);

	return Script_BuildNone();
}

ScriptObject* netSendPartyRemovePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int vid;
	if (!ScriptTuple_GetInteger(poArgs, 0, &vid))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyRemovePacket(vid);

	return Script_BuildNone();
}

ScriptObject* netSendPartySetStatePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();
	int iState;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iState))
		return Script_BuildException();
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iFlag))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartySetStatePacket(iVID, iState, iFlag);

	return Script_BuildNone();
}

ScriptObject* netSendPartyUseSkillPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSkillIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSkillIndex))
		return Script_BuildException();
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iVID))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyUseSkillPacket(iSkillIndex, iVID);

	return Script_BuildNone();
}

ScriptObject* netSendPartyParameterPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyParameterPacket(iMode);

	return Script_BuildNone();
}

ScriptObject* netSendSafeboxSaveMoneyPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMoney;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMoney))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxMoneyPacket(SAFEBOX_MONEY_STATE_SAVE, iMoney);

	return Script_BuildNone();
}

ScriptObject* netSendSafeboxWithdrawMoneyPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMoney;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMoney))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxMoneyPacket(SAFEBOX_MONEY_STATE_WITHDRAW, iMoney);

	return Script_BuildNone();
}

ScriptObject* netSendSafeboxCheckinPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos InventoryPos;
	int iSafeBoxPos;

	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			InventoryPos.window_type = INVENTORY;
			if (!ScriptTuple_GetInteger(poArgs, 0, &InventoryPos.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &iSafeBoxPos))
				return Script_BuildException();
			break;
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &InventoryPos.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &InventoryPos.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &iSafeBoxPos))
				return Script_BuildException();
			break;
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxCheckinPacket(InventoryPos, iSafeBoxPos);

	return Script_BuildNone();
}

ScriptObject* netSendSafeboxCheckoutPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSafeBoxPos;
	TItemPos InventoryPos;

	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iSafeBoxPos))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &InventoryPos.cell))
				return Script_BuildException();
			break;
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iSafeBoxPos))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &InventoryPos.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &InventoryPos.cell))
				return Script_BuildException();
			break;
		default:
			break;
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxCheckoutPacket(iSafeBoxPos, InventoryPos);

	return Script_BuildNone();
}

ScriptObject* netSendSafeboxItemMovePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSourcePos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSourcePos))
		return Script_BuildException();
	int iTargetPos;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iTargetPos))
		return Script_BuildException();
	int iCount;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iCount))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxItemMovePacket(iSourcePos, iTargetPos, iCount);

	return Script_BuildNone();
}

ScriptObject* netSendMallCheckoutPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMallPos;
	TItemPos InventoryPos;

	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iMallPos))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &InventoryPos.cell))
				return Script_BuildException();
			break;
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iMallPos))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &InventoryPos.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &InventoryPos.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}
	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMallCheckoutPacket(iMallPos, InventoryPos);

	return Script_BuildNone();
}

ScriptObject* netSendAnswerMakeGuildPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendAnswerMakeGuildPacket(szName);

	return Script_BuildNone();
}

ScriptObject* netSendQuestInputStringPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szString;
	if (!ScriptTuple_GetString(poArgs, 0, &szString))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendQuestInputStringPacket(szString);

	return Script_BuildNone();
}

ScriptObject* netSendQuestConfirmPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iAnswer;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iAnswer))
		return Script_BuildException();
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPID))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendQuestConfirmPacket(iAnswer, iPID);

	return Script_BuildNone();
}

ScriptObject* netSendGuildAddMemberPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildAddMemberPacket(iVID);

	return Script_BuildNone();
}

ScriptObject* netSendGuildRemoveMemberPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szKey;
	if (!ScriptTuple_GetString(poArgs, 0, &szKey))
		return Script_BuildException();

	CPythonGuild::TGuildMemberData* pGuildMemberData;
	if (!CPythonGuild::Instance().GetMemberDataPtrByName(szKey, &pGuildMemberData))
	{
		TraceError("netSendGuildRemoveMemberPacket(szKey=%s) - Can't Find Guild Member\n", szKey);
		return Script_BuildNone();
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildRemoveMemberPacket(pGuildMemberData->dwPID);

	return Script_BuildNone();
}

ScriptObject* netSendGuildChangeGradeNamePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGradeNumber;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Script_BuildException();
	char* szGradeName;
	if (!ScriptTuple_GetString(poArgs, 1, &szGradeName))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeGradeNamePacket(iGradeNumber, szGradeName);

	return Script_BuildNone();
}

ScriptObject* netSendGuildChangeGradeAuthorityPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGradeNumber;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Script_BuildException();
	int iAuthority;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAuthority))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeGradeAuthorityPacket(iGradeNumber, iAuthority);

	return Script_BuildNone();
}

ScriptObject* netSendGuildOfferPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iExperience;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iExperience))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildOfferPacket(iExperience);

	return Script_BuildNone();
}

ScriptObject* netSnedGuildPostCommentPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szComment;
	if (!ScriptTuple_GetString(poArgs, 0, &szComment))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildPostCommentPacket(szComment);

	return Script_BuildNone();
}

ScriptObject* netSnedGuildDeleteCommentPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildDeleteCommentPacket(iIndex);

	return Script_BuildNone();
}

ScriptObject* netSendGuildRefreshCommentsPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iHightestIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iHightestIndex))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildRefreshCommentsPacket(iHightestIndex);

	return Script_BuildNone();
}

ScriptObject* netSendGuildChangeMemberGradePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPID))
		return Script_BuildException();
	int iGradeNumber;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iGradeNumber))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeMemberGradePacket(iPID, iGradeNumber);

	return Script_BuildNone();
}

ScriptObject* netSendGuildUseSkillPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSkillID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSkillID))
		return Script_BuildException();
	int iTargetVID;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iTargetVID))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildUseSkillPacket(iSkillID, iTargetVID);

	return Script_BuildNone();
}

ScriptObject* netSendGuildChangeMemberGeneralPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPID))
		return Script_BuildException();
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iFlag))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeMemberGeneralPacket(iPID, iFlag);

	return Script_BuildNone();
}

ScriptObject* netSendGuildInviteAnswerPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGuildID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGuildID))
		return Script_BuildException();
	int iAnswer;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAnswer))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildInviteAnswerPacket(iGuildID, iAnswer);

	return Script_BuildNone();
}

ScriptObject* netSendGuildChargeGSPPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGSP;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGSP))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChargeGSPPacket(iGSP);

	return Script_BuildNone();
}

ScriptObject* netSendGuildDepositMoneyPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGSP;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGSP))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildDepositMoneyPacket(iGSP);

	return Script_BuildNone();
}

ScriptObject* netSendGuildWithdrawMoneyPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGSP;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGSP))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildWithdrawMoneyPacket(iGSP);

	return Script_BuildNone();
}

ScriptObject* netSendRequestRefineInfoPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	//	CPythonNetworkStream& rns=CPythonNetworkStream::Instance();
	//	rns.SendRequestRefineInfoPacket(iSlotIndex);
	assert(!"netSendRequestRefineInfoPacket - \xB4\xF5\xC0\xCC\xBB\xF3 \xBB\xE7\xBF\xEB\xC7\xCF\xC1\xF6 \xBE\xCA\xB4\xC2 \xC7\xD4\xBC\xF6 \xC0\xD4\xB4\xCF\xB4\xD9");

	return Script_BuildNone();
}

ScriptObject* netSendRefinePacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iType))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendRefinePacket(iSlotIndex, iType);

	return Script_BuildNone();
}

ScriptObject* netSendSelectItemPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemPos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemPos))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSelectItemPacket(iItemPos);

	return Script_BuildNone();
}

ScriptObject* netSetPacketSequenceMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rns.SetPacketSequenceMode(true);
	rkAccountConnector.SetPacketSequenceMode(true);

	return Script_BuildNone();
}

ScriptObject* netSetEmpireLanguageMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BuildException();

	// CPythonNetworkStream& rns=CPythonNetworkStream::Instance();
	gs_bEmpireLanuageEnable = iMode;

	return Script_BuildNone();
}

ScriptObject* netSetSkillGroupFake(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.__TEST_SetSkillGroupFake(iIndex);

	return Script_BuildNone();
}

#include <Client/UI/GuildMarkUploader.h>
#include <Client/UI/GuildMarkDownloader.h>

ScriptObject* netSendGuildSymbol(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szIP;
	if (!ScriptTuple_GetString(poArgs, 0, &szIP))
		return Script_BuildException();
	int iPort;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPort))
		return Script_BuildException();
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 2, &szFileName))
		return Script_BuildException();
	int iGuildID;
	if (!ScriptTuple_GetInteger(poArgs, 3, &iGuildID))
		return Script_BuildException();

	CNetworkAddress kAddress;
	kAddress.Set(szIP, iPort);

	UINT uiError;

	CGuildMarkUploader& rkGuildMarkUploader = CGuildMarkUploader::Instance();
	if (!rkGuildMarkUploader.ConnectToSendSymbol(kAddress, 0, 0, iGuildID, szFileName, &uiError))
	{
		assert(!"Failed connecting to send symbol");
	}

	return Script_BuildNone();
}

ScriptObject* netDisconnectUploader(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CGuildMarkUploader& rkGuildMarkUploader = CGuildMarkUploader::Instance();
	rkGuildMarkUploader.Disconnect();
	return Script_BuildNone();
}

ScriptObject* netRecvGuildSymbol(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szIP;
	if (!ScriptTuple_GetString(poArgs, 0, &szIP))
		return Script_BuildException();
	int iPort;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPort))
		return Script_BuildException();
	int iGuildID;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iGuildID))
		return Script_BuildException();

	CNetworkAddress kAddress;
	kAddress.Set(szIP, iPort);

	std::vector<DWORD> kVec_dwGuildID;
	kVec_dwGuildID.clear();
	kVec_dwGuildID.push_back(iGuildID);

	CGuildMarkDownloader& rkGuildMarkDownloader = CGuildMarkDownloader::Instance();
	if (!rkGuildMarkDownloader.ConnectToRecvSymbol(kAddress, 0, 0, kVec_dwGuildID))
	{
		assert(!"Failed connecting to recv symbol");
	}

	return Script_BuildNone();
}

ScriptObject* netRegisterErrorLog(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szLog;
	if (!ScriptTuple_GetString(poArgs, 0, &szLog))
		return Script_BuildException();

	return Script_BuildNone();
}

void initnet()
{
	static ScriptMethodDef s_methods[] =
		{
			{"GetBettingGuildWarValue", netGetBettingGuildWarValue, SCRIPT_VARARGS, NULL},
			{"EnableChatInsultFilter", netEnableChatInsultFilter, SCRIPT_VARARGS, NULL},
			{"SetServerInfo", netSetServerInfo, SCRIPT_VARARGS, NULL},
			{"GetServerInfo", netGetServerInfo, SCRIPT_VARARGS, NULL},
			{"PreserveServerCommand", netPreserveServerCommand, SCRIPT_VARARGS, NULL},
			{"GetPreservedServerCommand", netGetPreservedServerCommand, SCRIPT_VARARGS, NULL},

			{"StartGame", netStartGame, SCRIPT_VARARGS, NULL},
			{"Warp", netWarp, SCRIPT_VARARGS, NULL},
			{"IsTest", netIsTest, SCRIPT_VARARGS, NULL},
			{"SetMarkServer", netSetMarkServer, SCRIPT_VARARGS, NULL},
			{"IsChatInsultIn", netIsChatInsultIn, SCRIPT_VARARGS, NULL},
			{"IsInsultIn", netIsInsultIn, SCRIPT_VARARGS, NULL},
			{"LoadInsultList", netLoadInsultList, SCRIPT_VARARGS, NULL},
			{"UploadMark", netUploadMark, SCRIPT_VARARGS, NULL},
			{"UploadSymbol", netUploadSymbol, SCRIPT_VARARGS, NULL},
			{"GetGuildID", netGetGuildID, SCRIPT_VARARGS, NULL},
			{"GetEmpireID", netGetEmpireID, SCRIPT_VARARGS, NULL},
			{"GetMainActorVID", netGetMainActorVID, SCRIPT_VARARGS, NULL},
			{"GetMainActorRace", netGetMainActorRace, SCRIPT_VARARGS, NULL},
			{"GetMainActorEmpire", netGetMainActorEmpire, SCRIPT_VARARGS, NULL},
			{"GetMainActorSkillGroup", netGetMainActorSkillGroup, SCRIPT_VARARGS, NULL},
			{"GetAccountCharacterSlotDataInteger", netGetAccountCharacterSlotDataInteger, SCRIPT_VARARGS, NULL},
			{"GetAccountCharacterSlotDataString", netGetAccountCharacterSlotDataString, SCRIPT_VARARGS, NULL},

			// FIELD_MUSIC
			{"GetFieldMusicFileName", netGetFieldMusicFileName, SCRIPT_VARARGS, NULL},
			{"GetFieldMusicVolume", netGetFieldMusicVolume, SCRIPT_VARARGS, NULL},
			// END_OF_FIELD_MUSIC

			{"ToggleGameDebugInfo", netToggleGameDebugInfo, SCRIPT_VARARGS, NULL},
			{"SetLoginInfo", netSetLoginInfo, SCRIPT_VARARGS, NULL},
			{"SetPhaseWindow", netSetPhaseWindow, SCRIPT_VARARGS, NULL},
			{"ClearPhaseWindow", netClearPhaseWindow, SCRIPT_VARARGS, NULL},
			{"SetServerCommandParserWindow", netSetServerCommandParserWindow, SCRIPT_VARARGS, NULL},
			{"SetAccountConnectorHandler", netSetAccountConnectorHandler, SCRIPT_VARARGS, NULL},
			{"SetHandler", netSetHandler, SCRIPT_VARARGS, NULL},
			{"SetTCPRecvBufferSize", netSetTCPRecvBufferSize, SCRIPT_VARARGS, NULL},
			{"SetTCPSendBufferSize", netSetTCPSendBufferSize, SCRIPT_VARARGS, NULL},
			{"DirectEnter", netDirectEnter, SCRIPT_VARARGS, NULL},

			{"LogOutGame", netLogOutGame, SCRIPT_VARARGS, NULL},
			{"ExitGame", netExitGame, SCRIPT_VARARGS, NULL},
			{"ExitApplication", netExitApplication, SCRIPT_VARARGS, NULL},
			{"ConnectTCP", netConnectTCP, SCRIPT_VARARGS, NULL},
			{"ConnectToAccountServer", netConnectToAccountServer, SCRIPT_VARARGS, NULL},

			{"SendLoginPacket", netSendLoginPacket, SCRIPT_VARARGS, NULL},
			{"SendChinaMatrixCardPacket", netSendChinaMatrixCardPacket, SCRIPT_VARARGS, NULL},
			{"SendRunupMatrixCardPacket", netSendRunupMatrixCardPacket, SCRIPT_VARARGS, NULL},
			{"SendNEWCIBNPasspodAnswerPacket", netSendNEWCIBNPasspodAnswerPacket, SCRIPT_VARARGS, NULL},
			{"SendSelectEmpirePacket", netSendSelectEmpirePacket, SCRIPT_VARARGS, NULL},
			{"SendSelectCharacterPacket", netSendSelectCharacterPacket, SCRIPT_VARARGS, NULL},
			{"SendChangeNamePacket", netSendChangeNamePacket, SCRIPT_VARARGS, NULL},
			{"SendCreateCharacterPacket", netSendCreateCharacterPacket, SCRIPT_VARARGS, NULL},
			{"SendDestroyCharacterPacket", netSendDestroyCharacterPacket, SCRIPT_VARARGS, NULL},
			{"SendEnterGamePacket", netSendEnterGamePacket, SCRIPT_VARARGS, NULL},

			{"SendItemUsePacket", netSendItemUsePacket, SCRIPT_VARARGS, NULL},
			{"SendItemUseToItemPacket", netSendItemUseToItemPacket, SCRIPT_VARARGS, NULL},
			{"SendItemDropPacket", netSendItemDropPacket, SCRIPT_VARARGS, NULL},
			{"SendItemDropPacketNew", netSendItemDropPacketNew, SCRIPT_VARARGS, NULL},
			{"SendElkDropPacket", netSendElkDropPacket, SCRIPT_VARARGS, NULL},
			{"SendGoldDropPacketNew", netSendGoldDropPacketNew, SCRIPT_VARARGS, NULL},
			{"SendItemMovePacket", netSendItemMovePacket, SCRIPT_VARARGS, NULL},
			{"SendItemPickUpPacket", netSendItemPickUpPacket, SCRIPT_VARARGS, NULL},
			{"SendGiveItemPacket", netSendGiveItemPacket, SCRIPT_VARARGS, NULL},

			{"SetOfflinePhase", netSetOfflinePhase, SCRIPT_VARARGS, NULL},
			{"Disconnect", netDisconnect, SCRIPT_VARARGS, NULL},
			{"IsConnect", netIsConnect, SCRIPT_VARARGS, NULL},

			{"SendChatPacket", netSendChatPacket, SCRIPT_VARARGS, NULL},
			{"SendEmoticon", netSendEmoticon, SCRIPT_VARARGS, NULL},
			{"SendWhisperPacket", netSendWhisperPacket, SCRIPT_VARARGS, NULL},

			{"SendCharacterPositionPacket", netSendCharacterPositionPacket, SCRIPT_VARARGS, NULL},

			{"SendShopEndPacket", netSendShopEndPacket, SCRIPT_VARARGS, NULL},
			{"SendShopBuyPacket", netSendShopBuyPacket, SCRIPT_VARARGS, NULL},
			{"SendShopSellPacket", netSendShopSellPacket, SCRIPT_VARARGS, NULL},
			{"SendShopSellPacketNew", netSendShopSellPacketNew, SCRIPT_VARARGS, NULL},

			{"SendExchangeStartPacket", netSendExchangeStartPacket, SCRIPT_VARARGS, NULL},
			{"SendExchangeItemAddPacket", netSendExchangeItemAddPacket, SCRIPT_VARARGS, NULL},
			{"SendExchangeItemDelPacket", netSendExchangeItemDelPacket, SCRIPT_VARARGS, NULL},
			{"SendExchangeElkAddPacket", netSendExchangeElkAddPacket, SCRIPT_VARARGS, NULL},
			{"SendExchangeAcceptPacket", netSendExchangeAcceptPacket, SCRIPT_VARARGS, NULL},
			{"SendExchangeExitPacket", netSendExchangeExitPacket, SCRIPT_VARARGS, NULL},

			{"SendOnClickPacket", netOnClickPacket, SCRIPT_VARARGS, NULL},

			// Emoticon String
			{"RegisterEmoticonString", netRegisterEmoticonString, SCRIPT_VARARGS, NULL},

			// Messenger
			{"SendMessengerAddByVIDPacket", netSendMessengerAddByVIDPacket, SCRIPT_VARARGS, NULL},
			{"SendMessengerAddByNamePacket", netSendMessengerAddByNamePacket, SCRIPT_VARARGS, NULL},
			{"SendMessengerRemovePacket", netSendMessengerRemovePacket, SCRIPT_VARARGS, NULL},

			// Party
			{"SendPartyInvitePacket", netSendPartyInvitePacket, SCRIPT_VARARGS, NULL},
			{"SendPartyInviteAnswerPacket", netSendPartyInviteAnswerPacket, SCRIPT_VARARGS, NULL},
			{"SendPartyExitPacket", netSendPartyExitPacket, SCRIPT_VARARGS, NULL},
			{"SendPartyRemovePacket", netSendPartyRemovePacket, SCRIPT_VARARGS, NULL},
			{"SendPartySetStatePacket", netSendPartySetStatePacket, SCRIPT_VARARGS, NULL},
			{"SendPartyUseSkillPacket", netSendPartyUseSkillPacket, SCRIPT_VARARGS, NULL},
			{"SendPartyParameterPacket", netSendPartyParameterPacket, SCRIPT_VARARGS, NULL},

			// Safebox
			{"SendSafeboxSaveMoneyPacket", netSendSafeboxSaveMoneyPacket, SCRIPT_VARARGS, NULL},
			{"SendSafeboxWithdrawMoneyPacket", netSendSafeboxWithdrawMoneyPacket, SCRIPT_VARARGS, NULL},
			{"SendSafeboxCheckinPacket", netSendSafeboxCheckinPacket, SCRIPT_VARARGS, NULL},
			{"SendSafeboxCheckoutPacket", netSendSafeboxCheckoutPacket, SCRIPT_VARARGS, NULL},
			{"SendSafeboxItemMovePacket", netSendSafeboxItemMovePacket, SCRIPT_VARARGS, NULL},

			// Mall
			{"SendMallCheckoutPacket", netSendMallCheckoutPacket, SCRIPT_VARARGS, NULL},

			// Guild
			{"SendAnswerMakeGuildPacket", netSendAnswerMakeGuildPacket, SCRIPT_VARARGS, NULL},
			{"SendQuestInputStringPacket", netSendQuestInputStringPacket, SCRIPT_VARARGS, NULL},
			{"SendQuestConfirmPacket", netSendQuestConfirmPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildAddMemberPacket", netSendGuildAddMemberPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildRemoveMemberPacket", netSendGuildRemoveMemberPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildChangeGradeNamePacket", netSendGuildChangeGradeNamePacket, SCRIPT_VARARGS, NULL},
			{"SendGuildChangeGradeAuthorityPacket", netSendGuildChangeGradeAuthorityPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildOfferPacket", netSendGuildOfferPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildPostCommentPacket", netSnedGuildPostCommentPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildDeleteCommentPacket", netSnedGuildDeleteCommentPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildRefreshCommentsPacket", netSendGuildRefreshCommentsPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildChangeMemberGradePacket", netSendGuildChangeMemberGradePacket, SCRIPT_VARARGS, NULL},
			{"SendGuildUseSkillPacket", netSendGuildUseSkillPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildChangeMemberGeneralPacket", netSendGuildChangeMemberGeneralPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildInviteAnswerPacket", netSendGuildInviteAnswerPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildChargeGSPPacket", netSendGuildChargeGSPPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildDepositMoneyPacket", netSendGuildDepositMoneyPacket, SCRIPT_VARARGS, NULL},
			{"SendGuildWithdrawMoneyPacket", netSendGuildWithdrawMoneyPacket, SCRIPT_VARARGS, NULL},

			// Refine
			{"SendRequestRefineInfoPacket", netSendRequestRefineInfoPacket, SCRIPT_VARARGS, NULL},
			{"SendRefinePacket", netSendRefinePacket, SCRIPT_VARARGS, NULL},
			{"SendSelectItemPacket", netSendSelectItemPacket, SCRIPT_VARARGS, NULL},

			// SYSTEM
			{"SetPacketSequenceMode", netSetPacketSequenceMode, SCRIPT_VARARGS, NULL},
			{"SetEmpireLanguageMode", netSetEmpireLanguageMode, SCRIPT_VARARGS, NULL},

			// For Test
			{"SetSkillGroupFake", netSetSkillGroupFake, SCRIPT_VARARGS, NULL},

			// Guild Symbol
			{"SendGuildSymbol", netSendGuildSymbol, SCRIPT_VARARGS, NULL},
			{"DisconnectUploader", netDisconnectUploader, SCRIPT_VARARGS, NULL},
			{"RecvGuildSymbol", netRecvGuildSymbol, SCRIPT_VARARGS, NULL},

			// Log
			{"RegisterErrorLog", netRegisterErrorLog, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("net", s_methods);

	ScriptModule_AddIntConstant(poModule, "ERROR_NONE", CPythonNetworkStream::ERROR_NONE);
	ScriptModule_AddIntConstant(poModule, "ERROR_CONNECT_MARK_SERVER", CPythonNetworkStream::ERROR_CONNECT_MARK_SERVER);
	ScriptModule_AddIntConstant(poModule, "ERROR_LOAD_MARK", CPythonNetworkStream::ERROR_LOAD_MARK);
	ScriptModule_AddIntConstant(poModule, "ERROR_MARK_WIDTH", CPythonNetworkStream::ERROR_MARK_WIDTH);
	ScriptModule_AddIntConstant(poModule, "ERROR_MARK_HEIGHT", CPythonNetworkStream::ERROR_MARK_HEIGHT);

	// MARK_BUG_FIX
	ScriptModule_AddIntConstant(poModule, "ERROR_MARK_UPLOAD_NEED_RECONNECT", CPythonNetworkStream::ERROR_MARK_UPLOAD_NEED_RECONNECT);
	ScriptModule_AddIntConstant(poModule, "ERROR_MARK_CHECK_NEED_RECONNECT", CPythonNetworkStream::ERROR_MARK_CHECK_NEED_RECONNECT);
	// END_OF_MARK_BUG_FIX

	ScriptModule_AddIntConstant(poModule, "PHASE_WINDOW_LOGIN", CPythonNetworkStream::PHASE_WINDOW_LOGIN);
	ScriptModule_AddIntConstant(poModule, "PHASE_WINDOW_SELECT", CPythonNetworkStream::PHASE_WINDOW_SELECT);
	ScriptModule_AddIntConstant(poModule, "PHASE_WINDOW_CREATE", CPythonNetworkStream::PHASE_WINDOW_CREATE);
	ScriptModule_AddIntConstant(poModule, "PHASE_WINDOW_LOAD", CPythonNetworkStream::PHASE_WINDOW_LOAD);
	ScriptModule_AddIntConstant(poModule, "PHASE_WINDOW_GAME", CPythonNetworkStream::PHASE_WINDOW_GAME);
	ScriptModule_AddIntConstant(poModule, "PHASE_WINDOW_EMPIRE", CPythonNetworkStream::PHASE_WINDOW_EMPIRE);
	ScriptModule_AddIntConstant(poModule, "PHASE_WINDOW_LOGO", CPythonNetworkStream::PHASE_WINDOW_LOGO);

	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_ID", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_ID);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_NAME", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_NAME);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_RACE", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_RACE);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_LEVEL", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_LEVEL);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_STR", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_STR);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_DEX", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_DEX);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_INT", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_INT);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_HTH", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_HTH);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_PLAYTIME", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_PLAYTIME);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_FORM", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_FORM);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_ADDR", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_ADDR);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_PORT", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_PORT);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_GUILD_ID", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_GUILD_ID);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_GUILD_NAME", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_GUILD_NAME);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG);
	ScriptModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_HAIR", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_HAIR);

	ScriptModule_AddIntConstant(poModule, "SERVER_COMMAND_LOG_OUT", CPythonNetworkStream::SERVER_COMMAND_LOG_OUT);
	ScriptModule_AddIntConstant(poModule, "SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER", CPythonNetworkStream::SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER);
	ScriptModule_AddIntConstant(poModule, "SERVER_COMMAND_QUIT", CPythonNetworkStream::SERVER_COMMAND_QUIT);

	ScriptModule_AddIntConstant(poModule, "EMPIRE_A", 1);
	ScriptModule_AddIntConstant(poModule, "EMPIRE_B", 2);
	ScriptModule_AddIntConstant(poModule, "EMPIRE_C", 3);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL", DS_SUB_HEADER_REFINE_FAIL);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE", DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL", DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY", DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL", DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL", DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_SUCCEED", DS_SUB_HEADER_REFINE_SUCCEED);
}
