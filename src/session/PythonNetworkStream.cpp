#include <Client/UI/StdAfx.h>
#include <Neoeter/Render/NetPacketHeaderMap.h>

#include <Client/Session/PythonNetworkStream.h>
#include <Client/UI/Packet.h>
#include <Client/UI/QuitRequest.h>
#include <Client/UI/NetworkActorManager.h>

#include <Client/UI/GuildMarkDownloader.h>
#include <Client/UI/GuildMarkUploader.h>
#include <Client/UI/MarkManager.h>

#include <Client/UI/ProcessCRC.h>

// MARK_BUG_FIX
static DWORD gs_nextDownloadMarkTime = 0;
// END_OF_MARK_BUG_FIX

// Packet ---------------------------------------------------------------------------
namespace
{
	class CPacketHeaderMap : public CNetworkPacketHeaderMap
	{
		public:
			CPacketHeaderMap()
			{
				std::size_t count = 0;
				const protocol::SizeEntry* entries = protocol::SizeTable(&count);

				for (std::size_t i = 0; i < count; ++i)
					Set(entries[i].header, TPacketType(entries[i].size, entries[i].dynamic));
			}
	};
}

int g_iLastPacket[2] = {0, 0};

void CPythonNetworkStream::ExitApplication()
{
	if (__IsNotPing())
	{
		AbsoluteExitApplication();
	}
	else
	{
		SendChatPacket("/quit");
	}
}

void CPythonNetworkStream::ExitGame()
{
	if (__IsNotPing())
	{
		LogOutGame();
	}
	else
	{
		SendChatPacket("/phase_select");
	}
}

void CPythonNetworkStream::LogOutGame()
{
	if (__IsNotPing())
	{
		AbsoluteExitGame();
	}
	else
	{
		SendChatPacket("/logout");
	}
}

void CPythonNetworkStream::AbsoluteExitGame()
{
	if (!IsOnline())
		return;

	OnRemoteDisconnect();
	Disconnect();
}

void CPythonNetworkStream::AbsoluteExitApplication()
{
	Client_RequestQuit();
}

bool CPythonNetworkStream::__IsNotPing()
{
	// 원래는 핑이 안올때 체크이나 서버랑 정확히 맞추어야 한다.
	return false;
}

DWORD CPythonNetworkStream::GetGuildID()
{
	return m_dwGuildID;
}

UINT CPythonNetworkStream::UploadMark(const char* c_szImageFileName)
{
	// MARK_BUG_FIX
	// 길드를 만든 직후는 길드 아이디가 0이다.
	if (0 == m_dwGuildID)
		return ERROR_MARK_UPLOAD_NEED_RECONNECT;

	gs_nextDownloadMarkTime = 0;
	// END_OF_MARK_BUG_FIX

	UINT uError = ERROR_UNKNOWN;
	CGuildMarkUploader& rkGuildMarkUploader = CGuildMarkUploader::Instance();
	if (!rkGuildMarkUploader.Connect(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey, m_dwGuildID, c_szImageFileName, &uError))
	{
		switch (uError)
		{
			case CGuildMarkUploader::ERROR_CONNECT:
				return ERROR_CONNECT_MARK_SERVER;
				break;
			case CGuildMarkUploader::ERROR_LOAD:
				return ERROR_LOAD_MARK;
				break;
			case CGuildMarkUploader::ERROR_WIDTH:
				return ERROR_MARK_WIDTH;
				break;
			case CGuildMarkUploader::ERROR_HEIGHT:
				return ERROR_MARK_HEIGHT;
				break;
			default:
				return ERROR_UNKNOWN;
		}
	}

	// MARK_BUG_FIX
	__DownloadMark();
	// END_OF_MARK_BUG_FIX

	if (CGuildMarkManager::INVALID_MARK_ID == CGuildMarkManager::Instance().GetMarkID(m_dwGuildID))
		return ERROR_MARK_CHECK_NEED_RECONNECT;

	return ERROR_NONE;
}

