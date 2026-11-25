#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonPlayer.h>
#include <Client/UI/PythonApplication.h>

extern const DWORD c_iSkillIndex_Tongsol = 121;
extern const DWORD c_iSkillIndex_Combo = 122;
extern const DWORD c_iSkillIndex_Fishing = 123;
extern const DWORD c_iSkillIndex_Mining = 124;
extern const DWORD c_iSkillIndex_Making = 125;
extern const DWORD c_iSkillIndex_Language1 = 126;
extern const DWORD c_iSkillIndex_Language2 = 127;
extern const DWORD c_iSkillIndex_Language3 = 128;
extern const DWORD c_iSkillIndex_Polymorph = 129;
extern const DWORD c_iSkillIndex_Riding = 130;
extern const DWORD c_iSkillIndex_Summon = 131;

enum
{
	EMOTION_CLAP = 1,
	EMOTION_CHEERS_1,
	EMOTION_CHEERS_2,
	EMOTION_DANCE_1,
	EMOTION_DANCE_2,
	EMOTION_DANCE_3,
	EMOTION_DANCE_4,
	EMOTION_DANCE_5,
	EMOTION_DANCE_6, // 강남스타일
	EMOTION_CONGRATULATION,
	EMOTION_FORGIVE,
	EMOTION_ANGRY,

	EMOTION_ATTRACTIVE,
	EMOTION_SAD,
	EMOTION_SHY,
	EMOTION_CHEERUP,
	EMOTION_BANTER,
	EMOTION_JOY,

	EMOTION_KISS = 51,
	EMOTION_FRENCH_KISS,
	EMOTION_SLAP,

};

std::map<int, CGraphicImage*> m_kMap_iEmotionIndex_pkIconImage;

extern int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
class CBeltInventoryHelper
{
	public:
		typedef BYTE TGradeUnit;

		static TGradeUnit GetBeltGradeByRefineLevel(int refineLevel)
		{
			static TGradeUnit beltGradeByLevelTable[] =
				{
					0, // 벨트+0
					1, // +1
					1, // +2
					2, // +3
					2, // +4,
					3, // +5
					4, // +6,
					5, // +7,
					6, // +8,
					7, // +9
				};

			return beltGradeByLevelTable[refineLevel];
		}

		// 현재 벨트 레벨을 기준으로, 어떤 셀들을 이용할 수 있는지 리턴
		static const TGradeUnit* GetAvailableRuleTableByGrade()
		{
			/**
				벨트는 총 +0 ~ +9 레벨을 가질 수 있으며, 레벨에 따라 7단계 등급으로 구분되어 인벤토리가 활성 화 된다.
				벨트 레벨에 따른 사용 가능한 셀은 아래 그림과 같음. 현재 등급 >= 활성가능 등급이면 사용 가능.
				(단, 현재 레벨이 0이면 무조건 사용 불가, 괄호 안의 숫자는 등급)

					2(1)  4(2)  6(4)  8(6)
					5(3)  5(3)  6(4)  8(6)
					7(5)  7(5)  7(5)  8(6)
					9(7)  9(7)  9(7)  9(7)

				벨트 인벤토리의 크기는 4x4 (16칸)
			*/

			static TGradeUnit availableRuleByGrade[c_Belt_Inventory_Slot_Count] = {
				1, 2, 4, 6,
				3, 3, 4, 6,
				5, 5, 5, 6,
				7, 7, 7, 7};

			return availableRuleByGrade;
		}

		static bool IsAvailableCell(WORD cell, int beltGrade /*int beltLevel*/)
		{
			// 기획 또 바뀜.. 아놔...
			// const TGradeUnit beltGrade = GetBeltGradeByRefineLevel(beltLevel);
			const TGradeUnit* ruleTable = GetAvailableRuleTableByGrade();

			return ruleTable[cell] <= beltGrade;
		}
};
#endif

ScriptObject* playerPickCloseItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().PickCloseItem();
	return Script_BuildNone();
}

ScriptObject* playerSetGameWindow(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* pyHandle;
	if (!ScriptTuple_GetObject(poArgs, 0, &pyHandle))
		return Script_BadArgument();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetGameWindow(pyHandle);
	return Script_BuildNone();
}

ScriptObject* playerSetQuickCameraMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nIsEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nIsEnable))
		return Script_BadArgument();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetQuickCameraMode(nIsEnable ? true : false);

	return Script_BuildNone();
}

// Test Code
ScriptObject* playerSetMainCharacterIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CPythonPlayer::Instance().SetMainCharacterIndex(iVID);
	CPythonCharacterManager::Instance().SetMainInstance(iVID);

	return Script_BuildNone();
}
// Test Code

ScriptObject* playerGetMainCharacterIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().GetMainCharacterIndex());
}

ScriptObject* playerGetMainCharacterName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("s", CPythonPlayer::Instance().GetName());
}

ScriptObject* playerGetMainCharacterPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TPixelPosition kPPosMainActor;
	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_GetMainActorPosition(&kPPosMainActor);
	return Script_BuildValue("fff", kPPosMainActor.x, kPPosMainActor.y, kPPosMainActor.z);
}

ScriptObject* playerIsMainCharacterIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().IsMainCharacterIndex(iVID));
}

ScriptObject* playerCanAttackInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase* pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Script_BuildValue("i", 0);
	if (!pTargetInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pMainInstance->IsAttackableInstance(*pTargetInstance));
}

ScriptObject* playerIsActingEmotion(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pMainInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pMainInstance->IsActingEmotion());
}

ScriptObject* playerIsPVPInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase* pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Script_BuildValue("i", 0);
	if (!pTargetInstance)
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", pMainInstance->IsPVPInstance(*pTargetInstance));
}

ScriptObject* playerIsSameEmpire(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase* pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Script_BuildValue("i", FALSE);
	if (!pTargetInstance)
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", pMainInstance->IsSameEmpire(*pTargetInstance));
}

ScriptObject* playerIsChallengeInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().IsChallengeInstance(iVID));
}

ScriptObject* playerIsRevengeInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().IsRevengeInstance(iVID));
}

ScriptObject* playerIsCantFightInstance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().IsCantFightInstance(iVID));
}

ScriptObject* playerGetCharacterDistance(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase* pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Script_BuildValue("f", -1.0f);
	if (!pTargetInstance)
		return Script_BuildValue("f", -1.0f);

	return Script_BuildValue("f", pMainInstance->GetDistance(pTargetInstance));
}

ScriptObject* playerIsInSafeArea(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pMainInstance)
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", pMainInstance->IsInSafe());
}

ScriptObject* playerIsMountingHorse(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pMainInstance)
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", pMainInstance->IsMountingHorse());
}

ScriptObject* playerIsObserverMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	return Script_BuildValue("i", rkPlayer.IsObserverMode());
}

ScriptObject* playerActEmotion(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.ActEmotion(iVID);
	return Script_BuildNone();
}

ScriptObject* playerShowPlayer(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (pMainInstance)
		pMainInstance->GetGraphicThingInstanceRef().Show();
	return Script_BuildNone();
}

ScriptObject* playerHidePlayer(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (pMainInstance)
		pMainInstance->GetGraphicThingInstanceRef().Hide();
	return Script_BuildNone();
}

ScriptObject* playerComboAttack(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().NEW_Attack();
	return Script_BuildNone();
}

ScriptObject* playerRegisterEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iEft;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iEft))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &szFileName))
		return Script_BadArgument();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	if (!rkPlayer.RegisterEffect(iEft, szFileName, false))
		return Script_BuildException("CPythonPlayer::RegisterEffect(eEft=%d, szFileName=%s", iEft, szFileName);

	return Script_BuildNone();
}

ScriptObject* playerRegisterCacheEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iEft;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iEft))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &szFileName))
		return Script_BadArgument();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	if (!rkPlayer.RegisterEffect(iEft, szFileName, true))
		return Script_BuildException("CPythonPlayer::RegisterEffect(eEft=%d, szFileName=%s", iEft, szFileName);

	return Script_BuildNone();
}

ScriptObject* playerSetAttackKeyState(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int isPressed;
	if (!ScriptTuple_GetInteger(poArgs, 0, &isPressed))
		return Script_BuildException("playerSetAttackKeyState(isPressed) - There is no first argument");

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetAttackKeyState(isPressed ? true : false);
	return Script_BuildNone();
}

ScriptObject* playerSetSingleDIKKeyState(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eDIK;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eDIK))
		return Script_BuildException("playerSetSingleDIKKeyState(eDIK, isPressed) - There is no first argument");

	int isPressed;
	if (!ScriptTuple_GetInteger(poArgs, 1, &isPressed))
		return Script_BuildException("playerSetSingleDIKKeyState(eDIK, isPressed) - There is no second argument");

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_SetSingleDIKKeyState(eDIK, isPressed ? true : false);
	return Script_BuildNone();
}

ScriptObject* playerEndKeyWalkingImmediately(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().NEW_Stop();
	return Script_BuildNone();
}

ScriptObject* playerStartMouseWalking(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildNone();
}

ScriptObject* playerEndMouseWalking(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildNone();
}

ScriptObject* playerResetCameraRotation(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().NEW_ResetCameraRotation();
	return Script_BuildNone();
}

ScriptObject* playerSetAutoCameraRotationSpeed(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fCmrRotSpd;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fCmrRotSpd))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_SetAutoCameraRotationSpeed(fCmrRotSpd);
	return Script_BuildNone();
}

ScriptObject* playerSetMouseState(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eMBT;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eMBT))
		return Script_BuildException();

	int eMBS;
	if (!ScriptTuple_GetInteger(poArgs, 1, &eMBS))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_SetMouseState(eMBT, eMBS);

	return Script_BuildNone();
}

ScriptObject* playerSetMouseFunc(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eMBT;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eMBT))
		return Script_BuildException();

	int eMBS;
	if (!ScriptTuple_GetInteger(poArgs, 1, &eMBS))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_SetMouseFunc(eMBT, eMBS);

	return Script_BuildNone();
}

ScriptObject* playerGetMouseFunc(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eMBT;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eMBT))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	return Script_BuildValue("i", rkPlayer.NEW_GetMouseFunc(eMBT));
}

ScriptObject* playerSetMouseMiddleButtonState(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eMBS;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eMBS))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_SetMouseMiddleButtonState(eMBS);

	return Script_BuildNone();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptObject* playerGetName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("s", CPythonPlayer::Instance().GetName());
}

ScriptObject* playerGetRace(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().GetRace());
}

ScriptObject* playerGetJob(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int race = CPythonPlayer::Instance().GetRace();
	int job = RaceToJob(race);
	return Script_BuildValue("i", job);
}

