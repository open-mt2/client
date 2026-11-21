#pragma once
#include <Client/Game/ClientConstants.h>
#include <Neoeter/Platform/Base/Singleton.h>
#include <list>
#include <map>

/*
 *	NPC 데이터 프로토 타잎을 관리 한다.
 */
class CPythonNonPlayer : public CSingleton<CPythonNonPlayer>
{
	public:
		enum EClickEvent
		{
			ON_CLICK_EVENT_NONE = 0,
			ON_CLICK_EVENT_BATTLE = 1,
			ON_CLICK_EVENT_SHOP = 2,
			ON_CLICK_EVENT_TALK = 3,
			ON_CLICK_EVENT_VEHICLE = 4,

			ON_CLICK_EVENT_MAX_NUM,
		};

		enum EMobEnchants
		{
			MOB_ENCHANT_CURSE,
			MOB_ENCHANT_SLOW,
			MOB_ENCHANT_POISON,
			MOB_ENCHANT_STUN,
			MOB_ENCHANT_CRITICAL,
			MOB_ENCHANT_PENETRATE,
			MOB_ENCHANTS_MAX_NUM
		};
		enum EMobResists
		{
			MOB_RESIST_SWORD,
			MOB_RESIST_TWOHAND,
			MOB_RESIST_DAGGER,
			MOB_RESIST_BELL,
			MOB_RESIST_FAN,
			MOB_RESIST_BOW,
			MOB_RESIST_FIRE,
			MOB_RESIST_ELECT,
			MOB_RESIST_MAGIC,
			MOB_RESIST_WIND,
			MOB_RESIST_POISON,
			MOB_RESISTS_MAX_NUM
		};

#define MOB_ATTRIBUTE_MAX_NUM 12
#define MOB_SKILL_MAX_NUM 5

#pragma pack(push)
#pragma pack(1)
		typedef struct SMobSkillLevel
		{
				uint32_t dwVnum;
				uint8_t bLevel;
		} TMobSkillLevel;

		typedef struct SMobTable
		{
				uint32_t dwVnum;
				char szName[CHARACTER_NAME_MAX_LEN + 1];
				char szLocaleName[CHARACTER_NAME_MAX_LEN + 1];

				uint8_t bType;		 // Monster, NPC
				uint8_t bRank;		 // PAWN, KNIGHT, KING
				uint8_t bBattleType; // MELEE, etc..
				uint8_t bLevel;		 // Level
				uint8_t bSize;

				uint32_t dwGoldMin;
				uint32_t dwGoldMax;
				uint32_t dwExp;
				uint32_t dwMaxHP;
				uint8_t bRegenCycle;
				uint8_t bRegenPercent;
				uint16_t wDef;

				uint32_t dwAIFlag;
				uint32_t dwRaceFlag;
				uint32_t dwImmuneFlag;

				uint8_t bStr, bDex, bCon, bInt;
				uint32_t dwDamageRange[2];

				short sAttackSpeed;
				short sMovingSpeed;
				uint8_t bAggresiveHPPct;
				uint16_t wAggressiveSight;
				uint16_t wAttackRange;

				char cEnchants[MOB_ENCHANTS_MAX_NUM];
				char cResists[MOB_RESISTS_MAX_NUM];

				uint32_t dwResurrectionVnum;
				uint32_t dwDropItemVnum;

				uint8_t bMountCapacity;
				uint8_t bOnClickType;

				uint8_t bEmpire;
				char szFolder[64 + 1];
				float fDamMultiply;
				uint32_t dwSummonVnum;
				uint32_t dwDrainSP;
				uint32_t dwMonsterColor;
				uint32_t dwPolymorphItemVnum;

				TMobSkillLevel Skills[MOB_SKILL_MAX_NUM];

				uint8_t bBerserkPoint;
				uint8_t bStoneSkinPoint;
				uint8_t bGodSpeedPoint;
				uint8_t bDeathBlowPoint;
				uint8_t bRevivePoint;
		} TMobTable;
#pragma pack(pop)

		typedef std::list<TMobTable*> TMobTableList;
		typedef std::map<uint32_t, TMobTable*> TNonPlayerDataMap;

	public:
		CPythonNonPlayer(void);
		virtual ~CPythonNonPlayer(void);

		void Clear();
		void Destroy();

		bool LoadNonPlayerData(const char* c_szFileName);

		const TMobTable* GetTable(uint32_t dwVnum);
		bool GetName(uint32_t dwVnum, const char** c_pszName);
		bool GetInstanceType(uint32_t dwVnum, uint8_t* pbType);
		uint8_t GetEventType(uint32_t dwVnum);
		uint8_t GetEventTypeByVID(uint32_t dwVID);
		uint32_t GetMonsterColor(uint32_t dwVnum);
		const char* GetMonsterName(uint32_t dwVnum);

		// Function for outer
		void GetMatchableMobList(int iLevel, int iInterval, TMobTableList* pMobTableList);

	protected:
		TNonPlayerDataMap m_NonPlayerDataMap;
};
