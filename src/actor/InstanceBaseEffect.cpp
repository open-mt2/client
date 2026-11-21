#include <Client/UI/StdAfx.h>
#include <Client/Actor/InstanceBase.h>
#include <Client/Actor/InstanceBaseEffect.h>
#include <Client/Feature/TextTail/PythonTextTail.h>
#include <Client/UI/AbstractApplication.h>
#include <Client/UI/AbstractPlayer.h>
#include <Client/Player/PythonPlayer.h>
#include <Client/Feature/System/PythonSystem.h>

#include <Neoeter/Effect/EffectManager.h>
#include <Neoeter/Effect/ParticleSystemData.h>
#include <Neoeter/Render/Camera.h>

float CInstanceBase::ms_fDustGap;
float CInstanceBase::ms_fHorseDustGap;
uint32_t CInstanceBase::ms_adwCRCAffectEffect[CInstanceBase::EFFECT_NUM];
std::string CInstanceBase::ms_astAffectEffectAttachBone[CInstanceBase::EFFECT_NUM];

#define BYTE_COLOR_TO_D3DX_COLOR(r, g, b) D3DXCOLOR(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, 1.0f)

/*
D3DXCOLOR CInstanceBaseEffect::ms_kD3DXClrPC(0xFFFFD84D);//1.0f, 0.8470f, 0.3f, 1.0f
D3DXCOLOR CInstanceBaseEffect::ms_kD3DXClrNPC(0xFF7BE85E);//0.4823f, 0.9098f, 0.3686f, 1.0f
D3DXCOLOR CInstanceBaseEffect::ms_kD3DXClrMOB(0xFFEC170a);//0.9254f, 0.0901f, 0.0392f, 1.0f
D3DXCOLOR CInstanceBaseEffect::ms_kD3DXClrPVP(0xFF8532D9);
D3DXCOLOR CInstanceBaseEffect::ms_kD3DXClrPVPSelf(0xFFEE36DF);
D3DXCOLOR CInstanceBaseEffect::ms_kD3DXClrKiller = BYTE_COLOR_TO_D3DX_COLOR(180, 100, 0);
D3DXCOLOR CInstanceBaseEffect::ms_kD3DXClrTitle[CInstanceBaseEffect::TITLE_MAX_NUM] =
{
	BYTE_COLOR_TO_D3DX_COLOR(  0, 204, 255),
	BYTE_COLOR_TO_D3DX_COLOR(  0, 144, 255),
	BYTE_COLOR_TO_D3DX_COLOR( 92, 110, 255),
	BYTE_COLOR_TO_D3DX_COLOR(155, 155, 255),
	0xFFFFFFFF, // None
	BYTE_COLOR_TO_D3DX_COLOR(207, 117,   0),
	BYTE_COLOR_TO_D3DX_COLOR(235,  83,   0),
	BYTE_COLOR_TO_D3DX_COLOR(227,   0,   0),
	BYTE_COLOR_TO_D3DX_COLOR(255,   0,   0),
};
*/

D3DXCOLOR g_akD3DXClrTitle[CInstanceBase::TITLE_NUM];
D3DXCOLOR g_akD3DXClrName[CInstanceBase::NAMECOLOR_NUM];

CInstanceBaseEffect::CInstanceBaseEffect(CInstanceBase& rkOwner) : m_rkOwner(rkOwner)
{
}

std::map<int, std::string> g_TitleNameMap;
std::set<uint32_t> g_kSet_dwPVPReadyKey;
std::set<uint32_t> g_kSet_dwPVPKey;
std::set<uint32_t> g_kSet_dwGVGKey;
std::set<uint32_t> g_kSet_dwDUELKey;

bool g_isEmpireNameMode = false;

void CInstanceBaseEffect::SetEmpireNameMode(bool isEnable)
{
	g_isEmpireNameMode = isEnable;

	if (isEnable)
	{
		g_akD3DXClrName[CInstanceBase::NAMECOLOR_MOB] = g_akD3DXClrName[CInstanceBase::NAMECOLOR_EMPIRE_MOB];
		g_akD3DXClrName[CInstanceBase::NAMECOLOR_NPC] = g_akD3DXClrName[CInstanceBase::NAMECOLOR_EMPIRE_NPC];
		g_akD3DXClrName[CInstanceBase::NAMECOLOR_PC] = g_akD3DXClrName[CInstanceBase::NAMECOLOR_NORMAL_PC];

		for (uint32_t uEmpire = 1; uEmpire < CInstanceBase::EMPIRE_NUM; ++uEmpire)
			g_akD3DXClrName[CInstanceBase::NAMECOLOR_PC + uEmpire] = g_akD3DXClrName[CInstanceBase::NAMECOLOR_EMPIRE_PC + uEmpire];
	}
	else
	{
		g_akD3DXClrName[CInstanceBase::NAMECOLOR_MOB] = g_akD3DXClrName[CInstanceBase::NAMECOLOR_NORMAL_MOB];
		g_akD3DXClrName[CInstanceBase::NAMECOLOR_NPC] = g_akD3DXClrName[CInstanceBase::NAMECOLOR_NORMAL_NPC];

		for (uint32_t uEmpire = 0; uEmpire < CInstanceBase::EMPIRE_NUM; ++uEmpire)
			g_akD3DXClrName[CInstanceBase::NAMECOLOR_PC + uEmpire] = g_akD3DXClrName[CInstanceBase::NAMECOLOR_NORMAL_PC];
	}
}

const D3DXCOLOR& CInstanceBaseEffect::GetIndexedNameColor(uint32_t eNameColor)
{
	if (eNameColor >= CInstanceBase::NAMECOLOR_NUM)
	{
		static D3DXCOLOR s_kD3DXClrNameDefault(0xffffffff);
		return s_kD3DXClrNameDefault;
	}

	return g_akD3DXClrName[eNameColor];
}

