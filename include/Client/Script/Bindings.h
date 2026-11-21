#pragma once

#include <cstdint>

namespace Neoeter
{
	using RegisterModule = void (*)();

	struct ModuleEntry
	{
		const char* name;
		RegisterModule register_module;
	};

	struct ModuleTable
	{
		const ModuleEntry* entries;
		uint32_t count;
	};

	const ModuleTable& ClientModules();
}