UINT CPythonNetworkStream::UploadSymbol(const char* c_szImageFileName)
{
	UINT uError = ERROR_UNKNOWN;
	CGuildMarkUploader& rkGuildMarkUploader = CGuildMarkUploader::Instance();
	if (!rkGuildMarkUploader.ConnectToSendSymbol(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey, m_dwGuildID, c_szImageFileName, &uError))
	{
		switch (uError)
		{
			case CGuildMarkUploader::ERROR_CONNECT:
				return ERROR_CONNECT_MARK_SERVER;
				break;
			case CGuildMarkUploader::ERROR_LOAD:
				return ERROR_LOAD_MARK;
				break;
			case CGuildMarkUploader::ERROR_WIDTH:
				return ERROR_MARK_WIDTH;
				break;
			case CGuildMarkUploader::ERROR_HEIGHT:
				return ERROR_MARK_HEIGHT;
				break;
			default:
				return ERROR_UNKNOWN;
		}
	}

	return ERROR_NONE;
}

void CPythonNetworkStream::__DownloadMark()
{
	// 3분 안에는 다시 접속하지 않는다.
	DWORD curTime = ELTimer_GetMSec();

	if (curTime < gs_nextDownloadMarkTime)
		return;

	gs_nextDownloadMarkTime = curTime + 60000 * 3; // 3분

	CGuildMarkDownloader& rkGuildMarkDownloader = CGuildMarkDownloader::Instance();
	rkGuildMarkDownloader.Connect(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey);
}

void CPythonNetworkStream::__DownloadSymbol(const std::vector<DWORD>& c_rkVec_dwGuildID)
{
	CGuildMarkDownloader& rkGuildMarkDownloader = CGuildMarkDownloader::Instance();
	rkGuildMarkDownloader.ConnectToRecvSymbol(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey, c_rkVec_dwGuildID);
}

void CPythonNetworkStream::SetPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd)
{
	if (ePhaseWnd >= PHASE_WINDOW_NUM)
		return;

	m_apoPhaseWnd[ePhaseWnd] = poPhaseWnd;
}

void CPythonNetworkStream::ClearPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd)
{
	if (ePhaseWnd >= PHASE_WINDOW_NUM)
		return;

	if (poPhaseWnd != m_apoPhaseWnd[ePhaseWnd])
		return;

	m_apoPhaseWnd[ePhaseWnd] = 0;
}

void CPythonNetworkStream::SetServerCommandParserWindow(PyObject* poWnd)
{
	m_poSerCommandParserWnd = poWnd;
}

bool CPythonNetworkStream::IsSelectedEmpire()
{
	if (m_dwEmpireID)
		return true;

	return false;
}

UINT CPythonNetworkStream::GetAccountCharacterSlotDatau(UINT iSlot, UINT eType)
{
	if (iSlot >= PLAYER_PER_ACCOUNT4)
		return 0;

	TSimplePlayerInformation& rkSimplePlayerInfo = m_akSimplePlayerInfo[iSlot];

	switch (eType)
	{
		case ACCOUNT_CHARACTER_SLOT_ID:
			return rkSimplePlayerInfo.dwID;
		case ACCOUNT_CHARACTER_SLOT_RACE:
			return rkSimplePlayerInfo.byJob;
		case ACCOUNT_CHARACTER_SLOT_LEVEL:
			return rkSimplePlayerInfo.byLevel;
		case ACCOUNT_CHARACTER_SLOT_STR:
			return rkSimplePlayerInfo.byST;
		case ACCOUNT_CHARACTER_SLOT_DEX:
			return rkSimplePlayerInfo.byDX;
		case ACCOUNT_CHARACTER_SLOT_HTH:
			return rkSimplePlayerInfo.byHT;
		case ACCOUNT_CHARACTER_SLOT_INT:
			return rkSimplePlayerInfo.byIQ;
		case ACCOUNT_CHARACTER_SLOT_PLAYTIME:
			return rkSimplePlayerInfo.dwPlayMinutes;
		case ACCOUNT_CHARACTER_SLOT_FORM:
			//			return rkSimplePlayerInfo.wParts[CRaceData::PART_MAIN];
			return rkSimplePlayerInfo.wMainPart;
		case ACCOUNT_CHARACTER_SLOT_PORT:
			return rkSimplePlayerInfo.wPort;
		case ACCOUNT_CHARACTER_SLOT_GUILD_ID:
			return m_adwGuildID[iSlot];
			break;
		case ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG:
			return rkSimplePlayerInfo.bChangeName;
			break;
		case ACCOUNT_CHARACTER_SLOT_HAIR:
			return rkSimplePlayerInfo.wHairPart;
			break;
	}
	return 0;
}

