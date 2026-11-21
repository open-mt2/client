#include <Client/Map/StdAfx.h>
#include <Client/Map/ScreenDepthOfField.h>

#include <Neoeter/Render/Camera.h>
#include <Neoeter/Render/StateManager.h>

#include <cmath>

static const uint32_t c_dwDofFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
static const uint32_t c_dwDofMaskFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2;
static const int c_iBlurPassCount = 3;
static const int c_iBlurTapCount = 4;
static const int c_iMaskTextureSize = 128;

CScreenDepthOfField& CScreenDepthOfField::Instance()
{
	static CScreenDepthOfField* s_pInstance = new CScreenDepthOfField;
	return *s_pInstance;
}

CScreenDepthOfField::CScreenDepthOfField()
{
	m_bEnabled = true;
	m_bInScene = false;
	m_bUseDepthOfField = true;
	m_bDepthOfFieldActive = false;

	m_fBlurRadius = 3.0f;
	m_fFocusOffset = 60.0f;
	m_fFocusBand = 180.0f;
	m_fHorizonY = 0.0f;

	m_fFocusDistance = 240.0f;
	m_fFocusFalloff = 120.0f;
	m_fFocusScreenX = 0.0f;
	m_fFocusScreenY = 0.0f;

	m_fMaskDistance = 0.0f;
	m_fMaskFalloff = 0.0f;

	m_bAutoFocus = false;
	m_fWorldFocusDistance = 2900.0f;
	m_fWorldFocusRange = 4400.0f;
	m_fAutoFocusDistance = 2900.0f;

	m_fNearZ = 100.0f;
	m_fFarZ = 25600.0f;

	m_bFogEnabled = true;
	m_fFogNear = 5200.0f;
	m_fFogFar = 24000.0f;
	m_fSkyFogStrength = 0.0f;

	m_iWidth = 0;
	m_iHeight = 0;

	m_BackupViewport.X = 0;
	m_BackupViewport.Y = 0;
	m_BackupViewport.Width = 0;
	m_BackupViewport.Height = 0;
	m_BackupViewport.MinZ = 0.0f;
	m_BackupViewport.MaxZ = 1.0f;

	m_lpMaskTexture = NULL;

	m_lpSceneTexture = NULL;
	m_lpSceneSurface = NULL;
	m_lpDepthTexture = NULL;

	for (int i = 0; i < 2; ++i)
	{
		m_lpBlurTexture[i] = NULL;
		m_lpBlurSurface[i] = NULL;
		m_lpBlurDepthSurface[i] = NULL;
	}
}

CScreenDepthOfField::~CScreenDepthOfField()
{
	__DestroyResources();
}

void CScreenDepthOfField::SetEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
	if (!m_bEnabled)
		m_bDepthOfFieldActive = false;
}

bool CScreenDepthOfField::IsEnabled() const
{
	return m_bEnabled;
}

void CScreenDepthOfField::SetUseDepthOfField(bool bUse)
{
	m_bUseDepthOfField = bUse;
}

bool CScreenDepthOfField::IsUseDepthOfField() const
{
	return m_bUseDepthOfField;
}

bool CScreenDepthOfField::IsDepthOfFieldActive() const
{
	return m_bDepthOfFieldActive;
}

void CScreenDepthOfField::SetAutoFocus(bool bAuto)
{
	m_bAutoFocus = bAuto;
}

bool CScreenDepthOfField::IsAutoFocus() const
{
	return m_bAutoFocus;
}

void CScreenDepthOfField::SetWorldFocusDistance(float fDistance)
{
	m_fWorldFocusDistance = fDistance;
}

float CScreenDepthOfField::GetWorldFocusDistance() const
{
	return m_fWorldFocusDistance;
}

void CScreenDepthOfField::SetWorldFocusRange(float fRange)
{
	m_fWorldFocusRange = fRange;
}

float CScreenDepthOfField::GetWorldFocusRange() const
{
	return m_fWorldFocusRange;
}

void CScreenDepthOfField::SetFogEnabled(bool bEnabled)
{
	m_bFogEnabled = bEnabled;
}

bool CScreenDepthOfField::IsFogEnabled() const
{
	return m_bFogEnabled;
}

void CScreenDepthOfField::SetFogNear(float fNear)
{
	m_fFogNear = fNear;
}

float CScreenDepthOfField::GetFogNear() const
{
	return m_fFogNear;
}

void CScreenDepthOfField::SetFogFar(float fFar)
{
	m_fFogFar = fFar;
}

