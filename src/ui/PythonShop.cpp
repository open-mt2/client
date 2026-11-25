#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonShop.h>

#include <Client/UI/PythonNetworkStream.h>

// BOOL CPythonShop::GetSlotItemID(DWORD dwSlotPos, DWORD* pdwItemID)
//{
//	if (!CheckSlotIndex(dwSlotPos))
//		return FALSE;
//	const TShopItemData * itemData;
//	if (!GetItemData(dwSlotPos, &itemData))
//		return FALSE;
//	*pdwItemID=itemData->vnum;
//	return TRUE;
// }
void CPythonShop::SetTabCoinType(BYTE tabIdx, BYTE coinType)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return;
	}
	m_aShoptabs[tabIdx].coinType = coinType;
}

BYTE CPythonShop::GetTabCoinType(BYTE tabIdx)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return 0xff;
	}
	return m_aShoptabs[tabIdx].coinType;
}

void CPythonShop::SetTabName(BYTE tabIdx, const char* name)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return;
	}
	m_aShoptabs[tabIdx].name = name;
}

const char* CPythonShop::GetTabName(BYTE tabIdx)
{
	if (tabIdx >= m_bTabCount)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d.", tabIdx, SHOP_TAB_COUNT_MAX);
		return NULL;
	}

	return m_aShoptabs[tabIdx].name.c_str();
}

void CPythonShop::SetItemData(DWORD dwIndex, const TShopItemData& c_rShopItemData)
{
	BYTE tabIdx = dwIndex / SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % SHOP_HOST_ITEM_MAX_NUM;

	SetItemData(tabIdx, dwSlotPos, c_rShopItemData);
}

BOOL CPythonShop::GetItemData(DWORD dwIndex, const TShopItemData** c_ppItemData)
{
	BYTE tabIdx = dwIndex / SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % SHOP_HOST_ITEM_MAX_NUM;

	return GetItemData(tabIdx, dwSlotPos, c_ppItemData);
}

void CPythonShop::SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData& c_rShopItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, SHOP_HOST_ITEM_MAX_NUM);
		return;
	}

	m_aShoptabs[tabIdx].items[dwSlotPos] = c_rShopItemData;
}

BOOL CPythonShop::GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData** c_ppItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, SHOP_HOST_ITEM_MAX_NUM);
		return FALSE;
	}

	*c_ppItemData = &m_aShoptabs[tabIdx].items[dwSlotPos];

	return TRUE;
}
//
// BOOL CPythonShop::CheckSlotIndex(DWORD dwSlotPos)
//{
//	if (dwSlotPos >= SHOP_HOST_ITEM_MAX_NUM * SHOP_TAB_COUNT_MAX)
//		return FALSE;
//
//	return TRUE;
//}

void CPythonShop::ClearPrivateShopStock()
{
	m_PrivateShopItemStock.clear();
}
void CPythonShop::AddPrivateShopItemStock(TItemPos ItemPos, BYTE dwDisplayPos, DWORD dwPrice)
{
	DelPrivateShopItemStock(ItemPos);

	TShopItemTable SellingItem;
	SellingItem.vnum = 0;
	SellingItem.count = 0;
	SellingItem.pos = ItemPos;
	SellingItem.price = dwPrice;
	SellingItem.display_pos = dwDisplayPos;
	m_PrivateShopItemStock.insert(std::make_pair(ItemPos, SellingItem));
}
void CPythonShop::DelPrivateShopItemStock(TItemPos ItemPos)
{
	if (m_PrivateShopItemStock.end() == m_PrivateShopItemStock.find(ItemPos))
		return;

	m_PrivateShopItemStock.erase(ItemPos);
}
int CPythonShop::GetPrivateShopItemPrice(TItemPos ItemPos)
{
	TPrivateShopItemStock::iterator itor = m_PrivateShopItemStock.find(ItemPos);

	if (m_PrivateShopItemStock.end() == itor)
		return 0;

	TShopItemTable& rShopItemTable = itor->second;
	return rShopItemTable.price;
}
struct ItemStockSortFunc
{
		bool operator()(TShopItemTable& rkLeft, TShopItemTable& rkRight)
		{
			return rkLeft.display_pos < rkRight.display_pos;
		}
};
void CPythonShop::BuildPrivateShop(const char* c_szName)
{
	std::vector<TShopItemTable> ItemStock;
	ItemStock.reserve(m_PrivateShopItemStock.size());

	TPrivateShopItemStock::iterator itor = m_PrivateShopItemStock.begin();
	for (; itor != m_PrivateShopItemStock.end(); ++itor)
	{
		ItemStock.push_back(itor->second);
	}

	std::sort(ItemStock.begin(), ItemStock.end(), ItemStockSortFunc());

	CPythonNetworkStream::Instance().SendBuildPrivateShopPacket(c_szName, ItemStock);
}

