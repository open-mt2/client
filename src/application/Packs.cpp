#include <Client/Application/Packs.h>

#include <Client/Application/Context.h>

#include <Neoeter/Audio/SoundData.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/Debug.h>
#include <Neoeter/Platform/Base/FileLoader.h>
#include <Neoeter/Platform/Base/MappedFile.h>
#include <Neoeter/Platform/Base/Stl.h>
#include <Neoeter/Render/TextFileLoader.h>
#include <Platform/Platform.h>

namespace Neoeter
{
	bool Packs::Mount(const std::string& folder)
	{
		if (_access(folder.c_str(), 0) != 0)
		{
			TraceError("Pack folder '%s' not found; the client will run without pack assets", folder.c_str());
			return true;
		}

		bool packFirst = true;
		const std::string packFolder = folder + "/";
		std::string fileName;

		if (Context::IsDistribute())
		{
			Tracef("Info: Pack search mode set to archive-first.\n");
			fileName = packFolder + "Index";
		}
		else if (_access((packFolder + "Index.dev").c_str(), 0) == 0)
		{
			packFirst = false;
			Tracef("Info: Pack search mode set to file-first.\n");
			fileName = packFolder + "Index.dev";
		}
		else
		{
			packFirst = true;
			Tracef("Info: Pack search mode set to archive-first (fallback).\n");
			fileName = packFolder + "Index";
		}

		CMappedFile file;
		LPCVOID data = nullptr;

		if (!file.Create(fileName.c_str(), &data, 0, 0))
		{
			TraceError("Pack index not found: %s; the client will run without pack assets", fileName.c_str());
			return true;
		}

		CMemoryTextFileLoader textLoader;
		textLoader.Bind(file.Size(), data);

		CTextFileLoader::SetCacheMode();
		CEterPackManager::Instance().SetSearchMode(packFirst);

		CSoundData::SetPackMode();

		std::string packName;
		std::string texCachePackName;
		CTokenVector tokens;

		const DWORD lineCount = textLoader.GetLineCount();
		if (lineCount < 2)
		{
			TraceError("Pack index '%s' is empty; the client will run without pack assets", fileName.c_str());
			return true;
		}

		for (DWORD i = 0; i + 1 < lineCount; ++i)
		{
			textLoader.SplitLineByTab(i, &tokens);

			if (tokens.size() != 2)
			{
				TraceError("Invalid number of tokens on line %d: expected %d, got %d!", i, 2, tokens.size());
				return false;
			}

			const std::string& name = tokens[0];
			const std::string& container = tokens[1];

			packName = packFolder + name;
			CEterPackManager::Instance().RegisterPack(packName, container);

			texCachePackName = packName + "_texcache";
			if (CEterPackManager::Instance().PackExists(texCachePackName, container))
				CEterPackManager::Instance().RegisterPack(texCachePackName, container);
		}

		return true;
	}
}
