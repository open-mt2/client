#pragma once

#include <cstdint>
#include <string>

namespace Neoeter
{
	class PackManifest
	{
		public:
			enum class Result
			{
				Success,
				MissingField,
				UnsupportedVersion,
				InvalidLine,
			};

			static Result Parse(const std::string& text, PackManifest* manifest);
			static const char* Describe(Result result);

			uint32_t Version() const;
			const std::string& Name() const;
			const std::string& Host() const;
			const std::string& Entry() const;

		private:
			uint32_t m_version = 0;
			std::string m_name;
			std::string m_host;
			std::string m_entry;
	};
}