float CScreenDepthOfField::GetFogFar() const
{
	return m_fFogFar;
}

void CScreenDepthOfField::SetSkyFogStrength(float fStrength)
{
	m_fSkyFogStrength = fStrength;
}

float CScreenDepthOfField::GetSkyFogStrength() const
{
	return m_fSkyFogStrength;
}

void CScreenDepthOfField::SetBlurRadius(float fRadius)
{
	m_fBlurRadius = fRadius;
}

float CScreenDepthOfField::GetBlurRadius() const
{
	return m_fBlurRadius;
}

void CScreenDepthOfField::SetFocusOffset(float fOffset)
{
	m_fFocusOffset = fOffset;
}

float CScreenDepthOfField::GetFocusOffset() const
{
	return m_fFocusOffset;
}

void CScreenDepthOfField::SetFocusBand(float fBand)
{
	m_fFocusBand = fBand;
}

float CScreenDepthOfField::GetFocusBand() const
{
	return m_fFocusBand;
}

void CScreenDepthOfField::SetFocusDistance(float fDistance)
{
	m_fFocusDistance = fDistance;
}

float CScreenDepthOfField::GetFocusDistance() const
{
	return m_fFocusDistance;
}

void CScreenDepthOfField::SetFocusFalloff(float fFalloff)
{
	m_fFocusFalloff = fFalloff;
}

float CScreenDepthOfField::GetFocusFalloff() const
{
	return m_fFocusFalloff;
}

float CScreenDepthOfField::GetHorizonY() const
{
	return m_fHorizonY;
}

void CScreenDepthOfField::ReleaseResources()
{
	__DestroyResources();
}

bool CScreenDepthOfField::__CreateResources(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	const int iBlurWidth = (iWidth / 2) > 0 ? (iWidth / 2) : 1;
	const int iBlurHeight = (iHeight / 2) > 0 ? (iHeight / 2) : 1;

	if (FAILED(ms_lpd3dDevice->CreateTexture(iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_lpSceneTexture, NULL)))
		return false;

	if (FAILED(m_lpSceneTexture->GetSurfaceLevel(0, &m_lpSceneSurface)))
		return false;

	if (FAILED(ms_lpd3dDevice->CreateDepthTexture(iWidth, iHeight, &m_lpDepthTexture)))
		return false;

	for (int i = 0; i < 2; ++i)
	{
		if (FAILED(ms_lpd3dDevice->CreateTexture(iBlurWidth, iBlurHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_lpBlurTexture[i], NULL)))
			return false;

		if (FAILED(m_lpBlurTexture[i]->GetSurfaceLevel(0, &m_lpBlurSurface[i])))
			return false;

		if (FAILED(ms_lpd3dDevice->CreateDepthStencilSurface(iBlurWidth, iBlurHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, FALSE, &m_lpBlurDepthSurface[i], NULL)))
			return false;
	}

	return true;
}

void CScreenDepthOfField::__DestroyResources()
{
	SAFE_RELEASE(m_lpMaskTexture);

	SAFE_RELEASE(m_lpSceneSurface);
	SAFE_RELEASE(m_lpDepthTexture);
	SAFE_RELEASE(m_lpSceneTexture);

	for (int i = 0; i < 2; ++i)
	{
		SAFE_RELEASE(m_lpBlurSurface[i]);
		SAFE_RELEASE(m_lpBlurDepthSurface[i]);
		SAFE_RELEASE(m_lpBlurTexture[i]);
	}

	m_iWidth = 0;
	m_iHeight = 0;
}

bool CScreenDepthOfField::__CreateMaskTexture()
{
	if (FAILED(ms_lpd3dDevice->CreateTexture(c_iMaskTextureSize, c_iMaskTextureSize, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_lpMaskTexture, NULL)))
		return false;

	__UpdateMaskTexture();
	return true;
}

