#include <Client/Game/StdAfx.h>
#include <Client/Game/Balance.h>

unsigned GetGuildLevelExp(int level)
{
	static const int GUILD_LEVEL_MAX = 20;

	if (level < 0 || level >= GUILD_LEVEL_MAX)
		return 0;

	static const DWORD GUILD_EXP_LIST[GUILD_LEVEL_MAX + 1] =
		{
			0,		   // 0
			6000UL,	   // 1
			18000UL,   // 2
			36000UL,   // 3
			64000UL,   // 4
			94000UL,   // 5
			130000UL,  // 6
			172000UL,  // 7
			220000UL,  // 8
			274000UL,  // 9
			334000UL,  // 10
			400000UL,  // 11
			600000UL,  // 12
			840000UL,  // 13
			1120000UL, // 14
			1440000UL, // 15
			1800000UL, // 16
			2600000UL, // 17
			3200000UL, // 18
			4000000UL, // 19
			16800000UL // 20
		};

	return GUILD_EXP_LIST[level];
}

int GetSkillPower(unsigned level)
{
	static const unsigned SKILL_POWER_NUM = 50;

	if (level >= SKILL_POWER_NUM)
		return 0;

	static const unsigned SKILL_POWERS[SKILL_POWER_NUM] =
		{
			0,
			5,
			6,
			8,
			10,
			12,
			14,
			16,
			18,
			20,
			22,
			24,
			26,
			28,
			30,
			32,
			34,
			36,
			38,
			40,
			50, // master
			52,
			54,
			56,
			58,
			60,
			63,
			66,
			69,
			72,
			82, // grand_master
			85,
			88,
			91,
			94,
			98,
			102,
			106,
			110,
			115,
			125, // perfect_master
			125,
		};

	return SKILL_POWERS[level];
}
