#include <support/TestAssert.h>

#include <Client/Protocol/Decode.h>
#include <Client/Protocol/Wire.h>

#include <cstdint>
#include <cstring>
#include <vector>

namespace
{
	std::vector<uint8_t> QuickSlotAddBytes(uint8_t header, uint8_t position, uint8_t type, uint8_t slotPosition)
	{
		return {header, position, type, slotPosition};
	}

	std::vector<uint8_t> QuickSlotDelBytes(uint8_t header, uint8_t position)
	{
		return {header, position};
	}

	std::vector<uint8_t> ItemOwnershipBytes(uint8_t header, uint32_t vid, const char* name)
	{
		std::vector<uint8_t> bytes(sizeof(TPacketGCItemOwnership), 0);
		bytes[0] = header;
		std::memcpy(&bytes[1], &vid, sizeof(vid));
		std::strncpy(reinterpret_cast<char*>(&bytes[5]), name, protocol::kCharacterNameMaxLen);
		return bytes;
	}

	std::vector<uint8_t> TargetBytes(uint8_t header, uint32_t vid, uint8_t hpPercent)
	{
		std::vector<uint8_t> bytes(sizeof(TPacketGCTarget), 0);
		bytes[0] = header;
		std::memcpy(&bytes[1], &vid, sizeof(vid));
		bytes[5] = hpPercent;
		return bytes;
	}

	void test_registry()
	{
		TEST_SECTION("the registry covers every received header");

		std::size_t count = 0;
		const protocol::SizeEntry* entries = protocol::SizeTable(&count);

		TEST_ASSERT(count == 109);
		TEST_ASSERT(entries != nullptr);

		std::size_t size = 0;
		TEST_ASSERT(protocol::Size(HEADER_GC_WARP, &size) && size == sizeof(TPacketGCWarp));
		TEST_ASSERT(protocol::Size(HEADER_GC_QUICKSLOT_ADD, &size) && size == sizeof(TPacketGCQuickSlotAdd));
		TEST_ASSERT(protocol::Size(HEADER_GC_QUICKSLOT_DEL, &size) && size == sizeof(TPacketGCQuickSlotDel));
		TEST_ASSERT(protocol::Size(HEADER_GC_ITEM_OWNERSHIP, &size) && size == sizeof(TPacketGCItemOwnership));
		TEST_ASSERT(protocol::Size(HEADER_GC_TARGET, &size) && size == sizeof(TPacketGCTarget));

		TEST_ASSERT(protocol::Has(HEADER_GC_WARP));
		TEST_ASSERT(!protocol::Has(HEADER_GC_CHAT));
		TEST_ASSERT(protocol::IsDynamic(HEADER_GC_CHAT));
		TEST_ASSERT(!protocol::IsDynamic(HEADER_GC_WARP));
		TEST_ASSERT(protocol::IsRegistered(HEADER_GC_WHISPER));

		for (std::size_t i = 0; i < count; ++i)
		{
			if (!entries[i].dynamic && entries[i].size > protocol::kMaxPacketSize)

				if (!entries[i].dynamic)
					TEST_ASSERT(entries[i].size <= protocol::kMaxPacketSize);
		}

		TEST_PASS("sizes and kinds are owned by one table");
	}