void CInstanceBaseEffect::AddDamageEffect(uint32_t damage, uint8_t flag, int32_t bSelf, int32_t bTarget)
{
	if (CPythonSystem::Instance().IsShowDamage())
	{
		CInstanceBase::SEffectDamage sDamage;
		sDamage.bSelf = bSelf;
		sDamage.bTarget = bTarget;
		sDamage.damage = damage;
		sDamage.flag = flag;
		m_rkOwner.m_DamageQueue.push_back(sDamage);
	}
}

void CInstanceBaseEffect::ProcessDamage()
{
	if (m_rkOwner.m_DamageQueue.empty())
		return;

	CInstanceBase::SEffectDamage sDamage = m_rkOwner.m_DamageQueue.front();

	m_rkOwner.m_DamageQueue.pop_front();

	uint32_t damage = sDamage.damage;
	uint8_t flag = sDamage.flag;
	int32_t bSelf = sDamage.bSelf;
	int32_t bTarget = sDamage.bTarget;

	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	float cameraAngle = GetDegreeFromPosition2(pCamera->GetTarget().x, pCamera->GetTarget().y, pCamera->GetEye().x, pCamera->GetEye().y);

	uint32_t FONT_WIDTH = 30;

	CEffectManager& rkEftMgr = CEffectManager::Instance();

	D3DXVECTOR3 v3Pos = m_rkOwner.m_GraphicThingInstance.GetPosition();
	v3Pos.z += float(m_rkOwner.m_GraphicThingInstance.GetHeight());

	D3DXVECTOR3 v3Rot = D3DXVECTOR3(0.0f, 0.0f, cameraAngle);

	if ((flag & CInstanceBase::DAMAGE_DODGE) || (flag & CInstanceBase::DAMAGE_BLOCK))
	{
		if (bSelf)
			rkEftMgr.CreateEffect(CInstanceBase::ms_adwCRCAffectEffect[CInstanceBase::EFFECT_DAMAGE_MISS], v3Pos, v3Rot);
		else
			rkEftMgr.CreateEffect(CInstanceBase::ms_adwCRCAffectEffect[CInstanceBase::EFFECT_DAMAGE_TARGETMISS], v3Pos, v3Rot);
		//__AttachEffect(CInstanceBase::EFFECT_DAMAGE_MISS);
		return;
	}
	else if (flag & CInstanceBase::DAMAGE_CRITICAL)
	{
		// rkEftMgr.CreateEffect(CInstanceBase::ms_adwCRCAffectEffect[EFFECT_DAMAGE_CRITICAL],v3Pos,v3Rot);
		// return; 숫자도 표시.
	}

	std::string strDamageType;
	uint32_t rdwCRCEft = 0;
	/*
	if ( (flag & DAMAGE_POISON) )
	{
		strDamageType = "poison_";
		rdwCRCEft = EFFECT_DAMAGE_POISON;
	}
	else
	*/
	{
		if (bSelf)
		{
			strDamageType = "damage_";
			if (m_rkOwner.m_bDamageEffectType == 0)
				rdwCRCEft = CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE;
			else
				rdwCRCEft = CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE2;
			m_rkOwner.m_bDamageEffectType = !m_rkOwner.m_bDamageEffectType;
		}
		else if (bTarget == false)
		{
			strDamageType = "nontarget_";
			rdwCRCEft = CInstanceBase::EFFECT_DAMAGE_NOT_TARGET;
			return; // 현재 적용 안됨.
		}
		else
		{
			strDamageType = "target_";
			rdwCRCEft = CInstanceBase::EFFECT_DAMAGE_TARGET;
		}
	}

	uint32_t index = 0;
	uint32_t num = 0;
	std::vector<std::string> textures;
	while (damage > 0)
	{
		if (index > 7)
		{
			TraceError("ProcessDamage possible infinite loop");
			break;
		}
		num = damage % 10;
		damage /= 10;
		char numBuf[MAX_PATH];
		sprintf(numBuf, "%d.dds", num);
		textures.push_back("ymir work/effect/affect/damagevalue/" + strDamageType + numBuf);

		rkEftMgr.SetEffectTextures(CInstanceBase::ms_adwCRCAffectEffect[rdwCRCEft], textures);

		D3DXMATRIX matrix, matTrans;
		D3DXMatrixIdentity(&matrix);
		matrix._41 = v3Pos.x;
		matrix._42 = v3Pos.y;
		matrix._43 = v3Pos.z;
		D3DXMatrixTranslation(&matrix, v3Pos.x, v3Pos.y, v3Pos.z);
		D3DXMatrixMultiply(&matrix, &pCamera->GetInverseViewMatrix(), &matrix);
		D3DXMatrixTranslation(&matTrans, FONT_WIDTH * index, 0, 0);
		matTrans._41 = -matTrans._41;
		matrix = matTrans * matrix;
		D3DXMatrixMultiply(&matrix, &pCamera->GetViewMatrix(), &matrix);

		rkEftMgr.CreateEffect(CInstanceBase::ms_adwCRCAffectEffect[rdwCRCEft], D3DXVECTOR3(matrix._41, matrix._42, matrix._43), v3Rot);

		textures.clear();

		index++;
	}
}

void CInstanceBaseEffect::AttachSpecialEffect(uint32_t effect)
{
	__AttachEffect(effect);
}

void CInstanceBaseEffect::LevelUp()
{
	__AttachEffect(CInstanceBase::EFFECT_LEVELUP);
}

void CInstanceBaseEffect::SkillUp()
{
	__AttachEffect(CInstanceBase::EFFECT_SKILLUP);
}

void CInstanceBaseEffect::CreateSpecialEffect(uint32_t iEffectIndex)
{
	const D3DXMATRIX& c_rmatGlobal = m_rkOwner.m_GraphicThingInstance.GetTransform();

	uint32_t dwEffectIndex = CEffectManager::Instance().GetEmptyIndex();
	uint32_t dwEffectCRC = CInstanceBase::ms_adwCRCAffectEffect[iEffectIndex];
	CEffectManager::Instance().CreateEffectInstance(dwEffectIndex, dwEffectCRC);
	CEffectManager::Instance().SelectEffectInstance(dwEffectIndex);
	CEffectManager::Instance().SetEffectInstanceGlobalMatrix(c_rmatGlobal);
}

