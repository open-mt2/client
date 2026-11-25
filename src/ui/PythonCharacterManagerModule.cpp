#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonCharacterManager.h>
#include <Client/UI/PythonBackground.h>
#include <Client/UI/InstanceBase.h>
#include <Client/Game/RaceManager.h>

//////////////////////////////////////////////////////////////////////////
// RaceData 관련 시작
//////////////////////////////////////////////////////////////////////////

ScriptObject* chrmgrSetEmpireNameMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iEnable;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iEnable))
		return Script_BadArgument();

	CInstanceBase::SetEmpireNameMode(iEnable ? true : false);
	CPythonCharacterManager::Instance().RefreshAllPCTextTail();

	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterTitleName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BadArgument();
	char* szTitleName;
	if (!ScriptTuple_GetString(poArgs, 1, &szTitleName))
		return Script_BadArgument();

	CInstanceBase::RegisterTitleName(iIndex, szTitleName);
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterNameColor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index;
	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BadArgument();

	int ir;
	if (!ScriptTuple_GetInteger(poArgs, 1, &ir))
		return Script_BadArgument();
	int ig;
	if (!ScriptTuple_GetInteger(poArgs, 2, &ig))
		return Script_BadArgument();
	int ib;
	if (!ScriptTuple_GetInteger(poArgs, 3, &ib))
		return Script_BadArgument();

	CInstanceBase::RegisterNameColor(index, ir, ig, ib);
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterTitleColor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BadArgument();
	int ir;
	if (!ScriptTuple_GetInteger(poArgs, 1, &ir))
		return Script_BadArgument();
	int ig;
	if (!ScriptTuple_GetInteger(poArgs, 2, &ig))
		return Script_BadArgument();
	int ib;
	if (!ScriptTuple_GetInteger(poArgs, 3, &ib))
		return Script_BadArgument();

	CInstanceBase::RegisterTitleColor(iIndex, ir, ig, ib);
	return Script_BuildNone();
}

ScriptObject* chrmgrGetPickedVID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	DWORD dwPickedActorID;
	if (rkChrMgr.OLD_GetPickedInstanceVID(&dwPickedActorID))
		return Script_BuildValue("i", dwPickedActorID);
	else
		return Script_BuildValue("i", -1);
}

ScriptObject* chrmgrGetVIDInfo(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nVID))
		return Script_BadArgument();

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	char szDetail[256] = "";
	CInstanceBase* pkInstBase = rkChrMgr.GetInstancePtr(nVID);
	if (pkInstBase)
	{
		TPixelPosition kPPosInst;
		pkInstBase->NEW_GetPixelPosition(&kPPosInst);

		LONG xInst = kPPosInst.x;
		LONG yInst = kPPosInst.y;

		CPythonBackground& rkBG = CPythonBackground::Instance();
		rkBG.LocalPositionToGlobalPosition(xInst, yInst);
		sprintf(szDetail, "pos=(%d, %d)", xInst, yInst);
	}

	char szInfo[1024];
	sprintf(szInfo, "VID %d (isRegistered=%d, isAlive=%d, isDead=%d) %s",
			nVID,
			rkChrMgr.IsRegisteredVID(nVID),
			rkChrMgr.IsAliveVID(nVID),
			rkChrMgr.IsDeadVID(nVID),
			szDetail);

	return Script_BuildValue("s", szInfo);
}

ScriptObject* chrmgrSetPathName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szPathName;
	if (!ScriptTuple_GetString(poArgs, 0, &szPathName))
		return Script_BadArgument();

	CRaceManager::Instance().SetPathName(szPathName);
	return Script_BuildNone();
}

ScriptObject* chrmgrCreateRace(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iRace;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iRace))
		return Script_BadArgument();

	CRaceManager::Instance().CreateRace(iRace);
	return Script_BuildNone();
}

