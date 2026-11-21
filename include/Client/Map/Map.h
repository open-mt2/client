#pragma once

class CGraphicObjectInstance;

namespace Map
{
	typedef bool (*TBlockerPredicate)(CGraphicObjectInstance* object);

	void SetActorBlockerPredicate(TBlockerPredicate predicate);
	bool IsActorBlocker(CGraphicObjectInstance* object);
}
