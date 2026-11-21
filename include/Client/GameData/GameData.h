#pragma once

#include <Client/GameData/FileSource.h>

namespace GameData
{
	// Destroys the item table, then loads item_list.txt, item_proto and
	// itemdesc.txt from the locale path through the installed source. A missing
	// list or description only traces; a missing or malformed proto fails.
	bool LoadItemTables(const char* c_szLocalePath);
}
