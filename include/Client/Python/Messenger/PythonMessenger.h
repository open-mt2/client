#pragma once
#include <Neoeter/Platform/Base/Singleton.h>
#include <set>
#include <map>
#include <Python.h>

class CPythonMessenger : public CSingleton<CPythonMessenger>
{
	public:
		typedef std::set<std::string> TFriendNameMap;
		typedef std::map<std::string, uint8_t> TGuildMemberStateMap;

		enum EMessengerGroupIndex
		{
			MESSENGER_GRUOP_INDEX_FRIEND,
			MESSENGER_GRUOP_INDEX_GUILD,
		};

	public:
		CPythonMessenger();
		virtual ~CPythonMessenger();

		void Destroy();

		// Friend
		void RemoveFriend(const char* c_szKey);
		void OnFriendLogin(const char* c_szKey);
		void OnFriendLogout(const char* c_szKey);
		int32_t IsFriendByKey(const char* c_szKey);
		int32_t IsFriendByName(const char* c_szName);

		// Guild
		void AppendGuildMember(const char* c_szName);
		void RemoveGuildMember(const char* c_szName);
		void RemoveAllGuildMember();
		void LoginGuildMember(const char* c_szName);
		void LogoutGuildMember(const char* c_szName);
		void RefreshGuildMember();

		void SetMessengerHandler(PyObject* poHandler);

	protected:
		TFriendNameMap m_FriendNameMap;
		TGuildMemberStateMap m_GuildMemberStateMap;

	private:
		PyObject* m_poMessengerHandler;
};
