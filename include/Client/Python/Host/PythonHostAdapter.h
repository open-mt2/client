#pragma once

#include <Client/Script/ScriptHost.h>

namespace OMT2Client::Python::Host
{
	std::unique_ptr<OMT2Client::Script::IHost> CreatePythonHost();
}