ScriptObject* chrmgrSelectRace(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iRace;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iRace))
		return Script_BadArgument();

	CRaceManager::Instance().SelectRace(iRace);
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterAttachingBoneName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iPartIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iPartIndex))
		return Script_BadArgument();
	char* szBoneName;
	if (!ScriptTuple_GetString(poArgs, 1, &szBoneName))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->RegisterAttachingBoneName(iPartIndex, szBoneName);
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterMotionMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionIndex))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->RegisterMotionMode(iMotionIndex);
	return Script_BuildNone();
}

ScriptObject* chrmgrSetMotionRandomWeight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BadArgument();

	int iMotion;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMotion))
		return Script_BadArgument();

	int iSubMotion;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iSubMotion))
		return Script_BadArgument();

	int iPercentage;
	if (!ScriptTuple_GetInteger(poArgs, 3, &iPercentage))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	if (!pRaceData->SetMotionRandomWeight(iMode, iMotion, iSubMotion, iPercentage))
		Script_BuildException("Failed to SetMotionRandomWeight");

	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterNormalAttack(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BadArgument();

	int iMotion;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMotion))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->RegisterNormalAttack(iMode, iMotion);
	return Script_BuildNone();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 없어질 함수들
ScriptObject* chrmgrReserveComboAttack(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BadArgument();

	int iCount;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iCount))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->ReserveComboAttack(iMode, 0, iCount);
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterComboAttack(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BadArgument();

	int iComboIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iComboIndex))
		return Script_BadArgument();

	int iMotionIndex;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iMotionIndex))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->RegisterComboAttack(iMode, 0, iComboIndex, iMotionIndex);
	return Script_BuildNone();
}
// 없어질 함수들
///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptObject* chrmgrReserveComboAttackNew(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionMode))
		return Script_BadArgument();

	int iComboType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iComboType))
		return Script_BadArgument();

	int iCount;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iCount))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->ReserveComboAttack(iMotionMode, iComboType, iCount);
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterComboAttackNew(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMotionMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMotionMode))
		return Script_BadArgument();

	int iComboType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iComboType))
		return Script_BadArgument();

	int iComboIndex;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iComboIndex))
		return Script_BadArgument();

	int iMotionIndex;
	if (!ScriptTuple_GetInteger(poArgs, 3, &iMotionIndex))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->RegisterComboAttack(iMotionMode, iComboType, iComboIndex, iMotionIndex);
	return Script_BuildNone();
}

ScriptObject* chrmgrLoadRaceData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	const char* c_szFullFileName = CRaceManager::Instance().GetFullPathFileName(szFileName);
	if (!pRaceData->LoadRaceData(c_szFullFileName))
	{
		TraceError("Failed to load race data : %s\n", c_szFullFileName);
	}

	return Script_BuildNone();
}

ScriptObject* chrmgrLoadLocalRaceData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	if (!pRaceData->LoadRaceData(szFileName))
	{
		TraceError("Failed to load race data : %s\n", szFileName);
	}

	return Script_BuildNone();
}

/*
ScriptObject * chrmgrRegisterMotion(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BadArgument();

	int iMotion;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMotion))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 2, &szFileName))
		return Script_BadArgument();

	int iWeight = 0;
	ScriptTuple_GetInteger(poArgs, 3, &iWeight);
	iWeight = MIN(100, iWeight);

	CRaceData * pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->NEW_RegisterMotion(iMode, iMotion, CRaceManager::Instance().GetFullPathFileName(szFileName), iWeight);
	return Script_BuildNone();
}
*/

