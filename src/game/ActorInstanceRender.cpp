#include <Client/Game/StdAfx.h>
#include <Neoeter/Render/StateManager.h>

#include <Client/Game/ActorInstance.h>
#include <Client/Game/CActorInstanceRender.h>

bool CActorInstance::ms_isDirLine = false;

CActorInstanceRender::CActorInstanceRender(CActorInstance& rkOwner) : m_rkOwner(rkOwner)
{
}

bool CActorInstanceRender::IsDirLine()
{
	return CActorInstance::ms_isDirLine;
}

void CActorInstanceRender::ShowDirectionLine(bool isVisible)
{
	CActorInstance::ms_isDirLine = isVisible;
}

void CActorInstanceRender::SetMaterialColor(uint32_t dwColor)
{
	if (m_rkOwner.m_pkHorse)
		m_rkOwner.m_pkHorse->GetRender().SetMaterialColor(dwColor);

	m_rkOwner.m_dwMtrlColor &= 0xff000000;
	m_rkOwner.m_dwMtrlColor |= (dwColor & 0x00ffffff);
}

void CActorInstanceRender::SetMaterialAlpha(uint32_t dwAlpha)
{
	m_rkOwner.m_dwMtrlAlpha = dwAlpha;
}

void CActorInstanceRender::OnRender()
{
	D3DMATERIAL9 kMtrl;
	STATEMANAGER.GetMaterial(&kMtrl);

	kMtrl.Diffuse = D3DXCOLOR(m_rkOwner.m_dwMtrlColor);
	STATEMANAGER.SetMaterial(&kMtrl);

	// 현재는 이렇게.. 최종적인 형태는 Diffuse와 Blend의 분리로..
	// 아니면 이런 형태로 가되 Texture & State Sorting 지원으로.. - [levites]
	STATEMANAGER.SaveRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	switch (m_rkOwner.m_iRenderMode)
	{
		case CActorInstance::RENDER_MODE_NORMAL:
			BeginDiffuseRender();
			m_rkOwner.RenderWithOneTexture();
			EndDiffuseRender();
			BeginOpacityRender();
			m_rkOwner.BlendRenderWithOneTexture();
			EndOpacityRender();
			break;
		case CActorInstance::RENDER_MODE_BLEND:
			if (m_rkOwner.m_fAlphaValue == 1.0f)
			{
				BeginDiffuseRender();
				m_rkOwner.RenderWithOneTexture();
				EndDiffuseRender();
				BeginOpacityRender();
				m_rkOwner.BlendRenderWithOneTexture();
				EndOpacityRender();
			}
			else if (m_rkOwner.m_fAlphaValue > 0.0f)
			{
				BeginBlendRender();
				m_rkOwner.RenderWithOneTexture();
				m_rkOwner.BlendRenderWithOneTexture();
				EndBlendRender();
			}
			break;
		case CActorInstance::RENDER_MODE_ADD:
			BeginAddRender();
			m_rkOwner.RenderWithOneTexture();
			m_rkOwner.BlendRenderWithOneTexture();
			EndAddRender();
			break;
		case CActorInstance::RENDER_MODE_MODULATE:
			BeginModulateRender();
			m_rkOwner.RenderWithOneTexture();
			m_rkOwner.BlendRenderWithOneTexture();
			EndModulateRender();
			break;
	}

	STATEMANAGER.RestoreRenderState(D3DRS_CULLMODE);

	kMtrl.Diffuse = D3DXCOLOR(0xffffffff);
	STATEMANAGER.SetMaterial(&kMtrl);

	if (m_rkOwner.ms_isDirLine)
	{
		D3DXVECTOR3 kD3DVt3Cur(m_rkOwner.m_x, m_rkOwner.m_y, m_rkOwner.m_z);

		D3DXVECTOR3 kD3DVt3LookDir(0.0f, -1.0f, 0.0f);
		D3DXMATRIX kD3DMatLook;
		D3DXMatrixRotationZ(&kD3DMatLook, D3DXToRadian(m_rkOwner.GetRotation()));
		D3DXVec3TransformCoord(&kD3DVt3LookDir, &kD3DVt3LookDir, &kD3DMatLook);
		D3DXVec3Scale(&kD3DVt3LookDir, &kD3DVt3LookDir, 200.0f);
		D3DXVec3Add(&kD3DVt3LookDir, &kD3DVt3LookDir, &kD3DVt3Cur);

		D3DXVECTOR3 kD3DVt3AdvDir(0.0f, -1.0f, 0.0f);
		D3DXMATRIX kD3DMatAdv;
		D3DXMatrixRotationZ(&kD3DMatAdv, D3DXToRadian(m_rkOwner.GetAdvancingRotation()));
		D3DXVec3TransformCoord(&kD3DVt3AdvDir, &kD3DVt3AdvDir, &kD3DMatAdv);
		D3DXVec3Scale(&kD3DVt3AdvDir, &kD3DVt3AdvDir, 200.0f);
		D3DXVec3Add(&kD3DVt3AdvDir, &kD3DVt3AdvDir, &kD3DVt3Cur);

		static CScreen s_kScreen;

		STATEMANAGER.SaveTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		STATEMANAGER.SaveTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		STATEMANAGER.SaveTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
		STATEMANAGER.SetRenderState(D3DRS_LIGHTING, FALSE);

		s_kScreen.SetDiffuseColor(1.0f, 1.0f, 0.0f);
		s_kScreen.RenderLine3d(kD3DVt3Cur.x, kD3DVt3Cur.y, kD3DVt3Cur.z, kD3DVt3AdvDir.x, kD3DVt3AdvDir.y, kD3DVt3AdvDir.z);

		s_kScreen.SetDiffuseColor(0.0f, 1.0f, 1.0f);
		s_kScreen.RenderLine3d(kD3DVt3Cur.x, kD3DVt3Cur.y, kD3DVt3Cur.z, kD3DVt3LookDir.x, kD3DVt3LookDir.y, kD3DVt3LookDir.z);

		STATEMANAGER.SetRenderState(D3DRS_LIGHTING, TRUE);
		STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);

		STATEMANAGER.RestoreTextureStageState(0, D3DTSS_COLORARG1);
		STATEMANAGER.RestoreTextureStageState(0, D3DTSS_COLOROP);
		STATEMANAGER.RestoreTextureStageState(0, D3DTSS_ALPHAOP);
		STATEMANAGER.RestoreFVF();
	}
}