void CInstanceBaseEffect::__EffectContainer_Destroy()
{
	CInstanceBase::SEffectContainer::Dict& rkDctEftID = __EffectContainer_GetDict();

	CInstanceBase::SEffectContainer::Dict::iterator i;
	for (i = rkDctEftID.begin(); i != rkDctEftID.end(); ++i)
		__DetachEffect(i->second);

	rkDctEftID.clear();
}

void CInstanceBaseEffect::__EffectContainer_Initialize()
{
	CInstanceBase::SEffectContainer::Dict& rkDctEftID = __EffectContainer_GetDict();
	rkDctEftID.clear();
}

CInstanceBase::SEffectContainer::Dict& CInstanceBaseEffect::__EffectContainer_GetDict()
{
	return m_rkOwner.m_kEffectContainer.m_kDct_dwEftID;
}

// Return value 를 boolean 에서 ID 로 바꿉니다
uint32_t CInstanceBaseEffect::__EffectContainer_AttachEffect(uint32_t dwEftKey)
{
	CInstanceBase::SEffectContainer::Dict& rkDctEftID = __EffectContainer_GetDict();
	CInstanceBase::SEffectContainer::Dict::iterator f = rkDctEftID.find(dwEftKey);
	if (rkDctEftID.end() != f)
		return 0;

	uint32_t dwEftID = __AttachEffect(dwEftKey);
	rkDctEftID.insert(CInstanceBase::SEffectContainer::Dict::value_type(dwEftKey, dwEftID));
	return dwEftID;
}

void CInstanceBaseEffect::__EffectContainer_DetachEffect(uint32_t dwEftKey)
{
	CInstanceBase::SEffectContainer::Dict& rkDctEftID = __EffectContainer_GetDict();
	CInstanceBase::SEffectContainer::Dict::iterator f = rkDctEftID.find(dwEftKey);
	if (rkDctEftID.end() == f)
		return;

	__DetachEffect(f->second);

	rkDctEftID.erase(f);
}

void CInstanceBaseEffect::__AttachEmpireEffect(uint32_t eEmpire)
{
	if (!m_rkOwner.__IsExistMainInstance())
		return;

	CInstanceBase* pkInstMain = m_rkOwner.__GetMainInstancePtr();

	if (m_rkOwner.IsWarp())
		return;
	if (m_rkOwner.IsObject())
		return;
	if (m_rkOwner.IsFlag())
		return;
	if (m_rkOwner.IsResource())
		return;

	if (pkInstMain->IsGameMaster())
	{
	}
	else
	{
		if (pkInstMain->IsSameEmpire(m_rkOwner))
			return;

		// HIDE_OTHER_EMPIRE_EUNHYEONG_ASSASSIN
		if (m_rkOwner.GetBattle().IsAffect(CInstanceBase::AFFECT_EUNHYEONG))
			return;
		// END_OF_HIDE_OTHER_EMPIRE_EUNHYEONG_ASSASSIN
	}

	if (m_rkOwner.IsGameMaster())
		return;

	__EffectContainer_AttachEffect(CInstanceBase::EFFECT_EMPIRE + eEmpire);
}

void CInstanceBaseEffect::__AttachSelectEffect()
{
	__EffectContainer_AttachEffect(CInstanceBase::EFFECT_SELECT);
}

void CInstanceBaseEffect::__DetachSelectEffect()
{
	__EffectContainer_DetachEffect(CInstanceBase::EFFECT_SELECT);
}

void CInstanceBaseEffect::__AttachTargetEffect()
{
	__EffectContainer_AttachEffect(CInstanceBase::EFFECT_TARGET);
}

void CInstanceBaseEffect::__DetachTargetEffect()
{
	__EffectContainer_DetachEffect(CInstanceBase::EFFECT_TARGET);
}

void CInstanceBaseEffect::__StoneSmoke_Inialize()
{
	m_rkOwner.m_kStoneSmoke.m_dwEftID = 0;
}

void CInstanceBaseEffect::__StoneSmoke_Destroy()
{
	if (!m_rkOwner.m_kStoneSmoke.m_dwEftID)
		return;

	__DetachEffect(m_rkOwner.m_kStoneSmoke.m_dwEftID);
	m_rkOwner.m_kStoneSmoke.m_dwEftID = 0;
}

void CInstanceBaseEffect::__StoneSmoke_Create(uint32_t eSmoke)
{
	m_rkOwner.m_kStoneSmoke.m_dwEftID = m_rkOwner.m_GraphicThingInstance.GetAttach().AttachSmokeEffect(eSmoke);
}

void CInstanceBaseEffect::SetAlpha(float fAlpha)
{
	m_rkOwner.__SetBlendRenderingMode();
	m_rkOwner.__SetAlphaValue(fAlpha);
}

bool CInstanceBaseEffect::UpdateDeleting()
{
	m_rkOwner.Update();
	m_rkOwner.Transform();

	IAbstractApplication& rApp = IAbstractApplication::GetSingleton();

	float fAlpha = m_rkOwner.__GetAlphaValue() - (rApp.GetGlobalElapsedTime() * 1.5f);
	m_rkOwner.__SetAlphaValue(fAlpha);

	if (fAlpha < 0.0f)
		return false;

	return true;
}

void CInstanceBaseEffect::DeleteBlendOut()
{
	m_rkOwner.__SetBlendRenderingMode();
	m_rkOwner.__SetAlphaValue(1.0f);
	DetachTextTail();

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.NotifyDeletingCharacterInstance(m_rkOwner.GetVirtualID());
}

