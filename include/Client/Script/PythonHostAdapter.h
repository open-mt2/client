#pragma once

#include <Client/Script/ScriptHost.h>

namespace Neoeter
{
	std::unique_ptr<IHost> CreatePythonHost();
}