void CActorInstanceRender::BeginDiffuseRender()
{
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	STATEMANAGER.SaveRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CActorInstanceRender::EndDiffuseRender()
{
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHABLENDENABLE);
}

void CActorInstanceRender::BeginOpacityRender()
{
	STATEMANAGER.SaveRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	STATEMANAGER.SaveRenderState(D3DRS_ALPHAREF, 0);
	STATEMANAGER.SaveRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

void CActorInstanceRender::EndOpacityRender()
{
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHATESTENABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHAREF);
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHAFUNC);
}

void CActorInstanceRender::BeginBlendRender()
{
	STATEMANAGER.SaveRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	STATEMANAGER.SaveRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	STATEMANAGER.SaveRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	STATEMANAGER.SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, m_rkOwner.m_fAlphaValue));
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
}

void CActorInstanceRender::EndBlendRender()
{
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHABLENDENABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_SRCBLEND);
	STATEMANAGER.RestoreRenderState(D3DRS_DESTBLEND);
}

void CActorInstanceRender::BeginAddRender()
{
	STATEMANAGER.SetRenderState(D3DRS_TEXTUREFACTOR, m_rkOwner.m_AddColor);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	STATEMANAGER.SaveRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CActorInstanceRender::EndAddRender()
{
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHABLENDENABLE);
}

void CActorInstanceRender::RestoreRenderMode()
{
	// NOTE : This is temporary code. I wanna convert this code to that restore the mode to
	//        model's default setting which had has as like specular or normal. - [levites]
	m_rkOwner.m_iRenderMode = CActorInstance::RENDER_MODE_NORMAL;
	if (m_rkOwner.m_kBlendAlpha.m_isBlending)
	{
		m_rkOwner.m_kBlendAlpha.m_iOldRenderMode = m_rkOwner.m_iRenderMode;
	}
}

void CActorInstanceRender::SetAddRenderMode()
{
	m_rkOwner.m_iRenderMode = CActorInstance::RENDER_MODE_ADD;
	if (m_rkOwner.m_kBlendAlpha.m_isBlending)
	{
		m_rkOwner.m_kBlendAlpha.m_iOldRenderMode = m_rkOwner.m_iRenderMode;
	}
}

void CActorInstanceRender::SetRenderMode(int iRenderMode)
{
	m_rkOwner.m_iRenderMode = iRenderMode;
	if (m_rkOwner.m_kBlendAlpha.m_isBlending)
	{
		m_rkOwner.m_kBlendAlpha.m_iOldRenderMode = iRenderMode;
	}
}

void CActorInstanceRender::SetAddColor(const D3DXCOLOR& c_rColor)
{
	m_rkOwner.m_AddColor = c_rColor;
	m_rkOwner.m_AddColor.a = 1.0f;
}

