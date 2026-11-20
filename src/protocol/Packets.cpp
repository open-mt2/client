#include <Client/Protocol/Decode.h>
#include <Client/Protocol/Wire.h>

#include <cstring>

namespace
{
	constexpr uint8_t kQuickSlotAddHeader = 28;
	constexpr uint8_t kQuickSlotDelHeader = 29;
	constexpr uint8_t kItemOwnershipHeader = 31;
	constexpr uint8_t kTargetHeader = 63;

	const protocol::SizeEntry* Find(uint8_t header)
	{
		std::size_t count = 0;
		const protocol::SizeEntry* entries = protocol::SizeTable(&count);

		for (std::size_t i = 0; i < count; ++i)
		{
			if (entries[i].header == header)
				return &entries[i];
		}

		return nullptr;
	}

	const protocol::SizeEntry kSizeTable[] = {
		{kQuickSlotAddHeader, sizeof(TPacketGCQuickSlotAdd), false, true},
		{kQuickSlotDelHeader, sizeof(TPacketGCQuickSlotDel), false, true},
		{kItemOwnershipHeader, sizeof(TPacketGCItemOwnership), false, true},
		{kTargetHeader, sizeof(TPacketGCTarget), false, true},
		{HEADER_GC_EMPIRE, sizeof(TPacketGCEmpire), false, true},
		{HEADER_GC_WARP, sizeof(TPacketGCWarp), false, true},
		{HEADER_GC_SKILL_COOLTIME_END, sizeof(TPacketGCSkillCoolTimeEnd), false, false},
		{HEADER_GC_QUEST_INFO, sizeof(TPacketGCQuestInfo), true, false},
		{HEADER_GC_REQUEST_MAKE_GUILD, sizeof(TPacketGCBlank), false, true},
		{HEADER_GC_PVP, sizeof(TPacketGCPVP), false, true},
		{HEADER_GC_DUEL_START, sizeof(TPacketGCDuelStart), true, false},
		{HEADER_GC_CHARACTER_ADD, sizeof(TPacketGCCharacterAdd), false, true},
		{HEADER_GC_CHAR_ADDITIONAL_INFO, sizeof(TPacketGCCharacterAdditionalInfo), false, true},
		{HEADER_GC_CHARACTER_ADD2, sizeof(TPacketGCCharacterAdd2), false, true},
		{HEADER_GC_CHARACTER_UPDATE, sizeof(TPacketGCCharacterUpdate), false, true},
		{HEADER_GC_CHARACTER_UPDATE2, sizeof(TPacketGCCharacterUpdate2), false, true},
		{HEADER_GC_CHARACTER_DEL, sizeof(TPacketGCCharacterDelete), false, true},
		{HEADER_GC_CHARACTER_MOVE, sizeof(TPacketGCMove), false, true},
		{HEADER_GC_CHAT, sizeof(TPacketGCChat), true, false},
		{HEADER_GC_SYNC_POSITION, sizeof(TPacketGCC2C), true, false},
		{HEADER_GC_LOGIN_SUCCESS3, sizeof(TPacketGCLoginSuccess3), false, true},
		{HEADER_GC_LOGIN_SUCCESS4, sizeof(TPacketGCLoginSuccess4), false, true},
		{HEADER_GC_LOGIN_FAILURE, sizeof(TPacketGCLoginFailure), false, true},
		{HEADER_GC_PLAYER_CREATE_SUCCESS, sizeof(TPacketGCPlayerCreateSuccess), false, true},
		{HEADER_GC_PLAYER_CREATE_FAILURE, sizeof(TPacketGCCreateFailure), false, true},
		{HEADER_GC_PLAYER_DELETE_SUCCESS, sizeof(TPacketGCBlank), false, true},
		{HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID, sizeof(TPacketGCBlank), false, true},
		{HEADER_GC_STUN, sizeof(TPacketGCStun), false, true},
		{HEADER_GC_DEAD, sizeof(TPacketGCDead), false, true},
		{HEADER_GC_MAIN_CHARACTER, sizeof(TPacketGCMainCharacter), false, true},
		{HEADER_GC_MAIN_CHARACTER2_EMPIRE, sizeof(TPacketGCMainCharacter2_EMPIRE), false, true},
		{HEADER_GC_MAIN_CHARACTER3_BGM, sizeof(TPacketGCMainCharacter3_BGM), false, true},
		{HEADER_GC_MAIN_CHARACTER4_BGM_VOL, sizeof(TPacketGCMainCharacter4_BGM_VOL), false, true},
		{HEADER_GC_PLAYER_POINTS, sizeof(TPacketGCPoints), false, true},
		{HEADER_GC_PLAYER_POINT_CHANGE, sizeof(TPacketGCPointChange), false, true},
		{HEADER_GC_ITEM_SET, sizeof(TPacketGCItemSet), false, true},
		{HEADER_GC_ITEM_SET2, sizeof(TPacketGCItemSet2), false, true},
		{HEADER_GC_ITEM_USE, sizeof(TPacketGCItemUse), false, true},
		{HEADER_GC_ITEM_UPDATE, sizeof(TPacketGCItemUpdate), false, true},
		{HEADER_GC_ITEM_GROUND_ADD, sizeof(TPacketGCItemGroundAdd), false, true},
		{HEADER_GC_ITEM_GROUND_DEL, sizeof(TPacketGCItemGroundDel), false, true},
		{HEADER_GC_QUICKSLOT_SWAP, sizeof(TPacketGCQuickSlotSwap), false, true},
		{HEADER_GC_WHISPER, sizeof(TPacketGCWhisper), false, false},
		{HEADER_GC_CHARACTER_POSITION, sizeof(TPacketGCPosition), false, true},
		{HEADER_GC_MOTION, sizeof(TPacketGCMotion), false, true},
		{HEADER_GC_SHOP, sizeof(TPacketGCShop), true, false},
		{HEADER_GC_SHOP_SIGN, sizeof(TPacketGCShopSign), false, true},
		{HEADER_GC_EXCHANGE, sizeof(TPacketGCExchange), false, true},
		{HEADER_GC_PING, sizeof(TPacketGCPing), false, true},
		{HEADER_GC_SCRIPT, sizeof(TPacketGCScript), true, false},
		{HEADER_GC_QUEST_CONFIRM, sizeof(TPacketGCQuestConfirm), false, true},
		{HEADER_GC_MOUNT, sizeof(TPacketGCMount), false, true},
		{HEADER_GC_CHANGE_SPEED, sizeof(TPacketGCChangeSpeed), false, true},
		{HEADER_GC_HANDSHAKE, sizeof(TPacketGCHandshake), false, false},
		{HEADER_GC_HANDSHAKE_OK, sizeof(TPacketGCBlank), false, true},
		{HEADER_GC_OWNERSHIP, sizeof(TPacketGCOwnership), false, true},
		{HEADER_GC_CREATE_FLY, sizeof(TPacketGCCreateFly), false, true},
		{HEADER_GC_ADD_FLY_TARGETING, sizeof(TPacketGCFlyTargeting), false, true},
		{HEADER_GC_FLY_TARGETING, sizeof(TPacketGCFlyTargeting), false, true},
		{HEADER_GC_PHASE, sizeof(TPacketGCPhase), false, true},
		{HEADER_GC_SKILL_LEVEL, sizeof(TPacketGCSkillLevel), false, true},
		{HEADER_GC_SKILL_LEVEL_NEW, sizeof(TPacketGCSkillLevelNew), false, true},
		{HEADER_GC_MESSENGER, sizeof(TPacketGCMessenger), true, false},
		{HEADER_GC_GUILD, sizeof(TPacketGCGuild), true, false},
		{HEADER_GC_PARTY_INVITE, sizeof(TPacketGCPartyInvite), false, true},
		{HEADER_GC_PARTY_ADD, sizeof(TPacketGCPartyAdd), false, true},
		{HEADER_GC_PARTY_UPDATE, sizeof(TPacketGCPartyUpdate), false, true},
		{HEADER_GC_PARTY_REMOVE, sizeof(TPacketGCPartyRemove), false, true},
		{HEADER_GC_PARTY_LINK, sizeof(TPacketGCPartyLink), false, true},
		{HEADER_GC_PARTY_UNLINK, sizeof(TPacketGCPartyUnlink), false, true},
		{HEADER_GC_PARTY_PARAMETER, sizeof(TPacketGCPartyParameter), false, true},
		{HEADER_GC_SAFEBOX_SET, sizeof(TPacketGCItemSet), false, true},
		{HEADER_GC_SAFEBOX_DEL, sizeof(TPacketGCItemDel), false, true},
		{HEADER_GC_SAFEBOX_WRONG_PASSWORD, sizeof(TPacketGCSafeboxWrongPassword), false, true},
		{HEADER_GC_SAFEBOX_SIZE, sizeof(TPacketGCSafeboxSize), false, true},
		{HEADER_GC_SAFEBOX_MONEY_CHANGE, sizeof(TPacketGCSafeboxMoneyChange), false, true},
		{HEADER_GC_FISHING, sizeof(TPacketGCFishing), false, true},
		{HEADER_GC_DUNGEON, sizeof(TPacketGCDungeon), true, false},
		// Set(HEADER_GC_SLOW_TIMER, CNetworkPacketHeaderMap::TPacketType(sizeof(uint8_t), STATIC_SIZE_PACKET));, false},
		{HEADER_GC_TIME, sizeof(TPacketGCTime), false, true},
		{HEADER_GC_WALK_MODE, sizeof(TPacketGCWalkMode), false, true},
		{HEADER_GC_CHANGE_SKILL_GROUP, sizeof(TPacketGCChangeSkillGroup), false, true},
		{HEADER_GC_REFINE_INFORMATION, sizeof(TPacketGCRefineInformation), false, true},
		{HEADER_GC_REFINE_INFORMATION_NEW, sizeof(TPacketGCRefineInformationNew), false, true},
		{HEADER_GC_SEPCIAL_EFFECT, sizeof(TPacketGCSpecialEffect), false, true},
		{HEADER_GC_NPC_POSITION, sizeof(TPacketGCNPCPosition), true, false},
		{HEADER_GC_CHANGE_NAME, sizeof(TPacketGCChangeName), false, true},
		{HEADER_GC_CHINA_MATRIX_CARD, sizeof(TPacketGCChinaMatrixCard), false, true},
		{HEADER_GC_RUNUP_MATRIX_QUIZ, sizeof(TPacketGCRunupMatrixQuiz), false, true},
		{HEADER_GC_NEWCIBN_PASSPOD_REQUEST, sizeof(TPacketGCNEWCIBNPasspodRequest), false, true},
		{HEADER_GC_NEWCIBN_PASSPOD_FAILURE, sizeof(TPacketGCNEWCIBNPasspodFailure), false, true},
		{HEADER_GC_LOGIN_KEY, sizeof(TPacketGCLoginKey), false, true},
		{HEADER_GC_AUTH_SUCCESS, sizeof(TPacketGCAuthSuccess), false, false},
		{HEADER_GC_CHANNEL, sizeof(TPacketGCChannel), false, true},
		{HEADER_GC_VIEW_EQUIP, sizeof(TPacketGCViewEquip), false, true},
		{HEADER_GC_LAND_LIST, sizeof(TPacketGCLandList), true, false},
		{HEADER_GC_TARGET_UPDATE, sizeof(TPacketGCTargetUpdate), false, true},
		{HEADER_GC_TARGET_DELETE, sizeof(TPacketGCTargetDelete), false, true},
		{HEADER_GC_TARGET_CREATE_NEW, sizeof(TPacketGCTargetCreateNew), false, true},
		{HEADER_GC_AFFECT_ADD, sizeof(TPacketGCAffectAdd), false, true},
		{HEADER_GC_AFFECT_REMOVE, sizeof(TPacketGCAffectRemove), false, true},
		{HEADER_GC_MALL_OPEN, sizeof(TPacketGCMallOpen), false, true},
		{HEADER_GC_MALL_SET, sizeof(TPacketGCItemSet), false, true},
		{HEADER_GC_MALL_DEL, sizeof(TPacketGCItemDel), false, true},
		{HEADER_GC_LOVER_INFO, sizeof(TPacketGCLoverInfo), false, true},
		{HEADER_GC_LOVE_POINT_UPDATE, sizeof(TPacketGCLovePointUpdate), false, true},
		{HEADER_GC_DIG_MOTION, sizeof(TPacketGCDigMotion), false, true},
		{HEADER_GC_DAMAGE_INFO, sizeof(TPacketGCDamageInfo), false, true},
		{HEADER_GC_SPECIFIC_EFFECT, sizeof(TPacketGCSpecificEffect), false, true},
		{HEADER_GC_DRAGON_SOUL_REFINE, sizeof(TPacketGCDragonSoulRefine), false, true},
	};
}

