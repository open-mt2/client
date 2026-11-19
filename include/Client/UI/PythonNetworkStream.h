#pragma once

#include <Neoeter/Render/FuncObject.h>
#include <Neoeter/Render/NetStream.h>
#include <Neoeter/Render/NetPacketHeaderMap.h>

#include <Client/UI/InsultChecker.h>

#include <Client/UI/Packet.h>
#include <Client/Protocol/Decode.h>

class CInstanceBase;
class CNetworkActorManager;
struct SNetworkActorData;
struct SNetworkUpdateActorData;

class CPythonNetworkStream : public CNetworkStream, public CSingleton<CPythonNetworkStream>
{
	public:
		enum
		{
			SERVER_COMMAND_LOG_OUT = 0,
			SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER = 1,
			SERVER_COMMAND_QUIT = 2,

			MAX_ACCOUNT_PLAYER
		};

		enum
		{
			ERROR_NONE,
			ERROR_UNKNOWN,
			ERROR_CONNECT_MARK_SERVER,
			ERROR_LOAD_MARK,
			ERROR_MARK_WIDTH,
			ERROR_MARK_HEIGHT,

			// MARK_BUG_FIX
			ERROR_MARK_UPLOAD_NEED_RECONNECT,
			ERROR_MARK_CHECK_NEED_RECONNECT,
			// END_OF_MARK_BUG_FIX
		};

		enum
		{
			ACCOUNT_CHARACTER_SLOT_ID,
			ACCOUNT_CHARACTER_SLOT_NAME,
			ACCOUNT_CHARACTER_SLOT_RACE,
			ACCOUNT_CHARACTER_SLOT_LEVEL,
			ACCOUNT_CHARACTER_SLOT_STR,
			ACCOUNT_CHARACTER_SLOT_DEX,
			ACCOUNT_CHARACTER_SLOT_HTH,
			ACCOUNT_CHARACTER_SLOT_INT,
			ACCOUNT_CHARACTER_SLOT_PLAYTIME,
			ACCOUNT_CHARACTER_SLOT_FORM,
			ACCOUNT_CHARACTER_SLOT_ADDR,
			ACCOUNT_CHARACTER_SLOT_PORT,
			ACCOUNT_CHARACTER_SLOT_GUILD_ID,
			ACCOUNT_CHARACTER_SLOT_GUILD_NAME,
			ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG,
			ACCOUNT_CHARACTER_SLOT_HAIR,
		};

		enum
		{
			PHASE_WINDOW_LOGO,
			PHASE_WINDOW_LOGIN,
			PHASE_WINDOW_SELECT,
			PHASE_WINDOW_CREATE,
			PHASE_WINDOW_LOAD,
			PHASE_WINDOW_GAME,
			PHASE_WINDOW_EMPIRE,
			PHASE_WINDOW_NUM,
		};

	public:
		CPythonNetworkStream();
		virtual ~CPythonNetworkStream();

		bool SendSpecial(int nLen, void* pvBuf);

		void StartGame();
		void Warp(LONG lGlobalX, LONG lGlobalY);

		void NotifyHack(const char* c_szMsg);
		void SetWaitFlag();

		void SendEmoticon(UINT eEmoticon);

		void ExitApplication();
		void ExitGame();
		void LogOutGame();
		void AbsoluteExitGame();
		void AbsoluteExitApplication();

		void EnableChatInsultFilter(bool isEnable);
		bool IsChatInsultIn(const char* c_szMsg);
		bool IsInsultIn(const char* c_szMsg);

		DWORD GetGuildID();

		UINT UploadMark(const char* c_szImageFileName);
		UINT UploadSymbol(const char* c_szImageFileName);

		bool LoadInsultList(const char* c_szInsultListFileName);
		bool LoadConvertTable(DWORD dwEmpireID, const char* c_szFileName);

		UINT GetAccountCharacterSlotDatau(UINT iSlot, UINT eType);
		const char* GetAccountCharacterSlotDataz(UINT iSlot, UINT eType);

		// SUPPORT_BGM
		const char* GetFieldMusicFileName();
		float GetFieldMusicVolume();
		// END_OF_SUPPORT_BGM

		bool IsSelectedEmpire();

		void ToggleGameDebugInfo();

