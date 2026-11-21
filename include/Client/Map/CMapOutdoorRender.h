#pragma once

#include <Client/Map/MapType.h>
#include <Client/Map/MapOutdoorTypes.h>
#include <Client/Map/Area.h>
#include <Client/Map/TerrainPatch.h>
#include <Client/Map/TerrainQuadtree.h>

#include <cstdint>

class CMapOutdoor;

class CMapOutdoorRender
{
	public:
		explicit CMapOutdoorRender(CMapOutdoor& rkOwner);

		void OnRender();
		void ApplyLight(uint32_t dwVersion, const D3DLIGHT9& c_rkLight);

		CTerrainQuadtreeNode* AllocQuadTreeNode(long x0, long y0, long x1, long y1);
		bool BeginRenderCharacterShadowToTexture();
		void BuildQuadTree();
		void CreateCharacterShadowTexture();
		void DrawPatchAttr(long patchnum);
		void DrawWater(long patchnum);
		void DrawWireFrame(long patchnum, uint16_t wPrimitiveCount, D3DPRIMITIVETYPE ePrimitiveType);
		void EndRenderCharacterShadowToTexture();
		void FreeQuadTree();
		CArea::TCRCWithNumberVector& GetRenderedGraphicThingInstanceNum(uint32_t* pdwGraphicThingInstanceNum, uint32_t* pdwCRCNum);
		std::vector<int>& GetRenderedSplatNum(int* piPatch, int* piSplat, float* pfSplatRatio);
		void InitializeVisibleParts();
		bool IsVisiblePart(int ePart);
		void LoadWaterTexture();
		void NEW_DrawWireFrame(CTerrainPatchProxy* pTerrainPatchProxy, uint16_t wPrimitiveCount, D3DPRIMITIVETYPE ePrimitiveType);
		void RecurseRenderAttr(CTerrainQuadtreeNode* Node, bool bCullEnable = TRUE);
		void ReleaseCharacterShadowTexture();
		void RenderAfterLensFlare();
		void RenderArea(bool bRenderAmbience = true);
		void RenderBeforeLensFlare();
		void RenderBlendArea();
		void RenderCloud();
		void RenderCollision();
		void RenderDungeon();
		void RenderEffect();
		void RenderMarkedArea();
		void RenderPCBlocker();
		void RenderScreenFiltering();
		void RenderSky();
		void RenderTerrain();
		void RenderTree();
		void RenderWater();
		void SelectIndexBuffer(uint8_t byLODLevel, uint16_t* pwPrimitiveCount, D3DPRIMITIVETYPE* pePrimitiveType);
		void SetInverseViewAndDynamicShaodwMatrices();
		void SetPatchDrawVector();
		void SetShadowTextureSize(uint16_t size);
		void SetSplatLimit(int iSplatNum);
		void SetVisiblePart(int ePart, bool isVisible);
		void SubDivideNode(CTerrainQuadtreeNode* Node);
		void UnloadWaterTexture();
		float __GetFogDistance();
		float __GetNoFogDistance();
		void __HardwareTransformPatch_RenderPatchNone(long patchnum, uint16_t wPrimitiveCount, D3DPRIMITIVETYPE ePrimitiveType);
		void __HardwareTransformPatch_RenderPatchSplat(long patchnum, uint16_t wPrimitiveCount, D3DPRIMITIVETYPE ePrimitiveType);
		void __RenderTerrain_AppendPatch(const D3DXVECTOR3& c_rv3Center, float fDistance, long lPatchNum);
		void __RenderTerrain_RecurseRenderQuadTree(CTerrainQuadtreeNode* Node, bool bCullCheckNeed = true);
		int __RenderTerrain_RecurseRenderQuadTree_CheckBoundingCircle(const D3DXVECTOR3& c_v3Center, const float& c_fRadius);
		void __RenderTerrain_RenderHardwareTransformPatch();
		void __RenderTerrain_RenderSoftwareTransformPatch();
		void __SoftwareTransformPatch_ApplyDynamicShadowRenderState();
		void __SoftwareTransformPatch_ApplyFogShadowRenderState();
		void __SoftwareTransformPatch_ApplyRenderState();
		void __SoftwareTransformPatch_ApplyStaticShadowRenderState();
		void __SoftwareTransformPatch_BuildPipeline(SoftwareTransformPatch_SRenderState& rkTPRS);
		bool __SoftwareTransformPatch_Create();
		void __SoftwareTransformPatch_Destroy();
		void __SoftwareTransformPatch_Initialize();
		void __SoftwareTransformPatch_RenderPatchNone(SoftwareTransformPatch_SRenderState& rkTPRS, long patchnum, uint16_t wPrimitiveCount, D3DPRIMITIVETYPE ePrimitiveType);
		void __SoftwareTransformPatch_RenderPatchSplat(SoftwareTransformPatch_SRenderState& rkTPRS, long patchnum, uint16_t wPrimitiveCount, D3DPRIMITIVETYPE ePrimitiveType, bool isFogEnable);
		void __SoftwareTransformPatch_RestoreDynamicShadowRenderState();
		void __SoftwareTransformPatch_RestoreFogShadowRenderState();
		void __SoftwareTransformPatch_RestoreRenderState(uint32_t dwFogEnable);
		void __SoftwareTransformPatch_RestoreStaticShadowRenderState();
		bool __SoftwareTransformPatch_SetShadowStream(SoftwareTransformPatch_STLVertex* akTransVertex);
		bool __SoftwareTransformPatch_SetSplatStream(SoftwareTransformPatch_STLVertex* akTransVertex);
		bool __SoftwareTransformPatch_SetTransform(SoftwareTransformPatch_SRenderState& rkTPRS, SoftwareTransformPatch_STLVertex* akTransVertex, CTerrainPatchProxy& rkTerrainPatchProxy, uint32_t uTerrainX, uint32_t uTerrainY, bool isFogEnable, bool isDynamicShadow);

	private:
		CMapOutdoor& m_rkOwner;
};
