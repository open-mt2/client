#include <Client/Protocol/Decode.h>

#include <cstring>

namespace
{
	constexpr uint8_t kQuickSlotAddHeader = 28;
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

			case kItemOwnershipHeader:
				return DecodeItemOwnership(bytes, size, value);

			case kTargetHeader:
				return DecodeTarget(bytes, size, value);

			default:
				return false;
		}
	}

	const protocol::SizeEntry kSizeTable[] = {
		{kQuickSlotAddHeader, sizeof(WireQuickSlotAdd)},
		{kItemOwnershipHeader, sizeof(WireItemOwnership)},
		{kTargetHeader, sizeof(WireTarget)},
	};
}

bool protocol::Has(uint8_t header)
{
	return Size(header, nullptr);
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
