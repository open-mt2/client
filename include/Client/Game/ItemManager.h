#pragma once

#include <Client/Game/ItemData.h>

class CItemManager : public CSingleton<CItemManager>
{
	public:
		enum EItemDescCol
		{
			ITEMDESC_COL_VNUM,
			ITEMDESC_COL_NAME,
			ITEMDESC_COL_DESC,
			ITEMDESC_COL_SUMM,
			ITEMDESC_COL_NUM,
		};

	public:
		typedef std::map<uint32_t, CItemData*> TItemMap;
		typedef std::map<std::string, CItemData*> TItemNameMap;

	public:
		CItemManager();
		virtual ~CItemManager();

		void Destroy();

		int32_t SelectItemData(uint32_t dwIndex);
		CItemData* GetSelectedItemDataPointer();

		int32_t GetItemDataPointer(uint32_t dwItemID, CItemData** ppItemData);

		/////
		bool LoadItemDesc(const char* c_szFileName);
		bool LoadItemList(const char* c_szFileName);
		bool LoadItemTable(const char* c_szFileName);
		CItemData* MakeItemData(uint32_t dwIndex);

	protected:
		TItemMap m_ItemMap;
		std::vector<CItemData*> m_vec_ItemRange;
		CItemData* m_pSelectedItemData;
};