ScriptObject* playerGetPlayTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().GetPlayTime());
}

ScriptObject* playerSetPlayTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iTime;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iTime))
		return Script_BuildException();

	CPythonPlayer::Instance().SetPlayTime(iTime);
	return Script_BuildNone();
}

ScriptObject* playerIsSkillCoolTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().IsSkillCoolTime(iSlotIndex));
}

ScriptObject* playerGetSkillCoolTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	float fCoolTime = CPythonPlayer::Instance().GetSkillCoolTime(iSlotIndex);
	float fElapsedCoolTime = CPythonPlayer::Instance().GetSkillElapsedCoolTime(iSlotIndex);
	return Script_BuildValue("ff", fCoolTime, fElapsedCoolTime);
}

ScriptObject* playerIsSkillActive(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().IsSkillActive(iSlotIndex));
}

ScriptObject* playerUseGuildSkill(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSkillSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSkillSlotIndex))
		return Script_BuildException();

	CPythonPlayer::Instance().UseGuildSkill(iSkillSlotIndex);
	return Script_BuildNone();
}

ScriptObject* playerAffectIndexToSkillIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iAffectIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iAffectIndex))
		return Script_BuildException();

	DWORD dwSkillIndex;
	if (!CPythonPlayer::Instance().AffectIndexToSkillIndex(iAffectIndex, &dwSkillIndex))
		return Script_BuildValue("i", 0);

	return Script_BuildValue("i", dwSkillIndex);
}

ScriptObject* playerGetEXP(ScriptObject* poSelf, ScriptObject* poArgs)
{
	DWORD dwEXP = CPythonPlayer::Instance().GetStatus(POINT_EXP);
	return Script_BuildValue("l", dwEXP);
}

ScriptObject* playerGetStatus(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iType))
		return Script_BuildException();

	long iValue = CPythonPlayer::Instance().GetStatus(iType);

	if (POINT_ATT_SPEED == iType)
	{
		CInstanceBase* pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
		if (pInstance && (CItemData::WEAPON_TWO_HANDED == pInstance->GetWeaponType()))
		{
			iValue -= TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;
		}
	}

	return Script_BuildValue("i", iValue);
}

ScriptObject* playerSetStatus(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iType))
		return Script_BuildException();

	int iValue;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iValue))
		return Script_BuildException();

	CPythonPlayer::Instance().SetStatus(iType, iValue);
	return Script_BuildNone();
}

ScriptObject* playerGetElk(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().GetStatus(POINT_GOLD));
}

ScriptObject* playerGetGuildID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pInstance)
		return Script_BuildValue("i", 0);
	return Script_BuildValue("i", pInstance->GetGuildID());
}

ScriptObject* playerGetGuildName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (pInstance)
	{
		DWORD dwID = pInstance->GetGuildID();
		std::string strName;
		if (CPythonGuild::Instance().GetGuildName(dwID, &strName))
			return Script_BuildValue("s", strName.c_str());
	}
	return Script_BuildValue("s", "");
}

ScriptObject* playerGetAlignmentData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CInstanceBase* pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	int iAlignmentPoint = 0;
	int iAlignmentGrade = 4;
	if (pInstance)
	{
		iAlignmentPoint = pInstance->GetAlignment();
		iAlignmentGrade = pInstance->GetAlignmentGrade();
	}
	return Script_BuildValue("ii", iAlignmentPoint, iAlignmentGrade);
}

ScriptObject* playerSetSkill(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	int iSkillIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iSkillIndex))
		return Script_BuildException();

	CPythonPlayer::Instance().SetSkill(iSlotIndex, iSkillIndex);
	return Script_BuildNone();
}

ScriptObject* playerGetSkillIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().GetSkillIndex(iSlotIndex));
}

ScriptObject* playerGetSkillSlotIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSkillIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSkillIndex))
		return Script_BuildException();

	DWORD dwSlotIndex;
	if (!CPythonPlayer::Instance().GetSkillSlotIndex(iSkillIndex, &dwSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("i", dwSlotIndex);
}

ScriptObject* playerGetSkillGrade(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().GetSkillGrade(iSlotIndex));
}

ScriptObject* playerGetSkillLevel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().GetSkillLevel(iSlotIndex));
}

ScriptObject* playerGetSkillCurrentEfficientPercentage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("f", CPythonPlayer::Instance().GetSkillCurrentEfficientPercentage(iSlotIndex));
}
ScriptObject* playerGetSkillNextEfficientPercentage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("f", CPythonPlayer::Instance().GetSkillNextEfficientPercentage(iSlotIndex));
}

ScriptObject* playerClickSkillSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSkillSlot;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSkillSlot))
		return Script_BadArgument();

	CPythonPlayer::Instance().ClickSkillSlot(iSkillSlot);

	return Script_BuildNone();
}

ScriptObject* playerChangeCurrentSkillNumberOnly(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BadArgument();

	CPythonPlayer::Instance().ChangeCurrentSkillNumberOnly(iSlotIndex);

	return Script_BuildNone();
}

ScriptObject* playerClearSkillDict(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().ClearSkillDict();
	return Script_BuildNone();
}

ScriptObject* playerMoveItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos srcCell;
	TItemPos dstCell;
	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &srcCell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &dstCell.cell))
				return Script_BuildException();
			break;
		case 4:
			if (!ScriptTuple_GetByte(poArgs, 0, &srcCell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &srcCell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetByte(poArgs, 2, &dstCell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 3, &dstCell.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}

	CPythonPlayer::Instance().MoveItemData(srcCell, dstCell);
	return Script_BuildNone();
}

ScriptObject* playerSendClickItemPacket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ivid;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ivid))
		return Script_BuildException();

	CPythonPlayer::Instance().SendClickItemPacket(ivid);
	return Script_BuildNone();
}

ScriptObject* playerGetItemIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
		{
			int iSlotIndex;
			if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Script_BuildException();

			int ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, iSlotIndex));
			return Script_BuildValue("i", ItemIndex);
		}
		case 2:
		{
			TItemPos Cell;
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();

			int ItemIndex = CPythonPlayer::Instance().GetItemIndex(Cell);
			return Script_BuildValue("i", ItemIndex);
		}
		default:
			return Script_BuildException();
	}
}

ScriptObject* playerGetItemFlags(ScriptObject* poSelf, ScriptObject* poArgs)
{
	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
		{
			int iSlotIndex;
			if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Script_BuildException();

			DWORD flags = CPythonPlayer::Instance().GetItemFlags(TItemPos(INVENTORY, iSlotIndex));
			return Script_BuildValue("i", flags);
		}
		case 2:
		{
			TItemPos Cell;
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();

			DWORD flags = CPythonPlayer::Instance().GetItemFlags(Cell);
			return Script_BuildValue("i", flags);
		}
		default:
			return Script_BuildException();
	}
}

ScriptObject* playerGetItemCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
		{
			int iSlotIndex;
			if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Script_BuildException();

			int ItemNum = CPythonPlayer::Instance().GetItemCount(TItemPos(INVENTORY, iSlotIndex));
			return Script_BuildValue("i", ItemNum);
		}
		case 2:
		{
			TItemPos Cell;
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();

			int ItemNum = CPythonPlayer::Instance().GetItemCount(Cell);

			return Script_BuildValue("i", ItemNum);
		}
		default:
			return Script_BuildException();
	}
}

ScriptObject* playerSetItemCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
		{
			int iSlotIndex;
			if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Script_BuildException();

			BYTE bCount;
			if (!ScriptTuple_GetInteger(poArgs, 1, &bCount))
				return Script_BuildException();

			if (0 == bCount)
				return Script_BuildException();

			CPythonPlayer::Instance().SetItemCount(TItemPos(INVENTORY, iSlotIndex), bCount);
			return Script_BuildNone();
		}
		case 3:
		{
			TItemPos Cell;
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();

			BYTE bCount;
			if (!ScriptTuple_GetInteger(poArgs, 2, &bCount))
				return Script_BuildException();

			CPythonPlayer::Instance().SetItemCount(Cell, bCount);

			return Script_BuildNone();
		}
		default:
			return Script_BuildException();
	}
}

ScriptObject* playerGetItemCountByVnum(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ivnum;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ivnum))
		return Script_BuildException();

	int ItemNum = CPythonPlayer::Instance().GetItemCountByVnum(ivnum);
	return Script_BuildValue("i", ItemNum);
}

ScriptObject* playerGetItemMetinSocket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;
	int iMetinSocketIndex;

	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
				return Script_BuildException();

			break;
		case 3:
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &iMetinSocketIndex))
				return Script_BuildException();

			break;

		default:
			return Script_BuildException();
	}
	int nMetinSocketValue = CPythonPlayer::Instance().GetItemMetinSocket(Cell, iMetinSocketIndex);
	return Script_BuildValue("i", nMetinSocketValue);
}

ScriptObject* playerGetItemAttribute(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;
	int iAttributeSlotIndex;
	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &iAttributeSlotIndex))
				return Script_BuildException();

			break;
		case 3:
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 2, &iAttributeSlotIndex))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}
	BYTE byType;
	short sValue;
	CPythonPlayer::Instance().GetItemAttribute(Cell, iAttributeSlotIndex, &byType, &sValue);

	return Script_BuildValue("ii", byType, sValue);
}

ScriptObject* playerGetItemLink(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;

	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();
			break;
		case 2:
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}
	const TItemData* pPlayerItem = CPythonPlayer::Instance().GetItemData(Cell);
	CItemData* pItemData = NULL;
	char buf[1024];

	if (pPlayerItem && CItemManager::Instance().GetItemDataPointer(pPlayerItem->vnum, &pItemData))
	{
		char itemlink[256];
		int len;
		bool isAttr = false;

		len = snprintf(itemlink, sizeof(itemlink), "item:%x:%x:%x:%x:%x",
					   pPlayerItem->vnum, pPlayerItem->flags,
					   pPlayerItem->alSockets[0], pPlayerItem->alSockets[1], pPlayerItem->alSockets[2]);

		for (int i = 0; i < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++i)
			if (pPlayerItem->aAttr[i].bType != 0)
			{
				len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%x:%d",
								pPlayerItem->aAttr[i].bType, pPlayerItem->aAttr[i].sValue);
				isAttr = true;
			}

		if (GetDefaultCodePage() == CP_ARABIC)
		{
			if (isAttr)
				// item:item:number:flags:socket0:socket1:socket2"
				snprintf(buf, sizeof(buf), " |h|r[%s]|cffffc700|H%s|h", pItemData->GetName(), itemlink);
			else
				snprintf(buf, sizeof(buf), " |h|r[%s]|cfff1e6c0|H%s|h", pItemData->GetName(), itemlink);
		}
		else
		{
			if (isAttr)
				// item:item:number:flags:socket0:socket1:socket2"
				snprintf(buf, sizeof(buf), "|cffffc700|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
			else
				snprintf(buf, sizeof(buf), "|cfff1e6c0|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
		}
	}
	else
		buf[0] = '\0';

	return Script_BuildValue("s", buf);
}

