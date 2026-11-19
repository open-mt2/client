#include <Client/Protocol/Decode.h>
#include <Client/Protocol/Wire.h>

#include <cstring>

namespace
{
	constexpr uint8_t kQuickSlotAddHeader = 28;
	constexpr uint8_t kQuickSlotDelHeader = 29;
	constexpr uint8_t kItemOwnershipHeader = 31;
	constexpr uint8_t kTargetHeader = 63;

#pragma pack(push, 1)

	struct WireQuickSlotAdd
	{
			uint8_t header;
			uint8_t position;
			uint8_t type;
			uint8_t slotPosition;
	};

	struct WireQuickSlotDel
	{
			uint8_t header;
			uint8_t position;
	};

	struct WireItemOwnership
	{
			uint8_t header;
			uint32_t vid;
			char name[protocol::kCharacterNameMaxLen + 1];
	};

	struct WireTarget
	{
			uint8_t header;
			uint32_t vid;
			uint8_t hpPercent;
	};

#pragma pack(pop)

	static_assert(sizeof(WireQuickSlotAdd) == 4, "quickslot add wire size");
	static_assert(sizeof(WireQuickSlotDel) == 2, "quickslot del wire size");
	static_assert(sizeof(WireItemOwnership) == 30, "item ownership wire size");
	static_assert(sizeof(WireTarget) == 6, "target wire size");

	template <typename T>
	bool ReadWire(const void* bytes, std::size_t size, uint8_t header, T* wire)
	{
		if (size != sizeof(T))
			return false;

		std::memcpy(wire, bytes, sizeof(T));
		return wire->header == header;
	}

	bool DecodeQuickSlotAdd(const void* bytes, std::size_t size, protocol::Value* value)
	{
		WireQuickSlotAdd wire;

		if (!ReadWire(bytes, size, kQuickSlotAddHeader, &wire))
			return false;

		*value = protocol::QuickSlotAddValue{wire.position, wire.type, wire.slotPosition};
		return true;
	}

	bool DecodeQuickSlotDel(const void* bytes, std::size_t size, protocol::Value* value)
	{
		WireQuickSlotDel wire;

		if (!ReadWire(bytes, size, kQuickSlotDelHeader, &wire))
			return false;

		*value = protocol::QuickSlotDelValue{wire.position};
		return true;
	}

	bool DecodeItemOwnership(const void* bytes, std::size_t size, protocol::Value* value)
	{
		WireItemOwnership wire;

		if (!ReadWire(bytes, size, kItemOwnershipHeader, &wire))
			return false;

		protocol::ItemOwnershipValue decoded = {};
		decoded.vid = wire.vid;
		std::memcpy(decoded.name.data(), wire.name, sizeof(wire.name));

		*value = decoded;
		return true;
	}

	bool DecodeTarget(const void* bytes, std::size_t size, protocol::Value* value)
	{
		WireTarget wire;

		if (!ReadWire(bytes, size, kTargetHeader, &wire))
			return false;

		*value = protocol::TargetValue{wire.vid, wire.hpPercent};
		return true;
	}

	bool DecodeByHeader(uint8_t header, const void* bytes, std::size_t size, protocol::Value* value)
	{
		switch (header)
		{
			case kQuickSlotAddHeader:
				return DecodeQuickSlotAdd(bytes, size, value);

			case kQuickSlotDelHeader:
				return DecodeQuickSlotDel(bytes, size, value);

			case kItemOwnershipHeader:
				return DecodeItemOwnership(bytes, size, value);

			case kTargetHeader:
				return DecodeTarget(bytes, size, value);

			default:
				return false;
		}
	}

