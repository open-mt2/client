#pragma once

#include <Client/Script/Bindings.h>
#include <Client/Script/PackManifest.h>

#include <memory>
#include <string>
#include <vector>

namespace Neoeter
{
	struct ScriptContext
	{
		std::string commandLine;
		bool debug = false;
	};

	class IHost
	{
		public:
			virtual ~IHost() = default;

			virtual bool Load(const PackManifest& manifest, const ScriptContext& context, const ModuleTable& modules, std::string* error) = 0;
			virtual void Unload() = 0;
	};

	class ScriptHost
	{
		public:
			using HostFactory = std::unique_ptr<IHost> (*)();

			enum class Result
			{
				Success,
				ManifestError,
				HostUnavailable,
				LoadError,
			};

			void RegisterHost(const std::string& name, HostFactory factory);

			Result Start(const std::string& manifestText, const ScriptContext& context);
			void Stop();

			bool IsRunning() const;
			const std::string& LastError() const;

		private:
			struct HostEntry
			{
				std::string name;
				HostFactory factory;
			};

			std::vector<HostEntry> m_hosts;
			std::unique_ptr<IHost> m_host;
			std::string m_lastError;
	};
}
