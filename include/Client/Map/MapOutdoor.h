#pragma once

#include <Neoeter/Render/SkyBox.h>
#include <Neoeter/Render/LensFlare.h>
#include <Neoeter/Render/ScreenFilter.h>

#include <Neoeter/Terrain/TerrainType.h>
#include <Neoeter/Terrain/TextureSet.h>

#include <Neoeter/Model/SpeedTree/CSpeedTreeDirectX.h>

#include <Client/Map/MapBase.h>
#include <Client/Map/MapOutdoorTypes.h>
#include <Client/Map/CMapOutdoorIndexBuffer.h>
#include <Client/Map/CMapOutdoorLoad.h>
#include <Client/Map/CMapOutdoorRender.h>
#include <Client/Map/CMapOutdoorUpdate.h>
#include <Client/Map/Area.h>
#include <Client/Map/AreaTerrain.h>
#include <Client/Map/AreaLoaderThread.h>

#include <Client/Map/MonsterAreaInfo.h>

#define LOAD_SIZE_WIDTH 1

#define AROUND_AREA_NUM 1 + (LOAD_SIZE_WIDTH * 2) * (LOAD_SIZE_WIDTH * 2) * 2
#define MAX_PREPARE_SIZE 9
#define MAX_MAPSIZE 256 // 0 ~ 255, cellsize 200 = 64km

#define TERRAINPATCH_LODMAX 3

typedef struct SOutdoorMapCoordinate
{
		short m_sTerrainCoordX; // Terrain 좌표
		short m_sTerrainCoordY;
} TOutdoorMapCoordinate;

typedef std::map<const std::string, TOutdoorMapCoordinate> TOutdoorMapCoordinateMap;

class CTerrainPatchProxy;
class CTerrainQuadtreeNode;

class CMapOutdoor : public CMapBase
{
	public:
		enum
		{
			VIEW_NONE = 0,
			VIEW_PART,
			VIEW_ALL,
		};

		enum EPart
		{
			PART_TERRAIN,
			PART_OBJECT,
			PART_CLOUD,
			PART_WATER,
			PART_TREE,
			PART_SKY,
			PART_NUM,
		};

		enum ETerrainRenderSort
		{
			DISTANCE_SORT,
			TEXTURE_SORT,
		};

	public:
		CMapOutdoor();
		virtual ~CMapOutdoor();

		CMapOutdoorRender& GetRender();
		CMapOutdoorIndexBuffer& GetIndexBuffer();
		CMapOutdoorLoad& GetLoad();
		CMapOutdoorUpdate& GetUpdate();

		virtual void OnBeginEnvironment();

	protected:
		bool Initialize();
		void InitializeFog();

		virtual bool Destroy();
		virtual void OnSetEnvironmentDataPtr();
		virtual void OnResetEnvironmentDataPtr();

		virtual void OnRender();

		virtual void OnPreAssignTerrainPtr() {};

	public:
		virtual bool Load(float x, float y, float z);
		virtual float GetHeight(float x, float y);
		virtual float GetCacheHeight(float x, float y);

		virtual bool Update(float fX, float fY, float fZ);
		virtual void UpdateAroundAmbience(float fX, float fY, float fZ);

	public:
		void Clear();

		void ApplyLight(uint32_t dwVersion, const D3DLIGHT9& c_rkLight);
		void SetEnvironmentScreenFilter();
		void SetEnvironmentSkyBox();
		void SetEnvironmentLensFlare();
		void RefreshEnvironmentSky();
		void RefreshEnvironmentLensFlare();
		void ApplyEnvironmentSkyColors();

		void ClearGuildArea();
		void RegisterGuildArea(int isx, int isy, int iex, int iey);

		void VisibleMarkedArea();
		void DisableMarkedArea();

		void SetWireframe(bool bWireFrame);
		bool IsWireframe();

