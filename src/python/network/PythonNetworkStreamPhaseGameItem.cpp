#include <Client/UI/StdAfx.h>
#include <Client/Python/Network/PythonNetworkStream.h>
#include <Client/Session/NetworkStreamPhases.h>
#include <Client/Python/Item/PythonItem.h>
#include <Client/Python/Shop/PythonShop.h>
#include <Client/Python/Exchange/PythonExchange.h>
#include <Client/Python/SafeBox/PythonSafeBox.h>
#include <Client/Python/Character/PythonCharacterManager.h>

#include <Client/UI/AbstractPlayer.h>

//////////////////////////////////////////////////////////////////////////
// SafeBox

bool CPythonNetworkStream::SendSafeBoxMoneyPacket(uint8_t byState, uint32_t dwMoney)
{
	assert(!"CPythonNetworkStream::SendSafeBoxMoneyPacket - \xBB\xE7\xBF\xEB\xC7\xCF\xC1\xF6 \xBE\xCA\xB4\xC2 \xC7\xD4\xBC\xF6");
	return false;

	//	TPacketCGSafeboxMoney kSafeboxMoney;
	//	kSafeboxMoney.bHeader = HEADER_CG_SAFEBOX_MONEY;
	//	kSafeboxMoney.bState = byState;
	//	kSafeboxMoney.dwMoney = dwMoney;
	//	if (!Send(sizeof(kSafeboxMoney), &kSafeboxMoney))
	//		return false;
	//
	//	return SendSequence();
}

bool CPythonNetworkStream::SendSafeBoxCheckinPacket(TItemPos InventoryPos, uint8_t bySafeBoxPos)
{
	__PlayInventoryItemDropSound(InventoryPos);

	TPacketCGSafeboxCheckin kSafeboxCheckin;
	kSafeboxCheckin.bHeader = HEADER_CG_SAFEBOX_CHECKIN;
	kSafeboxCheckin.ItemPos = InventoryPos;
	kSafeboxCheckin.bSafePos = bySafeBoxPos;
	if (!Send(sizeof(kSafeboxCheckin), &kSafeboxCheckin))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendSafeBoxCheckoutPacket(uint8_t bySafeBoxPos, TItemPos InventoryPos)
{
	__PlaySafeBoxItemDropSound(bySafeBoxPos);

	TPacketCGSafeboxCheckout kSafeboxCheckout;
	kSafeboxCheckout.bHeader = HEADER_CG_SAFEBOX_CHECKOUT;
	kSafeboxCheckout.bSafePos = bySafeBoxPos;
	kSafeboxCheckout.ItemPos = InventoryPos;
	if (!Send(sizeof(kSafeboxCheckout), &kSafeboxCheckout))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendSafeBoxItemMovePacket(uint8_t bySourcePos, uint8_t byTargetPos, uint8_t byCount)
{
	__PlaySafeBoxItemDropSound(bySourcePos);

	TPacketCGItemMove kItemMove;
	kItemMove.header = HEADER_CG_SAFEBOX_ITEM_MOVE;
	kItemMove.pos = TItemPos(INVENTORY, bySourcePos);
	kItemMove.num = byCount;
	kItemMove.change_pos = TItemPos(INVENTORY, byTargetPos);
	if (!Send(sizeof(kItemMove), &kItemMove))
		return false;

	return SendSequence();
}

bool CNetworkStreamGameItemPhase::RecvSafeBoxSetPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemSet2 kItemSet = value.Get<TPacketGCItemSet2>();

	TItemData kItemData;
	kItemData.vnum = kItemSet.vnum;
	kItemData.count = kItemSet.count;
	kItemData.flags = kItemSet.flags;
	kItemData.anti_flags = kItemSet.anti_flags;
	for (int isocket = 0; isocket < ITEM_SOCKET_SLOT_MAX_NUM; ++isocket)
		kItemData.alSockets[isocket] = kItemSet.alSockets[isocket];
	for (int iattr = 0; iattr < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++iattr)
		kItemData.aAttr[iattr] = kItemSet.aAttr[iattr];

	CPythonSafeBox::Instance().SetItemData(kItemSet.Cell.cell, kItemData);

	m_rkStream.__RefreshSafeboxWindow();

	return true;
}

bool CNetworkStreamGameItemPhase::RecvSafeBoxDelPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemDel kItemDel = value.Get<TPacketGCItemDel>();

	CPythonSafeBox::Instance().DelItemData(kItemDel.pos);

	m_rkStream.__RefreshSafeboxWindow();

	return true;
}

