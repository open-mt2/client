#include <Client/UI/StdAfx.h>
#include <Client/Script/Bindings.h>
#include <Neoeter/Python/Host/PythonLauncher.h>

#include <support/PythonApiSnapshot.h>
#include <support/TestAssert.h>

#include <cstdio>
#include <vector>

// Defined by Context.cpp for the client executable, which is not linked
// into this library-only test.  The shipping client initialises it to false.
bool __IS_TEST_SERVER_MODE__ = false;

namespace
{

	// Every module the client registers, in the order the script host receives
	// them from the module table.  A new module is added to the table, not here.
	void RegisterClientModules()
	{
		const Neoeter::ModuleTable& modules = Neoeter::ClientModules();
		for (uint32_t i = 0; i < modules.count; ++i)
			modules.entries[i].register_module();
	}

	// Every module the client registers, in alphabetical order so the snapshot is
	// easy to review.  grpThing is registered by EterPythonLib's own test but not
	// by the client module table, so it is deliberately absent here.
	const char* kClientModules[] = {
		"ServerStateChecker",
		"app",
		"background",
		"chat",
		"chr",
		"chrmgr",
		"dbg",
		"effect",
		"event",
		"eventMgr",
		"exchange",
		"fly",
		"grp",
		"grpImage",
		"grpText",
		"guild",
		"ime",
		"item",
		"math",
		"messenger",
		"miniMap",
		"net",
		"nonplayer",
		"pack",
		"player",
		"profiler",
		"quest",
		"safebox",
		"shop",
		"skill",
		"snd",
		"systemSetting",
		"textTail",
		"time",
		"wndMgr",
	};

} // namespace

// Boot the full client interpreter headless, register every module from
// the client module table and pin the exposed API surface.
int main(int argc, char** argv)
{
	TEST_ASSERT_MSG(argc == 2, "usage: test_python_client_api <golden>");

	TEST_SECTION("UserInterface embedded interpreter");
	CPythonLauncher launcher;
	TEST_ASSERT_MSG(launcher.Create("test.python.client"),
					"CPythonLauncher::Create failed");
	TEST_PASS("interpreter booted (Py_NoSiteFlag=1, no display)");

	RegisterClientModules();

	const std::vector<const char*> modules(
		kClientModules,
		kClientModules + sizeof(kClientModules) / sizeof(kClientModules[0]));
	PythonApiSnapshot::Capture("UserInterface client", modules, argv[1]);

	TEST_PASS("every client module imports");
	return 0;
}
