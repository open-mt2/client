#include <Client/UI/StdAfx.h>
#include <Client/Python/Network/PythonNetworkStream.h>
#include <Client/Session/NetworkStreamPhases.h>
#include <Client/Game/Packet.h>
#include <Client/Utils/Test.h>
#include <Client/Session/AccountConnector.h>

// Login ---------------------------------------------------------------------------
void CPythonNetworkStream::LoginPhase()
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

	switch (header)
	{
		default:
			if (RecvDefaultPacket(header))
				return;
			break;
	}

	RecvErrorPacket(header);
}

void CPythonNetworkStream::SetLoginPhase()
{
	if ("Login" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Login Phase ##");
	Tracen("");

	m_strPhase = "Login";

	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::LoginPhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveLoginPhase);

	m_dwChangingPhaseTime = ELTimer_GetMSec();

	if (__DirectEnterMode_IsSet())
	{
		if (0 != m_dwLoginKey)
			SendLoginPacketNew(m_stID.c_str(), m_stPassword.c_str());
		else
			SendLoginPacket(m_stID.c_str(), m_stPassword.c_str());

		// 비밀번호를 메모리에 계속 갖고 있는 문제가 있어서, 사용 즉시 날리는 것으로 변경
		ClearLoginInfo();
		CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
		rkAccountConnector.ClearLoginInfo();
	}
	else
	{
		if (0 != m_dwLoginKey)
			SendLoginPacketNew(m_stID.c_str(), m_stPassword.c_str());
		else
			SendLoginPacket(m_stID.c_str(), m_stPassword.c_str());

		// 비밀번호를 메모리에 계속 갖고 있는 문제가 있어서, 사용 즉시 날리는 것으로 변경
		ClearLoginInfo();
		CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
		rkAccountConnector.ClearLoginInfo();

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOGIN], "OnLoginStart", Py_BuildValue("()"));

		__ClearSelectCharacterData();
	}
}

bool CNetworkStreamLoginPhase::__RecvEmpirePacket(const protocol::Packet& value)
{
	TPacketGCEmpire kPacketEmpire = value.Get<TPacketGCEmpire>();

	m_rkStream.m_dwEmpireID = kPacketEmpire.bEmpire;
	return true;
}

bool CNetworkStreamLoginPhase::__RecvLoginSuccessPacket3(const protocol::Packet& value)
{
	TPacketGCLoginSuccess3 kPacketLoginSuccess = value.Get<TPacketGCLoginSuccess3>();

	for (int i = 0; i < PLAYER_PER_ACCOUNT3; ++i)
	{
		m_rkStream.m_akSimplePlayerInfo[i] = kPacketLoginSuccess.akSimplePlayerInformation[i];
		m_rkStream.m_adwGuildID[i] = kPacketLoginSuccess.guild_id[i];
		m_rkStream.m_astrGuildName[i] = kPacketLoginSuccess.guild_name[i];
	}

	m_rkStream.m_kMarkAuth.m_dwHandle = kPacketLoginSuccess.handle;
	m_rkStream.m_kMarkAuth.m_dwRandomKey = kPacketLoginSuccess.random_key;

	if (m_rkStream.__DirectEnterMode_IsSet())
	{
	}
	else
	{
		PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_SELECT], "Refresh", Py_BuildValue("()"));
	}

	return true;
}

bool CNetworkStreamLoginPhase::__RecvLoginSuccessPacket4(const protocol::Packet& value)
{
	TPacketGCLoginSuccess4 kPacketLoginSuccess = value.Get<TPacketGCLoginSuccess4>();

	for (int i = 0; i < PLAYER_PER_ACCOUNT4; ++i)
	{
		m_rkStream.m_akSimplePlayerInfo[i] = kPacketLoginSuccess.akSimplePlayerInformation[i];
		m_rkStream.m_adwGuildID[i] = kPacketLoginSuccess.guild_id[i];
		m_rkStream.m_astrGuildName[i] = kPacketLoginSuccess.guild_name[i];
	}

	m_rkStream.m_kMarkAuth.m_dwHandle = kPacketLoginSuccess.handle;
	m_rkStream.m_kMarkAuth.m_dwRandomKey = kPacketLoginSuccess.random_key;

	if (m_rkStream.__DirectEnterMode_IsSet())
	{
	}
	else
	{
		PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_SELECT], "Refresh", Py_BuildValue("()"));
	}

	return true;
}

void CPythonNetworkStream::OnConnectFailure()
{
	if (__DirectEnterMode_IsSet())
	{
		ClosePhase();
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOGIN], "OnConnectFailure", Py_BuildValue("()"));
	}
}