		bool GetPickingPointWithRay(const CRay& rRay, D3DXVECTOR3* v3IntersectPt);
		bool GetPickingPointWithRayOnlyTerrain(const CRay& rRay, D3DXVECTOR3* v3IntersectPt);
		bool GetPickingPoint(D3DXVECTOR3* v3IntersectPt);
		void GetTerrainCount(short* psTerrainCountX, short* psTerrainCountY)
		{
			*psTerrainCountX = m_sTerrainCountX;
			*psTerrainCountY = m_sTerrainCountY;
		}

		bool SetTerrainCount(short sTerrainCountX, short sTerrainCountY);

		// Shadow
		void SetDrawShadow(bool bDrawShadow);
		void SetDrawCharacterShadow(bool bDrawChrShadow);

		uint32_t GetShadowMapColor(float fx, float fy);

	protected:
		bool __PickTerrainHeight(float& fPos, const D3DXVECTOR3& v3Start, const D3DXVECTOR3& v3End, float fStep, float fRayRange, float fLimitRange, D3DXVECTOR3* pv3Pick);

		void SaveAlphaFogOperation();
		void RestoreAlphaFogOperation();

		//////////////////////////////////////////////////////////////////////////
		// New
		//////////////////////////////////////////////////////////////////////////
		// 여러가지 맵들을 얻는다.
		void GetHeightMap(const uint8_t& c_rucTerrainNum, uint16_t** pwHeightMap);
		void GetNormalMap(const uint8_t& c_rucTerrainNum, char** pucNormalMap);

		// Water
		void GetWaterMap(const uint8_t& c_rucTerrainNum, uint8_t** pucWaterMap);
		void GetWaterHeight(uint8_t byTerrainNum, uint8_t byWaterNum, long* plWaterHeight);

		//////////////////////////////////////////////////////////////////////////
		// Terrain
		//////////////////////////////////////////////////////////////////////////
	protected:
		// 데이터
		CTerrain* m_pTerrain[AROUND_AREA_NUM];		  // Terrain
		CTerrainPatchProxy* m_pTerrainPatchProxyList; // CTerrain을 랜더링 할때 실제로 랜더링하는 폴리곤 패치들... Seamless Map 을 위해 CTerrain으로부터 독립...

		long m_lViewRadius;	  // 시야 거리.. 셀단위임..
		float m_fHeightScale; // 높이 스케일... 1.0일때 0~655.35미터까지 표현 가능.

		short m_sTerrainCountX, m_sTerrainCountY; // seamless map 안에 들어가는 Terrain개수

		TOutdoorMapCoordinate m_CurCoordinate; // 현재의 좌표

		long m_lCurCoordStartX, m_lCurCoordStartY;
		TOutdoorMapCoordinate m_PrevCoordinate; // 현재의 좌표
		TOutdoorMapCoordinateMap m_EntryPointMap;

		uint16_t m_wPatchCount;

		//////////////////////////////////////////////////////////////////////////
		// Index Buffer
#ifdef WORLD_EDITOR
		uint16_t* m_pwIndices; /* temp Index buffer */

		CGraphicIndexBuffer m_IndexBuffer;
		uint16_t m_wNumIndices;
#else
		uint16_t* m_pwaIndices[TERRAINPATCH_LODMAX];

		CGraphicIndexBuffer m_IndexBuffer[TERRAINPATCH_LODMAX];
		uint16_t m_wNumIndices[TERRAINPATCH_LODMAX];
#endif
		virtual void DestroyTerrain();

		void CreateTerrainPatchProxyList();
		void DestroyTerrainPatchProxyList();

		//////////////////////////////////////////////////////////////////////////
		// Index Buffer

	public:
		int32_t GetTerrainPointer(uint8_t c_ucTerrainNum, CTerrain** ppTerrain);
		float GetTerrainHeight(float fx, float fy);
		bool GetWaterHeight(int iX, int iY, long* plWaterHeight);
		bool GetNormal(int ix, int iy, D3DXVECTOR3* pv3Normal);

		long GetViewRadius() { return m_lViewRadius; }
		float GetHeightScale() { return m_fHeightScale; }

