#include <support/TestAssert.h>

#include <Client/UI/FrameClear.h>

// Every GL frame must start from a cleared colour buffer (open-mt2/devkit#137).
//
// The Windows build presents through OpenGL like Linux, but the colour clear was
// compiled only for debug and Linux builds. After a swap the back buffer keeps an
// older frame, so any screen region the UI leaves undrawn alternates between
// stale frames: character select flickered after login.

namespace
{

struct RecordingGraphic
{
	int depthClears = 0;
	int fullClears = 0;
	float r = -1.0f;
	float g = -1.0f;
	float b = -1.0f;
	float a = -1.0f;

	void ClearDepthBuffer() { ++depthClears; }
	void Clear() { ++fullClears; }
	void SetClearColor(float red, float green, float blue, float alpha = 1.0f)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
};

} // namespace

int main()
{
	TEST_SECTION("a frame clears depth and colour");
	{
		RecordingGraphic graphic;
		BeginFrameClear(graphic);
		TEST_ASSERT(graphic.depthClears == 1);
		TEST_ASSERT(graphic.fullClears == 1);
		TEST_PASS("the colour buffer is cleared, not only the depth buffer");
	}

	TEST_SECTION("the clear colour is the Linux one on every GL target");
	{
		RecordingGraphic graphic;
		BeginFrameClear(graphic);
		TEST_ASSERT(graphic.r == 0.1f);
		TEST_ASSERT(graphic.g == 0.1f);
		TEST_ASSERT(graphic.b == 0.15f);
		TEST_ASSERT(graphic.a == 1.0f);
		TEST_PASS("0.1, 0.1, 0.15, opaque");
	}

	return 0;
}
