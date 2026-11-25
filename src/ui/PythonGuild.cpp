#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonGuild.h>
#include <Client/UI/AbstractPlayer.h>
#include <Client/UI/MarkManager.h>
#include <Client/Game/Balance.h>

std::map<DWORD, DWORD> g_GuildSkillSlotToIndexMap;

void CPythonGuild::EnableGuild()
{
	m_bGuildEnable = TRUE;
}

void CPythonGuild::SetGuildMoney(DWORD dwMoney)
{
	m_GuildInfo.dwGuildMoney = dwMoney;
}

void CPythonGuild::SetGuildEXP(BYTE byLevel, DWORD dwEXP)
{
	m_GuildInfo.dwGuildLevel = byLevel;
	m_GuildInfo.dwCurrentExperience = dwEXP;
}

void CPythonGuild::SetGradeData(BYTE byGradeNumber, const TGuildGradeData& rGuildGradeData)
{
	m_GradeDataMap[byGradeNumber] = rGuildGradeData;
}

void CPythonGuild::SetGradeName(BYTE byGradeNumber, const char* c_szName)
{
	if (!__IsGradeData(byGradeNumber))
		return;

	TGuildGradeData& rGradeData = m_GradeDataMap.find(byGradeNumber)->second;
	rGradeData.strName = c_szName;
}

void CPythonGuild::SetGradeAuthority(BYTE byGradeNumber, BYTE byAuthority)
{
	if (!__IsGradeData(byGradeNumber))
		return;

	TGuildGradeData& rGradeData = m_GradeDataMap.find(byGradeNumber)->second;
	rGradeData.byAuthorityFlag = byAuthority;
}

void CPythonGuild::ClearComment()
{
	m_GuildBoardCommentVector.clear();
}

void CPythonGuild::RegisterComment(DWORD dwCommentID, const char* c_szName, const char* c_szComment)
{
	if (0 == strlen(c_szComment))
		return;

	TGuildBoardCommentData CommentData;
	CommentData.dwCommentID = dwCommentID;
	CommentData.strName = c_szName;
	CommentData.strComment = c_szComment;

	m_GuildBoardCommentVector.push_back(CommentData);
}

void CPythonGuild::RegisterMember(TGuildMemberData& rGuildMemberData)
{
	TGuildMemberData* pGuildMemberData;
	if (GetMemberDataPtrByPID(rGuildMemberData.dwPID, &pGuildMemberData))
	{
		pGuildMemberData->byGeneralFlag = rGuildMemberData.byGeneralFlag;
		pGuildMemberData->byGrade = rGuildMemberData.byGrade;
		pGuildMemberData->byLevel = rGuildMemberData.byLevel;
		pGuildMemberData->dwOffer = rGuildMemberData.dwOffer;
	}
	else
	{
		m_GuildMemberDataVector.push_back(rGuildMemberData);
	}

	__CalculateLevelAverage();
	__SortMember();
}

struct CPythonGuild_FFindGuildMemberByPID
{
		CPythonGuild_FFindGuildMemberByPID(DWORD dwSearchingPID_) : dwSearchingPID(dwSearchingPID_) {}
		int operator()(CPythonGuild::TGuildMemberData& rGuildMemberData)
		{
			return rGuildMemberData.dwPID == dwSearchingPID;
		}

		DWORD dwSearchingPID;
};

struct CPythonGuild_FFindGuildMemberByName
{
		CPythonGuild_FFindGuildMemberByName(const char* c_szSearchingName) : strSearchingName(c_szSearchingName) {}
		int operator()(CPythonGuild::TGuildMemberData& rGuildMemberData)
		{
			return 0 == strSearchingName.compare(rGuildMemberData.strName.c_str());
		}

		std::string strSearchingName;
};