ScriptObject* playerGetISellItemPrice(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos Cell;

	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
			if (!ScriptTuple_GetInteger(poArgs, 0, &Cell.cell))
				return Script_BuildException();
			break;
		case 2:
			if (!ScriptTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}
	CItemData* pItemData;

	if (!CItemManager::Instance().GetItemDataPointer(CPythonPlayer::Instance().GetItemIndex(Cell), &pItemData))
		return Script_BuildValue("i", 0);

	int iPrice;

	if (pItemData->IsFlag(CItemData::ITEM_FLAG_COUNT_PER_1GOLD))
		iPrice = CPythonPlayer::Instance().GetItemCount(Cell) / pItemData->GetISellItemPrice();
	else
		iPrice = pItemData->GetISellItemPrice() * CPythonPlayer::Instance().GetItemCount(Cell);

	iPrice /= 5;
	return Script_BuildValue("i", iPrice);
}

ScriptObject* playerGetQuickPage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().GetQuickPage());
}

ScriptObject* playerSetQuickPage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPageIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPageIndex))
		return Script_BuildException();

	CPythonPlayer::Instance().SetQuickPage(iPageIndex);
	return Script_BuildNone();
}

ScriptObject* playerLocalQuickSlotIndexToGlobalQuickSlotIndex(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iLocalSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iLocalSlotIndex))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	return Script_BuildValue("i", rkPlayer.LocalQuickSlotIndexToGlobalQuickSlotIndex(iLocalSlotIndex));
}

ScriptObject* playerGetLocalQuickSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	DWORD dwWndType;
	DWORD dwWndItemPos;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.GetLocalQuickSlotData(iSlotIndex, &dwWndType, &dwWndItemPos);

	return Script_BuildValue("ii", dwWndType, dwWndItemPos);
}

ScriptObject* playerGetGlobalQuickSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	DWORD dwWndType;
	DWORD dwWndItemPos;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.GetGlobalQuickSlotData(iSlotIndex, &dwWndType, &dwWndItemPos);

	return Script_BuildValue("ii", dwWndType, dwWndItemPos);
}

ScriptObject* playerRequestAddLocalQuickSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nSlotIndex;
	int nWndType;
	int nWndItemPos;

	if (!ScriptTuple_GetInteger(poArgs, 0, &nSlotIndex))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 1, &nWndType))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 2, &nWndItemPos))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.RequestAddLocalQuickSlot(nSlotIndex, nWndType, nWndItemPos);

	return Script_BuildNone();
}

ScriptObject* playerRequestAddToEmptyLocalQuickSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nWndType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nWndType))
		return Script_BuildException();

	int nWndItemPos;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nWndItemPos))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.RequestAddToEmptyLocalQuickSlot(nWndType, nWndItemPos);

	return Script_BuildNone();
}

ScriptObject* playerRequestDeleteGlobalQuickSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nGlobalSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nGlobalSlotIndex))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.RequestDeleteGlobalQuickSlot(nGlobalSlotIndex);
	return Script_BuildNone();
}

ScriptObject* playerRequestMoveGlobalQuickSlotToLocalQuickSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nGlobalSrcSlotIndex;
	int nLocalDstSlotIndex;

	if (!ScriptTuple_GetInteger(poArgs, 0, &nGlobalSrcSlotIndex))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 1, &nLocalDstSlotIndex))
		return Script_BuildException();

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.RequestMoveGlobalQuickSlotToLocalQuickSlot(nGlobalSrcSlotIndex, nLocalDstSlotIndex);
	return Script_BuildNone();
}

ScriptObject* playerRequestUseLocalQuickSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iLocalPosition;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iLocalPosition))
		return Script_BuildException();

	CPythonPlayer::Instance().RequestUseLocalQuickSlot(iLocalPosition);

	return Script_BuildNone();
}

ScriptObject* playerRemoveQuickSlotByValue(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iType))
		return Script_BuildException();

	int iPosition;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iPosition))
		return Script_BuildException();

	CPythonPlayer::Instance().RemoveQuickSlotByValue(iType, iPosition);

	return Script_BuildNone();
}

ScriptObject* playerisItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	char Flag = CPythonPlayer::Instance().IsItem(TItemPos(INVENTORY, iSlotIndex));

	return Script_BuildValue("i", Flag);
}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
ScriptObject* playerIsBeltInventorySlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	char Flag = CPythonPlayer::Instance().IsBeltInventorySlot(TItemPos(INVENTORY, iSlotIndex));

	return Script_BuildValue("i", Flag);
}
#endif

ScriptObject* playerIsEquipmentSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	if (iSlotIndex >= c_Equipment_Start)
		if (iSlotIndex <= c_DragonSoul_Equip_End)
			return Script_BuildValue("i", 1);

	return Script_BuildValue("i", 0);
}

ScriptObject* playerIsDSEquipmentSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bWindowType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &bWindowType))
		return Script_BuildException();
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iSlotIndex))
		return Script_BuildException();

	if (INVENTORY == bWindowType)
		if (iSlotIndex >= c_DragonSoul_Equip_Start)
			if (iSlotIndex <= c_DragonSoul_Equip_End)
				return Script_BuildValue("i", 1);

	return Script_BuildValue("i", 0);
}

ScriptObject* playerIsCostumeSlot(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

#ifdef ENABLE_COSTUME_SYSTEM
	if (iSlotIndex >= c_Costume_Slot_Start)
		if (iSlotIndex <= c_Costume_Slot_End)
			return Script_BuildValue("i", 1);
#endif

	return Script_BuildValue("i", 0);
}

ScriptObject* playerIsOpenPrivateShop(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().IsOpenPrivateShop());
}

ScriptObject* playerIsValuableItem(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos SlotIndex;

	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
			if (!ScriptTuple_GetInteger(poArgs, 0, &SlotIndex.cell))
				return Script_BuildException();
			break;
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &SlotIndex.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 0, &SlotIndex.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}

	DWORD dwItemIndex = CPythonPlayer::Instance().GetItemIndex(SlotIndex);
	CItemManager::Instance().SelectItemData(dwItemIndex);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find item data");

	BOOL hasMetinSocket = FALSE;
	BOOL isHighPrice = FALSE;

	for (int i = 0; i < METIN_SOCKET_COUNT; ++i)
		if (CPythonPlayer::METIN_SOCKET_TYPE_NONE != CPythonPlayer::Instance().GetItemMetinSocket(SlotIndex, i))
			hasMetinSocket = TRUE;

	DWORD dwValue = pItemData->GetISellItemPrice();
	if (dwValue > 5000)
		isHighPrice = TRUE;

	return Script_BuildValue("i", hasMetinSocket || isHighPrice);
}

int GetItemGrade(const char* c_szItemName)
{
	std::string strName = c_szItemName;
	if (strName.empty())
		return 0;

	char chGrade = strName[strName.length() - 1];
	if (chGrade < '0' || chGrade > '9')
		chGrade = '0';

	int iGrade = chGrade - '0';
	return iGrade;
}

ScriptObject* playerGetItemGrade(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos SlotIndex;
	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
			if (!ScriptTuple_GetInteger(poArgs, 0, &SlotIndex.cell))
				return Script_BuildException();
			break;
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &SlotIndex.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &SlotIndex.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}

	int iItemIndex = CPythonPlayer::Instance().GetItemIndex(SlotIndex);
	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Script_BuildException("Can't find item data");

	return Script_BuildValue("i", GetItemGrade(pItemData->GetName()));
}

#if defined(GAIDEN)
ScriptObject* playerGetItemUnbindTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	return Script_BuildValue("i", (int)CPythonPlayer::instance().GetItemUnbindTime(iSlotIndex));
}
#endif

enum
{
	REFINE_SCROLL_TYPE_MAKE_SOCKET = 1,
	REFINE_SCROLL_TYPE_UP_GRADE = 2,
};

enum
{
	REFINE_CANT,
	REFINE_OK,
	REFINE_ALREADY_MAX_SOCKET_COUNT,
	REFINE_NEED_MORE_GOOD_SCROLL,
	REFINE_CANT_MAKE_SOCKET_ITEM,
	REFINE_NOT_NEXT_GRADE_ITEM,
	REFINE_CANT_REFINE_METIN_TO_EQUIPMENT,
	REFINE_CANT_REFINE_ROD,
};

ScriptObject* playerCanRefine(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iScrollItemIndex;
	TItemPos TargetSlotIndex;

	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
				return Script_BadArgument();
			break;
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
				return Script_BadArgument();
			break;
		default:
			return Script_BadArgument();
	}

	if (CPythonPlayer::Instance().IsEquipmentSlot(TargetSlotIndex))
	{
		return Script_BuildValue("i", REFINE_CANT_REFINE_METIN_TO_EQUIPMENT);
	}

	// Scroll
	CItemManager::Instance().SelectItemData(iScrollItemIndex);
	CItemData* pScrollItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pScrollItemData)
		return Script_BuildValue("i", REFINE_CANT);
	int iScrollType = pScrollItemData->GetType();
	int iScrollSubType = pScrollItemData->GetSubType();
	if (iScrollType != CItemData::ITEM_TYPE_USE)
		return Script_BuildValue("i", REFINE_CANT);
	if (iScrollSubType != CItemData::USE_TUNING)
		return Script_BuildValue("i", REFINE_CANT);

	// Target Item
	int iTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemManager::Instance().SelectItemData(iTargetItemIndex);
	CItemData* pTargetItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pTargetItemData)
		return Script_BuildValue("i", REFINE_CANT);
	int iTargetType = pTargetItemData->GetType();
	// int iTargetSubType = pTargetItemData->GetSubType();
	if (CItemData::ITEM_TYPE_ROD == iTargetType)
		return Script_BuildValue("i", REFINE_CANT_REFINE_ROD);

	if (pTargetItemData->HasNextGrade())
	{
		return Script_BuildValue("i", REFINE_OK);
	}
	else
	{
		return Script_BuildValue("i", REFINE_NOT_NEXT_GRADE_ITEM);
	}

	return Script_BuildValue("i", REFINE_CANT);
}