	const protocol::SizeEntry kSizeTable[] = {
		{kQuickSlotAddHeader, 4, false},
		{kQuickSlotDelHeader, 2, false},
		{kItemOwnershipHeader, 30, false},
		{kTargetHeader, 6, false},
		{HEADER_GC_EMPIRE, sizeof(TPacketGCEmpire), false},
		{HEADER_GC_WARP, sizeof(TPacketGCWarp), false},
		{HEADER_GC_SKILL_COOLTIME_END, sizeof(TPacketGCSkillCoolTimeEnd), false},
		{HEADER_GC_QUEST_INFO, sizeof(TPacketGCQuestInfo), true},
		{HEADER_GC_REQUEST_MAKE_GUILD, sizeof(TPacketGCBlank), false},
		{HEADER_GC_PVP, sizeof(TPacketGCPVP), false},
		{HEADER_GC_DUEL_START, sizeof(TPacketGCDuelStart), true},
		{HEADER_GC_CHARACTER_ADD, sizeof(TPacketGCCharacterAdd), false},
		{HEADER_GC_CHAR_ADDITIONAL_INFO, sizeof(TPacketGCCharacterAdditionalInfo), false},
		{HEADER_GC_CHARACTER_ADD2, sizeof(TPacketGCCharacterAdd2), false},
		{HEADER_GC_CHARACTER_UPDATE, sizeof(TPacketGCCharacterUpdate), false},
		{HEADER_GC_CHARACTER_UPDATE2, sizeof(TPacketGCCharacterUpdate2), false},
		{HEADER_GC_CHARACTER_DEL, sizeof(TPacketGCCharacterDelete), false},
		{HEADER_GC_CHARACTER_MOVE, sizeof(TPacketGCMove), false},
		{HEADER_GC_CHAT, sizeof(TPacketGCChat), true},
		{HEADER_GC_SYNC_POSITION, sizeof(TPacketGCC2C), true},
		{HEADER_GC_LOGIN_SUCCESS3, sizeof(TPacketGCLoginSuccess3), false},
		{HEADER_GC_LOGIN_SUCCESS4, sizeof(TPacketGCLoginSuccess4), false},
		{HEADER_GC_LOGIN_FAILURE, sizeof(TPacketGCLoginFailure), false},
		{HEADER_GC_PLAYER_CREATE_SUCCESS, sizeof(TPacketGCPlayerCreateSuccess), false},
		{HEADER_GC_PLAYER_CREATE_FAILURE, sizeof(TPacketGCCreateFailure), false},
		{HEADER_GC_PLAYER_DELETE_SUCCESS, sizeof(TPacketGCBlank), false},
		{HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID, sizeof(TPacketGCBlank), false},
		{HEADER_GC_STUN, sizeof(TPacketGCStun), false},
		{HEADER_GC_DEAD, sizeof(TPacketGCDead), false},
		{HEADER_GC_MAIN_CHARACTER, sizeof(TPacketGCMainCharacter), false},
		{HEADER_GC_MAIN_CHARACTER2_EMPIRE, sizeof(TPacketGCMainCharacter2_EMPIRE), false},
		{HEADER_GC_MAIN_CHARACTER3_BGM, sizeof(TPacketGCMainCharacter3_BGM), false},
		{HEADER_GC_MAIN_CHARACTER4_BGM_VOL, sizeof(TPacketGCMainCharacter4_BGM_VOL), false},
		{HEADER_GC_PLAYER_POINTS, sizeof(TPacketGCPoints), false},
		{HEADER_GC_PLAYER_POINT_CHANGE, sizeof(TPacketGCPointChange), false},
		{HEADER_GC_ITEM_SET, sizeof(TPacketGCItemSet), false},
		{HEADER_GC_ITEM_SET2, sizeof(TPacketGCItemSet2), false},
		{HEADER_GC_ITEM_USE, sizeof(TPacketGCItemUse), false},
		{HEADER_GC_ITEM_UPDATE, sizeof(TPacketGCItemUpdate), false},
		{HEADER_GC_ITEM_GROUND_ADD, sizeof(TPacketGCItemGroundAdd), false},
		{HEADER_GC_ITEM_GROUND_DEL, sizeof(TPacketGCItemGroundDel), false},
		{HEADER_GC_QUICKSLOT_SWAP, sizeof(TPacketGCQuickSlotSwap), false},
		{HEADER_GC_WHISPER, sizeof(TPacketGCWhisper), false},
		{HEADER_GC_CHARACTER_POSITION, sizeof(TPacketGCPosition), false},
		{HEADER_GC_MOTION, sizeof(TPacketGCMotion), false},
		{HEADER_GC_SHOP, sizeof(TPacketGCShop), true},
		{HEADER_GC_SHOP_SIGN, sizeof(TPacketGCShopSign), false},
		{HEADER_GC_EXCHANGE, sizeof(TPacketGCExchange), false},
		{HEADER_GC_PING, sizeof(TPacketGCPing), false},
		{HEADER_GC_SCRIPT, sizeof(TPacketGCScript), true},
		{HEADER_GC_QUEST_CONFIRM, sizeof(TPacketGCQuestConfirm), false},
		{HEADER_GC_MOUNT, sizeof(TPacketGCMount), false},
		{HEADER_GC_CHANGE_SPEED, sizeof(TPacketGCChangeSpeed), false},
		{HEADER_GC_HANDSHAKE, sizeof(TPacketGCHandshake), false},
		{HEADER_GC_HANDSHAKE_OK, sizeof(TPacketGCBlank), false},
		{HEADER_GC_OWNERSHIP, sizeof(TPacketGCOwnership), false},
		{HEADER_GC_CREATE_FLY, sizeof(TPacketGCCreateFly), false},
		{HEADER_GC_ADD_FLY_TARGETING, sizeof(TPacketGCFlyTargeting), false},
		{HEADER_GC_FLY_TARGETING, sizeof(TPacketGCFlyTargeting), false},
		{HEADER_GC_PHASE, sizeof(TPacketGCPhase), false},
		{HEADER_GC_SKILL_LEVEL, sizeof(TPacketGCSkillLevel), false},
		{HEADER_GC_SKILL_LEVEL_NEW, sizeof(TPacketGCSkillLevelNew), false},
		{HEADER_GC_MESSENGER, sizeof(TPacketGCMessenger), true},
		{HEADER_GC_GUILD, sizeof(TPacketGCGuild), true},
		{HEADER_GC_PARTY_INVITE, sizeof(TPacketGCPartyInvite), false},
		{HEADER_GC_PARTY_ADD, sizeof(TPacketGCPartyAdd), false},
		{HEADER_GC_PARTY_UPDATE, sizeof(TPacketGCPartyUpdate), false},
		{HEADER_GC_PARTY_REMOVE, sizeof(TPacketGCPartyRemove), false},
		{HEADER_GC_PARTY_LINK, sizeof(TPacketGCPartyLink), false},
		{HEADER_GC_PARTY_UNLINK, sizeof(TPacketGCPartyUnlink), false},
		{HEADER_GC_PARTY_PARAMETER, sizeof(TPacketGCPartyParameter), false},
		{HEADER_GC_SAFEBOX_SET, sizeof(TPacketGCItemSet), false},
		{HEADER_GC_SAFEBOX_DEL, sizeof(TPacketGCItemDel), false},
		{HEADER_GC_SAFEBOX_WRONG_PASSWORD, sizeof(TPacketGCSafeboxWrongPassword), false},
		{HEADER_GC_SAFEBOX_SIZE, sizeof(TPacketGCSafeboxSize), false},
		{HEADER_GC_SAFEBOX_MONEY_CHANGE, sizeof(TPacketGCSafeboxMoneyChange), false},
		{HEADER_GC_FISHING, sizeof(TPacketGCFishing), false},
		{HEADER_GC_DUNGEON, sizeof(TPacketGCDungeon), true},
		// Set(HEADER_GC_SLOW_TIMER, CNetworkPacketHeaderMap::TPacketType(sizeof(BYTE), STATIC_SIZE_PACKET));
		{HEADER_GC_TIME, sizeof(TPacketGCTime), false},
		{HEADER_GC_WALK_MODE, sizeof(TPacketGCWalkMode), false},
		{HEADER_GC_CHANGE_SKILL_GROUP, sizeof(TPacketGCChangeSkillGroup), false},
		{HEADER_GC_REFINE_INFORMATION, sizeof(TPacketGCRefineInformation), false},
		{HEADER_GC_REFINE_INFORMATION_NEW, sizeof(TPacketGCRefineInformationNew), false},
		{HEADER_GC_SEPCIAL_EFFECT, sizeof(TPacketGCSpecialEffect), false},
		{HEADER_GC_NPC_POSITION, sizeof(TPacketGCNPCPosition), true},
		{HEADER_GC_CHANGE_NAME, sizeof(TPacketGCChangeName), false},
		{HEADER_GC_CHINA_MATRIX_CARD, sizeof(TPacketGCChinaMatrixCard), false},
		{HEADER_GC_RUNUP_MATRIX_QUIZ, sizeof(TPacketGCRunupMatrixQuiz), false},
		{HEADER_GC_NEWCIBN_PASSPOD_REQUEST, sizeof(TPacketGCNEWCIBNPasspodRequest), false},
		{HEADER_GC_NEWCIBN_PASSPOD_FAILURE, sizeof(TPacketGCNEWCIBNPasspodFailure), false},
		{HEADER_GC_LOGIN_KEY, sizeof(TPacketGCLoginKey), false},
		{HEADER_GC_AUTH_SUCCESS, sizeof(TPacketGCAuthSuccess), false},
		{HEADER_GC_CHANNEL, sizeof(TPacketGCChannel), false},
		{HEADER_GC_VIEW_EQUIP, sizeof(TPacketGCViewEquip), false},
		{HEADER_GC_LAND_LIST, sizeof(TPacketGCLandList), true},
		{HEADER_GC_TARGET_UPDATE, sizeof(TPacketGCTargetUpdate), false},
		{HEADER_GC_TARGET_DELETE, sizeof(TPacketGCTargetDelete), false},
		{HEADER_GC_TARGET_CREATE_NEW, sizeof(TPacketGCTargetCreateNew), false},
		{HEADER_GC_AFFECT_ADD, sizeof(TPacketGCAffectAdd), false},
		{HEADER_GC_AFFECT_REMOVE, sizeof(TPacketGCAffectRemove), false},
		{HEADER_GC_MALL_OPEN, sizeof(TPacketGCMallOpen), false},
		{HEADER_GC_MALL_SET, sizeof(TPacketGCItemSet), false},
		{HEADER_GC_MALL_DEL, sizeof(TPacketGCItemDel), false},
		{HEADER_GC_LOVER_INFO, sizeof(TPacketGCLoverInfo), false},
		{HEADER_GC_LOVE_POINT_UPDATE, sizeof(TPacketGCLovePointUpdate), false},
		{HEADER_GC_DIG_MOTION, sizeof(TPacketGCDigMotion), false},
		{HEADER_GC_DAMAGE_INFO, sizeof(TPacketGCDamageInfo), false},
		{HEADER_GC_SPECIFIC_EFFECT, sizeof(TPacketGCSpecificEffect), false},
		{HEADER_GC_DRAGON_SOUL_REFINE, sizeof(TPacketGCDragonSoulRefine), false},
	};
}

bool protocol::Has(uint8_t header)
{
	switch (header)
	{
		case kQuickSlotAddHeader:
		case kQuickSlotDelHeader:
		case kItemOwnershipHeader:
		case kTargetHeader:
			return true;

		default:
			return false;
	}
}

bool protocol::IsRegistered(uint8_t header)
{
	return Size(header, nullptr);
}

bool protocol::IsDynamic(uint8_t header)
{
	for (const SizeEntry& entry : kSizeTable)
	{
		if (entry.header == header)
			return entry.dynamic;
	}

	return false;
}

bool protocol::Size(uint8_t header, std::size_t* size)
{
	for (const SizeEntry& entry : kSizeTable)
	{
		if (entry.header != header)
			continue;

		if (size)
			*size = entry.size;

		return true;
	}

	return false;
}

const protocol::SizeEntry* protocol::SizeTable(std::size_t* count)
{
	if (count)
		*count = sizeof(kSizeTable) / sizeof(kSizeTable[0]);

	return kSizeTable;
}

bool protocol::Decode(uint8_t header, const void* bytes, std::size_t size, Value* value)
{
	if (!value)
		return false;

	return DecodeByHeader(header, bytes, size, value);
}
