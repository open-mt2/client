#pragma once

#include <Client/GameData/FileSource.h>

// Serves game data entries from the loaded packs, then from loose files, which
// is the lookup order the client already used for every table.
class CPackFileSource : public CGameDataFileSource
{
	public:
		bool ReadFile(const std::string& c_rstPath, std::vector<uint8_t>& rBytes) override;
		void ListFiles(const std::string& c_rstPrefix, std::vector<std::string>& rPaths) override;
};