bool CNetworkStreamGameItemPhase::RecvSafeBoxWrongPasswordPacket(const OMT2Client::Protocol::Packet& value)
{
	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "OnSafeBoxError", Py_BuildValue("()"));

	return true;
}

bool CNetworkStreamGameItemPhase::RecvSafeBoxSizePacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCSafeboxSize kSafeBoxSize = value.Get<TPacketGCSafeboxSize>();

	CPythonSafeBox::Instance().OpenSafeBox(kSafeBoxSize.bSize);
	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "OpenSafeboxWindow", Py_BuildValue("(i)", kSafeBoxSize.bSize));

	return true;
}

bool CNetworkStreamGameItemPhase::RecvSafeBoxMoneyChangePacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCSafeboxMoneyChange kMoneyChange = value.Get<TPacketGCSafeboxMoneyChange>();

	CPythonSafeBox::Instance().SetMoney(kMoneyChange.dwMoney);
	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "RefreshSafeboxMoney", Py_BuildValue("()"));

	return true;
}

// SafeBox
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Mall
bool CPythonNetworkStream::SendMallCheckoutPacket(uint8_t byMallPos, TItemPos InventoryPos)
{
	__PlayMallItemDropSound(byMallPos);

	TPacketCGMallCheckout kMallCheckoutPacket;
	kMallCheckoutPacket.bHeader = HEADER_CG_MALL_CHECKOUT;
	kMallCheckoutPacket.bMallPos = byMallPos;
	kMallCheckoutPacket.ItemPos = InventoryPos;
	if (!Send(sizeof(kMallCheckoutPacket), &kMallCheckoutPacket))
		return false;

	return SendSequence();
}

bool CNetworkStreamGameItemPhase::RecvMallOpenPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCMallOpen kMallOpen = value.Get<TPacketGCMallOpen>();

	CPythonSafeBox::Instance().OpenMall(kMallOpen.bSize);
	PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "OpenMallWindow", Py_BuildValue("(i)", kMallOpen.bSize));

	return true;
}
bool CNetworkStreamGameItemPhase::RecvMallItemSetPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemSet2 kItemSet = value.Get<TPacketGCItemSet2>();

	TItemData kItemData;
	kItemData.vnum = kItemSet.vnum;
	kItemData.count = kItemSet.count;
	kItemData.flags = kItemSet.flags;
	kItemData.anti_flags = kItemSet.anti_flags;
	for (int isocket = 0; isocket < ITEM_SOCKET_SLOT_MAX_NUM; ++isocket)
		kItemData.alSockets[isocket] = kItemSet.alSockets[isocket];
	for (int iattr = 0; iattr < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++iattr)
		kItemData.aAttr[iattr] = kItemSet.aAttr[iattr];

	CPythonSafeBox::Instance().SetMallItemData(kItemSet.Cell.cell, kItemData);

	m_rkStream.__RefreshMallWindow();

	return true;
}
bool CNetworkStreamGameItemPhase::RecvMallItemDelPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemDel kItemDel = value.Get<TPacketGCItemDel>();

	CPythonSafeBox::Instance().DelMallItemData(kItemDel.pos);

	m_rkStream.__RefreshMallWindow();
	Tracef(" >> CPythonNetworkStream::RecvMallItemDelPacket\n");

	return true;
}
// Mall
//////////////////////////////////////////////////////////////////////////