		const TOutdoorMapCoordinate& GetEntryPoint(const std::string& c_rstrEntryPointName) const;
		void SetEntryPoint(const std::string& c_rstrEntryPointName, const TOutdoorMapCoordinate& c_rOutdoorMapCoordinate);
		const TOutdoorMapCoordinate& GetCurCoordinate() { return m_CurCoordinate; }
		const TOutdoorMapCoordinate& GetPrevCoordinate() { return m_PrevCoordinate; }

		//////////////////////////////////////////////////////////////////////////
		// Area
		//////////////////////////////////////////////////////////////////////////
	protected:
		CArea* m_pArea[AROUND_AREA_NUM]; // Data

		virtual void DestroyArea();

#ifdef WORLD_EDITOR
#endif

		void __BuildDynamicSphereInstanceVector();

		void ConvertToMapCoords(float fx, float fy, int* iCellX, int* iCellY, uint8_t* pucSubCellX, uint8_t* pucSubCellY, uint16_t* pwTerrainNumX, uint16_t* pwTerrainNumY);

	public:
		int32_t GetAreaPointer(const uint8_t c_ucAreaNum, CArea** ppArea);

	public:
		//////////////////////////////////////////////////////////////////////////
		// For Grass
		//////////////////////////////////////////////////////////////////////////
		float GetHeight(float* pPos);
		bool GetBrushColor(float fX, float fY, float* pLowColor, float* pHighColor);
		bool isAttrOn(float fX, float fY, uint8_t byAttr);
		bool GetAttr(float fX, float fY, uint8_t* pbyAttr);
		bool isAttrOn(int iX, int iY, uint8_t byAttr);
		bool GetAttr(int iX, int iY, uint8_t* pbyAttr);

		void SetMaterialDiffuse(float fr, float fg, float fb);
		void SetMaterialAmbient(float fr, float fg, float fb);
		void SetTerrainMaterial(const PR_MATERIAL* pMaterial);

		bool GetTerrainNum(float fx, float fy, uint8_t* pbyTerrainNum);
		bool GetTerrainNumFromCoord(uint16_t wCoordX, uint16_t wCoordY, uint8_t* pbyTerrainNum);

	protected:
		//////////////////////////////////////////////////////////////////////////
		// New
		//////////////////////////////////////////////////////////////////////////
		long m_lCenterX, m_lCenterY;   // Terrain 좌표 내의 셀 좌표...
		long m_lOldReadX, m_lOldReadY; /* Last center */

		//////////////////////////////////////////////////////////////////////////
		// Octree
		//////////////////////////////////////////////////////////////////////////
		CTerrainQuadtreeNode* m_pRootNode;

		struct TPatchDrawStruct
		{
				float fDistance;
				uint8_t byTerrainNum;
				long lPatchNum;
				CTerrainPatchProxy* pTerrainPatchProxy;

				bool operator<(const TPatchDrawStruct& rhs) const
				{
					return fDistance < rhs.fDistance;
				}
		};

	public:
		typedef std::vector<uint8_t> TTerrainNumVector;
		struct FSortPatchDrawStructWithTerrainNum
		{
				static TTerrainNumVector m_TerrainNumVector;
				FSortPatchDrawStructWithTerrainNum()
				{
					m_TerrainNumVector.clear();
				}

