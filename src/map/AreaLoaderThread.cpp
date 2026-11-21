// AreaLoaderThread.cpp: implementation of the CAreaLoaderThread class.
//
//////////////////////////////////////////////////////////////////////

#include <Client/Map/StdAfx.h>

#include <Neoeter/Platform/Port.h>
#include <Neoeter/Render/ResourceManager.h>

#include <Client/Map/AreaLoaderThread.h>
#include <Client/Map/AreaTerrain.h>
#include <Client/Map/MapOutdoor.h>

#ifdef CreateSemaphore
#undef CreateSemaphore
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TEMP_CAreaLoaderThread::TEMP_CAreaLoaderThread() : m_hThread(NULL), m_pArg(NULL), m_hSemaphore(NULL), m_bShutdowned(false)
{
}

TEMP_CAreaLoaderThread::~TEMP_CAreaLoaderThread()
{
	Shutdown();
	Destroy();
}

bool TEMP_CAreaLoaderThread::Create(void* arg)
{
	if (m_hThread != NULL)
		return false;

	m_hSemaphore = Neoeter::Platform::CreateSemaphore(0, 65535);
	if (m_hSemaphore == NULL)
		return false;

	Arg(arg);
	m_hThread = Neoeter::Platform::StartThread(&TEMP_CAreaLoaderThread::EntryPoint, this);
	if (m_hThread == NULL)
	{
		Neoeter::Platform::CloseSemaphore(m_hSemaphore);
		m_hSemaphore = NULL;
		return false;
	}

	return true;
}

uint32_t TEMP_CAreaLoaderThread::EntryPoint(void* pThis)
{
	TEMP_CAreaLoaderThread* thread = static_cast<TEMP_CAreaLoaderThread*>(pThis);
	return thread->Run(thread->Arg());
}

uint32_t TEMP_CAreaLoaderThread::Run(void* arg)
{
	return Execute(arg);
}

//////////////////////////////////////////////////////////////////////////
void TEMP_CAreaLoaderThread::Destroy()
{
	/*
		while(!m_pTerrainRequestDeque.empty())
		{
			CTerrain * pTerrain = m_pTerrainRequestDeque.front();
			delete pTerrain;
			pTerrain = NULL;
			m_pTerrainRequestDeque.pop_front();
		}

		while(!m_pTerrainCompleteDeque.empty())
		{
			CTerrain * pTerrain = m_pTerrainCompleteDeque.front();
			delete pTerrain;
			pTerrain = NULL;
			m_pTerrainCompleteDeque.pop_front();
		}
	*/

	/*stl_wipe(m_pTerrainRequestDeque);
	stl_wipe(m_pTerrainCompleteDeque);
	stl_wipe(m_pAreaRequestDeque);
	stl_wipe(m_pAreaCompleteDeque);*/
}

void TEMP_CAreaLoaderThread::Shutdown()
{
	if (m_bShutdowned.exchange(true))
		return;

	if (m_hSemaphore != NULL)
		Neoeter::Platform::ReleaseSemaphore(m_hSemaphore, 1);

	bool joined = true;
	if (m_hThread != NULL)
	{
		joined = Neoeter::Platform::JoinThread(m_hThread, 10000);
		Neoeter::Platform::CloseThread(m_hThread);
		m_hThread = NULL;
	}

	if (joined && m_hSemaphore != NULL)
	{
		Neoeter::Platform::CloseSemaphore(m_hSemaphore);
		m_hSemaphore = NULL;
	}
}

uint32_t TEMP_CAreaLoaderThread::Execute(void* pvArg)
{
	while (!m_bShutdowned.load())
	{
		if (!Neoeter::Platform::WaitSemaphore(m_hSemaphore, Neoeter::Platform::kPlatformInfinite))
			break;

		if (m_bShutdowned.load())
			break;

		ProcessTerrain();
		ProcessArea();
	}

	Destroy();
	return 1;
}

void TEMP_CAreaLoaderThread::Request(CTerrain* pTerrain) // called in main thread
{
	m_TerrainRequestMutex.Lock();
	m_pTerrainRequestDeque.push_back(pTerrain);
	m_TerrainRequestMutex.Unlock();

	if (!Neoeter::Platform::ReleaseSemaphore(m_hSemaphore, 1))
		TraceError("TEMP_CAreaLoaderThread::Request: ReleaseSemaphore error");
}

bool TEMP_CAreaLoaderThread::Fetch(CTerrain** ppTerrain) // called in main thread
{
	m_TerrainCompleteMutex.Lock();

	if (m_pTerrainCompleteDeque.empty())
	{
		m_TerrainCompleteMutex.Unlock();
		return false;
	}

	*ppTerrain = m_pTerrainCompleteDeque.front();
	m_pTerrainCompleteDeque.pop_front();

	m_TerrainCompleteMutex.Unlock();
	return true;
}

void TEMP_CAreaLoaderThread::Request(CArea* pArea) // called in main thread
{
	m_AreaRequestMutex.Lock();
	m_pAreaRequestDeque.push_back(pArea);
	m_AreaRequestMutex.Unlock();

	if (!Neoeter::Platform::ReleaseSemaphore(m_hSemaphore, 1))
		TraceError("TEMP_CAreaLoaderThread::Request: ReleaseSemaphore error");
}

bool TEMP_CAreaLoaderThread::Fetch(CArea** ppArea) // called in main thread
{
	m_AreaCompleteMutex.Lock();

	if (m_pAreaCompleteDeque.empty())
	{
		m_AreaCompleteMutex.Unlock();
		return false;
	}

	*ppArea = m_pAreaCompleteDeque.front();
	m_pAreaCompleteDeque.pop_front();

	m_AreaCompleteMutex.Unlock();
	return true;
}

