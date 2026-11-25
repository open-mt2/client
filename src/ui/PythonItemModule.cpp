#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonItem.h>

#include <Client/Game/ItemManager.h>
#include <Client/UI/InstanceBase.h>
#include <Client/UI/AbstractApplication.h>

extern int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;

ScriptObject* itemSetUseSoundFileName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iUseSound;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iUseSound))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &szFileName))
		return Script_BadArgument();

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.SetUseSoundFileName(iUseSound, szFileName);
	return Script_BuildNone();
}

ScriptObject* itemSetDropSoundFileName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iDropSound;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iDropSound))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &szFileName))
		return Script_BadArgument();

	CPythonItem& rkItem = CPythonItem::Instance();
	rkItem.SetDropSoundFileName(iDropSound, szFileName);
	return Script_BuildNone();
}

ScriptObject* itemSelectItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BadArgument();

	if (!CItemManager::Instance().SelectItemData(iIndex))
	{
		TraceError("Cannot find item by %d", iIndex);
		CItemManager::Instance().SelectItemData(60001);
	}

	return Script_BuildNone();
}

ScriptObject* itemGetItemName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("s", pItemData->GetName());
}

ScriptObject* itemGetItemDescription(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("s", pItemData->GetDescription());
}

ScriptObject* itemGetItemSummary(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("s", pItemData->GetSummary());
}

ScriptObject* itemGetIconImage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	//	if (CItemData::ITEM_TYPE_SKILLBOOK == pItemData->GetType())
	//	{
	//		char szItemName[64+1];
	//		_snprintf(szItemName, "d:/ymir work/ui/items/etc/book_%02d.sub", );
	//		CGraphicImage * pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(szItemName);
	//	}

	return Script_BuildHandle(pItemData->GetIconImage());
}

ScriptObject* itemGetIconImageFileName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	CGraphicSubImage* pImage = pItemData->GetIconImage();
	if (!pImage)
		return Script_BuildValue("s", "Noname");

	return Script_BuildValue("s", pImage->GetFileName());
}

ScriptObject* itemGetItemSize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("(ii)", 1, pItemData->GetSize());
}

ScriptObject* itemGetItemType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->GetType());
}

ScriptObject* itemGetItemSubType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->GetSubType());
}

ScriptObject* itemGetIBuyItemPrice(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->GetIBuyItemPrice());
}

ScriptObject* itemGetISellItemPrice(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->GetISellItemPrice());
}

ScriptObject* itemIsAntiFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BadArgument();

	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->IsAntiFlag(iFlag));
}

ScriptObject* itemIsFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BadArgument();

	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->IsFlag(iFlag));
}

ScriptObject* itemIsWearableFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BadArgument();

	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->IsWearableFlag(iFlag));
}

ScriptObject* itemIs1GoldItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("no selected item data");

	return Script_BuildValue("i", pItemData->IsFlag(CItemData::ITEM_FLAG_COUNT_PER_1GOLD));
}

ScriptObject* itemGetLimit(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iValueIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Script_BadArgument();

	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Not yet select item data");

	CItemData::TItemLimit ItemLimit;
	if (!pItemData->GetLimit(iValueIndex, &ItemLimit))
		return Script_BuildException();

	return Script_BuildValue("ii", ItemLimit.bType, ItemLimit.lValue);
}

ScriptObject* itemGetAffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iValueIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Script_BadArgument();

	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Not yet select item data");

	CItemData::TItemApply ItemApply;
	if (!pItemData->GetApply(iValueIndex, &ItemApply))
		return Script_BuildException();

	if ((CItemData::APPLY_ATT_SPEED == ItemApply.bType) && (CItemData::ITEM_TYPE_WEAPON == pItemData->GetType()) && (CItemData::WEAPON_TWO_HANDED == pItemData->GetSubType()))
	{
		ItemApply.lValue -= TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;
	}

	return Script_BuildValue("ii", ItemApply.bType, ItemApply.lValue);
}

ScriptObject* itemGetValue(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iValueIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Script_BadArgument();

	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Not yet select item data");

	return Script_BuildValue("i", pItemData->GetValue(iValueIndex));
}

ScriptObject* itemGetSocket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iValueIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Script_BadArgument();

	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Not yet select item data");

	return Script_BuildValue("i", pItemData->GetSocket(iValueIndex));
}

ScriptObject* itemGetIconInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Not yet select item data");

	CGraphicSubImage* pImage = pItemData->GetIconImage();
	if (!pImage)
		return Script_BuildException("Cannot get icon image by %d", pItemData->GetIndex());

	CGraphicImageInstance* pImageInstance = CGraphicImageInstance::New();
	pImageInstance->SetImagePointer(pImage);

	return Script_BuildHandle(pImageInstance);
}

ScriptObject* itemDeleteIconInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CGraphicImageInstance* pImageInstance;
	if (!ScriptTuple_GetHandle(poArgs, 0, &pImageInstance))
		return Script_BadArgument();

	CGraphicImageInstance::Delete(pImageInstance);

	return Script_BuildNone();
}

