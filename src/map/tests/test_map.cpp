#include <Client/Map/Map.h>

#include <cassert>
#include <cstdio>

namespace
{
	const int32_t REGION_BASE_X = 3200;
	const int32_t REGION_BASE_Y = 6400;
	const int32_t REGION_END_X = 9600;
	const int32_t REGION_END_Y = 12800;

	class CFakeQuery : public Map::IQuery
	{
		public:
			float HeightAt(float x, float y) override
			{
				return x + y;
			}

			bool RegionAt(int32_t globalX, int32_t globalY, Map::SRegion* region) override
			{
				if (globalX < REGION_BASE_X || globalX >= REGION_END_X)
					return false;

				if (globalY < REGION_BASE_Y || globalY >= REGION_END_Y)
					return false;

				region->m_strName = "test_region";
				region->m_dwBaseX = REGION_BASE_X;
				region->m_dwBaseY = REGION_BASE_Y;
				region->m_dwEndX = REGION_END_X;
				region->m_dwEndY = REGION_END_Y;
				return true;
			}

			void GlobalPositionToLocalPosition(int32_t& x, int32_t& y) override
			{
				Map::GlobalPositionToLocalPosition(x, y, REGION_BASE_X, REGION_BASE_Y);
			}

			void LocalPositionToGlobalPosition(int32_t& x, int32_t& y) override
			{
				Map::LocalPositionToGlobalPosition(x, y, REGION_BASE_X, REGION_BASE_Y);
			}
	};

	bool IsTestBlocker(CGraphicObjectInstance* object)
	{
		return object == reinterpret_cast<CGraphicObjectInstance*>(1);
	}

	void test_offset_conversion()
	{
		int32_t x = 6400;
		int32_t y = 9600;
		Map::GlobalPositionToLocalPosition(x, y, REGION_BASE_X, REGION_BASE_Y);
		assert(x == 3200);
		assert(y == 3200);

		Map::LocalPositionToGlobalPosition(x, y, REGION_BASE_X, REGION_BASE_Y);
		assert(x == 6400);
		assert(y == 9600);

		std::printf("[PASS] test_offset_conversion\n");
	}

	void test_region_bounds()
	{
		Map::SRegion region;
		region.m_strName = "test_region";
		region.m_dwBaseX = REGION_BASE_X;
		region.m_dwBaseY = REGION_BASE_Y;
		region.m_dwEndX = REGION_END_X;
		region.m_dwEndY = REGION_END_Y;

		assert(Map::RegionContains(region, REGION_BASE_X, REGION_BASE_Y));
		assert(Map::RegionContains(region, REGION_END_X - 1, REGION_END_Y - 1));
		assert(!Map::RegionContains(region, REGION_BASE_X - 1, REGION_BASE_Y));
		assert(!Map::RegionContains(region, REGION_BASE_X, REGION_BASE_Y - 1));
		assert(!Map::RegionContains(region, REGION_END_X, REGION_BASE_Y));
		assert(!Map::RegionContains(region, REGION_BASE_X, REGION_END_Y));

		std::printf("[PASS] test_region_bounds\n");
	}

	void test_query_calls()
	{
		CFakeQuery query;
		Map::SetQuery(&query);
		assert(Map::GetQuery() == &query);

		assert(Map::HeightAt(3.0f, 4.0f) == 7.0f);

		int32_t x = REGION_BASE_X + 10;
		int32_t y = REGION_BASE_Y + 20;
		Map::GlobalPositionToLocalPosition(x, y);
		assert(x == 10);
		assert(y == 20);

		Map::LocalPositionToGlobalPosition(x, y);
		assert(x == REGION_BASE_X + 10);
		assert(y == REGION_BASE_Y + 20);

		Map::SRegion region;
		assert(Map::RegionAt(REGION_BASE_X + 1, REGION_BASE_Y + 1, &region));
		assert(region.m_strName != nullptr);
		assert(region.m_dwBaseX == REGION_BASE_X);
		assert(region.m_dwEndY == REGION_END_Y);
		assert(!Map::RegionAt(REGION_END_X, REGION_BASE_Y, &region));

		std::printf("[PASS] test_query_calls\n");
	}

	void test_no_query()
	{
		Map::SetQuery(nullptr);
		assert(Map::GetQuery() == nullptr);
		assert(Map::HeightAt(3.0f, 4.0f) == 0.0f);

		int32_t x = 10;
		int32_t y = 20;
		Map::GlobalPositionToLocalPosition(x, y);
		assert(x == 10);
		assert(y == 20);

		Map::SRegion region;
		assert(!Map::RegionAt(REGION_BASE_X, REGION_BASE_Y, &region));

		std::printf("[PASS] test_no_query\n");
	}

	void test_actor_blocker()
	{
		assert(!Map::IsActorBlocker(nullptr));
		assert(!Map::IsActorBlocker(reinterpret_cast<CGraphicObjectInstance*>(1)));

		Map::SetActorBlockerPredicate(&IsTestBlocker);
		assert(!Map::IsActorBlocker(nullptr));
		assert(Map::IsActorBlocker(reinterpret_cast<CGraphicObjectInstance*>(1)));

		Map::SetActorBlockerPredicate(nullptr);
		assert(!Map::IsActorBlocker(reinterpret_cast<CGraphicObjectInstance*>(1)));

		std::printf("[PASS] test_actor_blocker\n");
	}
}

int main()
{
	test_offset_conversion();
	test_region_bounds();
	test_query_calls();
	test_no_query();
	test_actor_blocker();

	std::printf("[PASS] Map_QueryTests\n");
	return 0;
}
