#include <Client/UI/StdAfx.h>
#include <Client/Player/PythonPlayer.h>
#include <Client/Player/PythonPlayerSkill.h>
#include <Client/Feature/TextTail/PythonTextTail.h>
#include <Client/Session/PythonNetworkStream.h>
#include <Client/UI/Instance/InstanceBase.h>
#include <Client/Feature/Chat/PythonChat.h>
#include <Client/Feature/Guild/PythonGuild.h>
#include <Client/GameData/ItemManager.h>

CPythonPlayerSkill::CPythonPlayerSkill(CPythonPlayer& rkOwner) : m_rkOwner(rkOwner)
{
}

void CPythonPlayerSkill::ClearAffects()
{
	PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "ClearAffects", Py_BuildValue("()"));
}

void CPythonPlayerSkill::SetAffect(uint32_t uAffect)
{
	PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "SetAffect", Py_BuildValue("(i)", uAffect));

	/////

	uint32_t dwSkillIndex;
	if (!m_rkOwner.AffectIndexToSkillIndex(uAffect, &dwSkillIndex))
		return;

	CPythonSkill::TSkillData* pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(dwSkillIndex, &pSkillData))
		return;

	if (!pSkillData->IsToggleSkill())
		return;

	uint32_t dwSkillSlotIndex;
	if (!m_rkOwner.GetSkillSlotIndex(dwSkillIndex, &dwSkillSlotIndex))
		return;

	m_rkOwner.__ActivateSkillSlot(dwSkillSlotIndex);
}

void CPythonPlayerSkill::ResetAffect(uint32_t uAffect)
{
	// 2004.07.17.myevan.스킬 아닌 이펙트가 안 사라지는 문제
	PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "ResetAffect", Py_BuildValue("(i)", uAffect));

	uint32_t dwSkillIndex;
	if (!m_rkOwner.AffectIndexToSkillIndex(uAffect, &dwSkillIndex))
		return;

	CPythonSkill::TSkillData* pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(dwSkillIndex, &pSkillData))
		return;

	if (!pSkillData->IsToggleSkill())
		return;

	uint32_t dwSkillSlotIndex;
	if (!m_rkOwner.GetSkillSlotIndex(dwSkillIndex, &dwSkillSlotIndex))
		return;

	m_rkOwner.__DeactivateSkillSlot(dwSkillSlotIndex);
}

bool CPythonPlayerSkill::FindSkillSlotIndexBySkillIndex(uint32_t dwSkillIndex, uint32_t* pdwSkillSlotIndex)
{
	for (int i = 0; i < SKILL_MAX_NUM; ++i)
	{
		TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[i];
		if (dwSkillIndex == rkSkillInst.dwIndex)
		{
			*pdwSkillSlotIndex = i;
			return true;
		}
	}

	return false;
}

void CPythonPlayerSkill::ChangeCurrentSkillNumberOnly(uint32_t dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return;

	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSlotIndex];

	CPythonSkill::TSkillData* pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(rkSkillInst.dwIndex, &pSkillData))
		return;

	if (!pSkillData->IsCanUseSkill())
		return;

	if (!m_rkOwner.GetInput().__IsRightButtonSkillMode())
	{
		if (!m_rkOwner.GetInput().__IsTarget())
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NEED_TARGET"));
			return;
		}

		ClickSkillSlot(dwSlotIndex);
	}
	else
	{
		m_rkOwner.m_dwcurSkillSlotIndex = dwSlotIndex;
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "ChangeCurrentSkill", Py_BuildValue("(i)", dwSlotIndex));
	}
}