enum
{
	ATTACH_METIN_CANT,
	ATTACH_METIN_OK,
	ATTACH_METIN_NOT_MATCHABLE_ITEM,
	ATTACH_METIN_NO_MATCHABLE_SOCKET,
	ATTACH_METIN_NOT_EXIST_GOLD_SOCKET,
	ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT,
};

ScriptObject* playerCanAttachMetin(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMetinItemID;
	TItemPos TargetSlotIndex;

	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iMetinItemID))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
				return Script_BuildException();
			break;
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iMetinItemID))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}
	if (CPythonPlayer::Instance().IsEquipmentSlot(TargetSlotIndex))
	{
		return Script_BuildValue("i", ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT);
	}

	CItemData* pMetinItemData;
	if (!CItemManager::Instance().GetItemDataPointer(iMetinItemID, &pMetinItemData))
		return Script_BuildException("can't find item data");

	DWORD dwTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemData* pTargetItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwTargetItemIndex, &pTargetItemData))
		return Script_BuildException("can't find item data");

	DWORD dwMetinWearFlags = pMetinItemData->GetWearFlags();
	DWORD dwTargetWearFlags = pTargetItemData->GetWearFlags();
	if (0 == (dwMetinWearFlags & dwTargetWearFlags))
		return Script_BuildValue("i", ATTACH_METIN_NOT_MATCHABLE_ITEM);
	if (CItemData::ITEM_TYPE_ROD == pTargetItemData->GetType())
		return Script_BuildValue("i", ATTACH_METIN_CANT);

	BOOL bNotExistGoldSocket = FALSE;

	int iSubType = pMetinItemData->GetSubType();
	for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
	{
		DWORD dwSocketType = CPythonPlayer::Instance().GetItemMetinSocket(TargetSlotIndex, i);
		if (CItemData::METIN_NORMAL == iSubType)
		{
			if (CPythonPlayer::METIN_SOCKET_TYPE_SILVER == dwSocketType ||
				CPythonPlayer::METIN_SOCKET_TYPE_GOLD == dwSocketType)
			{
				return Script_BuildValue("i", ATTACH_METIN_OK);
			}
		}
		else if (CItemData::METIN_GOLD == iSubType)
		{
			if (CPythonPlayer::METIN_SOCKET_TYPE_GOLD == dwSocketType)
			{
				return Script_BuildValue("i", ATTACH_METIN_OK);
			}
			else if (CPythonPlayer::METIN_SOCKET_TYPE_SILVER == dwSocketType)
			{
				bNotExistGoldSocket = TRUE;
			}
		}
	}

	if (bNotExistGoldSocket)
	{
		return Script_BuildValue("i", ATTACH_METIN_NOT_EXIST_GOLD_SOCKET);
	}

	return Script_BuildValue("i", ATTACH_METIN_NO_MATCHABLE_SOCKET);
}

enum
{
	DETACH_METIN_CANT,
	DETACH_METIN_OK,
};

ScriptObject* playerCanDetach(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iScrollItemIndex;
	TItemPos TargetSlotIndex;
	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
				return Script_BadArgument();
			break;
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
				return Script_BadArgument();
			break;
		default:
			return Script_BadArgument();
	}

	// Scroll
	CItemManager::Instance().SelectItemData(iScrollItemIndex);
	CItemData* pScrollItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pScrollItemData)
		return Script_BuildException("Can't find item data");
	int iScrollType = pScrollItemData->GetType();
	int iScrollSubType = pScrollItemData->GetSubType();
	if (iScrollType != CItemData::ITEM_TYPE_USE)
		return Script_BuildValue("i", DETACH_METIN_CANT);
	if (iScrollSubType != CItemData::USE_DETACHMENT)
		return Script_BuildValue("i", DETACH_METIN_CANT);

	// Target Item
	int iTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemManager::Instance().SelectItemData(iTargetItemIndex);
	CItemData* pTargetItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pTargetItemData)
		return Script_BuildException("Can't find item data");
	// int iTargetType = pTargetItemData->GetType();
	// int iTargetSubType = pTargetItemData->GetSubType();

	if (pTargetItemData->IsFlag(CItemData::ITEM_FLAG_REFINEABLE))
	{
		for (int iSlotCount = 0; iSlotCount < METIN_SOCKET_COUNT; ++iSlotCount)
			if (CPythonPlayer::Instance().GetItemMetinSocket(TargetSlotIndex, iSlotCount) > 2)
			{
				return Script_BuildValue("i", DETACH_METIN_OK);
			}
	}

	return Script_BuildValue("i", DETACH_METIN_CANT);
}

ScriptObject* playerCanUnlock(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iKeyItemIndex;
	TItemPos TargetSlotIndex;
	switch (ScriptTuple_Size(poArgs))
	{
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iKeyItemIndex))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
				return Script_BadArgument();
			break;
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &iKeyItemIndex))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
				return Script_BadArgument();
			if (!ScriptTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
				return Script_BadArgument();
			break;
		default:
			return Script_BadArgument();
	}

	// Key
	CItemManager::Instance().SelectItemData(iKeyItemIndex);
	CItemData* pKeyItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pKeyItemData)
		return Script_BuildException("Can't find item data");
	int iKeyType = pKeyItemData->GetType();
	if (iKeyType != CItemData::ITEM_TYPE_TREASURE_KEY)
		return Script_BuildValue("i", FALSE);

	// Target Item
	int iTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemManager::Instance().SelectItemData(iTargetItemIndex);
	CItemData* pTargetItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pTargetItemData)
		return Script_BuildException("Can't find item data");
	int iTargetType = pTargetItemData->GetType();
	if (iTargetType != CItemData::ITEM_TYPE_TREASURE_BOX)
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", TRUE);
}

ScriptObject* playerIsRefineGradeScroll(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos ScrollSlotIndex;
	switch (ScriptTuple_Size(poArgs))
	{
		case 1:
			if (!ScriptTuple_GetInteger(poArgs, 0, &ScrollSlotIndex.cell))
				return Script_BuildException();
			break;
		case 2:
			if (!ScriptTuple_GetInteger(poArgs, 0, &ScrollSlotIndex.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &ScrollSlotIndex.cell))
				return Script_BuildException();
			break;
		default:
			return Script_BuildException();
	}

	int iScrollItemIndex = CPythonPlayer::Instance().GetItemIndex(ScrollSlotIndex);
	CItemManager::Instance().SelectItemData(iScrollItemIndex);
	CItemData* pScrollItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pScrollItemData)
		return Script_BuildException("Can't find item data");

	return Script_BuildValue("i", REFINE_SCROLL_TYPE_UP_GRADE == pScrollItemData->GetValue(0));
}

ScriptObject* playerUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().Update();
	return Script_BuildNone();
}

ScriptObject* playerRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildNone();
}

ScriptObject* playerClear(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().Clear();
	return Script_BuildNone();
}

ScriptObject* playerClearTarget(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().SetTarget(0);
	return Script_BuildNone();
}

ScriptObject* playerSetTarget(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CPythonPlayer::Instance().SetTarget(iVID);
	return Script_BuildNone();
}

ScriptObject* playerOpenCharacterMenu(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	CPythonPlayer::Instance().OpenCharacterMenu(iVID);
	return Script_BuildNone();
}

ScriptObject* playerIsPartyMember(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonPlayer::Instance().IsPartyMemberByVID(iVID));
}

ScriptObject* playerIsPartyLeader(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVID))
		return Script_BuildException();

	DWORD dwPID;
	if (!CPythonPlayer::Instance().PartyMemberVIDToPID(iVID, &dwPID))
		return Script_BuildValue("i", FALSE);

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(dwPID, &pPartyMemberInfo))
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", CPythonPlayer::PARTY_ROLE_LEADER == pPartyMemberInfo->byState);
}

ScriptObject* playerIsPartyLeaderByPID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPID))
		return Script_BuildException();

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", CPythonPlayer::PARTY_ROLE_LEADER == pPartyMemberInfo->byState);
}

ScriptObject* playerGetPartyMemberHPPercentage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPID))
		return Script_BuildException();

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", pPartyMemberInfo->byHPPercentage);
}

ScriptObject* playerGetPartyMemberState(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPID))
		return Script_BuildException();

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("i", pPartyMemberInfo->byState);
}

ScriptObject* playerGetPartyMemberAffects(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPID))
		return Script_BuildException();

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Script_BuildValue("i", FALSE);

	return Script_BuildValue("iiiiiii", pPartyMemberInfo->sAffects[0],
							 pPartyMemberInfo->sAffects[1],
							 pPartyMemberInfo->sAffects[2],
							 pPartyMemberInfo->sAffects[3],
							 pPartyMemberInfo->sAffects[4],
							 pPartyMemberInfo->sAffects[5],
							 pPartyMemberInfo->sAffects[6]);
}

ScriptObject* playerRemovePartyMember(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPID))
		return Script_BuildException();

	CPythonPlayer::Instance().RemovePartyMember(iPID);
	return Script_BuildNone();
}

ScriptObject* playerExitParty(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer::Instance().ExitParty();
	return Script_BuildNone();
}

ScriptObject* playerGetPKMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().GetPKMode());
}

ScriptObject* playerSetWeaponAttackBonusFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();

	return Script_BuildNone();
}

ScriptObject* playerToggleCoolTime(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().__ToggleCoolTime());
}

ScriptObject* playerToggleLevelLimit(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().__ToggleLevelLimit());
}

ScriptObject* playerGetTargetVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonPlayer::Instance().GetTargetVID());
}

ScriptObject* playerRegisterEmotionIcon(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &szFileName))
		return Script_BuildException();

	CGraphicImage* pImage = (CGraphicImage*)CResourceManager::Instance().GetResourcePointer(szFileName);
	m_kMap_iEmotionIndex_pkIconImage.insert(std::make_pair(iIndex, pImage));

	return Script_BuildNone();
}

ScriptObject* playerGetEmotionIconImage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	if (m_kMap_iEmotionIndex_pkIconImage.end() == m_kMap_iEmotionIndex_pkIconImage.find(iIndex))
		return Script_BuildHandle(NULL);

	return Script_BuildHandle(m_kMap_iEmotionIndex_pkIconImage[iIndex]);
}

ScriptObject* playerSetItemData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();

	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iVirtualID))
		return Script_BuildException();

	int iNum;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iNum))
		return Script_BuildException();

	TItemData kItemInst;
	ZeroMemory(&kItemInst, sizeof(kItemInst));
	kItemInst.vnum = iVirtualID;
	kItemInst.count = iNum;
	CPythonPlayer::Instance().SetItemData(TItemPos(INVENTORY, iSlotIndex), kItemInst);
	return Script_BuildNone();
}

