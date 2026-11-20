#include <Client/UI/StdAfx.h>
#include <Client/Session/PythonNetworkStream.h>
#include <Client/UI/Packet.h>
#include <Client/UI/PythonApplication.h>
#include <Client/UI/NetworkActorManager.h>

#include <Client/UI/AbstractPlayer.h>

#include <Neoeter/IO/EterPackManager.h>

void CPythonNetworkStream::EnableChatInsultFilter(bool isEnable)
{
	m_isEnableChatInsultFilter = isEnable;
}

void CPythonNetworkStream::__FilterInsult(char* szLine, uint32_t uLineLen)
{
	m_kInsultChecker.FilterInsult(szLine, uLineLen);
}

bool CPythonNetworkStream::IsChatInsultIn(const char* c_szMsg)
{
	if (m_isEnableChatInsultFilter)
		return false;

	return IsInsultIn(c_szMsg);
}

bool CPythonNetworkStream::IsInsultIn(const char* c_szMsg)
{
	return m_kInsultChecker.IsInsultIn(c_szMsg, strlen(c_szMsg));
}

bool CPythonNetworkStream::LoadInsultList(const char* c_szInsultListFileName)
{
	CEterPackManager::TPackDataPtr data;
	CMemoryTextFileLoader kMemTextFileLoader;
	if (!CEterPackManager::Instance().Get(c_szInsultListFileName, data))
		return false;

	kMemTextFileLoader.Bind(data->size(), data->data());

	m_kInsultChecker.Clear();
	for (uint32_t dwLineIndex = 0; dwLineIndex < kMemTextFileLoader.GetLineCount(); ++dwLineIndex)
	{
		const std::string& c_rstLine = kMemTextFileLoader.GetLineString(dwLineIndex);
		m_kInsultChecker.AppendInsult(c_rstLine);
	}
	return true;
}

bool CPythonNetworkStream::LoadConvertTable(uint32_t dwEmpireID, const char* c_szFileName)
{
	if (dwEmpireID < 1 || dwEmpireID >= 4)
		return false;

	std::stringstream data;
	if (!CEterPackManager::Instance().Get(c_szFileName, data))
		return false;

	uint32_t dwEngCount = 26;
	uint32_t dwHanCount = (0xc8 - 0xb0 + 1) * (0xfe - 0xa1 + 1);
	uint32_t dwHanSize = dwHanCount * 2;
	uint32_t dwFileSize = dwEngCount * 2 + dwHanSize;

	if (data.str().length() < dwFileSize)
		return false;

	STextConvertTable& rkTextConvTable = m_aTextConvTable[dwEmpireID - 1];
	data.read((char*)rkTextConvTable.acUpper, dwEngCount);
	data.read((char*)rkTextConvTable.acLower, dwEngCount);
	data.read((char*)rkTextConvTable.aacHan, dwHanSize);

	if (!data)
		return false;

	return true;
}

// Loading ---------------------------------------------------------------------------
void CPythonNetworkStream::LoadingPhase()
{
	TPacketHeader header;

	if (!CheckPacket(&header))
		return;

	if (protocol::Has(header))
	{
		if (!DecodeProtocolPacket(header))
			RecvErrorPacket(header);

		return;
	}

	switch (header)
	{
		default:
			GamePhase();
			return;
			break;
	}

	RecvErrorPacket(header);
}

void CPythonNetworkStream::SetLoadingPhase()
{
	if ("Loading" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Loading Phase ##");
	Tracen("");

	m_strPhase = "Loading";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::LoadingPhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveLoadingPhase);

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.Clear();

	CFlyingManager::Instance().DeleteAllInstances();
	CEffectManager::Instance().DeleteAllInstances();

	__DirectEnterMode_Initialize();
}

bool CPythonNetworkStream::RecvMainCharacter(const protocol::Packet& value)
{
	TPacketGCMainCharacter MainChrPacket = value.Get<TPacketGCMainCharacter>();

	m_dwMainActorRace = MainChrPacket.wRaceNum;
	m_dwMainActorEmpire = 0;
	m_dwMainActorSkillGroup = MainChrPacket.bySkillGroup;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(MainChrPacket.szName);
	rkPlayer.SetMainCharacterIndex(MainChrPacket.dwVID);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", MainChrPacket.lX, MainChrPacket.lY));

	// Tracef(" >> RecvMainCharacter\n");

	SendClientVersionPacket();
	return true;
}

// SUPPORT_BGM
bool CPythonNetworkStream::RecvMainCharacter2_EMPIRE(const protocol::Packet& value)
{
	TPacketGCMainCharacter2_EMPIRE mainChrPacket = value.Get<TPacketGCMainCharacter2_EMPIRE>();

	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szName);
	rkPlayer.SetMainCharacterIndex(mainChrPacket.dwVID);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	// Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	SendClientVersionPacket();
	return true;
}

bool CPythonNetworkStream::RecvMainCharacter3_BGM(const protocol::Packet& value)
{
	TPacketGCMainCharacter3_BGM mainChrPacket = value.Get<TPacketGCMainCharacter3_BGM>();

	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szUserName);
	rkPlayer.SetMainCharacterIndex(mainChrPacket.dwVID);

	__SetFieldMusicFileName(mainChrPacket.szBGMName);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	// Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	SendClientVersionPacket();
	return true;
}

bool CPythonNetworkStream::RecvMainCharacter4_BGM_VOL(const protocol::Packet& value)
{
	TPacketGCMainCharacter4_BGM_VOL mainChrPacket = value.Get<TPacketGCMainCharacter4_BGM_VOL>();

	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szUserName);
	rkPlayer.SetMainCharacterIndex(mainChrPacket.dwVID);

	__SetFieldMusicFileInfo(mainChrPacket.szBGMName, mainChrPacket.fBGMVol);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	// Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	SendClientVersionPacket();
	return true;
}

static std::string gs_fieldMusic_fileName;
static float gs_fieldMusic_volume = 1.0f / 5.0f * 0.1f;

void CPythonNetworkStream::__SetFieldMusicFileName(const char* musicName)
{
	gs_fieldMusic_fileName = musicName;
}

void CPythonNetworkStream::__SetFieldMusicFileInfo(const char* musicName, float vol)
{
	gs_fieldMusic_fileName = musicName;
	gs_fieldMusic_volume = vol;
}

const char* CPythonNetworkStream::GetFieldMusicFileName()
{
	return gs_fieldMusic_fileName.c_str();
}

float CPythonNetworkStream::GetFieldMusicVolume()
{
	return gs_fieldMusic_volume;
}
// END_OF_SUPPORT_BGM

bool CPythonNetworkStream::__RecvPlayerPoints(const protocol::Packet& value)
{
	TPacketGCPoints PointsPacket = value.Get<TPacketGCPoints>();

	for (uint32_t i = 0; i < POINT_MAX_NUM; ++i)
		CPythonPlayer::Instance().SetStatus(i, PointsPacket.points[i]);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshStatus", Py_BuildValue("()"));
	return true;
}

void CPythonNetworkStream::StartGame()
{
	m_isStartGame = TRUE;
}

bool CPythonNetworkStream::SendEnterGame()
{
	TPacketCGEnterFrontGame EnterFrontGamePacket;

	EnterFrontGamePacket.header = HEADER_CG_ENTERGAME;

	if (!Send(sizeof(EnterFrontGamePacket), &EnterFrontGamePacket))
	{
		Tracen("Send EnterFrontGamePacket");
		return false;
	}

	if (!SendSequence())
		return false;

	__SendInternalBuffer();
	return true;
}
