#include <Client/GameData/GameData.h>

#include <Client/GameData/ItemManager.h>
#include <Neoeter/Platform/Base/Debug.h>

#include <cstdio>

namespace GameData
{
	bool LoadItemTables(const char* c_szLocalePath)
	{
		CItemManager& rkItemMgr = CItemManager::Instance();

		char szItemList[256];
		char szItemProto[256];
		char szItemDesc[256];
		snprintf(szItemList, sizeof(szItemList), "%s/item_list.txt", c_szLocalePath);
		snprintf(szItemProto, sizeof(szItemProto), "%s/item_proto", c_szLocalePath);
		snprintf(szItemDesc, sizeof(szItemDesc), "%s/itemdesc.txt", c_szLocalePath);

		rkItemMgr.Destroy();

		if (!rkItemMgr.LoadItemList(szItemList))
			TraceError("GameData::LoadItemTables - LoadItemList(%s) Error", szItemList);

		if (!rkItemMgr.LoadItemTable(szItemProto))
		{
			TraceError("GameData::LoadItemTables - LoadItemProto(%s) Error", szItemProto);
			return false;
		}

		if (!rkItemMgr.LoadItemDesc(szItemDesc))
			Tracenf("GameData::LoadItemTables - LoadItemDesc(%s) Error", szItemDesc);

		return true;
	}
}
