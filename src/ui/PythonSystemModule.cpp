#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonSystem.h>

ScriptObject* systemGetWidth(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().GetWidth());
}

ScriptObject* systemGetHeight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().GetHeight());
}

ScriptObject* systemSetInterfaceHandler(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poHandler;
	if (!ScriptTuple_GetObject(poArgs, 0, &poHandler))
		return Script_BuildException();

	CPythonSystem::Instance().SetInterfaceHandler(poHandler);
	return Script_BuildNone();
}

ScriptObject* systemDestroyInterfaceHandler(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonSystem::Instance().DestroyInterfaceHandler();
	return Script_BuildNone();
}

ScriptObject* systemReserveResource(ScriptObject* poSelf, ScriptObject* poArgs)
{
	std::set<std::string> ResourceSet;
	CResourceManager::Instance().PushBackgroundLoadingSet(ResourceSet);
	return Script_BuildNone();
}

ScriptObject* systemisInterfaceConfig(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int isInterfaceConfig = CPythonSystem::Instance().isInterfaceConfig();
	return Script_BuildValue("i", isInterfaceConfig);
}

ScriptObject* systemSaveWindowStatus(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	int iVisible;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iVisible))
		return Script_BuildException();

	int iMinimized;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iMinimized))
		return Script_BuildException();

	int ix;
	if (!ScriptTuple_GetInteger(poArgs, 3, &ix))
		return Script_BuildException();

	int iy;
	if (!ScriptTuple_GetInteger(poArgs, 4, &iy))
		return Script_BuildException();

	int iHeight;
	if (!ScriptTuple_GetInteger(poArgs, 5, &iHeight))
		return Script_BuildException();

	CPythonSystem::Instance().SaveWindowStatus(iIndex, iVisible, iMinimized, ix, iy, iHeight);
	return Script_BuildNone();
}

ScriptObject* systemGetWindowStatus(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iIndex))
		return Script_BuildException();

	const CPythonSystem::TWindowStatus& c_rWindowStatus = CPythonSystem::Instance().GetWindowStatusReference(iIndex);
	return Script_BuildValue("iiiii", c_rWindowStatus.isVisible,
							 c_rWindowStatus.isMinimized,
							 c_rWindowStatus.ixPosition,
							 c_rWindowStatus.iyPosition,
							 c_rWindowStatus.iHeight);
}

ScriptObject* systemGetConfig(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonSystem::TConfig* tmp = CPythonSystem::Instance().GetConfig();

	int iRes = CPythonSystem::Instance().GetResolutionIndex(tmp->width, tmp->height, tmp->bpp);
	int iFrequency = CPythonSystem::Instance().GetFrequencyIndex(iRes, tmp->frequency);

	return Script_BuildValue("iiiiiiii", iRes,
							 iFrequency,
							 tmp->is_software_cursor,
							 tmp->is_object_culling,
							 tmp->music_volume,
							 tmp->voice_volume,
							 tmp->gamma,
							 tmp->iDistance);
}

ScriptObject* systemSetSaveID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iValue;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iValue))
		return Script_BuildException();

	char* szSaveID;
	if (!ScriptTuple_GetString(poArgs, 1, &szSaveID))
		return Script_BuildException();

	CPythonSystem::Instance().SetSaveID(iValue, szSaveID);
	return Script_BuildNone();
}

ScriptObject* systemisSaveID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int value = CPythonSystem::Instance().IsSaveID();
	return Script_BuildValue("i", value);
}

ScriptObject* systemGetSaveID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	const char* c_szSaveID = CPythonSystem::Instance().GetSaveID();
	return Script_BuildValue("s", c_szSaveID);
}

ScriptObject* systemGetMusicVolume(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("f", CPythonSystem::Instance().GetMusicVolume());
}

ScriptObject* systemGetSoundVolume(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().GetSoundVolume());
}

ScriptObject* systemSetMusicVolume(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fVolume;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fVolume))
		return Script_BuildException();

	CPythonSystem::Instance().SetMusicVolume(fVolume);
	return Script_BuildNone();
}

ScriptObject* systemSetSoundVolumef(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fVolume;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fVolume))
		return Script_BuildException();

	CPythonSystem::Instance().SetSoundVolumef(fVolume);
	return Script_BuildNone();
}

ScriptObject* systemIsSoftwareCursor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().IsSoftwareCursor());
}

ScriptObject* systemSetViewChatFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();

	CPythonSystem::Instance().SetViewChatFlag(iFlag);

	return Script_BuildNone();
}

ScriptObject* systemIsViewChat(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().IsViewChat());
}

ScriptObject* systemSetAlwaysShowNameFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();

	CPythonSystem::Instance().SetAlwaysShowNameFlag(iFlag);

	return Script_BuildNone();
}

ScriptObject* systemSetShowDamageFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();

	CPythonSystem::Instance().SetShowDamageFlag(iFlag);

	return Script_BuildNone();
}

ScriptObject* systemSetShowSalesTextFlag(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();

	CPythonSystem::Instance().SetShowSalesTextFlag(iFlag);

	return Script_BuildNone();
}

ScriptObject* systemIsAlwaysShowName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().IsAlwaysShowName());
}

ScriptObject* systemIsShowDamage(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().IsShowDamage());
}

ScriptObject* systemIsShowSalesText(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().IsShowSalesText());
}

ScriptObject* systemSetConfig(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int res_index;
	int width;
	int height;
	int bpp;
	int frequency_index;
	int frequency;
	int software_cursor;
	int shadow;
	int object_culling;
	int music_volume;
	int voice_volume;
	int gamma;
	int distance;

	if (!ScriptTuple_GetInteger(poArgs, 0, &res_index))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 1, &frequency_index))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 2, &software_cursor))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 3, &shadow))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 4, &object_culling))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 5, &music_volume))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 6, &voice_volume))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 7, &gamma))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 8, &distance))
		return Script_BuildException();

	if (!CPythonSystem::Instance().GetResolution(res_index, (DWORD*)&width, (DWORD*)&height, (DWORD*)&bpp))
		return Script_BuildNone();

	if (!CPythonSystem::Instance().GetFrequency(res_index, frequency_index, (DWORD*)&frequency))
		return Script_BuildNone();

	CPythonSystem::TConfig tmp;

	memcpy(&tmp, CPythonSystem::Instance().GetConfig(), sizeof(tmp));

	tmp.width = width;
	tmp.height = height;
	tmp.bpp = bpp;
	tmp.frequency = frequency;
	tmp.is_software_cursor = software_cursor ? true : false;
	tmp.is_object_culling = object_culling ? true : false;
	tmp.music_volume = (char)music_volume;
	tmp.voice_volume = (char)voice_volume;
	tmp.gamma = gamma;
	tmp.iDistance = distance;

	CPythonSystem::Instance().SetConfig(&tmp);
	return Script_BuildNone();
}

ScriptObject* systemApplyConfig(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonSystem::Instance().ApplyConfig();
	return Script_BuildNone();
}

ScriptObject* systemSaveConfig(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ret = CPythonSystem::Instance().SaveConfig();
	return Script_BuildValue("i", ret);
}

ScriptObject* systemGetResolutionCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().GetResolutionCount());
}

ScriptObject* systemGetFrequencyCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index;

	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonSystem::Instance().GetFrequencyCount(index));
}

ScriptObject* systemGetResolution(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index;
	DWORD width = 0, height = 0, bpp = 0;

	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BuildException();

	CPythonSystem::Instance().GetResolution(index, &width, &height, &bpp);
	return Script_BuildValue("iii", width, height, bpp);
}

ScriptObject* systemGetCurrentResolution(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonSystem::TConfig* tmp = CPythonSystem::Instance().GetConfig();
	return Script_BuildValue("iii", tmp->width, tmp->height, tmp->bpp);
}

ScriptObject* systemGetFrequency(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int index, frequency_index;
	DWORD frequency = 0;

	if (!ScriptTuple_GetInteger(poArgs, 0, &index))
		return Script_BuildException();

	if (!ScriptTuple_GetInteger(poArgs, 1, &frequency_index))
		return Script_BuildException();

	CPythonSystem::Instance().GetFrequency(index, frequency_index, &frequency);
	return Script_BuildValue("i", frequency);
}

ScriptObject* systemGetShadowLevel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSystem::Instance().GetShadowLevel());
}

ScriptObject* systemSetShadowLevel(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int level;

	if (!ScriptTuple_GetInteger(poArgs, 0, &level))
		return Script_BuildException();

	if (level > 0)
		CPythonSystem::Instance().SetShadowLevel(level);

	return Script_BuildNone();
}

