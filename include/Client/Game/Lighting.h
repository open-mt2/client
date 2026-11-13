#pragma once

#include <Client/Game/MapType.h>

enum ELightingMode
{
	LIGHTING_MODE_OFF = 0,
	LIGHTING_MODE_MODELS = 1,
	LIGHTING_MODE_FULL = 2,
};

void Lighting_Initialize();
ELightingMode Lighting_GetMode();
void Lighting_SetMode(ELightingMode eMode);
bool Lighting_SetModeByName(const char* c_szMode);
const char* Lighting_GetModeName();
bool Lighting_IsEnabled();
bool Lighting_IsTerrainEnabled();
float Lighting_GetTreeFillStrength();

struct SDayNightTint
{
		D3DXCOLOR sun;
		D3DXCOLOR ambient;
		D3DXCOLOR fog;
		D3DXCOLOR sky;
		float flare;
};

void DayNight_Initialize();
bool DayNight_IsEnabled();
void DayNight_SetEnabled(bool bEnabled);
float DayNight_GetHour();
void DayNight_SetHour(float fHour);
bool DayNight_IsHourFrozen();
void DayNight_ComputeTint(float fHour, SDayNightTint& rkTint);
void DayNight_Apply(const SDayNightTint& c_rkTint, const TEnvironmentData& c_rkBase, TEnvironmentData& rkOut);
