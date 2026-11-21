#include <Client/UI/StdAfx.h>
#include <Client/Python/Network/PythonNetworkStream.h>
#include <Client/Session/NetworkStreamPhases.h>
#include <Client/Python/Application/PythonApplication.h>
#include <Client/Game/Packet.h>
#include <Neoeter/IO/EterPackManager.h>

// HandShake ---------------------------------------------------------------------------
void CPythonNetworkStream::HandShakePhase()
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
		case HEADER_GC_HANDSHAKE:
		{
			if (!Recv(sizeof(TPacketGCHandshake), &m_HandshakeData))
				return;

			Tracenf("HANDSHAKE RECV %u %d", m_HandshakeData.dwTime, m_HandshakeData.lDelta);

			ELTimer_SetServerMSec(m_HandshakeData.dwTime + m_HandshakeData.lDelta);

			// m_dwBaseServerTime = m_HandshakeData.dwTime+ m_HandshakeData.lDelta;
			// m_dwBaseClientTime = ELTimer_GetMSec();

			m_HandshakeData.dwTime = m_HandshakeData.dwTime + m_HandshakeData.lDelta + m_HandshakeData.lDelta;
			m_HandshakeData.lDelta = 0;

			Tracenf("HANDSHAKE SEND %u", m_HandshakeData.dwTime);

			if (!Send(sizeof(TPacketGCHandshake), &m_HandshakeData))
			{
				assert(!"Failed Sending Handshake");
				return;
			}

			CTimer::Instance().SetBaseTime();
			return;
		}
		break;
	}

	RecvErrorPacket(header);
}

void CPythonNetworkStream::SetHandShakePhase()
{
	if ("HandShake" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Hand Shake Phase ##");
	Tracen("");

	m_strPhase = "HandShake";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::HandShakePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveHandshakePhase);

	SetGameOnline();

	if (__DirectEnterMode_IsSet())
	{
		// None
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOGIN], "OnHandShake", Py_BuildValue("()"));
	}
}

bool CPythonNetworkStream::RecvHandshakePacket()
{
	TPacketGCHandshake kHandshakeData;
	if (!Recv(sizeof(TPacketGCHandshake), &kHandshakeData))
		return false;

	Tracenf("HANDSHAKE RECV %u %d", kHandshakeData.dwTime, kHandshakeData.lDelta);

	ELTimer_SetServerMSec(kHandshakeData.dwTime + kHandshakeData.lDelta);
	m_kServerTimeSync.m_dwChangeServerTime = kHandshakeData.dwTime + kHandshakeData.lDelta;
	m_kServerTimeSync.m_dwChangeClientTime = ELTimer_GetMSec();

	kHandshakeData.dwTime = kHandshakeData.dwTime + kHandshakeData.lDelta + kHandshakeData.lDelta;
	kHandshakeData.lDelta = 0;

	Tracenf("HANDSHAKE SEND %u", kHandshakeData.dwTime);

	kHandshakeData.header = HEADER_CG_HANDSHAKE;
	if (!Send(sizeof(TPacketGCHandshake), &kHandshakeData))
	{
		assert(!"Failed Sending Handshake");
		return false;
	}

	CTimer::Instance().SetBaseTime();

	return true;
}

bool CNetworkStreamHandShakePhase::RecvHandshakeOKPacket(const protocol::Packet& value)
{
	uint32_t dwDelta = ELTimer_GetMSec() - m_rkStream.m_kServerTimeSync.m_dwChangeClientTime;
	ELTimer_SetServerMSec(m_rkStream.m_kServerTimeSync.m_dwChangeServerTime + dwDelta);

	Tracenf("HANDSHAKE OK RECV %u %u", m_rkStream.m_kServerTimeSync.m_dwChangeServerTime, dwDelta);

	return true;
}

CNetworkStreamHandShakePhase::CNetworkStreamHandShakePhase(CPythonNetworkStream& rkStream) : CNetworkStreamPhase(rkStream)
{
}

ENetworkStreamPhaseResult CNetworkStreamHandShakePhase::Handle(uint8_t header, const protocol::Packet& value)
{
	using Handler = bool (CNetworkStreamHandShakePhase::*)(const protocol::Packet&);

	struct FEntry
	{
			uint8_t header;
			Handler handler;
	};

	static const FEntry entries[] = {
		{HEADER_GC_HANDSHAKE_OK, &CNetworkStreamHandShakePhase::RecvHandshakeOKPacket},
	};

	for (const FEntry& entry : entries)
	{
		if (entry.header == header)
			return (this->*entry.handler)(value) ? ENetworkStreamPhaseResult::Handled : ENetworkStreamPhaseResult::Failed;
	}

	return ENetworkStreamPhaseResult::NotHandled;
}
