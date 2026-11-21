#pragma once

#include <Client/Feature/Skill/PythonSkill.h>
#include <Client/Game/GameType.h>
#include <Client/Map/MapType.h>

#include <cstdint>

class CPythonPlayer;
class CInstanceBase;

typedef struct SSkillInstance
{
		uint32_t dwIndex;
		int iType;
		int iGrade;
		int iLevel;
		float fcurEfficientPercentage;
		float fnextEfficientPercentage;
		int32_t isCoolTime;

		float fCoolTime;
		float fLastUsedTime;
		int32_t bActive;
} TSkillInstance;

class CPythonPlayerSkill
{
	public:
		explicit CPythonPlayerSkill(CPythonPlayer& rkOwner);

		void ChangeCurrentSkillNumberOnly(uint32_t dwSlotIndex);
		void ClearAffects();
		void ClickSkillSlot(uint32_t dwSlotIndex);
		bool FindSkillSlotIndexBySkillIndex(uint32_t dwSkillIndex, uint32_t* pdwSkillSlotIndex);
		void ResetAffect(uint32_t uAffect);
		void SetAffect(uint32_t uAffect);
		void SetComboSkillFlag(int32_t bFlag);
		void UseGuildSkill(uint32_t dwSkillSlotIndex);
		bool __CanUseSkill();
		bool __CheckDashAffect(CInstanceBase& rkInstMain);
		bool __CheckRestSkillCoolTime(uint32_t dwSkillSlotIndex);
		bool __CheckShortArrow(TSkillInstance& rkSkillInst, CPythonSkill::TSkillData& rkSkillData);
		bool __CheckShortLife(TSkillInstance& rkSkillInst, CPythonSkill::TSkillData& rkSkillData);
		bool __CheckShortMana(TSkillInstance& rkSkillInst, CPythonSkill::TSkillData& rkSkillData);
		bool __CheckSkillUsable(uint32_t dwSlotIndex);
		bool __CheckSpecialSkill(uint32_t dwSkillIndex);
		uint32_t __GetSkillTargetRange(CPythonSkill::TSkillData& rkSkillData);
		uint8_t __GetSkillType(uint32_t dwSkillSlotIndex);
		bool __HasEnoughArrow();
		bool __HasItem(uint32_t dwItemID);
		bool __ProcessEnemySkillTargetRange(CInstanceBase& rkInstMain, CInstanceBase& rkInstTarget, CPythonSkill::TSkillData& rkSkillData, uint32_t dwSkillSlotIndex);
		void __RunCoolTime(uint32_t dwSkillSlotIndex);
		void __SendUseSkill(uint32_t dwSkillSlotIndex, uint32_t dwTargetVID);
		void __UseCurrentSkill();
		bool __UseSkill(uint32_t dwSlotIndex);

	private:
		CPythonPlayer& m_rkOwner;
};