void CPythonGuild::ChangeGuildMemberGrade(DWORD dwPID, BYTE byGrade)
{
	TGuildMemberData* pGuildMemberData;
	if (!GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
		return;

	pGuildMemberData->byGrade = byGrade;
}

void CPythonGuild::ChangeGuildMemberGeneralFlag(DWORD dwPID, BYTE byFlag)
{
	TGuildMemberData* pGuildMemberData;
	if (!GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
		return;

	pGuildMemberData->byGeneralFlag = byFlag;
}

void CPythonGuild::RemoveMember(DWORD dwPID)
{
	TGuildMemberDataVector::iterator itor;
	itor = std::find_if(m_GuildMemberDataVector.begin(),
						m_GuildMemberDataVector.end(),
						CPythonGuild_FFindGuildMemberByPID(dwPID));

	if (m_GuildMemberDataVector.end() == itor)
		return;

	m_GuildMemberDataVector.erase(itor);
}

void CPythonGuild::RegisterGuildName(DWORD dwID, const char* c_szName)
{
	m_GuildNameMap.insert(make_pair(dwID, std::string(c_szName)));
}

BOOL CPythonGuild::IsMainPlayer(DWORD dwPID)
{
	TGuildMemberData* pGuildMemberData;
	if (!GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
		return FALSE;

	IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
	return 0 == pGuildMemberData->strName.compare(rPlayer.GetName());
}

BOOL CPythonGuild::IsGuildEnable()
{
	return m_bGuildEnable;
}

CPythonGuild::TGuildInfo& CPythonGuild::GetGuildInfoRef()
{
	return m_GuildInfo;
}

BOOL CPythonGuild::GetGradeDataPtr(DWORD dwGradeNumber, TGuildGradeData** ppData)
{
	TGradeDataMap::iterator itor = m_GradeDataMap.find(dwGradeNumber);
	if (m_GradeDataMap.end() == itor)
		return FALSE;

	*ppData = &(itor->second);

	return TRUE;
}

const CPythonGuild::TGuildBoardCommentDataVector& CPythonGuild::GetGuildBoardCommentVector()
{
	return m_GuildBoardCommentVector;
}

DWORD CPythonGuild::GetMemberCount()
{
	return m_GuildMemberDataVector.size();
}

BOOL CPythonGuild::GetMemberDataPtr(DWORD dwIndex, TGuildMemberData** ppData)
{
	if (dwIndex >= m_GuildMemberDataVector.size())
		return FALSE;

	*ppData = &m_GuildMemberDataVector[dwIndex];

	return TRUE;
}

BOOL CPythonGuild::GetMemberDataPtrByPID(DWORD dwPID, TGuildMemberData** ppData)
{
	TGuildMemberDataVector::iterator itor;
	itor = std::find_if(m_GuildMemberDataVector.begin(),
						m_GuildMemberDataVector.end(),
						CPythonGuild_FFindGuildMemberByPID(dwPID));

	if (m_GuildMemberDataVector.end() == itor)
		return FALSE;

	*ppData = &(*itor);
	return TRUE;
}

BOOL CPythonGuild::GetMemberDataPtrByName(const char* c_szName, TGuildMemberData** ppData)
{
	TGuildMemberDataVector::iterator itor;
	itor = std::find_if(m_GuildMemberDataVector.begin(),
						m_GuildMemberDataVector.end(),
						CPythonGuild_FFindGuildMemberByName(c_szName));

	if (m_GuildMemberDataVector.end() == itor)
		return FALSE;

	*ppData = &(*itor);
	return TRUE;
}

DWORD CPythonGuild::GetGuildMemberLevelSummary()
{
	return m_dwMemberLevelSummary;
}

DWORD CPythonGuild::GetGuildMemberLevelAverage()
{
	return m_dwMemberLevelAverage;
}

DWORD CPythonGuild::GetGuildExperienceSummary()
{
	return m_dwMemberExperienceSummary;
}

CPythonGuild::TGuildSkillData& CPythonGuild::GetGuildSkillDataRef()
{
	return m_GuildSkillData;
}

bool CPythonGuild::GetGuildName(DWORD dwID, std::string* pstrGuildName)
{
	if (m_GuildNameMap.end() == m_GuildNameMap.find(dwID))
		return false;

	*pstrGuildName = m_GuildNameMap[dwID];

	return true;
}

DWORD CPythonGuild::GetGuildID()
{
	return m_GuildInfo.dwGuildID;
}

BOOL CPythonGuild::HasGuildLand()
{
	return m_GuildInfo.bHasLand;
}

void CPythonGuild::StartGuildWar(DWORD dwEnemyGuildID)
{
	int i;

	for (i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (dwEnemyGuildID == m_adwEnemyGuildID[i])
			return;

	for (i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (0 == m_adwEnemyGuildID[i])
		{
			m_adwEnemyGuildID[i] = dwEnemyGuildID;
			break;
		}
}

void CPythonGuild::EndGuildWar(DWORD dwEnemyGuildID)
{
	for (int i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (dwEnemyGuildID == m_adwEnemyGuildID[i])
			m_adwEnemyGuildID[i] = 0;
}

DWORD CPythonGuild::GetEnemyGuildID(DWORD dwIndex)
{
	if (dwIndex >= ENEMY_GUILD_SLOT_MAX_COUNT)
		return 0;

	return m_adwEnemyGuildID[dwIndex];
}

BOOL CPythonGuild::IsDoingGuildWar()
{
	for (int i = 0; i < ENEMY_GUILD_SLOT_MAX_COUNT; ++i)
		if (0 != m_adwEnemyGuildID[i])
		{
			return TRUE;
		}

	return FALSE;
}

void CPythonGuild::__CalculateLevelAverage()
{
	m_dwMemberLevelSummary = 0;
	m_dwMemberLevelAverage = 0;
	m_dwMemberExperienceSummary = 0;

	if (m_GuildMemberDataVector.empty())
		return;

	TGuildMemberDataVector::iterator itor;

	// Sum Level & Experience
	itor = m_GuildMemberDataVector.begin();
	for (; itor != m_GuildMemberDataVector.end(); ++itor)
	{
		TGuildMemberData& rGuildMemberData = *itor;
		m_dwMemberLevelSummary += rGuildMemberData.byLevel;
		m_dwMemberExperienceSummary += rGuildMemberData.dwOffer;
	}

	assert(!m_GuildMemberDataVector.empty());
	m_dwMemberLevelAverage = m_dwMemberLevelSummary / m_GuildMemberDataVector.size();
}

struct CPythonGuild_SLessMemberGrade
{
		bool operator()(CPythonGuild::TGuildMemberData& rleft, CPythonGuild::TGuildMemberData& rright)
		{
			if (rleft.byGrade < rright.byGrade)
				return true;

			return false;
		}
};

void CPythonGuild::__SortMember()
{
	std::sort(m_GuildMemberDataVector.begin(), m_GuildMemberDataVector.end(), CPythonGuild_SLessMemberGrade());
}

BOOL CPythonGuild::__IsGradeData(BYTE byGradeNumber)
{
	return m_GradeDataMap.end() != m_GradeDataMap.find(byGradeNumber);
}

void CPythonGuild::__Initialize()
{
	ZeroMemory(&m_GuildInfo, sizeof(m_GuildInfo));
	ZeroMemory(&m_GuildSkillData, sizeof(m_GuildSkillData));
	ZeroMemory(&m_adwEnemyGuildID, ENEMY_GUILD_SLOT_MAX_COUNT * sizeof(DWORD));
	m_GradeDataMap.clear();
	m_GuildMemberDataVector.clear();
	m_dwMemberLevelSummary = 0;
	m_dwMemberLevelAverage = 0;
	m_bGuildEnable = FALSE;
	m_GuildNameMap.clear();
}

void CPythonGuild::Destroy()
{
	__Initialize();
}

CPythonGuild::CPythonGuild()
{
	__Initialize();
}
CPythonGuild::~CPythonGuild()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptObject* guildIsGuildEnable(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonGuild::Instance().IsGuildEnable());
}

ScriptObject* guildGetGuildID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonGuild::Instance().GetGuildID());
}

ScriptObject* guildHasGuildLand(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonGuild::Instance().HasGuildLand());
}

ScriptObject* guildGetGuildName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGuildID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGuildID))
	{
		CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
		return Script_BuildValue("s", rGuildInfo.szGuildName);
	}

	std::string strGuildName;
	if (!CPythonGuild::Instance().GetGuildName(iGuildID, &strGuildName))
		return Script_BuildValue("s", "Noname");

	return Script_BuildValue("s", strGuildName.c_str());
}