void CScreenDepthOfField::__UpdateMaskTexture()
{
	if (!m_lpMaskTexture)
		return;

	D3DLOCKED_RECT kLockedRect;
	if (FAILED(m_lpMaskTexture->LockRect(0, &kLockedRect, NULL, 0)))
		return;

	const float fHalf = (float)(c_iMaskTextureSize / 2);
	const float fOuter = m_fFocusDistance + m_fFocusFalloff;
	const float fScale = fOuter / fHalf;
	const float fFalloff = m_fFocusFalloff > 1.0f ? m_fFocusFalloff : 1.0f;

	for (int y = 0; y < c_iMaskTextureSize; ++y)
	{
		uint8_t* pRow = (uint8_t*)kLockedRect.pBits + y * kLockedRect.Pitch;
		const float fDy = ((float)y + 0.5f - fHalf) * fScale;

		for (int x = 0; x < c_iMaskTextureSize; ++x)
		{
			const float fDx = ((float)x + 0.5f - fHalf) * fScale;
			const float fDistance = std::sqrt(fDx * fDx + fDy * fDy);

			float fT = (fDistance - m_fFocusDistance) / fFalloff;
			if (fT < 0.0f)
				fT = 0.0f;
			if (fT > 1.0f)
				fT = 1.0f;

			const float fAlpha = 1.0f - fT * fT * (3.0f - 2.0f * fT);
			const uint8_t byAlpha = (uint8_t)(fAlpha * 255.0f);

			uint8_t* pPixel = pRow + x * 4;
			pPixel[0] = 255;
			pPixel[1] = 255;
			pPixel[2] = 255;
			pPixel[3] = byAlpha;
		}
	}

	m_lpMaskTexture->UnlockRect(0);

	m_fMaskDistance = m_fFocusDistance;
	m_fMaskFalloff = m_fFocusFalloff;
}

bool CScreenDepthOfField::__EnsureMaskTexture()
{
	if (!m_lpMaskTexture)
	{
		if (!__CreateMaskTexture())
			return false;
	}

	if (m_fMaskDistance != m_fFocusDistance || m_fMaskFalloff != m_fFocusFalloff)
		__UpdateMaskTexture();

	return true;
}

bool CScreenDepthOfField::__EnsureResources()
{
	if (!ms_lpd3dDevice)
		return false;

	D3DVIEWPORT9 kViewport;
	ms_lpd3dDevice->GetViewport(&kViewport);

	const int iWidth = (int)kViewport.Width;
	const int iHeight = (int)kViewport.Height;

	if (iWidth <= 0 || iHeight <= 0)
		return false;

	if (m_lpSceneTexture && m_iWidth == iWidth && m_iHeight == iHeight)
		return true;

	__DestroyResources();

	if (!__CreateResources(iWidth, iHeight))
	{
		TraceError("CScreenDepthOfField: could not create the render targets (%dx%d)", iWidth, iHeight);
		__DestroyResources();
		return false;
	}

	return true;
}

void CScreenDepthOfField::__ComputeFocusPoint()
{
	m_fFocusScreenX = (float)m_iWidth * 0.5f;
	m_fFocusScreenY = (float)m_iHeight * 0.5f;

	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (!pCamera)
		return;

	D3DXVECTOR3 v3Target = pCamera->GetTarget();

	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;
	D3DXMatrixIdentity(&matWorld);
	STATEMANAGER.GetTransform(D3DTS_VIEW, &matView);
	STATEMANAGER.GetTransform(D3DTS_PROJECTION, &matProjection);

	D3DVIEWPORT9 kViewport = m_BackupViewport;
	D3DXVECTOR3 v3Screen;
	D3DXVec3Project(&v3Screen, &v3Target, &kViewport, &matProjection, &matView, &matWorld);

	if (v3Screen.z < 0.0f || v3Screen.z > 1.0f)
		return;

	m_fFocusScreenX = v3Screen.x;
	m_fFocusScreenY = v3Screen.y;
}

float CScreenDepthOfField::__ComputeHorizonY() const
{
	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (!pCamera)
		return 0.0f;

	D3DXVECTOR3 v3Direction = pCamera->GetView();
	v3Direction.z = 0.0f;

	if (D3DXVec3LengthSq(&v3Direction) < 0.000001f)
		return 0.0f;

	D3DXVec3Normalize(&v3Direction, &v3Direction);

	D3DXVECTOR3 v3Far = pCamera->GetEye() + v3Direction * 100000.0f;

	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;
	D3DXMatrixIdentity(&matWorld);
	STATEMANAGER.GetTransform(D3DTS_VIEW, &matView);
	STATEMANAGER.GetTransform(D3DTS_PROJECTION, &matProjection);

	D3DVIEWPORT9 kViewport = m_BackupViewport;
	D3DXVECTOR3 v3Screen;
	D3DXVec3Project(&v3Screen, &v3Far, &kViewport, &matProjection, &matView, &matWorld);

	return v3Screen.y;
}

