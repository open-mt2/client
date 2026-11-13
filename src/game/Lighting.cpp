#include <Client/Game/StdAfx.h>
#include <Client/Game/Lighting.h>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

namespace
{
	bool gs_lightingInitialized = false;
	ELightingMode gs_lightingMode = LIGHTING_MODE_MODELS;
	float gs_treeFillStrength = 0.35f;

	bool gs_dayNightInitialized = false;
	bool gs_dayNightEnabled = true;
	bool gs_hourFrozen = false;
	float gs_fixedHour = 12.0f;

	float Clamp01(float value)
	{
		if (value < 0.0f) return 0.0f;
		if (value > 1.0f) return 1.0f;
		return value;
	}

	void MultiplyColor(D3DXCOLOR& color, const float multiplier[3])
	{
		color.r = Clamp01(color.r * multiplier[0]);
		color.g = Clamp01(color.g * multiplier[1]);
		color.b = Clamp01(color.b * multiplier[2]);
	}

	void MultiplyColor(D3DCOLORVALUE& color, const float multiplier[3])
	{
		color.r = Clamp01(color.r * multiplier[0]);
		color.g = Clamp01(color.g * multiplier[1]);
		color.b = Clamp01(color.b * multiplier[2]);
	}

	void MultiplyColor(TColor& color, const float multiplier[3])
	{
		color.r = Clamp01(color.r * multiplier[0]);
		color.g = Clamp01(color.g * multiplier[1]);
		color.b = Clamp01(color.b * multiplier[2]);
	}

	struct TDayNightKey
	{
			float hour;
			float sun[3];
			float ambient[3];
			float fog[3];
			float sky[3];
			float flare;
	};

	const TDayNightKey s_dayNightKeys[] = {
		{0.0f, {0.55f, 0.58f, 0.72f}, {0.65f, 0.65f, 0.72f}, {0.42f, 0.46f, 0.58f}, {0.32f, 0.36f, 0.50f}, 0.0f},
		{4.5f, {0.55f, 0.58f, 0.72f}, {0.65f, 0.65f, 0.72f}, {0.42f, 0.46f, 0.58f}, {0.32f, 0.36f, 0.50f}, 0.0f},
		{6.0f, {1.00f, 0.68f, 0.45f}, {0.85f, 0.72f, 0.62f}, {1.00f, 0.72f, 0.50f}, {1.00f, 0.62f, 0.42f}, 0.7f},
		{7.5f, {1.00f, 0.95f, 0.90f}, {1.00f, 0.96f, 0.92f}, {1.00f, 0.96f, 0.92f}, {1.00f, 0.96f, 0.92f}, 1.0f},
		{12.0f, {1.00f, 1.00f, 1.00f}, {1.00f, 1.00f, 1.00f}, {1.00f, 1.00f, 1.00f}, {1.00f, 1.00f, 1.00f}, 1.0f},
		{16.5f, {1.00f, 0.95f, 0.90f}, {1.00f, 0.96f, 0.92f}, {1.00f, 0.96f, 0.92f}, {1.00f, 0.96f, 0.92f}, 1.0f},
		{18.0f, {1.00f, 0.70f, 0.45f}, {0.90f, 0.74f, 0.62f}, {1.00f, 0.70f, 0.48f}, {1.00f, 0.58f, 0.40f}, 0.7f},
		{19.5f, {0.65f, 0.60f, 0.70f}, {0.75f, 0.70f, 0.78f}, {0.55f, 0.52f, 0.62f}, {0.45f, 0.43f, 0.55f}, 0.15f},
		{21.0f, {0.55f, 0.58f, 0.72f}, {0.65f, 0.65f, 0.72f}, {0.42f, 0.46f, 0.58f}, {0.32f, 0.36f, 0.50f}, 0.0f},
		{24.0f, {0.55f, 0.58f, 0.72f}, {0.65f, 0.65f, 0.72f}, {0.42f, 0.46f, 0.58f}, {0.32f, 0.36f, 0.50f}, 0.0f},
	};

	const int s_dayNightKeyCount = static_cast<int>(sizeof(s_dayNightKeys) / sizeof(s_dayNightKeys[0]));