void CInstanceBaseEffect::ClearPVPKeySystem()
{
	g_kSet_dwPVPReadyKey.clear();
	g_kSet_dwPVPKey.clear();
	g_kSet_dwGVGKey.clear();
	g_kSet_dwDUELKey.clear();
}

void CInstanceBaseEffect::InsertPVPKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	uint32_t dwPVPKey = __GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwPVPKey.insert(dwPVPKey);
}

void CInstanceBaseEffect::InsertPVPReadyKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	uint32_t dwPVPReadyKey = __GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwPVPKey.insert(dwPVPReadyKey);
}

void CInstanceBaseEffect::RemovePVPKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	uint32_t dwPVPKey = __GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwPVPKey.erase(dwPVPKey);
}

void CInstanceBaseEffect::InsertGVGKey(uint32_t dwSrcGuildVID, uint32_t dwDstGuildVID)
{
	uint32_t dwGVGKey = __GetPVPKey(dwSrcGuildVID, dwDstGuildVID);
	g_kSet_dwGVGKey.insert(dwGVGKey);
}

void CInstanceBaseEffect::RemoveGVGKey(uint32_t dwSrcGuildVID, uint32_t dwDstGuildVID)
{
	uint32_t dwGVGKey = __GetPVPKey(dwSrcGuildVID, dwDstGuildVID);
	g_kSet_dwGVGKey.erase(dwGVGKey);
}

void CInstanceBaseEffect::InsertDUELKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	uint32_t dwPVPKey = __GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwDUELKey.insert(dwPVPKey);
}

uint32_t CInstanceBaseEffect::__GetPVPKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	if (dwVIDSrc > dwVIDDst)
		std::swap(dwVIDSrc, dwVIDDst);

	uint32_t awSrc[2];
	awSrc[0] = dwVIDSrc;
	awSrc[1] = dwVIDDst;

	const uint8_t* s = (const uint8_t*)awSrc;
	const uint8_t* end = s + sizeof(awSrc);
	unsigned long h = 0;

	while (s < end)
	{
		h *= 16777619;
		h ^= (uint8_t) * (uint8_t*)(s++);
	}

	return h;
}

bool CInstanceBaseEffect::__FindPVPKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	uint32_t dwPVPKey = __GetPVPKey(dwVIDSrc, dwVIDDst);

	if (g_kSet_dwPVPKey.end() == g_kSet_dwPVPKey.find(dwPVPKey))
		return false;

	return true;
}

bool CInstanceBaseEffect::__FindPVPReadyKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	uint32_t dwPVPKey = __GetPVPKey(dwVIDSrc, dwVIDDst);

	if (g_kSet_dwPVPReadyKey.end() == g_kSet_dwPVPReadyKey.find(dwPVPKey))
		return false;

	return true;
}
// 길드전시 상대 길드인지 확인할때.
bool CInstanceBaseEffect::__FindGVGKey(uint32_t dwSrcGuildID, uint32_t dwDstGuildID)
{
	uint32_t dwGVGKey = __GetPVPKey(dwSrcGuildID, dwDstGuildID);

	if (g_kSet_dwGVGKey.end() == g_kSet_dwGVGKey.find(dwGVGKey))
		return false;

	return true;
}
// 대련 모드에서는 대련 상대만 공격할 수 있다.
bool CInstanceBaseEffect::__FindDUELKey(uint32_t dwVIDSrc, uint32_t dwVIDDst)
{
	uint32_t dwDUELKey = __GetPVPKey(dwVIDSrc, dwVIDDst);

	if (g_kSet_dwDUELKey.end() == g_kSet_dwDUELKey.find(dwDUELKey))
		return false;

	return true;
}

bool CInstanceBaseEffect::IsPVPInstance(CInstanceBase& rkInstSel)
{
	uint32_t dwVIDSrc = m_rkOwner.GetVirtualID();
	uint32_t dwVIDDst = rkInstSel.GetVirtualID();

	uint32_t dwGuildIDSrc = m_rkOwner.GetGuildID();
	uint32_t dwGuildIDDst = rkInstSel.GetGuildID();

	if (m_rkOwner.GetDuelMode()) // 대련 모드일때는 ~_~
		return true;

	return __FindPVPKey(dwVIDSrc, dwVIDDst) || __FindGVGKey(dwGuildIDSrc, dwGuildIDDst);
	//__FindDUELKey(dwVIDSrc, dwVIDDst);
}

const D3DXCOLOR& CInstanceBaseEffect::GetNameColor()
{
	return GetIndexedNameColor(GetNameColorIndex());
}

uint32_t CInstanceBaseEffect::GetNameColorIndex()
{
	if (m_rkOwner.IsPC())
	{
		if (m_rkOwner.m_isKiller)
		{
			return CInstanceBase::NAMECOLOR_PK;
		}

		if (m_rkOwner.__IsExistMainInstance() && !m_rkOwner.__IsMainInstance())
		{
			CInstanceBase* pkInstMain = m_rkOwner.__GetMainInstancePtr();
			if (!pkInstMain)
			{
				TraceError("CInstanceBaseEffect::GetNameColorIndex - MainInstance is NULL");
				return CInstanceBase::NAMECOLOR_PC;
			}
			uint32_t dwVIDMain = pkInstMain->GetVirtualID();
			uint32_t dwVIDSelf = m_rkOwner.GetVirtualID();

			if (pkInstMain->GetDuelMode())
			{
				switch (pkInstMain->GetDuelMode())
				{
					case CInstanceBase::DUEL_CANNOTATTACK:
						return CInstanceBase::NAMECOLOR_PC + m_rkOwner.GetEmpireID();
					case CInstanceBase::DUEL_START:
						if (__FindDUELKey(dwVIDMain, dwVIDSelf))
							return CInstanceBase::NAMECOLOR_PVP;
						else
							return CInstanceBase::NAMECOLOR_PC + m_rkOwner.GetEmpireID();
				}
			}

			if (pkInstMain->IsSameEmpire(m_rkOwner))
			{
				if (__FindPVPKey(dwVIDMain, dwVIDSelf))
				{
					return CInstanceBase::NAMECOLOR_PVP;
				}

				uint32_t dwGuildIDMain = pkInstMain->GetGuildID();
				uint32_t dwGuildIDSelf = m_rkOwner.GetGuildID();
				if (__FindGVGKey(dwGuildIDMain, dwGuildIDSelf))
				{
					return CInstanceBase::NAMECOLOR_PVP;
				}
				/*
				if (__FindDUELKey(dwVIDMain, dwVIDSelf))
				{
					return CInstanceBase::NAMECOLOR_PVP;
				}
				*/
			}
			else
			{
				return CInstanceBase::NAMECOLOR_PVP;
			}
		}

		IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
		if (rPlayer.IsPartyMemberByVID(m_rkOwner.GetVirtualID()))
			return CInstanceBase::NAMECOLOR_PARTY;

		return CInstanceBase::NAMECOLOR_PC + m_rkOwner.GetEmpireID();
	}
	else if (m_rkOwner.IsNPC())
	{
		return CInstanceBase::NAMECOLOR_NPC;
	}
	else if (m_rkOwner.IsEnemy())
	{
		return CInstanceBase::NAMECOLOR_MOB;
	}
	else if (m_rkOwner.IsPoly())
	{
		return CInstanceBase::NAMECOLOR_MOB;
	}

	return D3DXCOLOR(0xffffffff);
}