ScriptObject* itemIsEquipmentVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemVID))
		return Script_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Not yet select item data");

	return Script_BuildValue("i", pItemData->IsEquipment());
}

// 2005.05.20.myevan.통합 USE_TYPE 체크
ScriptObject* itemGetUseType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemVID))
		return Script_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find select item data");

	return Script_BuildValue("s", pItemData->GetUseTypeString());
}

ScriptObject* itemIsRefineScroll(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Script_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find select item data");

	if (pItemData->GetType() != CItemData::ITEM_TYPE_USE)
		return Script_BuildValue("i", FALSE);

	switch (pItemData->GetSubType())
	{
		case CItemData::USE_TUNING:
			return Script_BuildValue("i", TRUE);
			break;
	}

	return Script_BuildValue("i", FALSE);
}

ScriptObject* itemIsDetachScroll(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Script_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find select item data");

	int iType = pItemData->GetType();
	int iSubType = pItemData->GetSubType();
	if (iType == CItemData::ITEM_TYPE_USE)
		if (iSubType == CItemData::USE_DETACHMENT)
		{
			return Script_BuildValue("i", TRUE);
		}

	return Script_BuildValue("i", FALSE);
}

ScriptObject* itemCanAddToQuickSlotItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Script_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_USE == pItemData->GetType() || CItemData::ITEM_TYPE_QUEST == pItemData->GetType())
	{
		return Script_BuildValue("i", TRUE);
	}

	return Script_BuildValue("i", FALSE);
}

ScriptObject* itemIsKey(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Script_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_TREASURE_KEY == pItemData->GetType())
	{
		return Script_BuildValue("i", TRUE);
	}

	return Script_BuildValue("i", FALSE);
}

ScriptObject* itemIsMetin(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iItemIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Script_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_METIN == pItemData->GetType())
	{
		return Script_BuildValue("i", TRUE);
	}

	return Script_BuildValue("i", FALSE);
}

ScriptObject* itemRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonItem::Instance().Render();
	return Script_BuildNone();
}

ScriptObject* itemUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	long lMouseX = 0;
	long lMouseY = 0;
	UI::CWindowManager::Instance().GetMousePosition(lMouseX, lMouseY);

	POINT ptMouse;
	ptMouse.x = lMouseX;
	ptMouse.y = lMouseY;

	CPythonItem::Instance().Update(ptMouse);
	return Script_BuildNone();
}

ScriptObject* itemCreateItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BadArgument();
	int iVirtualNumber;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iVirtualNumber))
		return Script_BadArgument();

	float x;
	if (!ScriptTuple_GetFloat(poArgs, 2, &x))
		return Script_BadArgument();
	float y;
	if (!ScriptTuple_GetFloat(poArgs, 3, &y))
		return Script_BadArgument();
	float z;
	if (!ScriptTuple_GetFloat(poArgs, 4, &z))
		return Script_BadArgument();

	bool bDrop = true;
	ScriptTuple_GetBoolean(poArgs, 5, &bDrop);

	CPythonItem::Instance().CreateItem(iVirtualID, iVirtualNumber, x, y, z, bDrop);

	return Script_BuildNone();
}

ScriptObject* itemDeleteItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BadArgument();

	CPythonItem::Instance().DeleteItem(iVirtualID);
	return Script_BuildNone();
}

ScriptObject* itemPick(ScriptObject* poSelf, ScriptObject* poArgs)
{
	DWORD dwItemID;
	if (CPythonItem::Instance().GetPickedItemID(&dwItemID))
		return Script_BuildValue("i", dwItemID);
	else
		return Script_BuildValue("i", -1);
}

ScriptObject* itemLoadItemTable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BadArgument();

	CItemManager::Instance().LoadItemTable(szFileName);
	return Script_BuildNone();
}