ScriptObject* chrmgrRegisterCacheMotionData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BadArgument();

	int iMotion;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMotion))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 2, &szFileName))
		return Script_BadArgument();

	int iWeight = 0;
	ScriptTuple_GetInteger(poArgs, 3, &iWeight);
	iWeight = MIN(100, iWeight);

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	const char* c_szFullFileName = CRaceManager::Instance().GetFullPathFileName(szFileName);
	CGraphicThing* pkMotionThing = pRaceData->RegisterMotionData(iMode, iMotion, c_szFullFileName, iWeight);

	if (pkMotionThing)
		CResourceManager::Instance().LoadStaticCache(pkMotionThing->GetFileName());

	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterMotionData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iMode;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iMode))
		return Script_BadArgument();

	int iMotion;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMotion))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 2, &szFileName))
		return Script_BadArgument();

	int iWeight = 0;
	ScriptTuple_GetInteger(poArgs, 3, &iWeight);
	iWeight = MIN(100, iWeight);

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	const char* c_szFullFileName = CRaceManager::Instance().GetFullPathFileName(szFileName);
	pRaceData->RegisterMotionData(iMode, iMotion, c_szFullFileName, iWeight);

	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterRaceSrcName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BadArgument();

	char* szSrcName;
	if (!ScriptTuple_GetString(poArgs, 1, &szSrcName))
		return Script_BadArgument();

	CRaceManager::Instance().RegisterRaceSrcName(szName, szSrcName);

	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterRaceName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iRaceIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iRaceIndex))
		return Script_BadArgument();

	char* szName;
	if (!ScriptTuple_GetString(poArgs, 1, &szName))
		return Script_BadArgument();

	CRaceManager::Instance().RegisterRaceName(iRaceIndex, szName);

	return Script_BuildNone();
}

ScriptObject* chrmgrSetShapeModel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eShape;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eShape))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &szFileName))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->SetShapeModel(eShape, szFileName);
	return Script_BuildNone();
}

ScriptObject* chrmgrAppendShapeSkin(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eShape;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eShape))
		return Script_BadArgument();

	int ePart;
	if (!ScriptTuple_GetInteger(poArgs, 1, &ePart))
		return Script_BadArgument();

	char* szSrcFileName;
	if (!ScriptTuple_GetString(poArgs, 2, &szSrcFileName))
		return Script_BadArgument();

	char* szDstFileName;
	if (!ScriptTuple_GetString(poArgs, 3, &szDstFileName))
		return Script_BadArgument();

	CRaceData* pRaceData = CRaceManager::Instance().GetSelectedRaceDataPointer();
	if (!pRaceData)
		return Script_BuildException("RaceData has not selected!");

	pRaceData->AppendShapeSkin(eShape, ePart, szSrcFileName, szDstFileName);
	return Script_BuildNone();
}
//////////////////////////////////////////////////////////////////////////
// RaceData 관련 끝
//////////////////////////////////////////////////////////////////////////

ScriptObject* chrmgrSetMovingSpeed(ScriptObject* poSelf, ScriptObject* poArgs)
{
#ifndef _DISTRIBUTE
	int nMovSpd;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nMovSpd))
		return Script_BadArgument();

	if (nMovSpd < 0)
		return Script_BuildException("MovingSpeed < 0");

	CInstanceBase* pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Script_BuildException("MainCharacter has not selected!");

	pkInst->SetMoveSpeed(nMovSpd);
#endif
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eEftType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eEftType))
		return Script_BadArgument();

	char* szBoneName;
	if (!ScriptTuple_GetString(poArgs, 1, &szBoneName))
		return Script_BadArgument();

	char* szPathName;
	if (!ScriptTuple_GetString(poArgs, 2, &szPathName))
		return Script_BadArgument();

	CInstanceBase::RegisterEffect(eEftType, szBoneName, szPathName, false);
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterCacheEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int eEftType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &eEftType))
		return Script_BadArgument();

	char* szBoneName;
	if (!ScriptTuple_GetString(poArgs, 1, &szBoneName))
		return Script_BadArgument();

	char* szPathName;
	if (!ScriptTuple_GetString(poArgs, 2, &szPathName))
		return Script_BadArgument();

	CInstanceBase::RegisterEffect(eEftType, szBoneName, szPathName, true);

	return Script_BuildNone();
}

ScriptObject* chrmgrSetDustGap(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nGap;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nGap))
		return Script_BadArgument();

	CInstanceBase::SetDustGap(nGap);
	return Script_BuildNone();
}

ScriptObject* chrmgrSetHorseDustGap(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nGap;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nGap))
		return Script_BadArgument();

	CInstanceBase::SetHorseDustGap(nGap);
	return Script_BuildNone();
}