void TEMP_CAreaLoaderThread::ProcessArea() // called in loader thread
{
	m_AreaRequestMutex.Lock();

	if (m_pAreaRequestDeque.empty())
	{
		m_AreaRequestMutex.Unlock();
		return;
	}

	CArea* pArea = m_pAreaRequestDeque.front();
	m_pAreaRequestDeque.pop_front();

	Tracef("TEMP_CAreaLoaderThread::ProcessArea() RequestDeque Size : %d\n", m_pAreaRequestDeque.size());
	m_AreaRequestMutex.Unlock();

	uint32_t dwStartTime = ELTimer_GetMSec();

	// Area Load
	uint16_t wAreaCoordX, wAreaCoordY;
	pArea->GetCoordinate(&wAreaCoordX, &wAreaCoordY);
	uint32_t dwID = (uint32_t)(wAreaCoordX) * 1000L + (uint32_t)(wAreaCoordY);

	const std::string& c_rStrMapName = pArea->GetOwner()->GetName();

	char szAreaPathName[64 + 1];
	_snprintf(szAreaPathName, sizeof(szAreaPathName), "%s\\%06u\\", c_rStrMapName.c_str(), dwID);

	pArea->Load(szAreaPathName);

	Tracef("TEMP_CAreaLoaderThread::ProcessArea LoadArea : %d ms elapsed\n", ELTimer_GetMSec() - dwStartTime);

	m_AreaCompleteMutex.Lock();
	m_pAreaCompleteDeque.push_back(pArea);
	m_AreaCompleteMutex.Unlock();

	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
}

void TEMP_CAreaLoaderThread::ProcessTerrain() // called in loader thread
{
	m_TerrainRequestMutex.Lock();

	if (m_pTerrainRequestDeque.empty())
	{
		m_TerrainRequestMutex.Unlock();
		return;
	}

	CTerrain* pTerrain = m_pTerrainRequestDeque.front();
	m_pTerrainRequestDeque.pop_front();

	Tracef("TEMP_CAreaLoaderThread::ProcessTerrain() RequestDeque Size : %d\n", m_pTerrainRequestDeque.size());
	m_TerrainRequestMutex.Unlock();

	uint32_t dwStartTime = ELTimer_GetMSec();

	// Terrain Load
	uint16_t wCoordX, wCoordY;
	pTerrain->GetCoordinate(&wCoordX, &wCoordY);
	uint32_t dwID = (uint32_t)(wCoordX) * 1000L + (uint32_t)(wCoordY);

	const std::string& c_rStrMapName = pTerrain->GetOwner()->GetName();
	char filename[256];
	sprintf(filename, "%s\\%06u\\AreaProperty.txt", c_rStrMapName.c_str(), dwID);

	CTokenVectorMap stTokenVectorMap;

	if (!LoadMultipleTextData(filename, stTokenVectorMap))
		return;

	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);

	if (stTokenVectorMap.end() == stTokenVectorMap.find("scripttype"))
		return;

	if (stTokenVectorMap.end() == stTokenVectorMap.find("areaname"))
		return;

	const std::string& c_rstrType = stTokenVectorMap["scripttype"][0];
	const std::string& c_rstrAreaName = stTokenVectorMap["areaname"][0];

	if (c_rstrType != "AreaProperty")
		return;

	char szRawHeightFieldname[64 + 1];
	char szWaterMapName[64 + 1];
	char szAttrMapName[64 + 1];
	char szShadowTexName[64 + 1];
	char szShadowMapName[64 + 1];
	char szMiniMapTexName[64 + 1];
	char szSplatName[64 + 1];

	_snprintf(szRawHeightFieldname, sizeof(szRawHeightFieldname), "%s\\%06u\\height.raw", c_rStrMapName.c_str(), dwID);
	_snprintf(szSplatName, sizeof(szSplatName), "%s\\%06u\\tile.raw", c_rStrMapName.c_str(), dwID);
	_snprintf(szAttrMapName, sizeof(szAttrMapName), "%s\\%06u\\attr.atr", c_rStrMapName.c_str(), dwID);
	_snprintf(szWaterMapName, sizeof(szWaterMapName), "%s\\%06u\\water.wtr", c_rStrMapName.c_str(), dwID);
	_snprintf(szShadowTexName, sizeof(szShadowTexName), "%s\\%06u\\shadowmap.dds", c_rStrMapName.c_str(), dwID);
	_snprintf(szShadowMapName, sizeof(szShadowMapName), "%s\\%06u\\shadowmap.raw", c_rStrMapName.c_str(), dwID);
	_snprintf(szMiniMapTexName, sizeof(szMiniMapTexName), "%s\\%06u\\minimap.dds", c_rStrMapName.c_str(), dwID);

	pTerrain->CopySettingFromGlobalSetting();

	pTerrain->LoadWaterMap(szWaterMapName);
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->LoadHeightMap(szRawHeightFieldname);
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->LoadAttrMap(szAttrMapName);
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->RAW_LoadTileMap(szSplatName, true);
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->LoadShadowTexture(szShadowTexName);
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->LoadShadowMap(szShadowMapName);
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->LoadMiniMapTexture(szMiniMapTexName);
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->SetName(c_rstrAreaName.c_str());
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
	pTerrain->CalculateTerrainPatch();
	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);

	pTerrain->SetReady();

	Tracef("TEMP_CAreaLoaderThread::ProcessTerrain LoadTerrain : %d ms elapsed\n", ELTimer_GetMSec() - dwStartTime);

	m_TerrainCompleteMutex.Lock();
	m_pTerrainCompleteDeque.push_back(pTerrain);
	m_TerrainCompleteMutex.Unlock();

	Neoeter::Platform::SleepMillis(g_iLoadingDelayTime);
}
