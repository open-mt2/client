#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonChat.h>
#include <Client/UI/PythonItem.h>
#include <Client/Game/ItemManager.h>

ScriptObject* chatSetChatColor(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iType))
		return Script_BuildException();

	int r;
	if (!ScriptTuple_GetInteger(poArgs, 1, &r))
		return Script_BuildException();

	int g;
	if (!ScriptTuple_GetInteger(poArgs, 2, &g))
		return Script_BuildException();

	int b;
	if (!ScriptTuple_GetInteger(poArgs, 3, &b))
		return Script_BuildException();

	CPythonChat::Instance().SetChatColor(iType, r, g, b);
	return Script_BuildNone();
}

ScriptObject* chatClear(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonChat::Instance().Destroy();
	return Script_BuildNone();
}

ScriptObject* chatClose(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonChat::Instance().Close();
	return Script_BuildNone();
}

ScriptObject* chatCreateChatSet(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonChat::Instance().CreateChatSet(iID));
}

ScriptObject* chatUpdate(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	CPythonChat::Instance().Update(iID);
	return Script_BuildNone();
}

ScriptObject* chatRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	CPythonChat::Instance().Render(iID);
	return Script_BuildNone();
}

ScriptObject* chatSetBoardState(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	int iState;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iState))
		return Script_BuildException();

	CPythonChat::Instance().SetBoardState(iID, iState);

	return Script_BuildNone();
}

ScriptObject* chatSetPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	int ix;
	if (!ScriptTuple_GetInteger(poArgs, 1, &ix))
		return Script_BuildException();
	int iy;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iy))
		return Script_BuildException();

	CPythonChat::Instance().SetPosition(iID, ix, iy);

	return Script_BuildNone();
}

ScriptObject* chatSetHeight(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	int iHeight;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iHeight))
		return Script_BuildException();

	CPythonChat::Instance().SetHeight(iID, iHeight);

	return Script_BuildNone();
}

ScriptObject* chatSetStep(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	int iStep;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iStep))
		return Script_BuildException();

	CPythonChat::Instance().SetStep(iID, iStep);

	return Script_BuildNone();
}

ScriptObject* chatToggleChatMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iType))
		return Script_BuildException();

	CPythonChat::Instance().ToggleChatMode(iID, iType);

	return Script_BuildNone();
}

ScriptObject* chatEnableChatMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iType))
		return Script_BuildException();

	CPythonChat::Instance().EnableChatMode(iID, iType);

	return Script_BuildNone();
}

ScriptObject* chatDisableChatMode(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iType))
		return Script_BuildException();

	CPythonChat::Instance().DisableChatMode(iID, iType);

	return Script_BuildNone();
}

ScriptObject* chatSetEndPos(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();
	float fPos;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fPos))
		return Script_BuildException();

	CPythonChat::Instance().SetEndPos(iID, fPos);

	return Script_BuildNone();
}

ScriptObject* chatGetLineCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonChat::Instance().GetLineCount(iID));
}

ScriptObject* chatGetVisibleLineCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonChat::Instance().GetVisibleLineCount(iID));
}

ScriptObject* chatGetLineStep(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonChat::Instance().GetLineStep(iID));
}

ScriptObject* chatAppendChat(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iType))
		return Script_BuildException();

	char* szChat;
	if (!ScriptTuple_GetString(poArgs, 1, &szChat))
		return Script_BuildException();

	CPythonChat::Instance().AppendChat(iType, szChat);

	return Script_BuildNone();
}

ScriptObject* chatAppendChatWithDelay(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iType))
		return Script_BuildException();

	char* szChat;
	if (!ScriptTuple_GetString(poArgs, 1, &szChat))
		return Script_BuildException();

	int iDelay;
	if (!ScriptTuple_GetInteger(poArgs, 2, &iDelay))
		return Script_BuildException();

	CPythonChat::Instance().AppendChatWithDelay(iType, szChat, iDelay);

	return Script_BuildNone();
}

ScriptObject* chatArrangeShowingChat(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iID))
		return Script_BuildException();

	CPythonChat::Instance().ArrangeShowingChat(iID);

	return Script_BuildNone();
}

ScriptObject* chatIgnoreCharacter(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonChat::Instance().IgnoreCharacter(szName);

	return Script_BuildNone();
}