void CPythonPlayerSkill::ClickSkillSlot(uint32_t dwSlotIndex)
{
	if (dwSlotIndex >= SKILL_MAX_NUM)
		return;

	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSlotIndex];

	CPythonSkill::TSkillData* pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(rkSkillInst.dwIndex, &pSkillData))
		return;

	if (CPythonSkill::SKILL_TYPE_GUILD == pSkillData->byType)
	{
		UseGuildSkill(dwSlotIndex);
		return;
	}

	if (!pSkillData->IsCanUseSkill())
		return;

	if (pSkillData->IsStandingSkill())
	{
		if (pSkillData->IsToggleSkill())
		{
			if (m_rkOwner.IsSkillActive(dwSlotIndex))
			{
				CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
				if (!pkInstMain)
					return;
				if (pkInstMain->GetBattle().IsUsingSkill())
					return;

				CPythonNetworkStream::Instance().SendUseSkillPacket(rkSkillInst.dwIndex);
				return;
			}
		}

		__UseSkill(dwSlotIndex);
		return;
	}

	if (m_rkOwner.m_dwcurSkillSlotIndex == dwSlotIndex)
	{
		__UseSkill(m_rkOwner.m_dwcurSkillSlotIndex);
		return;
	}

	if (!m_rkOwner.GetInput().__IsRightButtonSkillMode())
	{
		__UseSkill(dwSlotIndex);
	}
	else
	{
		m_rkOwner.m_dwcurSkillSlotIndex = dwSlotIndex;
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "ChangeCurrentSkill", Py_BuildValue("(i)", dwSlotIndex));
	}
}

bool CPythonPlayerSkill::__CheckSkillUsable(uint32_t dwSlotIndex)
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	if (dwSlotIndex >= SKILL_MAX_NUM)
		return false;

	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSlotIndex];

	CPythonSkill::TSkillData* pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(rkSkillInst.dwIndex, &pSkillData))
		return false;

	if (pkInstMain->IsMountingHorse())
	{
		if (!pSkillData->IsHorseSkill())
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NOT_HORSE_SKILL"));
			return false;
		}
	}

	if (pSkillData->IsHorseSkill())
	{
		if (!pkInstMain->IsMountingHorse())
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "HAVE_TO_RIDE"));
			return false;
		}
	}

	// 2004.07.26.levites - 안전지대에서 공격 못하도록 수정
	// NOTE : 공격 스킬은 안전지대에서 사용하지 못합니다 - [levites]
	if (pSkillData->IsAttackSkill())
	{
		if (pkInstMain->IsInSafe())
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "IN_SAFE"));

			return false;
		}
	}

	// NOTE : 패시브 스킬은 사용하지 못합니다 - [levites]
	if (!pSkillData->IsCanUseSkill())
		return false;
	//	if (CPythonSkill::SKILL_TYPE_PASSIVE == pSkillData->byType)
	//		return false;

	// NOTE : [Only Assassin] 빈병이 있는지 체크 합니다.
	if (pSkillData->IsNeedEmptyBottle())
	{
		if (!__HasItem(27995))
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NEED_EMPTY_BOTTLE"));
			return false;
		}
	}

	// NOTE : [Only Assassin] 독병이 있는지 체크 합니다.
	if (pSkillData->IsNeedPoisonBottle())
	{
		if (!__HasItem(27996))
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NEED_POISON_BOTTLE"));
			return false;
		}
	}

	// NOTE : 낚시 중일때는 스킬을 사용하지 못합니다.
	if (pkInstMain->IsFishingMode())
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "REMOVE_FISHING_ROD"));
		return false;
	}

	// NOTE : 레벨 체크
	if (m_rkOwner.m_sysIsLevelLimit)
	{
		if (rkSkillInst.iLevel <= 0)
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NOT_YET_LEARN"));
			return false;
		}
	}

	// NOTE : 들고 있는 무기 체크
	if (!pSkillData->CanUseWeaponType(pkInstMain->GetWeaponType()))
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NOT_MATCHABLE_WEAPON"));
		return false;
	}

	if (!pSkillData->IsHorseSkill()) // HORSE 스킬 중에 화살을 쓰지 않는 스킬이 있기 때문에
	{
		if (__CheckShortArrow(rkSkillInst, *pSkillData))
			return false;

		// NOTE : 활이 필요할 경우 화살 개수 체크
		if (pSkillData->IsNeedBow())
		{
			if (!__HasEnoughArrow())
				return false;
		}
	}

	if (__CheckDashAffect(*pkInstMain))
	{
		if (!pSkillData->IsChargeSkill())
		{
			if (__CheckRestSkillCoolTime(dwSlotIndex))
			{
				PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "WAIT_COOLTIME"));
				return false;
			}
		}
	}
	else
	{
		if (__CheckRestSkillCoolTime(dwSlotIndex))
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "WAIT_COOLTIME"));
			return false;
		}

		if (__CheckShortLife(rkSkillInst, *pSkillData))
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NOT_ENOUGH_HP"));
			return false;
		}

		if (__CheckShortMana(rkSkillInst, *pSkillData))
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NOT_ENOUGH_SP"));
			return false;
		}
	}

	return true;
}

