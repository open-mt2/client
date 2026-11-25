#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonApplication.h>

ScriptObject* sndPlaySound(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.PlaySound2D(szFileName);
	return Script_BuildNone();
}

ScriptObject* sndPlaySound3D(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fx;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fx))
		return Script_BuildException();
	float fy;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fy))
		return Script_BuildException();
	float fz;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fz))
		return Script_BuildException();
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 3, &szFileName))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.PlaySound3D(fx, fy, fz, szFileName);
	return Script_BuildNone();
}

ScriptObject* sndPlayMusic(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.PlayMusic(szFileName);
	return Script_BuildNone();
}

ScriptObject* sndFadeInMusic(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.FadeInMusic(szFileName);
	return Script_BuildNone();
}

ScriptObject* sndFadeOutMusic(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.FadeOutMusic(szFileName);
	return Script_BuildNone();
}

ScriptObject* sndFadeOutAllMusic(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.FadeOutAllMusic();
	return Script_BuildNone();
}

ScriptObject* sndFadeLimitOutMusic(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szFileName;
	if (!ScriptTuple_GetString(poArgs, 0, &szFileName))
		return Script_BuildException();

	float fLimitVolume;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fLimitVolume))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.FadeLimitOutMusic(szFileName, fLimitVolume);
	return Script_BuildNone();
}

ScriptObject* sndStopAllSound(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.StopAllSound3D();
	return Script_BuildNone();
}

ScriptObject* sndSetMusicVolume(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fVolume;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fVolume))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.SetMusicVolume(fVolume);
	return Script_BuildNone();
}

ScriptObject* sndSetSoundVolumef(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fVolume;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fVolume))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.SetSoundVolumeRatio(fVolume);
	return Script_BuildNone();
}

ScriptObject* sndSetSoundVolume(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVolume;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVolume))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.SetSoundVolumeGrade(iVolume);
	return Script_BuildNone();
}

ScriptObject* sndSetSoundScale(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fScale;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fScale))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.SetSoundScale(fScale);
	return Script_BuildNone();
}

ScriptObject* sndSetAmbienceSoundScale(ScriptObject* poSelf, ScriptObject* poArgs)
{
	float fScale;
	if (!ScriptTuple_GetFloat(poArgs, 0, &fScale))
		return Script_BuildException();

	CSoundManager& rkSndMgr = CSoundManager::Instance();
	rkSndMgr.SetAmbienceSoundScale(fScale);
	return Script_BuildNone();
}

void initsnd()
{
	static ScriptMethodDef s_methods[] =
		{
			{"PlaySound", sndPlaySound, SCRIPT_VARARGS, NULL},
			{"PlaySound3D", sndPlaySound3D, SCRIPT_VARARGS, NULL},
			{"PlayMusic", sndPlayMusic, SCRIPT_VARARGS, NULL},
			{"FadeInMusic", sndFadeInMusic, SCRIPT_VARARGS, NULL},
			{"FadeOutMusic", sndFadeOutMusic, SCRIPT_VARARGS, NULL},
			{"FadeOutAllMusic", sndFadeOutAllMusic, SCRIPT_VARARGS, NULL},
			{"FadeLimitOutMusic", sndFadeLimitOutMusic, SCRIPT_VARARGS, NULL},
			{"StopAllSound", sndStopAllSound, SCRIPT_VARARGS, NULL},

			{"SetMusicVolumef", sndSetMusicVolume, SCRIPT_VARARGS, NULL},
			{"SetMusicVolume", sndSetMusicVolume, SCRIPT_VARARGS, NULL},
			{"SetSoundVolumef", sndSetSoundVolumef, SCRIPT_VARARGS, NULL},
			{"SetSoundVolume", sndSetSoundVolume, SCRIPT_VARARGS, NULL},
			{"SetSoundScale", sndSetSoundScale, SCRIPT_VARARGS, NULL},
			{"SetAmbienceSoundScale", sndSetAmbienceSoundScale, SCRIPT_VARARGS, NULL},
			{NULL, NULL, 0, NULL},
		};

	ScriptModule_Create("snd", s_methods);
}