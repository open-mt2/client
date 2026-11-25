#include <Client/Application/CLI.h>
#include <Client/Application/ScriptRuntime.h>

#include <Neoeter/DotNet/DotNetRuntime.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Script/ScriptBackend.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <memory>
#include <vector>

#define TEST_ASSERT(cond)                                                          \
	do {                                                                           \
		if (!(cond))                                                               \
		{                                                                          \
			std::fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #cond); \
			std::exit(1);                                                          \
		}                                                                          \
	} while (0)

using Neoeter::CommandLine;
using Neoeter::ScriptRuntime;

static void TestCommandLine()
{
	unsetenv("OPENMT2_SCRIPT_RUNTIME");
	{
		CommandLine line("");
		TEST_ASSERT(line.ScriptRuntimeKind() == ScriptRuntime::Kind::Auto);
		TEST_ASSERT(line.Process() == CommandLine::Result::Continue);
	}
	{
		CommandLine line("--script-runtime=python foo bar");
		TEST_ASSERT(line.ScriptRuntimeKind() == ScriptRuntime::Kind::Python);
		TEST_ASSERT(line.PythonCommandLine() == "foo bar");
	}
	{
		CommandLine line("foo --script-runtime dotnet");
		TEST_ASSERT(line.ScriptRuntimeKind() == ScriptRuntime::Kind::DotNet);
		TEST_ASSERT(line.PythonCommandLine() == "foo");
	}
	{
		CommandLine line("--script-runtime=dotnet");
		TEST_ASSERT(line.PythonCommandLine().empty());
	}
	{
		CommandLine line("--script-runtime=lua");
		TEST_ASSERT(line.Process() == CommandLine::Result::InvalidArguments);
	}

	setenv("OPENMT2_SCRIPT_RUNTIME", "dotnet", 1);
	{
		CommandLine fromEnv("x");
		TEST_ASSERT(fromEnv.ScriptRuntimeKind() == ScriptRuntime::Kind::DotNet);
		CommandLine overridden("--script-runtime=python");
		TEST_ASSERT(overridden.ScriptRuntimeKind() == ScriptRuntime::Kind::Python);
	}
	unsetenv("OPENMT2_SCRIPT_RUNTIME");
	std::printf("  [PASS] --script-runtime and OPENMT2_SCRIPT_RUNTIME\n");
}

int main(int argc, char** argv)
{
	TEST_ASSERT(argc == 2); // <entry assembly>
	TestCommandLine();

	CEterPackManager packs;
	std::ifstream file(argv[1], std::ios::binary);
	TEST_ASSERT(file.is_open());
	packs.SetOverride(ScriptRuntime::DotNetEntryAssembly,
					  std::make_shared<std::vector<char>>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));

	// auto picks .NET because the entry assembly is in the packs.
	ScriptRuntime runtime;
	TEST_ASSERT(runtime.Start(ScriptRuntime::Kind::Auto, "alpha beta") == ScriptRuntime::Result::Success);
	TEST_ASSERT(runtime.Active() == ScriptRuntime::Kind::DotNet);
	TEST_ASSERT(Script_GetActiveBackend() == &CDotNetRuntime::Instance());
	std::printf("  [PASS] auto -> dotnet, native modules, scriptHost, app.RunPythonFile\n");

	runtime.Stop();
	TEST_ASSERT(Script_GetActiveBackend() == nullptr);
	TEST_ASSERT(!CDotNetRuntime::Instance().IsInitialized());

	std::printf("[test_script_runtime] All tests passed\n");
	return 0;
}
