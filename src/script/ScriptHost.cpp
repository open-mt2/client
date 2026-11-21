#include <Client/Script/ScriptHost.h>

namespace Neoeter
{
	void ScriptHost::RegisterHost(const std::string& name, HostFactory factory)
	{
		if (name.empty() || !factory)
			return;

		for (HostEntry& entry : m_hosts)
		{
			if (entry.name == name)
			{
				entry.factory = factory;
				return;
			}
		}

		m_hosts.push_back(HostEntry{name, factory});
	}

	ScriptHost::Result ScriptHost::Start(const std::string& manifestText, const ScriptContext& context)
	{
		if (m_host)
			Stop();

		m_lastError.clear();

		PackManifest manifest;
		const PackManifest::Result parseResult = PackManifest::Parse(manifestText, &manifest);
		if (parseResult != PackManifest::Result::Success)
		{
			m_lastError = PackManifest::Describe(parseResult);
			return Result::ManifestError;
		}

		HostFactory factory = nullptr;
		for (const HostEntry& entry : m_hosts)
		{
			if (entry.name == manifest.Host())
			{
				factory = entry.factory;
				break;
			}
		}

		if (!factory)
		{
			m_lastError = "no script host named '" + manifest.Host() + "'";
			return Result::HostUnavailable;
		}

		std::unique_ptr<IHost> host = factory();
		if (!host)
		{
			m_lastError = "the script host could not be created";
			return Result::LoadError;
		}

		std::string error;
		if (!host->Load(manifest, context, ClientModules(), &error))
		{
			m_lastError = error.empty() ? "the pack could not be loaded" : error;
			return Result::LoadError;
		}

		m_host = std::move(host);
		return Result::Success;
	}

	void ScriptHost::Stop()
	{
		if (!m_host)
			return;

		m_host->Unload();
		m_host.reset();
	}

	bool ScriptHost::IsRunning() const
	{
		return m_host != nullptr;
	}

	const std::string& ScriptHost::LastError() const
	{
		return m_lastError;
	}
}