ScriptObject* chatIsIgnoreCharacter(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonChat::Instance().IsIgnoreCharacter(szName);

	return Script_BuildNone();
}

ScriptObject* chatCreateWhisper(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonChat::Instance().CreateWhisper(szName);

	return Script_BuildNone();
}

ScriptObject* chatAppendWhisper(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iType;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iType))
		return Script_BuildException();

	char* szName;
	if (!ScriptTuple_GetString(poArgs, 1, &szName))
		return Script_BuildException();

	char* szChat;
	if (!ScriptTuple_GetString(poArgs, 2, &szChat))
		return Script_BuildException();

	CPythonChat::Instance().AppendWhisper(iType, szName, szChat);
	return Script_BuildNone();
}

ScriptObject* chatRenderWhisper(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	float fx;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fx))
		return Script_BuildException();

	float fy;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fy))
		return Script_BuildException();

	CWhisper* pWhisper;
	if (CPythonChat::Instance().GetWhisper(szName, &pWhisper))
	{
		pWhisper->Render(fx, fy);
	}

	return Script_BuildNone();
}

ScriptObject* chatSetWhisperBoxSize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	float fWidth;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fWidth))
		return Script_BuildException();

	float fHeight;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fHeight))
		return Script_BuildException();

	CWhisper* pWhisper;
	if (CPythonChat::Instance().GetWhisper(szName, &pWhisper))
	{
		pWhisper->SetBoxSize(fWidth, fHeight);
	}

	return Script_BuildNone();
}

ScriptObject* chatSetWhisperPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	float fPosition;
	if (!ScriptTuple_GetFloat(poArgs, 1, &fPosition))
		return Script_BuildException();

	CWhisper* pWhisper;
	if (CPythonChat::Instance().GetWhisper(szName, &pWhisper))
	{
		pWhisper->SetPosition(fPosition);
	}

	return Script_BuildNone();
}

ScriptObject* chatClearWhisper(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	CPythonChat::Instance().ClearWhisper(szName);

	return Script_BuildNone();
}

ScriptObject* chatInitWhisper(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poInterface;
	if (!ScriptTuple_GetObject(poArgs, 0, &poInterface))
		return Script_BuildException();

	CPythonChat::Instance().InitWhisper(poInterface);
	return Script_BuildNone();
}

ScriptObject* chatGetLinkFromHyperlink(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szHyperlink;

	if (!ScriptTuple_GetString(poArgs, 0, &szHyperlink))
		return Script_BuildException();

	std::string stHyperlink(szHyperlink);
	std::vector<std::string> results;

	split_string(stHyperlink, ":", results, false);

	// item:vnum:flag:socket0:socket1:socket2
	if (0 == results[0].compare("item"))
	{
		if (results.size() < 6)
			return Script_BuildValue("s", "");

		CItemData* pItemData = NULL;

		if (CItemManager::Instance().GetItemDataPointer(htoi(results[1].c_str()), &pItemData))
		{
			char buf[1024] = {0};
			char itemlink[256];
			int len;
			bool isAttr = false;

			len = snprintf(itemlink, sizeof(itemlink), "item:%x:%x:%x:%x:%x",
						   htoi(results[1].c_str()),
						   htoi(results[2].c_str()),
						   htoi(results[3].c_str()),
						   htoi(results[4].c_str()),
						   htoi(results[5].c_str()));

			if (results.size() >= 8)
			{
				for (int i = 6; i < results.size(); i += 2)
				{
					len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%x:%d",
									htoi(results[i].c_str()),
									atoi(results[i + 1].c_str()));
					isAttr = true;
				}
			}

			if (isAttr)
				//"item:번호:플래그:소켓0:소켓1:소켓2"
				snprintf(buf, sizeof(buf), "|cffffc700|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
			else
				snprintf(buf, sizeof(buf), "|cfff1e6c0|H%s|h[%s]|h|r", itemlink, pItemData->GetName());

			return Script_BuildValue("s", buf);
		}
	}

	return Script_BuildValue("s", "");
}

