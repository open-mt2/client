#include <Client/Script/PackManifest.h>

#include <cstddef>
#include <vector>

namespace OMT2Client::Script
{
	namespace
	{
		const uint32_t kSupportedVersion = 1;

		std::string Trim(const std::string& text)
		{
			const std::string::size_type first = text.find_first_not_of(" \t\r");
			if (first == std::string::npos)
				return "";

			const std::string::size_type last = text.find_last_not_of(" \t\r");
			return text.substr(first, last - first + 1);
		}

		bool ParseVersion(const std::string& text, uint32_t* version)
		{
			if (text.empty())
				return false;

			uint32_t value = 0;
			for (char c : text)
			{
				if (c < '0' || c > '9')
					return false;

				value = value * 10 + uint32_t(c - '0');
				if (value > kSupportedVersion)
					return false;
			}

			*version = value;
			return true;
		}
	}

	PackManifest::Result PackManifest::Parse(const std::string& text, PackManifest* manifest)
	{
		if (!manifest)
			return Result::MissingField;

		PackManifest parsed;
		bool hasVersion = false;

		std::string line;
		for (std::string::size_type start = 0; start <= text.size();)
		{
			const std::string::size_type end = text.find('\n', start);
			line = text.substr(start, end == std::string::npos ? std::string::npos : end - start);
			start = end == std::string::npos ? text.size() + 1 : end + 1;

			line = Trim(line);
			if (line.empty() || line[0] == '#')
				continue;

			const std::string::size_type separator = line.find('=');
			if (separator == std::string::npos)
				return Result::InvalidLine;

			const std::string key = Trim(line.substr(0, separator));
			const std::string value = Trim(line.substr(separator + 1));

			if (key == "version")
			{
				if (!ParseVersion(value, &parsed.m_version))
					return Result::UnsupportedVersion;
				hasVersion = true;
			}
			else if (key == "name")
			{
				parsed.m_name = value;
			}
			else if (key == "host")
			{
				parsed.m_host = value;
			}
			else if (key == "entry")
			{
				parsed.m_entry = value;
			}
		}

		if (!hasVersion || parsed.m_version != kSupportedVersion)
			return Result::UnsupportedVersion;

		if (parsed.m_name.empty() || parsed.m_host.empty() || parsed.m_entry.empty())
			return Result::MissingField;

		*manifest = parsed;
		return Result::Success;
	}

	const char* PackManifest::Describe(Result result)
	{
		switch (result)
		{
			case Result::Success:
				return "success";
			case Result::MissingField:
				return "name, host and entry are required";
			case Result::UnsupportedVersion:
				return "unsupported manifest version";
			case Result::InvalidLine:
				return "a line is not a key=value pair";
		}

		return "unknown manifest error";
	}

	uint32_t PackManifest::Version() const
	{
		return m_version;
	}

	const std::string& PackManifest::Name() const
	{
		return m_name;
	}

	const std::string& PackManifest::Host() const
	{
		return m_host;
	}

	const std::string& PackManifest::Entry() const
	{
		return m_entry;
	}
}
