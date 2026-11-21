#include <Client/UI/StdAfx.h>
#include <Client/Platform/ProcessCRC.h>
bool GetExeCRC(uint32_t& r_dwProcCRC, uint32_t& r_dwFileCRC)
{
	r_dwProcCRC = 0;
	r_dwFileCRC = 0;
	return true;
}

void BuildProcessCRC() {}
uint8_t GetProcessCRCMagicCubePiece() { return 0; }