bool CPythonPlayerSkill::__CheckShortArrow(TSkillInstance& rkSkillInst, CPythonSkill::TSkillData& rkSkillData)
{
	if (!rkSkillData.IsNeedBow())
		return false;

	if (__HasEnoughArrow())
		return false;

	return true;
}

bool CPythonPlayerSkill::__CheckShortMana(TSkillInstance& rkSkillInst, CPythonSkill::TSkillData& rkSkillData)
{
	extern const uint32_t c_iSkillIndex_Summon;
	if (c_iSkillIndex_Summon == rkSkillInst.dwIndex)
		return false;

	int iNeedSP = rkSkillData.GetNeedSP(rkSkillInst.fcurEfficientPercentage);
	int icurSP = m_rkOwner.GetStatus(POINT_SP);

	// NOTE : ToggleSkill 이 아닌데 소모 SP 가 0 이다.
	if (!rkSkillData.IsToggleSkill())
	{
		if (iNeedSP == 0)
		{
			CPythonChat::Instance().AppendChat(CHAT_TYPE_INFO, "!!! Find strange game data. Please reinstall OpenMT2.");
			return true;
		}
	}

	if (rkSkillData.CanUseIfNotEnough())
	{
		if (icurSP <= 0)
			return true;
	}
	else
	{
		if (-1 != iNeedSP)
			if (iNeedSP > icurSP)
				return true;
	}

	return false;
}

bool CPythonPlayerSkill::__CheckShortLife(TSkillInstance& rkSkillInst, CPythonSkill::TSkillData& rkSkillData)
{
	if (!rkSkillData.IsUseHPSkill())
		return false;

	uint32_t dwNeedHP = rkSkillData.GetNeedSP(rkSkillInst.fcurEfficientPercentage);
	if (dwNeedHP <= m_rkOwner.GetStatus(POINT_HP))
		return false;

	return true;
}

bool CPythonPlayerSkill::__CheckRestSkillCoolTime(uint32_t dwSlotIndex)
{
	if (!m_rkOwner.m_sysIsCoolTime)
		return false;

	if (dwSlotIndex >= SKILL_MAX_NUM)
		return false;

	float fElapsedTime = CTimer::Instance().GetCurrentSecond() - m_rkOwner.m_playerStatus.aSkill[dwSlotIndex].fLastUsedTime;
	if (fElapsedTime >= m_rkOwner.m_playerStatus.aSkill[dwSlotIndex].fCoolTime)
		return false;

	return true;
}

bool CPythonPlayerSkill::__CheckDashAffect(CInstanceBase& rkInstMain)
{
	return rkInstMain.GetBattle().IsAffect(CInstanceBase::AFFECT_DASH);
}

void CPythonPlayerSkill::__UseCurrentSkill()
{
	__UseSkill(m_rkOwner.m_dwcurSkillSlotIndex);
}

uint32_t CPythonPlayerSkill::__GetSkillTargetRange(CPythonSkill::TSkillData& rkSkillData)
{
	return rkSkillData.GetTargetRange() + m_rkOwner.GetStatus(POINT_BOW_DISTANCE) * 100;
}