bool protocol::Has(uint8_t header)
{
	const SizeEntry* entry = Find(header);
	return entry && entry->decoded;
}

bool protocol::IsRegistered(uint8_t header)
{
	return Find(header) != nullptr;
}

bool protocol::IsDynamic(uint8_t header)
{
	const SizeEntry* entry = Find(header);
	return entry && entry->dynamic;
}

bool protocol::Size(uint8_t header, std::size_t* size)
{
	const SizeEntry* entry = Find(header);

	if (!entry)
		return false;

	if (size)
		*size = entry->size;

	return true;
}

const protocol::SizeEntry* protocol::SizeTable(std::size_t* count)
{
	if (count)
		*count = sizeof(kSizeTable) / sizeof(kSizeTable[0]);

	return kSizeTable;
}

bool protocol::Decode(uint8_t header, const void* bytes, std::size_t size, Packet* packet)
{
	if (!packet || !bytes)
		return false;

	const SizeEntry* entry = Find(header);

	if (!entry || !entry->decoded || entry->size != size || size > kMaxPacketSize)
		return false;

	if (static_cast<const uint8_t*>(bytes)[0] != header)
		return false;

	packet->header = header;
	packet->size = size;
	std::memcpy(packet->bytes.data(), bytes, size);
	return true;
}
