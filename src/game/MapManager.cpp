#include <Client/Game/StdAfx.h>
#include <Neoeter/Render/StateManager.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Neoeter/Platform/Base/Timer.h>

#include <Client/Game/MapManager.h>
#include <Client/Game/MapOutdoor.h>
#include <Client/Game/Lighting.h>

#include <Neoeter/Render/RenderOptions.h>

#include <Client/Game/PropertyLoader.h>

#include <cmath>

//////////////////////////////////////////////////////////////////////////
// 기본 함수
//////////////////////////////////////////////////////////////////////////

bool CMapManager::IsMapOutdoor()
{
	if (m_pkMap)
		return true;

	return false;
}

CMapOutdoor& CMapManager::GetMapOutdoorRef()
{
	assert(NULL != m_pkMap);
	return *m_pkMap;
}

namespace
{
	const float DAY_NIGHT_TRANSITION_SECONDS = 0.12f;
	const float DAY_NIGHT_MAX_HOURS_PER_SECOND = 12.0f;
	const float DAY_NIGHT_TINT_EPSILON = 0.0005f;

	bool IsClose(float left, float right)
	{
		return std::fabs(left - right) < DAY_NIGHT_TINT_EPSILON;
	}

	bool IsSameDayNightTint(const SDayNightTint& left, const SDayNightTint& right)
	{
		return IsClose(left.sun.r, right.sun.r) && IsClose(left.sun.g, right.sun.g) && IsClose(left.sun.b, right.sun.b) &&
			   IsClose(left.ambient.r, right.ambient.r) && IsClose(left.ambient.g, right.ambient.g) && IsClose(left.ambient.b, right.ambient.b) &&
			   IsClose(left.fog.r, right.fog.r) && IsClose(left.fog.g, right.fog.g) && IsClose(left.fog.b, right.fog.b) &&
			   IsClose(left.sky.r, right.sky.r) && IsClose(left.sky.g, right.sky.g) && IsClose(left.sky.b, right.sky.b) &&
			   IsClose(left.flare, right.flare);
	}

	float NormalizeDayNightHour(float fHour)
	{
		while (fHour < 0.0f)
			fHour += 24.0f;
		while (fHour >= 24.0f)
			fHour -= 24.0f;
		return fHour;
	}

	float AdvanceDayNightHour(float fCurrent, float fTarget, float fDeltaTime)
	{
		const float fDelta = NormalizeDayNightHour(fTarget - fCurrent + 12.0f) - 12.0f;
		if (std::fabs(fDelta) < 0.001f)
			return fTarget;

		float fStep = fDelta * (1.0f - std::exp(-fDeltaTime / DAY_NIGHT_TRANSITION_SECONDS));

		const float fMaxStep = DAY_NIGHT_MAX_HOURS_PER_SECOND * fDeltaTime;
		if (std::fabs(fStep) > fMaxStep)
			fStep = (fStep < 0.0f) ? -fMaxStep : fMaxStep;

		return NormalizeDayNightHour(fCurrent + fStep);
	}
}

CMapManager::CMapManager() : mc_pcurEnvironmentData(NULL),
							 m_pBaseEnvironmentData(NULL),
							 m_dwLastDayNightUpdate(0),
							 m_fDayNightHour(0.0f),
							 m_bDayNightApplied(false),
							 m_bEnvironmentLightingPushed(false),
							 m_bLastDayNightTintValid(false)
{
	m_pkMap = NULL;
	m_isSoftwareTilingEnableReserved = false;

	Lighting_Initialize();
	DayNight_Initialize();

	Tracef("Lighting mode: %s, day/night: %s, hour: %.2f", Lighting_GetModeName(),
		   DayNight_IsEnabled() ? "on" : "off", DayNight_GetHour());
}

CMapManager::~CMapManager()
{
	Destroy();
}

bool CMapManager::IsSoftwareTilingEnable()
{
	return CTerrainPatch::SOFTWARE_TRANSFORM_PATCH_ENABLE;
}

void CMapManager::ReserveSoftwareTilingEnable(bool isEnable)
{
	m_isSoftwareTilingEnableReserved = isEnable;
}

void CMapManager::Initialize()
{
	mc_pcurEnvironmentData = NULL;
	m_pBaseEnvironmentData = NULL;
	m_bDayNightApplied = false;
	m_bEnvironmentLightingPushed = false;
	m_bLastDayNightTintValid = false;
	__LoadMapInfoVector();
}

