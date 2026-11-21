#include <Client/Map/Map.h>

namespace
{
	Map::TBlockerPredicate s_actorBlockerPredicate = nullptr;
}

namespace Map
{
	void SetActorBlockerPredicate(TBlockerPredicate predicate)
	{
		s_actorBlockerPredicate = predicate;
	}

	bool IsActorBlocker(CGraphicObjectInstance* object)
	{
		if (!s_actorBlockerPredicate)
			return false;

		return s_actorBlockerPredicate(object);
	}
}
