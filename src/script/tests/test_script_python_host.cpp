#include <Client/Python/Host/PythonHostAdapter.h>

#include <Client/Utils/Test.h>

#include <Neoeter/IO/EterPackManager.h>

#include <support/TestAssert.h>

#include <cstdio>
#include <string>

bool __IS_TEST_SERVER_MODE__ = false;

int main()
{
	const char* entryName = "test_script_python_host_entry.py";
	FILE* entry = std::fopen(entryName, "wb");
	TEST_ASSERT(entry != nullptr);
	std::fputs("script_entry_value = 7\n", entry);
	std::fclose(entry);

	CEterPackManager packManager;
	packManager.SetSearchMode(false);

	Neoeter::ScriptHost script;
	script.RegisterHost("python", &Neoeter::CreatePythonHost);

	Neoeter::ScriptContext context;
	context.commandLine = "-test";
	context.debug = true;

	const std::string manifest =
		"version=1\n"
		"name=test\n"
		"host=python\n"
		"entry=test_script_python_host_entry.py\n";

	const Neoeter::ScriptHost::Result result = script.Start(manifest, context);
	TEST_ASSERT_MSG(result == Neoeter::ScriptHost::Result::Success, script.LastError().c_str());
	TEST_ASSERT(script.IsRunning());

	script.Stop();
	TEST_ASSERT(!script.IsRunning());

	std::remove(entryName);
	std::printf("[PASS] script python host\n");
	return 0;
}