void CPythonShop::Open(BOOL isPrivateShop, BOOL isMainPrivateShop)
{
	m_isShoping = TRUE;
	m_isPrivateShop = isPrivateShop;
	m_isMainPlayerPrivateShop = isMainPrivateShop;
}

void CPythonShop::Close()
{
	m_isShoping = FALSE;
	m_isPrivateShop = FALSE;
	m_isMainPlayerPrivateShop = FALSE;
}

BOOL CPythonShop::IsOpen()
{
	return m_isShoping;
}

BOOL CPythonShop::IsPrivateShop()
{
	return m_isPrivateShop;
}

BOOL CPythonShop::IsMainPlayerPrivateShop()
{
	return m_isMainPlayerPrivateShop;
}

void CPythonShop::Clear()
{
	m_isShoping = FALSE;
	m_isPrivateShop = FALSE;
	m_isMainPlayerPrivateShop = FALSE;
	ClearPrivateShopStock();
	m_bTabCount = 1;

	for (int i = 0; i < SHOP_TAB_COUNT_MAX; i++)
		memset(m_aShoptabs[i].items, 0, sizeof(TShopItemData) * SHOP_HOST_ITEM_MAX_NUM);
}

CPythonShop::CPythonShop(void)
{
	Clear();
}

CPythonShop::~CPythonShop(void)
{
}

ScriptObject* shopOpen(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int isPrivateShop = false;
	ScriptTuple_GetInteger(poArgs, 0, &isPrivateShop);
	int isMainPrivateShop = false;
	ScriptTuple_GetInteger(poArgs, 1, &isMainPrivateShop);

	CPythonShop& rkShop = CPythonShop::Instance();
	rkShop.Open(isPrivateShop, isMainPrivateShop);
	return Script_BuildNone();
}

ScriptObject* shopClose(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonShop& rkShop = CPythonShop::Instance();
	rkShop.Close();
	return Script_BuildNone();
}

ScriptObject* shopIsOpen(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonShop& rkShop = CPythonShop::Instance();
	return Script_BuildValue("i", rkShop.IsOpen());
}

ScriptObject* shopIsPrviateShop(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonShop& rkShop = CPythonShop::Instance();
	return Script_BuildValue("i", rkShop.IsPrivateShop());
}

ScriptObject* shopIsMainPlayerPrivateShop(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonShop& rkShop = CPythonShop::Instance();
	return Script_BuildValue("i", rkShop.IsMainPlayerPrivateShop());
}

ScriptObject* shopGetItemID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nPos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nPos))
		return Script_BuildException();

	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(nPos, &c_pItemData))
		return Script_BuildValue("i", c_pItemData->vnum);

	return Script_BuildValue("i", 0);
}

ScriptObject* shopGetItemCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Script_BuildValue("i", c_pItemData->count);

	return Script_BuildValue("i", 0);
}

ScriptObject* shopGetItemPrice(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Script_BuildValue("i", c_pItemData->price);

	return Script_BuildValue("i", 0);
}

ScriptObject* shopGetItemMetinSocket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	int iMetinSocketIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
		return Script_BuildException();

	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Script_BuildValue("i", c_pItemData->alSockets[iMetinSocketIndex]);

	return Script_BuildValue("i", 0);
}

