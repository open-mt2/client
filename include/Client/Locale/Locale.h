#pragma once

#include <Neoeter/Platform/Locale/CodePageId.h>

#define ENABLE_COSTUME_SYSTEM
#define ENABLE_ENERGY_SYSTEM
#define ENABLE_DRAGON_SOUL_SYSTEM
#define ENABLE_NEW_EQUIPMENT_SYSTEM

inline unsigned LocaleService_GetCodePage() { return CP_LATIN; }
inline const char* LocaleService_GetName() { return "EUROPE"; }
inline const char* LocaleService_GetLocaleName() { return "en"; }
inline const char* LocaleService_GetLocalePath() { return "locale/en"; }

inline void LocaleService_SetCHEONMA(bool) {}
inline bool LocaleService_IsCHEONMA() { return false; }