void CScreenDepthOfField::BeginScene(float fNear, float fFar)
{
	if (!m_bEnabled || !ms_lpd3dDevice)
		return;

	m_fNearZ = fNear;
	m_fFarZ = fFar;

	ms_lpd3dDevice->GetViewport(&m_BackupViewport);

	if (!__EnsureResources())
		return;

	m_fHorizonY = __ComputeHorizonY();
	__ComputeFocusPoint();

	if (!__EnsureMaskTexture())
		return;

	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (pCamera)
		m_fAutoFocusDistance = D3DXVec3Length(&(pCamera->GetTarget() - pCamera->GetEye()));

	D3DVIEWPORT9 kViewport;
	kViewport.X = 0;
	kViewport.Y = 0;
	kViewport.Width = m_iWidth;
	kViewport.Height = m_iHeight;
	kViewport.MinZ = 0.0f;
	kViewport.MaxZ = 1.0f;
	ms_lpd3dDevice->SetViewport(&kViewport);

	ms_lpd3dDevice->SetRenderTarget(0, m_lpSceneSurface);
	ms_lpd3dDevice->SetDepthTexture(m_lpDepthTexture);
	ms_lpd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF1A1A26, 1.0f, 0);

	m_bInScene = true;
}

void CScreenDepthOfField::__BindRenderTarget(IDirect3DSurface9* pColorSurface, IDirect3DSurface9* pDepthSurface, int iWidth, int iHeight)
{
	ms_lpd3dDevice->SetRenderTarget(0, pColorSurface);
	ms_lpd3dDevice->SetDepthStencilSurface(pDepthSurface);

	D3DVIEWPORT9 kViewport;
	kViewport.X = 0;
	kViewport.Y = 0;
	kViewport.Width = iWidth;
	kViewport.Height = iHeight;
	kViewport.MinZ = 0.0f;
	kViewport.MaxZ = 1.0f;
	ms_lpd3dDevice->SetViewport(&kViewport);
}

void CScreenDepthOfField::__DrawScreenQuad(IDirect3DTexture9* pTexture, int iWidth, int iHeight, float fY0, float fY1, float fUOffset, float fVOffset, uint32_t dwTopColor, uint32_t dwBottomColor)
{
	if (iWidth <= 0 || iHeight <= 0)
		return;

	const float fInvHeight = 1.0f / (float)iHeight;

	const float fU0 = fUOffset;
	const float fU1 = 1.0f + fUOffset;
	const float fV0 = fY0 * fInvHeight + fVOffset;
	const float fV1 = fY1 * fInvHeight + fVOffset;

	STATEMANAGER.SetTexture(0, pTexture);
	STATEMANAGER.SetFVF(c_dwDofFVF);

	TDofVertex aVertices[4] = {
		{0.0f, fY0, 0.0f, 1.0f, dwTopColor, fU0, fV0},
		{(float)iWidth, fY0, 0.0f, 1.0f, dwTopColor, fU1, fV0},
		{0.0f, fY1, 0.0f, 1.0f, dwBottomColor, fU0, fV1},
		{(float)iWidth, fY1, 0.0f, 1.0f, dwBottomColor, fU1, fV1}};

	STATEMANAGER.DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertices, sizeof(TDofVertex));
}

void CScreenDepthOfField::__DownsampleScene()
{
	const int iBlurWidth = (m_iWidth / 2) > 0 ? (m_iWidth / 2) : 1;
	const int iBlurHeight = (m_iHeight / 2) > 0 ? (m_iHeight / 2) : 1;

	__BindRenderTarget(m_lpBlurSurface[0], m_lpBlurDepthSurface[0], iBlurWidth, iBlurHeight);
	ms_lpd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF000000, 1.0f, 0);

	STATEMANAGER.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	__DrawScreenQuad(m_lpSceneTexture, iBlurWidth, iBlurHeight, 0.0f, (float)iBlurHeight, 0.0f, 0.0f, 0xFFFFFFFF, 0xFFFFFFFF);
}