bool CPythonPlayerSkill::__ProcessEnemySkillTargetRange(CInstanceBase& rkInstMain, CInstanceBase& rkInstTarget, CPythonSkill::TSkillData& rkSkillData, uint32_t dwSkillSlotIndex)
{
	uint32_t dwSkillTargetRange = __GetSkillTargetRange(rkSkillData);
	float fSkillTargetRange = float(dwSkillTargetRange);
	if (fSkillTargetRange <= 0.0f)
		return true;

	// #0000806: [M2EU] 수룡에게 무사(나한군) 탄환격 스킬 사용 안됨
	float fTargetDistance = rkInstMain.GetDistance(&rkInstTarget);

	extern bool IS_HUGE_RACE(unsigned int vnum);
	if (IS_HUGE_RACE(rkInstTarget.GetRace()))
	{
		fTargetDistance -= 200.0f; // TEMP: 일단 하드 코딩 처리. 정석적으로는 바운드 스피어를 고려해야함
	}

	if (fTargetDistance >= fSkillTargetRange)
	{
		if (rkSkillData.IsChargeSkill())
		{
			if (!m_rkOwner.GetInput().__IsReservedUseSkill(dwSkillSlotIndex))
				__SendUseSkill(dwSkillSlotIndex, 0);
		}

		m_rkOwner.GetInput().__ReserveUseSkill(rkInstTarget.GetVirtualID(), dwSkillSlotIndex, dwSkillTargetRange);

		return false;
	}

	// 2004.07.05.myevan. 궁신탄영 사용시 맵에 끼임. 사용하기전 갈수 있는곳 체크
	TPixelPosition kPPosTarget;
	rkInstTarget.GetTransform().NEW_GetPixelPosition(&kPPosTarget);

	IBackground& rkBG = IBackground::Instance();
	if (rkBG.IsBlock(kPPosTarget.x, kPPosTarget.y))
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "CANNOT_APPROACH"));
		return false;
	}

	return true;
}

bool CPythonPlayerSkill::__CanUseSkill()
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	if (m_rkOwner.IsObserverMode())
		return false;

	// Fix me
	// 뉴마운트. 승마스킬레벨 20 미만인 경우, 고급 마운트를 타고 승마 관련 스킬 못 쓰도록 못하도록 하드 코딩...
	// 나중에 시간 나면 can use skill 체크를 서버에서 해주자...
	if (pkInstMain->IsMountingHorse() && (m_rkOwner.GetSkillGrade(109) < 1 && m_rkOwner.GetSkillLevel(109) < 20))
	{
		return false;
	}

	return pkInstMain->CanUseSkill();
}