ScriptObject* chrmgrToggleDirectionLine(ScriptObject* poSelf, ScriptObject* poArgs)
{
	static bool s_isVisible = true;
	CActorInstance::ShowDirectionLine(s_isVisible);

	s_isVisible = !s_isVisible;
	return Script_BuildNone();
}

ScriptObject* chrmgrRegisterPointEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iEft;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iEft))
		return Script_BadArgument();

	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 1, &szFileName))
		return Script_BadArgument();

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.RegisterPointEffect(iEft, szFileName);
	return Script_BuildNone();
}

ScriptObject* chrmgrShowPointEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nVID))
		return Script_BadArgument();

	int nEft;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nEft))
		return Script_BadArgument();

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.ShowPointEffect(nEft, nVID >= 0 ? nVID : 0xffffffff);
	return Script_BuildNone();
}

void CPythonCharacterManager::SCRIPT_SetAffect(DWORD dwVID, DWORD eState, BOOL isVisible)
{
	CInstanceBase* pkInstSel = (dwVID == 0xffffffff) ? GetSelectedInstancePtr() : GetInstancePtr(dwVID);
	if (!pkInstSel)
		return;

	pkInstSel->SCRIPT_SetAffect(eState, isVisible ? true : false);
}

ScriptObject* chrmgrSetAffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nVID))
		return Script_BadArgument();

	int nEft;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nEft))
		return Script_BadArgument();

	int nVisible;
	if (!ScriptTuple_GetInteger(poArgs, 2, &nVisible))
		return Script_BadArgument();

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.SCRIPT_SetAffect(nVID >= 0 ? nVID : 0xffffffff, nEft, nVisible);
	return Script_BuildNone();
}

void CPythonCharacterManager::SetEmoticon(DWORD dwVID, DWORD eState)
{
	CInstanceBase* pkInstSel = (dwVID == 0xffffffff) ? GetSelectedInstancePtr() : GetInstancePtr(dwVID);
	if (!pkInstSel)
		return;

	pkInstSel->SetEmoticon(eState);
}

bool CPythonCharacterManager::IsPossibleEmoticon(DWORD dwVID)
{
	CInstanceBase* pkInstSel = (dwVID == 0xffffffff) ? GetSelectedInstancePtr() : GetInstancePtr(dwVID);
	if (!pkInstSel)
		return false;

	return pkInstSel->IsPossibleEmoticon();
}

ScriptObject* chrmgrSetEmoticon(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nVID))
		return Script_BadArgument();

	int nEft;
	if (!ScriptTuple_GetInteger(poArgs, 1, &nEft))
		return Script_BadArgument();

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.SetEmoticon(nVID >= 0 ? nVID : 0xffffffff, nEft);
	return Script_BuildNone();
}

ScriptObject* chrmgrIsPossibleEmoticon(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int nVID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &nVID))
		return Script_BadArgument();

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	int result = rkChrMgr.IsPossibleEmoticon(nVID >= 0 ? nVID : 0xffffffff);

	return Script_BuildValue("i", result);
}

