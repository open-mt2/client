#include <Client/UI/StdAfx.h>
#include <Client/UI/Locale.h>
#include <Neoeter/Platform/Locale/Japanese.h>

const char* LSS_YMIR = "YMIR";
const char* LSS_JAPAN = "JAPAN";
const char* LSS_ENGLISH = "ENGLISH";
const char* LSS_HONGKONG = "HONGKONG";
const char* LSS_TAIWAN = "TAIWAN";
const char* LSS_NEWCIBN = "NEWCIBN";
const char* LSS_EUROPE = "EUROPE";

static bool IS_CHEONMA = false;

const char* LocaleService_GetName() { return LSS_EUROPE; }
const char* LocaleService_GetLocaleName() { return "en"; }
const char* LocaleService_GetLocalePath() { return "locale/en"; }
unsigned LocaleService_GetCodePage() { return CP_LATIN; }

// CHEONMA
void LocaleService_SetCHEONMA(bool isEnable)
{
	IS_CHEONMA = isEnable;
}

bool LocaleService_IsCHEONMA()
{
	return LocaleService_IsYMIR();
}

// END_OF_CHEONMA

bool LocaleService_IsYMIR() { return (stricmp(LocaleService_GetName(), LSS_YMIR) == 0) || (stricmp(LocaleService_GetLocaleName(), "ymir") == 0); }
bool LocaleService_IsJAPAN() { return (stricmp(LocaleService_GetName(), LSS_JAPAN) == 0) || (stricmp(LocaleService_GetLocaleName(), "japan") == 0); }
bool LocaleService_IsENGLISH() { return (stricmp(LocaleService_GetName(), LSS_ENGLISH) == 0); }
bool LocaleService_IsEUROPE() { return (stricmp(LocaleService_GetName(), LSS_EUROPE) == 0); }
bool LocaleService_IsHONGKONG() { return (stricmp(LocaleService_GetName(), LSS_HONGKONG) == 0); }
bool LocaleService_IsTAIWAN() { return (stricmp(LocaleService_GetName(), LSS_TAIWAN) == 0); }
bool LocaleService_IsNEWCIBN() { return (stricmp(LocaleService_GetName(), LSS_NEWCIBN) == 0); }

#if defined(LOCALE_SERVICE_WE_JAPAN)
BOOL LocaleService_IsLeadByte(const char chByte)
{
	return ShiftJIS_IsLeadByte(chByte);
}

int LocaleService_StringCompareCI(LPCSTR szStringLeft, LPCSTR szStringRight, size_t sizeLength)
{
	return ShiftJIS_StringCompareCI(szStringLeft, szStringRight, sizeLength);
}
#else
BOOL LocaleService_IsLeadByte(const char chByte)
{
	return (((unsigned char)chByte) & 0x80) != 0;
}

int LocaleService_StringCompareCI(LPCSTR szStringLeft, LPCSTR szStringRight, size_t sizeLength)
{
	return strnicmp(szStringLeft, szStringRight, sizeLength);
}
#endif