		void SetMarkServer(const char* c_szAddr, UINT uPort);
		void ConnectLoginServer(const char* c_szAddr, UINT uPort);
		void ConnectGameServer(UINT iChrSlot);

		void SetLoginInfo(const char* c_szID, const char* c_szPassword);
		void SetLoginKey(DWORD dwLoginKey);
		void ClearLoginInfo(void);

		void SetHandler(PyObject* poHandler);
		void SetPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void ClearPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void SetServerCommandParserWindow(PyObject* poPhaseWnd);

		bool SendSyncPositionElementPacket(DWORD dwVictimVID, DWORD dwVictimX, DWORD dwVictimY);

		bool SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim);
		bool SendCharacterStatePacket(const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg);
		bool SendUseSkillPacket(DWORD dwSkillIndex, DWORD dwTargetVID = 0);
		bool SendTargetPacket(DWORD dwVID);

		// OLDCODE:
		bool SendCharacterStartWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterEndWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterCheckWalkingPacket(float fRotation, long lx, long ly);

		bool SendCharacterPositionPacket(BYTE iPosition);

		bool SendItemUsePacket(TItemPos pos);
		bool SendItemUseToItemPacket(TItemPos source_pos, TItemPos target_pos);
		bool SendItemDropPacket(TItemPos pos, DWORD elk);
		bool SendItemDropPacketNew(TItemPos pos, DWORD elk, DWORD count);
		bool SendItemMovePacket(TItemPos pos, TItemPos change_pos, BYTE num);
		bool SendItemPickUpPacket(DWORD vid);

		bool SendQuickSlotAddPacket(BYTE wpos, BYTE type, BYTE pos);
		bool SendQuickSlotDelPacket(BYTE wpos);
		bool SendQuickSlotMovePacket(BYTE wpos, BYTE change_pos);

		// PointReset 개 임시
		bool SendPointResetPacket();

		// Shop
		bool SendShopEndPacket();
		bool SendShopBuyPacket(BYTE byCount);
		bool SendShopSellPacket(BYTE bySlot);
		bool SendShopSellPacketNew(BYTE bySlot, BYTE byCount);

		// Exchange
		bool SendExchangeStartPacket(DWORD vid);
		bool SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos);
		bool SendExchangeElkAddPacket(DWORD elk);
		bool SendExchangeItemDelPacket(BYTE pos);
		bool SendExchangeAcceptPacket();
		bool SendExchangeExitPacket();

		// Quest
		bool SendScriptAnswerPacket(int iAnswer);
		bool SendScriptButtonPacket(unsigned int iIndex);
		bool SendAnswerMakeGuildPacket(const char* c_szName);
		bool SendQuestInputStringPacket(const char* c_szString);
		bool SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID);

		// Event
		bool SendOnClickPacket(DWORD vid);

		// Fly
		bool SendFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendAddFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendShootPacket(UINT uSkill);

		// Command
		bool ClientCommand(const char* c_szCommand);
		void ServerCommand(char* c_szCommand);

		// Emoticon
		void RegisterEmoticonString(const char* pcEmoticonString);

		// Party
		bool SendPartyInvitePacket(DWORD dwVID);
		bool SendPartyInviteAnswerPacket(DWORD dwLeaderVID, BYTE byAccept);
		bool SendPartyRemovePacket(DWORD dwPID);
		bool SendPartySetStatePacket(DWORD dwVID, BYTE byState, BYTE byFlag);
		bool SendPartyUseSkillPacket(BYTE bySkillIndex, DWORD dwVID);
		bool SendPartyParameterPacket(BYTE byDistributeMode);

		// SafeBox
		bool SendSafeBoxMoneyPacket(BYTE byState, DWORD dwMoney);
		bool SendSafeBoxCheckinPacket(TItemPos InventoryPos, BYTE bySafeBoxPos);
		bool SendSafeBoxCheckoutPacket(BYTE bySafeBoxPos, TItemPos InventoryPos);
		bool SendSafeBoxItemMovePacket(BYTE bySourcePos, BYTE byTargetPos, BYTE byCount);

		// Mall
		bool SendMallCheckoutPacket(BYTE byMallPos, TItemPos InventoryPos);

		// Guild
		bool SendGuildAddMemberPacket(DWORD dwVID);
		bool SendGuildRemoveMemberPacket(DWORD dwPID);
		bool SendGuildChangeGradeNamePacket(BYTE byGradeNumber, const char* c_szName);
		bool SendGuildChangeGradeAuthorityPacket(BYTE byGradeNumber, BYTE byAuthority);
		bool SendGuildOfferPacket(DWORD dwExperience);
		bool SendGuildPostCommentPacket(const char* c_szMessage);
		bool SendGuildDeleteCommentPacket(DWORD dwIndex);
		bool SendGuildRefreshCommentsPacket(DWORD dwHighestIndex);
		bool SendGuildChangeMemberGradePacket(DWORD dwPID, BYTE byGrade);
		bool SendGuildUseSkillPacket(DWORD dwSkillID, DWORD dwTargetVID);
		bool SendGuildChangeMemberGeneralPacket(DWORD dwPID, BYTE byFlag);
		bool SendGuildInvitePacket(DWORD dwVID);
		bool SendGuildInviteAnswerPacket(DWORD dwGuildID, BYTE byAnswer);
		bool SendGuildChargeGSPPacket(DWORD dwMoney);
		bool SendGuildDepositMoneyPacket(DWORD dwMoney);
		bool SendGuildWithdrawMoneyPacket(DWORD dwMoney);

		// Mall
		bool RecvMallOpenPacket(const protocol::Packet& value);
		bool RecvMallItemSetPacket(const protocol::Packet& value);
		bool RecvMallItemDelPacket(const protocol::Packet& value);

		// Lover
		bool RecvLoverInfoPacket(const protocol::Packet& value);
		bool RecvLovePointUpdatePacket(const protocol::Packet& value);

		// Dig
		bool RecvDigMotionPacket(const protocol::Packet& value);

		// Fishing
		bool SendFishingPacket(int iRotation);
		bool SendGiveItemPacket(DWORD dwTargetVID, TItemPos ItemPos, int iItemCount);

		// Private Shop
		bool SendBuildPrivateShopPacket(const char* c_szName, const std::vector<TShopItemTable>& c_rSellingItemStock);

		// Refine
		bool SendRefinePacket(BYTE byPos, BYTE byType);
		bool SendSelectItemPacket(DWORD dwItemPos);

		// Client Version
		bool SendClientVersionPacket();

		// CRC Report
		bool __SendCRCReportPacket();

		// 용홍석 강화
		bool SendDragonSoulRefinePacket(BYTE bRefineType, TItemPos* pos);

		// Handshake
		bool RecvHandshakePacket();
		bool RecvHandshakeOKPacket(const protocol::Packet& value);

		// ETC
		DWORD GetMainActorVID();
		DWORD GetMainActorRace();
		DWORD GetMainActorEmpire();
		DWORD GetMainActorSkillGroup();
		void SetEmpireID(DWORD dwEmpireID);
		DWORD GetEmpireID();
		void __TEST_SetSkillGroupFake(int iIndex);

		//////////////////////////////////////////////////////////////////////////
		// Phase 관련
		//////////////////////////////////////////////////////////////////////////
	public:
		void SetOffLinePhase();
		void SetHandShakePhase();
		void SetLoginPhase();
		void SetSelectPhase();
		void SetLoadingPhase();
		void SetGamePhase();
		void ClosePhase();

		// Login Phase
		bool SendLoginPacket(const char* c_szName, const char* c_szPassword);
		bool SendLoginPacketNew(const char* c_szName, const char* c_szPassword);
		bool SendChinaMatrixCardPacket(const char* c_szMatrixCardString);
		bool SendRunupMatrixAnswerPacket(const char* c_szMatrixCardString);
		bool SendNEWCIBNPasspodAnswerPacket(const char* answer);
		bool SendDirectEnterPacket(const char* c_szName, const char* c_szPassword, UINT uChrSlot);

		bool SendEnterGame();

		// Select Phase
		bool SendSelectEmpirePacket(DWORD dwEmpireID);
		bool SendSelectCharacterPacket(BYTE account_Index);
		bool SendChangeNamePacket(BYTE index, const char* name);
		bool SendCreateCharacterPacket(BYTE index, const char* name, BYTE job, BYTE shape, BYTE byStat1, BYTE byStat2, BYTE byStat3, BYTE byStat4);
		bool SendDestroyCharacterPacket(BYTE index, const char* szPrivateCode);

		// Main Game Phase
		bool SendC2CPacket(DWORD dwSize, void* pData);
		bool SendChatPacket(const char* c_szChat, BYTE byType = CHAT_TYPE_TALKING);
		bool SendWhisperPacket(const char* name, const char* c_szChat);
		bool SendMessengerAddByVIDPacket(DWORD vid);
		bool SendMessengerAddByNamePacket(const char* c_szName);
		bool SendMessengerRemovePacket(const char* c_szKey, const char* c_szName);

	protected:
		bool OnProcess(); // State들을 실제로 실행한다.
		void OffLinePhase();
		void HandShakePhase();
		void LoginPhase();
		void SelectPhase();
		void LoadingPhase();
		void GamePhase();

		bool __IsNotPing();

		void __DownloadMark();
		void __DownloadSymbol(const std::vector<DWORD>& c_rkVec_dwGuildID);

		void __PlayInventoryItemUseSound(TItemPos uSlotPos);
		void __PlayInventoryItemDropSound(TItemPos uSlotPos);
		// void __PlayShopItemDropSound(UINT uSlotPos);
		void __PlaySafeBoxItemDropSound(UINT uSlotPos);
		void __PlayMallItemDropSound(UINT uSlotPos);

		bool __CanActMainInstance();

		enum REFRESH_WINDOW_TYPE
		{
			RefreshStatus = (1 << 0),
			RefreshAlignmentWindow = (1 << 1),
			RefreshCharacterWindow = (1 << 2),
			RefreshEquipmentWindow = (1 << 3),
			RefreshInventoryWindow = (1 << 4),
			RefreshExchangeWindow = (1 << 5),
			RefreshSkillWindow = (1 << 6),
			RefreshSafeboxWindow = (1 << 7),
			RefreshMessengerWindow = (1 << 8),
			RefreshGuildWindowInfoPage = (1 << 9),
			RefreshGuildWindowBoardPage = (1 << 10),
			RefreshGuildWindowMemberPage = (1 << 11),
			RefreshGuildWindowMemberPageGradeComboBox = (1 << 12),
			RefreshGuildWindowSkillPage = (1 << 13),
			RefreshGuildWindowGradePage = (1 << 14),
			RefreshTargetBoard = (1 << 15),
			RefreshMallWindow = (1 << 16),
		};

		void __RefreshStatus();
		void __RefreshAlignmentWindow();
		void __RefreshCharacterWindow();
		void __RefreshEquipmentWindow();
		void __RefreshInventoryWindow();
		void __RefreshExchangeWindow();
		void __RefreshSkillWindow();
		void __RefreshSafeboxWindow();
		void __RefreshMessengerWindow();
		void __RefreshGuildWindowInfoPage();
		void __RefreshGuildWindowBoardPage();
		void __RefreshGuildWindowMemberPage();
		void __RefreshGuildWindowMemberPageGradeComboBox();
		void __RefreshGuildWindowSkillPage();
		void __RefreshGuildWindowGradePage();
		void __RefreshTargetBoardByVID(DWORD dwVID);
		void __RefreshTargetBoardByName(const char* c_szName);
		void __RefreshTargetBoard();
		void __RefreshMallWindow();

		bool __SendHack(const char* c_szMsg);

	protected:
		bool RecvObserverAddPacket();
		bool RecvObserverRemovePacket();
		bool RecvObserverMovePacket();

		// Common
		bool RecvErrorPacket(int header);
		bool RecvPingPacket(const protocol::Packet& value);
		bool RecvDefaultPacket(int header);
		bool RecvPhasePacket(const protocol::Packet& value);
		bool DecodeProtocolPacket(TPacketHeader header);
		bool DispatchProtocolPacket(const protocol::Packet& value);

		// Login Phase
		bool __RecvLoginSuccessPacket3(const protocol::Packet& value);
		bool __RecvLoginSuccessPacket4(const protocol::Packet& value);
		bool __RecvLoginFailurePacket(const protocol::Packet& value);
		bool __RecvEmpirePacket(const protocol::Packet& value);
		bool __RecvChinaMatrixCardPacket(const protocol::Packet& value);
		bool __RecvRunupMatrixQuizPacket(const protocol::Packet& value);
		bool __RecvNEWCIBNPasspodRequestPacket(const protocol::Packet& value);
		bool __RecvNEWCIBNPasspodFailurePacket(const protocol::Packet& value);
		bool __RecvLoginKeyPacket(const protocol::Packet& value);

		// Select Phase
		bool __RecvPlayerCreateSuccessPacket(const protocol::Packet& value);
		bool __RecvPlayerCreateFailurePacket(const protocol::Packet& value);
		bool __RecvPlayerDestroySuccessPacket(const protocol::Packet& value);
		bool __RecvPlayerDestroyFailurePacket(const protocol::Packet& value);
		bool __RecvPreserveItemPacket();
		bool __RecvPlayerPoints(const protocol::Packet& value);
		bool __RecvChangeName(const protocol::Packet& value);

		// Loading Phase
		bool RecvMainCharacter(const protocol::Packet& value);
		bool RecvMainCharacter2_EMPIRE(const protocol::Packet& value);
		bool RecvMainCharacter3_BGM(const protocol::Packet& value);
		bool RecvMainCharacter4_BGM_VOL(const protocol::Packet& value);

		void __SetFieldMusicFileName(const char* musicName);
		void __SetFieldMusicFileInfo(const char* musicName, float vol);
		// END_OF_SUPPORT_BGM

		// Main Game Phase
		bool RecvWarpPacket(const protocol::Packet& value);
		bool RecvPVPPacket(const protocol::Packet& value);
		bool RecvDuelStartPacket();
		bool RecvGlobalTimePacket();
		bool RecvCharacterAppendPacket(const protocol::Packet& value);
		bool RecvCharacterAdditionalInfo(const protocol::Packet& value);
		bool RecvCharacterAppendPacketNew(const protocol::Packet& value);
		bool RecvCharacterUpdatePacket(const protocol::Packet& value);
		bool RecvCharacterUpdatePacketNew(const protocol::Packet& value);
		bool RecvCharacterDeletePacket(const protocol::Packet& value);
		bool RecvChatPacket();
		bool RecvOwnerShipPacket(const protocol::Packet& value);
		bool RecvSyncPositionPacket();
		bool RecvWhisperPacket();
		bool RecvPointChange(const protocol::Packet& value); // Alarm to python
		bool RecvChangeSpeedPacket(const protocol::Packet& value);

		bool RecvStunPacket(const protocol::Packet& value);
		bool RecvDeadPacket(const protocol::Packet& value);
		bool RecvCharacterMovePacket(const protocol::Packet& value);

		bool RecvItemSetPacket(const protocol::Packet& value);	  // Alarm to python
		bool RecvItemSetPacket2(const protocol::Packet& value);	  // Alarm to python
		bool RecvItemUsePacket(const protocol::Packet& value);	  // Alarm to python
		bool RecvItemUpdatePacket(const protocol::Packet& value); // Alarm to python
		bool RecvItemGroundAddPacket(const protocol::Packet& value);
		bool RecvItemGroundDelPacket(const protocol::Packet& value);
		bool RecvItemOwnership(const protocol::Packet& value);

		bool RecvQuickSlotAddPacket(const protocol::Packet& value);	 // Alarm to python
		bool RecvQuickSlotDelPacket(const protocol::Packet& value);	 // Alarm to python
		bool RecvQuickSlotMovePacket(const protocol::Packet& value); // Alarm to python

		bool RecvCharacterPositionPacket(const protocol::Packet& value);
		bool RecvMotionPacket(const protocol::Packet& value);

		bool RecvShopPacket();
		bool RecvShopSignPacket(const protocol::Packet& value);
		bool RecvExchangePacket(const protocol::Packet& value);

		// Quest
		bool RecvScriptPacket();
		bool RecvQuestInfoPacket();
		bool RecvQuestConfirmPacket(const protocol::Packet& value);
		bool RecvRequestMakeGuild(const protocol::Packet& value);

		// Skill
		bool RecvSkillLevel(const protocol::Packet& value);
		bool RecvSkillLevelNew(const protocol::Packet& value);
		bool RecvSkillCoolTimeEnd();

		// Target
		bool RecvTargetPacket(const protocol::Packet& value);
		bool RecvViewEquipPacket(const protocol::Packet& value);
		bool RecvDamageInfoPacket(const protocol::Packet& value);

		// Mount
		bool RecvMountPacket(const protocol::Packet& value);

		// Fly
		bool RecvCreateFlyPacket(const protocol::Packet& value);
		bool RecvFlyTargetingPacket(const protocol::Packet& value);
		bool RecvAddFlyTargetingPacket(const protocol::Packet& value);

		// Messenger
		bool RecvMessenger();

		// Guild
		bool RecvGuild();

		// Party
		bool RecvPartyInvite(const protocol::Packet& value);
		bool RecvPartyAdd(const protocol::Packet& value);
		bool RecvPartyUpdate(const protocol::Packet& value);
		bool RecvPartyRemove(const protocol::Packet& value);
		bool RecvPartyLink(const protocol::Packet& value);
		bool RecvPartyUnlink(const protocol::Packet& value);
		bool RecvPartyParameter(const protocol::Packet& value);

		// SafeBox
		bool RecvSafeBoxSetPacket(const protocol::Packet& value);
		bool RecvSafeBoxDelPacket(const protocol::Packet& value);
		bool RecvSafeBoxWrongPasswordPacket(const protocol::Packet& value);
		bool RecvSafeBoxSizePacket(const protocol::Packet& value);
		bool RecvSafeBoxMoneyChangePacket(const protocol::Packet& value);

		// Fishing
		bool RecvFishing(const protocol::Packet& value);

		// Dungeon
		bool RecvDungeon();

		// Time
		bool RecvTimePacket(const protocol::Packet& value);

		// WalkMode
		bool RecvWalkModePacket(const protocol::Packet& value);

		// ChangeSkillGroup
		bool RecvChangeSkillGroupPacket(const protocol::Packet& value);

		// Refine
		bool RecvRefineInformationPacket(const protocol::Packet& value);
		bool RecvRefineInformationPacketNew(const protocol::Packet& value);

		// Use Potion
		bool RecvSpecialEffect(const protocol::Packet& value);

		// 서버에서 지정한 이팩트 발동 패킷.
		bool RecvSpecificEffect(const protocol::Packet& value);

		// 용혼석 관련
		bool RecvDragonSoulRefine(const protocol::Packet& value);

		// MiniMap Info
		bool RecvNPCList();
		bool RecvLandPacket();
		bool RecvTargetCreatePacket();
		bool RecvTargetCreatePacketNew(const protocol::Packet& value);
		bool RecvTargetUpdatePacket(const protocol::Packet& value);
		bool RecvTargetDeletePacket(const protocol::Packet& value);

		// Affect
		bool RecvAffectAddPacket(const protocol::Packet& value);
		bool RecvAffectRemovePacket(const protocol::Packet& value);

		// Channel
		bool RecvChannelPacket(const protocol::Packet& value);

	protected:
		// 이모티콘
		bool ParseEmoticon(const char* pChatMsg, DWORD* pdwEmoticon);

		// 파이썬으로 보내는 콜들
		void OnConnectFailure();
		void OnScriptEventStart(int iSkin, int iIndex);

		void OnRemoteDisconnect();
		void OnDisconnect();

		void SetGameOnline();
		void SetGameOffline();
		BOOL IsGameOnline();

	protected:
		bool CheckPacket(TPacketHeader* pRetHeader);

		void __InitializeGamePhase();
		void __InitializeMarkAuth();
		void __GlobalPositionToLocalPosition(LONG& rGlobalX, LONG& rGlobalY);
		void __LocalPositionToGlobalPosition(LONG& rLocalX, LONG& rLocalY);

		bool __IsPlayerAttacking();
		bool __IsEquipItemInSlot(TItemPos Cell);

		void __ShowMapName(LONG lLocalX, LONG lLocalY);

		void __LeaveOfflinePhase() {}
		void __LeaveHandshakePhase() {}
		void __LeaveLoginPhase() {}
		void __LeaveSelectPhase() {}
		void __LeaveLoadingPhase() {}
		void __LeaveGamePhase();

		void __ClearNetworkActorManager();

		void __ClearSelectCharacterData();

		// DELETEME
		// void __SendWarpPacket();

		void __ConvertEmpireText(DWORD dwEmpireID, char* szText);

		void __RecvCharacterAppendPacket(SNetworkActorData* pkNetActorData);
		void __RecvCharacterUpdatePacket(SNetworkUpdateActorData* pkNetUpdateActorData);

		void __FilterInsult(char* szLine, UINT uLineLen);

		void __SetGuildID(DWORD id);

	protected:
		TPacketGCHandshake m_HandshakeData;
		DWORD m_dwChangingPhaseTime;
		DWORD m_dwBindupRetryCount;
		DWORD m_dwMainActorVID;
		DWORD m_dwMainActorRace;
		DWORD m_dwMainActorEmpire;
		DWORD m_dwMainActorSkillGroup;
		BOOL m_isGameOnline;
		BOOL m_isStartGame;

		DWORD m_dwGuildID;
		DWORD m_dwEmpireID;

		struct SServerTimeSync
		{
				DWORD m_dwChangeServerTime;
				DWORD m_dwChangeClientTime;
		} m_kServerTimeSync;

		void __ServerTimeSync_Initialize();
		// DWORD m_dwBaseServerTime;
		// DWORD m_dwBaseClientTime;

		DWORD m_dwLastGamePingTime;

		std::string m_stID;
		std::string m_stPassword;
		std::string m_strLastCommand;
		std::string m_strPhase;
		DWORD m_dwLoginKey;
		BOOL m_isWaitLoginKey;

		std::string m_stMarkIP;

		CFuncObject<CPythonNetworkStream> m_phaseProcessFunc;
		CFuncObject<CPythonNetworkStream> m_phaseLeaveFunc;

		PyObject* m_poHandler;
		PyObject* m_apoPhaseWnd[PHASE_WINDOW_NUM];
		PyObject* m_poSerCommandParserWnd;

		TSimplePlayerInformation m_akSimplePlayerInfo[PLAYER_PER_ACCOUNT4];
		DWORD m_adwGuildID[PLAYER_PER_ACCOUNT4];
		std::string m_astrGuildName[PLAYER_PER_ACCOUNT4];
		bool m_bSimplePlayerInfo;

		CRef<CNetworkActorManager> m_rokNetActorMgr;

		bool m_isRefreshStatus;
		bool m_isRefreshCharacterWnd;
		bool m_isRefreshEquipmentWnd;
		bool m_isRefreshInventoryWnd;
		bool m_isRefreshExchangeWnd;
		bool m_isRefreshSkillWnd;
		bool m_isRefreshSafeboxWnd;
		bool m_isRefreshMallWnd;
		bool m_isRefreshMessengerWnd;
		bool m_isRefreshGuildWndInfoPage;
		bool m_isRefreshGuildWndBoardPage;
		bool m_isRefreshGuildWndMemberPage;
		bool m_isRefreshGuildWndMemberPageGradeComboBox;
		bool m_isRefreshGuildWndSkillPage;
		bool m_isRefreshGuildWndGradePage;

		// Emoticon
		std::vector<std::string> m_EmoticonStringVector;

		struct STextConvertTable
		{
				char acUpper[26];
				char acLower[26];
				BYTE aacHan[5000][2];
		} m_aTextConvTable[3];

		struct SMarkAuth
		{
				CNetworkAddress m_kNetAddr;
				DWORD m_dwHandle;
				DWORD m_dwRandomKey;
		} m_kMarkAuth;

		DWORD m_dwSelectedCharacterIndex;

		CInsultChecker m_kInsultChecker;

		bool m_isEnableChatInsultFilter;
		bool m_bComboSkillFlag;

		std::deque<std::string> m_kQue_stHack;

	private:
		struct SDirectEnterMode
		{
				bool m_isSet;
				DWORD m_dwChrSlotIndex;
		} m_kDirectEnterMode;

		void __DirectEnterMode_Initialize();
		void __DirectEnterMode_Set(UINT uChrSlotIndex);
		bool __DirectEnterMode_IsSet();

	public:
		DWORD EXPORT_GetBettingGuildWarValue(const char* c_szValueName);

	private:
		struct SBettingGuildWar
		{
				DWORD m_dwBettingMoney;
				DWORD m_dwObserverCount;
		} m_kBettingGuildWar;

		CInstanceBase* m_pInstTarget;

		void __BettingGuildWar_Initialize();
		void __BettingGuildWar_SetObserverCount(UINT uObserverCount);
		void __BettingGuildWar_SetBettingMoney(UINT uBettingMoney);
};
