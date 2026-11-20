#include <Client/UI/StdAfx.h>
#include <Client/Session/PythonNetworkStream.h>
#include <Client/UI/Packet.h>

extern uint32_t g_adwEncryptKey[4];
extern uint32_t g_adwDecryptKey[4];

// Select Character ---------------------------------------------------------------------------
void CPythonNetworkStream::SetSelectPhase()
{
	if ("Select" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Select Phase ##");
	Tracen("");

	m_strPhase = "Select";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::SelectPhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveSelectPhase);

	if (__DirectEnterMode_IsSet())
	{
		PyCallClassMemberFunc(m_poHandler, "SetLoadingPhase", Py_BuildValue("()"));
	}
	else
	{
		if (IsSelectedEmpire())
			PyCallClassMemberFunc(m_poHandler, "SetSelectCharacterPhase", Py_BuildValue("()"));
		else
			PyCallClassMemberFunc(m_poHandler, "SetSelectEmpirePhase", Py_BuildValue("()"));
	}
}

void CPythonNetworkStream::SelectPhase()
{
	TPacketHeader header;

	if (!CheckPacket(&header))
		return;

	if (protocol::Has(header))
	{
		if (DecodeProtocolPacket(header))
			return;

		RecvErrorPacket(header);
		return;
	}

	if (HEADER_GC_HANDSHAKE == header)
	{
		RecvHandshakePacket();
		return;
	}

	RecvErrorPacket(header);
}

bool CPythonNetworkStream::SendSelectEmpirePacket(uint32_t dwEmpireID)
{
	TPacketCGEmpire kPacketEmpire;
	kPacketEmpire.bHeader = HEADER_CG_EMPIRE;
	kPacketEmpire.bEmpire = dwEmpireID;

	if (!Send(sizeof(kPacketEmpire), &kPacketEmpire))
	{
		Tracen("SendSelectEmpirePacket - Error");
		return false;
	}

	SetEmpireID(dwEmpireID);
	return SendSequence();
}

bool CPythonNetworkStream::SendSelectCharacterPacket(uint8_t Index)
{
	TPacketCGSelectCharacter SelectCharacterPacket;

	SelectCharacterPacket.header = HEADER_CG_PLAYER_SELECT;
	SelectCharacterPacket.player_index = Index;

	if (!Send(sizeof(TPacketCGSelectCharacter), &SelectCharacterPacket))
	{
		Tracen("SendSelectCharacterPacket - Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendDestroyCharacterPacket(uint8_t index, const char* szPrivateCode)
{
	TPacketCGDestroyCharacter DestroyCharacterPacket;

	DestroyCharacterPacket.header = HEADER_CG_PLAYER_DESTROY;
	DestroyCharacterPacket.index = index;
	strncpy(DestroyCharacterPacket.szPrivateCode, szPrivateCode, PRIVATE_CODE_LENGTH - 1);

	if (!Send(sizeof(TPacketCGDestroyCharacter), &DestroyCharacterPacket))
	{
		Tracen("SendDestroyCharacterPacket");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendCreateCharacterPacket(uint8_t index, const char* name, uint8_t job, uint8_t shape, uint8_t byCON, uint8_t byINT, uint8_t bySTR, uint8_t byDEX)
{
	TPacketCGCreateCharacter createCharacterPacket;

	createCharacterPacket.header = HEADER_CG_PLAYER_CREATE;
	createCharacterPacket.index = index;
	strncpy(createCharacterPacket.name, name, CHARACTER_NAME_MAX_LEN);
	createCharacterPacket.job = job;
	createCharacterPacket.shape = shape;
	createCharacterPacket.CON = byCON;
	createCharacterPacket.int32_t = byINT;
	createCharacterPacket.STR = bySTR;
	createCharacterPacket.DEX = byDEX;

	if (!Send(sizeof(TPacketCGCreateCharacter), &createCharacterPacket))
	{
		Tracen("Failed to SendCreateCharacterPacket");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendChangeNamePacket(uint8_t index, const char* name)
{
	TPacketCGChangeName ChangeNamePacket;
	ChangeNamePacket.header = HEADER_CG_CHANGE_NAME;
	ChangeNamePacket.index = index;
	strncpy(ChangeNamePacket.name, name, CHARACTER_NAME_MAX_LEN);

	if (!Send(sizeof(TPacketCGChangeName), &ChangeNamePacket))
	{
		Tracen("Failed to SendChangeNamePacket");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::__RecvPlayerCreateSuccessPacket(const protocol::Packet& value)
{
	const TPacketGCPlayerCreateSuccess kCreateSuccessPacket = value.Get<TPacketGCPlayerCreateSuccess>();

	if (kCreateSuccessPacket.bAccountCharacterSlot >= PLAYER_PER_ACCOUNT4)
	{
		TraceError("CPythonNetworkStream::RecvPlayerCreateSuccessPacket - OUT OF RANGE SLOT(%d) > PLATER_PER_ACCOUNT(%d)",
				   kCreateSuccessPacket.bAccountCharacterSlot, PLAYER_PER_ACCOUNT4);
		return true;
	}

	m_akSimplePlayerInfo[kCreateSuccessPacket.bAccountCharacterSlot] = kCreateSuccessPacket.kSimplePlayerInfomation;
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_CREATE], "OnCreateSuccess", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::__RecvPlayerCreateFailurePacket(const protocol::Packet& value)
{
	const TPacketGCCreateFailure packet = value.Get<TPacketGCCreateFailure>();

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_CREATE], "OnCreateFailure", Py_BuildValue("(i)", packet.bType));
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_SELECT], "OnCreateFailure", Py_BuildValue("(i)", packet.bType));
	return true;
}

bool CPythonNetworkStream::__RecvPlayerDestroySuccessPacket(const protocol::Packet& value)
{
	const TPacketGCDestroyCharacterSuccess packet = value.Get<TPacketGCDestroyCharacterSuccess>();

	memset(&m_akSimplePlayerInfo[packet.account_index], 0, sizeof(m_akSimplePlayerInfo[packet.account_index]));
	m_adwGuildID[packet.account_index] = 0;
	m_astrGuildName[packet.account_index] = "";

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_SELECT], "OnDeleteSuccess", Py_BuildValue("(i)", packet.account_index));
	return true;
}

bool CPythonNetworkStream::__RecvPlayerDestroyFailurePacket(const protocol::Packet& value)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_SELECT], "OnDeleteFailure", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::__RecvChangeName(const protocol::Packet& value)
{
	const TPacketGCChangeName ChangeNamePacket = value.Get<TPacketGCChangeName>();

	for (int i = 0; i < PLAYER_PER_ACCOUNT4; ++i)
	{
		if (ChangeNamePacket.pid == m_akSimplePlayerInfo[i].dwID)
		{
			m_akSimplePlayerInfo[i].bChangeName = FALSE;
			snprintf(m_akSimplePlayerInfo[i].szName, sizeof(m_akSimplePlayerInfo[i].szName), "%s", ChangeNamePacket.name);

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_SELECT], "OnChangeName", Py_BuildValue("(is)", i, ChangeNamePacket.name));
			return true;
		}
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_SELECT], "OnCreateFailure", Py_BuildValue("(i)", 100));
	return true;
}
