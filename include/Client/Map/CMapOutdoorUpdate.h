#pragma once

#include <Client/Map/MapType.h>
#include <Client/Map/MapOutdoorTypes.h>
#include <Client/Map/Area.h>
#include <Client/Map/TerrainPatch.h>
#include <Client/Map/TerrainQuadtree.h>

#include <cstdint>

class CMapOutdoor;

class CMapOutdoorUpdate
{
	public:
		explicit CMapOutdoorUpdate(CMapOutdoor& rkOwner);

		bool Update(float fX, float fY, float fZ);
		void UpdateAroundAmbience(float fX, float fY, float fZ);

		void AssignPatch(long lPatchNum, long lx0, long ly0, long lx1, long ly1);
		void ConvertTerrainToTnL(long lx, long ly);
		virtual void UpdateAreaList(long lCenterX, long lCenterY);
		void UpdateQuadTreeHeights(CTerrainQuadtreeNode* Node);
		void UpdateSky();
		void UpdateTerrain(float fX, float fY);
		virtual void __ClearGarvage();
		void __CollectCollisionPCBlocker(D3DXVECTOR3& v3Eye, D3DXVECTOR3& v3Target, float fDistance);
		void __CollectCollisionShadowReceiver(D3DXVECTOR3& v3Target, D3DXVECTOR3& v3Light);
		void __CollectShadowReceiver(D3DXVECTOR3& v3Target, D3DXVECTOR3& v3Light);
		void __Game_UpdateArea(D3DXVECTOR3& v3Player);
		bool __IsInPCBlockerList(CGraphicObjectInstance* pkObjInstTest);
		bool __IsInShadowReceiverList(CGraphicObjectInstance* pkObjInstTest);
		void __NEW_WorldEditor_UpdateArea();
		void __UpdateArea(D3DXVECTOR3& v3Player);
		void __UpdateAroundAreaList();
		virtual void __UpdateGarvage();

	private:
		CMapOutdoor& m_rkOwner;
};
