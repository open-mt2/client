#include <Client/UI/StdAfx.h>
#include <Client/UI/Util/ProcessScanner.h>
void ProcessScanner_Destroy() {}
bool ProcessScanner_Create() { return true; }
void ProcessScanner_ReleaseQuitEvent() {}
bool ProcessScanner_PopProcessQueue(std::vector<CRCPair>*) { return false; }
