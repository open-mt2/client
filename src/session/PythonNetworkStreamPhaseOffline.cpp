#include <Client/UI/StdAfx.h>
#include <Client/Session/PythonNetworkStream.h>
#include <Client/Session/NetworkStreamPhases.h>
#include <Client/UI/App/PythonApplication.h>
#include <Client/Protocol/Packet.h>

void CPythonNetworkStream::OffLinePhase()
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

	RecvErrorPacket(header);
}

CNetworkStreamOfflinePhase::CNetworkStreamOfflinePhase(CPythonNetworkStream& rkStream) : CNetworkStreamPhase(rkStream)
{
}

ENetworkStreamPhaseResult CNetworkStreamOfflinePhase::Handle(uint8_t header, const protocol::Packet& value)
{
	using Handler = bool (CNetworkStreamOfflinePhase::*)(const protocol::Packet&);

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
