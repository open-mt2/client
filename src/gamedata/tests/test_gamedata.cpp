#include <Client/GameData/FileSource.h>
#include <Client/GameData/ItemManager.h>
#include <Client/GameData/PropertyManager.h>
#include <Client/GameData/Property.h>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace
{
	// Answers the game data port from memory, so a table can be loaded without a
	// pack and without a file on disk.
	class CFakeFileSource : public CGameDataFileSource
	{
		public:
			void Add(const std::string& c_rstPath, const std::string& c_rstContent)
			{
				m_files[c_rstPath] = c_rstContent;
			}

			const std::string& GetLastPath() const
			{
				return m_stLastPath;
			}

			bool ReadFile(const std::string& c_rstPath, std::vector<uint8_t>& rBytes) override
			{
				m_stLastPath = c_rstPath;

				std::map<std::string, std::string>::const_iterator it = m_files.find(c_rstPath);
				if (m_files.end() == it)
					return false;

				rBytes.assign(it->second.begin(), it->second.end());
				return true;
			}

			void ListFiles(const std::string& c_rstPrefix, std::vector<std::string>& rPaths) override
			{
				for (std::map<std::string, std::string>::const_iterator it = m_files.begin(); it != m_files.end(); ++it)
				{
					if (0 == it->first.compare(0, c_rstPrefix.size(), c_rstPrefix))
						rPaths.push_back(it->first);
				}
			}

		private:
			std::map<std::string, std::string> m_files;
			std::string m_stLastPath;
	};

	void test_item_desc_crosses_the_port()
	{
		CFakeFileSource source;
		source.Add("locale/test/itemdesc.txt", "1001\tSword\tA sword\tA short sword\n");
		SetGameDataFileSource(&source);

		CItemManager itemMgr;
		CItemData* pItem = itemMgr.MakeItemData(1001);
		assert(pItem != NULL);

		assert(itemMgr.LoadItemDesc("locale/test/itemdesc.txt"));
		assert(source.GetLastPath() == "locale/test/itemdesc.txt");
		assert(std::strcmp(pItem->GetDescription(), "A sword") == 0);
		assert(std::strcmp(pItem->GetSummary(), "A short sword") == 0);

		CItemData* pUnknown = NULL;
		assert(!itemMgr.GetItemDataPointer(2002, &pUnknown));

		SetGameDataFileSource(NULL);

		std::printf("[PASS] test_item_desc_crosses_the_port\n");
	}

	void test_missing_entry_fails()
	{
		CFakeFileSource source;
		SetGameDataFileSource(&source);

		CItemManager itemMgr;
		assert(!itemMgr.LoadItemDesc("locale/test/itemdesc.txt"));

		CPropertyManager propMgr;

		CProperty* pProperty = NULL;
		assert(!propMgr.Register("property/test.txt", &pProperty));
		assert(pProperty == NULL);

		SetGameDataFileSource(NULL);

		std::printf("[PASS] test_missing_entry_fails\n");
	}

	void test_property_scan_registers_entries()
	{
		CFakeFileSource source;
		source.Add("property/test.txt", "YPRT\r\n12345\r\nmaptype\tindoor\r\nscripttype\tobject\r\n");
		source.Add("property/reserve", "777\n");
		SetGameDataFileSource(&source);

		CPropertyManager propMgr;
		propMgr.Scan("property/");

		CProperty* pProperty = NULL;
		assert(propMgr.Get(12345, &pProperty));
		assert(pProperty != NULL);

		const char* c_szValue = NULL;
		assert(pProperty->GetString("maptype", &c_szValue));
		assert(std::strcmp(c_szValue, "indoor") == 0);

		SetGameDataFileSource(NULL);

		std::printf("[PASS] test_property_scan_registers_entries\n");
	}

	void test_no_source_reads_no_table()
	{
		SetGameDataFileSource(NULL);

		CItemManager itemMgr;
		CPropertyManager propMgr;

		assert(!itemMgr.LoadItemDesc("locale/test/itemdesc.txt"));

		CProperty* pProperty = NULL;
		assert(!propMgr.Register("property/test.txt", &pProperty));

		propMgr.Scan("property/");

		std::printf("[PASS] test_no_source_reads_no_table\n");
	}
}

int main()
{
	test_item_desc_crosses_the_port();
	test_missing_entry_fails();
	test_property_scan_registers_entries();
	test_no_source_reads_no_table();

	std::printf("All GameData tests passed successfully!\n");
	return 0;
}