// Item
// Recieve
bool CNetworkStreamGameItemPhase::RecvItemSetPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemSet packet_item_set = value.Get<TPacketGCItemSet>();

	TItemData kItemData;
	kItemData.vnum = packet_item_set.vnum;
	kItemData.count = packet_item_set.count;
	kItemData.flags = 0;
	for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		kItemData.alSockets[i] = packet_item_set.alSockets[i];
	for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		kItemData.aAttr[j] = packet_item_set.aAttr[j];

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();

	rkPlayer.SetItemData(packet_item_set.Cell, kItemData);

	m_rkStream.__RefreshInventoryWindow();
	return true;
}

bool CNetworkStreamGameItemPhase::RecvItemSetPacket2(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemSet2 packet_item_set = value.Get<TPacketGCItemSet2>();

	TItemData kItemData;
	kItemData.vnum = packet_item_set.vnum;
	kItemData.count = packet_item_set.count;
	kItemData.flags = packet_item_set.flags;
	kItemData.anti_flags = packet_item_set.anti_flags;

	for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		kItemData.alSockets[i] = packet_item_set.alSockets[i];
	for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		kItemData.aAttr[j] = packet_item_set.aAttr[j];

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.SetItemData(packet_item_set.Cell, kItemData);

	if (packet_item_set.highlight)
		PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "BINARY_Highlight_Item", Py_BuildValue("(ii)", packet_item_set.Cell.window_type, packet_item_set.Cell.cell));

	m_rkStream.__RefreshInventoryWindow();
	return true;
}

bool CNetworkStreamGameItemPhase::RecvItemUsePacket(const OMT2Client::Protocol::Packet& value)
{
	m_rkStream.__RefreshInventoryWindow();
	return true;
}

bool CNetworkStreamGameItemPhase::RecvItemUpdatePacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemUpdate packet_item_update = value.Get<TPacketGCItemUpdate>();

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.SetItemCount(packet_item_update.Cell, packet_item_update.count);
	for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		rkPlayer.SetItemMetinSocket(packet_item_update.Cell, i, packet_item_update.alSockets[i]);
	for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		rkPlayer.SetItemAttribute(packet_item_update.Cell, j, packet_item_update.aAttr[j].bType, packet_item_update.aAttr[j].sValue);

	m_rkStream.__RefreshInventoryWindow();
	return true;
}

bool CNetworkStreamGameItemPhase::RecvItemGroundAddPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemGroundAdd packet_item_ground_add = value.Get<TPacketGCItemGroundAdd>();

	m_rkStream.__GlobalPositionToLocalPosition(packet_item_ground_add.lX, packet_item_ground_add.lY);

	CPythonItem::Instance().CreateItem(packet_item_ground_add.dwVID,
									   packet_item_ground_add.dwVnum,
									   packet_item_ground_add.lX,
									   packet_item_ground_add.lY,
									   packet_item_ground_add.lZ);
	return true;
}

bool CNetworkStreamGameItemPhase::RecvItemOwnership(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemOwnership ownership = value.Get<TPacketGCItemOwnership>();

	CPythonItem::Instance().SetOwnership(ownership.vid, ownership.name);
	return true;
}

bool CNetworkStreamGameItemPhase::RecvItemGroundDelPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCItemGroundDel packet_item_ground_del = value.Get<TPacketGCItemGroundDel>();

	CPythonItem::Instance().DeleteItem(packet_item_ground_del.vid);
	return true;
}

bool CNetworkStreamGameItemPhase::RecvQuickSlotAddPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCQuickSlotAdd packet = value.Get<TPacketGCQuickSlotAdd>();

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.AddQuickSlot(packet.position, static_cast<char>(packet.type), static_cast<char>(packet.slotPosition));

	m_rkStream.__RefreshInventoryWindow();

	return true;
}

