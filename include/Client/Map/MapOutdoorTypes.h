#pragma once

#include <Client/Map/MapType.h>

struct SoftwareTransformPatch_SData
{
		enum
		{
			SPLAT_VB_NUM = 8,
			NONE_VB_NUM = 8,
		};

		IDirect3DVertexBuffer9* m_pkVBSplat[SPLAT_VB_NUM];
		IDirect3DVertexBuffer9* m_pkVBNone[NONE_VB_NUM];
		uint32_t m_dwSplatPos;
		uint32_t m_dwNonePos;
		uint32_t m_dwLightVersion;
};

struct SoftwareTransformPatch_SRenderState
{
		D3DXMATRIX m_m4Proj;
		D3DXMATRIX m_m4Frustum;
		D3DXMATRIX m_m4DynamicShadow;
		D3DLIGHT9 m_kLight;
		D3DMATERIAL9 m_kMtrl;
		D3DXVECTOR3 m_v3Player;
		uint32_t m_dwFogColor;
		float m_fScreenHalfWidth;
		float m_fScreenHalfHeight;

		float m_fFogNearDistance;
		float m_fFogFarDistance;
		float m_fFogNearTransZ;
		float m_fFogFarTransZ;
		float m_fFogLenInv;
};

struct SoftwareTransformPatch_STVertex
{
		D3DXVECTOR4 kPosition;
};

struct SoftwareTransformPatch_STLVertex
{
		D3DXVECTOR4 kPosition;
		uint32_t dwDiffuse;
		uint32_t dwFog;
		D3DXVECTOR2 kTexTile;
		D3DXVECTOR2 kTexAlpha;
		D3DXVECTOR2 kTexStaticShadow;
		D3DXVECTOR2 kTexDynamicShadow;
};
