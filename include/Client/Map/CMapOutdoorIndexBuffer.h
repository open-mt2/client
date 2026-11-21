#pragma once

#include <Client/Map/MapType.h>
#include <Client/Map/MapOutdoorTypes.h>
#include <Client/Map/Area.h>
#include <Client/Map/TerrainPatch.h>
#include <Client/Map/TerrainQuadtree.h>

#include <cstdint>

class CMapOutdoor;

class CMapOutdoorIndexBuffer
{
	public:
		explicit CMapOutdoorIndexBuffer(CMapOutdoor& rkOwner);

		void ADDLvl1B(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1BL(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1BR(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1L(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1M(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1R(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1T(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1TL(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl1TR(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2B(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2BL(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2BR(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2L(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2M(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2R(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2T(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2TL(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void ADDLvl2TR(uint16_t* pIndices, uint16_t& rwCount, const uint16_t& c_rwCurCount, const uint8_t& c_rucNumLineWarp);
		void SetIndexBuffer();

	private:
		CMapOutdoor& m_rkOwner;
};