void initChat()
{
	static ScriptMethodDef s_methods[] =
		{
			{"SetChatColor", chatSetChatColor, SCRIPT_VARARGS, NULL},
			{"Clear", chatClear, SCRIPT_VARARGS, NULL},
			{"Close", chatClose, SCRIPT_VARARGS, NULL},

			{"CreateChatSet", chatCreateChatSet, SCRIPT_VARARGS, NULL},
			{"Update", chatUpdate, SCRIPT_VARARGS, NULL},
			{"Render", chatRender, SCRIPT_VARARGS, NULL},

			{"SetBoardState", chatSetBoardState, SCRIPT_VARARGS, NULL},
			{"SetPosition", chatSetPosition, SCRIPT_VARARGS, NULL},
			{"SetHeight", chatSetHeight, SCRIPT_VARARGS, NULL},
			{"SetStep", chatSetStep, SCRIPT_VARARGS, NULL},
			{"ToggleChatMode", chatToggleChatMode, SCRIPT_VARARGS, NULL},
			{"EnableChatMode", chatEnableChatMode, SCRIPT_VARARGS, NULL},
			{"DisableChatMode", chatDisableChatMode, SCRIPT_VARARGS, NULL},
			{"SetEndPos", chatSetEndPos, SCRIPT_VARARGS, NULL},

			{"GetLineCount", chatGetLineCount, SCRIPT_VARARGS, NULL},
			{"GetVisibleLineCount", chatGetVisibleLineCount, SCRIPT_VARARGS, NULL},
			{"GetLineStep", chatGetLineStep, SCRIPT_VARARGS, NULL},

			// Chat
			{"AppendChat", chatAppendChat, SCRIPT_VARARGS, NULL},
			{"AppendChatWithDelay", chatAppendChatWithDelay, SCRIPT_VARARGS, NULL},
			{"ArrangeShowingChat", chatArrangeShowingChat, SCRIPT_VARARGS, NULL},

			// Ignore
			{"IgnoreCharacter", chatIgnoreCharacter, SCRIPT_VARARGS, NULL},
			{"IsIgnoreCharacter", chatIsIgnoreCharacter, SCRIPT_VARARGS, NULL},

			// Whisper
			{"CreateWhisper", chatCreateWhisper, SCRIPT_VARARGS, NULL},
			{"AppendWhisper", chatAppendWhisper, SCRIPT_VARARGS, NULL},
			{"RenderWhisper", chatRenderWhisper, SCRIPT_VARARGS, NULL},
			{"SetWhisperBoxSize", chatSetWhisperBoxSize, SCRIPT_VARARGS, NULL},
			{"SetWhisperPosition", chatSetWhisperPosition, SCRIPT_VARARGS, NULL},
			{"ClearWhisper", chatClearWhisper, SCRIPT_VARARGS, NULL},
			{"InitWhisper", chatInitWhisper, SCRIPT_VARARGS, NULL},

			// Link
			{"GetLinkFromHyperlink", chatGetLinkFromHyperlink, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("chat", s_methods);

	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_TALKING", CHAT_TYPE_TALKING);
	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_INFO", CHAT_TYPE_INFO);
	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_NOTICE", CHAT_TYPE_NOTICE);
	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_PARTY", CHAT_TYPE_PARTY);
	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_GUILD", CHAT_TYPE_GUILD);
	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_COMMAND", CHAT_TYPE_COMMAND);
	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_SHOUT", CHAT_TYPE_SHOUT);
	ScriptModule_AddIntConstant(poModule, "CHAT_TYPE_WHISPER", CHAT_TYPE_WHISPER);
	ScriptModule_AddIntConstant(poModule, "WHISPER_TYPE_CHAT", CPythonChat::WHISPER_TYPE_CHAT);
	ScriptModule_AddIntConstant(poModule, "WHISPER_TYPE_SYSTEM", CPythonChat::WHISPER_TYPE_SYSTEM);
	ScriptModule_AddIntConstant(poModule, "WHISPER_TYPE_GM", CPythonChat::WHISPER_TYPE_GM);

	ScriptModule_AddIntConstant(poModule, "BOARD_STATE_VIEW", CPythonChat::BOARD_STATE_VIEW);
	ScriptModule_AddIntConstant(poModule, "BOARD_STATE_EDIT", CPythonChat::BOARD_STATE_EDIT);
	ScriptModule_AddIntConstant(poModule, "BOARD_STATE_LOG", CPythonChat::BOARD_STATE_LOG);

	ScriptModule_AddIntConstant(poModule, "CHAT_SET_CHAT_WINDOW", 0);
	ScriptModule_AddIntConstant(poModule, "CHAT_SET_LOG_WINDOW", 1);
}
