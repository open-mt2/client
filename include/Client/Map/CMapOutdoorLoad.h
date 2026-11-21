#pragma once

#include <Client/Map/MapType.h>
#include <Client/Map/MapOutdoorTypes.h>
#include <Client/Map/Area.h>
#include <Client/Map/TerrainPatch.h>
#include <Client/Map/TerrainQuadtree.h>

#include <cstdint>

class CMapOutdoor;

class CMapOutdoorLoad
{
	public:
		explicit CMapOutdoorLoad(CMapOutdoor& rkOwner);

		bool Load(float x, float y, float z);

		void AssignTerrainPtr(); // 현재 좌표에서 주위(ex. 3x3)에 있는 것들의 포인터를 연결한다. (업데이트 시 불려짐)
		std::string& GetEnvironmentDataName();
		virtual bool LoadArea(uint16_t wAreaCoordX, uint16_t wAreaCoordY, uint16_t wCellCoordX, uint16_t wCellCoordY);
		bool LoadMonsterAreaInfo();
		bool LoadSetting(const char* c_szFileName);
		virtual bool LoadTerrain(uint16_t wTerrainCoordX, uint16_t wTerrainCoordY, uint16_t wCellCoordX, uint16_t wCellCoordY);
		bool isAreaLoaded(uint16_t wX, uint16_t wY);
		bool isTerrainLoaded(uint16_t wX, uint16_t wY);

	private:
		CMapOutdoor& m_rkOwner;
};