void CMapManager::Create()
{
	assert(NULL == m_pkMap && "CMapManager::Create");
	if (m_pkMap)
	{
		Clear();
		return;
	}

	CTerrainPatch::SOFTWARE_TRANSFORM_PATCH_ENABLE = m_isSoftwareTilingEnableReserved;

	m_pkMap = (CMapOutdoor*)AllocMap();

	assert(NULL != m_pkMap && "CMapManager::Create MAP is NULL");
}

void CMapManager::Destroy()
{
	stl_wipe_second(m_EnvironmentDataMap);

	if (m_pkMap)
	{
		m_pkMap->Clear();
		delete m_pkMap;
		m_pkMap = NULL;
	}
}

void CMapManager::Clear()
{
	if (m_pkMap)
		m_pkMap->Clear();
}

CMapBase* CMapManager::AllocMap()
{
	return new CMapOutdoor;
}

//////////////////////////////////////////////////////////////////////////
// Map
//////////////////////////////////////////////////////////////////////////
void CMapManager::LoadProperty()
{
	CPropertyLoader PropertyLoader;
	PropertyLoader.SetPropertyManager(&m_PropertyManager);
	PropertyLoader.Create("*.*", "Property");
}

bool CMapManager::LoadMap(const std::string& c_rstrMapName, float x, float y, float z)
{
	CMapOutdoor& rkMap = GetMapOutdoorRef();

	rkMap.Leave();
	rkMap.SetName(c_rstrMapName);
	rkMap.LoadProperty();

	if (CMapBase::MAPTYPE_INDOOR == rkMap.GetType())
	{
		TraceError("CMapManager::LoadMap() Indoor Map Load Failed");
		return false;
	}
	else if (CMapBase::MAPTYPE_OUTDOOR == rkMap.GetType())
	{
		if (!rkMap.Load(x, y, z))
		{
			TraceError("CMapManager::LoadMap() Outdoor Map Load Failed");
			return false;
		}

		RegisterEnvironmentData(0, rkMap.GetEnvironmentDataName().c_str());

		SetEnvironmentData(0);
	}
	else
	{
		TraceError("CMapManager::LoadMap() Invalid Map Type");
		return false;
	}

	rkMap.Enter();
	return true;
}

bool CMapManager::IsMapReady()
{
	if (!m_pkMap)
		return false;

	return m_pkMap->IsReady();
}

bool CMapManager::UnloadMap(const std::string c_strMapName)
{
	CMapOutdoor& rkMap = GetMapOutdoorRef();
	if (c_strMapName != rkMap.GetName() && "" != rkMap.GetName())
	{
		LogBoxf("%s: Unload Map Failed", c_strMapName.c_str());
		return false;
	}

	Clear();
	return true;
}

bool CMapManager::UpdateMap(float fx, float fy, float fz)
{
	if (!m_pkMap)
		return false;

	UpdateDayNight();

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.Update(fx, -fy, fz);
}

void CMapManager::UpdateAroundAmbience(float fx, float fy, float fz)
{
	if (!m_pkMap)
		return;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	rkMap.UpdateAroundAmbience(fx, -fy, fz);
}

float CMapManager::GetHeight(float fx, float fy)
{
	if (!m_pkMap)
	{
		TraceError("CMapManager::GetHeight(%f, %f) - Access without a map created", fx, fy);
		return 0.0f;
	}
	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetHeight(fx, fy);
}

float CMapManager::GetTerrainHeight(float fx, float fy)
{
	if (!m_pkMap)
	{
		TraceError("CMapManager::GetTerrainHeight(%f, %f) - Access without a map created", fx, fy);
		return 0.0f;
	}
	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetTerrainHeight(fx, fy);
}

bool CMapManager::GetWaterHeight(int iX, int iY, long* plWaterHeight)
{
	if (!m_pkMap)
	{
		TraceError("CMapManager::GetTerrainHeight(%f, %f) - Access without a map created", iX, iY);
		return false;
	}

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetWaterHeight(iX, iY, plWaterHeight);
}

