#include <Client/UI/StdAfx.h>
#include <Neoeter/IO/EterPackManager.h>
#include <Client/Python/NonPlayer/PythonNonPlayer.h>
#include <Client/Actor/InstanceBase.h>
#include <Client/Python/Character/PythonCharacterManager.h>

bool CPythonNonPlayer::LoadNonPlayerData(const char* c_szFileName)
{
	static uint32_t s_adwMobProtoKey[4] =
		{
			4813894,
			18955,
			552631,
			6822045};

	Tracef("CPythonNonPlayer::LoadNonPlayerData: %s, sizeof(TMobTable)=%u\n", c_szFileName, sizeof(TMobTable));

	std::stringstream data;
	if (!CEterPackManager::Instance().Get(c_szFileName, data))
		return false;

	uint32_t dwFourCC, dwElements, dwDataSize;

	data.read((char*)&dwFourCC, sizeof(uint32_t));
	if (!data)
		return false;

	if (dwFourCC != MAKEFOURCC('M', 'M', 'P', 'T'))
	{
		TraceError("CPythonNonPlayer::LoadNonPlayerData: invalid Mob proto type %s", c_szFileName);
		return false;
	}

	data.read((char*)&dwElements, sizeof(uint32_t));
	if (!data)
		return false;

	data.read((char*)&dwDataSize, sizeof(uint32_t));
	if (!data)
		return false;

	auto pbData = std::make_shared<std::vector<uint8_t>>(dwDataSize);

	data.read((char*)pbData->data(), pbData->size());
	if (!data)
		return false;

	/////

	CLZObject zObj;

	if (!CLZO::Instance().Decompress(zObj, pbData->data(), s_adwMobProtoKey))
		return false;

	if ((zObj.GetSize() % sizeof(TMobTable)) != 0)
	{
		TraceError("CPythonNonPlayer::LoadNonPlayerData: invalid size %u check data format.", zObj.GetSize());
		return false;
	}

	TMobTable* pTable = (TMobTable*)zObj.GetBuffer();
	for (uint32_t i = 0; i < dwElements; ++i, ++pTable)
	{
		TMobTable* pNonPlayerData = new TMobTable;

		memcpy(pNonPlayerData, pTable, sizeof(TMobTable));

		// TraceError("%d : %s type[%d] color[%d]", pNonPlayerData->dwVnum, pNonPlayerData->szLocaleName, pNonPlayerData->bType, pNonPlayerData->dwMonsterColor);
		m_NonPlayerDataMap.insert(TNonPlayerDataMap::value_type(pNonPlayerData->dwVnum, pNonPlayerData));
	}

	return true;
}

bool CPythonNonPlayer::GetName(uint32_t dwVnum, const char** c_pszName)
{
	const TMobTable* p = GetTable(dwVnum);

	if (!p)
		return false;

	*c_pszName = p->szLocaleName;

	return true;
}

bool CPythonNonPlayer::GetInstanceType(uint32_t dwVnum, uint8_t* pbType)
{
	const TMobTable* p = GetTable(dwVnum);

	// dwVnum를 찾을 수 없으면 플레이어 캐릭터로 간주 한다. 문제성 코드 -_- [cronan]
	if (!p)
		return false;

	*pbType = p->bType;

	return true;
}

const CPythonNonPlayer::TMobTable* CPythonNonPlayer::GetTable(uint32_t dwVnum)
{
	TNonPlayerDataMap::iterator itor = m_NonPlayerDataMap.find(dwVnum);

	if (itor == m_NonPlayerDataMap.end())
		return NULL;

	return itor->second;
}

uint8_t CPythonNonPlayer::GetEventType(uint32_t dwVnum)
{
	const TMobTable* p = GetTable(dwVnum);

	if (!p)
	{
		// Tracef("CPythonNonPlayer::GetEventType - Failed to find virtual number\n");
		return ON_CLICK_EVENT_NONE;
	}

	return p->bOnClickType;
}

uint8_t CPythonNonPlayer::GetEventTypeByVID(uint32_t dwVID)
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwVID);

	if (NULL == pInstance)
	{
		// Tracef("CPythonNonPlayer::GetEventTypeByVID - There is no Virtual Number\n");
		return ON_CLICK_EVENT_NONE;
	}

	uint16_t dwVnum = pInstance->GetVirtualNumber();
	return GetEventType(dwVnum);
}

const char* CPythonNonPlayer::GetMonsterName(uint32_t dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
	{
		static const char* sc_szEmpty = "";
		return sc_szEmpty;
	}

	return c_pTable->szLocaleName;
}

uint32_t CPythonNonPlayer::GetMonsterColor(uint32_t dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;

	return c_pTable->dwMonsterColor;
}

void CPythonNonPlayer::GetMatchableMobList(int iLevel, int iInterval, TMobTableList* pMobTableList)
{
	/*
		pMobTableList->clear();

		TNonPlayerDataMap::iterator itor = m_NonPlayerDataMap.begin();
		for (; itor != m_NonPlayerDataMap.end(); ++itor)
		{
			TMobTable * pMobTable = itor->second;

			int iLowerLevelLimit = iLevel-iInterval;
			int iUpperLevelLimit = iLevel+iInterval;

			if ((pMobTable->abLevelRange[0] >= iLowerLevelLimit && pMobTable->abLevelRange[0] <= iUpperLevelLimit) ||
				(pMobTable->abLevelRange[1] >= iLowerLevelLimit && pMobTable->abLevelRange[1] <= iUpperLevelLimit))
			{
				pMobTableList->push_back(pMobTable);
			}
		}
	*/
}

void CPythonNonPlayer::Clear()
{
}

void CPythonNonPlayer::Destroy()
{
	for (TNonPlayerDataMap::iterator itor = m_NonPlayerDataMap.begin(); itor != m_NonPlayerDataMap.end(); ++itor)
	{
		delete itor->second;
	}
	m_NonPlayerDataMap.clear();
}

CPythonNonPlayer::CPythonNonPlayer()
{
	Clear();
}

CPythonNonPlayer::~CPythonNonPlayer(void)
{
	Destroy();
}