void CActorInstanceRender::BeginModulateRender()
{
	STATEMANAGER.SetRenderState(D3DRS_TEXTUREFACTOR, m_rkOwner.m_AddColor);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	STATEMANAGER.SaveRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CActorInstanceRender::EndModulateRender()
{
	STATEMANAGER.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	STATEMANAGER.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	STATEMANAGER.RestoreRenderState(D3DRS_ALPHABLENDENABLE);
}

void CActorInstanceRender::SetModulateRenderMode()
{
	m_rkOwner.m_iRenderMode = CActorInstance::RENDER_MODE_MODULATE;
	if (m_rkOwner.m_kBlendAlpha.m_isBlending)
	{
		m_rkOwner.m_kBlendAlpha.m_iOldRenderMode = m_rkOwner.m_iRenderMode;
	}
}

void CActorInstanceRender::RenderCollisionData()
{
	static CScreen s_Screen;

	STATEMANAGER.SetRenderState(D3DRS_LIGHTING, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_rkOwner.m_pAttributeInstance)
	{
		for (uint32_t col = 0; col < m_rkOwner.GetCollisionInstanceCount(); ++col)
		{
			CBaseCollisionInstance* pInstance = m_rkOwner.GetCollisionInstanceData(col);
			pInstance->Render();
		}
	}

	STATEMANAGER.SetRenderState(D3DRS_ZENABLE, FALSE);
	s_Screen.SetColorOperation();
	s_Screen.SetDiffuseColor(1.0f, 0.0f, 0.0f);
	CActorInstance::TCollisionPointInstanceList::iterator itor;
	/*itor = m_AttackingPointInstanceList.begin();
	for (; itor != m_AttackingPointInstanceList.end(); ++itor)
	{
		const CActorInstance::TCollisionPointInstance & c_rInstance = *itor;
		for (uint32_t i = 0; i < c_rInstance.SphereInstanceVector.size(); ++i)
		{
			const CDynamicSphereInstance & c_rSphereInstance = c_rInstance.SphereInstanceVector[i];
			s_Screen.RenderCircle3d(c_rSphereInstance.v3Position.x,
									c_rSphereInstance.v3Position.y,
									c_rSphereInstance.v3Position.z,
									c_rSphereInstance.fRadius);
		}
	}*/
	s_Screen.SetDiffuseColor(1.0f, (m_rkOwner.isShow()) ? 1.0f : 0.0f, 0.0f);
	D3DXVECTOR3 center;
	float r;
	m_rkOwner.GetBoundingSphere(center, r);
	s_Screen.RenderCircle3d(center.x, center.y, center.z, r);

	s_Screen.SetDiffuseColor(0.0f, 0.0f, 1.0f);
	itor = m_rkOwner.m_DefendingPointInstanceList.begin();
	for (; itor != m_rkOwner.m_DefendingPointInstanceList.end(); ++itor)
	{
		const CActorInstance::TCollisionPointInstance& c_rInstance = *itor;
		for (uint32_t i = 0; i < c_rInstance.SphereInstanceVector.size(); ++i)
		{
			const CDynamicSphereInstance& c_rSphereInstance = c_rInstance.SphereInstanceVector[i];
			s_Screen.RenderCircle3d(c_rSphereInstance.v3Position.x,
									c_rSphereInstance.v3Position.y,
									c_rSphereInstance.v3Position.z,
									c_rSphereInstance.fRadius);
		}
	}

	s_Screen.SetDiffuseColor(0.0f, 1.0f, 0.0f);
	itor = m_rkOwner.m_BodyPointInstanceList.begin();
	for (; itor != m_rkOwner.m_BodyPointInstanceList.end(); ++itor)
	{
		const CActorInstance::TCollisionPointInstance& c_rInstance = *itor;
		for (uint32_t i = 0; i < c_rInstance.SphereInstanceVector.size(); ++i)
		{
			const CDynamicSphereInstance& c_rSphereInstance = c_rInstance.SphereInstanceVector[i];
			s_Screen.RenderCircle3d(c_rSphereInstance.v3Position.x,
									c_rSphereInstance.v3Position.y,
									c_rSphereInstance.v3Position.z,
									c_rSphereInstance.fRadius);
		}
	}

	s_Screen.SetDiffuseColor(1.0f, 0.0f, 0.0f);
	//	if (m_SplashArea.fDisappearingTime > m_rkOwner.GetLocalTime())
	{
		CDynamicSphereInstanceVector::iterator itor = m_rkOwner.m_kSplashArea.SphereInstanceVector.begin();
		for (; itor != m_rkOwner.m_kSplashArea.SphereInstanceVector.end(); ++itor)
		{
			const CDynamicSphereInstance& c_rInstance = *itor;
			s_Screen.RenderCircle3d(c_rInstance.v3Position.x,
									c_rInstance.v3Position.y,
									c_rInstance.v3Position.z,
									c_rInstance.fRadius);
		}
	}

	STATEMANAGER.SetRenderState(D3DRS_ZENABLE, TRUE);
	STATEMANAGER.RestoreRenderState(D3DRS_CULLMODE);
	STATEMANAGER.SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CActorInstanceRender::RenderToShadowMap()
{
	if (CActorInstance::RENDER_MODE_BLEND == m_rkOwner.m_iRenderMode)
		if (m_rkOwner.GetAlphaValue() < 0.5f)
			return;

	m_rkOwner.RenderToShadowMap();

	if (m_rkOwner.m_pkHorse)
		m_rkOwner.m_pkHorse->GetRender().RenderToShadowMap();
}