	void EnsureLightingInitialized()
	{
		if (gs_lightingInitialized)
			return;

		gs_lightingInitialized = true;

		const char* mode = std::getenv("OPENMT2_LIGHTING");
		if (mode && !Lighting_SetModeByName(mode))
			TraceError("OPENMT2_LIGHTING has an unknown value '%s' (expected off, models or full)", mode);

		const char* treeFill = std::getenv("OPENMT2_TREE_FILL");
		if (treeFill)
		{
			gs_treeFillStrength = static_cast<float>(std::atof(treeFill));
			if (gs_treeFillStrength < 0.0f) gs_treeFillStrength = 0.0f;
			if (gs_treeFillStrength > 2.0f) gs_treeFillStrength = 2.0f;
		}
	}

	void EnsureDayNightInitialized()
	{
		if (gs_dayNightInitialized)
			return;

		gs_dayNightInitialized = true;

		const char* enabled = std::getenv("OPENMT2_DAYNIGHT");
		if (enabled)
			gs_dayNightEnabled = !(std::strcmp(enabled, "0") == 0 || std::strcmp(enabled, "false") == 0 || std::strcmp(enabled, "off") == 0);

		const char* hour = std::getenv("OPENMT2_TIME");
		if (hour)
		{
			gs_fixedHour = static_cast<float>(std::atof(hour));
			gs_hourFrozen = true;
		}
	}
}

void Lighting_Initialize()
{
	EnsureLightingInitialized();
}

ELightingMode Lighting_GetMode()
{
	EnsureLightingInitialized();
	return gs_lightingMode;
}

void Lighting_SetMode(ELightingMode eMode)
{
	gs_lightingMode = eMode;
}

bool Lighting_SetModeByName(const char* c_szMode)
{
	if (!c_szMode)
		return false;

	if (std::strcmp(c_szMode, "off") == 0)
		gs_lightingMode = LIGHTING_MODE_OFF;
	else if (std::strcmp(c_szMode, "models") == 0)
		gs_lightingMode = LIGHTING_MODE_MODELS;
	else if (std::strcmp(c_szMode, "full") == 0)
		gs_lightingMode = LIGHTING_MODE_FULL;
	else
		return false;

	return true;
}

const char* Lighting_GetModeName()
{
	switch (Lighting_GetMode())
	{
		case LIGHTING_MODE_OFF:
			return "off";
		case LIGHTING_MODE_FULL:
			return "full";
		default:
			return "models";
	}
}

bool Lighting_IsEnabled()
{
	return Lighting_GetMode() != LIGHTING_MODE_OFF;
}

bool Lighting_IsTerrainEnabled()
{
	return Lighting_GetMode() == LIGHTING_MODE_FULL;
}

float Lighting_GetTreeFillStrength()
{
	EnsureLightingInitialized();
	return gs_treeFillStrength;
}

void DayNight_Initialize()
{
	EnsureDayNightInitialized();
}

bool DayNight_IsEnabled()
{
	EnsureDayNightInitialized();
	return gs_dayNightEnabled;
}

void DayNight_SetEnabled(bool bEnabled)
{
	gs_dayNightEnabled = bEnabled;
}

float DayNight_GetHour()
{
	EnsureDayNightInitialized();

	if (gs_hourFrozen)
		return gs_fixedHour;

	std::time_t now = std::time(nullptr);
	std::tm* local = std::localtime(&now);
	if (!local)
		return 12.0f;

	return static_cast<float>(local->tm_hour) +
		   static_cast<float>(local->tm_min) / 60.0f +
		   static_cast<float>(local->tm_sec) / 3600.0f;
}

void DayNight_SetHour(float fHour)
{
	while (fHour < 0.0f)
		fHour += 24.0f;
	while (fHour >= 24.0f)
		fHour -= 24.0f;

	gs_fixedHour = fHour;
	gs_hourFrozen = true;
}

bool DayNight_IsHourFrozen()
{
	EnsureDayNightInitialized();
	return gs_hourFrozen;
}