const D3DXCOLOR& CInstanceBaseEffect::GetTitleColor()
{
	uint32_t uGrade = m_rkOwner.GetAlignmentGrade();
	if (uGrade >= CInstanceBase::TITLE_NUM)
	{
		static D3DXCOLOR s_kD3DXClrTitleDefault(0xffffffff);
		return s_kD3DXClrTitleDefault;
	}

	return g_akD3DXClrTitle[uGrade];
}

void CInstanceBaseEffect::AttachTextTail()
{
	if (m_rkOwner.m_isTextTail)
	{
		TraceError("CInstanceBaseEffect::AttachTextTail - VID [%d] ALREADY EXIST", m_rkOwner.GetVirtualID());
		return;
	}

	m_rkOwner.m_isTextTail = true;

	uint32_t dwVID = m_rkOwner.GetVirtualID();

	float fTextTailHeight = m_rkOwner.IsMountingHorse() ? 110.0f : 10.0f;

	static D3DXCOLOR s_kD3DXClrTextTail = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CPythonTextTail::Instance().RegisterCharacterTextTail(m_rkOwner.m_dwGuildID, dwVID, s_kD3DXClrTextTail, fTextTailHeight);

	// CHARACTER_LEVEL
	if (m_rkOwner.m_dwLevel)
	{
		UpdateTextTailLevel(m_rkOwner.m_dwLevel);
	}
}

void CInstanceBaseEffect::DetachTextTail()
{
	if (!m_rkOwner.m_isTextTail)
		return;

	m_rkOwner.m_isTextTail = false;
	CPythonTextTail::Instance().DeleteCharacterTextTail(m_rkOwner.GetVirtualID());
}

void CInstanceBaseEffect::UpdateTextTailLevel(uint32_t level)
{
	// static D3DXCOLOR s_kLevelColor = D3DXCOLOR(119.0f/255.0f, 246.0f/255.0f, 168.0f/255.0f, 1.0f);
	static D3DXCOLOR s_kLevelColor = D3DXCOLOR(152.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f, 1.0f);

	m_rkOwner.m_dwLevel = level;

	char szText[256];
	sprintf(szText, "Lv %d", level);
	CPythonTextTail::Instance().AttachLevel(m_rkOwner.GetVirtualID(), szText, s_kLevelColor);
}

void CInstanceBaseEffect::RefreshTextTail()
{
	CPythonTextTail::Instance().SetCharacterTextTailColor(m_rkOwner.GetVirtualID(), GetNameColor());

	int iAlignmentGrade = m_rkOwner.GetAlignmentGrade();
	if (CInstanceBase::TITLE_NONE == iAlignmentGrade)
	{
		CPythonTextTail::Instance().DetachTitle(m_rkOwner.GetVirtualID());
	}
	else
	{
		std::map<int, std::string>::iterator itor = g_TitleNameMap.find(iAlignmentGrade);
		if (g_TitleNameMap.end() != itor)
		{
			const std::string& c_rstrTitleName = itor->second;
			CPythonTextTail::Instance().AttachTitle(m_rkOwner.GetVirtualID(), c_rstrTitleName.c_str(), GetTitleColor());
		}
	}
}

void CInstanceBaseEffect::RefreshTextTailTitle()
{
	RefreshTextTail();
}

// 2004.07.25.myevan.이펙트 안 붙는 문제 해결
/////////////////////////////////////////////////
void CInstanceBaseEffect::__ClearAffectFlagContainer()
{
	m_rkOwner.m_kAffectFlagContainer.Clear();
}

void CInstanceBaseEffect::__ClearAffects()
{
	if (m_rkOwner.IsStone())
	{
		__StoneSmoke_Destroy();
	}
	else
	{
		for (int iAffect = 0; iAffect < CInstanceBase::AFFECT_NUM; ++iAffect)
		{
			__DetachEffect(m_rkOwner.m_adwCRCAffectEffect[iAffect]);
			m_rkOwner.m_adwCRCAffectEffect[iAffect] = 0;
		}

		__ClearAffectFlagContainer();
	}

	m_rkOwner.m_GraphicThingInstance.__OnClearAffects();
}

/////////////////////////////////////////////////

void CInstanceBaseEffect::__SetNormalAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer)
{
	for (int i = 0; i < CAffectFlagContainer::BIT_SIZE; ++i)
	{
		bool isOldSet = m_rkOwner.m_kAffectFlagContainer.IsSet(i);
		bool isNewSet = c_rkAffectFlagContainer.IsSet(i);

		if (isOldSet != isNewSet)
		{
			__SetAffect(i, isNewSet);

			if (isNewSet)
				m_rkOwner.m_GraphicThingInstance.__OnSetAffect(i);
			else
				m_rkOwner.m_GraphicThingInstance.__OnResetAffect(i);
		}
	}

	m_rkOwner.m_kAffectFlagContainer.CopyInstance(c_rkAffectFlagContainer);
}