ScriptObject* playerSetItemMetinSocket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos ItemPos;
	int iMetinSocketNumber;
	int iNum;

	switch (ScriptTuple_Size(poArgs))
	{
		case 3:
			if (!ScriptTuple_GetInteger(poArgs, 0, &ItemPos.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &iMetinSocketNumber))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 2, &iNum))
				return Script_BuildException();

			break;
		case 4:
			if (!ScriptTuple_GetInteger(poArgs, 0, &ItemPos.window_type))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 1, &ItemPos.cell))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 2, &iMetinSocketNumber))
				return Script_BuildException();
			if (!ScriptTuple_GetInteger(poArgs, 3, &iNum))
				return Script_BuildException();

			break;
		default:
			return Script_BuildException();
	}

	CPythonPlayer::Instance().SetItemMetinSocket(ItemPos, iMetinSocketNumber, iNum);
	return Script_BuildNone();
}

ScriptObject* playerSetItemAttribute(ScriptObject* poSelf, ScriptObject* poArgs)
{
	TItemPos ItemPos;
	int iAttributeSlotIndex;
	int iAttributeType;
	int iAttributeValue;

	switch (ScriptTuple_Size(poArgs))
	{
		case 4:
			if (!ScriptTuple_GetInteger(poArgs, 0, &ItemPos.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &iAttributeSlotIndex))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 2, &iAttributeType))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 3, &iAttributeValue))
				return Script_BuildException();
			break;
		case 5:
			if (!ScriptTuple_GetInteger(poArgs, 0, &ItemPos.window_type))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 1, &ItemPos.cell))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 2, &iAttributeSlotIndex))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 3, &iAttributeType))
				return Script_BuildException();

			if (!ScriptTuple_GetInteger(poArgs, 4, &iAttributeValue))
				return Script_BuildException();
			break;
	}
	CPythonPlayer::Instance().SetItemAttribute(ItemPos, iAttributeSlotIndex, iAttributeType, iAttributeValue);
	return Script_BuildNone();
}

ScriptObject* playerSetAutoPotionInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int potionType = 0;
	if (!ScriptTuple_GetInteger(poArgs, 0, &potionType))
		return Script_BadArgument();

	CPythonPlayer* player = CPythonPlayer::InstancePtr();

	CPythonPlayer::SAutoPotionInfo& potionInfo = player->GetAutoPotionInfo(potionType);

	if (!ScriptTuple_GetBoolean(poArgs, 1, &potionInfo.bActivated))
		return Script_BadArgument();

	if (!ScriptTuple_GetLong(poArgs, 2, &potionInfo.currentAmount))
		return Script_BadArgument();

	if (!ScriptTuple_GetLong(poArgs, 3, &potionInfo.totalAmount))
		return Script_BadArgument();

	if (!ScriptTuple_GetLong(poArgs, 4, &potionInfo.inventorySlotIndex))
		return Script_BadArgument();

	return Script_BuildNone();
}

ScriptObject* playerGetAutoPotionInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonPlayer* player = CPythonPlayer::InstancePtr();

	int potionType = 0;
	if (!ScriptTuple_GetInteger(poArgs, 0, &potionType))
		return Script_BadArgument();

	CPythonPlayer::SAutoPotionInfo& potionInfo = player->GetAutoPotionInfo(potionType);

	return Script_BuildValue("biii", potionInfo.bActivated, int(potionInfo.currentAmount), int(potionInfo.totalAmount), int(potionInfo.inventorySlotIndex));
}

ScriptObject* playerSlotTypeToInvenType(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int slotType = 0;
	if (!ScriptTuple_GetInteger(poArgs, 0, &slotType))
		return Script_BadArgument();

	return Script_BuildValue("i", SlotTypeToInvenType((BYTE)slotType));
}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
// 플레이어가 벨트를 착용 중인지?
ScriptObject* playerIsEquippingBelt(ScriptObject* poSelf, ScriptObject* poArgs)
{
	const CPythonPlayer* player = CPythonPlayer::InstancePtr();
	bool bEquipping = false;

	const TItemData* data = player->GetItemData(TItemPos(EQUIPMENT, c_Equipment_Belt));

	if (NULL != data)
		bEquipping = 0 < data->count;

	return Script_BuildValue("b", bEquipping);
}

// 검사하려는 벨트 인벤토리 Cell이 사용 가능한 칸인지? (사용가능 여부는 착용 중인 벨트의 강화 정도에 따라 달라짐)
ScriptObject* playerIsAvailableBeltInventoryCell(ScriptObject* poSelf, ScriptObject* poArgs)
{
	const CPythonPlayer* player = CPythonPlayer::InstancePtr();
	const TItemData* pData = player->GetItemData(TItemPos(EQUIPMENT, c_Equipment_Belt));

	if (NULL == pData || 0 == pData->count)
		return Script_BuildValue("b", false);

	CItemManager::Instance().SelectItemData(pData->vnum);
	CItemData* pItem = CItemManager::Instance().GetSelectedItemDataPointer();

	long beltGrade = pItem->GetValue(0);

	int pos = 0;
	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BadArgument();

	// return Script_BuildValue("b", CBeltInventoryHelper::IsAvailableCell(pos - c_Belt_Inventory_Slot_Start, GetItemGrade(pItem->GetName())));
	return Script_BuildValue("b", CBeltInventoryHelper::IsAvailableCell(pos - c_Belt_Inventory_Slot_Start, beltGrade));
}
#endif