				bool operator()(const TPatchDrawStruct& lhs, const TPatchDrawStruct& rhs)
				{
					uint32_t lhsTerrainNumOrder = 0, rhsTerrainNumOrder = 0;
					bool blhsOrderFound = false;
					bool brhsOrderFound = false;

					TTerrainNumVector::iterator lhsIterator = std::find(m_TerrainNumVector.begin(), m_TerrainNumVector.end(), lhs.byTerrainNum);
					TTerrainNumVector::iterator rhsIterator = std::find(m_TerrainNumVector.begin(), m_TerrainNumVector.end(), rhs.byTerrainNum);

					if (lhsIterator != m_TerrainNumVector.end())
					{
						blhsOrderFound = true;
						lhsTerrainNumOrder = lhsIterator - m_TerrainNumVector.begin();
					}
					if (rhsIterator != m_TerrainNumVector.end())
					{
						brhsOrderFound = true;
						rhsTerrainNumOrder = rhsIterator - m_TerrainNumVector.begin();
					}
					if (!brhsOrderFound)
					{
						m_TerrainNumVector.push_back(rhs.byTerrainNum);
						rhsTerrainNumOrder = m_TerrainNumVector.size() - 1;
					}
					if (!blhsOrderFound)
					{
						lhsIterator = std::find(m_TerrainNumVector.begin(), m_TerrainNumVector.end(), lhs.byTerrainNum);
						if (lhsIterator != m_TerrainNumVector.end())
						{
							blhsOrderFound = true;
							lhsTerrainNumOrder = lhsIterator - m_TerrainNumVector.begin();
						}
						if (!blhsOrderFound)
						{
							m_TerrainNumVector.push_back(lhs.byTerrainNum);
							lhsTerrainNumOrder = m_TerrainNumVector.size() - 1;
						}
					}

					return lhsTerrainNumOrder < rhsTerrainNumOrder;
				}
		};

	protected:
		std::vector<std::pair<float, long>> m_PatchVector;
		std::vector<TPatchDrawStruct> m_PatchDrawStructVector;

		bool m_bDrawWireFrame;
		bool m_bDrawShadow;
		bool m_bDrawChrShadow;

		//////////////////////////////////////////////////////////////////////////
		// Water
		D3DXMATRIX m_matBump;
		// Water
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// Alpha Fog
		CGraphicImageInstance m_AlphaFogImageInstance;
		D3DXMATRIX m_matAlphaFogTexture;
		// Alpha Fog
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// Character Shadow
		LPDIRECT3DTEXTURE9 m_lpCharacterShadowMapTexture;
		LPDIRECT3DSURFACE9 m_lpCharacterShadowMapRenderTargetSurface;
		LPDIRECT3DSURFACE9 m_lpCharacterShadowMapDepthSurface;
		D3DVIEWPORT9 m_ShadowMapViewport;
		uint16_t m_wShadowMapSize;

		// Backup Device Context
		LPDIRECT3DSURFACE9 m_lpBackupRenderTargetSurface;
		LPDIRECT3DSURFACE9 m_lpBackupDepthSurface;
		D3DVIEWPORT9 m_BackupViewport;

		// Character Shadow
		//////////////////////////////////////////////////////////////////////////

		// View Frustum Culling
		D3DXPLANE m_plane[6];

		void BuildViewFrustum(const D3DXMATRIX& mat);

		CTextureSet m_TextureSet;

	protected:
		CSkyBox m_SkyBox;
		CLensFlare m_LensFlare;
		CScreenFilter m_ScreenFilter;

	protected:
		D3DXMATRIX m_matWorldForCommonUse;
		D3DXMATRIX m_matViewInverse;

		D3DXMATRIX m_matSplatAlpha;
		D3DXMATRIX m_matStaticShadow;
		D3DXMATRIX m_matDynamicShadow;
		D3DXMATRIX m_matDynamicShadowScale;
		D3DXMATRIX m_matLightView;

		float m_fTerrainTexCoordBase;
		float m_fWaterTexCoordBase;

		float m_fXforDistanceCaculation, m_fYforDistanceCaculation;

	protected:
		typedef std::vector<CTerrain*> TTerrainPtrVector;
		typedef TTerrainPtrVector::iterator TTerrainPtrVectorIterator;
		typedef std::vector<CArea*> TAreaPtrVector;
		typedef TAreaPtrVector::iterator TAreaPtrVectorIterator;

		TTerrainPtrVector m_TerrainVector;
		TTerrainPtrVector m_TerrainDeleteVector;
		TTerrainPtrVector m_TerrainLoadRequestVector;
		TTerrainPtrVector m_TerrainLoadWaitVector;
		TTerrainPtrVectorIterator m_TerrainPtrVectorIterator;