void CInstanceBaseEffect::__SetStoneSmokeFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer)
{
	m_rkOwner.m_kAffectFlagContainer.CopyInstance(c_rkAffectFlagContainer);

	uint32_t eSmoke;
	if (m_rkOwner.m_kAffectFlagContainer.IsSet(CInstanceBase::STONE_SMOKE8))
		eSmoke = 3;
	else if (m_rkOwner.m_kAffectFlagContainer.IsSet(CInstanceBase::STONE_SMOKE5) | m_rkOwner.m_kAffectFlagContainer.IsSet(CInstanceBase::STONE_SMOKE6) | m_rkOwner.m_kAffectFlagContainer.IsSet(CInstanceBase::STONE_SMOKE7))
		eSmoke = 2;
	else if (m_rkOwner.m_kAffectFlagContainer.IsSet(CInstanceBase::STONE_SMOKE2) | m_rkOwner.m_kAffectFlagContainer.IsSet(CInstanceBase::STONE_SMOKE3) | m_rkOwner.m_kAffectFlagContainer.IsSet(CInstanceBase::STONE_SMOKE4))
		eSmoke = 1;
	else
		eSmoke = 0;

	__StoneSmoke_Destroy();
	__StoneSmoke_Create(eSmoke);
}

void CInstanceBaseEffect::SetAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer)
{
	if (m_rkOwner.IsBuilding())
	{
		return;
	}
	else if (m_rkOwner.IsStone())
	{
		__SetStoneSmokeFlagContainer(c_rkAffectFlagContainer);
	}
	else
	{
		__SetNormalAffectFlagContainer(c_rkAffectFlagContainer);
	}
}

void CInstanceBaseEffect::SCRIPT_SetAffect(uint32_t eAffect, bool isVisible)
{
	__SetAffect(eAffect, isVisible);
}

void CInstanceBaseEffect::__SetReviveInvisibilityAffect(bool isVisible)
{
	if (isVisible)
	{
		// NOTE : Dress 를 입고 있으면 Alpha 를 넣지 않는다.
		if (m_rkOwner.IsWearingDress())
			return;

		m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(0.5f, 1.0f);
	}
	else
	{
		m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(1.0f, 1.0f);
	}
}

void CInstanceBaseEffect::__Assassin_SetEunhyeongAffect(bool isVisible)
{
	if (isVisible)
	{
		// NOTE : Dress 를 입고 있으면 Alpha 를 넣지 않는다.
		if (m_rkOwner.IsWearingDress())
			return;

		if (m_rkOwner.__IsMainInstance() || m_rkOwner.__MainCanSeeHiddenThing())
		{
			m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(0.5f, 1.0f);
		}
		else
		{
			// 2004.10.16.myevan.은형법 완전 투명
			m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(0.0f, 1.0f);
			m_rkOwner.m_GraphicThingInstance.GetAttach().HideAllAttachingEffect();
		}
	}
	else
	{
		m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(1.0f, 1.0f);
		m_rkOwner.m_GraphicThingInstance.GetAttach().ShowAllAttachingEffect();
	}
}

void CInstanceBaseEffect::__Shaman_SetParalysis(bool isParalysis)
{
	m_rkOwner.m_GraphicThingInstance.SetParalysis(isParalysis);
}

void CInstanceBaseEffect::__Warrior_SetGeomgyeongAffect(bool isVisible)
{
	if (isVisible)
	{
		if (m_rkOwner.IsWearingDress())
			return;

		if (m_rkOwner.m_kWarrior.m_dwGeomgyeongEffect)
			__DetachEffect(m_rkOwner.m_kWarrior.m_dwGeomgyeongEffect);

		m_rkOwner.m_GraphicThingInstance.SetReachScale(1.5f);
		if (m_rkOwner.m_GraphicThingInstance.IsTwoHandMode())
			m_rkOwner.m_kWarrior.m_dwGeomgyeongEffect = __AttachEffect(CInstanceBase::EFFECT_WEAPON + CInstanceBase::WEAPON_TWOHAND);
		else
			m_rkOwner.m_kWarrior.m_dwGeomgyeongEffect = __AttachEffect(CInstanceBase::EFFECT_WEAPON + CInstanceBase::WEAPON_ONEHAND);
	}
	else
	{
		m_rkOwner.m_GraphicThingInstance.SetReachScale(1.0f);

		__DetachEffect(m_rkOwner.m_kWarrior.m_dwGeomgyeongEffect);
		m_rkOwner.m_kWarrior.m_dwGeomgyeongEffect = 0;
	}
}