bool CNetworkStreamGameItemPhase::RecvQuickSlotDelPacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCQuickSlotDel packet = value.Get<TPacketGCQuickSlotDel>();

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.DeleteQuickSlot(packet.position);

	m_rkStream.__RefreshInventoryWindow();

	return true;
}

bool CNetworkStreamGameItemPhase::RecvQuickSlotMovePacket(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCQuickSlotSwap packet_quick_slot_swap = value.Get<TPacketGCQuickSlotSwap>();

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.MoveQuickSlot(packet_quick_slot_swap.pos, packet_quick_slot_swap.change_pos);

	m_rkStream.__RefreshInventoryWindow();

	return true;
}

bool CPythonNetworkStream::SendShopEndPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop packet_shop;
	packet_shop.header = HEADER_CG_SHOP;
	packet_shop.subheader = SHOP_SUBHEADER_CG_END;

	if (!Send(sizeof(packet_shop), &packet_shop))
	{
		Tracef("SendShopEndPacket Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendShopBuyPacket(uint8_t bPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_BUY;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendShopBuyPacket Error\n");
		return false;
	}

	uint8_t bCount = 1;
	if (!Send(sizeof(uint8_t), &bCount))
	{
		Tracef("SendShopBuyPacket Error\n");
		return false;
	}

	if (!Send(sizeof(uint8_t), &bPos))
	{
		Tracef("SendShopBuyPacket Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendShopSellPacket(uint8_t bySlot)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_SELL;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendShopSellPacket Error\n");
		return false;
	}
	if (!Send(sizeof(uint8_t), &bySlot))
	{
		Tracef("SendShopAddSellPacket Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendShopSellPacketNew(uint8_t bySlot, uint8_t byCount)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_SELL2;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendShopSellPacket Error\n");
		return false;
	}
	if (!Send(sizeof(uint8_t), &bySlot))
	{
		Tracef("SendShopAddSellPacket Error\n");
		return false;
	}
	if (!Send(sizeof(uint8_t), &byCount))
	{
		Tracef("SendShopAddSellPacket Error\n");
		return false;
	}

	Tracef(" SendShopSellPacketNew(bySlot=%d, byCount=%d)\n", bySlot, byCount);

	return SendSequence();
}

