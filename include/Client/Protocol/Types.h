#pragma once

#include <cstddef>
#include <cstdint>

namespace protocol
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
#pragma pack(pop)
}
