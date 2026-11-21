#pragma once
#include <utility>
#include <cstdint>
#include <string>
#include <vector>

typedef std::pair<uint32_t, std::string> CRCPair;

void ProcessScanner_Destroy();
bool ProcessScanner_Create();
void ProcessScanner_ReleaseQuitEvent();

bool ProcessScanner_PopProcessQueue(std::vector<CRCPair>* pkVct_crcPair);
