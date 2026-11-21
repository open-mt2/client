#pragma once

#include <Neoeter/Render/StdAfx.h>
#include <Neoeter/Python/Bindings/StdAfx.h>
#include <Client/Game/StdAfx.h>
#include <Neoeter/Python/Host/StdAfx.h>
#include <Neoeter/Audio/StdAfx.h>
#include <Neoeter/Effect/StdAfx.h>
#include <Neoeter/Terrain/StdAfx.h>
#include <Neoeter/Model/SpeedTree/StdAfx.h>

#ifndef __D3DRM_H__
#define __D3DRM_H__
#endif

#include <Client/UI/Locale.h>

#include <Client/UI/GameType.h>
extern uint32_t __DEFAULT_CODE_PAGE__;

#define APP_NAME "Metin 2"

enum
{
	POINT_MAX_NUM = 255,
	CHARACTER_NAME_MAX_LEN = 24,
#if defined(LOCALE_SERVICE_JAPAN)
	PLAYER_NAME_MAX_LEN = 16,
#else
	PLAYER_NAME_MAX_LEN = 12,
#endif
};

void initapp();
void initime();
void initsystem();
void initchr();
void initchrmgr();
void initChat();
void initTextTail();
void initime();
void initItem();
void initNonPlayer();
void initnet();
void initPlayer();
void initSectionDisplayer();
void initServerStateChecker();
void initTrade();
void initMiniMap();
void initProfiler();
void initEvent();
void initeffect();
void initsnd();
void initeventmgr();
void initBackground();
void initwndMgr();
void initshop();
void initpack();
void initskill();
void initfly();
void initquest();
void initsafebox();
void initguild();
void initMessenger();

extern void ApplicationSetErrorString(const char* szErrorString);