void initItem()
{
	static ScriptMethodDef s_methods[] =
		{
			{"SetUseSoundFileName", itemSetUseSoundFileName, SCRIPT_VARARGS, NULL},
			{"SetDropSoundFileName", itemSetDropSoundFileName, SCRIPT_VARARGS, NULL},
			{"SelectItem", itemSelectItem, SCRIPT_VARARGS, NULL},

			{"GetItemName", itemGetItemName, SCRIPT_VARARGS, NULL},
			{"GetItemDescription", itemGetItemDescription, SCRIPT_VARARGS, NULL},
			{"GetItemSummary", itemGetItemSummary, SCRIPT_VARARGS, NULL},
			{"GetIconImage", itemGetIconImage, SCRIPT_VARARGS, NULL},
			{"GetIconImageFileName", itemGetIconImageFileName, SCRIPT_VARARGS, NULL},
			{"GetItemSize", itemGetItemSize, SCRIPT_VARARGS, NULL},
			{"GetItemType", itemGetItemType, SCRIPT_VARARGS, NULL},
			{"GetItemSubType", itemGetItemSubType, SCRIPT_VARARGS, NULL},
			{"GetIBuyItemPrice", itemGetIBuyItemPrice, SCRIPT_VARARGS, NULL},
			{"GetISellItemPrice", itemGetISellItemPrice, SCRIPT_VARARGS, NULL},
			{"IsAntiFlag", itemIsAntiFlag, SCRIPT_VARARGS, NULL},
			{"IsFlag", itemIsFlag, SCRIPT_VARARGS, NULL},
			{"IsWearableFlag", itemIsWearableFlag, SCRIPT_VARARGS, NULL},
			{"Is1GoldItem", itemIs1GoldItem, SCRIPT_VARARGS, NULL},
			{"GetLimit", itemGetLimit, SCRIPT_VARARGS, NULL},
			{"GetAffect", itemGetAffect, SCRIPT_VARARGS, NULL},
			{"GetValue", itemGetValue, SCRIPT_VARARGS, NULL},
			{"GetSocket", itemGetSocket, SCRIPT_VARARGS, NULL},
			{"GetIconInstance", itemGetIconInstance, SCRIPT_VARARGS, NULL},
			{"GetUseType", itemGetUseType, SCRIPT_VARARGS, NULL},
			{"DeleteIconInstance", itemDeleteIconInstance, SCRIPT_VARARGS, NULL},
			{"IsEquipmentVID", itemIsEquipmentVID, SCRIPT_VARARGS, NULL},
			{"IsRefineScroll", itemIsRefineScroll, SCRIPT_VARARGS, NULL},
			{"IsDetachScroll", itemIsDetachScroll, SCRIPT_VARARGS, NULL},
			{"IsKey", itemIsKey, SCRIPT_VARARGS, NULL},
			{"IsMetin", itemIsMetin, SCRIPT_VARARGS, NULL},
			{"CanAddToQuickSlotItem", itemCanAddToQuickSlotItem, SCRIPT_VARARGS, NULL},

			{"Update", itemUpdate, SCRIPT_VARARGS, NULL},
			{"Render", itemRender, SCRIPT_VARARGS, NULL},
			{"CreateItem", itemCreateItem, SCRIPT_VARARGS, NULL},
			{"DeleteItem", itemDeleteItem, SCRIPT_VARARGS, NULL},
			{"Pick", itemPick, SCRIPT_VARARGS, NULL},

			{"LoadItemTable", itemLoadItemTable, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("item", s_methods);

	ScriptModule_AddIntConstant(poModule, "USESOUND_ACCESSORY", CPythonItem::USESOUND_ACCESSORY);
	ScriptModule_AddIntConstant(poModule, "USESOUND_ARMOR", CPythonItem::USESOUND_ARMOR);
	ScriptModule_AddIntConstant(poModule, "USESOUND_BOW", CPythonItem::USESOUND_BOW);
	ScriptModule_AddIntConstant(poModule, "USESOUND_DEFAULT", CPythonItem::USESOUND_DEFAULT);
	ScriptModule_AddIntConstant(poModule, "USESOUND_WEAPON", CPythonItem::USESOUND_WEAPON);
	ScriptModule_AddIntConstant(poModule, "USESOUND_POTION", CPythonItem::USESOUND_POTION);
	ScriptModule_AddIntConstant(poModule, "USESOUND_PORTAL", CPythonItem::USESOUND_PORTAL);

	ScriptModule_AddIntConstant(poModule, "DROPSOUND_ACCESSORY", CPythonItem::DROPSOUND_ACCESSORY);
	ScriptModule_AddIntConstant(poModule, "DROPSOUND_ARMOR", CPythonItem::DROPSOUND_ARMOR);
	ScriptModule_AddIntConstant(poModule, "DROPSOUND_BOW", CPythonItem::DROPSOUND_BOW);
	ScriptModule_AddIntConstant(poModule, "DROPSOUND_DEFAULT", CPythonItem::DROPSOUND_DEFAULT);
	ScriptModule_AddIntConstant(poModule, "DROPSOUND_WEAPON", CPythonItem::DROPSOUND_WEAPON);

	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_COUNT", c_Equipment_Count);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_HEAD", c_Equipment_Head);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_BODY", c_Equipment_Body);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_WEAPON", c_Equipment_Weapon);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_WRIST", c_Equipment_Wrist);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_SHOES", c_Equipment_Shoes);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_NECK", c_Equipment_Neck);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_EAR", c_Equipment_Ear);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE1", c_Equipment_Unique1);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE2", c_Equipment_Unique2);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_ARROW", c_Equipment_Arrow);

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_RING1", c_Equipment_Ring1);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_RING2", c_Equipment_Ring2);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_BELT", c_Equipment_Belt);
#endif

	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_NONE", CItemData::ITEM_TYPE_NONE);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_WEAPON", CItemData::ITEM_TYPE_WEAPON);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_ARMOR", CItemData::ITEM_TYPE_ARMOR);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_USE", CItemData::ITEM_TYPE_USE);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_AUTOUSE", CItemData::ITEM_TYPE_AUTOUSE);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_MATERIAL", CItemData::ITEM_TYPE_MATERIAL);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL", CItemData::ITEM_TYPE_SPECIAL);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_TOOL", CItemData::ITEM_TYPE_TOOL);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_LOTTERY", CItemData::ITEM_TYPE_LOTTERY);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_ELK", CItemData::ITEM_TYPE_ELK);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_METIN", CItemData::ITEM_TYPE_METIN);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_CONTAINER", CItemData::ITEM_TYPE_CONTAINER);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_FISH", CItemData::ITEM_TYPE_FISH);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_ROD", CItemData::ITEM_TYPE_ROD);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_RESOURCE", CItemData::ITEM_TYPE_RESOURCE);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_CAMPFIRE", CItemData::ITEM_TYPE_CAMPFIRE);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_UNIQUE", CItemData::ITEM_TYPE_UNIQUE);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_SKILLBOOK", CItemData::ITEM_TYPE_SKILLBOOK);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_QUEST", CItemData::ITEM_TYPE_QUEST);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_POLYMORPH", CItemData::ITEM_TYPE_POLYMORPH);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_BOX", CItemData::ITEM_TYPE_TREASURE_BOX);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_KEY", CItemData::ITEM_TYPE_TREASURE_KEY);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_PICK", CItemData::ITEM_TYPE_PICK);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_BLEND", CItemData::ITEM_TYPE_BLEND);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_DS", CItemData::ITEM_TYPE_DS);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL_DS", CItemData::ITEM_TYPE_SPECIAL_DS);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_RING", CItemData::ITEM_TYPE_RING);
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_BELT", CItemData::ITEM_TYPE_BELT);

