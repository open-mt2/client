#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonNetworkStream.h>
#include <Client/UI/PythonApplication.h>
#include <Client/UI/Packet.h>

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
