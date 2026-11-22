#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include <Client/Protocol/Types.h>

namespace OMT2Client::Protocol
{
	constexpr std::size_t kMaxPacketSize = 4096;

	struct Packet
	{
			uint8_t header;
			std::size_t size;
			std::array<uint8_t, kMaxPacketSize> bytes;

			// Returns a copy of the decoded value. Decode has already checked that
			// the packet length matches the registered size of this fixed-size packet.
			template <typename T>
			T Get() const
			{
				static_assert(std::is_trivially_copyable<T>::value, "wire value must be trivially copyable");
				static_assert(sizeof(T) <= kMaxPacketSize, "wire value exceeds the packet buffer");

				T value = {};

				if (sizeof(T) != size)
					return value;

				std::memcpy(&value, bytes.data(), sizeof(T));
				return value;
			}
	};

	struct SizeEntry
	{
			uint8_t header;
			std::size_t size;
			bool dynamic;
			bool decoded;
	};

	bool Has(uint8_t header);

	bool IsRegistered(uint8_t header);

	bool IsDynamic(uint8_t header);

	bool Size(uint8_t header, std::size_t* size);

	// The returned table is static and stays valid for the process lifetime.
	const SizeEntry* SizeTable(std::size_t* count);

	// Fails on an unknown header, on a dynamic header, on a buffer whose length
	// is not the registered size for that header, and on a null value.
	bool Decode(uint8_t header, const void* bytes, std::size_t size, Packet* packet);
}