#ifdef ENABLE_COSTUME_SYSTEM
	ScriptModule_AddIntConstant(poModule, "ITEM_TYPE_COSTUME", CItemData::ITEM_TYPE_COSTUME);

	// Item Sub Type
	ScriptModule_AddIntConstant(poModule, "COSTUME_TYPE_BODY", CItemData::COSTUME_BODY);
	ScriptModule_AddIntConstant(poModule, "COSTUME_TYPE_HAIR", CItemData::COSTUME_HAIR);

	// 인벤토리 및 장비창에서의 슬롯 번호
	ScriptModule_AddIntConstant(poModule, "COSTUME_SLOT_START", c_Costume_Slot_Start);
	ScriptModule_AddIntConstant(poModule, "COSTUME_SLOT_COUNT", c_Costume_Slot_Count);
	ScriptModule_AddIntConstant(poModule, "COSTUME_SLOT_BODY", c_Costume_Slot_Body);
	ScriptModule_AddIntConstant(poModule, "COSTUME_SLOT_HAIR", c_Costume_Slot_Hair);
	ScriptModule_AddIntConstant(poModule, "COSTUME_SLOT_END", c_Costume_Slot_End);
#endif

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	ScriptModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_START", c_Belt_Inventory_Slot_Start);
	ScriptModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_COUNT", c_Belt_Inventory_Slot_Count);
	ScriptModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_END", c_Belt_Inventory_Slot_End);