ScriptObject* guildGetGuildMasterName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();

	CPythonGuild::TGuildMemberData* pData;
	if (!CPythonGuild::Instance().GetMemberDataPtrByPID(rGuildInfo.dwMasterPID, &pData))
		return Script_BuildValue("s", "Noname");

	return Script_BuildValue("s", pData->strName.c_str());
}

ScriptObject* guildGetEnemyGuildName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildValue("s", "");

	DWORD dwEnemyGuildID = CPythonGuild::Instance().GetEnemyGuildID(iIndex);

	std::string strEnemyGuildName;
	if (!CPythonGuild::Instance().GetGuildName(dwEnemyGuildID, &strEnemyGuildName))
		return Script_BuildValue("s", "");

	return Script_BuildValue("s", strEnemyGuildName.c_str());
}

ScriptObject* guildGetGuildMoney(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
	return Script_BuildValue("i", rGuildInfo.dwGuildMoney);
}

ScriptObject* guildGetGuildBoardCommentCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	const CPythonGuild::TGuildBoardCommentDataVector& rCommentVector = CPythonGuild::Instance().GetGuildBoardCommentVector();
	return Script_BuildValue("i", rCommentVector.size());
}

ScriptObject* guildGetGuildBoardCommentData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	const CPythonGuild::TGuildBoardCommentDataVector& c_rCommentVector = CPythonGuild::Instance().GetGuildBoardCommentVector();
	if (DWORD(iIndex) >= c_rCommentVector.size())
		return Script_BuildValue("iss", 0, "Noname", "Noname");

	const CPythonGuild::TGuildBoardCommentData& c_rData = c_rCommentVector[iIndex];

	return Script_BuildValue("iss", c_rData.dwCommentID, c_rData.strName.c_str(), c_rData.strComment.c_str());
}