//////////////////////////////////////////////////////////////////////////
// Environment
//////////////////////////////////////////////////////////////////////////
void CMapManager::BeginEnvironment()
{
	if (!m_pkMap)
		return;

	if (!mc_pcurEnvironmentData)
		return;

	CMapOutdoor& rkMap = GetMapOutdoorRef();

	m_bEnvironmentLightingPushed = !Lighting_IsEnabled();
	if (m_bEnvironmentLightingPushed)
		Neoeter::Render::PushLightingDisabled();

	STATEMANAGER.SaveRenderState(D3DRS_LIGHTING, TRUE);

	// Fog
	STATEMANAGER.SaveRenderState(D3DRS_FOGENABLE, mc_pcurEnvironmentData->bFogEnable);

	// Material
	STATEMANAGER.SetMaterial(&mc_pcurEnvironmentData->Material);

	// Directional Light
	if (mc_pcurEnvironmentData->bDirLightsEnable[ENV_DIRLIGHT_BACKGROUND])
	{
		ms_lpd3dDevice->LightEnable(0, TRUE);

		rkMap.ApplyLight((DWORD)(uintptr_t)mc_pcurEnvironmentData, mc_pcurEnvironmentData->DirLights[ENV_DIRLIGHT_BACKGROUND]);
	}
	else
		ms_lpd3dDevice->LightEnable(0, FALSE);

	if (mc_pcurEnvironmentData->bFogEnable)
	{
		DWORD dwFogColor = mc_pcurEnvironmentData->FogColor;
		STATEMANAGER.SetRenderState(D3DRS_FOGCOLOR, dwFogColor);

		if (mc_pcurEnvironmentData->bDensityFog)
		{
			float fDensity = 0.00015f;
			DWORD dwDensity;
			memcpy(&dwDensity, &fDensity, sizeof(dwDensity));
			STATEMANAGER.SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP); // pixel fog
			STATEMANAGER.SetRenderState(D3DRS_FOGDENSITY, dwDensity);	  // vertex fog
		}
		else
		{
			CSpeedTreeForest& rkForest = CSpeedTreeForest::Instance();
			rkForest.SetFog(
				mc_pcurEnvironmentData->GetFogNearDistance(),
				mc_pcurEnvironmentData->GetFogFarDistance());

			float fFogNear = mc_pcurEnvironmentData->GetFogNearDistance();
			float fFogFar = mc_pcurEnvironmentData->GetFogFarDistance();
			DWORD dwFogNear, dwFogFar;
			memcpy(&dwFogNear, &fFogNear, sizeof(dwFogNear));
			memcpy(&dwFogFar, &fFogFar, sizeof(dwFogFar));
			STATEMANAGER.SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR); // vertex fox
			STATEMANAGER.SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);		 // vertex fox
			STATEMANAGER.SetRenderState(D3DRS_FOGSTART, dwFogNear);			 // USED BY D3DFOG_LINEAR
			STATEMANAGER.SetRenderState(D3DRS_FOGEND, dwFogFar);			 // USED BY D3DFOG_LINEAR
		}
	}

	rkMap.OnBeginEnvironment();
}

void CMapManager::EndEnvironment()
{
	if (m_bEnvironmentLightingPushed)
	{
		Neoeter::Render::PopLightingDisabled();
		m_bEnvironmentLightingPushed = false;
	}

	if (!mc_pcurEnvironmentData)
		return;

	STATEMANAGER.RestoreRenderState(D3DRS_LIGHTING);
	STATEMANAGER.RestoreRenderState(D3DRS_FOGENABLE);
}

void CMapManager::SetEnvironmentData(int nEnvDataIndex)
{
	const TEnvironmentData* c_pEnvironmenData;

	if (GetEnvironmentData(nEnvDataIndex, &c_pEnvironmenData))
		SetEnvironmentDataPtr(c_pEnvironmenData);
}

void CMapManager::SetEnvironmentDataPtr(const TEnvironmentData* c_pEnvironmentData)
{
	if (!m_pkMap)
		return;

	if (!c_pEnvironmentData)
	{
		assert(!"null environment data");
		TraceError("null environment data");
		return;
	}

	CMapOutdoor& rkMap = GetMapOutdoorRef();

	m_pBaseEnvironmentData = c_pEnvironmentData;
	m_kDayNightEnvironmentData = *c_pEnvironmentData;
	m_bDayNightApplied = false;
	m_bLastDayNightTintValid = false;
	m_dwLastDayNightUpdate = 0;
	mc_pcurEnvironmentData = &m_kDayNightEnvironmentData;

	Tracef("DayNight environment updated");

	rkMap.SetEnvironmentDataPtr(mc_pcurEnvironmentData);
	UpdateDayNight();
}

