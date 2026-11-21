#pragma once

#include <cstdint>

class CGraphicObjectInstance;

namespace Map
{
	struct SRegion
	{
			const char* m_strName;
			uint32_t m_dwBaseX;
			uint32_t m_dwBaseY;
			uint32_t m_dwEndX;
			uint32_t m_dwEndY;
	};

	class IQuery
	{
		public:
			virtual ~IQuery() {}

			virtual float HeightAt(float x, float y) = 0;
			virtual bool RegionAt(int32_t globalX, int32_t globalY, SRegion* region) = 0;
			virtual void GlobalPositionToLocalPosition(int32_t& x, int32_t& y) = 0;
			virtual void LocalPositionToGlobalPosition(int32_t& x, int32_t& y) = 0;
	};

	typedef bool (*TBlockerPredicate)(CGraphicObjectInstance* object);

	void SetQuery(IQuery* query);
	IQuery* GetQuery();

	float HeightAt(float x, float y);
	bool RegionAt(int32_t globalX, int32_t globalY, SRegion* region);
	void GlobalPositionToLocalPosition(int32_t& x, int32_t& y);
	void LocalPositionToGlobalPosition(int32_t& x, int32_t& y);

	bool RegionContains(const SRegion& region, int32_t x, int32_t y);
	void GlobalPositionToLocalPosition(int32_t& x, int32_t& y, uint32_t baseX, uint32_t baseY);
	void LocalPositionToGlobalPosition(int32_t& x, int32_t& y, uint32_t baseX, uint32_t baseY);

	void SetActorBlockerPredicate(TBlockerPredicate predicate);
	bool IsActorBlocker(CGraphicObjectInstance* object);
}