void CInstanceBaseEffect::__SetAffect(uint32_t eAffect, bool isVisible)
{
	switch (eAffect)
	{
		case CInstanceBase::AFFECT_YMIR:
			if (m_rkOwner.GetBattle().IsAffect(CInstanceBase::AFFECT_INVISIBILITY))
				return;
			break;
			/*
					case AFFECT_GWIGEOM: // 전기 속성 공격으로 바뀔 예정
						if (isVisible)
						{
							m_rkOwner.m_GraphicThingInstance.SetBattleHitEffect(CInstanceBase::ms_adwCRCAffectEffect[EFFECT_ELECTRIC_HIT]);
							m_rkOwner.m_GraphicThingInstance.SetBattleAttachEffect(CInstanceBase::ms_adwCRCAffectEffect[EFFECT_ELECTRIC_ATTACH]);
						}
						else
						{
							m_rkOwner.m_GraphicThingInstance.SetBattleHitEffect(CInstanceBase::ms_adwCRCAffectEffect[EFFECT_HIT]);
							m_rkOwner.m_GraphicThingInstance.SetBattleAttachEffect(0);
						}
						return;
						break;
					case AFFECT_HWAYEOM: // 화염 속성 공격으로 바뀔 예정
						if (isVisible)
						{
							m_rkOwner.m_GraphicThingInstance.SetBattleHitEffect(CInstanceBase::ms_adwCRCAffectEffect[EFFECT_FLAME_HIT]);
							m_rkOwner.m_GraphicThingInstance.SetBattleAttachEffect(CInstanceBase::ms_adwCRCAffectEffect[EFFECT_FLAME_ATTACH]);
						}
						else
						{
							m_rkOwner.m_GraphicThingInstance.SetBattleHitEffect(CInstanceBase::ms_adwCRCAffectEffect[EFFECT_HIT]);
							m_rkOwner.m_GraphicThingInstance.SetBattleAttachEffect(0);
						}
						// 화염참은 공격할 때만 일시적으로 Visible 합니다.
						return;
						break;
			*/
		case CInstanceBase::AFFECT_CHEONGEUN:
			m_rkOwner.m_GraphicThingInstance.SetResistFallen(isVisible);
			break;
		case CInstanceBase::AFFECT_GEOMGYEONG:
			__Warrior_SetGeomgyeongAffect(isVisible);
			return;
			break;
		case CInstanceBase::AFFECT_REVIVE_INVISIBILITY:
			__Assassin_SetEunhyeongAffect(isVisible);
			break;
		case CInstanceBase::AFFECT_EUNHYEONG:
			__Assassin_SetEunhyeongAffect(isVisible);
			break;
		case CInstanceBase::AFFECT_GYEONGGONG:
		case CInstanceBase::AFFECT_KWAESOK:
			// 경공술, 쾌속은 뛸때만 Attaching 시킵니다. - [levites]
			if (isVisible)
				if (!m_rkOwner.GetMovement().IsWalking())
					return;
			break;
		case CInstanceBase::AFFECT_INVISIBILITY:
			// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
			if (isVisible)
			{
				m_rkOwner.m_GraphicThingInstance.ClearAttachingEffect();
				__EffectContainer_Destroy();
				DetachTextTail();
			}
			else
			{
				m_rkOwner.m_GraphicThingInstance.BlendAlphaValue(1.0f, 1.0f);
				AttachTextTail();
				RefreshTextTail();
			}
			return;
			break;
			//		case AFFECT_FAINT:
			//			m_rkOwner.m_GraphicThingInstance.SetFaint(isVisible);
			//			break;
			//		case AFFECT_SLEEP:
			//			m_rkOwner.m_GraphicThingInstance.SetSleep(isVisible);
			//			break;
		case CInstanceBase::AFFECT_STUN:
			m_rkOwner.m_GraphicThingInstance.SetSleep(isVisible);
			break;
	}

	if (eAffect >= CInstanceBase::AFFECT_NUM)
	{
		TraceError("CInstanceBase[VID:%d]::SetAffect(eAffect:%d<CInstanceBase::AFFECT_NUM:%d, isVisible=%d)", m_rkOwner.GetVirtualID(), eAffect, isVisible);
		return;
	}

	if (isVisible)
	{
		if (!m_rkOwner.m_adwCRCAffectEffect[eAffect])
		{
			m_rkOwner.m_adwCRCAffectEffect[eAffect] = __AttachEffect(CInstanceBase::EFFECT_AFFECT + eAffect);
		}
	}
	else
	{
		if (m_rkOwner.m_adwCRCAffectEffect[eAffect])
		{
			__DetachEffect(m_rkOwner.m_adwCRCAffectEffect[eAffect]);
			m_rkOwner.m_adwCRCAffectEffect[eAffect] = 0;
		}
	}
}

bool CInstanceBaseEffect::IsPossibleEmoticon()
{
	CEffectManager& rkEftMgr = CEffectManager::Instance();
	for (uint32_t eEmoticon = 0; eEmoticon < CInstanceBase::EMOTICON_NUM; eEmoticon++)
	{
		uint32_t effectID = CInstanceBase::ms_adwCRCAffectEffect[CInstanceBase::EFFECT_EMOTICON + eEmoticon];
		if (effectID && rkEftMgr.IsAliveEffect(effectID))
			return false;
	}

	if (ELTimer_GetMSec() - m_rkOwner.m_dwEmoticonTime < 1000)
	{
		TraceError("ELTimer_GetMSec() - m_rkOwner.m_dwEmoticonTime");
		return false;
	}

	return true;
}

void CInstanceBaseEffect::SetFishEmoticon()
{
	SetEmoticon(CInstanceBase::EMOTICON_FISH);
}

void CInstanceBaseEffect::SetEmoticon(uint32_t eEmoticon)
{
	if (eEmoticon >= CInstanceBase::EMOTICON_NUM)
	{
		TraceError("CInstanceBase[VID:%d]::SetEmoticon(eEmoticon:%d<CInstanceBase::EMOTICON_NUM:%d, isVisible=%d)",
				   m_rkOwner.GetVirtualID(), eEmoticon);
		return;
	}
	if (IsPossibleEmoticon())
	{
		D3DXVECTOR3 v3Pos = m_rkOwner.m_GraphicThingInstance.GetPosition();
		v3Pos.z += float(m_rkOwner.m_GraphicThingInstance.GetHeight());

		// CEffectManager& rkEftMgr=CEffectManager::Instance();
		CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();

		D3DXVECTOR3 v3Dir = (pCamera->GetEye() - v3Pos) * 9 / 10;
		v3Pos = pCamera->GetEye() - v3Dir;

		v3Pos = D3DXVECTOR3(0, 0, 0);
		v3Pos.z += float(m_rkOwner.m_GraphicThingInstance.GetHeight());

		// rkEftMgr.CreateEffect(CInstanceBase::ms_adwCRCAffectEffect[CInstanceBase::EFFECT_EMOTICON+eEmoticon],v3Pos,D3DXVECTOR3(0,0,0));
		m_rkOwner.m_GraphicThingInstance.GetAttach().AttachEffectByID(0, NULL, CInstanceBase::ms_adwCRCAffectEffect[CInstanceBase::EFFECT_EMOTICON + eEmoticon], &v3Pos);
		m_rkOwner.m_dwEmoticonTime = ELTimer_GetMSec();
	}
}