#endif

	ScriptModule_AddIntConstant(poModule, "WEAPON_SWORD", CItemData::WEAPON_SWORD);
	ScriptModule_AddIntConstant(poModule, "WEAPON_DAGGER", CItemData::WEAPON_DAGGER);
	ScriptModule_AddIntConstant(poModule, "WEAPON_BOW", CItemData::WEAPON_BOW);
	ScriptModule_AddIntConstant(poModule, "WEAPON_TWO_HANDED", CItemData::WEAPON_TWO_HANDED);
	ScriptModule_AddIntConstant(poModule, "WEAPON_BELL", CItemData::WEAPON_BELL);
	ScriptModule_AddIntConstant(poModule, "WEAPON_FAN", CItemData::WEAPON_FAN);
	ScriptModule_AddIntConstant(poModule, "WEAPON_ARROW", CItemData::WEAPON_ARROW);
	ScriptModule_AddIntConstant(poModule, "WEAPON_NUM_TYPES", CItemData::WEAPON_NUM_TYPES);

	ScriptModule_AddIntConstant(poModule, "USE_POTION", CItemData::USE_POTION);
	ScriptModule_AddIntConstant(poModule, "USE_TALISMAN", CItemData::USE_TALISMAN);
	ScriptModule_AddIntConstant(poModule, "USE_TUNING", CItemData::USE_TUNING);
	ScriptModule_AddIntConstant(poModule, "USE_MOVE", CItemData::USE_MOVE);
	ScriptModule_AddIntConstant(poModule, "USE_TREASURE_BOX", CItemData::USE_TREASURE_BOX);
	ScriptModule_AddIntConstant(poModule, "USE_MONEYBAG", CItemData::USE_MONEYBAG);
	ScriptModule_AddIntConstant(poModule, "USE_BAIT", CItemData::USE_BAIT);
	ScriptModule_AddIntConstant(poModule, "USE_ABILITY_UP", CItemData::USE_ABILITY_UP);
	ScriptModule_AddIntConstant(poModule, "USE_AFFECT", CItemData::USE_AFFECT);
	ScriptModule_AddIntConstant(poModule, "USE_CREATE_STONE", CItemData::USE_CREATE_STONE);
	ScriptModule_AddIntConstant(poModule, "USE_SPECIAL", CItemData::USE_SPECIAL);
	ScriptModule_AddIntConstant(poModule, "USE_POTION_NODELAY", CItemData::USE_POTION_NODELAY);
	ScriptModule_AddIntConstant(poModule, "USE_CLEAR", CItemData::USE_CLEAR);
	ScriptModule_AddIntConstant(poModule, "USE_INVISIBILITY", CItemData::USE_INVISIBILITY);
	ScriptModule_AddIntConstant(poModule, "USE_DETACHMENT", CItemData::USE_DETACHMENT);
	ScriptModule_AddIntConstant(poModule, "USE_TIME_CHARGE_PER", CItemData::USE_TIME_CHARGE_PER);
	ScriptModule_AddIntConstant(poModule, "USE_TIME_CHARGE_FIX", CItemData::USE_TIME_CHARGE_FIX);

	ScriptModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_NORMAL", CItemData::MATERIAL_DS_REFINE_NORMAL);
	ScriptModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_BLESSED", CItemData::MATERIAL_DS_REFINE_BLESSED);
	ScriptModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_HOLLY", CItemData::MATERIAL_DS_REFINE_HOLLY);

	ScriptModule_AddIntConstant(poModule, "METIN_NORMAL", CItemData::METIN_NORMAL);
	ScriptModule_AddIntConstant(poModule, "METIN_GOLD", CItemData::METIN_GOLD);

	ScriptModule_AddIntConstant(poModule, "LIMIT_NONE", CItemData::LIMIT_NONE);
	ScriptModule_AddIntConstant(poModule, "LIMIT_LEVEL", CItemData::LIMIT_LEVEL);
	ScriptModule_AddIntConstant(poModule, "LIMIT_STR", CItemData::LIMIT_STR);
	ScriptModule_AddIntConstant(poModule, "LIMIT_DEX", CItemData::LIMIT_DEX);
	ScriptModule_AddIntConstant(poModule, "LIMIT_INT", CItemData::LIMIT_INT);
	ScriptModule_AddIntConstant(poModule, "LIMIT_CON", CItemData::LIMIT_CON);
	ScriptModule_AddIntConstant(poModule, "LIMIT_PCBANG", CItemData::LIMIT_PCBANG);
	ScriptModule_AddIntConstant(poModule, "LIMIT_REAL_TIME", CItemData::LIMIT_REAL_TIME);
	ScriptModule_AddIntConstant(poModule, "LIMIT_REAL_TIME_START_FIRST_USE", CItemData::LIMIT_REAL_TIME_START_FIRST_USE);
	ScriptModule_AddIntConstant(poModule, "LIMIT_TIMER_BASED_ON_WEAR", CItemData::LIMIT_TIMER_BASED_ON_WEAR);
	ScriptModule_AddIntConstant(poModule, "LIMIT_TYPE_MAX_NUM", CItemData::LIMIT_MAX_NUM);
	ScriptModule_AddIntConstant(poModule, "LIMIT_MAX_NUM", CItemData::ITEM_LIMIT_MAX_NUM);

	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_FEMALE", CItemData::ITEM_ANTIFLAG_FEMALE);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MALE", CItemData::ITEM_ANTIFLAG_MALE);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_WARRIOR", CItemData::ITEM_ANTIFLAG_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_ASSASSIN", CItemData::ITEM_ANTIFLAG_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SURA", CItemData::ITEM_ANTIFLAG_SURA);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SHAMAN", CItemData::ITEM_ANTIFLAG_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GET", CItemData::ITEM_ANTIFLAG_GET);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_DROP", CItemData::ITEM_ANTIFLAG_DROP);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SELL", CItemData::ITEM_ANTIFLAG_SELL);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_A", CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_B", CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_R", CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SAVE", CItemData::ITEM_ANTIFLAG_SAVE);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GIVE", CItemData::ITEM_ANTIFLAG_GIVE);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_PKDROP", CItemData::ITEM_ANTIFLAG_PKDROP);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_STACK", CItemData::ITEM_ANTIFLAG_STACK);
	ScriptModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MYSHOP", CItemData::ITEM_ANTIFLAG_MYSHOP);

	ScriptModule_AddIntConstant(poModule, "ITEM_FLAG_RARE", CItemData::ITEM_FLAG_RARE);
	ScriptModule_AddIntConstant(poModule, "ITEM_FLAG_UNIQUE", CItemData::ITEM_FLAG_UNIQUE);
	ScriptModule_AddIntConstant(poModule, "ITEM_FLAG_CONFIRM_WHEN_USE", CItemData::ITEM_FLAG_CONFIRM_WHEN_USE);

	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_FEMALE", CItemData::ITEM_ANTIFLAG_FEMALE);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_MALE", CItemData::ITEM_ANTIFLAG_MALE);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_WARRIOR", CItemData::ITEM_ANTIFLAG_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_ASSASSIN", CItemData::ITEM_ANTIFLAG_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_SURA", CItemData::ITEM_ANTIFLAG_SURA);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_SHAMAN", CItemData::ITEM_ANTIFLAG_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_GET", CItemData::ITEM_ANTIFLAG_GET);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_DROP", CItemData::ITEM_ANTIFLAG_DROP);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_SELL", CItemData::ITEM_ANTIFLAG_SELL);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_A", CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_B", CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_R", CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_SAVE", CItemData::ITEM_ANTIFLAG_SAVE);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_GIVE", CItemData::ITEM_ANTIFLAG_GIVE);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_PKDROP", CItemData::ITEM_ANTIFLAG_PKDROP);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_STACK", CItemData::ITEM_ANTIFLAG_STACK);
	ScriptModule_AddIntConstant(poModule, "ANTIFLAG_MYSHOP", CItemData::ITEM_ANTIFLAG_MYSHOP);

	ScriptModule_AddIntConstant(poModule, "WEARABLE_BODY", CItemData::WEARABLE_BODY);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_HEAD", CItemData::WEARABLE_HEAD);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_FOOTS", CItemData::WEARABLE_FOOTS);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_WRIST", CItemData::WEARABLE_WRIST);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_WEAPON", CItemData::WEARABLE_WEAPON);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_NECK", CItemData::WEARABLE_NECK);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_EAR", CItemData::WEARABLE_EAR);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_UNIQUE", CItemData::WEARABLE_UNIQUE);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_SHIELD", CItemData::WEARABLE_SHIELD);
	ScriptModule_AddIntConstant(poModule, "WEARABLE_ARROW", CItemData::WEARABLE_ARROW);

	ScriptModule_AddIntConstant(poModule, "ARMOR_BODY", CItemData::ARMOR_BODY);
	ScriptModule_AddIntConstant(poModule, "ARMOR_HEAD", CItemData::ARMOR_HEAD);
	ScriptModule_AddIntConstant(poModule, "ARMOR_SHIELD", CItemData::ARMOR_SHIELD);
	ScriptModule_AddIntConstant(poModule, "ARMOR_WRIST", CItemData::ARMOR_WRIST);
	ScriptModule_AddIntConstant(poModule, "ARMOR_FOOTS", CItemData::ARMOR_FOOTS);
	ScriptModule_AddIntConstant(poModule, "ARMOR_NECK", CItemData::ARMOR_NECK);
	ScriptModule_AddIntConstant(poModule, "ARMOR_EAR", CItemData::ARMOR_EAR);

	ScriptModule_AddIntConstant(poModule, "ITEM_APPLY_MAX_NUM", CItemData::ITEM_APPLY_MAX_NUM);
	ScriptModule_AddIntConstant(poModule, "ITEM_SOCKET_MAX_NUM", CItemData::ITEM_SOCKET_MAX_NUM);

	ScriptModule_AddIntConstant(poModule, "APPLY_NONE", CItemData::APPLY_NONE);
	ScriptModule_AddIntConstant(poModule, "APPLY_STR", CItemData::APPLY_STR);
	ScriptModule_AddIntConstant(poModule, "APPLY_DEX", CItemData::APPLY_DEX);
	ScriptModule_AddIntConstant(poModule, "APPLY_CON", CItemData::APPLY_CON);
	ScriptModule_AddIntConstant(poModule, "APPLY_INT", CItemData::APPLY_INT);
	ScriptModule_AddIntConstant(poModule, "APPLY_MAX_HP", CItemData::APPLY_MAX_HP);
	ScriptModule_AddIntConstant(poModule, "APPLY_MAX_SP", CItemData::APPLY_MAX_SP);
	ScriptModule_AddIntConstant(poModule, "APPLY_HP_REGEN", CItemData::APPLY_HP_REGEN);
	ScriptModule_AddIntConstant(poModule, "APPLY_SP_REGEN", CItemData::APPLY_SP_REGEN);
	ScriptModule_AddIntConstant(poModule, "APPLY_DEF_GRADE_BONUS", CItemData::APPLY_DEF_GRADE_BONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATT_GRADE_BONUS", CItemData::APPLY_ATT_GRADE_BONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATT_SPEED", CItemData::APPLY_ATT_SPEED);
	ScriptModule_AddIntConstant(poModule, "APPLY_MOV_SPEED", CItemData::APPLY_MOV_SPEED);
	ScriptModule_AddIntConstant(poModule, "APPLY_CAST_SPEED", CItemData::APPLY_CAST_SPEED);
	ScriptModule_AddIntConstant(poModule, "APPLY_MAGIC_ATT_GRADE", CItemData::APPLY_MAGIC_ATT_GRADE);
	ScriptModule_AddIntConstant(poModule, "APPLY_MAGIC_DEF_GRADE", CItemData::APPLY_MAGIC_DEF_GRADE);
	ScriptModule_AddIntConstant(poModule, "APPLY_SKILL", CItemData::APPLY_SKILL);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ANIMAL", CItemData::APPLY_ATTBONUS_ANIMAL);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD", CItemData::APPLY_ATTBONUS_UNDEAD);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL", CItemData::APPLY_ATTBONUS_DEVIL);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_HUMAN", CItemData::APPLY_ATTBONUS_HUMAN);
	ScriptModule_AddIntConstant(poModule, "APPLY_BOW_DISTANCE", CItemData::APPLY_BOW_DISTANCE);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_BOW", CItemData::APPLY_RESIST_BOW);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_FIRE", CItemData::APPLY_RESIST_FIRE);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_ELEC", CItemData::APPLY_RESIST_ELEC);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_MAGIC", CItemData::APPLY_RESIST_MAGIC);
	ScriptModule_AddIntConstant(poModule, "APPLY_POISON_PCT", CItemData::APPLY_POISON_PCT);
	ScriptModule_AddIntConstant(poModule, "APPLY_SLOW_PCT", CItemData::APPLY_SLOW_PCT);
	ScriptModule_AddIntConstant(poModule, "APPLY_STUN_PCT", CItemData::APPLY_STUN_PCT);
	ScriptModule_AddIntConstant(poModule, "APPLY_CRITICAL_PCT", CItemData::APPLY_CRITICAL_PCT);			  // n% 확률로 두배 타격
	ScriptModule_AddIntConstant(poModule, "APPLY_PENETRATE_PCT", CItemData::APPLY_PENETRATE_PCT);		  // n% 확률로 적의 방어력 무시
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ORC", CItemData::APPLY_ATTBONUS_ORC);			  // 웅귀에게 n% 추가 데미지
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MILGYO", CItemData::APPLY_ATTBONUS_MILGYO);	  // 밀교에게 n% 추가 데미지
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD", CItemData::APPLY_ATTBONUS_UNDEAD);	  // 시체에게 n% 추가 데미지
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL", CItemData::APPLY_ATTBONUS_DEVIL);		  // 악마에게 n% 추가 데미지
	ScriptModule_AddIntConstant(poModule, "APPLY_STEAL_HP", CItemData::APPLY_STEAL_HP);					  // n% 확률로 타격의 10% 를 생명력으로 흡수
	ScriptModule_AddIntConstant(poModule, "APPLY_STEAL_SP", CItemData::APPLY_STEAL_SP);					  // n% 확률로 타격의 10% 를 정신력으로 흡수
	ScriptModule_AddIntConstant(poModule, "APPLY_MANA_BURN_PCT", CItemData::APPLY_MANA_BURN_PCT);		  // n% 확률로 상대의 마나를 깎는다
	ScriptModule_AddIntConstant(poModule, "APPLY_DAMAGE_SP_RECOVER", CItemData::APPLY_DAMAGE_SP_RECOVER); // n% 확률로 정신력 2 회복
	ScriptModule_AddIntConstant(poModule, "APPLY_BLOCK", CItemData::APPLY_BLOCK);						  // n% 확률로 물리공격 완벽 방어
	ScriptModule_AddIntConstant(poModule, "APPLY_DODGE", CItemData::APPLY_DODGE);						  // n% 확률로 물리공격 완벽 회피
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_SWORD", CItemData::APPLY_RESIST_SWORD);			  // 한손검에 의한 피해를 n% 감소
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_TWOHAND", CItemData::APPLY_RESIST_TWOHAND);		  // 양손검에 의한 피해를 n% 감소
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_DAGGER", CItemData::APPLY_RESIST_DAGGER);		  // 단도에 의한 피해를 n% 감소
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_BELL", CItemData::APPLY_RESIST_BELL);			  // 방울에 의한 피해를 n% 감소
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_FAN", CItemData::APPLY_RESIST_FAN);				  // 부채에 의한 피해를 n% 감소
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_WIND", CItemData::APPLY_RESIST_WIND);			  // 바람에 의한 피해를 n% 감소
	ScriptModule_AddIntConstant(poModule, "APPLY_REFLECT_MELEE", CItemData::APPLY_REFLECT_MELEE);		  // 근접 타격 n% 를 적에게 되돌린다
	ScriptModule_AddIntConstant(poModule, "APPLY_REFLECT_CURSE", CItemData::APPLY_REFLECT_CURSE);		  // 적이 나에게 저주 사용시 n% 확률로 되돌린다
	ScriptModule_AddIntConstant(poModule, "APPLY_POISON_REDUCE", CItemData::APPLY_POISON_REDUCE);		  // 독에 의한 데미지 감소
	ScriptModule_AddIntConstant(poModule, "APPLY_KILL_SP_RECOVER", CItemData::APPLY_KILL_SP_RECOVER);	  // 적을 죽였을때 n% 확률로 정신력 10 회복
	ScriptModule_AddIntConstant(poModule, "APPLY_EXP_DOUBLE_BONUS", CItemData::APPLY_EXP_DOUBLE_BONUS);	  // n% 확률로 경험치 획득량 2배
	ScriptModule_AddIntConstant(poModule, "APPLY_GOLD_DOUBLE_BONUS", CItemData::APPLY_GOLD_DOUBLE_BONUS); // n% 확률로 돈 획득량 2배
	ScriptModule_AddIntConstant(poModule, "APPLY_ITEM_DROP_BONUS", CItemData::APPLY_ITEM_DROP_BONUS);	  // n% 확률로 아이템 획득량 2배
	ScriptModule_AddIntConstant(poModule, "APPLY_POTION_BONUS", CItemData::APPLY_POTION_BONUS);			  // 물약 복용시 n% 만큼 성능 증대
	ScriptModule_AddIntConstant(poModule, "APPLY_KILL_HP_RECOVER", CItemData::APPLY_KILL_HP_RECOVER);	  // 죽일때마다 생명력 회복
	ScriptModule_AddIntConstant(poModule, "APPLY_IMMUNE_STUN", CItemData::APPLY_IMMUNE_STUN);			  // 기절 하지 않는다
	ScriptModule_AddIntConstant(poModule, "APPLY_IMMUNE_SLOW", CItemData::APPLY_IMMUNE_SLOW);			  // 느려지지 않는다
	ScriptModule_AddIntConstant(poModule, "APPLY_IMMUNE_FALL", CItemData::APPLY_IMMUNE_FALL);			  // 넘어지지 않는다
	ScriptModule_AddIntConstant(poModule, "APPLY_MAX_STAMINA", CItemData::APPLY_MAX_STAMINA);			  // 최대 스테미너 증가
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WARRIOR", CItemData::APPLY_ATT_BONUS_TO_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ASSASSIN", CItemData::APPLY_ATT_BONUS_TO_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SURA", CItemData::APPLY_ATT_BONUS_TO_SURA);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SHAMAN", CItemData::APPLY_ATT_BONUS_TO_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MONSTER", CItemData::APPLY_ATT_BONUS_TO_MONSTER);
	ScriptModule_AddIntConstant(poModule, "APPLY_MALL_ATTBONUS", CItemData::APPLY_MALL_ATTBONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_MALL_DEFBONUS", CItemData::APPLY_MALL_DEFBONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_MALL_EXPBONUS", CItemData::APPLY_MALL_EXPBONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_MALL_ITEMBONUS", CItemData::APPLY_MALL_ITEMBONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_MALL_GOLDBONUS", CItemData::APPLY_MALL_GOLDBONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_MAX_HP_PCT", CItemData::APPLY_MAX_HP_PCT);
	ScriptModule_AddIntConstant(poModule, "APPLY_MAX_SP_PCT", CItemData::APPLY_MAX_SP_PCT);
	ScriptModule_AddIntConstant(poModule, "APPLY_SKILL_DAMAGE_BONUS", CItemData::APPLY_SKILL_DAMAGE_BONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DAMAGE_BONUS", CItemData::APPLY_NORMAL_HIT_DAMAGE_BONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_SKILL_DEFEND_BONUS", CItemData::APPLY_SKILL_DEFEND_BONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DEFEND_BONUS", CItemData::APPLY_NORMAL_HIT_DEFEND_BONUS);

	ScriptModule_AddIntConstant(poModule, "APPLY_PC_BANG_EXP_BONUS", CItemData::APPLY_PC_BANG_EXP_BONUS);
	ScriptModule_AddIntConstant(poModule, "APPLY_PC_BANG_DROP_BONUS", CItemData::APPLY_PC_BANG_DROP_BONUS);

	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_WARRIOR", CItemData::APPLY_RESIST_WARRIOR);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_ASSASSIN", CItemData::APPLY_RESIST_ASSASSIN);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_SURA", CItemData::APPLY_RESIST_SURA);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_SHAMAN", CItemData::APPLY_RESIST_SHAMAN);
	ScriptModule_AddIntConstant(poModule, "APPLY_ENERGY", CItemData::APPLY_ENERGY); // 기력
	ScriptModule_AddIntConstant(poModule, "APPLY_COSTUME_ATTR_BONUS", CItemData::APPLY_COSTUME_ATTR_BONUS);

	ScriptModule_AddIntConstant(poModule, "APPLY_MAGIC_ATTBONUS_PER", CItemData::APPLY_MAGIC_ATTBONUS_PER);
	ScriptModule_AddIntConstant(poModule, "APPLY_MELEE_MAGIC_ATTBONUS_PER", CItemData::APPLY_MELEE_MAGIC_ATTBONUS_PER);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_ICE", CItemData::APPLY_RESIST_ICE);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_EARTH", CItemData::APPLY_RESIST_EARTH);
	ScriptModule_AddIntConstant(poModule, "APPLY_RESIST_DARK", CItemData::APPLY_RESIST_DARK);
	ScriptModule_AddIntConstant(poModule, "APPLY_ANTI_CRITICAL_PCT", CItemData::APPLY_ANTI_CRITICAL_PCT);
	ScriptModule_AddIntConstant(poModule, "APPLY_ANTI_PENETRATE_PCT", CItemData::APPLY_ANTI_PENETRATE_PCT);
}
