#pragma once

#include <Client/Protocol/Packet.h>
#include <Client/Game/Affects.h>

/*
 *	상점 처리
 *
 *	2003-01-16 anoa	일차 완료
 *	2003-12-26 levites 수정
 *
 *	2012-10-29 rtsummit 새로운 화폐 출현 및 tab 기능 추가로 인한 shop 확장.
 *
 */
typedef enum
{
	SHOP_COIN_TYPE_GOLD, // DEFAULT VALUE
	SHOP_COIN_TYPE_SECONDARY_COIN,
} EShopCoinType;

class CPythonShop : public CSingleton<CPythonShop>
{
	public:
		CPythonShop(void);
		virtual ~CPythonShop(void);

		void Clear();

		void SetItemData(uint32_t dwIndex, const TShopItemData& c_rShopItemData);
		int32_t GetItemData(uint32_t dwIndex, const TShopItemData** c_ppItemData);

		void SetItemData(uint8_t tabIdx, uint32_t dwSlotPos, const TShopItemData& c_rShopItemData);
		int32_t GetItemData(uint8_t tabIdx, uint32_t dwSlotPos, const TShopItemData** c_ppItemData);

		void SetTabCount(uint8_t bTabCount) { m_bTabCount = bTabCount; }
		uint8_t GetTabCount() { return m_bTabCount; }

		void SetTabCoinType(uint8_t tabIdx, uint8_t coinType);
		uint8_t GetTabCoinType(uint8_t tabIdx);

		void SetTabName(uint8_t tabIdx, const char* name);
		const char* GetTabName(uint8_t tabIdx);

		// int32_t GetSlotItemID(uint32_t dwSlotPos, uint32_t* pdwItemID);

		void Open(int32_t isPrivateShop, int32_t isMainPrivateShop);
		void Close();
		int32_t IsOpen();
		int32_t IsPrivateShop();
		int32_t IsMainPlayerPrivateShop();

		void ClearPrivateShopStock();
		void AddPrivateShopItemStock(TItemPos ItemPos, uint8_t byDisplayPos, uint32_t dwPrice);
		void DelPrivateShopItemStock(TItemPos ItemPos);
		int GetPrivateShopItemPrice(TItemPos ItemPos);
		void BuildPrivateShop(const char* c_szName);

	protected:
		int32_t CheckSlotIndex(uint32_t dwIndex);

	protected:
		int32_t m_isShoping;
		int32_t m_isPrivateShop;
		int32_t m_isMainPlayerPrivateShop;

		struct ShopTab
		{
				ShopTab()
				{
					coinType = SHOP_COIN_TYPE_GOLD;
				}
				uint8_t coinType;
				std::string name;
				TShopItemData items[SHOP_HOST_ITEM_MAX_NUM];
		};

		uint8_t m_bTabCount;
		ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];

		typedef std::map<TItemPos, TShopItemTable> TPrivateShopItemStock;
		TPrivateShopItemStock m_PrivateShopItemStock;
};