bool CPythonPlayerSkill::__UseSkill(uint32_t dwSlotIndex)
{
	// PrivateShop
	if (m_rkOwner.IsOpenPrivateShop())
	{
		return true;
	}

	if (!__CanUseSkill())
	{
		return false;
	}

	if (dwSlotIndex >= SKILL_MAX_NUM)
	{
		Tracenf("CPythonPlayerSkill::__UseSkill(dwSlotIndex=%d) It's not available skill slot number", dwSlotIndex);
		return false;
	}

	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSlotIndex];

	if (__CheckSpecialSkill(rkSkillInst.dwIndex))
	{
		return true;
	}

	CPythonSkill::TSkillData* pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(rkSkillInst.dwIndex, &pSkillData))
	{
		Tracenf("CPythonPlayerSkill::__UseSkill(dwSlotIndex=%d) There is no skill data", dwSlotIndex);
		return false;
	}

	if (pSkillData->IsToggleSkill())
	{
		if (m_rkOwner.IsSkillActive(dwSlotIndex))
		{
			CPythonNetworkStream::Instance().SendUseSkillPacket(rkSkillInst.dwIndex, dwSlotIndex);
			return false;
		}
	}

	if (!__CheckSkillUsable(dwSlotIndex))
	{
		return false;
	}

	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
	{
		Tracenf("CPythonPlayerSkill::__UseSkill(dwSlotIndex=%d) There is no main player", dwSlotIndex);
		return false;
	}

	if (pkInstMain->GetBattle().IsUsingSkill())
	{
		return false;
	}

	CInstanceBase* pkInstTarget = NULL;

	// NOTE : 타겟이 필요한 경우
	if (pSkillData->IsNeedTarget() ||
		pSkillData->CanChangeDirection() ||
		pSkillData->IsAutoSearchTarget())
	{
		if (pSkillData->IsNeedCorpse())
			pkInstTarget = m_rkOwner.GetInput().__GetDeadTargetInstancePtr();
		else
			pkInstTarget = m_rkOwner.GetInput().__GetAliveTargetInstancePtr();

		// 현재 타겟이 없으면..
		if (!pkInstTarget)
		{
			// 업데이트하고..
			m_rkOwner.GetInput().__ChangeTargetToPickedInstance();

			// 다시 얻어낸다.
			if (pSkillData->IsNeedCorpse())
				pkInstTarget = m_rkOwner.GetInput().__GetDeadTargetInstancePtr();
			else
				pkInstTarget = m_rkOwner.GetInput().__GetAliveTargetInstancePtr();
		}

		if (pkInstTarget)
		{
			if (pSkillData->IsOnlyForAlliance())
			{
				if (pkInstMain == pkInstTarget)
				{
					if (!pSkillData->CanUseForMe())
					{
						PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "CANNOT_USE_SELF"));
						return false;
					}
				}
				else if (!pkInstMain->IsAttackableInstance(*pkInstTarget) && pkInstTarget->IsPC())
				{
					uint32_t dwSkillRange = __GetSkillTargetRange(*pSkillData);

					if (dwSkillRange > 0)
					{
						float fDistance = pkInstMain->GetDistance(pkInstTarget);
						if (fDistance >= float(dwSkillRange))
						{
							m_rkOwner.GetInput().__ReserveUseSkill(pkInstTarget->GetVirtualID(), dwSlotIndex, dwSkillRange);
							return false;
						}
					}
				}
				else
				{
					if (pSkillData->CanUseForMe())
					{
						pkInstTarget = pkInstMain;
						Tracef(" [ALERT] Reset to yourself because the skill is targeted at an enemy even though you're using it on a teammate\n");
					}
					else
					{
						PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "ONLY_FOR_ALLIANCE"));
						return false;
					}
				}
			}
			else
			{
				if (pkInstTarget->IsInSafe())
				{
					PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "CANNOT_ATTACK_ENEMY_IN_SAFE_AREA"));
					return false;
				}

				if (pkInstMain->IsAttackableInstance(*pkInstTarget))
				{
					if (!__ProcessEnemySkillTargetRange(*pkInstMain, *pkInstTarget, *pSkillData, dwSlotIndex))
						return false;
				}
				else
				{
					PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "CANNOT_ATTACK"));
					return false;
				}
			}

			pkInstMain->GetBattle().SetFlyTargetInstance(*pkInstTarget);

			if (pkInstMain != pkInstTarget)
			{
				if (pkInstMain->GetBattle().IsFlyTargetObject())
				{
					pkInstMain->GetTransform().NEW_LookAtFlyTarget();
				}
				else
				{
					pkInstMain->GetTransform().NEW_LookAtDestInstance(*pkInstTarget);
				}
			}
		}
		else
		{
			if (pSkillData->IsAutoSearchTarget())
			{
				if (pkInstMain->GetBattle().NEW_GetFrontInstance(&pkInstTarget, 2000.0f))
				{
					m_rkOwner.GetInput().SetTarget(pkInstTarget->GetVirtualID());
					if (!__ProcessEnemySkillTargetRange(*pkInstMain, *pkInstTarget, *pSkillData, dwSlotIndex))
						return false;
				}
				else
				{
					PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NEED_TARGET"));
					return false;
				}
			}

			if (pSkillData->CanUseForMe())
			{
				pkInstTarget = pkInstMain;
				pkInstMain->GetBattle().SetFlyTargetInstance(*pkInstMain);
				Tracef(" [ALERT] Use on player because there are no targets\n");
			}
			else if (pSkillData->IsNeedCorpse())
			{
				PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "ONLY_FOR_CORPSE"));
				return false;
			}
			else
			{
				PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NEED_TARGET"));
				return false;
			}
		}
	}

	if (pSkillData->CanChangeDirection())
	{
		uint32_t dwPickedActorID;
		TPixelPosition kPPosPickedGround;

		if (pkInstTarget && pkInstTarget != pkInstMain)
		{
			pkInstMain->GetTransform().NEW_LookAtDestInstance(*pkInstTarget);
		}
		else if (m_rkOwner.__GetPickedActorID(&dwPickedActorID))
		{
			CInstanceBase* pkInstVictim = m_rkOwner.NEW_FindActorPtr(dwPickedActorID);
			if (pkInstVictim)
				pkInstMain->GetTransform().NEW_LookAtDestInstance(*pkInstVictim);
		}
		else if (m_rkOwner.__GetPickedGroundPos(&kPPosPickedGround))
		{
			pkInstMain->GetTransform().NEW_LookAtDestPixelPosition(kPPosPickedGround);
		}
		else
		{
			Tracenf("CPythonPlayerSkill::__UseSkill(%d) - Need to set the orientation relative to the screen", dwSlotIndex);
		}
	}

	// 관격술 처리
	uint32_t dwTargetMaxCount = pSkillData->GetTargetCount(rkSkillInst.fcurEfficientPercentage);
	uint32_t dwRange = __GetSkillTargetRange(*pSkillData);
	if (dwTargetMaxCount > 0 && pkInstTarget)
	{
		uint32_t dwTargetCount = 1;
		std::vector<CInstanceBase*> kVct_pkInstTarget;

		if (pSkillData->IsFanRange())
		{
			if (pkInstMain->GetBattle().NEW_GetInstanceVectorInFanRange(float(dwRange), *pkInstTarget, &kVct_pkInstTarget))
			{
				std::vector<CInstanceBase*>::iterator i;
				for (i = kVct_pkInstTarget.begin(); i != kVct_pkInstTarget.end(); ++i)
				{
					if (dwTargetCount >= dwTargetMaxCount)
						break;

					CInstanceBase* pkInstEach = *i;

					if (pkInstTarget != pkInstEach && !pkInstEach->IsDead())
					{
						pkInstMain->GetBattle().AddFlyTargetInstance(*pkInstEach);
						CPythonNetworkStream::Instance().SendAddFlyTargetingPacket(pkInstEach->GetVirtualID(), pkInstEach->GetGraphicThingInstanceRef().OnGetFlyTargetPosition());

						dwTargetCount++;
					}
				}
			}
		}
		else if (pSkillData->IsCircleRange())
		{
			if (pkInstMain->GetBattle().NEW_GetInstanceVectorInCircleRange(float(dwRange), &kVct_pkInstTarget))
			{
				std::vector<CInstanceBase*>::iterator i;
				for (i = kVct_pkInstTarget.begin(); i != kVct_pkInstTarget.end(); ++i)
				{
					if (dwTargetCount >= dwTargetMaxCount)
						break;

					CInstanceBase* pkInstEach = *i;

					if (pkInstTarget != pkInstEach && !pkInstEach->IsDead())
					{
						pkInstMain->GetBattle().AddFlyTargetInstance(*pkInstEach);
						CPythonNetworkStream::Instance().SendAddFlyTargetingPacket(pkInstEach->GetVirtualID(), pkInstEach->GetGraphicThingInstanceRef().OnGetFlyTargetPosition());

						dwTargetCount++;
					}
				}
			}
		}

		if (dwTargetCount < dwTargetMaxCount)
		{
			while (dwTargetCount < dwTargetMaxCount)
			{
				TPixelPosition kPPosDst;
				pkInstMain->GetBattle().NEW_GetRandomPositionInFanRange(*pkInstTarget, &kPPosDst);

				kPPosDst.x = kPPosDst.x;
				kPPosDst.y = -kPPosDst.y;

				pkInstMain->GetBattle().AddFlyTargetPosition(kPPosDst);
				CPythonNetworkStream::Instance().SendAddFlyTargetingPacket(0, kPPosDst);

				dwTargetCount++;
			}
		}
	}

	/////
	// NOTE : 멀리서 적을 클릭해놓고 스킬을 쓰면 스킬을 쓴뒤 바로 적을 공격하는 문제를 수정하기 위한 코드 - [levites]
	m_rkOwner.GetInput().__ClearReservedAction();
	/////

	if (!pSkillData->IsNoMotion())
	{
		uint32_t dwMotionIndex = pSkillData->GetSkillMotionIndex(rkSkillInst.iGrade);
		uint32_t dwLoopCount = pSkillData->GetMotionLoopCount(rkSkillInst.fcurEfficientPercentage);
		if (!pkInstMain->GetBattle().NEW_UseSkill(rkSkillInst.dwIndex, dwMotionIndex, dwLoopCount, pSkillData->IsMovingSkill() ? true : false))
		{
			Tracenf("CPythonPlayerSkill::__UseSkill(%d) - pkInstMain->NEW_UseSkill - ERROR", dwSlotIndex);
			return false;
		}
	}

	uint32_t dwTargetVID = pkInstTarget ? pkInstTarget->GetVirtualID() : 0;

	__SendUseSkill(dwSlotIndex, dwTargetVID);
	return true;
}