void CInstanceBaseEffect::SetDustGap(float fDustGap)
{
	CInstanceBase::ms_fDustGap = fDustGap;
}

void CInstanceBaseEffect::SetHorseDustGap(float fDustGap)
{
	CInstanceBase::ms_fHorseDustGap = fDustGap;
}

void CInstanceBaseEffect::__DetachEffect(uint32_t dwEID)
{
	m_rkOwner.m_GraphicThingInstance.GetAttach().DettachEffect(dwEID);
}

uint32_t CInstanceBaseEffect::__AttachEffect(uint32_t eEftType)
{
	// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
	if (m_rkOwner.GetBattle().IsAffect(CInstanceBase::AFFECT_INVISIBILITY))
		return 0;

	if (eEftType >= CInstanceBase::EFFECT_NUM)
		return 0;

	if (CInstanceBase::ms_astAffectEffectAttachBone[eEftType].empty())
	{
		return m_rkOwner.m_GraphicThingInstance.GetAttach().AttachEffectByID(0, NULL, CInstanceBase::ms_adwCRCAffectEffect[eEftType]);
	}
	else
	{
		std::string& rstrBoneName = CInstanceBase::ms_astAffectEffectAttachBone[eEftType];
		const char* c_szBoneName;
		// 양손에 붙일 때 사용한다.
		// 이런 식의 예외 처리를 해놓은 것은 캐릭터 마다 Equip 의 Bone Name 이 다르기 때문.
		if (0 == rstrBoneName.compare("PART_WEAPON"))
		{
			if (m_rkOwner.m_GraphicThingInstance.GetAttach().GetAttachingBoneName(CRaceData::PART_WEAPON, &c_szBoneName))
			{
				return m_rkOwner.m_GraphicThingInstance.GetAttach().AttachEffectByID(0, c_szBoneName, CInstanceBase::ms_adwCRCAffectEffect[eEftType]);
			}
		}
		else if (0 == rstrBoneName.compare("PART_WEAPON_LEFT"))
		{
			if (m_rkOwner.m_GraphicThingInstance.GetAttach().GetAttachingBoneName(CRaceData::PART_WEAPON_LEFT, &c_szBoneName))
			{
				return m_rkOwner.m_GraphicThingInstance.GetAttach().AttachEffectByID(0, c_szBoneName, CInstanceBase::ms_adwCRCAffectEffect[eEftType]);
			}
		}
		else
		{
			return m_rkOwner.m_GraphicThingInstance.GetAttach().AttachEffectByID(0, rstrBoneName.c_str(), CInstanceBase::ms_adwCRCAffectEffect[eEftType]);
		}
	}

	return 0;
}

void CInstanceBaseEffect::__ComboProcess()
{
	/*
	uint32_t dwcurComboIndex = m_rkOwner.m_GraphicThingInstance.GetComboIndex();

	if (0 != dwcurComboIndex)
	{
		if (m_rkOwner.m_dwLastComboIndex != m_rkOwner.m_GraphicThingInstance.GetComboIndex())
		{
			if (!m_rkOwner.m_GraphicThingInstance.IsHandMode() & m_rkOwner.GetBattle().IsAffect(AFFECT_HWAYEOM))
			{
				__AttachEffect(EFFECT_FLAME_ATTACK);
			}
		}
	}

	m_rkOwner.m_dwLastComboIndex = dwcurComboIndex;
	*/
}

bool CInstanceBaseEffect::RegisterEffect(uint32_t eEftType, const char* c_szEftAttachBone, const char* c_szEftName, bool isCache)
{
	if (eEftType >= CInstanceBase::EFFECT_NUM)
		return false;

	CInstanceBase::ms_astAffectEffectAttachBone[eEftType] = c_szEftAttachBone;

	uint32_t& rdwCRCEft = CInstanceBase::ms_adwCRCAffectEffect[eEftType];
	if (!CEffectManager::Instance().RegisterEffect2(c_szEftName, &rdwCRCEft, isCache))
	{
		TraceError("CInstanceBaseEffect::RegisterEffect(eEftType=%d, c_szEftAttachBone=%s, c_szEftName=%s, isCache=%d) - Error", eEftType, c_szEftAttachBone, c_szEftName, isCache);
		rdwCRCEft = 0;
		return false;
	}

	return true;
}

void CInstanceBaseEffect::RegisterTitleName(int iIndex, const char* c_szTitleName)
{
	g_TitleNameMap.insert(std::make_pair(iIndex, c_szTitleName));
}

D3DXCOLOR __RGBToD3DXColoru(uint32_t r, uint32_t g, uint32_t b)
{
	uint32_t dwColor = 0xff;
	dwColor <<= 8;
	dwColor |= r;
	dwColor <<= 8;
	dwColor |= g;
	dwColor <<= 8;
	dwColor |= b;

	return D3DXCOLOR(dwColor);
}

bool CInstanceBaseEffect::RegisterNameColor(uint32_t uIndex, uint32_t r, uint32_t g, uint32_t b)
{
	if (uIndex >= CInstanceBase::NAMECOLOR_NUM)
		return false;

	g_akD3DXClrName[uIndex] = __RGBToD3DXColoru(r, g, b);
	return true;
}

bool CInstanceBaseEffect::RegisterTitleColor(uint32_t uIndex, uint32_t r, uint32_t g, uint32_t b)
{
	if (uIndex >= CInstanceBase::TITLE_NUM)
		return false;

	g_akD3DXClrTitle[uIndex] = __RGBToD3DXColoru(r, g, b);
	return true;
}
