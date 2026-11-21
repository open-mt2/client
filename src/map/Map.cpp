#include <Client/Map/Map.h>

namespace
{
	Map::IQuery* s_query = nullptr;
	Map::TBlockerPredicate s_actorBlockerPredicate = nullptr;
}

namespace Map
{
	void SetQuery(IQuery* query)
	{
		s_query = query;
	}

	IQuery* GetQuery()
	{
		return s_query;
	}

	float HeightAt(float x, float y)
	{
		if (!s_query)
			return 0.0f;

		return s_query->HeightAt(x, y);
	}

	bool RegionAt(int32_t globalX, int32_t globalY, SRegion* region)
	{
		if (!s_query)
			return false;

		return s_query->RegionAt(globalX, globalY, region);
	}

	void GlobalPositionToLocalPosition(int32_t& x, int32_t& y)
	{
		if (!s_query)
			return;

		s_query->GlobalPositionToLocalPosition(x, y);
	}

	void LocalPositionToGlobalPosition(int32_t& x, int32_t& y)
	{
		if (!s_query)
			return;

		s_query->LocalPositionToGlobalPosition(x, y);
	}

	bool RegionContains(const SRegion& region, int32_t x, int32_t y)
	{
		if (x < static_cast<int32_t>(region.m_dwBaseX) || x >= static_cast<int32_t>(region.m_dwEndX))
			return false;

		if (y < static_cast<int32_t>(region.m_dwBaseY) || y >= static_cast<int32_t>(region.m_dwEndY))
			return false;

		return true;
	}

	void GlobalPositionToLocalPosition(int32_t& x, int32_t& y, uint32_t baseX, uint32_t baseY)
	{
		x -= static_cast<int32_t>(baseX);
		y -= static_cast<int32_t>(baseY);
	}

	void LocalPositionToGlobalPosition(int32_t& x, int32_t& y, uint32_t baseX, uint32_t baseY)
	{
		x += static_cast<int32_t>(baseX);
		y += static_cast<int32_t>(baseY);
	}

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