void CPythonPlayerSkill::__SendUseSkill(uint32_t dwSkillSlotIndex, uint32_t dwTargetVID)
{
	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSkillSlotIndex];

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendUseSkillPacket(rkSkillInst.dwIndex, dwTargetVID);

	__RunCoolTime(dwSkillSlotIndex);
}

uint8_t CPythonPlayerSkill::__GetSkillType(uint32_t dwSkillSlotIndex)
{
	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSkillSlotIndex];

	CPythonSkill::TSkillData* pkSkillData;
	CPythonSkill& rkPythonSkill = CPythonSkill::Instance();
	if (!rkPythonSkill.GetSkillData(rkSkillInst.dwIndex, &pkSkillData))
	{
		// TraceError("CPythonPlayerSkill::__GetSkillType(dwSkillSlotIndex=%d) - NOT CHECK", dwSkillSlotIndex);
		return 0;
	}
	return pkSkillData->GetType();
}

void CPythonPlayerSkill::__RunCoolTime(uint32_t dwSkillSlotIndex)
{
	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSkillSlotIndex];

	CPythonSkill::TSkillData* pkSkillData;
	if (!CPythonSkill::Instance().GetSkillData(rkSkillInst.dwIndex, &pkSkillData))
	{
		TraceError("CPythonPlayerSkill::__SendUseSkill(dwSkillSlotIndex=%d) - NOT CHECK", dwSkillSlotIndex);
		return;
	}

	CPythonSkill::TSkillData& rkSkillData = *pkSkillData;

	rkSkillInst.fCoolTime = rkSkillData.GetSkillCoolTime(rkSkillInst.fcurEfficientPercentage);
	rkSkillInst.fLastUsedTime = CTimer::Instance().GetCurrentSecond();

	int iSpd = 100 - m_rkOwner.GetStatus(POINT_CASTING_SPEED);
	if (iSpd > 0)
		iSpd = 100 + iSpd;
	else if (iSpd < 0)
		iSpd = 10000 / (100 - iSpd);
	else
		iSpd = 100;

	rkSkillInst.fCoolTime = rkSkillInst.fCoolTime * iSpd / 100;

	PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "RunUseSkillEvent", Py_BuildValue("(if)", dwSkillSlotIndex, rkSkillInst.fCoolTime));
}

