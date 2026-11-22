#include <Client/UI/StdAfx.h>
#include <Client/Python/Network/PythonNetworkStream.h>
#include <Client/Session/NetworkStreamPhases.h>
#include <Client/Python/Application/PythonApplication.h>
#include <Client/Game/Packet.h>

void CPythonNetworkStream::OffLinePhase()
{
	TPacketHeader header;

	if (!CheckPacket(&header))
		return;

	if (OMT2Client::Protocol::Has(header))
	{
		if (DecodeProtocolPacket(header))
			return;

		RecvErrorPacket(header);
		return;
	}

	RecvErrorPacket(header);
}

CNetworkStreamOfflinePhase::CNetworkStreamOfflinePhase(CPythonNetworkStream& rkStream) : CNetworkStreamPhase(rkStream)
{
}

ENetworkStreamPhaseResult CNetworkStreamOfflinePhase::Handle(uint8_t header, const OMT2Client::Protocol::Packet& value)
{
	using Handler = bool (CNetworkStreamOfflinePhase::*)(const OMT2Client::Protocol::Packet&);

	struct FEntry
	{
			uint8_t header;
			Handler handler;
	};

	static const FEntry entries[] = {};

	for (const FEntry& entry : entries)
	{
		if (entry.header == header)
			return (this->*entry.handler)(value) ? ENetworkStreamPhaseResult::Handled : ENetworkStreamPhaseResult::Failed;
	}

	return ENetworkStreamPhaseResult::NotHandled;
}