ScriptObject* shopGetItemAttribute(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	int iAttrSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Script_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		const TShopItemData* c_pItemData;
		if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
			return Script_BuildValue("ii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Script_BuildValue("ii", 0, 0);
}

ScriptObject* shopClearPrivateShopStock(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonShop::Instance().ClearPrivateShopStock();
	return Script_BuildNone();
}
ScriptObject* shopAddPrivateShopItemStock(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bItemWindowType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Script_BuildException();
	WORD wItemSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Script_BuildException();
	int iDisplaySlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iDisplaySlotIndex))
		return Script_BuildException();
	int iPrice;
	if (!ScriptTuple_GetInteger(poArgs, 3, &iPrice))
		return Script_BuildException();

	CPythonShop::Instance().AddPrivateShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex), iDisplaySlotIndex, iPrice);
	return Script_BuildNone();
}
ScriptObject* shopDelPrivateShopItemStock(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bItemWindowType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Script_BuildException();
	WORD wItemSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Script_BuildException();

	CPythonShop::Instance().DelPrivateShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex));
	return Script_BuildNone();
}
ScriptObject* shopGetPrivateShopItemPrice(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bItemWindowType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Script_BuildException();
	WORD wItemSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Script_BuildException();

	int iValue = CPythonShop::Instance().GetPrivateShopItemPrice(TItemPos(bItemWindowType, wItemSlotIndex));
	return Script_BuildValue("i", iValue);
}
ScriptObject* shopBuildPrivateShop(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonShop::Instance().BuildPrivateShop(szName);
	return Script_BuildNone();
}

ScriptObject* shopGetTabCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonShop::instance().GetTabCount());
}

ScriptObject* shopGetTabName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bTabIdx;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bTabIdx))
		return Script_BuildException();

	return Script_BuildValue("s", CPythonShop::instance().GetTabName(bTabIdx));
}

ScriptObject* shopGetTabCoinType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bTabIdx;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bTabIdx))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonShop::instance().GetTabCoinType(bTabIdx));
}

void initshop()
{
	static ScriptMethodDef s_methods[] =
		{
			// Shop
			{"Open", shopOpen, SCRIPT_VARARGS, NULL},
			{"Close", shopClose, SCRIPT_VARARGS, NULL},
			{"IsOpen", shopIsOpen, SCRIPT_VARARGS, NULL},
			{"IsPrivateShop", shopIsPrviateShop, SCRIPT_VARARGS, NULL},
			{"IsMainPlayerPrivateShop", shopIsMainPlayerPrivateShop, SCRIPT_VARARGS, NULL},
			{"GetItemID", shopGetItemID, SCRIPT_VARARGS, NULL},
			{"GetItemCount", shopGetItemCount, SCRIPT_VARARGS, NULL},
			{"GetItemPrice", shopGetItemPrice, SCRIPT_VARARGS, NULL},
			{"GetItemMetinSocket", shopGetItemMetinSocket, SCRIPT_VARARGS, NULL},
			{"GetItemAttribute", shopGetItemAttribute, SCRIPT_VARARGS, NULL},
			{"GetTabCount", shopGetTabCount, SCRIPT_VARARGS, NULL},
			{"GetTabName", shopGetTabName, SCRIPT_VARARGS, NULL},
			{"GetTabCoinType", shopGetTabCoinType, SCRIPT_VARARGS, NULL},

			// Private Shop
			{"ClearPrivateShopStock", shopClearPrivateShopStock, SCRIPT_VARARGS, NULL},
			{"AddPrivateShopItemStock", shopAddPrivateShopItemStock, SCRIPT_VARARGS, NULL},
			{"DelPrivateShopItemStock", shopDelPrivateShopItemStock, SCRIPT_VARARGS, NULL},
			{"GetPrivateShopItemPrice", shopGetPrivateShopItemPrice, SCRIPT_VARARGS, NULL},
			{"BuildPrivateShop", shopBuildPrivateShop, SCRIPT_VARARGS, NULL},
			{NULL, NULL, 0, NULL},
		};
	ScriptObject* poModule = ScriptModule_Create("shop", s_methods);

	ScriptModule_AddIntConstant(poModule, "SHOP_SLOT_COUNT", SHOP_HOST_ITEM_MAX_NUM);
	ScriptModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_GOLD", SHOP_COIN_TYPE_GOLD);
	ScriptModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_SECONDARY_COIN", SHOP_COIN_TYPE_SECONDARY_COIN);
}