ScriptObject* guildGetGuildLevel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
	return Script_BuildValue("i", rGuildInfo.dwGuildLevel);
}

ScriptObject* guildGetGuildExperience(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();

	int GULID_MAX_LEVEL = 20;
	if (rGuildInfo.dwGuildLevel >= GULID_MAX_LEVEL)
		return Script_BuildValue("ii", 0, 0);

	unsigned lastExp = GetGuildLevelExp(rGuildInfo.dwGuildLevel);

	return Script_BuildValue("ii", rGuildInfo.dwCurrentExperience, lastExp - rGuildInfo.dwCurrentExperience);
}

ScriptObject* guildGetGuildMemberCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
	return Script_BuildValue("ii", rGuildInfo.dwCurrentMemberCount, rGuildInfo.dwMaxMemberCount);
}

ScriptObject* guildGetGuildMemberLevelSummary(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonGuild::Instance().GetGuildMemberLevelSummary());
}

ScriptObject* guildGetGuildMemberLevelAverage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonGuild::Instance().GetGuildMemberLevelAverage());
}

ScriptObject* guildGetGuildExperienceSummary(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonGuild::Instance().GetGuildExperienceSummary());
}

ScriptObject* guildGetGuildSkillPoint(ScriptObject* poSelf, ScriptObject* poArgs)
{
	const CPythonGuild::TGuildSkillData& c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Script_BuildValue("i", c_rSkillData.bySkillPoint);
}