// Send
bool CPythonNetworkStream::SendItemUsePacket(TItemPos pos)
{
	if (!__CanActMainInstance())
		return true;

	if (__IsEquipItemInSlot(pos))
	{
		if (CPythonExchange::Instance().isTrading())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_EXCHANGE"));
			return true;
		}

		if (CPythonShop::Instance().IsOpen())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_SHOP"));
			return true;
		}

		if (__IsPlayerAttacking())
			return true;
	}

	__PlayInventoryItemUseSound(pos);

	TPacketCGItemUse itemUsePacket;
	itemUsePacket.header = HEADER_CG_ITEM_USE;
	itemUsePacket.pos = pos;

	if (!Send(sizeof(TPacketCGItemUse), &itemUsePacket))
	{
		Tracen("SendItemUsePacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendItemUseToItemPacket(TItemPos source_pos, TItemPos target_pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemUseToItem itemUseToItemPacket;
	itemUseToItemPacket.header = HEADER_CG_ITEM_USE_TO_ITEM;
	itemUseToItemPacket.source_pos = source_pos;
	itemUseToItemPacket.target_pos = target_pos;

	if (!Send(sizeof(TPacketCGItemUseToItem), &itemUseToItemPacket))
	{
		Tracen("SendItemUseToItemPacket Error");
		return false;
	}

#ifdef _DEBUG
	Tracef(" << SendItemUseToItemPacket(src=%d, dst=%d)\n", source_pos, target_pos);
#endif

	return SendSequence();
}

bool CPythonNetworkStream::SendItemDropPacket(TItemPos pos, uint32_t elk)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemDrop itemDropPacket;
	itemDropPacket.header = HEADER_CG_ITEM_DROP;
	itemDropPacket.pos = pos;
	itemDropPacket.elk = elk;

	if (!Send(sizeof(TPacketCGItemDrop), &itemDropPacket))
	{
		Tracen("SendItemDropPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendItemDropPacketNew(TItemPos pos, uint32_t elk, uint32_t count)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemDrop2 itemDropPacket;
	itemDropPacket.header = HEADER_CG_ITEM_DROP2;
	itemDropPacket.pos = pos;
	itemDropPacket.gold = elk;
	itemDropPacket.count = count;

	if (!Send(sizeof(itemDropPacket), &itemDropPacket))
	{
		Tracen("SendItemDropPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::__IsEquipItemInSlot(TItemPos uSlotPos)
{
	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	return rkPlayer.IsEquipItemInSlot(uSlotPos);
}

void CPythonNetworkStream::__PlayInventoryItemUseSound(TItemPos uSlotPos)
{
	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	uint32_t dwItemID = rkPlayer.GetItemIndex(uSlotPos);

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.PlayUseSound(dwItemID);
}

void CPythonNetworkStream::__PlayInventoryItemDropSound(TItemPos uSlotPos)
{
	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	uint32_t dwItemID = rkPlayer.GetItemIndex(uSlotPos);

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.PlayDropSound(dwItemID);
}

// void CPythonNetworkStream::__PlayShopItemDropSound(uint32_t uSlotPos)
//{
//	uint32_t dwItemID;
//	CPythonShop& rkShop=CPythonShop::Instance();
//	if (!rkShop.GetSlotItemID(uSlotPos, &dwItemID))
//		return;
//
//	CPythonItem& rkItem=CPythonItem::Instance();
//	rkItem.PlayDropSound(dwItemID);
// }

void CPythonNetworkStream::__PlaySafeBoxItemDropSound(uint32_t uSlotPos)
{
	uint32_t dwItemID;
	CPythonSafeBox& rkSafeBox = CPythonSafeBox::Instance();
	if (!rkSafeBox.GetSlotItemID(uSlotPos, &dwItemID))
		return;

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.PlayDropSound(dwItemID);
}

void CPythonNetworkStream::__PlayMallItemDropSound(uint32_t uSlotPos)
{
	uint32_t dwItemID;
	CPythonSafeBox& rkSafeBox = CPythonSafeBox::Instance();
	if (!rkSafeBox.GetSlotMallItemID(uSlotPos, &dwItemID))
		return;

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.PlayDropSound(dwItemID);
}

bool CPythonNetworkStream::SendItemMovePacket(TItemPos pos, TItemPos change_pos, uint8_t num)
{
	if (!__CanActMainInstance())
		return true;

	if (__IsEquipItemInSlot(pos))
	{
		if (CPythonExchange::Instance().isTrading())
		{
			if (pos.IsEquipCell() || change_pos.IsEquipCell())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_EXCHANGE"));
				return true;
			}
		}

		if (CPythonShop::Instance().IsOpen())
		{
			if (pos.IsEquipCell() || change_pos.IsEquipCell())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_SHOP"));
				return true;
			}
		}

		if (__IsPlayerAttacking())
			return true;
	}

	__PlayInventoryItemDropSound(pos);

	TPacketCGItemMove itemMovePacket;
	itemMovePacket.header = HEADER_CG_ITEM_MOVE;
	itemMovePacket.pos = pos;
	itemMovePacket.change_pos = change_pos;
	itemMovePacket.num = num;

	if (!Send(sizeof(TPacketCGItemMove), &itemMovePacket))
	{
		Tracen("SendItemMovePacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendItemPickUpPacket(uint32_t vid)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemPickUp itemPickUpPacket;
	itemPickUpPacket.header = HEADER_CG_ITEM_PICKUP;
	itemPickUpPacket.vid = vid;

	if (!Send(sizeof(TPacketCGItemPickUp), &itemPickUpPacket))
	{
		Tracen("SendItemPickUpPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendQuickSlotAddPacket(uint8_t wpos, uint8_t type, uint8_t pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGQuickSlotAdd quickSlotAddPacket;

	quickSlotAddPacket.header = HEADER_CG_QUICKSLOT_ADD;
	quickSlotAddPacket.pos = wpos;
	quickSlotAddPacket.slot.Type = type;
	quickSlotAddPacket.slot.Position = pos;

	if (!Send(sizeof(TPacketCGQuickSlotAdd), &quickSlotAddPacket))
	{
		Tracen("SendQuickSlotAddPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendQuickSlotDelPacket(uint8_t pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGQuickSlotDel quickSlotDelPacket;

	quickSlotDelPacket.header = HEADER_CG_QUICKSLOT_DEL;
	quickSlotDelPacket.pos = pos;

	if (!Send(sizeof(TPacketCGQuickSlotDel), &quickSlotDelPacket))
	{
		Tracen("SendQuickSlotDelPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendQuickSlotMovePacket(uint8_t pos, uint8_t change_pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGQuickSlotSwap quickSlotSwapPacket;

	quickSlotSwapPacket.header = HEADER_CG_QUICKSLOT_SWAP;
	quickSlotSwapPacket.pos = pos;
	quickSlotSwapPacket.change_pos = change_pos;

	if (!Send(sizeof(TPacketCGQuickSlotSwap), &quickSlotSwapPacket))
	{
		Tracen("SendQuickSlotSwapPacket Error");
		return false;
	}

	return SendSequence();
}

bool CNetworkStreamGameItemPhase::RecvSpecialEffect(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCSpecialEffect kSpecialEffect = value.Get<TPacketGCSpecialEffect>();

	uint32_t effect = -1;
	bool bPlayPotionSound = false; // 포션을 먹을 경우는 포션 사운드를 출력하자.!!
	bool bAttachEffect = true;	   // 캐리터에 붙는 어태치 이펙트와 일반 이펙트 구분.!!
	switch (kSpecialEffect.type)
	{
		case SE_HPUP_RED:
			effect = CInstanceBase::EFFECT_HPUP_RED;
			bPlayPotionSound = true;
			break;
		case SE_SPUP_BLUE:
			effect = CInstanceBase::EFFECT_SPUP_BLUE;
			bPlayPotionSound = true;
			break;
		case SE_SPEEDUP_GREEN:
			effect = CInstanceBase::EFFECT_SPEEDUP_GREEN;
			bPlayPotionSound = true;
			break;
		case SE_DXUP_PURPLE:
			effect = CInstanceBase::EFFECT_DXUP_PURPLE;
			bPlayPotionSound = true;
			break;
		case SE_CRITICAL:
			effect = CInstanceBase::EFFECT_CRITICAL;
			break;
		case SE_PENETRATE:
			effect = CInstanceBase::EFFECT_PENETRATE;
			break;
		case SE_BLOCK:
			effect = CInstanceBase::EFFECT_BLOCK;
			break;
		case SE_DODGE:
			effect = CInstanceBase::EFFECT_DODGE;
			break;
		case SE_CHINA_FIREWORK:
			effect = CInstanceBase::EFFECT_FIRECRACKER;
			bAttachEffect = false;
			break;
		case SE_SPIN_TOP:
			effect = CInstanceBase::EFFECT_SPIN_TOP;
			bAttachEffect = false;
			break;
		case SE_SUCCESS:
			effect = CInstanceBase::EFFECT_SUCCESS;
			bAttachEffect = false;
			break;
		case SE_FAIL:
			effect = CInstanceBase::EFFECT_FAIL;
			break;
		case SE_FR_SUCCESS:
			effect = CInstanceBase::EFFECT_FR_SUCCESS;
			bAttachEffect = false;
			break;
		case SE_LEVELUP_ON_14_FOR_GERMANY: // 레벨업 14일때 ( 독일전용 )
			effect = CInstanceBase::EFFECT_LEVELUP_ON_14_FOR_GERMANY;
			bAttachEffect = false;
			break;
		case SE_LEVELUP_UNDER_15_FOR_GERMANY: // 레벨업 15일때 ( 독일전용 )
			effect = CInstanceBase::EFFECT_LEVELUP_UNDER_15_FOR_GERMANY;
			bAttachEffect = false;
			break;
		case SE_PERCENT_DAMAGE1:
			effect = CInstanceBase::EFFECT_PERCENT_DAMAGE1;
			break;
		case SE_PERCENT_DAMAGE2:
			effect = CInstanceBase::EFFECT_PERCENT_DAMAGE2;
			break;
		case SE_PERCENT_DAMAGE3:
			effect = CInstanceBase::EFFECT_PERCENT_DAMAGE3;
			break;
		case SE_AUTO_HPUP:
			effect = CInstanceBase::EFFECT_AUTO_HPUP;
			break;
		case SE_AUTO_SPUP:
			effect = CInstanceBase::EFFECT_AUTO_SPUP;
			break;
		case SE_EQUIP_RAMADAN_RING:
			effect = CInstanceBase::EFFECT_RAMADAN_RING_EQUIP;
			break;
		case SE_EQUIP_HALLOWEEN_CANDY:
			effect = CInstanceBase::EFFECT_HALLOWEEN_CANDY_EQUIP;
			break;
		case SE_EQUIP_HAPPINESS_RING:
			effect = CInstanceBase::EFFECT_HAPPINESS_RING_EQUIP;
			break;
		case SE_EQUIP_LOVE_PENDANT:
			effect = CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP;
			break;

		default:
			TraceError("%d is a special effect number that does not exist. TPacketGCSpecialEffect", kSpecialEffect.type);
			break;
	}

	if (bPlayPotionSound)
	{
		IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
		if (rkPlayer.IsMainCharacterIndex(kSpecialEffect.vid))
		{
			CPythonItem& rkItem = CPythonItem::Instance();
			rkItem.PlayUsePotionSound();
		}
	}

	if (-1 != effect)
	{
		CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kSpecialEffect.vid);
		if (pInstance)
		{
			if (bAttachEffect)
				pInstance->GetEffect().AttachSpecialEffect(effect);
			else
				pInstance->GetEffect().CreateSpecialEffect(effect);
		}
	}

	return true;
}

bool CNetworkStreamGameItemPhase::RecvSpecificEffect(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCSpecificEffect kSpecificEffect = value.Get<TPacketGCSpecificEffect>();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kSpecificEffect.vid);
	// EFFECT_TEMP
	if (pInstance)
	{
		CInstanceBaseEffect::RegisterEffect(CInstanceBase::EFFECT_TEMP, "", kSpecificEffect.effect_file, false);
		pInstance->GetEffect().AttachSpecialEffect(CInstanceBase::EFFECT_TEMP);
	}

	return true;
}

bool CNetworkStreamGameItemPhase::RecvDragonSoulRefine(const OMT2Client::Protocol::Packet& value)
{
	TPacketGCDragonSoulRefine kDragonSoul = value.Get<TPacketGCDragonSoulRefine>();

	switch (kDragonSoul.bSubType)
	{
		case DS_SUB_HEADER_OPEN:
			PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "BINARY_DragonSoulRefineWindow_Open", Py_BuildValue("()"));
			break;
		case DS_SUB_HEADER_REFINE_FAIL:
		case DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE:
		case DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL:
		case DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY:
		case DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL:
		case DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL:
			PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "BINARY_DragonSoulRefineWindow_RefineFail", Py_BuildValue("(iii)", kDragonSoul.bSubType, kDragonSoul.Pos.window_type, kDragonSoul.Pos.cell));
			break;
		case DS_SUB_HEADER_REFINE_SUCCEED:
			PyCallClassMemberFunc(m_rkStream.m_apoPhaseWnd[CPythonNetworkStream::PHASE_WINDOW_GAME], "BINARY_DragonSoulRefineWindow_RefineSucceed",
								  Py_BuildValue("(ii)", kDragonSoul.Pos.window_type, kDragonSoul.Pos.cell));
			break;
	}

	return true;
}

CNetworkStreamGameItemPhase::CNetworkStreamGameItemPhase(CPythonNetworkStream& rkStream) : CNetworkStreamPhase(rkStream)
{
}

ENetworkStreamPhaseResult CNetworkStreamGameItemPhase::Handle(uint8_t header, const OMT2Client::Protocol::Packet& value)
{
	using Handler = bool (CNetworkStreamGameItemPhase::*)(const OMT2Client::Protocol::Packet&);

	struct FEntry
	{
			uint8_t header;
			Handler handler;
	};

	static const FEntry entries[] = {
		{HEADER_GC_DRAGON_SOUL_REFINE, &CNetworkStreamGameItemPhase::RecvDragonSoulRefine},
		{HEADER_GC_ITEM_GROUND_ADD, &CNetworkStreamGameItemPhase::RecvItemGroundAddPacket},
		{HEADER_GC_ITEM_GROUND_DEL, &CNetworkStreamGameItemPhase::RecvItemGroundDelPacket},
		{HEADER_GC_ITEM_OWNERSHIP, &CNetworkStreamGameItemPhase::RecvItemOwnership},
		{HEADER_GC_ITEM_SET, &CNetworkStreamGameItemPhase::RecvItemSetPacket},
		{HEADER_GC_ITEM_SET2, &CNetworkStreamGameItemPhase::RecvItemSetPacket2},
		{HEADER_GC_ITEM_UPDATE, &CNetworkStreamGameItemPhase::RecvItemUpdatePacket},
		{HEADER_GC_ITEM_USE, &CNetworkStreamGameItemPhase::RecvItemUsePacket},
		{HEADER_GC_MALL_DEL, &CNetworkStreamGameItemPhase::RecvMallItemDelPacket},
		{HEADER_GC_MALL_SET, &CNetworkStreamGameItemPhase::RecvMallItemSetPacket},
		{HEADER_GC_MALL_OPEN, &CNetworkStreamGameItemPhase::RecvMallOpenPacket},
		{HEADER_GC_QUICKSLOT_ADD, &CNetworkStreamGameItemPhase::RecvQuickSlotAddPacket},
		{HEADER_GC_QUICKSLOT_DEL, &CNetworkStreamGameItemPhase::RecvQuickSlotDelPacket},
		{HEADER_GC_QUICKSLOT_SWAP, &CNetworkStreamGameItemPhase::RecvQuickSlotMovePacket},
		{HEADER_GC_SAFEBOX_DEL, &CNetworkStreamGameItemPhase::RecvSafeBoxDelPacket},
		{HEADER_GC_SAFEBOX_MONEY_CHANGE, &CNetworkStreamGameItemPhase::RecvSafeBoxMoneyChangePacket},
		{HEADER_GC_SAFEBOX_SET, &CNetworkStreamGameItemPhase::RecvSafeBoxSetPacket},
		{HEADER_GC_SAFEBOX_SIZE, &CNetworkStreamGameItemPhase::RecvSafeBoxSizePacket},
		{HEADER_GC_SAFEBOX_WRONG_PASSWORD, &CNetworkStreamGameItemPhase::RecvSafeBoxWrongPasswordPacket},
		{HEADER_GC_SEPCIAL_EFFECT, &CNetworkStreamGameItemPhase::RecvSpecialEffect},
		{HEADER_GC_SPECIFIC_EFFECT, &CNetworkStreamGameItemPhase::RecvSpecificEffect},
	};

	for (const FEntry& entry : entries)
	{
		if (entry.header == header)
			return (this->*entry.handler)(value) ? ENetworkStreamPhaseResult::Handled : ENetworkStreamPhaseResult::Failed;
	}

	return ENetworkStreamPhaseResult::NotHandled;
}