void CMapManager::ResetEnvironmentDataPtr(const TEnvironmentData* c_pEnvironmentData)
{
	if (!m_pkMap)
		return;

	if (!c_pEnvironmentData)
	{
		assert(!"null environment data");
		TraceError("null environment data");
		return;
	}

	CMapOutdoor& rkMap = GetMapOutdoorRef();

	m_pBaseEnvironmentData = c_pEnvironmentData;
	m_kDayNightEnvironmentData = *c_pEnvironmentData;
	m_bDayNightApplied = false;
	m_bLastDayNightTintValid = false;
	m_dwLastDayNightUpdate = 0;
	mc_pcurEnvironmentData = &m_kDayNightEnvironmentData;

	Tracef("DayNight environment reset");

	rkMap.ResetEnvironmentDataPtr(mc_pcurEnvironmentData);
	UpdateDayNight();
}

void CMapManager::UpdateDayNight()
{
	if (!m_pkMap || !m_pBaseEnvironmentData)
		return;

	if (!DayNight_IsEnabled())
	{
		if (!m_bDayNightApplied)
			return;

		const float fFogNearDistance = m_kDayNightEnvironmentData.m_fFogNearDistance;
		const float fFogFarDistance = m_kDayNightEnvironmentData.m_fFogFarDistance;
		const D3DXVECTOR3 v3SkyBoxScale = m_kDayNightEnvironmentData.v3SkyBoxScale;

		m_kDayNightEnvironmentData = *m_pBaseEnvironmentData;

		m_kDayNightEnvironmentData.m_fFogNearDistance = fFogNearDistance;
		m_kDayNightEnvironmentData.m_fFogFarDistance = fFogFarDistance;
		m_kDayNightEnvironmentData.v3SkyBoxScale = v3SkyBoxScale;

		m_bDayNightApplied = false;
		m_bLastDayNightTintValid = false;

		CMapOutdoor& rkMap = GetMapOutdoorRef();
		rkMap.RefreshEnvironmentSky();
		rkMap.RefreshEnvironmentLensFlare();
		return;
	}

	const DWORD dwNow = CTimer::Instance().GetCurrentMillisecond();

	if (!m_bDayNightApplied)
	{
		m_fDayNightHour = DayNight_GetHour();
		m_dwLastDayNightUpdate = dwNow;
	}
	else
	{
		const float fDeltaTime = static_cast<float>(dwNow - m_dwLastDayNightUpdate) / 1000.0f;
		m_dwLastDayNightUpdate = dwNow;
		m_fDayNightHour = AdvanceDayNightHour(m_fDayNightHour, DayNight_GetHour(), fDeltaTime);
	}

	SDayNightTint kTint;
	DayNight_ComputeTint(m_fDayNightHour, kTint);

	if (m_bDayNightApplied && m_bLastDayNightTintValid && IsSameDayNightTint(kTint, m_kLastDayNightTint))
		return;

	const float fFogNearDistance = m_kDayNightEnvironmentData.m_fFogNearDistance;
	const float fFogFarDistance = m_kDayNightEnvironmentData.m_fFogFarDistance;
	const D3DXVECTOR3 v3SkyBoxScale = m_kDayNightEnvironmentData.v3SkyBoxScale;

	DayNight_Apply(kTint, *m_pBaseEnvironmentData, m_kDayNightEnvironmentData);

	m_kDayNightEnvironmentData.m_fFogNearDistance = fFogNearDistance;
	m_kDayNightEnvironmentData.m_fFogFarDistance = fFogFarDistance;
	m_kDayNightEnvironmentData.v3SkyBoxScale = v3SkyBoxScale;

	m_kLastDayNightTint = kTint;
	m_bLastDayNightTintValid = true;
	m_bDayNightApplied = true;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	rkMap.RefreshEnvironmentSky();
	rkMap.RefreshEnvironmentLensFlare();
}

bool CMapManager::IsDayNightEnabled()
{
	return DayNight_IsEnabled();
}

void CMapManager::SetDayNightEnabled(bool bEnabled)
{
	DayNight_SetEnabled(bEnabled);
	m_bLastDayNightTintValid = false;
	m_dwLastDayNightUpdate = 0;
	UpdateDayNight();
}

void CMapManager::SetTimeOfDay(float fHour)
{
	DayNight_SetHour(fHour);
	UpdateDayNight();
}

float CMapManager::GetTimeOfDay()
{
	return DayNight_GetHour();
}

void CMapManager::RefreshDayNight()
{
	m_bDayNightApplied = false;
	m_bLastDayNightTintValid = false;
	m_dwLastDayNightUpdate = 0;
	UpdateDayNight();
}

void CMapManager::BlendEnvironmentData(const TEnvironmentData* c_pEnvironmentData, int iTransitionTime)
{
}