// 용혼석 강화
ScriptObject* playerSendDragonSoulRefine(ScriptObject* poSelf, ScriptObject* poArgs)
{
	BYTE bSubHeader;
	ScriptObject* pDic;
	TItemPos RefineItemPoses[DS_REFINE_WINDOW_MAX_NUM];
	if (!ScriptTuple_GetByte(poArgs, 0, &bSubHeader))
		return Script_BuildException();
	switch (bSubHeader)
	{
		case DS_SUB_HEADER_CLOSE:
			break;
		case DS_SUB_HEADER_DO_UPGRADE:
		case DS_SUB_HEADER_DO_IMPROVEMENT:
		case DS_SUB_HEADER_DO_REFINE:
		{
			if (!ScriptTuple_GetObject(poArgs, 1, &pDic))
				return Script_BuildException();
			long pos = 0;
			ScriptObject *key, *value;

			while (ScriptDict_Next(pDic, &pos, &key, &value))
			{
				int i = Script_AsLongLong(key);
				if (i > DS_REFINE_WINDOW_MAX_NUM)
					return Script_BuildException();

				if (!ScriptTuple_GetByte(value, 0, &RefineItemPoses[i].window_type) || !ScriptTuple_GetInteger(value, 1, &RefineItemPoses[i].cell))
				{
					return Script_BuildException();
				}
			}
		}
		break;
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendDragonSoulRefinePacket(bSubHeader, RefineItemPoses);

	return Script_BuildNone();
}
void initPlayer()
{
	static ScriptMethodDef s_methods[] =
		{
			{"GetAutoPotionInfo", playerGetAutoPotionInfo, SCRIPT_VARARGS, NULL},
			{"SetAutoPotionInfo", playerSetAutoPotionInfo, SCRIPT_VARARGS, NULL},

			{"PickCloseItem", playerPickCloseItem, SCRIPT_VARARGS, NULL},
			{"SetGameWindow", playerSetGameWindow, SCRIPT_VARARGS, NULL},
			{"RegisterEffect", playerRegisterEffect, SCRIPT_VARARGS, NULL},
			{"RegisterCacheEffect", playerRegisterCacheEffect, SCRIPT_VARARGS, NULL},
			{"SetMouseState", playerSetMouseState, SCRIPT_VARARGS, NULL},
			{"SetMouseFunc", playerSetMouseFunc, SCRIPT_VARARGS, NULL},
			{"GetMouseFunc", playerGetMouseFunc, SCRIPT_VARARGS, NULL},
			{"SetMouseMiddleButtonState", playerSetMouseMiddleButtonState, SCRIPT_VARARGS, NULL},
			{"SetMainCharacterIndex", playerSetMainCharacterIndex, SCRIPT_VARARGS, NULL},
			{"GetMainCharacterIndex", playerGetMainCharacterIndex, SCRIPT_VARARGS, NULL},
			{"GetMainCharacterName", playerGetMainCharacterName, SCRIPT_VARARGS, NULL},
			{"GetMainCharacterPosition", playerGetMainCharacterPosition, SCRIPT_VARARGS, NULL},
			{"IsMainCharacterIndex", playerIsMainCharacterIndex, SCRIPT_VARARGS, NULL},
			{"CanAttackInstance", playerCanAttackInstance, SCRIPT_VARARGS, NULL},
			{"IsActingEmotion", playerIsActingEmotion, SCRIPT_VARARGS, NULL},
			{"IsPVPInstance", playerIsPVPInstance, SCRIPT_VARARGS, NULL},
			{"IsSameEmpire", playerIsSameEmpire, SCRIPT_VARARGS, NULL},
			{"IsChallengeInstance", playerIsChallengeInstance, SCRIPT_VARARGS, NULL},
			{"IsRevengeInstance", playerIsRevengeInstance, SCRIPT_VARARGS, NULL},
			{"IsCantFightInstance", playerIsCantFightInstance, SCRIPT_VARARGS, NULL},
			{"GetCharacterDistance", playerGetCharacterDistance, SCRIPT_VARARGS, NULL},
			{"IsInSafeArea", playerIsInSafeArea, SCRIPT_VARARGS, NULL},
			{"IsMountingHorse", playerIsMountingHorse, SCRIPT_VARARGS, NULL},
			{"IsObserverMode", playerIsObserverMode, SCRIPT_VARARGS, NULL},
			{"ActEmotion", playerActEmotion, SCRIPT_VARARGS, NULL},

			{"ShowPlayer", playerShowPlayer, SCRIPT_VARARGS, NULL},
			{"HidePlayer", playerHidePlayer, SCRIPT_VARARGS, NULL},

			{"ComboAttack", playerComboAttack, SCRIPT_VARARGS, NULL},

			{"SetAutoCameraRotationSpeed", playerSetAutoCameraRotationSpeed, SCRIPT_VARARGS, NULL},
			{"SetAttackKeyState", playerSetAttackKeyState, SCRIPT_VARARGS, NULL},
			{"SetSingleDIKKeyState", playerSetSingleDIKKeyState, SCRIPT_VARARGS, NULL},
			{"EndKeyWalkingImmediately", playerEndKeyWalkingImmediately, SCRIPT_VARARGS, NULL},
			{"StartMouseWalking", playerStartMouseWalking, SCRIPT_VARARGS, NULL},
			{"EndMouseWalking", playerEndMouseWalking, SCRIPT_VARARGS, NULL},
			{"ResetCameraRotation", playerResetCameraRotation, SCRIPT_VARARGS, NULL},
			{"SetQuickCameraMode", playerSetQuickCameraMode, SCRIPT_VARARGS, NULL},

			///////////////////////////////////////////////////////////////////////////////////////////

			{"SetSkill", playerSetSkill, SCRIPT_VARARGS, NULL},
			{"GetSkillIndex", playerGetSkillIndex, SCRIPT_VARARGS, NULL},
			{"GetSkillSlotIndex", playerGetSkillSlotIndex, SCRIPT_VARARGS, NULL},
			{"GetSkillGrade", playerGetSkillGrade, SCRIPT_VARARGS, NULL},
			{"GetSkillLevel", playerGetSkillLevel, SCRIPT_VARARGS, NULL},
			{"GetSkillCurrentEfficientPercentage", playerGetSkillCurrentEfficientPercentage, SCRIPT_VARARGS, NULL},
			{"GetSkillNextEfficientPercentage", playerGetSkillNextEfficientPercentage, SCRIPT_VARARGS, NULL},
			{"ClickSkillSlot", playerClickSkillSlot, SCRIPT_VARARGS, NULL},
			{"ChangeCurrentSkillNumberOnly", playerChangeCurrentSkillNumberOnly, SCRIPT_VARARGS, NULL},
			{"ClearSkillDict", playerClearSkillDict, SCRIPT_VARARGS, NULL},

			{"GetItemIndex", playerGetItemIndex, SCRIPT_VARARGS, NULL},
			{"GetItemFlags", playerGetItemFlags, SCRIPT_VARARGS, NULL},
			{"GetItemCount", playerGetItemCount, SCRIPT_VARARGS, NULL},
			{"GetItemCountByVnum", playerGetItemCountByVnum, SCRIPT_VARARGS, NULL},
			{"GetItemMetinSocket", playerGetItemMetinSocket, SCRIPT_VARARGS, NULL},
			{"GetItemAttribute", playerGetItemAttribute, SCRIPT_VARARGS, NULL},
#if defined(GAIDEN)
			{"GetItemUnbindTime", playerGetItemUnbindTime, SCRIPT_VARARGS, NULL},
#endif
			{"GetISellItemPrice", playerGetISellItemPrice, SCRIPT_VARARGS, NULL},
			{"MoveItem", playerMoveItem, SCRIPT_VARARGS, NULL},
			{"SendClickItemPacket", playerSendClickItemPacket, SCRIPT_VARARGS, NULL},

			///////////////////////////////////////////////////////////////////////////////////////////

			{"GetName", playerGetName, SCRIPT_VARARGS, NULL},
			{"GetJob", playerGetJob, SCRIPT_VARARGS, NULL},
			{"GetRace", playerGetRace, SCRIPT_VARARGS, NULL},
			{"GetPlayTime", playerGetPlayTime, SCRIPT_VARARGS, NULL},
			{"SetPlayTime", playerSetPlayTime, SCRIPT_VARARGS, NULL},

			{"IsSkillCoolTime", playerIsSkillCoolTime, SCRIPT_VARARGS, NULL},
			{"GetSkillCoolTime", playerGetSkillCoolTime, SCRIPT_VARARGS, NULL},
			{"IsSkillActive", playerIsSkillActive, SCRIPT_VARARGS, NULL},
			{"UseGuildSkill", playerUseGuildSkill, SCRIPT_VARARGS, NULL},
			{"AffectIndexToSkillIndex", playerAffectIndexToSkillIndex, SCRIPT_VARARGS, NULL},
			{"GetEXP", playerGetEXP, SCRIPT_VARARGS, NULL},
			{"GetStatus", playerGetStatus, SCRIPT_VARARGS, NULL},
			{"SetStatus", playerSetStatus, SCRIPT_VARARGS, NULL},
			{"GetElk", playerGetElk, SCRIPT_VARARGS, NULL},
			{"GetMoney", playerGetElk, SCRIPT_VARARGS, NULL},
			{"GetGuildID", playerGetGuildID, SCRIPT_VARARGS, NULL},
			{"GetGuildName", playerGetGuildName, SCRIPT_VARARGS, NULL},
			{"GetAlignmentData", playerGetAlignmentData, SCRIPT_VARARGS, NULL},
			{"RequestAddLocalQuickSlot", playerRequestAddLocalQuickSlot, SCRIPT_VARARGS, NULL},
			{"RequestAddToEmptyLocalQuickSlot", playerRequestAddToEmptyLocalQuickSlot, SCRIPT_VARARGS, NULL},
			{"RequestDeleteGlobalQuickSlot", playerRequestDeleteGlobalQuickSlot, SCRIPT_VARARGS, NULL},
			{"RequestMoveGlobalQuickSlotToLocalQuickSlot", playerRequestMoveGlobalQuickSlotToLocalQuickSlot, SCRIPT_VARARGS, NULL},
			{"RequestUseLocalQuickSlot", playerRequestUseLocalQuickSlot, SCRIPT_VARARGS, NULL},
			{"LocalQuickSlotIndexToGlobalQuickSlotIndex", playerLocalQuickSlotIndexToGlobalQuickSlotIndex, SCRIPT_VARARGS, NULL},

			{"GetQuickPage", playerGetQuickPage, SCRIPT_VARARGS, NULL},
			{"SetQuickPage", playerSetQuickPage, SCRIPT_VARARGS, NULL},
			{"GetLocalQuickSlot", playerGetLocalQuickSlot, SCRIPT_VARARGS, NULL},
			{"GetGlobalQuickSlot", playerGetGlobalQuickSlot, SCRIPT_VARARGS, NULL},
			{"RemoveQuickSlotByValue", playerRemoveQuickSlotByValue, SCRIPT_VARARGS, NULL},

			{"isItem", playerisItem, SCRIPT_VARARGS, NULL},
			{"IsEquipmentSlot", playerIsEquipmentSlot, SCRIPT_VARARGS, NULL},
			{"IsDSEquipmentSlot", playerIsDSEquipmentSlot, SCRIPT_VARARGS, NULL},
			{"IsCostumeSlot", playerIsCostumeSlot, SCRIPT_VARARGS, NULL},
			{"IsValuableItem", playerIsValuableItem, SCRIPT_VARARGS, NULL},
			{"IsOpenPrivateShop", playerIsOpenPrivateShop, SCRIPT_VARARGS, NULL},

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
			{"IsBeltInventorySlot", playerIsBeltInventorySlot, SCRIPT_VARARGS, NULL},
			{"IsEquippingBelt", playerIsEquippingBelt, SCRIPT_VARARGS, NULL},
			{"IsAvailableBeltInventoryCell", playerIsAvailableBeltInventoryCell, SCRIPT_VARARGS, NULL},
#endif

			// Refine
			{"GetItemGrade", playerGetItemGrade, SCRIPT_VARARGS, NULL},
			{"CanRefine", playerCanRefine, SCRIPT_VARARGS, NULL},
			{"CanDetach", playerCanDetach, SCRIPT_VARARGS, NULL},
			{"CanUnlock", playerCanUnlock, SCRIPT_VARARGS, NULL},
			{"CanAttachMetin", playerCanAttachMetin, SCRIPT_VARARGS, NULL},
			{"IsRefineGradeScroll", playerIsRefineGradeScroll, SCRIPT_VARARGS, NULL},

			{"ClearTarget", playerClearTarget, SCRIPT_VARARGS, NULL},
			{"SetTarget", playerSetTarget, SCRIPT_VARARGS, NULL},
			{"OpenCharacterMenu", playerOpenCharacterMenu, SCRIPT_VARARGS, NULL},

			{"Update", playerUpdate, SCRIPT_VARARGS, NULL},
			{"Render", playerRender, SCRIPT_VARARGS, NULL},
			{"Clear", playerClear, SCRIPT_VARARGS, NULL},

			// Party
			{"IsPartyMember", playerIsPartyMember, SCRIPT_VARARGS, NULL},
			{"IsPartyLeader", playerIsPartyLeader, SCRIPT_VARARGS, NULL},
			{"IsPartyLeaderByPID", playerIsPartyLeaderByPID, SCRIPT_VARARGS, NULL},
			{"GetPartyMemberHPPercentage", playerGetPartyMemberHPPercentage, SCRIPT_VARARGS, NULL},
			{"GetPartyMemberState", playerGetPartyMemberState, SCRIPT_VARARGS, NULL},
			{"GetPartyMemberAffects", playerGetPartyMemberAffects, SCRIPT_VARARGS, NULL},
			{"RemovePartyMember", playerRemovePartyMember, SCRIPT_VARARGS, NULL},
			{"ExitParty", playerExitParty, SCRIPT_VARARGS, NULL},

			// PK Mode
			{"GetPKMode", playerGetPKMode, SCRIPT_VARARGS, NULL},

			// Emotion
			{"RegisterEmotionIcon", playerRegisterEmotionIcon, SCRIPT_VARARGS, NULL},
			{"GetEmotionIconImage", playerGetEmotionIconImage, SCRIPT_VARARGS, NULL},

			// For System
			{"SetWeaponAttackBonusFlag", playerSetWeaponAttackBonusFlag, SCRIPT_VARARGS, NULL},
			{"ToggleCoolTime", playerToggleCoolTime, SCRIPT_VARARGS, NULL},
			{"ToggleLevelLimit", playerToggleLevelLimit, SCRIPT_VARARGS, NULL},
			{"GetTargetVID", playerGetTargetVID, SCRIPT_VARARGS, NULL},

			{"SetItemData", playerSetItemData, SCRIPT_VARARGS, NULL},
			{"SetItemMetinSocket", playerSetItemMetinSocket, SCRIPT_VARARGS, NULL},
			{"SetItemAttribute", playerSetItemAttribute, SCRIPT_VARARGS, NULL},
			{"SetItemCount", playerSetItemCount, SCRIPT_VARARGS, NULL},

			{"GetItemLink", playerGetItemLink, SCRIPT_VARARGS, NULL},
			{"SlotTypeToInvenType", playerSlotTypeToInvenType, SCRIPT_VARARGS, NULL},
			{"SendDragonSoulRefine", playerSendDragonSoulRefine, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("player", s_methods);
	ScriptModule_AddIntConstant(poModule, "LEVEL", POINT_LEVEL);
	ScriptModule_AddIntConstant(poModule, "VOICE", POINT_VOICE);
	ScriptModule_AddIntConstant(poModule, "EXP", POINT_EXP);
	ScriptModule_AddIntConstant(poModule, "NEXT_EXP", POINT_NEXT_EXP);
	ScriptModule_AddIntConstant(poModule, "HP", POINT_HP);
	ScriptModule_AddIntConstant(poModule, "MAX_HP", POINT_MAX_HP);
	ScriptModule_AddIntConstant(poModule, "SP", POINT_SP);
	ScriptModule_AddIntConstant(poModule, "MAX_SP", POINT_MAX_SP);
	ScriptModule_AddIntConstant(poModule, "STAMINA", POINT_STAMINA);
	ScriptModule_AddIntConstant(poModule, "MAX_STAMINA", POINT_MAX_STAMINA);
	ScriptModule_AddIntConstant(poModule, "ELK", POINT_GOLD);
	ScriptModule_AddIntConstant(poModule, "ST", POINT_ST);
	ScriptModule_AddIntConstant(poModule, "HT", POINT_HT);
	ScriptModule_AddIntConstant(poModule, "DX", POINT_DX);
	ScriptModule_AddIntConstant(poModule, "IQ", POINT_IQ);
	ScriptModule_AddIntConstant(poModule, "ATT_POWER", POINT_ATT_POWER);
	ScriptModule_AddIntConstant(poModule, "ATT_MIN", POINT_MIN_ATK);
	ScriptModule_AddIntConstant(poModule, "ATT_MAX", POINT_MAX_ATK);
	ScriptModule_AddIntConstant(poModule, "MIN_MAGIC_WEP", POINT_MIN_MAGIC_WEP);
	ScriptModule_AddIntConstant(poModule, "MAX_MAGIC_WEP", POINT_MAX_MAGIC_WEP);
	ScriptModule_AddIntConstant(poModule, "ATT_SPEED", POINT_ATT_SPEED);
	ScriptModule_AddIntConstant(poModule, "ATT_BONUS", POINT_ATT_GRADE_BONUS);
	ScriptModule_AddIntConstant(poModule, "EVADE_RATE", POINT_EVADE_RATE);
	ScriptModule_AddIntConstant(poModule, "MOVING_SPEED", POINT_MOV_SPEED);
	ScriptModule_AddIntConstant(poModule, "DEF_GRADE", POINT_DEF_GRADE);
	ScriptModule_AddIntConstant(poModule, "DEF_BONUS", POINT_DEF_GRADE_BONUS);
	ScriptModule_AddIntConstant(poModule, "CASTING_SPEED", POINT_CASTING_SPEED);
	ScriptModule_AddIntConstant(poModule, "MAG_ATT", POINT_MAGIC_ATT_GRADE);
	ScriptModule_AddIntConstant(poModule, "MAG_DEF", POINT_MAGIC_DEF_GRADE);
	ScriptModule_AddIntConstant(poModule, "EMPIRE_POINT", POINT_EMPIRE_POINT);
	ScriptModule_AddIntConstant(poModule, "STAT", POINT_STAT);
	ScriptModule_AddIntConstant(poModule, "SKILL_PASSIVE", POINT_SUB_SKILL);
	ScriptModule_AddIntConstant(poModule, "SKILL_SUPPORT", POINT_SUB_SKILL);
	ScriptModule_AddIntConstant(poModule, "SKILL_ACTIVE", POINT_SKILL);
	ScriptModule_AddIntConstant(poModule, "SKILL_HORSE", POINT_HORSE_SKILL);
	ScriptModule_AddIntConstant(poModule, "PLAYTIME", POINT_PLAYTIME);
	ScriptModule_AddIntConstant(poModule, "BOW_DISTANCE", POINT_BOW_DISTANCE);
	ScriptModule_AddIntConstant(poModule, "HP_RECOVERY", POINT_HP_RECOVERY);
	ScriptModule_AddIntConstant(poModule, "SP_RECOVERY", POINT_SP_RECOVERY);
	ScriptModule_AddIntConstant(poModule, "ATTACKER_BONUS", POINT_PARTY_ATT_GRADE);
	ScriptModule_AddIntConstant(poModule, "MAX_NUM", POINT_MAX_NUM);
	////
	ScriptModule_AddIntConstant(poModule, "POINT_CRITICAL_PCT", POINT_CRITICAL_PCT);
	ScriptModule_AddIntConstant(poModule, "POINT_PENETRATE_PCT", POINT_PENETRATE_PCT);
	ScriptModule_AddIntConstant(poModule, "POINT_MALL_ATTBONUS", POINT_MALL_ATTBONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_MALL_DEFBONUS", POINT_MALL_DEFBONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_MALL_EXPBONUS", POINT_MALL_EXPBONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_MALL_ITEMBONUS", POINT_MALL_ITEMBONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_MALL_GOLDBONUS", POINT_MALL_GOLDBONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_MAX_HP_PCT", POINT_MAX_HP_PCT);
	ScriptModule_AddIntConstant(poModule, "POINT_MAX_SP_PCT", POINT_MAX_SP_PCT);

	ScriptModule_AddIntConstant(poModule, "POINT_SKILL_DAMAGE_BONUS", POINT_SKILL_DAMAGE_BONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_NORMAL_HIT_DAMAGE_BONUS", POINT_NORMAL_HIT_DAMAGE_BONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_SKILL_DEFEND_BONUS", POINT_SKILL_DEFEND_BONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_NORMAL_HIT_DEFEND_BONUS", POINT_NORMAL_HIT_DEFEND_BONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_PC_BANG_EXP_BONUS", POINT_PC_BANG_EXP_BONUS);
	ScriptModule_AddIntConstant(poModule, "POINT_PC_BANG_DROP_BONUS", POINT_PC_BANG_DROP_BONUS);

	ScriptModule_AddIntConstant(poModule, "ENERGY", POINT_ENERGY);
	ScriptModule_AddIntConstant(poModule, "ENERGY_END_TIME", POINT_ENERGY_END_TIME);

	ScriptModule_AddIntConstant(poModule, "SKILL_GRADE_NORMAL", CPythonPlayer::SKILL_NORMAL);
	ScriptModule_AddIntConstant(poModule, "SKILL_GRADE_MASTER", CPythonPlayer::SKILL_MASTER);
	ScriptModule_AddIntConstant(poModule, "SKILL_GRADE_GRAND_MASTER", CPythonPlayer::SKILL_GRAND_MASTER);
	ScriptModule_AddIntConstant(poModule, "SKILL_GRADE_PERFECT_MASTER", CPythonPlayer::SKILL_PERFECT_MASTER);

	ScriptModule_AddIntConstant(poModule, "CATEGORY_ACTIVE", CPythonPlayer::CATEGORY_ACTIVE);
	ScriptModule_AddIntConstant(poModule, "CATEGORY_PASSIVE", CPythonPlayer::CATEGORY_PASSIVE);

	ScriptModule_AddIntConstant(poModule, "INVENTORY_PAGE_SIZE", c_Inventory_Page_Size);
	ScriptModule_AddIntConstant(poModule, "INVENTORY_PAGE_COUNT", c_Inventory_Page_Count);
	ScriptModule_AddIntConstant(poModule, "INVENTORY_SLOT_COUNT", c_Inventory_Count);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_SLOT_START", c_Equipment_Start);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT_PAGE_COUNT", c_Equipment_Count);

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	ScriptModule_AddIntConstant(poModule, "NEW_EQUIPMENT_SLOT_START", c_New_Equipment_Start);
	ScriptModule_AddIntConstant(poModule, "NEW_EQUIPMENT_SLOT_COUNT", c_New_Equipment_Count);
#endif

	ScriptModule_AddIntConstant(poModule, "MBF_SKILL", CPythonPlayer::MBF_SKILL);
	ScriptModule_AddIntConstant(poModule, "MBF_ATTACK", CPythonPlayer::MBF_ATTACK);
	ScriptModule_AddIntConstant(poModule, "MBF_CAMERA", CPythonPlayer::MBF_CAMERA);
	ScriptModule_AddIntConstant(poModule, "MBF_SMART", CPythonPlayer::MBF_SMART);
	ScriptModule_AddIntConstant(poModule, "MBF_MOVE", CPythonPlayer::MBF_MOVE);
	ScriptModule_AddIntConstant(poModule, "MBF_AUTO", CPythonPlayer::MBF_AUTO);
	ScriptModule_AddIntConstant(poModule, "MBS_PRESS", CPythonPlayer::MBS_PRESS);
	ScriptModule_AddIntConstant(poModule, "MBS_CLICK", CPythonPlayer::MBS_CLICK);
	ScriptModule_AddIntConstant(poModule, "MBT_RIGHT", CPythonPlayer::MBT_RIGHT);
	ScriptModule_AddIntConstant(poModule, "MBT_LEFT", CPythonPlayer::MBT_LEFT);

	// Public code with server
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_NONE", SLOT_TYPE_NONE);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_INVENTORY", SLOT_TYPE_INVENTORY);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_SKILL", SLOT_TYPE_SKILL);
	// Special indecies for client
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_SHOP", SLOT_TYPE_SHOP);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_EXCHANGE_OWNER", SLOT_TYPE_EXCHANGE_OWNER);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_EXCHANGE_TARGET", SLOT_TYPE_EXCHANGE_TARGET);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_QUICK_SLOT", SLOT_TYPE_QUICK_SLOT);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_SAFEBOX", SLOT_TYPE_SAFEBOX);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_PRIVATE_SHOP", SLOT_TYPE_PRIVATE_SHOP);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_MALL", SLOT_TYPE_MALL);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_EMOTION", SLOT_TYPE_EMOTION);
	ScriptModule_AddIntConstant(poModule, "SLOT_TYPE_DRAGON_SOUL_INVENTORY", SLOT_TYPE_DRAGON_SOUL_INVENTORY);

	ScriptModule_AddIntConstant(poModule, "RESERVED_WINDOW", RESERVED_WINDOW);
	ScriptModule_AddIntConstant(poModule, "INVENTORY", INVENTORY);
	ScriptModule_AddIntConstant(poModule, "EQUIPMENT", EQUIPMENT);
	ScriptModule_AddIntConstant(poModule, "SAFEBOX", SAFEBOX);
	ScriptModule_AddIntConstant(poModule, "MALL", MALL);
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_INVENTORY", DRAGON_SOUL_INVENTORY);
	ScriptModule_AddIntConstant(poModule, "GROUND", GROUND);

	ScriptModule_AddIntConstant(poModule, "ITEM_MONEY", -1);

	ScriptModule_AddIntConstant(poModule, "SKILL_SLOT_COUNT", SKILL_MAX_NUM);

	ScriptModule_AddIntConstant(poModule, "EFFECT_PICK", CPythonPlayer::EFFECT_PICK);

	ScriptModule_AddIntConstant(poModule, "METIN_SOCKET_TYPE_NONE", CPythonPlayer::METIN_SOCKET_TYPE_NONE);
	ScriptModule_AddIntConstant(poModule, "METIN_SOCKET_TYPE_SILVER", CPythonPlayer::METIN_SOCKET_TYPE_SILVER);
	ScriptModule_AddIntConstant(poModule, "METIN_SOCKET_TYPE_GOLD", CPythonPlayer::METIN_SOCKET_TYPE_GOLD);
	ScriptModule_AddIntConstant(poModule, "METIN_SOCKET_MAX_NUM", ITEM_SOCKET_SLOT_MAX_NUM);
	ScriptModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_MAX_NUM", ITEM_ATTRIBUTE_SLOT_MAX_NUM);

	ScriptModule_AddIntConstant(poModule, "REFINE_CANT", REFINE_CANT);
	ScriptModule_AddIntConstant(poModule, "REFINE_OK", REFINE_OK);
	ScriptModule_AddIntConstant(poModule, "REFINE_ALREADY_MAX_SOCKET_COUNT", REFINE_ALREADY_MAX_SOCKET_COUNT);
	ScriptModule_AddIntConstant(poModule, "REFINE_NEED_MORE_GOOD_SCROLL", REFINE_NEED_MORE_GOOD_SCROLL);
	ScriptModule_AddIntConstant(poModule, "REFINE_CANT_MAKE_SOCKET_ITEM", REFINE_CANT_MAKE_SOCKET_ITEM);
	ScriptModule_AddIntConstant(poModule, "REFINE_NOT_NEXT_GRADE_ITEM", REFINE_NOT_NEXT_GRADE_ITEM);
	ScriptModule_AddIntConstant(poModule, "REFINE_CANT_REFINE_METIN_TO_EQUIPMENT", REFINE_CANT_REFINE_METIN_TO_EQUIPMENT);
	ScriptModule_AddIntConstant(poModule, "REFINE_CANT_REFINE_ROD", REFINE_CANT_REFINE_ROD);
	ScriptModule_AddIntConstant(poModule, "ATTACH_METIN_CANT", ATTACH_METIN_CANT);
	ScriptModule_AddIntConstant(poModule, "ATTACH_METIN_OK", ATTACH_METIN_OK);
	ScriptModule_AddIntConstant(poModule, "ATTACH_METIN_NOT_MATCHABLE_ITEM", ATTACH_METIN_NOT_MATCHABLE_ITEM);
	ScriptModule_AddIntConstant(poModule, "ATTACH_METIN_NO_MATCHABLE_SOCKET", ATTACH_METIN_NO_MATCHABLE_SOCKET);
	ScriptModule_AddIntConstant(poModule, "ATTACH_METIN_NOT_EXIST_GOLD_SOCKET", ATTACH_METIN_NOT_EXIST_GOLD_SOCKET);
	ScriptModule_AddIntConstant(poModule, "ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT", ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT);
	ScriptModule_AddIntConstant(poModule, "DETACH_METIN_CANT", DETACH_METIN_CANT);
	ScriptModule_AddIntConstant(poModule, "DETACH_METIN_OK", DETACH_METIN_OK);

	// Party
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_NORMAL", CPythonPlayer::PARTY_ROLE_NORMAL);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_LEADER", CPythonPlayer::PARTY_ROLE_LEADER);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_ATTACKER", CPythonPlayer::PARTY_ROLE_ATTACKER);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_TANKER", CPythonPlayer::PARTY_ROLE_TANKER);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_BUFFER", CPythonPlayer::PARTY_ROLE_BUFFER);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_SKILL_MASTER", CPythonPlayer::PARTY_ROLE_SKILL_MASTER);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_BERSERKER", CPythonPlayer::PARTY_ROLE_BERSERKER);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_DEFENDER", CPythonPlayer::PARTY_ROLE_DEFENDER);
	ScriptModule_AddIntConstant(poModule, "PARTY_STATE_MAX_NUM", CPythonPlayer::PARTY_ROLE_MAX_NUM);

	// Skill Index
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_TONGSOL", c_iSkillIndex_Tongsol);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_FISHING", c_iSkillIndex_Fishing);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_MINING", c_iSkillIndex_Mining);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_MAKING", c_iSkillIndex_Making);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_COMBO", c_iSkillIndex_Combo);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_LANGUAGE1", c_iSkillIndex_Language1);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_LANGUAGE2", c_iSkillIndex_Language2);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_LANGUAGE3", c_iSkillIndex_Language3);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_POLYMORPH", c_iSkillIndex_Polymorph);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_RIDING", c_iSkillIndex_Riding);
	ScriptModule_AddIntConstant(poModule, "SKILL_INDEX_SUMMON", c_iSkillIndex_Summon);

	// PK Mode
	ScriptModule_AddIntConstant(poModule, "PK_MODE_PEACE", PK_MODE_PEACE);
	ScriptModule_AddIntConstant(poModule, "PK_MODE_REVENGE", PK_MODE_REVENGE);
	ScriptModule_AddIntConstant(poModule, "PK_MODE_FREE", PK_MODE_FREE);
	ScriptModule_AddIntConstant(poModule, "PK_MODE_PROTECT", PK_MODE_PROTECT);
	ScriptModule_AddIntConstant(poModule, "PK_MODE_GUILD", PK_MODE_GUILD);
	ScriptModule_AddIntConstant(poModule, "PK_MODE_MAX_NUM", PK_MODE_MAX_NUM);

	// Block Mode
	ScriptModule_AddIntConstant(poModule, "BLOCK_EXCHANGE", BLOCK_EXCHANGE);
	ScriptModule_AddIntConstant(poModule, "BLOCK_PARTY", BLOCK_PARTY_INVITE);
	ScriptModule_AddIntConstant(poModule, "BLOCK_GUILD", BLOCK_GUILD_INVITE);
	ScriptModule_AddIntConstant(poModule, "BLOCK_WHISPER", BLOCK_WHISPER);
	ScriptModule_AddIntConstant(poModule, "BLOCK_FRIEND", BLOCK_MESSENGER_INVITE);
	ScriptModule_AddIntConstant(poModule, "BLOCK_PARTY_REQUEST", BLOCK_PARTY_REQUEST);

	// Party
	ScriptModule_AddIntConstant(poModule, "PARTY_EXP_NON_DISTRIBUTION", PARTY_EXP_DISTRIBUTION_NON_PARITY);
	ScriptModule_AddIntConstant(poModule, "PARTY_EXP_DISTRIBUTION_PARITY", PARTY_EXP_DISTRIBUTION_PARITY);

	// Emotion
	ScriptModule_AddIntConstant(poModule, "EMOTION_CLAP", EMOTION_CLAP);
	ScriptModule_AddIntConstant(poModule, "EMOTION_CHEERS_1", EMOTION_CHEERS_1);
	ScriptModule_AddIntConstant(poModule, "EMOTION_CHEERS_2", EMOTION_CHEERS_2);
	ScriptModule_AddIntConstant(poModule, "EMOTION_DANCE_1", EMOTION_DANCE_1);
	ScriptModule_AddIntConstant(poModule, "EMOTION_DANCE_2", EMOTION_DANCE_2);
	ScriptModule_AddIntConstant(poModule, "EMOTION_DANCE_3", EMOTION_DANCE_3);
	ScriptModule_AddIntConstant(poModule, "EMOTION_DANCE_4", EMOTION_DANCE_4);
	ScriptModule_AddIntConstant(poModule, "EMOTION_DANCE_5", EMOTION_DANCE_5);
	ScriptModule_AddIntConstant(poModule, "EMOTION_DANCE_6", EMOTION_DANCE_6); // PSY 강남스타일
	ScriptModule_AddIntConstant(poModule, "EMOTION_CONGRATULATION", EMOTION_CONGRATULATION);
	ScriptModule_AddIntConstant(poModule, "EMOTION_FORGIVE", EMOTION_FORGIVE);
	ScriptModule_AddIntConstant(poModule, "EMOTION_ANGRY", EMOTION_ANGRY);
	ScriptModule_AddIntConstant(poModule, "EMOTION_ATTRACTIVE", EMOTION_ATTRACTIVE);
	ScriptModule_AddIntConstant(poModule, "EMOTION_SAD", EMOTION_SAD);
	ScriptModule_AddIntConstant(poModule, "EMOTION_SHY", EMOTION_SHY);
	ScriptModule_AddIntConstant(poModule, "EMOTION_CHEERUP", EMOTION_CHEERUP);
	ScriptModule_AddIntConstant(poModule, "EMOTION_BANTER", EMOTION_BANTER);
	ScriptModule_AddIntConstant(poModule, "EMOTION_JOY", EMOTION_JOY);

	ScriptModule_AddIntConstant(poModule, "EMOTION_KISS", EMOTION_KISS);
	ScriptModule_AddIntConstant(poModule, "EMOTION_FRENCH_KISS", EMOTION_FRENCH_KISS);
	ScriptModule_AddIntConstant(poModule, "EMOTION_SLAP", EMOTION_SLAP);

	//// 자동물약 타입
	ScriptModule_AddIntConstant(poModule, "AUTO_POTION_TYPE_HP", CPythonPlayer::AUTO_POTION_TYPE_HP);
	ScriptModule_AddIntConstant(poModule, "AUTO_POTION_TYPE_SP", CPythonPlayer::AUTO_POTION_TYPE_SP);

	// 용혼석
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_PAGE_SIZE", c_DragonSoul_Inventory_Box_Size);
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_PAGE_COUNT", DRAGON_SOUL_GRADE_MAX);
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_SLOT_COUNT", c_DragonSoul_Inventory_Count);
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_EQUIPMENT_SLOT_START", c_DragonSoul_Equip_Start);
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_EQUIPMENT_PAGE_COUNT", DS_DECK_MAX_NUM);
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_EQUIPMENT_FIRST_SIZE", c_DragonSoul_Equip_Slot_Max);

	// 용혼석 개량창
	ScriptModule_AddIntConstant(poModule, "DRAGON_SOUL_REFINE_CLOSE", DS_SUB_HEADER_CLOSE);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_DO_UPGRADE", DS_SUB_HEADER_DO_UPGRADE);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_DO_IMPROVEMENT", DS_SUB_HEADER_DO_IMPROVEMENT);
	ScriptModule_AddIntConstant(poModule, "DS_SUB_HEADER_DO_REFINE", DS_SUB_HEADER_DO_REFINE);
}