ScriptObject* guildGetDragonPowerPoint(ScriptObject* poSelf, ScriptObject* poArgs)
{
	const CPythonGuild::TGuildSkillData& c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Script_BuildValue("ii", c_rSkillData.wGuildPoint, c_rSkillData.wMaxGuildPoint);
}

ScriptObject* guildGetGuildSkillLevel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	assert(FALSE && !"guildGetGuildSkillLevel - \xBB\xE7\xBF\xEB\xC7\xCF\xC1\xF6 \xBE\xCA\xB4\xC2 \xC7\xD4\xBC\xF6\xC0\xD4\xB4\xCF\xB4\xD9.");

	int iSkillIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSkillIndex))
		return Script_BuildException();

	const CPythonGuild::TGuildSkillData& c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Script_BuildValue("i", c_rSkillData.bySkillLevel[iSkillIndex]);
}

ScriptObject* guildGetSkillLevel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	std::map<DWORD, DWORD>::iterator itor = g_GuildSkillSlotToIndexMap.find(iSlotIndex);

	if (g_GuildSkillSlotToIndexMap.end() == itor)
		return Script_BuildValue("i", 0);

	DWORD dwSkillIndex = itor->second;
	assert(dwSkillIndex < CPythonGuild::GUILD_SKILL_MAX_NUM);

	const CPythonGuild::TGuildSkillData& c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Script_BuildValue("i", c_rSkillData.bySkillLevel[dwSkillIndex]);
}

ScriptObject* guildGetSkillMaxLevelNew(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	std::map<DWORD, DWORD>::iterator itor = g_GuildSkillSlotToIndexMap.find(iSlotIndex);

	if (g_GuildSkillSlotToIndexMap.end() == itor)
		return Script_BuildValue("i", 0);

	DWORD dwSkillIndex = itor->second;
	assert(dwSkillIndex < CPythonGuild::GUILD_SKILL_MAX_NUM);

	const CPythonGuild::TGuildSkillData& c_rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
	return Script_BuildValue("i", c_rSkillData.bySkillLevel[dwSkillIndex]);
}

ScriptObject* guildSetSkillIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();
	int iSkillIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iSkillIndex))
		return Script_BuildException();

	g_GuildSkillSlotToIndexMap.insert(std::make_pair(iSlotIndex, iSkillIndex));

	return Script_BuildNone();
}

ScriptObject* guildGetSkillIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	std::map<DWORD, DWORD>::iterator itor = g_GuildSkillSlotToIndexMap.find(iSlotIndex);

	if (g_GuildSkillSlotToIndexMap.end() == itor)
		return Script_BuildValue("i", 0);

	DWORD dwSkillIndex = itor->second;
	return Script_BuildValue("i", dwSkillIndex);
}

ScriptObject* guildGetGradeData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGradeNumber;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Script_BuildException();

	CPythonGuild::TGuildGradeData* pData;
	if (!CPythonGuild::Instance().GetGradeDataPtr(iGradeNumber, &pData))
		return Script_BuildValue("si", "?", 0);

	return Script_BuildValue("si", pData->strName.c_str(), pData->byAuthorityFlag);
}

ScriptObject* guildGetGradeName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGradeNumber;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Script_BuildException();

	CPythonGuild::TGuildGradeData* pData;
	if (!CPythonGuild::Instance().GetGradeDataPtr(iGradeNumber, &pData))
		return Script_BuildValue("s", "?");

	return Script_BuildValue("s", pData->strName.c_str());
}

ScriptObject* guildGetMemberCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonGuild::Instance().GetMemberCount());
}

