#include <Client/GameData/FileSource.h>

namespace
{
	CGameDataFileSource* s_pSource = NULL;
}

void SetGameDataFileSource(CGameDataFileSource* pSource)
{
	s_pSource = pSource;
}

bool GameDataReadFile(const std::string& c_rstPath, std::vector<uint8_t>& rBytes)
{
	if (!s_pSource)
		return false;

	return s_pSource->ReadFile(c_rstPath, rBytes);
}

bool GameDataListFiles(const std::string& c_rstPrefix, std::vector<std::string>& rPaths)
{
	if (!s_pSource)
		return false;

	s_pSource->ListFiles(c_rstPrefix, rPaths);
	return true;
}