bool CMapManager::RegisterEnvironmentData(DWORD dwIndex, const char* c_szFileName)
{
	TEnvironmentData* pEnvironmentData = AllocEnvironmentData();

	if (!LoadEnvironmentData(c_szFileName, pEnvironmentData))
	{
		DeleteEnvironmentData(pEnvironmentData);
		return false;
	}

	TEnvironmentDataMap::iterator f = m_EnvironmentDataMap.find(dwIndex);
	if (m_EnvironmentDataMap.end() == f)
	{
		m_EnvironmentDataMap.insert(TEnvironmentDataMap::value_type(dwIndex, pEnvironmentData));
	}
	else
	{
		if (m_pBaseEnvironmentData == f->second)
			m_pBaseEnvironmentData = NULL;
		delete f->second;
		f->second = pEnvironmentData;
	}
	return true;
}

void CMapManager::GetCurrentEnvironmentData(const TEnvironmentData** c_ppEnvironmentData)
{
	*c_ppEnvironmentData = mc_pcurEnvironmentData;
}

bool CMapManager::GetEnvironmentData(DWORD dwIndex, const TEnvironmentData** c_ppEnvironmentData)
{
	TEnvironmentDataMap::iterator itor = m_EnvironmentDataMap.find(dwIndex);

	if (m_EnvironmentDataMap.end() == itor)
	{
		*c_ppEnvironmentData = NULL;
		return false;
	}

	*c_ppEnvironmentData = itor->second;
	return true;
}

void CMapManager::RefreshPortal()
{
	if (!IsMapReady())
		return;

	CMapOutdoor& rMap = GetMapOutdoorRef();
	for (int i = 0; i < AROUND_AREA_NUM; ++i)
	{
		CArea* pArea;
		if (!rMap.GetAreaPointer(i, &pArea))
			continue;

		pArea->RefreshPortal();
	}
}

void CMapManager::ClearPortal()
{
	if (!IsMapReady())
		return;

	CMapOutdoor& rMap = GetMapOutdoorRef();
	for (int i = 0; i < AROUND_AREA_NUM; ++i)
	{
		CArea* pArea;
		if (!rMap.GetAreaPointer(i, &pArea))
			continue;

		pArea->ClearPortal();
	}
}

void CMapManager::AddShowingPortalID(int iID)
{
	if (!IsMapReady())
		return;

	CMapOutdoor& rMap = GetMapOutdoorRef();
	for (int i = 0; i < AROUND_AREA_NUM; ++i)
	{
		CArea* pArea;
		if (!rMap.GetAreaPointer(i, &pArea))
			continue;

		pArea->AddShowingPortalID(iID);
	}
}

TEnvironmentData* CMapManager::AllocEnvironmentData()
{
	TEnvironmentData* pEnvironmentData = new TEnvironmentData;
	Environment_Init(*pEnvironmentData);
	return pEnvironmentData;
}

void CMapManager::DeleteEnvironmentData(TEnvironmentData* pEnvironmentData)
{
	delete pEnvironmentData;
	pEnvironmentData = NULL;
}

BOOL CMapManager::LoadEnvironmentData(const char* c_szFileName, TEnvironmentData* pEnvironmentData)
{
	if (!pEnvironmentData)
		return FALSE;

	return (BOOL)Environment_Load(*pEnvironmentData, c_szFileName);
}

DWORD CMapManager::GetShadowMapColor(float fx, float fy)
{
	if (!IsMapReady())
		return 0xFFFFFFFF;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetShadowMapColor(fx, fy);
}

std::vector<int>& CMapManager::GetRenderedSplatNum(int* piPatch, int* piSplat, float* pfSplatRatio)
{
	if (!m_pkMap)
	{
		static std::vector<int> s_emptyVector;
		*piPatch = 0;
		*piSplat = 0;
		return s_emptyVector;
	}

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetRenderedSplatNum(piPatch, piSplat, pfSplatRatio);
}

CArea::TCRCWithNumberVector& CMapManager::GetRenderedGraphicThingInstanceNum(DWORD* pdwGraphicThingInstanceNum, DWORD* pdwCRCNum)
{
	if (!m_pkMap)
	{
		static CArea::TCRCWithNumberVector s_emptyVector;
		*pdwGraphicThingInstanceNum = 0;
		*pdwCRCNum = 0;
		return s_emptyVector;
	}

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetRenderedGraphicThingInstanceNum(pdwGraphicThingInstanceNum, pdwCRCNum);
}