ScriptObject* guildGetMemberData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonGuild::TGuildMemberData* pData;
	if (!CPythonGuild::Instance().GetMemberDataPtr(iIndex, &pData))
		return Script_BuildValue("isiiiii", -1, "", 0, 0, 0, 0, 0);

	return Script_BuildValue("isiiiii", pData->dwPID, pData->strName.c_str(), pData->byGrade, pData->byJob, pData->byLevel, pData->dwOffer, pData->byGeneralFlag);
}

ScriptObject* guildMemberIndexToPID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonGuild::TGuildMemberData* pData;
	if (!CPythonGuild::Instance().GetMemberDataPtr(iIndex, &pData))
		return Script_BuildValue("i", -1);

	return Script_BuildValue("i", pData->dwPID);
}

ScriptObject* guildIsMember(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CPythonGuild::TGuildMemberData* pData;
	if (CPythonGuild::Instance().GetMemberDataPtr(iIndex, &pData))
		return Script_BuildValue("i", TRUE);

	return Script_BuildValue("i", FALSE);
}

ScriptObject* guildIsMemberByName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonGuild::TGuildMemberData* pData;
	if (CPythonGuild::Instance().GetMemberDataPtrByName(szName, &pData))
		return Script_BuildValue("i", TRUE);

	return Script_BuildValue("i", FALSE);
}

ScriptObject* guildMainPlayerHasAuthority(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iAuthority;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iAuthority))
		return Script_BuildException();

	IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
	const char* c_szMainPlayerName = rPlayer.GetName();

	CPythonGuild::TGuildMemberData* pMemberData;
	if (!CPythonGuild::Instance().GetMemberDataPtrByName(c_szMainPlayerName, &pMemberData))
		return Script_BuildValue("i", FALSE);

	CPythonGuild::TGuildGradeData* pGradeData;
	if (!CPythonGuild::Instance().GetGradeDataPtr(pMemberData->byGrade, &pGradeData))
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", iAuthority == (pGradeData->byAuthorityFlag & iAuthority));
}

ScriptObject* guildDestroy(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonGuild::Instance().Destroy();
	g_GuildSkillSlotToIndexMap.clear();
	return Script_BuildNone();
}

ScriptObject* guildGuildIDToMarkID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int guild_id;
	if (!ScriptTuple_GetInteger(poArgs, 0, &guild_id))
		return Script_BuildException();

	return Script_BuildValue("i", CGuildMarkManager::Instance().GetMarkID(guild_id));
}

ScriptObject* guildGetMarkImageFilenameByMarkID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int markID;

	if (!ScriptTuple_GetInteger(poArgs, 0, &markID))
		return Script_BuildException();

	std::string imagePath;
	CGuildMarkManager::Instance().GetMarkImageFilename(markID / CGuildMarkImage::MARK_TOTAL_COUNT, imagePath);
	return Script_BuildValue("s", imagePath.c_str());
}

ScriptObject* guildGetMarkIndexByMarkID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int markID;

	if (!ScriptTuple_GetInteger(poArgs, 0, &markID))
		return Script_BuildException();

	return Script_BuildValue("i", markID % CGuildMarkImage::MARK_TOTAL_COUNT);
}

