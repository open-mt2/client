#include <support/TestAssert.h>

#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

#include <Neoeter/Platform/Base/lzo.h>
#include <Neoeter/Platform/Platform.h>
#include <Client/UI/MarkImage.h>

static const char* kPath = "/tmp/openmt2_test_mark_image.tga";

// CGuildMarkImage embeds the 80 compressed blocks (~3 MB); keep it on the heap
// so the test does not blow the 8 MB default stack.
static std::unique_ptr<CGuildMarkImage> NewImage()
{
	return std::unique_ptr<CGuildMarkImage>(new CGuildMarkImage());
}

static void FillMark(uint8_t* data, uint8_t seed)
{
	for (int i = 0; i < SGuildMark::SIZE; ++i)
	{
		data[i * 4 + 0] = (uint8_t)(seed + i);	   // B
		data[i * 4 + 1] = (uint8_t)(seed + i + 1); // G
		data[i * 4 + 2] = (uint8_t)(seed + i + 2); // R
		data[i * 4 + 3] = 0xFF;					   // A
	}
}

int main()
{
	// CGuildMarkImage::BuildAllBlocks compresses through the CLZO singleton,
	// which the client creates in main; the test must do the same.
	static CLZO lzo;

	TEST_SECTION("CGuildMarkImage round-trip");

	std::remove(kPath);

	// Build creates a blank 512x512 TGA on disk.
	{
		std::unique_ptr<CGuildMarkImage> image = NewImage();
		TEST_ASSERT(image->Build(kPath));

		FILE* file = std::fopen(kPath, "rb");
		TEST_ASSERT(file != NULL);
		std::fclose(file);
	}

	// Draw one mark, save, reload and compare the pixels.
	{
		uint8_t mark[SGuildMark::SIZE * 4];
		FillMark(mark, 7);

		std::unique_ptr<CGuildMarkImage> image = NewImage();
		TEST_ASSERT(image->Load(kPath));
		TEST_ASSERT(image->SaveMark(0, mark));
		TEST_ASSERT(image->Save(kPath));
	}

	{
		std::unique_ptr<CGuildMarkImage> image = NewImage();
		TEST_ASSERT(image->Load(kPath));

		uint8_t expected[SGuildMark::SIZE * 4];
		FillMark(expected, 7);

		uint8_t actual[SGuildMark::SIZE * 4];
		std::memset(actual, 0, sizeof(actual));
		image->GetData(0, 0, SGuildMark::WIDTH, SGuildMark::HEIGHT, actual);

		TEST_ASSERT(std::memcmp(expected, actual, sizeof(expected)) == 0);

		// The first position is now occupied; the next one must be reported.
		TEST_ASSERT(image->GetEmptyPosition() == 1);
	}

	// A mark slot that was never written stays empty.
	{
		std::unique_ptr<CGuildMarkImage> image = NewImage();
		TEST_ASSERT(image->Load(kPath));

		uint8_t blank[SGuildMark::SIZE * 4];
		std::memset(blank, 0, sizeof(blank));

		uint8_t actual[SGuildMark::SIZE * 4];
		std::memset(actual, 0xAA, sizeof(actual));
		image->GetData(SGuildMark::WIDTH, 0, SGuildMark::WIDTH, SGuildMark::HEIGHT, actual);

		TEST_ASSERT(std::memcmp(blank, actual, sizeof(blank)) == 0);
	}

	std::remove(kPath);
	return 0;
}