void initsystem()
{
	static ScriptMethodDef s_methods[] =
		{
			{"GetWidth", systemGetWidth, SCRIPT_VARARGS, NULL},
			{"GetHeight", systemGetHeight, SCRIPT_VARARGS, NULL},

			{"SetInterfaceHandler", systemSetInterfaceHandler, SCRIPT_VARARGS, NULL},
			{"DestroyInterfaceHandler", systemDestroyInterfaceHandler, SCRIPT_VARARGS, NULL},
			{"ReserveResource", systemReserveResource, SCRIPT_VARARGS, NULL},

			{"isInterfaceConfig", systemisInterfaceConfig, SCRIPT_VARARGS, NULL},
			{"SaveWindowStatus", systemSaveWindowStatus, SCRIPT_VARARGS, NULL},
			{"GetWindowStatus", systemGetWindowStatus, SCRIPT_VARARGS, NULL},

			{"GetResolutionCount", systemGetResolutionCount, SCRIPT_VARARGS, NULL},
			{"GetFrequencyCount", systemGetFrequencyCount, SCRIPT_VARARGS, NULL},

			{"GetCurrentResolution", systemGetCurrentResolution, SCRIPT_VARARGS, NULL},

			{"GetResolution", systemGetResolution, SCRIPT_VARARGS, NULL},
			{"GetFrequency", systemGetFrequency, SCRIPT_VARARGS, NULL},

			{"ApplyConfig", systemApplyConfig, SCRIPT_VARARGS, NULL},
			{"SetConfig", systemSetConfig, SCRIPT_VARARGS, NULL},
			{"SaveConfig", systemSaveConfig, SCRIPT_VARARGS, NULL},
			{"GetConfig", systemGetConfig, SCRIPT_VARARGS, NULL},

			{"SetSaveID", systemSetSaveID, SCRIPT_VARARGS, NULL},
			{"isSaveID", systemisSaveID, SCRIPT_VARARGS, NULL},
			{"GetSaveID", systemGetSaveID, SCRIPT_VARARGS, NULL},

			{"GetMusicVolume", systemGetMusicVolume, SCRIPT_VARARGS, NULL},
			{"GetSoundVolume", systemGetSoundVolume, SCRIPT_VARARGS, NULL},

			{"SetMusicVolume", systemSetMusicVolume, SCRIPT_VARARGS, NULL},
			{"SetSoundVolumef", systemSetSoundVolumef, SCRIPT_VARARGS, NULL},
			{"IsSoftwareCursor", systemIsSoftwareCursor, SCRIPT_VARARGS, NULL},

			{"SetViewChatFlag", systemSetViewChatFlag, SCRIPT_VARARGS, NULL},
			{"IsViewChat", systemIsViewChat, SCRIPT_VARARGS, NULL},

			{"SetAlwaysShowNameFlag", systemSetAlwaysShowNameFlag, SCRIPT_VARARGS, NULL},
			{"IsAlwaysShowName", systemIsAlwaysShowName, SCRIPT_VARARGS, NULL},

			{"SetShowDamageFlag", systemSetShowDamageFlag, SCRIPT_VARARGS, NULL},
			{"IsShowDamage", systemIsShowDamage, SCRIPT_VARARGS, NULL},

			{"SetShowSalesTextFlag", systemSetShowSalesTextFlag, SCRIPT_VARARGS, NULL},
			{"IsShowSalesText", systemIsShowSalesText, SCRIPT_VARARGS, NULL},

			{"GetShadowLevel", systemGetShadowLevel, SCRIPT_VARARGS, NULL},
			{"SetShadowLevel", systemSetShadowLevel, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL}};

	ScriptObject* poModule = ScriptModule_Create("systemSetting", s_methods);

	ScriptModule_AddIntConstant(poModule, "WINDOW_STATUS", CPythonSystem::WINDOW_STATUS);
	ScriptModule_AddIntConstant(poModule, "WINDOW_INVENTORY", CPythonSystem::WINDOW_INVENTORY);
	ScriptModule_AddIntConstant(poModule, "WINDOW_ABILITY", CPythonSystem::WINDOW_ABILITY);
	ScriptModule_AddIntConstant(poModule, "WINDOW_SOCIETY", CPythonSystem::WINDOW_SOCIETY);
	ScriptModule_AddIntConstant(poModule, "WINDOW_JOURNAL", CPythonSystem::WINDOW_JOURNAL);
	ScriptModule_AddIntConstant(poModule, "WINDOW_COMMAND", CPythonSystem::WINDOW_COMMAND);

	ScriptModule_AddIntConstant(poModule, "WINDOW_QUICK", CPythonSystem::WINDOW_QUICK);
	ScriptModule_AddIntConstant(poModule, "WINDOW_GAUGE", CPythonSystem::WINDOW_GAUGE);
	ScriptModule_AddIntConstant(poModule, "WINDOW_MINIMAP", CPythonSystem::WINDOW_MINIMAP);
	ScriptModule_AddIntConstant(poModule, "WINDOW_CHAT", CPythonSystem::WINDOW_CHAT);
}
