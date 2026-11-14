#pragma once

#include <Neoeter/Render/GrpBase.h>

class CScreenDepthOfField : public CGraphicBase
{
	public:
		static CScreenDepthOfField& Instance();

		CScreenDepthOfField();
		virtual ~CScreenDepthOfField();

		void SetEnabled(bool bEnabled);
		bool IsEnabled() const;

		void SetUseDepthOfField(bool bUse);
		bool IsUseDepthOfField() const;
		bool IsDepthOfFieldActive() const;

		void SetBlurRadius(float fRadius);
		float GetBlurRadius() const;

		void SetAutoFocus(bool bAuto);
		bool IsAutoFocus() const;
		void SetWorldFocusDistance(float fDistance);
		float GetWorldFocusDistance() const;
		void SetWorldFocusRange(float fRange);
		float GetWorldFocusRange() const;

		void SetFogEnabled(bool bEnabled);
		bool IsFogEnabled() const;
		void SetFogNear(float fNear);
		float GetFogNear() const;
		void SetFogFar(float fFar);
		float GetFogFar() const;
		void SetSkyFogStrength(float fStrength);
		float GetSkyFogStrength() const;

		void SetFocusOffset(float fOffset);
		float GetFocusOffset() const;

		void SetFocusBand(float fBand);
		float GetFocusBand() const;

		void SetFocusDistance(float fDistance);
		float GetFocusDistance() const;

		void SetFocusFalloff(float fFalloff);
		float GetFocusFalloff() const;

		float GetHorizonY() const;

		void BeginScene(float fNear, float fFar);
		void EndScene();
		void ReleaseResources();

	protected:
		struct TDofVertex
		{
				float x;
				float y;
				float z;
				float rhw;
				DWORD color;
				float u;
				float v;
		};

		struct TDofMaskVertex
		{
				float x;
				float y;
				float z;
				float rhw;
				DWORD color;
				float u0;
				float v0;
				float u1;
				float v1;
		};

		bool __EnsureResources();
		bool __CreateResources(int iWidth, int iHeight);
		void __DestroyResources();

		bool __EnsureMaskTexture();
		bool __CreateMaskTexture();
		void __UpdateMaskTexture();

		bool __PostProcessDepthOfField();

		void __BindRenderTarget(IDirect3DSurface9* pColorSurface, IDirect3DSurface9* pDepthSurface, int iWidth, int iHeight);
		void __DrawScreenQuad(IDirect3DTexture9* pTexture, int iWidth, int iHeight, float fY0, float fY1, float fUOffset, float fVOffset, DWORD dwTopColor, DWORD dwBottomColor);
		void __BlurPass(IDirect3DTexture9* pSource, int iTargetIndex, int iPass, bool bHorizontal);
		void __DownsampleScene();
		void __Composite();
		void __DrawFocusRestore();
		void __ComputeFocusPoint();
		float __ComputeHorizonY() const;

	protected:
		bool m_bEnabled;
		bool m_bInScene;
		bool m_bUseDepthOfField;
		bool m_bDepthOfFieldActive;

		float m_fBlurRadius;
		float m_fFocusOffset;
		float m_fFocusBand;
		float m_fHorizonY;

		float m_fFocusDistance;
		float m_fFocusFalloff;
		float m_fFocusScreenX;
		float m_fFocusScreenY;

		float m_fMaskDistance;
		float m_fMaskFalloff;

		bool m_bAutoFocus;
		float m_fWorldFocusDistance;
		float m_fWorldFocusRange;
		float m_fAutoFocusDistance;

		float m_fNearZ;
		float m_fFarZ;

		bool m_bFogEnabled;
		float m_fFogNear;
		float m_fFogFar;
		float m_fSkyFogStrength;

		int m_iWidth;
		int m_iHeight;

		D3DVIEWPORT9 m_BackupViewport;

		LPDIRECT3DTEXTURE9 m_lpMaskTexture;

		LPDIRECT3DTEXTURE9 m_lpSceneTexture;
		LPDIRECT3DSURFACE9 m_lpSceneSurface;
		LPDIRECT3DTEXTURE9 m_lpDepthTexture;

		LPDIRECT3DTEXTURE9 m_lpBlurTexture[2];
		LPDIRECT3DSURFACE9 m_lpBlurSurface[2];
		LPDIRECT3DSURFACE9 m_lpBlurDepthSurface[2];
};
