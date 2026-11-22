#include <Client/Script/Bindings.h>

#include <Client/Script/ModuleInit.h>

#include <Neoeter/Python/Host/PythonModules.h>

namespace OMT2Client::Script
{
	const ModuleTable& ClientModules()
	{
		static const ModuleEntry s_modules[] = {
			{"pack", initpack},
			{"dbg", initdbg},
			{"ime", initime},
			{"grp", initgrp},
			{"grpImage", initgrpImage},
			{"grpText", initgrpText},
			{"wndMgr", initwndMgr},
			{"app", initapp},
			{"systemSetting", initsystem},
			{"chr", initchr},
			{"chrmgr", initchrmgr},
			{"player", initPlayer},
			{"item", initItem},
			{"nonplayer", initNonPlayer},
			{"exchange", initTrade},
			{"chat", initChat},
			{"textTail", initTextTail},
			{"net", initnet},
			{"miniMap", initMiniMap},
			{"profiler", initProfiler},
			{"event", initEvent},
			{"effect", initeffect},
			{"fly", initfly},
			{"snd", initsnd},
			{"eventMgr", initeventmgr},
			{"shop", initshop},
			{"skill", initskill},
			{"quest", initquest},
			{"background", initBackground},
			{"messenger", initMessenger},
			{"safebox", initsafebox},
			{"guild", initguild},
			{"ServerStateChecker", initServerStateChecker},
		};

		static const ModuleTable s_table = {
			s_modules,
			uint32_t(sizeof(s_modules) / sizeof(s_modules[0])),
		};

		return s_table;
	}
}
