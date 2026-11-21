#include <Client/Script/Bindings.h>
#include <Client/Script/PackManifest.h>
#include <Client/Script/ScriptHost.h>

#include <Client/UI/Test.h>

#include <support/TestAssert.h>

#include <cstring>
#include <memory>
#include <string>

bool __IS_TEST_SERVER_MODE__ = false;

namespace
{
	struct FakeHostState
	{
		int loads = 0;
		int unloads = 0;
		int destroys = 0;
		bool fail = false;
		std::string host;
		std::string pack;
		std::string entry;
		std::string commandLine;
		bool debug = false;
		uint32_t modules = 0;
	};

	FakeHostState g_fake;

	class FakeHost : public Neoeter::IHost
	{
		public:
			~FakeHost() override
			{
				++g_fake.destroys;
			}

			bool Load(const Neoeter::PackManifest& manifest, const Neoeter::ScriptContext& context, const Neoeter::ModuleTable& modules, std::string* error) override
			{
				++g_fake.loads;
				g_fake.pack = manifest.Name();
				g_fake.entry = manifest.Entry();
				g_fake.commandLine = context.commandLine;
				g_fake.debug = context.debug;
				g_fake.modules = modules.count;

				if (g_fake.fail)
				{
					*error = "fake host rejected the pack";
					return false;
				}

				return true;
			}

			void Unload() override
			{
				++g_fake.unloads;
			}
	};

	std::unique_ptr<Neoeter::IHost> CreateFakeHost()
	{
		return std::unique_ptr<Neoeter::IHost>(new FakeHost());
	}

	const char* kManifest =
		"# pack\n"
		"version=1\n"
		"name=root\n"
		"host=fake\n"
		"entry=entry.py\n"
		"future_key=ignored\n";
}

int main()
{
	TEST_SECTION("manifest parsing");
	Neoeter::PackManifest manifest;
	TEST_ASSERT(Neoeter::PackManifest::Parse(kManifest, &manifest) == Neoeter::PackManifest::Result::Success);
	TEST_ASSERT(manifest.Version() == 1);
	TEST_ASSERT(manifest.Name() == "root");
	TEST_ASSERT(manifest.Host() == "fake");
	TEST_ASSERT(manifest.Entry() == "entry.py");

	TEST_ASSERT(Neoeter::PackManifest::Parse("version=1\nname=root\nhost=fake\n", &manifest) == Neoeter::PackManifest::Result::MissingField);
	TEST_ASSERT(Neoeter::PackManifest::Parse("version=2\nname=root\nhost=fake\nentry=entry.py\n", &manifest) == Neoeter::PackManifest::Result::UnsupportedVersion);
	TEST_ASSERT(Neoeter::PackManifest::Parse("version=1\nnot a pair\n", &manifest) == Neoeter::PackManifest::Result::InvalidLine);

	TEST_SECTION("module table");
	const Neoeter::ModuleTable& modules = Neoeter::ClientModules();
	TEST_ASSERT(modules.count == 33);
	for (uint32_t i = 0; i < modules.count; ++i)
	{
		TEST_ASSERT(modules.entries[i].name != nullptr);
		TEST_ASSERT(modules.entries[i].name[0] != '\0');
		TEST_ASSERT(modules.entries[i].register_module != nullptr);

		for (uint32_t j = i + 1; j < modules.count; ++j)
			TEST_ASSERT(std::strcmp(modules.entries[i].name, modules.entries[j].name) != 0);
	}

	TEST_SECTION("host selection and lifecycle");
	Neoeter::ScriptHost script;
	script.RegisterHost("fake", &CreateFakeHost);

	Neoeter::ScriptContext context;
	context.commandLine = "-test";
	context.debug = true;

	TEST_ASSERT(script.Start(kManifest, context) == Neoeter::ScriptHost::Result::Success);
	TEST_ASSERT(script.IsRunning());
	TEST_ASSERT(g_fake.loads == 1);
	TEST_ASSERT(g_fake.pack == "root");
	TEST_ASSERT(g_fake.entry == "entry.py");
	TEST_ASSERT(g_fake.commandLine == "-test");
	TEST_ASSERT(g_fake.debug);
	TEST_ASSERT(g_fake.modules == 33);

	script.Stop();
	TEST_ASSERT(!script.IsRunning());
	TEST_ASSERT(g_fake.unloads == 1);
	TEST_ASSERT(g_fake.destroys == 1);

	TEST_SECTION("unknown host");
	TEST_ASSERT(script.Start("version=1\nname=root\nhost=none\nentry=entry.py\n", context) == Neoeter::ScriptHost::Result::HostUnavailable);
	TEST_ASSERT(!script.IsRunning());
	TEST_ASSERT(script.LastError().find("none") != std::string::npos);
	TEST_ASSERT(g_fake.loads == 1);

	TEST_SECTION("host load failure");
	g_fake.fail = true;
	TEST_ASSERT(script.Start(kManifest, context) == Neoeter::ScriptHost::Result::LoadError);
	TEST_ASSERT(!script.IsRunning());
	TEST_ASSERT(script.LastError() == "fake host rejected the pack");
	g_fake.fail = false;

	TEST_SECTION("manifest failure");
	TEST_ASSERT(script.Start("name=root\n", context) == Neoeter::ScriptHost::Result::ManifestError);
	TEST_ASSERT(!script.IsRunning());

	std::printf("[PASS] script seam\n");
	return 0;
}