bool CMapManager::GetNormal(int ix, int iy, D3DXVECTOR3* pv3Normal)
{
	if (!IsMapReady())
		return false;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetNormal(ix, iy, pv3Normal);
}

bool CMapManager::isPhysicalCollision(const D3DXVECTOR3& c_rvCheckPosition)
{
	if (!IsMapReady())
		return false;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.isAttrOn(c_rvCheckPosition.x, -c_rvCheckPosition.y, CTerrainImpl::ATTRIBUTE_BLOCK);
}

bool CMapManager::isAttrOn(float fX, float fY, BYTE byAttr)
{
	if (!IsMapReady())
		return false;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.isAttrOn(fX, fY, byAttr);
}

bool CMapManager::GetAttr(float fX, float fY, BYTE* pbyAttr)
{
	if (!IsMapReady())
		return false;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetAttr(fX, fY, pbyAttr);
}

bool CMapManager::isAttrOn(int iX, int iY, BYTE byAttr)
{
	if (!IsMapReady())
		return false;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.isAttrOn(iX, iY, byAttr);
}

bool CMapManager::GetAttr(int iX, int iY, BYTE* pbyAttr)
{
	if (!IsMapReady())
		return false;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetAttr(iX, iY, pbyAttr);
}

// 2004.10.14.myevan.TEMP_CAreaLoaderThread
/*
bool CMapManager::BGLoadingEnable()
{
	if (!IsMapReady())
		return false;
	return ((CMapOutdoor*)m_pMap)->BGLoadingEnable();
}

void CMapManager::BGLoadingEnable(bool bBGLoadingEnable)
{
	if (!IsMapReady())
		return;
	((CMapOutdoor*)m_pMap)->BGLoadingEnable(bBGLoadingEnable);
}
*/

void CMapManager::SetTerrainRenderSort(CMapOutdoor::ETerrainRenderSort eTerrainRenderSort)
{
	if (!IsMapReady())
		return;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	rkMap.SetTerrainRenderSort(eTerrainRenderSort);
}

void CMapManager::SetTransparentTree(bool bTransparenTree)
{
	if (!IsMapReady())
		return;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	rkMap.SetTransparentTree(bTransparenTree);
}

CMapOutdoor::ETerrainRenderSort CMapManager::GetTerrainRenderSort()
{
	if (!IsMapReady())
		return CMapOutdoor::DISTANCE_SORT;

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	return rkMap.GetTerrainRenderSort();
}

void CMapManager::GetBaseXY(DWORD* pdwBaseX, DWORD* pdwBaseY)
{
	if (!IsMapReady())
	{
		*pdwBaseX = 0;
		*pdwBaseY = 0;
	}

	CMapOutdoor& rkMap = GetMapOutdoorRef();
	rkMap.GetBaseXY(pdwBaseX, pdwBaseY);
}

void CMapManager::__LoadMapInfoVector()
{
	CEterPackManager::TPackDataPtr data;
	CMemoryTextFileLoader textFileLoader;

	m_kVct_kMapInfo.clear();

	if (!CEterPackManager::Instance().Get(m_stAtlasInfoFileName, data))
		if (!CEterPackManager::Instance().Get("AtlasInfo.txt", data))
			return;

	textFileLoader.Bind(data->size(), data->data());

	char szMapName[256];
	int x, y;
	int width, height;
	for (UINT uLineIndex = 0; uLineIndex < textFileLoader.GetLineCount(); ++uLineIndex)
	{
		const std::string& c_rstLine = textFileLoader.GetLineString(uLineIndex);
		sscanf(c_rstLine.c_str(), "%s %d %d %d %d",
			   szMapName,
			   &x, &y, &width, &height);

		if ('\0' == szMapName[0])
			continue;

		TMapInfo kMapInfo;
		kMapInfo.m_strName = szMapName;
		kMapInfo.m_dwBaseX = x;
		kMapInfo.m_dwBaseY = y;

		kMapInfo.m_dwSizeX = width;
		kMapInfo.m_dwSizeY = height;

		kMapInfo.m_dwEndX = kMapInfo.m_dwBaseX + kMapInfo.m_dwSizeX * CTerrainImpl::TERRAIN_XSIZE;
		kMapInfo.m_dwEndY = kMapInfo.m_dwBaseY + kMapInfo.m_dwSizeY * CTerrainImpl::TERRAIN_YSIZE;

		m_kVct_kMapInfo.push_back(kMapInfo);
	}

	return;
}
