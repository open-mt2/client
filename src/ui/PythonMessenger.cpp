#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonMessenger.h>

void CPythonMessenger::RemoveFriend(const char* c_szKey)
{
	m_FriendNameMap.erase(c_szKey);
}

void CPythonMessenger::OnFriendLogin(const char* c_szKey /*, const char * c_szName*/)
{
	m_FriendNameMap.insert(c_szKey);

	if (m_poMessengerHandler)
		Script_CallMethod(m_poMessengerHandler, "OnLogin", Script_BuildValue("(is)", MESSENGER_GRUOP_INDEX_FRIEND, c_szKey));
}

void CPythonMessenger::OnFriendLogout(const char* c_szKey)
{
	m_FriendNameMap.insert(c_szKey);

	if (m_poMessengerHandler)
		Script_CallMethod(m_poMessengerHandler, "OnLogout", Script_BuildValue("(is)", MESSENGER_GRUOP_INDEX_FRIEND, c_szKey));
}

BOOL CPythonMessenger::IsFriendByKey(const char* c_szKey)
{
	return m_FriendNameMap.end() != m_FriendNameMap.find(c_szKey);
}

BOOL CPythonMessenger::IsFriendByName(const char* c_szName)
{
	return IsFriendByKey(c_szName);
}

void CPythonMessenger::AppendGuildMember(const char* c_szName)
{
	if (m_GuildMemberStateMap.end() != m_GuildMemberStateMap.find(c_szName))
		return;

	LogoutGuildMember(c_szName);
}

void CPythonMessenger::RemoveGuildMember(const char* c_szName)
{
	m_GuildMemberStateMap.erase(c_szName);

	if (m_poMessengerHandler)
		Script_CallMethod(m_poMessengerHandler, "OnRemoveList", Script_BuildValue("(is)", MESSENGER_GRUOP_INDEX_GUILD, c_szName));
}

void CPythonMessenger::RemoveAllGuildMember()
{
	m_GuildMemberStateMap.clear();

	if (m_poMessengerHandler)
		Script_CallMethod(m_poMessengerHandler, "OnRemoveAllList", Script_BuildValue("(i)", MESSENGER_GRUOP_INDEX_GUILD));
}

void CPythonMessenger::LoginGuildMember(const char* c_szName)
{
	m_GuildMemberStateMap[c_szName] = 1;
	if (m_poMessengerHandler)
		Script_CallMethod(m_poMessengerHandler, "OnLogin", Script_BuildValue("(is)", MESSENGER_GRUOP_INDEX_GUILD, c_szName));
}

void CPythonMessenger::LogoutGuildMember(const char* c_szName)
{
	m_GuildMemberStateMap[c_szName] = 0;
	if (m_poMessengerHandler)
		Script_CallMethod(m_poMessengerHandler, "OnLogout", Script_BuildValue("(is)", MESSENGER_GRUOP_INDEX_GUILD, c_szName));
}

void CPythonMessenger::RefreshGuildMember()
{
	for (TGuildMemberStateMap::iterator itor = m_GuildMemberStateMap.begin(); itor != m_GuildMemberStateMap.end(); ++itor)
	{
		if (itor->second)
			Script_CallMethod(m_poMessengerHandler, "OnLogin", Script_BuildValue("(is)", MESSENGER_GRUOP_INDEX_GUILD, (itor->first).c_str()));
		else
			Script_CallMethod(m_poMessengerHandler, "OnLogout", Script_BuildValue("(is)", MESSENGER_GRUOP_INDEX_GUILD, (itor->first).c_str()));
	}
}

void CPythonMessenger::Destroy()
{
	m_FriendNameMap.clear();
	m_GuildMemberStateMap.clear();
}

void CPythonMessenger::SetMessengerHandler(ScriptObject* poHandler)
{
	m_poMessengerHandler = poHandler;
}

CPythonMessenger::CPythonMessenger()
	: m_poMessengerHandler(NULL)
{
}

CPythonMessenger::~CPythonMessenger()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptObject* messengerRemoveFriend(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szKey;
	if (!ScriptTuple_GetString(poArgs, 0, &szKey))
		return Script_BuildException();

	CPythonMessenger::Instance().RemoveFriend(szKey);
	return Script_BuildNone();
}

ScriptObject* messengerIsFriendByName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 0, &szName))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonMessenger::Instance().IsFriendByName(szName));
}

ScriptObject* messengerDestroy(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMessenger::Instance().Destroy();
	return Script_BuildNone();
}

ScriptObject* messengerRefreshGuildMember(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonMessenger::Instance().RefreshGuildMember();
	return Script_BuildNone();
}

ScriptObject* messengerSetMessengerHandler(ScriptObject* poSelf, ScriptObject* poArgs)
{
	ScriptObject* poEventHandler;
	if (!ScriptTuple_GetObject(poArgs, 0, &poEventHandler))
		return Script_BuildException();

	CPythonMessenger::Instance().SetMessengerHandler(poEventHandler);
	return Script_BuildNone();
}

void initMessenger()
{
	static ScriptMethodDef s_methods[] =
		{
			{"RemoveFriend", messengerRemoveFriend, SCRIPT_VARARGS, NULL},
			{"IsFriendByName", messengerIsFriendByName, SCRIPT_VARARGS, NULL},
			{"Destroy", messengerDestroy, SCRIPT_VARARGS, NULL},
			{"RefreshGuildMember", messengerRefreshGuildMember, SCRIPT_VARARGS, NULL},
			{"SetMessengerHandler", messengerSetMessengerHandler, SCRIPT_VARARGS, NULL},
			{NULL, NULL, 0, NULL},
		};

	ScriptModule_Create("messenger", s_methods);
}