bool CPythonPlayerSkill::__HasEnoughArrow()
{
	CItemData* pItemData;
	if (CItemManager::Instance().GetItemDataPointer(m_rkOwner.GetItemIndex(TItemPos(INVENTORY, c_Equipment_Arrow)), &pItemData))
		if (CItemData::ITEM_TYPE_WEAPON == pItemData->GetType())
			if (CItemData::WEAPON_ARROW == pItemData->GetSubType())
			{
				return true;
			}

	PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotShotError", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "EMPTY_ARROW"));
	return false;
}

bool CPythonPlayerSkill::__HasItem(uint32_t dwItemID)
{
	for (int i = 0; i < c_Inventory_Count; ++i)
	{
		if (dwItemID == m_rkOwner.GetItemIndex(TItemPos(INVENTORY, i)))
			return true;
	}
	for (int i = 0; i < c_DragonSoul_Inventory_Count; ++i)
	{
		if (dwItemID == m_rkOwner.GetItemIndex(TItemPos(DRAGON_SOUL_INVENTORY, i)))
			return true;
	}

	return false;
}

extern const uint32_t c_iSkillIndex_Tongsol;
extern const uint32_t c_iSkillIndex_Fishing;
extern const uint32_t c_iSkillIndex_Mining;
extern const uint32_t c_iSkillIndex_Making;
extern const uint32_t c_iSkillIndex_Combo;
extern const uint32_t c_iSkillIndex_Language1;
extern const uint32_t c_iSkillIndex_Language2;
extern const uint32_t c_iSkillIndex_Language3;
extern const uint32_t c_iSkillIndex_Polymorph;