	void test_valid_buffer()
	{
		TEST_SECTION("decoded values from valid buffers");

		const std::vector<uint8_t> quickSlotAdd = QuickSlotAddBytes(HEADER_GC_QUICKSLOT_ADD, 3, 2, 7);
		protocol::Packet value;
		TEST_ASSERT(protocol::Decode(HEADER_GC_QUICKSLOT_ADD, quickSlotAdd.data(), quickSlotAdd.size(), &value));
		const TPacketGCQuickSlotAdd decodedAdd = value.Get<TPacketGCQuickSlotAdd>();
		TEST_ASSERT(decodedAdd.position == 3);
		TEST_ASSERT(decodedAdd.type == 2);
		TEST_ASSERT(decodedAdd.slotPosition == 7);

		const std::vector<uint8_t> quickSlotDel = QuickSlotDelBytes(HEADER_GC_QUICKSLOT_DEL, 9);
		TEST_ASSERT(protocol::Decode(HEADER_GC_QUICKSLOT_DEL, quickSlotDel.data(), quickSlotDel.size(), &value));
		const TPacketGCQuickSlotDel decodedDel = value.Get<TPacketGCQuickSlotDel>();
		TEST_ASSERT(decodedDel.position == 9);

		const std::vector<uint8_t> ownership = ItemOwnershipBytes(HEADER_GC_ITEM_OWNERSHIP, 1234, "player");
		TEST_ASSERT(protocol::Decode(HEADER_GC_ITEM_OWNERSHIP, ownership.data(), ownership.size(), &value));
		const TPacketGCItemOwnership decodedOwnership = value.Get<TPacketGCItemOwnership>();
		TEST_ASSERT(decodedOwnership.vid == 1234);
		TEST_ASSERT(std::strcmp(decodedOwnership.name, "player") == 0);

		const std::vector<uint8_t> target = TargetBytes(HEADER_GC_TARGET, 4321, 80);
		TEST_ASSERT(protocol::Decode(HEADER_GC_TARGET, target.data(), target.size(), &value));
		const TPacketGCTarget decodedTarget = value.Get<TPacketGCTarget>();
		TEST_ASSERT(decodedTarget.vid == 4321);
		TEST_ASSERT(decodedTarget.hpPercent == 80);

		TEST_PASS("bytes in, typed value out");
	}

	void test_truncated_buffer()
	{
		TEST_SECTION("truncated buffer is refused");

		protocol::Packet value;

		const std::vector<uint8_t> target = TargetBytes(HEADER_GC_TARGET, 1, 2);
		TEST_ASSERT(!protocol::Decode(HEADER_GC_TARGET, target.data(), target.size() - 1, &value));

		const std::vector<uint8_t> ownership(sizeof(TPacketGCItemOwnership) - 1, 0);
		TEST_ASSERT(!protocol::Decode(HEADER_GC_ITEM_OWNERSHIP, ownership.data(), ownership.size(), &value));

		TEST_PASS("a short buffer never decodes");
	}

	void test_wrong_size_header()
	{
		TEST_SECTION("wrong length is refused");

		protocol::Packet value;

		const std::vector<uint8_t> quickSlotAdd = QuickSlotAddBytes(HEADER_GC_QUICKSLOT_ADD, 0, 0, 0);
		TEST_ASSERT(!protocol::Decode(HEADER_GC_QUICKSLOT_ADD, quickSlotAdd.data(), quickSlotAdd.size() + 1, &value));

		const std::vector<uint8_t> target(sizeof(TPacketGCTarget), 0);
		TEST_ASSERT(!protocol::Decode(HEADER_GC_TARGET, target.data(), target.size() - 1, &value));

		TEST_PASS("the registered size is the only accepted length");
	}

	void test_unknown_header()
	{
		TEST_SECTION("unknown header is refused");

		protocol::Packet value;

		TEST_ASSERT(!protocol::Has(0xFE));
		TEST_ASSERT(!protocol::Size(0xFE, nullptr));
		TEST_ASSERT(!protocol::IsRegistered(0xFE));

		const std::vector<uint8_t> bytes(6, 0);
		TEST_ASSERT(!protocol::Decode(0xFE, bytes.data(), bytes.size(), &value));

		TEST_PASS("the registry answers only for registered headers");
	}

	void test_wrong_fixed_field()
	{
		TEST_SECTION("wrong header field is refused");

		protocol::Packet value;

		const std::vector<uint8_t> target = TargetBytes(HEADER_GC_ITEM_OWNERSHIP, 1234, 40);
		TEST_ASSERT(!protocol::Decode(HEADER_GC_TARGET, target.data(), target.size(), &value));

		const std::vector<uint8_t> quickSlotDel = {HEADER_GC_QUICKSLOT_ADD, 4};
		TEST_ASSERT(!protocol::Decode(HEADER_GC_QUICKSLOT_DEL, quickSlotDel.data(), quickSlotDel.size(), &value));

		TEST_PASS("the header byte inside the buffer must agree with the request");
	}
}

int main()
{
	test_registry();
	test_valid_buffer();
	test_truncated_buffer();
	test_wrong_size_header();
	test_unknown_header();
	test_wrong_fixed_field();

	std::printf("[test_protocol] all checks passed\n");
	return 0;
}