bool CNetworkStreamLoginPhase::__RecvLoginFailurePacket(const protocol::Packet& value)
{
	TPacketGCLoginFailure packet_failure = value.Get<TPacketGCLoginFailure>();

	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_LOGIN], "OnLoginFailure", Py_BuildValue("(s)", packet_failure.szStatus));
#ifdef _DEBUG
	Tracef(" RecvLoginFailurePacket : [%s]\n", packet_failure.szStatus);
#endif
	return true;
}

bool CPythonNetworkStream::SendDirectEnterPacket(const char* c_szID, const char* c_szPassword, uint32_t uChrSlot)
{
	TPacketCGDirectEnter kPacketDirectEnter;
	kPacketDirectEnter.bHeader = HEADER_CG_DIRECT_ENTER;
	kPacketDirectEnter.index = uChrSlot;
	strncpy(kPacketDirectEnter.login, c_szID, ID_MAX_NUM);
	strncpy(kPacketDirectEnter.passwd, c_szPassword, PASS_MAX_NUM);

	if (!Send(sizeof(kPacketDirectEnter), &kPacketDirectEnter))
	{
		Tracen("SendDirectEnter");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendLoginPacket(const char* c_szName, const char* c_szPassword)
{
	TPacketCGLogin LoginPacket;
	LoginPacket.header = HEADER_CG_LOGIN;

	strncpy(LoginPacket.name, c_szName, sizeof(LoginPacket.name) - 1);
	strncpy(LoginPacket.pwd, c_szPassword, sizeof(LoginPacket.pwd) - 1);

	LoginPacket.name[ID_MAX_NUM] = '\0';
	LoginPacket.pwd[PASS_MAX_NUM] = '\0';

	if (!Send(sizeof(LoginPacket), &LoginPacket))
	{
		Tracen("SendLogin Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendLoginPacketNew(const char* c_szName, const char* c_szPassword)
{
	TPacketCGLogin2 LoginPacket;
	LoginPacket.header = HEADER_CG_LOGIN2;
	LoginPacket.login_key = m_dwLoginKey;

	strncpy(LoginPacket.name, c_szName, sizeof(LoginPacket.name) - 1);
	LoginPacket.name[ID_MAX_NUM] = '\0';

	extern uint32_t g_adwEncryptKey[4];
	for (uint32_t i = 0; i < 4; ++i)
		LoginPacket.adwClientKey[i] = g_adwEncryptKey[i];

	if (!Send(sizeof(LoginPacket), &LoginPacket))
	{
		Tracen("SendLogin Error");
		return false;
	}

	if (!SendSequence())
	{
		Tracen("SendLogin Error");
		return false;
	}

	__SendInternalBuffer();

	return true;
}

bool CNetworkStreamLoginPhase::__RecvRunupMatrixQuizPacket(const protocol::Packet& value)
{
	TPacketGCRunupMatrixQuiz kMatrixQuizPacket = value.Get<TPacketGCRunupMatrixQuiz>();

	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_LOGIN], "BINARY_OnRunupMatrixQuiz", Py_BuildValue("(s)", kMatrixQuizPacket.szQuiz));
	return true;
}

bool CPythonNetworkStream::SendRunupMatrixAnswerPacket(const char* c_szMatrixCardString)
{
	TPacketCGRunupMatrixAnswer answerPacket;
	answerPacket.bHeader = HEADER_CG_RUNUP_MATRIX_ANSWER;
	strncpy(answerPacket.szAnswer, c_szMatrixCardString, RUNUP_MATRIX_ANSWER_MAX_LEN);
	answerPacket.szAnswer[RUNUP_MATRIX_ANSWER_MAX_LEN] = '\0';
	if (!Send(sizeof(answerPacket), &answerPacket))
	{
		TraceError("SendRunupMatrixCardPacketError");
		return false;
	}
	return SendSequence();
}

bool CNetworkStreamLoginPhase::__RecvNEWCIBNPasspodRequestPacket(const protocol::Packet& value)
{
	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_LOGIN], "BINARY_OnNEWCIBNPasspodRequest", Py_BuildValue("()"));
	return true;
}

bool CNetworkStreamLoginPhase::__RecvNEWCIBNPasspodFailurePacket(const protocol::Packet& value)
{
	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_LOGIN], "BINARY_OnNEWCIBNPasspodFailure", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::SendNEWCIBNPasspodAnswerPacket(const char* answer)
{
	TPacketCGNEWCIBNPasspodAnswer answerPacket;
	answerPacket.bHeader = HEADER_CG_NEWCIBN_PASSPOD_ANSWER;
	strncpy(answerPacket.szAnswer, answer, NEWCIBN_PASSPOD_ANSWER_MAX_LEN);
	answerPacket.szAnswer[NEWCIBN_PASSPOD_ANSWER_MAX_LEN] = '\0';
	if (!Send(sizeof(answerPacket), &answerPacket))
	{
		TraceError("SendNEWCIBNPasspodAnswerPacket");
		return false;
	}
	return SendSequence();
}

bool CPythonNetworkStream::SendChinaMatrixCardPacket(const char* c_szMatrixCardString)
{
	TPacketCGChinaMatrixCard MatrixCardPacket;
	MatrixCardPacket.bHeader = HEADER_CG_CHINA_MATRIX_CARD;
	strncpy(MatrixCardPacket.szAnswer, c_szMatrixCardString, CHINA_MATRIX_ANSWER_MAX_LEN);
	MatrixCardPacket.szAnswer[CHINA_MATRIX_ANSWER_MAX_LEN] = '\0';

	if (!Send(sizeof(MatrixCardPacket), &MatrixCardPacket))
	{
		Tracen("SendLogin Error");
		return false;
	}

	m_isWaitLoginKey = TRUE;

	return SendSequence();
}

#define ROW(rows, i) ((rows >> ((4 - i - 1) * 8)) & 0x000000FF)
#define COL(cols, i) ((cols >> ((4 - i - 1) * 8)) & 0x000000FF)

bool CNetworkStreamLoginPhase::__RecvChinaMatrixCardPacket(const protocol::Packet& value)
{
	TPacketGCChinaMatrixCard kMatrixCardPacket = value.Get<TPacketGCChinaMatrixCard>();

	PyObject* pyValue = Py_BuildValue("(iiiiiiii)", ROW(kMatrixCardPacket.dwRows, 0),
									  ROW(kMatrixCardPacket.dwRows, 1),
									  ROW(kMatrixCardPacket.dwRows, 2),
									  ROW(kMatrixCardPacket.dwRows, 3),
									  COL(kMatrixCardPacket.dwCols, 0),
									  COL(kMatrixCardPacket.dwCols, 1),
									  COL(kMatrixCardPacket.dwCols, 2),
									  COL(kMatrixCardPacket.dwCols, 3));
	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_LOGIN], "OnMatrixCard", pyValue);
	return true;
}