void DayNight_ComputeTint(float fHour, SDayNightTint& rkTint)
{
	while (fHour < 0.0f)
		fHour += 24.0f;
	while (fHour >= 24.0f)
		fHour -= 24.0f;

	const TDayNightKey* pFirst = &s_dayNightKeys[0];
	const TDayNightKey* pSecond = &s_dayNightKeys[s_dayNightKeyCount - 1];

	for (int i = 0; i < s_dayNightKeyCount - 1; ++i)
	{
		if (fHour >= s_dayNightKeys[i].hour && fHour <= s_dayNightKeys[i + 1].hour)
		{
			pFirst = &s_dayNightKeys[i];
			pSecond = &s_dayNightKeys[i + 1];
			break;
		}
	}

	const float span = pSecond->hour - pFirst->hour;
	const float weight = (span > 0.0f) ? (fHour - pFirst->hour) / span : 0.0f;

	rkTint.sun.r = pFirst->sun[0] + (pSecond->sun[0] - pFirst->sun[0]) * weight;
	rkTint.sun.g = pFirst->sun[1] + (pSecond->sun[1] - pFirst->sun[1]) * weight;
	rkTint.sun.b = pFirst->sun[2] + (pSecond->sun[2] - pFirst->sun[2]) * weight;
	rkTint.sun.a = 1.0f;

	rkTint.ambient.r = pFirst->ambient[0] + (pSecond->ambient[0] - pFirst->ambient[0]) * weight;
	rkTint.ambient.g = pFirst->ambient[1] + (pSecond->ambient[1] - pFirst->ambient[1]) * weight;
	rkTint.ambient.b = pFirst->ambient[2] + (pSecond->ambient[2] - pFirst->ambient[2]) * weight;
	rkTint.ambient.a = 1.0f;

	rkTint.fog.r = pFirst->fog[0] + (pSecond->fog[0] - pFirst->fog[0]) * weight;
	rkTint.fog.g = pFirst->fog[1] + (pSecond->fog[1] - pFirst->fog[1]) * weight;
	rkTint.fog.b = pFirst->fog[2] + (pSecond->fog[2] - pFirst->fog[2]) * weight;
	rkTint.fog.a = 1.0f;

	rkTint.sky.r = pFirst->sky[0] + (pSecond->sky[0] - pFirst->sky[0]) * weight;
	rkTint.sky.g = pFirst->sky[1] + (pSecond->sky[1] - pFirst->sky[1]) * weight;
	rkTint.sky.b = pFirst->sky[2] + (pSecond->sky[2] - pFirst->sky[2]) * weight;
	rkTint.sky.a = 1.0f;

	rkTint.flare = pFirst->flare + (pSecond->flare - pFirst->flare) * weight;
}

void DayNight_Apply(const SDayNightTint& c_rkTint, const TEnvironmentData& c_rkBase, TEnvironmentData& rkOut)
{
	rkOut = c_rkBase;

	const float sun[3] = {c_rkTint.sun.r, c_rkTint.sun.g, c_rkTint.sun.b};
	const float ambient[3] = {c_rkTint.ambient.r, c_rkTint.ambient.g, c_rkTint.ambient.b};
	const float fog[3] = {c_rkTint.fog.r, c_rkTint.fog.g, c_rkTint.fog.b};
	const float sky[3] = {c_rkTint.sky.r, c_rkTint.sky.g, c_rkTint.sky.b};

	for (int i = 0; i < ENV_DIRLIGHT_NUM; ++i)
	{
		MultiplyColor(rkOut.DirLights[i].Diffuse, sun);
		MultiplyColor(rkOut.DirLights[i].Ambient, ambient);
	}

	if (!Lighting_IsTerrainEnabled())
		MultiplyColor(rkOut.Material.Diffuse, sun);

	MultiplyColor(rkOut.FogColor, fog);

	for (size_t i = 0; i < rkOut.SkyBoxGradientColorVector.size(); ++i)
	{
		MultiplyColor(rkOut.SkyBoxGradientColorVector[i].m_FirstColor, sky);
		MultiplyColor(rkOut.SkyBoxGradientColorVector[i].m_SecondColor, sky);
	}

	MultiplyColor(rkOut.CloudGradientColor.m_FirstColor, sky);
	MultiplyColor(rkOut.CloudGradientColor.m_SecondColor, sky);

	MultiplyColor(rkOut.LensFlareBrightnessColor, sun);
	rkOut.fLensFlareMaxBrightness *= c_rkTint.flare;
	if (c_rkTint.flare < 0.01f)
		rkOut.bLensFlareEnable = FALSE;
}