void initguild()
{
	static ScriptMethodDef s_methods[] =
		{
			// Enable
			{"IsGuildEnable", guildIsGuildEnable, SCRIPT_VARARGS, NULL},
			{"GuildIDToMarkID", guildGuildIDToMarkID, SCRIPT_VARARGS, NULL},
			{"GetMarkImageFilenameByMarkID", guildGetMarkImageFilenameByMarkID, SCRIPT_VARARGS, NULL},
			{"GetMarkIndexByMarkID", guildGetMarkIndexByMarkID, SCRIPT_VARARGS, NULL},

			// GuildInfo
			{"GetGuildID", guildGetGuildID, SCRIPT_VARARGS, NULL},
			{"HasGuildLand", guildHasGuildLand, SCRIPT_VARARGS, NULL},
			{"GetGuildName", guildGetGuildName, SCRIPT_VARARGS, NULL},
			{"GetGuildMasterName", guildGetGuildMasterName, SCRIPT_VARARGS, NULL},
			{"GetEnemyGuildName", guildGetEnemyGuildName, SCRIPT_VARARGS, NULL},
			{"GetGuildMoney", guildGetGuildMoney, SCRIPT_VARARGS, NULL},

			// BoardPage
			{"GetGuildBoardCommentCount", guildGetGuildBoardCommentCount, SCRIPT_VARARGS, NULL},
			{"GetGuildBoardCommentData", guildGetGuildBoardCommentData, SCRIPT_VARARGS, NULL},

			// MemberPage
			{"GetGuildLevel", guildGetGuildLevel, SCRIPT_VARARGS, NULL},
			{"GetGuildExperience", guildGetGuildExperience, SCRIPT_VARARGS, NULL},
			{"GetGuildMemberCount", guildGetGuildMemberCount, SCRIPT_VARARGS, NULL},
			{"GetGuildMemberLevelSummary", guildGetGuildMemberLevelSummary, SCRIPT_VARARGS, NULL},
			{"GetGuildMemberLevelAverage", guildGetGuildMemberLevelAverage, SCRIPT_VARARGS, NULL},
			{"GetGuildExperienceSummary", guildGetGuildExperienceSummary, SCRIPT_VARARGS, NULL},

			// SkillPage
			{"GetGuildSkillPoint", guildGetGuildSkillPoint, SCRIPT_VARARGS, NULL},
			{"GetDragonPowerPoint", guildGetDragonPowerPoint, SCRIPT_VARARGS, NULL},
			{"GetGuildSkillLevel", guildGetGuildSkillLevel, SCRIPT_VARARGS, NULL},
			{"GetSkillLevel", guildGetSkillLevel, SCRIPT_VARARGS, NULL},
			{"GetSkillMaxLevelNew", guildGetSkillMaxLevelNew, SCRIPT_VARARGS, NULL},

			{"SetSkillIndex", guildSetSkillIndex, SCRIPT_VARARGS, NULL},
			{"GetSkillIndex", guildGetSkillIndex, SCRIPT_VARARGS, NULL},

			// GradePage
			{"GetGradeData", guildGetGradeData, SCRIPT_VARARGS, NULL},
			{"GetGradeName", guildGetGradeName, SCRIPT_VARARGS, NULL},

			// About Member
			{"GetMemberCount", guildGetMemberCount, SCRIPT_VARARGS, NULL},
			{"GetMemberData", guildGetMemberData, SCRIPT_VARARGS, NULL},
			{"MemberIndexToPID", guildMemberIndexToPID, SCRIPT_VARARGS, NULL},
			{"IsMember", guildIsMember, SCRIPT_VARARGS, NULL},
			{"IsMemberByName", guildIsMemberByName, SCRIPT_VARARGS, NULL},
			{"MainPlayerHasAuthority", guildMainPlayerHasAuthority, SCRIPT_VARARGS, NULL},

			// Guild
			{"Destroy", guildDestroy, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("guild", s_methods);
	ScriptModule_AddIntConstant(poModule, "AUTH_ADD_MEMBER", GUILD_AUTH_ADD_MEMBER);
	ScriptModule_AddIntConstant(poModule, "AUTH_REMOVE_MEMBER", GUILD_AUTH_REMOVE_MEMBER);
	ScriptModule_AddIntConstant(poModule, "AUTH_NOTICE", GUILD_AUTH_NOTICE);
	ScriptModule_AddIntConstant(poModule, "AUTH_SKILL", GUILD_AUTH_SKILL);
	ScriptModule_AddIntConstant(poModule, "ENEMY_GUILD_SLOT_MAX_COUNT", CPythonGuild::ENEMY_GUILD_SLOT_MAX_COUNT);
}
