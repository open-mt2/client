#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Where the game data tables come from. The tables ask this for bytes and never
// open a pack or a file themselves; the owner of the entries implements it.
class CGameDataFileSource
{
	public:
		virtual ~CGameDataFileSource() {}

		// Fills rBytes with the whole entry; false when the source cannot find it.
		virtual bool ReadFile(const std::string& c_rstPath, std::vector<uint8_t>& rBytes) = 0;

		// Lists the entries whose path starts with c_rstPrefix. Paths are
		// lowercased and use '/' separators, the form the tables register.
		virtual void ListFiles(const std::string& c_rstPrefix, std::vector<std::string>& rPaths) = 0;
};

// The source stays installed for the process lifetime. A process without one
// reads no table, which is the state a unit test starts from.
void SetGameDataFileSource(CGameDataFileSource* pSource);

// Reads one entry through the installed source; false when none is installed.
bool GameDataReadFile(const std::string& c_rstPath, std::vector<uint8_t>& rBytes);

// Lists entries through the installed source; false when none is installed.
bool GameDataListFiles(const std::string& c_rstPrefix, std::vector<std::string>& rPaths);
