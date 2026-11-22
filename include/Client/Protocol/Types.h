#pragma once

#include <cstddef>
#include <cstdint>

namespace OMT2Client::Protocol
{
	constexpr std::size_t kCharacterNameMaxLen = 24;
	constexpr std::size_t kPointMaxNum = 255;
	constexpr std::size_t kDragonSoulRefineWindowMaxNum = 15;
	constexpr std::size_t kItemSocketSlotMaxNum = 3;
	constexpr std::size_t kItemAttributeSlotMaxNum = 7;

#pragma pack(push, 1)
	struct ItemPos
	{
			uint8_t window_type;
			uint16_t cell;

			ItemPos() : window_type(1), cell(0xffff) {}
			ItemPos(uint8_t windowType, uint16_t cellIndex) : window_type(windowType), cell(cellIndex) {}

			bool operator==(const ItemPos& rhs) const
			{
				return window_type == rhs.window_type && cell == rhs.cell;
			}

			bool operator<(const ItemPos& rhs) const
			{
				return (window_type < rhs.window_type) || (window_type == rhs.window_type && cell < rhs.cell);
			}
	};

	struct Vector3
	{
			float x;
			float y;
			float z;
	};
#pragma pack(pop)
}

#pragma pack(push, 1)
typedef struct SQuickSlot
{
		uint8_t Type;
		uint8_t Position;
} TQuickSlot;

typedef struct TPlayerItemAttribute
{
		uint8_t bType;
		int16_t sValue;
} TPlayerItemAttribute;

typedef struct packet_shop_item
{
		uint32_t vnum;
		uint32_t price;
		uint8_t count;
		uint8_t display_pos;
		int32_t alSockets[OMT2Client::Protocol::kItemSocketSlotMaxNum];
		TPlayerItemAttribute aAttr[OMT2Client::Protocol::kItemAttributeSlotMaxNum];
} TShopItemData;
#pragma pack(pop)