		TAreaPtrVector m_AreaVector;
		TAreaPtrVector m_AreaDeleteVector;
		TAreaPtrVector m_AreaLoadRequestVector;
		TAreaPtrVector m_AreaLoadWaitVector;
		TAreaPtrVectorIterator m_AreaPtrVectorIterator;

		struct FPushToDeleteVector
		{
				enum EDeleteDir
				{
					DELETE_LEFT,
					DELETE_RIGHT,
					DELETE_TOP,
					DELETE_BOTTOM,
				};

				EDeleteDir m_eLRDeleteDir;
				EDeleteDir m_eTBDeleteDir;
				TOutdoorMapCoordinate m_CurCoordinate;

				FPushToDeleteVector(EDeleteDir eLRDeleteDir, EDeleteDir eTBDeleteDir, TOutdoorMapCoordinate CurCoord)
				{
					m_eLRDeleteDir = eLRDeleteDir;
					m_eTBDeleteDir = eTBDeleteDir;
					m_CurCoordinate = CurCoord;
				}
		};

		struct FPushTerrainToDeleteVector : public FPushToDeleteVector
		{
				TTerrainPtrVector m_ReturnTerrainVector;

				FPushTerrainToDeleteVector(EDeleteDir eLRDeleteDir, EDeleteDir eTBDeleteDir, TOutdoorMapCoordinate CurCoord)
					: FPushToDeleteVector(eLRDeleteDir, eTBDeleteDir, CurCoord)
				{
					m_ReturnTerrainVector.clear();
				}

				void operator()(CTerrain* pTerrain);
		};

		struct FPushAreaToDeleteVector : public FPushToDeleteVector
		{
				TAreaPtrVector m_ReturnAreaVector;

				FPushAreaToDeleteVector(EDeleteDir eLRDeleteDir, EDeleteDir eTBDeleteDir, TOutdoorMapCoordinate CurCoord)
					: FPushToDeleteVector(eLRDeleteDir, eTBDeleteDir, CurCoord)
				{
					m_ReturnAreaVector.clear();
				}

				void operator()(CArea* pArea);
		};

	protected:
	protected:
		uint32_t m_dwVisiblePartFlags;

		int m_iRenderedSplatNumSqSum;
		int m_iRenderedSplatNum;
		int m_iRenderedPatchNum;
		std::vector<int> m_RenderedTextureNumVector;
		int m_iSplatLimit;

	protected:
		int m_iPatchTerrainVertexCount;
		int m_iPatchWaterVertexCount;

		int m_iPatchTerrainVertexSize;
		int m_iPatchWaterVertexSize;

		uint32_t m_dwRenderedCRCNum;
		uint32_t m_dwRenderedGraphicThingInstanceNum;

		std::list<RECT> m_rkList_kGuildArea;
		SoftwareTransformPatch_SData m_kSTPD;

		void __SoftwareTransformPatch_BuildPipeline_BuildFogFuncTable(SoftwareTransformPatch_SRenderState& rkTPRS);

	protected:
		std::vector<CGraphicObjectInstance*> m_ShadowReceiverVector;
		std::vector<CGraphicObjectInstance*> m_PCBlockerVector;

	protected:
		float m_fOpaqueWaterDepth;
		CGraphicImageInstance m_WaterInstances[30];

	public:
		float GetOpaqueWaterDepth() { return m_fOpaqueWaterDepth; }
		void SetOpaqueWaterDepth(float fOpaqueWaterDepth) { m_fOpaqueWaterDepth = fOpaqueWaterDepth; }
		void SetTerrainRenderSort(ETerrainRenderSort eTerrainRenderSort) { m_eTerrainRenderSort = eTerrainRenderSort; }
		ETerrainRenderSort GetTerrainRenderSort() { return m_eTerrainRenderSort; }

	protected:
		ETerrainRenderSort m_eTerrainRenderSort;

	protected:
		CGraphicImageInstance m_attrImageInstance;
		CGraphicImageInstance m_BuildingTransparentImageInstance;
		D3DXMATRIX m_matBuildingTransparent;

