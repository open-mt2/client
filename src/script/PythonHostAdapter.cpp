#include <Client/Script/PythonHostAdapter.h>

#include <Neoeter/Python/Host/PythonHostApi.h>

#include <cstring>
#include <vector>

namespace Neoeter
{
	namespace
	{
		class PythonHost : public IHost
		{
			public:
				PythonHost()
				{
					neoeter_python_host_create(&m_host);
				}

				~PythonHost() override
				{
					neoeter_python_host_destroy(m_host);
				}

				bool Load(const PackManifest& manifest, const ScriptContext& context, const ModuleTable& modules, std::string* error) override
				{
					if (!m_host)
					{
						*error = "the Python host did not start";
						return false;
					}

					std::vector<neoeter_python_module> rows(modules.count);
					for (uint32_t i = 0; i < modules.count; ++i)
					{
						rows[i].name = modules.entries[i].name;
						rows[i].register_module = modules.entries[i].register_module;
					}

					neoeter_python_pack pack;
					std::memset(&pack, 0, sizeof(pack));
					pack.struct_size = sizeof(pack);
					pack.abi_version = NEOETER_PYTHON_HOST_ABI_VERSION;
					pack.pack_name = manifest.Name().c_str();
					pack.entry = manifest.Entry().c_str();
					pack.command_line = context.commandLine.c_str();
					pack.debug = context.debug ? 1 : 0;
					pack.modules = rows.empty() ? nullptr : rows.data();
					pack.module_count = uint32_t(rows.size());

					if (neoeter_python_host_load(m_host, &pack) != NEOETER_PYTHON_OK)
					{
						*error = neoeter_python_host_last_error(m_host);
						return false;
					}

					return true;
				}

				void Unload() override
				{
					neoeter_python_host_unload(m_host);
				}

			private:
				neoeter_python_host* m_host = nullptr;
		};
	}

	std::unique_ptr<IHost> CreatePythonHost()
	{
		return std::unique_ptr<IHost>(new PythonHost());
	}
}
