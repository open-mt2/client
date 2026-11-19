#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <variant>

#include <Client/Protocol/Types.h>

namespace protocol
{
	constexpr std::size_t kMaxWireSize = 32;

	struct QuickSlotAddValue
	{
			uint8_t position;
			uint8_t type;
			uint8_t slotPosition;
	};

	struct QuickSlotDelValue
	{
			uint8_t position;
	};

	struct ItemOwnershipValue
	{
			uint32_t vid;
			std::array<char, kCharacterNameMaxLen + 1> name;
	};

	struct TargetValue
	{
			uint32_t vid;
			uint8_t hpPercent;
	};

	using Value = std::variant<QuickSlotAddValue, QuickSlotDelValue, ItemOwnershipValue, TargetValue>;

	struct SizeEntry
	{
			uint8_t header;
			std::size_t size;
			bool dynamic;
	};

	// True when a decoder exists for the header.
	bool Has(uint8_t header);

	// True when the header is in the size registry, decoded or not.
	bool IsRegistered(uint8_t header);

	// True when the header carries a variable-length body.
	bool IsDynamic(uint8_t header);

	bool Size(uint8_t header, std::size_t* size);

	// The returned table is static and stays valid for the process lifetime.
	const SizeEntry* SizeTable(std::size_t* count);

	// Fails on an unknown header, on a buffer whose length is not the registered
	// size for that header, and on a header byte that disagrees with the request.
	bool Decode(uint8_t header, const void* bytes, std::size_t size, Value* value);
}
