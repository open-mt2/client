#include <Client/UI/StdAfx.h>
#include <Neoeter/Effect/EffectManager.h>
#include <Client/UI/PythonCharacterManager.h>

ScriptObject* effectRegisterEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CEffectManager::Instance().RegisterEffect(szFileName);
	return Script_BuildNone();
}

ScriptObject* effectUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CEffectManager::Instance().Update();
	return Script_BuildNone();
}

ScriptObject* effectRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CEffectManager::Instance().Render();
	return Script_BuildNone();
}

ScriptObject* effectCreateEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szEffectName;
	if (!ScriptTuple_GetString(poArgs, 0, &szEffectName))
		return Script_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetSelectedInstancePtr();
	TPixelPosition PixelPosition;
	pInstance->NEW_GetPixelPosition(&PixelPosition);
	int iIndex = CEffectManager::Instance().CreateEffect(szEffectName, PixelPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return Script_BuildValue("i", iIndex);
}

ScriptObject* effectDeleteEffect(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	CEffectManager::Instance().DestroyEffectInstance(iIndex);

	return Script_BuildNone();
}

ScriptObject* effectSetPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	float fx;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fx))
		return Script_BuildException();
	float fy;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fy))
		return Script_BuildException();
	float fz;
	if (!ScriptTuple_GetFloat(poArgs, 3, &fz))
		return Script_BuildException();

	CEffectManager::Instance().SelectEffectInstance(iIndex);
	CEffectManager::Instance().SetEffectInstancePosition(D3DXVECTOR3(fx, fy, fz));

	return Script_BuildNone();
}

ScriptObject* effectRegisterIndexedFlyData(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iType))
		return Script_BuildException();
	char* szFlyDataName;
	if (!ScriptTuple_GetString(poArgs, 2, &szFlyDataName))
		return Script_BuildException();

	CFlyingManager::Instance().RegisterIndexedFlyData(iIndex, iType, szFlyDataName);

	return Script_BuildNone();
}

void initeffect()
{
	static ScriptMethodDef s_methods[] =
		{
			{"RegisterEffect", effectRegisterEffect, SCRIPT_VARARGS, NULL},
			{"CreateEffect", effectCreateEffect, SCRIPT_VARARGS, NULL},
			{"DeleteEffect", effectDeleteEffect, SCRIPT_VARARGS, NULL},
			{"SetPosition", effectSetPosition, SCRIPT_VARARGS, NULL},

			{"RegisterIndexedFlyData", effectRegisterIndexedFlyData, SCRIPT_VARARGS, NULL},

			{"Update", effectUpdate, SCRIPT_VARARGS, NULL},
			{"Render", effectRender, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("effect", s_methods);
	ScriptModule_AddIntConstant(poModule, "INDEX_FLY_TYPE_NORMAL", CFlyingManager::INDEX_FLY_TYPE_NORMAL);
	ScriptModule_AddIntConstant(poModule, "INDEX_FLY_TYPE_FIRE_CRACKER", CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER);
	ScriptModule_AddIntConstant(poModule, "INDEX_FLY_TYPE_AUTO_FIRE", CFlyingManager::INDEX_FLY_TYPE_AUTO_FIRE);

	enum
	{
		FLY_NONE,
		FLY_EXP,
		FLY_HP_MEDIUM,
		FLY_HP_BIG,
		FLY_SP_SMALL,
		FLY_SP_MEDIUM,
		FLY_SP_BIG,
		FLY_FIREWORK1,
		FLY_FIREWORK2,
		FLY_FIREWORK3,
		FLY_FIREWORK4,
		FLY_FIREWORK5,
		FLY_FIREWORK6,
		FLY_FIREWORK_XMAS,
		FLY_CHAIN_LIGHTNING,
		FLY_HP_SMALL,
		FLY_SKILL_MUYEONG,
	};

	ScriptModule_AddIntConstant(poModule, "FLY_NONE", FLY_NONE);
	ScriptModule_AddIntConstant(poModule, "FLY_EXP", FLY_EXP);
	ScriptModule_AddIntConstant(poModule, "FLY_HP_MEDIUM", FLY_HP_MEDIUM);
	ScriptModule_AddIntConstant(poModule, "FLY_HP_BIG", FLY_HP_BIG);
	ScriptModule_AddIntConstant(poModule, "FLY_SP_SMALL", FLY_SP_SMALL);
	ScriptModule_AddIntConstant(poModule, "FLY_SP_MEDIUM", FLY_SP_MEDIUM);
	ScriptModule_AddIntConstant(poModule, "FLY_SP_BIG", FLY_SP_BIG);
	ScriptModule_AddIntConstant(poModule, "FLY_FIREWORK1", FLY_FIREWORK1);
	ScriptModule_AddIntConstant(poModule, "FLY_FIREWORK2", FLY_FIREWORK2);
	ScriptModule_AddIntConstant(poModule, "FLY_FIREWORK3", FLY_FIREWORK3);
	ScriptModule_AddIntConstant(poModule, "FLY_FIREWORK4", FLY_FIREWORK4);
	ScriptModule_AddIntConstant(poModule, "FLY_FIREWORK5", FLY_FIREWORK5);
	ScriptModule_AddIntConstant(poModule, "FLY_FIREWORK6", FLY_FIREWORK6);
	ScriptModule_AddIntConstant(poModule, "FLY_FIREWORK_XMAS", FLY_FIREWORK_XMAS);
	ScriptModule_AddIntConstant(poModule, "FLY_CHAIN_LIGHTNING", FLY_CHAIN_LIGHTNING);
	ScriptModule_AddIntConstant(poModule, "FLY_HP_SMALL", FLY_HP_SMALL);
	ScriptModule_AddIntConstant(poModule, "FLY_SKILL_MUYEONG", FLY_SKILL_MUYEONG);
}