bool CNetworkStreamLoginPhase::__RecvLoginKeyPacket(const protocol::Packet& value)
{
	TPacketGCLoginKey kLoginKeyPacket = value.Get<TPacketGCLoginKey>();

	m_rkStream.m_dwLoginKey = kLoginKeyPacket.dwLoginKey;
	m_rkStream.m_isWaitLoginKey = FALSE;

	return true;
}

CNetworkStreamLoginPhase::CNetworkStreamLoginPhase(CPythonNetworkStream& rkStream) : CNetworkStreamPhase(rkStream)
{
}

ENetworkStreamPhaseResult CNetworkStreamLoginPhase::Handle(uint8_t header, const protocol::Packet& value)
{
	using Handler = bool (CNetworkStreamLoginPhase::*)(const protocol::Packet&);

	struct FEntry
	{
			uint8_t header;
			Handler handler;
	};

	static const FEntry entries[] = {
		{HEADER_GC_CHINA_MATRIX_CARD, &CNetworkStreamLoginPhase::__RecvChinaMatrixCardPacket},
		{HEADER_GC_EMPIRE, &CNetworkStreamLoginPhase::__RecvEmpirePacket},
		{HEADER_GC_LOGIN_FAILURE, &CNetworkStreamLoginPhase::__RecvLoginFailurePacket},
		{HEADER_GC_LOGIN_KEY, &CNetworkStreamLoginPhase::__RecvLoginKeyPacket},
		{HEADER_GC_LOGIN_SUCCESS3, &CNetworkStreamLoginPhase::__RecvLoginSuccessPacket3},
		{HEADER_GC_LOGIN_SUCCESS4, &CNetworkStreamLoginPhase::__RecvLoginSuccessPacket4},
		{HEADER_GC_NEWCIBN_PASSPOD_FAILURE, &CNetworkStreamLoginPhase::__RecvNEWCIBNPasspodFailurePacket},
		{HEADER_GC_NEWCIBN_PASSPOD_REQUEST, &CNetworkStreamLoginPhase::__RecvNEWCIBNPasspodRequestPacket},
		{HEADER_GC_RUNUP_MATRIX_QUIZ, &CNetworkStreamLoginPhase::__RecvRunupMatrixQuizPacket},
	};

	for (const FEntry& entry : entries)
	{
		if (entry.header == header)
			return (this->*entry.handler)(value) ? ENetworkStreamPhaseResult::Handled : ENetworkStreamPhaseResult::Failed;
	}

	return ENetworkStreamPhaseResult::NotHandled;
}