void initchrmgr()
{
	static ScriptMethodDef s_methods[] =
		{
			// RaceData 관련
			{"SetEmpireNameMode", chrmgrSetEmpireNameMode, SCRIPT_VARARGS, NULL},
			{"GetVIDInfo", chrmgrGetVIDInfo, SCRIPT_VARARGS, NULL},
			{"GetPickedVID", chrmgrGetPickedVID, SCRIPT_VARARGS, NULL},
			{"SetShapeModel", chrmgrSetShapeModel, SCRIPT_VARARGS, NULL},
			{"AppendShapeSkin", chrmgrAppendShapeSkin, SCRIPT_VARARGS, NULL},
			{"SetPathName", chrmgrSetPathName, SCRIPT_VARARGS, NULL},
			{"LoadRaceData", chrmgrLoadRaceData, SCRIPT_VARARGS, NULL},
			{"LoadLocalRaceData", chrmgrLoadLocalRaceData, SCRIPT_VARARGS, NULL},
			{"CreateRace", chrmgrCreateRace, SCRIPT_VARARGS, NULL},
			{"SelectRace", chrmgrSelectRace, SCRIPT_VARARGS, NULL},
			{"RegisterAttachingBoneName", chrmgrRegisterAttachingBoneName, SCRIPT_VARARGS, NULL},
			{"RegisterMotionMode", chrmgrRegisterMotionMode, SCRIPT_VARARGS, NULL},
			//{ "RegisterMotion",				chrmgrRegisterMotion,					SCRIPT_VARARGS },
			{"SetMotionRandomWeight", chrmgrSetMotionRandomWeight, SCRIPT_VARARGS, NULL},
			{"RegisterNormalAttack", chrmgrRegisterNormalAttack, SCRIPT_VARARGS, NULL},
			{"ReserveComboAttack", chrmgrReserveComboAttack, SCRIPT_VARARGS, NULL},
			{"RegisterComboAttack", chrmgrRegisterComboAttack, SCRIPT_VARARGS, NULL},
			{"ReserveComboAttackNew", chrmgrReserveComboAttackNew, SCRIPT_VARARGS, NULL},
			{"RegisterComboAttackNew", chrmgrRegisterComboAttackNew, SCRIPT_VARARGS, NULL},
			{"RegisterMotionData", chrmgrRegisterMotionData, SCRIPT_VARARGS, NULL},
			{"RegisterRaceName", chrmgrRegisterRaceName, SCRIPT_VARARGS, NULL},
			{"RegisterRaceSrcName", chrmgrRegisterRaceSrcName, SCRIPT_VARARGS, NULL},
			{"RegisterCacheMotionData", chrmgrRegisterCacheMotionData, SCRIPT_VARARGS, NULL},

			// ETC
			{"SetAffect", chrmgrSetAffect, SCRIPT_VARARGS, NULL},
			{"SetEmoticon", chrmgrSetEmoticon, SCRIPT_VARARGS, NULL},
			{"IsPossibleEmoticon", chrmgrIsPossibleEmoticon, SCRIPT_VARARGS, NULL},
			{"RegisterEffect", chrmgrRegisterEffect, SCRIPT_VARARGS, NULL},
			{"RegisterCacheEffect", chrmgrRegisterCacheEffect, SCRIPT_VARARGS, NULL},
			{"RegisterPointEffect", chrmgrRegisterPointEffect, SCRIPT_VARARGS, NULL},
			{"ShowPointEffect", chrmgrShowPointEffect, SCRIPT_VARARGS, NULL},
			{"ToggleDirectionLine", chrmgrToggleDirectionLine, SCRIPT_VARARGS, NULL},
			{"SetMovingSpeed", chrmgrSetMovingSpeed, SCRIPT_VARARGS, NULL},
			{"SetDustGap", chrmgrSetDustGap, SCRIPT_VARARGS, NULL},
			{"SetHorseDustGap", chrmgrSetHorseDustGap, SCRIPT_VARARGS, NULL},

			{"RegisterTitleName", chrmgrRegisterTitleName, SCRIPT_VARARGS, NULL},
			{"RegisterNameColor", chrmgrRegisterNameColor, SCRIPT_VARARGS, NULL},
			{"RegisterTitleColor", chrmgrRegisterTitleColor, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("chrmgr", s_methods);

	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_MOB", CInstanceBase::NAMECOLOR_NORMAL_MOB);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_NPC", CInstanceBase::NAMECOLOR_NORMAL_NPC);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_PC", CInstanceBase::NAMECOLOR_NORMAL_PC);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_EMPIRE_MOB", CInstanceBase::NAMECOLOR_EMPIRE_MOB);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_EMPIRE_NPC", CInstanceBase::NAMECOLOR_EMPIRE_NPC);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_EMPIRE_PC", CInstanceBase::NAMECOLOR_EMPIRE_PC);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_FUNC", CInstanceBase::NAMECOLOR_FUNC);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_PK", CInstanceBase::NAMECOLOR_PK);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_PVP", CInstanceBase::NAMECOLOR_PVP);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_PARTY", CInstanceBase::NAMECOLOR_PARTY);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_WARP", CInstanceBase::NAMECOLOR_WARP);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_WAYPOINT", CInstanceBase::NAMECOLOR_WAYPOINT);
	ScriptModule_AddIntConstant(poModule, "NAMECOLOR_EXTRA", CInstanceBase::NAMECOLOR_EXTRA);

	ScriptModule_AddIntConstant(poModule, "EFFECT_SPAWN_DISAPPEAR", CInstanceBase::EFFECT_SPAWN_DISAPPEAR);
	ScriptModule_AddIntConstant(poModule, "EFFECT_SPAWN_APPEAR", CInstanceBase::EFFECT_SPAWN_APPEAR);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DUST", CInstanceBase::EFFECT_DUST);
	ScriptModule_AddIntConstant(poModule, "EFFECT_HORSE_DUST", CInstanceBase::EFFECT_HORSE_DUST);
	ScriptModule_AddIntConstant(poModule, "EFFECT_STUN", CInstanceBase::EFFECT_STUN);
	ScriptModule_AddIntConstant(poModule, "EFFECT_HIT", CInstanceBase::EFFECT_HIT);
	ScriptModule_AddIntConstant(poModule, "EFFECT_FLAME_ATTACK", CInstanceBase::EFFECT_FLAME_ATTACK);
	ScriptModule_AddIntConstant(poModule, "EFFECT_FLAME_HIT", CInstanceBase::EFFECT_FLAME_HIT);
	ScriptModule_AddIntConstant(poModule, "EFFECT_FLAME_ATTACH", CInstanceBase::EFFECT_FLAME_ATTACH);
	ScriptModule_AddIntConstant(poModule, "EFFECT_ELECTRIC_ATTACK", CInstanceBase::EFFECT_ELECTRIC_ATTACK);
	ScriptModule_AddIntConstant(poModule, "EFFECT_ELECTRIC_HIT", CInstanceBase::EFFECT_ELECTRIC_HIT);
	ScriptModule_AddIntConstant(poModule, "EFFECT_ELECTRIC_ATTACH", CInstanceBase::EFFECT_ELECTRIC_ATTACH);
	ScriptModule_AddIntConstant(poModule, "EFFECT_SELECT", CInstanceBase::EFFECT_SELECT);
	ScriptModule_AddIntConstant(poModule, "EFFECT_TARGET", CInstanceBase::EFFECT_TARGET);
	ScriptModule_AddIntConstant(poModule, "EFFECT_CRITICAL", CInstanceBase::EFFECT_CRITICAL);

	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_TARGET", CInstanceBase::EFFECT_DAMAGE_TARGET);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_NOT_TARGET", CInstanceBase::EFFECT_DAMAGE_NOT_TARGET);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_SELFDAMAGE", CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_SELFDAMAGE2", CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE2);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_POISON", CInstanceBase::EFFECT_DAMAGE_POISON);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_MISS", CInstanceBase::EFFECT_DAMAGE_MISS);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_TARGETMISS", CInstanceBase::EFFECT_DAMAGE_TARGETMISS);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DAMAGE_CRITICAL", CInstanceBase::EFFECT_DAMAGE_CRITICAL);

	ScriptModule_AddIntConstant(poModule, "EFFECT_LEVELUP", CInstanceBase::EFFECT_LEVELUP);
	ScriptModule_AddIntConstant(poModule, "EFFECT_SKILLUP", CInstanceBase::EFFECT_SKILLUP);
	ScriptModule_AddIntConstant(poModule, "EFFECT_HPUP_RED", CInstanceBase::EFFECT_HPUP_RED);
	ScriptModule_AddIntConstant(poModule, "EFFECT_SPUP_BLUE", CInstanceBase::EFFECT_SPUP_BLUE);
	ScriptModule_AddIntConstant(poModule, "EFFECT_SPEEDUP_GREEN", CInstanceBase::EFFECT_SPEEDUP_GREEN);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DXUP_PURPLE", CInstanceBase::EFFECT_DXUP_PURPLE);
	ScriptModule_AddIntConstant(poModule, "EFFECT_PENETRATE", CInstanceBase::EFFECT_PENETRATE);
	ScriptModule_AddIntConstant(poModule, "EFFECT_BLOCK", CInstanceBase::EFFECT_BLOCK);
	ScriptModule_AddIntConstant(poModule, "EFFECT_DODGE", CInstanceBase::EFFECT_DODGE);
	ScriptModule_AddIntConstant(poModule, "EFFECT_FIRECRACKER", CInstanceBase::EFFECT_FIRECRACKER);
	ScriptModule_AddIntConstant(poModule, "EFFECT_SPIN_TOP", CInstanceBase::EFFECT_SPIN_TOP);

	ScriptModule_AddIntConstant(poModule, "EFFECT_WEAPON", CInstanceBase::EFFECT_WEAPON);

	ScriptModule_AddIntConstant(poModule, "EFFECT_AFFECT", CInstanceBase::EFFECT_AFFECT);
	ScriptModule_AddIntConstant(poModule, "EFFECT_EMOTICON", CInstanceBase::EFFECT_EMOTICON);
	ScriptModule_AddIntConstant(poModule, "EFFECT_EMPIRE", CInstanceBase::EFFECT_EMPIRE);

	ScriptModule_AddIntConstant(poModule, "EFFECT_REFINED", CInstanceBase::EFFECT_REFINED);

	ScriptModule_AddIntConstant(poModule, "EFFECT_SUCCESS", CInstanceBase::EFFECT_SUCCESS);
	ScriptModule_AddIntConstant(poModule, "EFFECT_FAIL", CInstanceBase::EFFECT_FAIL);
	ScriptModule_AddIntConstant(poModule, "EFFECT_FR_SUCCESS", CInstanceBase::EFFECT_FR_SUCCESS);
	ScriptModule_AddIntConstant(poModule, "EFFECT_LEVELUP_ON_14_FOR_GERMANY", CInstanceBase::EFFECT_LEVELUP_ON_14_FOR_GERMANY);		  // 레벨업 14일때 ( 독일전용 )
	ScriptModule_AddIntConstant(poModule, "EFFECT_LEVELUP_UNDER_15_FOR_GERMANY", CInstanceBase::EFFECT_LEVELUP_UNDER_15_FOR_GERMANY); // 레벨업 15일때 ( 독일전용 )
	ScriptModule_AddIntConstant(poModule, "EFFECT_PERCENT_DAMAGE1", CInstanceBase::EFFECT_PERCENT_DAMAGE1);
	ScriptModule_AddIntConstant(poModule, "EFFECT_PERCENT_DAMAGE2", CInstanceBase::EFFECT_PERCENT_DAMAGE2);
	ScriptModule_AddIntConstant(poModule, "EFFECT_PERCENT_DAMAGE3", CInstanceBase::EFFECT_PERCENT_DAMAGE3);

	// 자동물약 HP, SP
	ScriptModule_AddIntConstant(poModule, "EFFECT_AUTO_HPUP", CInstanceBase::EFFECT_AUTO_HPUP);
	ScriptModule_AddIntConstant(poModule, "EFFECT_AUTO_SPUP", CInstanceBase::EFFECT_AUTO_SPUP);

	ScriptModule_AddIntConstant(poModule, "EFFECT_RAMADAN_RING_EQUIP", CInstanceBase::EFFECT_RAMADAN_RING_EQUIP);
	ScriptModule_AddIntConstant(poModule, "EFFECT_HALLOWEEN_CANDY_EQUIP", CInstanceBase::EFFECT_HALLOWEEN_CANDY_EQUIP);
	ScriptModule_AddIntConstant(poModule, "EFFECT_HAPPINESS_RING_EQUIP", CInstanceBase::EFFECT_HAPPINESS_RING_EQUIP);
	ScriptModule_AddIntConstant(poModule, "EFFECT_LOVE_PENDANT_EQUIP", CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP);
}