const char* CPythonNetworkStream::GetAccountCharacterSlotDataz(UINT iSlot, UINT eType)
{
	static const char* sc_szEmpty = "";

	if (iSlot >= PLAYER_PER_ACCOUNT4)
		return sc_szEmpty;

	TSimplePlayerInformation& rkSimplePlayerInfo = m_akSimplePlayerInfo[iSlot];

	switch (eType)
	{
		case ACCOUNT_CHARACTER_SLOT_ADDR:
		{
			BYTE ip[4];

			const int LEN = 4;
			for (int i = 0; i < LEN; i++)
			{
				ip[i] = BYTE(rkSimplePlayerInfo.lAddr & 0xff);
				rkSimplePlayerInfo.lAddr >>= 8;
			}

			static char s_szAddr[256];
			sprintf(s_szAddr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
			return s_szAddr;
		}
		break;
		case ACCOUNT_CHARACTER_SLOT_NAME:
			return rkSimplePlayerInfo.szName;
			break;
		case ACCOUNT_CHARACTER_SLOT_GUILD_NAME:
			return m_astrGuildName[iSlot].c_str();
			break;
	}
	return sc_szEmpty;
}

void CPythonNetworkStream::ConnectLoginServer(const char* c_szAddr, UINT uPort)
{
	CNetworkStream::Connect(c_szAddr, uPort);
}

void CPythonNetworkStream::SetMarkServer(const char* c_szAddr, UINT uPort)
{
	m_kMarkAuth.m_kNetAddr.Set(c_szAddr, uPort);
}

void CPythonNetworkStream::ConnectGameServer(UINT iChrSlot)
{
	if (iChrSlot >= PLAYER_PER_ACCOUNT4)
		return;

	m_dwSelectedCharacterIndex = iChrSlot;

	__DirectEnterMode_Set(iChrSlot);

	TSimplePlayerInformation& rkSimplePlayerInfo = m_akSimplePlayerInfo[iChrSlot];
	CNetworkStream::Connect((DWORD)rkSimplePlayerInfo.lAddr, rkSimplePlayerInfo.wPort);
}

void CPythonNetworkStream::SetLoginInfo(const char* c_szID, const char* c_szPassword)
{
	m_stID = c_szID;
	m_stPassword = c_szPassword;
}

void CPythonNetworkStream::ClearLoginInfo(void)
{
	m_stPassword = "";
}

void CPythonNetworkStream::SetLoginKey(DWORD dwLoginKey)
{
	m_dwLoginKey = dwLoginKey;
}

bool CPythonNetworkStream::CheckPacket(TPacketHeader* pRetHeader)
{
	*pRetHeader = 0;

	static CPacketHeaderMap s_packetHeaderMap;

	TPacketHeader header;

	if (!Peek(sizeof(TPacketHeader), &header))
		return false;

	if (0 == header)
	{
		if (!Recv(sizeof(TPacketHeader), &header))
			return false;

		while (Peek(sizeof(TPacketHeader), &header))
		{
			if (0 == header)
			{
				if (!Recv(sizeof(TPacketHeader), &header))
					return false;
			}
			else
			{
				break;
			}
		}

		if (0 == header)
			return false;
	}

	CNetworkPacketHeaderMap::TPacketType PacketType;

	if (!s_packetHeaderMap.Get(header, &PacketType))
	{
		TraceError("Unknown packet header: %d, last: %d %d", header, g_iLastPacket[0], g_iLastPacket[1]);
		ClearRecvBuffer();

		Client_RequestQuit();
		return false;
	}

	// Code for dynamic size packet
	if (PacketType.isDynamicSizePacket)
	{
		TDynamicSizePacketHeader DynamicSizePacketHeader;

		if (!Peek(sizeof(TDynamicSizePacketHeader), &DynamicSizePacketHeader))
			return false;

		if (!Peek(DynamicSizePacketHeader.size))
		{
			Tracef("CPythonNetworkStream::CheckPacket - Not enough dynamic packet size: header %d packet size: %d\n",
				   DynamicSizePacketHeader.header,
				   DynamicSizePacketHeader.size);
			return false;
		}
	}
	else
	{
		if (!Peek(PacketType.iPacketSize))
		{
			// Tracef("Not enough packet size: header %d packet size: %d, recv buffer size: %d",
			//	header,
			//	PacketType.iPacketSize,
			//	GetRecvBufferSize());
			return false;
		}
	}

	if (!header)
		return false;

	*pRetHeader = header;

	g_iLastPacket[0] = g_iLastPacket[1];
	g_iLastPacket[1] = header;
	// Tracenf("header %d size %d", header, PacketType.iPacketSize);
	// Tracenf("header %d size %d outputpos[%d] security %u", header, PacketType.iPacketSize, m_recvBufOutputPos, IsSecurityMode());
	return true;
}

bool CPythonNetworkStream::DispatchProtocolPacket(const protocol::Packet& value)
{
	using Handler = bool (CPythonNetworkStream::*)(const protocol::Packet&);

	struct FEntry
	{
			uint8_t header;
			Handler handler;
	};

	static const FEntry entries[] = {
		{HEADER_GC_QUICKSLOT_ADD, &CPythonNetworkStream::RecvQuickSlotAddPacket},
		{HEADER_GC_QUICKSLOT_DEL, &CPythonNetworkStream::RecvQuickSlotDelPacket},
		{HEADER_GC_ITEM_OWNERSHIP, &CPythonNetworkStream::RecvItemOwnership},
		{HEADER_GC_TARGET, &CPythonNetworkStream::RecvTargetPacket},
		{HEADER_GC_ADD_FLY_TARGETING, &CPythonNetworkStream::RecvAddFlyTargetingPacket},
		{HEADER_GC_AFFECT_ADD, &CPythonNetworkStream::RecvAffectAddPacket},
		{HEADER_GC_AFFECT_REMOVE, &CPythonNetworkStream::RecvAffectRemovePacket},
		{HEADER_GC_CHANGE_NAME, &CPythonNetworkStream::__RecvChangeName},
		{HEADER_GC_CHANGE_SKILL_GROUP, &CPythonNetworkStream::RecvChangeSkillGroupPacket},
		{HEADER_GC_CHANGE_SPEED, &CPythonNetworkStream::RecvChangeSpeedPacket},
		{HEADER_GC_CHANNEL, &CPythonNetworkStream::RecvChannelPacket},
		{HEADER_GC_CHARACTER_ADD, &CPythonNetworkStream::RecvCharacterAppendPacket},
		{HEADER_GC_CHARACTER_ADD2, &CPythonNetworkStream::RecvCharacterAppendPacketNew},
		{HEADER_GC_CHARACTER_DEL, &CPythonNetworkStream::RecvCharacterDeletePacket},
		{HEADER_GC_CHARACTER_MOVE, &CPythonNetworkStream::RecvCharacterMovePacket},
		{HEADER_GC_CHARACTER_POSITION, &CPythonNetworkStream::RecvCharacterPositionPacket},
		{HEADER_GC_CHARACTER_UPDATE, &CPythonNetworkStream::RecvCharacterUpdatePacket},
		{HEADER_GC_CHARACTER_UPDATE2, &CPythonNetworkStream::RecvCharacterUpdatePacketNew},
		{HEADER_GC_CHAR_ADDITIONAL_INFO, &CPythonNetworkStream::RecvCharacterAdditionalInfo},
		{HEADER_GC_CHINA_MATRIX_CARD, &CPythonNetworkStream::__RecvChinaMatrixCardPacket},
		{HEADER_GC_CREATE_FLY, &CPythonNetworkStream::RecvCreateFlyPacket},
		{HEADER_GC_DAMAGE_INFO, &CPythonNetworkStream::RecvDamageInfoPacket},
		{HEADER_GC_DEAD, &CPythonNetworkStream::RecvDeadPacket},
		{HEADER_GC_DIG_MOTION, &CPythonNetworkStream::RecvDigMotionPacket},
		{HEADER_GC_DRAGON_SOUL_REFINE, &CPythonNetworkStream::RecvDragonSoulRefine},
		{HEADER_GC_EMPIRE, &CPythonNetworkStream::__RecvEmpirePacket},
		{HEADER_GC_EXCHANGE, &CPythonNetworkStream::RecvExchangePacket},
		{HEADER_GC_FISHING, &CPythonNetworkStream::RecvFishing},
		{HEADER_GC_FLY_TARGETING, &CPythonNetworkStream::RecvFlyTargetingPacket},
		{HEADER_GC_HANDSHAKE_OK, &CPythonNetworkStream::RecvHandshakeOKPacket},
		{HEADER_GC_ITEM_GROUND_ADD, &CPythonNetworkStream::RecvItemGroundAddPacket},
		{HEADER_GC_ITEM_GROUND_DEL, &CPythonNetworkStream::RecvItemGroundDelPacket},
		{HEADER_GC_ITEM_SET, &CPythonNetworkStream::RecvItemSetPacket},
		{HEADER_GC_ITEM_SET2, &CPythonNetworkStream::RecvItemSetPacket2},
		{HEADER_GC_ITEM_UPDATE, &CPythonNetworkStream::RecvItemUpdatePacket},
		{HEADER_GC_ITEM_USE, &CPythonNetworkStream::RecvItemUsePacket},
		{HEADER_GC_LOGIN_FAILURE, &CPythonNetworkStream::__RecvLoginFailurePacket},
		{HEADER_GC_LOGIN_KEY, &CPythonNetworkStream::__RecvLoginKeyPacket},
		{HEADER_GC_LOGIN_SUCCESS3, &CPythonNetworkStream::__RecvLoginSuccessPacket3},
		{HEADER_GC_LOGIN_SUCCESS4, &CPythonNetworkStream::__RecvLoginSuccessPacket4},
		{HEADER_GC_LOVER_INFO, &CPythonNetworkStream::RecvLoverInfoPacket},
		{HEADER_GC_LOVE_POINT_UPDATE, &CPythonNetworkStream::RecvLovePointUpdatePacket},
		{HEADER_GC_MAIN_CHARACTER, &CPythonNetworkStream::RecvMainCharacter},
		{HEADER_GC_MAIN_CHARACTER2_EMPIRE, &CPythonNetworkStream::RecvMainCharacter2_EMPIRE},
		{HEADER_GC_MAIN_CHARACTER3_BGM, &CPythonNetworkStream::RecvMainCharacter3_BGM},
		{HEADER_GC_MAIN_CHARACTER4_BGM_VOL, &CPythonNetworkStream::RecvMainCharacter4_BGM_VOL},
		{HEADER_GC_MALL_DEL, &CPythonNetworkStream::RecvMallItemDelPacket},
		{HEADER_GC_MALL_OPEN, &CPythonNetworkStream::RecvMallOpenPacket},
		{HEADER_GC_MALL_SET, &CPythonNetworkStream::RecvMallItemSetPacket},
		{HEADER_GC_MOTION, &CPythonNetworkStream::RecvMotionPacket},
		{HEADER_GC_MOUNT, &CPythonNetworkStream::RecvMountPacket},
		{HEADER_GC_NEWCIBN_PASSPOD_FAILURE, &CPythonNetworkStream::__RecvNEWCIBNPasspodFailurePacket},
		{HEADER_GC_NEWCIBN_PASSPOD_REQUEST, &CPythonNetworkStream::__RecvNEWCIBNPasspodRequestPacket},
		{HEADER_GC_OWNERSHIP, &CPythonNetworkStream::RecvOwnerShipPacket},
		{HEADER_GC_PARTY_ADD, &CPythonNetworkStream::RecvPartyAdd},
		{HEADER_GC_PARTY_INVITE, &CPythonNetworkStream::RecvPartyInvite},
		{HEADER_GC_PARTY_LINK, &CPythonNetworkStream::RecvPartyLink},
		{HEADER_GC_PARTY_PARAMETER, &CPythonNetworkStream::RecvPartyParameter},
		{HEADER_GC_PARTY_REMOVE, &CPythonNetworkStream::RecvPartyRemove},
		{HEADER_GC_PARTY_UNLINK, &CPythonNetworkStream::RecvPartyUnlink},
		{HEADER_GC_PARTY_UPDATE, &CPythonNetworkStream::RecvPartyUpdate},
		{HEADER_GC_PHASE, &CPythonNetworkStream::RecvPhasePacket},
		{HEADER_GC_PING, &CPythonNetworkStream::RecvPingPacket},
		{HEADER_GC_PLAYER_CREATE_FAILURE, &CPythonNetworkStream::__RecvPlayerCreateFailurePacket},
		{HEADER_GC_PLAYER_CREATE_SUCCESS, &CPythonNetworkStream::__RecvPlayerCreateSuccessPacket},
		{HEADER_GC_PLAYER_DELETE_SUCCESS, &CPythonNetworkStream::__RecvPlayerDestroySuccessPacket},
		{HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID, &CPythonNetworkStream::__RecvPlayerDestroyFailurePacket},
		{HEADER_GC_PLAYER_POINTS, &CPythonNetworkStream::__RecvPlayerPoints},
		{HEADER_GC_PLAYER_POINT_CHANGE, &CPythonNetworkStream::RecvPointChange},
		{HEADER_GC_PVP, &CPythonNetworkStream::RecvPVPPacket},
		{HEADER_GC_QUEST_CONFIRM, &CPythonNetworkStream::RecvQuestConfirmPacket},
		{HEADER_GC_QUICKSLOT_SWAP, &CPythonNetworkStream::RecvQuickSlotMovePacket},
		{HEADER_GC_REFINE_INFORMATION, &CPythonNetworkStream::RecvRefineInformationPacket},
		{HEADER_GC_REFINE_INFORMATION_NEW, &CPythonNetworkStream::RecvRefineInformationPacketNew},
		{HEADER_GC_REQUEST_MAKE_GUILD, &CPythonNetworkStream::RecvRequestMakeGuild},
		{HEADER_GC_RUNUP_MATRIX_QUIZ, &CPythonNetworkStream::__RecvRunupMatrixQuizPacket},
		{HEADER_GC_SAFEBOX_DEL, &CPythonNetworkStream::RecvSafeBoxDelPacket},
		{HEADER_GC_SAFEBOX_MONEY_CHANGE, &CPythonNetworkStream::RecvSafeBoxMoneyChangePacket},
		{HEADER_GC_SAFEBOX_SET, &CPythonNetworkStream::RecvSafeBoxSetPacket},
		{HEADER_GC_SAFEBOX_SIZE, &CPythonNetworkStream::RecvSafeBoxSizePacket},
		{HEADER_GC_SAFEBOX_WRONG_PASSWORD, &CPythonNetworkStream::RecvSafeBoxWrongPasswordPacket},
		{HEADER_GC_SEPCIAL_EFFECT, &CPythonNetworkStream::RecvSpecialEffect},
		{HEADER_GC_SHOP_SIGN, &CPythonNetworkStream::RecvShopSignPacket},
		{HEADER_GC_SKILL_LEVEL, &CPythonNetworkStream::RecvSkillLevel},
		{HEADER_GC_SKILL_LEVEL_NEW, &CPythonNetworkStream::RecvSkillLevelNew},
		{HEADER_GC_SPECIFIC_EFFECT, &CPythonNetworkStream::RecvSpecificEffect},
		{HEADER_GC_STUN, &CPythonNetworkStream::RecvStunPacket},
		{HEADER_GC_TARGET_CREATE_NEW, &CPythonNetworkStream::RecvTargetCreatePacketNew},
		{HEADER_GC_TARGET_DELETE, &CPythonNetworkStream::RecvTargetDeletePacket},
		{HEADER_GC_TARGET_UPDATE, &CPythonNetworkStream::RecvTargetUpdatePacket},
		{HEADER_GC_TIME, &CPythonNetworkStream::RecvTimePacket},
		{HEADER_GC_VIEW_EQUIP, &CPythonNetworkStream::RecvViewEquipPacket},
		{HEADER_GC_WALK_MODE, &CPythonNetworkStream::RecvWalkModePacket},
		{HEADER_GC_WARP, &CPythonNetworkStream::RecvWarpPacket},
	};

	for (const FEntry& entry : entries)
	{
		if (entry.header == value.header)
			return (this->*entry.handler)(value);
	}

	return false;
}

bool CPythonNetworkStream::DecodeProtocolPacket(TPacketHeader header)
{
	std::size_t size = 0;

	if (!protocol::Size(header, &size) || size > protocol::kMaxPacketSize)
		return false;

	std::array<uint8_t, protocol::kMaxPacketSize> bytes;

	if (!Peek(static_cast<int>(size), bytes.data()))
		return false;

	protocol::Packet value;

	if (!protocol::Decode(header, bytes.data(), size, &value))
		return false;

	if (!Recv(static_cast<int>(size)))
		return false;

	return DispatchProtocolPacket(value);
}

bool CPythonNetworkStream::RecvErrorPacket(int header)
{
	TraceError("Phase %s does not handle this header (header: %d, last: %d, %d)",
			   m_strPhase.c_str(), header, g_iLastPacket[0], g_iLastPacket[1]);

	ClearRecvBuffer();
	return true;
}

bool CPythonNetworkStream::RecvPhasePacket(const protocol::Packet& value)
{
	const TPacketGCPhase packet_phase = value.Get<TPacketGCPhase>();

	switch (packet_phase.phase)
	{
		case PHASE_CLOSE: // 끊기는 상태 (또는 끊기 전 상태)
			ClosePhase();
			break;

		case PHASE_HANDSHAKE: // 악수..;;
			SetHandShakePhase();
			break;

		case PHASE_LOGIN: // 로그인 중
			SetLoginPhase();
			break;

		case PHASE_SELECT: // 캐릭터 선택 화면
			SetSelectPhase();

			BuildProcessCRC();

			// MARK_BUG_FIX
			__DownloadMark();
			// END_OF_MARK_BUG_FIX
			break;

		case PHASE_LOADING: // 선택 후 로딩 화면
			SetLoadingPhase();
			break;

		case PHASE_GAME: // 게임 화면
			SetGamePhase();
			break;

		case PHASE_DEAD: // 죽었을 때.. (게임 안에 있는 것일 수도..)
			break;
	}

	return true;
}

bool CPythonNetworkStream::RecvPingPacket(const protocol::Packet& value)
{
	Tracef("recv ping packet.\n");

	m_dwLastGamePingTime = ELTimer_GetMSec();

	TPacketCGPong kPacketPong;
	kPacketPong.bHeader = HEADER_CG_PONG;

	if (!Send(sizeof(TPacketCGPong), &kPacketPong))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvDefaultPacket(int header)
{
	if (!header)
		return true;

	TraceError("Unprocessed packet header %d, state %s\n", header, m_strPhase.c_str());
	ClearRecvBuffer();
	return true;
}

bool CPythonNetworkStream::OnProcess()
{
	if (m_isStartGame)
	{
		m_isStartGame = FALSE;

		PyCallClassMemberFunc(m_poHandler, "SetGamePhase", Py_BuildValue("()"));
		//		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartGame", Py_BuildValue("()"));
	}

	m_rokNetActorMgr->Update();

	if (m_phaseProcessFunc.IsEmpty())
		return true;

	// TPacketHeader header;
	// while(CheckPacket(&header))
	{
		m_phaseProcessFunc.Run();
	}

	return true;
}

// Set
void CPythonNetworkStream::SetOffLinePhase()
{
	if ("OffLine" != m_strPhase)
		m_phaseLeaveFunc.Run();

	m_strPhase = "OffLine";

	Tracen("");
	Tracen("## Network - OffLine Phase ##");
	Tracen("");

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::OffLinePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveOfflinePhase);

	SetGameOffline();

	m_dwSelectedCharacterIndex = 0;

	__DirectEnterMode_Initialize();
	__BettingGuildWar_Initialize();
}

void CPythonNetworkStream::ClosePhase()
{
	PyCallClassMemberFunc(m_poHandler, "SetLoginPhase", Py_BuildValue("()"));
}

// Game Online
void CPythonNetworkStream::SetGameOnline()
{
	m_isGameOnline = TRUE;
}

void CPythonNetworkStream::SetGameOffline()
{
	m_isGameOnline = FALSE;
}

BOOL CPythonNetworkStream::IsGameOnline()
{
	return m_isGameOnline;
}

// Handler
void CPythonNetworkStream::SetHandler(PyObject* poHandler)
{
	m_poHandler = poHandler;
}

// ETC
DWORD CPythonNetworkStream::GetMainActorVID()
{
	return m_dwMainActorVID;
}

DWORD CPythonNetworkStream::GetMainActorRace()
{
	return m_dwMainActorRace;
}

DWORD CPythonNetworkStream::GetMainActorEmpire()
{
	return m_dwMainActorEmpire;
}

DWORD CPythonNetworkStream::GetMainActorSkillGroup()
{
	return m_dwMainActorSkillGroup;
}

void CPythonNetworkStream::SetEmpireID(DWORD dwEmpireID)
{
	m_dwEmpireID = dwEmpireID;
}

DWORD CPythonNetworkStream::GetEmpireID()
{
	return m_dwEmpireID;
}

void CPythonNetworkStream::__ClearSelectCharacterData()
{
	memset(&m_akSimplePlayerInfo, 0, sizeof(m_akSimplePlayerInfo));

	for (int i = 0; i < PLAYER_PER_ACCOUNT4; ++i)
	{
		m_adwGuildID[i] = 0;
		m_astrGuildName[i] = "";
	}
}

void CPythonNetworkStream::__DirectEnterMode_Initialize()
{
	m_kDirectEnterMode.m_isSet = false;
	m_kDirectEnterMode.m_dwChrSlotIndex = 0;
}

void CPythonNetworkStream::__DirectEnterMode_Set(UINT uChrSlotIndex)
{
	m_kDirectEnterMode.m_isSet = true;
	m_kDirectEnterMode.m_dwChrSlotIndex = uChrSlotIndex;
}

bool CPythonNetworkStream::__DirectEnterMode_IsSet()
{
	return m_kDirectEnterMode.m_isSet;
}

void CPythonNetworkStream::__InitializeMarkAuth()
{
	m_kMarkAuth.m_dwHandle = 0;
	m_kMarkAuth.m_dwRandomKey = 0;
}

void CPythonNetworkStream::__BettingGuildWar_Initialize()
{
	m_kBettingGuildWar.m_dwBettingMoney = 0;
	m_kBettingGuildWar.m_dwObserverCount = 0;
}

void CPythonNetworkStream::__BettingGuildWar_SetObserverCount(UINT uObserverCount)
{
	m_kBettingGuildWar.m_dwObserverCount = uObserverCount;
}

void CPythonNetworkStream::__BettingGuildWar_SetBettingMoney(UINT uBettingMoney)
{
	m_kBettingGuildWar.m_dwBettingMoney = uBettingMoney;
}

DWORD CPythonNetworkStream::EXPORT_GetBettingGuildWarValue(const char* c_szValueName)
{
	if (stricmp(c_szValueName, "OBSERVER_COUNT") == 0)
		return m_kBettingGuildWar.m_dwObserverCount;

	if (stricmp(c_szValueName, "BETTING_MONEY") == 0)
		return m_kBettingGuildWar.m_dwBettingMoney;

	return 0;
}

void CPythonNetworkStream::__ServerTimeSync_Initialize()
{
	m_kServerTimeSync.m_dwChangeClientTime = 0;
	m_kServerTimeSync.m_dwChangeServerTime = 0;
}

void CPythonNetworkStream::SetWaitFlag()
{
	m_isWaitLoginKey = TRUE;
}

void CPythonNetworkStream::SendEmoticon(UINT eEmoticon)
{
	if (eEmoticon < m_EmoticonStringVector.size())
		SendChatPacket(m_EmoticonStringVector[eEmoticon].c_str());
	else
		assert(false && "SendEmoticon Error");
}

CPythonNetworkStream::CPythonNetworkStream()
{
	m_rokNetActorMgr = new CNetworkActorManager;

	memset(m_akSimplePlayerInfo, 0, sizeof(m_akSimplePlayerInfo));

	m_phaseProcessFunc.Clear();

	m_dwEmpireID = 0;
	m_dwGuildID = 0;

	m_dwMainActorVID = 0;
	m_dwMainActorRace = 0;
	m_dwMainActorEmpire = 0;
	m_dwMainActorSkillGroup = 0;
	m_poHandler = NULL;

	m_dwLastGamePingTime = 0;

	m_dwLoginKey = 0;
	m_isWaitLoginKey = FALSE;
	m_isStartGame = FALSE;
	m_isEnableChatInsultFilter = FALSE;
	m_bComboSkillFlag = FALSE;
	m_strPhase = "OffLine";

	__InitializeGamePhase();
	__InitializeMarkAuth();

	__DirectEnterMode_Initialize();
	__BettingGuildWar_Initialize();

	std::fill(m_apoPhaseWnd, m_apoPhaseWnd + PHASE_WINDOW_NUM, (PyObject*)NULL);
	m_poSerCommandParserWnd = NULL;

	SetOffLinePhase();
}

CPythonNetworkStream::~CPythonNetworkStream()
{
	Tracen("PythonNetworkMainStream Clear");
}