void CPythonPlayerSkill::UseGuildSkill(uint32_t dwSkillSlotIndex)
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;
	if (!pkInstMain->CanUseSkill())
		return;

	///////////////////////////////////////////////////////////////////////////////////////////////

	if (dwSkillSlotIndex >= SKILL_MAX_NUM)
	{
		Tracenf("CPythonPlayerSkill::UseGuildSkill(dwSkillSlotIndex=%d) It's not available skill slot number", dwSkillSlotIndex);
		return;
	}

	TSkillInstance& rkSkillInst = m_rkOwner.m_playerStatus.aSkill[dwSkillSlotIndex];

	uint32_t dwSkillIndex = rkSkillInst.dwIndex;

	CPythonSkill::TSkillData* pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(dwSkillIndex, &pSkillData))
		return;

	if (__CheckRestSkillCoolTime(dwSkillSlotIndex))
	{
		PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "WAIT_COOLTIME"));
		return;
	}

	if (pSkillData->IsOnlyForGuildWar())
	{
		if (!CPythonGuild::Instance().IsDoingGuildWar())
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "ONLY_FOR_GUILD_WAR"));
			return;
		}
	}

	uint32_t dwMotionIndex = pSkillData->GetSkillMotionIndex();
	if (!pkInstMain->GetBattle().NEW_UseSkill(dwSkillIndex, dwMotionIndex, 1, false))
	{
		Tracenf("CPythonPlayerSkill::UseGuildSkill(%d) - pkInstMain->NEW_UseSkill - ERROR", dwSkillIndex);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	CPythonNetworkStream::Instance().SendGuildUseSkillPacket(dwSkillIndex, 0);
	__RunCoolTime(dwSkillSlotIndex);
}

void CPythonPlayerSkill::SetComboSkillFlag(int32_t bFlag)
{
	uint32_t dwSlotIndex;
	if (!m_rkOwner.GetSkillSlotIndex(c_iSkillIndex_Combo, &dwSlotIndex))
	{
		Tracef("CPythonPlayerSkill::SetComboSkillFlag(killIndex=%d) - Can't Find Slot Index\n", c_iSkillIndex_Combo);
		return;
	}

	int iLevel = m_rkOwner.GetSkillLevel(dwSlotIndex);
	if (iLevel <= 0)
	{
		Tracef("CPythonPlayerSkill::SetComboSkillFlag(skillIndex=%d, skillLevel=%d) - Invalid Combo Skill Level\n", c_iSkillIndex_Combo, iLevel);
		return;
	}

	iLevel = MIN(iLevel, 2);

	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return;

	if (bFlag)
	{
		pkInstMain->SetComboType(iLevel);
		m_rkOwner.__ActivateSkillSlot(dwSlotIndex);
	}
	else
	{
		pkInstMain->SetComboType(0);
		m_rkOwner.__DeactivateSkillSlot(dwSlotIndex);
	}
}

bool CPythonPlayerSkill::__CheckSpecialSkill(uint32_t dwSkillIndex)
{
	CInstanceBase* pkInstMain = m_rkOwner.NEW_GetMainActorPtr();
	if (!pkInstMain)
		return false;

	// Fishing
	if (c_iSkillIndex_Fishing == dwSkillIndex)
	{
		if (pkInstMain->IsFishingMode())
		{
			m_rkOwner.GetInput().NEW_Fishing();
		}
		else
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "EQUIP_FISHING_ROD"));
		}
		return true;
	}

	// Combo
	else if (c_iSkillIndex_Combo == dwSkillIndex)
	{
		uint32_t dwSlotIndex;
		if (!m_rkOwner.GetSkillSlotIndex(dwSkillIndex, &dwSlotIndex))
			return false;

		int iLevel = m_rkOwner.GetSkillLevel(dwSlotIndex);
		if (iLevel > 0)
		{
			CPythonNetworkStream::Instance().SendUseSkillPacket(dwSkillIndex);
		}
		else
		{
			PyCallClassMemberFunc(m_rkOwner.m_ppyGameWindow, "OnCannotUseSkill", Py_BuildValue("(is)", m_rkOwner.GetMainCharacterIndex(), "NOT_YET_LEARN"));
		}

		return true;
	}

	return false;
}