	protected:
		CDynamicPool<CMonsterAreaInfo> m_kPool_kMonsterAreaInfo;
		TMonsterAreaInfoPtrVector m_MonsterAreaInfoPtrVector;
		TMonsterAreaInfoPtrVectorIterator m_MonsterAreaInfoPtrVectorIterator;

	public:
		CMonsterAreaInfo* AddMonsterAreaInfo(long lOriginX, long lOriginY, long lSizeX, long lSizeY);
		void RemoveAllMonsterAreaInfo();

		uint32_t GetMonsterAreaInfoCount() { return m_MonsterAreaInfoPtrVector.size(); }
		bool GetMonsterAreaInfoFromVectorIndex(uint32_t dwMonsterAreaInfoVectorIndex, CMonsterAreaInfo** ppMonsterAreaInfo);

		CMonsterAreaInfo* AddNewMonsterAreaInfo(long lOriginX, long lOriginY, long lSizeX, long lSizeY,
												CMonsterAreaInfo::EMonsterAreaInfoType eMonsterAreaInfoType,
												uint32_t dwVID, uint32_t dwCount, CMonsterAreaInfo::EMonsterDir eMonsterDir);

	public:
		void GetBaseXY(uint32_t* pdwBaseX, uint32_t* pdwBaseY);
		void SetBaseXY(uint32_t dwBaseX, uint32_t dwBaseY);

		void SetTransparentTree(bool bTransparentTree) { m_bTransparentTree = bTransparentTree; }
		void EnableTerrainOnlyForHeight(bool bFlag) { m_bEnableTerrainOnlyForHeight = bFlag; }
		void EnablePortal(bool bFlag);
		bool IsEnablePortal() { return m_bEnablePortal; }

	protected:
		uint32_t m_dwBaseX;
		uint32_t m_dwBaseY;

		D3DXVECTOR3 m_v3Player;

		bool m_bShowEntirePatchTextureCount;
		bool m_bTransparentTree;
		bool m_bEnableTerrainOnlyForHeight;
		bool m_bEnablePortal;

		// XMas
	private:
		struct SXMasTree
		{
				CSpeedTreeWrapper* m_pkTree;
				int m_iEffectID;
		} m_kXMas;

		void __XMasTree_Initialize();
		void __XMasTree_Create(float x, float y, float z, const char* c_szTreeName, const char* c_szEffName);

	public:
		void XMasTree_Destroy();
		void XMasTree_Set(float x, float y, float z, const char* c_szTreeName, const char* c_szEffName);

		// Special Effect
	private:
		typedef std::map<uint32_t, int> TSpecialEffectMap;
		TSpecialEffectMap m_kMap_dwID_iEffectID;

	public:
		void SpecialEffect_Create(uint32_t dwID, float x, float y, float z, const char* c_szEffName);
		void SpecialEffect_Delete(uint32_t dwID);
		void SpecialEffect_Destroy();

	private:
		struct SHeightCache
		{
				struct SItem
				{
						uint32_t m_dwKey;
						float m_fHeight;
				};

				enum
				{
					HASH_SIZE = 100,
				};

				std::vector<SItem> m_akVct_kItem[HASH_SIZE];

				bool m_isUpdated;
		} m_kHeightCache;

		void __HeightCache_Init();
		void __HeightCache_Update();

	public:
		void SetEnvironmentDataName(const std::string& strEnvironmentDataName);

	protected:
		std::string m_settings_envDataName;
		std::string m_envDataName;

	private:
		friend class CMapOutdoorRender;
		friend class CMapOutdoorIndexBuffer;
		friend class CMapOutdoorLoad;
		friend class CMapOutdoorUpdate;

		CMapOutdoorRender m_kRender;
		CMapOutdoorIndexBuffer m_kIndexBuffer;
		CMapOutdoorLoad m_kLoad;
		CMapOutdoorUpdate m_kUpdate;

		bool m_bSettingTerrainVisible;
};