void CScreenDepthOfField::__BlurPass(IDirect3DTexture9* pSource, int iTargetIndex, int iPass, bool bHorizontal)
{
	const int iBlurWidth = (m_iWidth / 2) > 0 ? (m_iWidth / 2) : 1;
	const int iBlurHeight = (m_iHeight / 2) > 0 ? (m_iHeight / 2) : 1;

	__BindRenderTarget(m_lpBlurSurface[iTargetIndex], m_lpBlurDepthSurface[iTargetIndex], iBlurWidth, iBlurHeight);
	ms_lpd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF000000, 1.0f, 0);

	STATEMANAGER.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	STATEMANAGER.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	STATEMANAGER.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	const float fRadius = m_fBlurRadius * (float)(1 << iPass);
	const float fOffsetU = fRadius / (float)m_iWidth;
	const float fOffsetV = fRadius / (float)m_iHeight;

	const uint32_t dwColor = D3DCOLOR_ARGB(64, 255, 255, 255);

	for (int iTap = 0; iTap < c_iBlurTapCount; ++iTap)
	{
		const float fSign = (iTap & 1) ? -1.0f : 1.0f;
		const float fScale = (iTap < 2) ? 0.5f : 1.5f;

		const float fUOffset = bHorizontal ? fSign * fScale * fOffsetU : 0.0f;
		const float fVOffset = bHorizontal ? 0.0f : fSign * fScale * fOffsetV;

		__DrawScreenQuad(pSource, iBlurWidth, iBlurHeight, 0.0f, (float)iBlurHeight, fUOffset, fVOffset, dwColor, dwColor);
	}
}

void CScreenDepthOfField::__DrawFocusRestore()
{
	if (!m_lpMaskTexture || m_fFocusDistance <= 0.0f)
		return;

	const float fOuter = m_fFocusDistance + m_fFocusFalloff;
	if (fOuter <= 1.0f)
		return;

	const float fInvMaskSize = 1.0f / (2.0f * fOuter);

	STATEMANAGER.SetTexture(0, m_lpSceneTexture);
	STATEMANAGER.SetTexture(1, m_lpMaskTexture);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	STATEMANAGER.SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	STATEMANAGER.SetFVF(c_dwDofMaskFVF);

	const float fX0 = 0.0f;
	const float fY0 = 0.0f;
	const float fX1 = (float)m_iWidth;
	const float fY1 = (float)m_iHeight;

	const float fU0 = 0.0f;
	const float fV0 = 0.0f;
	const float fU1 = 1.0f;
	const float fV1 = 1.0f;

	const float fMaskU0 = 0.5f + (fX0 - m_fFocusScreenX) * fInvMaskSize;
	const float fMaskU1 = 0.5f + (fX1 - m_fFocusScreenX) * fInvMaskSize;
	const float fMaskV0 = 0.5f + (fY0 - m_fFocusScreenY) * fInvMaskSize;
	const float fMaskV1 = 0.5f + (fY1 - m_fFocusScreenY) * fInvMaskSize;

	TDofMaskVertex aVertices[4] = {
		{fX0, fY0, 0.0f, 1.0f, 0xFFFFFFFF, fU0, fV0, fMaskU0, fMaskV0},
		{fX1, fY0, 0.0f, 1.0f, 0xFFFFFFFF, fU1, fV0, fMaskU1, fMaskV0},
		{fX0, fY1, 0.0f, 1.0f, 0xFFFFFFFF, fU0, fV1, fMaskU0, fMaskV1},
		{fX1, fY1, 0.0f, 1.0f, 0xFFFFFFFF, fU1, fV1, fMaskU1, fMaskV1}};

	STATEMANAGER.DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertices, sizeof(TDofMaskVertex));
}

void CScreenDepthOfField::__Composite()
{
	__BindRenderTarget(NULL, NULL, m_iWidth, m_iHeight);

	STATEMANAGER.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	__DrawScreenQuad(m_lpSceneTexture, m_iWidth, m_iHeight, 0.0f, (float)m_iHeight, 0.0f, 0.0f, 0xFFFFFFFF, 0xFFFFFFFF);

	float fFocusY = m_fHorizonY + m_fFocusOffset;
	if (fFocusY > 0.0f)
	{
		if (fFocusY > (float)m_iHeight)
			fFocusY = (float)m_iHeight;

		float fRampTop = fFocusY - m_fFocusBand;
		if (fRampTop < 0.0f)
			fRampTop = 0.0f;

		STATEMANAGER.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		STATEMANAGER.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		STATEMANAGER.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		if (fRampTop > 0.0f)
			__DrawScreenQuad(m_lpBlurTexture[0], m_iWidth, m_iHeight, 0.0f, fRampTop, 0.0f, 0.0f, 0xFFFFFFFF, 0xFFFFFFFF);

		if (fFocusY > fRampTop)
			__DrawScreenQuad(m_lpBlurTexture[0], m_iWidth, m_iHeight, fRampTop, fFocusY, 0.0f, 0.0f, 0xFFFFFFFF, 0x00FFFFFF);
	}

	if (m_lpMaskTexture && m_fFocusDistance > 0.0f)
	{
		STATEMANAGER.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		STATEMANAGER.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		STATEMANAGER.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		__DrawFocusRestore();
	}
}

