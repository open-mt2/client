#include <Client/Application/PackFileSource.h>

#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/FileDir.h>
#include <Neoeter/Platform/Base/Utils.h>

#include <algorithm>

namespace
{
	// Collects the loose files under a directory, with the same rules the
	// property loader used: every depth, extension required, lowercased paths.
	class CDiskScan : public CDir
	{
		public:
			explicit CDiskScan(std::vector<std::string>& rFiles) : m_pFiles(&rFiles) {}

			void Scan(const char* c_szPath)
			{
				Create("*.*", c_szPath);
			}

		protected:
			bool OnFolder(const char* c_szFilter, const char* c_szPathName, const char* c_szFileName) override
			{
				std::string stPath = c_szPathName;
				stPath += c_szFileName;

				CDiskScan scan(*m_pFiles);
				scan.Scan(stPath.c_str());
				return true;
			}

			bool OnFile(const char* c_szPathName, const char* c_szFileName) override
			{
				std::string stPath = c_szPathName;
				stPath += c_szFileName;
				StringPath(stPath);

				m_pFiles->push_back(stPath);
				return true;
			}

		private:
			std::vector<std::string>* m_pFiles;
	};
}

bool CPackFileSource::ReadFile(const std::string& c_rstPath, std::vector<uint8_t>& rBytes)
{
	CEterPackManager::TPackDataPtr data;
	if (!CEterPackManager::Instance().Get(c_rstPath, data) || !data)
		return false;

	rBytes.assign(data->begin(), data->end());
	return true;
}

void CPackFileSource::ListFiles(const std::string& c_rstPrefix, std::vector<std::string>& rPaths)
{
	std::string stDiskDirectory = c_rstPrefix;
	std::string stPrefix = c_rstPrefix;
	StringPath(stPrefix);

	const CEterPackManager::TFileMap& rkFileMap = CEterPackManager::Instance().GetFileMap();
	for (CEterPackManager::TFileMap::const_iterator it = rkFileMap.begin(); it != rkFileMap.end(); ++it)
	{
		std::string stPath = it->first;
		StringPath(stPath);

		if (0 == stPath.compare(0, stPrefix.size(), stPrefix))
			rPaths.push_back(stPath);
	}

	std::vector<std::string> diskPaths;
	CDiskScan scan(diskPaths);
	scan.Scan(stDiskDirectory.c_str());

	for (size_t i = 0; i < diskPaths.size(); ++i)
	{
		if (0 == diskPaths[i].compare(0, stPrefix.size(), stPrefix))
			rPaths.push_back(diskPaths[i]);
	}

	std::sort(rPaths.begin(), rPaths.end());
	rPaths.erase(std::unique(rPaths.begin(), rPaths.end()), rPaths.end());
}
