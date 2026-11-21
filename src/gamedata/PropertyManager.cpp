#include <Client/Game/StdAfx.h>
#include <Client/GameData/FileSource.h>

#include <Client/GameData/PropertyManager.h>
#include <Client/GameData/Property.h>

#include <string.h>
#include <vector>

CPropertyManager::CPropertyManager()
{
}

CPropertyManager::~CPropertyManager()
{
	Clear();
}

void CPropertyManager::Scan(const char* c_szPrefix)
{
	std::vector<std::string> paths;
	if (!GameDataListFiles(c_szPrefix, paths))
		return;

	static const char* c_szReservedPrefix = "property/reserve";
	const size_t c_iReservedLength = strlen(c_szReservedPrefix);

	for (size_t i = 0; i < paths.size(); ++i)
	{
		const std::string& c_rstPath = paths[i];

		if (0 == c_rstPath.compare(0, c_iReservedLength, c_szReservedPrefix))
		{
			LoadReservedCRC(c_rstPath.c_str());
			continue;
		}

		Register(c_rstPath.c_str());
	}
}

bool CPropertyManager::LoadReservedCRC(const char* c_pszFileName)
{
	std::vector<uint8_t> bytes;
	if (!GameDataReadFile(c_pszFileName, bytes))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(static_cast<int>(bytes.size()), bytes.data());

	for (uint32_t i = 0; i < textFileLoader.GetLineCount(); ++i)
	{
		const char* pszLine = textFileLoader.GetLineString(i).c_str();

		if (!pszLine || !*pszLine)
			continue;

		ReserveCRC(atoi(pszLine));
	}

	return true;
}

void CPropertyManager::ReserveCRC(uint32_t dwCRC)
{
	m_ReservedCRCSet.insert(dwCRC);
}

uint32_t CPropertyManager::GetUniqueCRC(const char* c_szSeed)
{
	std::string stTmp = c_szSeed;

	while (1)
	{
		uint32_t dwCRC = GetCRC32(stTmp.c_str(), stTmp.length());

		if (m_ReservedCRCSet.find(dwCRC) == m_ReservedCRCSet.end() &&
			m_PropertyByCRCMap.find(dwCRC) == m_PropertyByCRCMap.end())
			return dwCRC;

		char szAdd[2];
		szAdd[0] = static_cast<char>('0' + random() % 10);
		szAdd[1] = '\0';
		stTmp += szAdd;
	}
}

bool CPropertyManager::Register(const char* c_pszFileName, CProperty** ppProperty)
{
	std::vector<uint8_t> bytes;
	if (!GameDataReadFile(c_pszFileName, bytes))
		return false;

	CProperty* pProperty = new CProperty(c_pszFileName);

	if (!pProperty->ReadFromMemory(bytes.data(), static_cast<int>(bytes.size()), c_pszFileName))
	{
		delete pProperty;
		return false;
	}

	uint32_t dwCRC = pProperty->GetCRC();

	TPropertyCRCMap::iterator itor = m_PropertyByCRCMap.find(dwCRC);

	if (m_PropertyByCRCMap.end() != itor)
	{
		Tracef("Property already registered, replace %s to %s\n",
			   itor->second->GetFileName(),
			   c_pszFileName);

		delete itor->second;
		itor->second = pProperty;
	}
	else
		m_PropertyByCRCMap.insert(TPropertyCRCMap::value_type(dwCRC, pProperty));

	if (ppProperty)
		*ppProperty = pProperty;

	return true;
}

bool CPropertyManager::Get(const char* c_pszFileName, CProperty** ppProperty)
{
	return Register(c_pszFileName, ppProperty);
}

bool CPropertyManager::Get(uint32_t dwCRC, CProperty** ppProperty)
{
	TPropertyCRCMap::iterator itor = m_PropertyByCRCMap.find(dwCRC);

	if (m_PropertyByCRCMap.end() == itor)
		return false;

	*ppProperty = itor->second;
	return true;
}

void CPropertyManager::Clear()
{
	stl_wipe_second(m_PropertyByCRCMap);
}
