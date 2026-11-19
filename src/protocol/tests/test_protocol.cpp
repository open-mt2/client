#include <support/TestAssert.h>

#include <Client/Protocol/Decode.h>

#include <cstdint>
#include <cstring>
#include <vector>

namespace
{
	constexpr uint8_t kQuickSlotAdd = 28;
	constexpr uint8_t kQuickSlotDel = 29;
	constexpr uint8_t kItemOwnership = 31;
	constexpr uint8_t kTarget = 63;

	std::vector<uint8_t> QuickSlotAddBytes(uint8_t header, uint8_t position, uint8_t type, uint8_t slotPosition)
	{
		return {header, position, type, slotPosition};
	}

	std::vector<uint8_t> ItemOwnershipBytes(uint8_t header, uint32_t vid, const char* name)
	{
		std::vector<uint8_t> bytes(30, 0);
		bytes[0] = header;
		std::memcpy(&bytes[1], &vid, sizeof(vid));
		std::strncpy(reinterpret_cast<char*>(&bytes[5]), name, 24);
		return bytes;
	}

	std::vector<uint8_t> TargetBytes(uint8_t header, uint32_t vid, uint8_t hpPercent)
	{
		std::vector<uint8_t> bytes(6, 0);
		bytes[0] = header;
		std::memcpy(&bytes[1], &vid, sizeof(vid));
		bytes[5] = hpPercent;
		return bytes;
	}

	void test_valid_buffer()
	{
		TEST_SECTION("decoded values from valid buffers");

		std::size_t count = 0;
		protocol::SizeTable(&count);
		TEST_ASSERT(count == 109);

		std::size_t size = 0;
		TEST_ASSERT(protocol::Size(kQuickSlotAdd, &size) && size == 4);
		TEST_ASSERT(protocol::Size(kQuickSlotDel, &size) && size == 2);
		TEST_ASSERT(protocol::Size(kItemOwnership, &size) && size == 30);
		TEST_ASSERT(protocol::Size(kTarget, &size) && size == 6);

		protocol::Value value;

		const std::vector<uint8_t> quickSlotAdd = QuickSlotAddBytes(kQuickSlotAdd, 3, 2, 7);
		TEST_ASSERT(protocol::Decode(kQuickSlotAdd, quickSlotAdd.data(), quickSlotAdd.size(), &value));
		const auto& decodedAdd = std::get<protocol::QuickSlotAddValue>(value);
		TEST_ASSERT(decodedAdd.position == 3);
		TEST_ASSERT(decodedAdd.type == 2);
		TEST_ASSERT(decodedAdd.slotPosition == 7);

		const std::vector<uint8_t> quickSlotDel = {kQuickSlotDel, 9};
		TEST_ASSERT(protocol::Decode(kQuickSlotDel, quickSlotDel.data(), quickSlotDel.size(), &value));
		TEST_ASSERT(std::get<protocol::QuickSlotDelValue>(value).position == 9);

		const std::vector<uint8_t> ownership = ItemOwnershipBytes(kItemOwnership, 0xDEADBEEFu, "Ancient Sword");
		TEST_ASSERT(protocol::Decode(kItemOwnership, ownership.data(), ownership.size(), &value));
		const auto& decodedOwnership = std::get<protocol::ItemOwnershipValue>(value);
		TEST_ASSERT(decodedOwnership.vid == 0xDEADBEEFu);
		TEST_ASSERT(std::strncmp(decodedOwnership.name.data(), "Ancient Sword", 24) == 0);

		const std::vector<uint8_t> target = TargetBytes(kTarget, 0x01020304u, 55);
		TEST_ASSERT(protocol::Decode(kTarget, target.data(), target.size(), &value));
		const auto& decodedTarget = std::get<protocol::TargetValue>(value);
		TEST_ASSERT(decodedTarget.vid == 0x01020304u);
		TEST_ASSERT(decodedTarget.hpPercent == 55);

		TEST_PASS("bytes decode into plain values");
	}

	void test_truncated_buffer()
	{
		TEST_SECTION("truncated buffer is refused");

		protocol::Value value;

		const std::vector<uint8_t> ownership = ItemOwnershipBytes(kItemOwnership, 77, "Shield");
		TEST_ASSERT(!protocol::Decode(kItemOwnership, ownership.data(), ownership.size() - 1, &value));

		const std::vector<uint8_t> target = TargetBytes(kTarget, 5, 10);
		TEST_ASSERT(!protocol::Decode(kTarget, target.data(), target.size() - 1, &value));

		TEST_PASS("a short buffer never reaches the field reads");
	}

	void test_wrong_size_header()
	{
		TEST_SECTION("buffer length must match the registered size");

		protocol::Value value;

		const std::vector<uint8_t> ownership = ItemOwnershipBytes(kTarget, 42, "Ring");
		TEST_ASSERT(!protocol::Decode(kTarget, ownership.data(), ownership.size(), &value));

		const std::vector<uint8_t> quickSlotAdd = QuickSlotAddBytes(kItemOwnership, 1, 1, 1);
		TEST_ASSERT(!protocol::Decode(kItemOwnership, quickSlotAdd.data(), quickSlotAdd.size(), &value));

		TEST_PASS("an oversized buffer is refused");
	}

	void test_unknown_header()
	{
		TEST_SECTION("unknown header is refused");

		protocol::Value value;

		TEST_ASSERT(!protocol::Has(0xFE));
		TEST_ASSERT(!protocol::Size(0xFE, nullptr));

		const std::vector<uint8_t> bytes(6, 0);
		TEST_ASSERT(!protocol::Decode(0xFE, bytes.data(), bytes.size(), &value));

		TEST_PASS("the registry answers only for registered headers");
	}

	void test_wrong_fixed_field()
	{
		TEST_SECTION("wrong header field is refused");

		protocol::Value value;

		const std::vector<uint8_t> target = TargetBytes(kItemOwnership, 1234, 40);
		TEST_ASSERT(!protocol::Decode(kTarget, target.data(), target.size(), &value));

		const std::vector<uint8_t> quickSlotDel = {kQuickSlotAdd, 4};
		TEST_ASSERT(!protocol::Decode(kQuickSlotDel, quickSlotDel.data(), quickSlotDel.size(), &value));

		TEST_PASS("the header byte inside the buffer must agree with the request");
	}
}

int main()
{
	test_valid_buffer();
	test_truncated_buffer();
	test_wrong_size_header();
	test_unknown_header();
	test_wrong_fixed_field();

	std::printf("[test_protocol] all checks passed\n");
	return 0;
}