bool CScreenDepthOfField::__PostProcessDepthOfField()
{
	if (!m_lpSceneTexture || !m_lpDepthTexture)
		return false;

	DepthOfFieldParams params;
	params.nearZ = m_fNearZ;
	params.farZ = m_fFarZ;
	params.focusDistance = m_bAutoFocus ? m_fAutoFocusDistance : m_fWorldFocusDistance;
	params.focusRange = m_fWorldFocusRange > 1.0f ? m_fWorldFocusRange : 1.0f;
	params.blurRadius = m_fBlurRadius;

	return SUCCEEDED(ms_lpd3dDevice->PostProcessDepthOfField(m_lpSceneTexture, m_lpDepthTexture, params));
}

void CScreenDepthOfField::EndScene()
{
	if (!m_bInScene || !ms_lpd3dDevice)
		return;

	m_bInScene = false;

	ms_lpd3dDevice->SetRenderTarget(0, NULL);
	ms_lpd3dDevice->SetDepthStencilSurface(NULL);
	ms_lpd3dDevice->SetViewport(&m_BackupViewport);

	if (m_bUseDepthOfField && __PostProcessDepthOfField())
	{
		m_bDepthOfFieldActive = true;
		return;
	}

	m_bDepthOfFieldActive = false;

	LPDIRECT3DBASETEXTURE9 pOldTexture0 = NULL;
	LPDIRECT3DBASETEXTURE9 pOldTexture1 = NULL;
	STATEMANAGER.GetTexture(0, &pOldTexture0);
	STATEMANAGER.GetTexture(1, &pOldTexture1);

	uint32_t dwOldFVF = 0;
	STATEMANAGER.GetFVF(&dwOldFVF);

	STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_ZWRITEENABLE, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	STATEMANAGER.SaveRenderState(D3DRS_LIGHTING, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_FOGENABLE, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	STATEMANAGER.SaveRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	STATEMANAGER.SaveTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	STATEMANAGER.SaveTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	STATEMANAGER.SaveTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	STATEMANAGER.SaveTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	STATEMANAGER.SaveTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	STATEMANAGER.SaveTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	STATEMANAGER.SaveTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	STATEMANAGER.SaveTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SaveSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	STATEMANAGER.SaveSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	STATEMANAGER.SaveSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	STATEMANAGER.SaveSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	__DownsampleScene();

	for (int i = 0; i < c_iBlurPassCount; ++i)
	{
		__BlurPass(m_lpBlurTexture[0], 1, i, true);
		__BlurPass(m_lpBlurTexture[1], 0, i, false);
	}

	__Composite();

	ms_lpd3dDevice->SetViewport(&m_BackupViewport);

	STATEMANAGER.RestoreSamplerState(1, D3DSAMP_ADDRESSV);
	STATEMANAGER.RestoreSamplerState(1, D3DSAMP_ADDRESSU);
	STATEMANAGER.RestoreSamplerState(0, D3DSAMP_ADDRESSV);
	STATEMANAGER.RestoreSamplerState(0, D3DSAMP_ADDRESSU);
	STATEMANAGER.RestoreTextureStageState(1, D3DTSS_ALPHAARG1);
	STATEMANAGER.RestoreTextureStageState(1, D3DTSS_ALPHAOP);
	STATEMANAGER.RestoreTextureStageState(1, D3DTSS_COLORARG1);
	STATEMANAGER.RestoreTextureStageState(1, D3DTSS_COLOROP);
	STATEMANAGER.RestoreTextureStageState(0, D3DTSS_ALPHAARG1);
	STATEMANAGER.RestoreTextureStageState(0, D3DTSS_ALPHAOP);
	STATEMANAGER.RestoreTextureStageState(0, D3DTSS_COLORARG1);
	STATEMANAGER.RestoreTextureStageState(0, D3DTSS_COLOROP);
	STATEMANAGER.RestoreRenderState(D3DRS_DESTBLEND);
	STATEMANAGER.RestoreRenderState(D3DRS_SRCBLEND);
	STATEMANAGER.RestoreRenderState(D3DRS_FOGENABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_LIGHTING);
	STATEMANAGER.RestoreRenderState(D3DRS_CULLMODE);
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHATESTENABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHABLENDENABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_ZWRITEENABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);

	STATEMANAGER.SetFVF(dwOldFVF);
	STATEMANAGER.SetTexture(0, pOldTexture0);
	STATEMANAGER.SetTexture(1, pOldTexture1);
}
