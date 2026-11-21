#include <Client/UI/StdAfx.h>
#include <Client/Feature/Guild/GuildMarkDownloader.h>
#include <Client/Feature/Character/PythonCharacterManager.h>
#include <Client/Protocol/Packet.h>
#include <Client/Utils/Test.h>

// MARK_BUG_FIX
struct SMarkIndex
{
		uint16_t guild_id;
		uint16_t mark_id;
};

// END_OFMARK_BUG_FIX

CGuildMarkDownloader::CGuildMarkDownloader()
{
	SetRecvBufferSize(640 * 1024);
	SetSendBufferSize(1024);
	__Initialize();
}

CGuildMarkDownloader::~CGuildMarkDownloader()
{
	__OfflineState_Set();
}

bool CGuildMarkDownloader::Connect(const CNetworkAddress& c_rkNetAddr, uint32_t dwHandle, uint32_t dwRandomKey)
{
	__OfflineState_Set();

	m_dwHandle = dwHandle;
	m_dwRandomKey = dwRandomKey;
	m_dwTodo = TODO_RECV_MARK;
	return CNetworkStream::Connect(c_rkNetAddr);
}

bool CGuildMarkDownloader::ConnectToRecvSymbol(const CNetworkAddress& c_rkNetAddr, uint32_t dwHandle, uint32_t dwRandomKey, const std::vector<uint32_t>& c_rkVec_dwGuildID)
{
	__OfflineState_Set();

	m_dwHandle = dwHandle;
	m_dwRandomKey = dwRandomKey;
	m_dwTodo = TODO_RECV_SYMBOL;
	m_kVec_dwGuildID = c_rkVec_dwGuildID;
	return CNetworkStream::Connect(c_rkNetAddr);
}

void CGuildMarkDownloader::Process()
{
	CNetworkStream::Process();

	if (!__StateProcess())
	{
		__OfflineState_Set();
		Disconnect();
	}
}

void CGuildMarkDownloader::OnConnectFailure()
{
	__OfflineState_Set();
}

void CGuildMarkDownloader::OnConnectSuccess()
{
	__LoginState_Set();
}

void CGuildMarkDownloader::OnRemoteDisconnect()
{
	__OfflineState_Set();
}

void CGuildMarkDownloader::OnDisconnect()
{
	__OfflineState_Set();
}

void CGuildMarkDownloader::__Initialize()
{
	m_eState = STATE_OFFLINE;
	m_pkMarkMgr = NULL;
	m_currentRequestingImageIndex = 0;
	m_dwBlockIndex = 0;
	m_dwBlockDataPos = 0;
	m_dwBlockDataSize = 0;

	m_dwHandle = 0;
	m_dwRandomKey = 0;
	m_dwTodo = TODO_RECV_NONE;
	m_kVec_dwGuildID.clear();
}

bool CGuildMarkDownloader::__StateProcess()
{
	switch (m_eState)
	{
		case STATE_LOGIN:
			return __LoginState_Process();
			break;
		case STATE_COMPLETE:
			return false;
	}

	return true;
}

void CGuildMarkDownloader::__OfflineState_Set()
{
	__Initialize();
}

void CGuildMarkDownloader::__CompleteState_Set()
{
	m_eState = STATE_COMPLETE;
	CPythonCharacterManager::instance().RefreshAllGuildMark();
}

void CGuildMarkDownloader::__LoginState_Set()
{
	m_eState = STATE_LOGIN;
}

bool CGuildMarkDownloader::__LoginState_Process()
{
	uint8_t header;

	if (!Peek(sizeof(uint8_t), &header))
		return true;

	uint32_t needPacketSize = __GetPacketSize(header);

	if (!needPacketSize)
		return false;

	if (!Peek(needPacketSize))
		return true;

	__DispatchPacket(header);
	return true;
}

// MARK_BUG_FIX
uint32_t CGuildMarkDownloader::__GetPacketSize(uint32_t header)
{
	switch (header)
	{
		case HEADER_GC_PHASE:
			return sizeof(TPacketGCPhase);
		case HEADER_GC_HANDSHAKE:
			return sizeof(TPacketGCHandshake);
		case HEADER_GC_PING:
			return sizeof(TPacketGCPing);
		case HEADER_GC_MARK_IDXLIST:
			return sizeof(TPacketGCMarkIDXList);
		case HEADER_GC_MARK_BLOCK:
			return sizeof(TPacketGCMarkBlock);
		case HEADER_GC_GUILD_SYMBOL_DATA:
			return sizeof(TPacketGCGuildSymbolData);
		case HEADER_GC_MARK_DIFF_DATA: // 사용하지 않음
			return sizeof(uint8_t);
	}
	return 0;
}

bool CGuildMarkDownloader::__DispatchPacket(uint32_t header)
{
	switch (header)
	{
		case HEADER_GC_PHASE:
			return __LoginState_RecvPhase();
		case HEADER_GC_HANDSHAKE:
			return __LoginState_RecvHandshake();
		case HEADER_GC_PING:
			return __LoginState_RecvPing();
		case HEADER_GC_MARK_IDXLIST:
			return __LoginState_RecvMarkIndex();
		case HEADER_GC_MARK_BLOCK:
			return __LoginState_RecvMarkBlock();
		case HEADER_GC_GUILD_SYMBOL_DATA:
			return __LoginState_RecvSymbolData();
		case HEADER_GC_MARK_DIFF_DATA: // 사용하지 않음
			return true;
	}
	return false;
}
// END_OF_MARK_BUG_FIX

bool CGuildMarkDownloader::__LoginState_RecvHandshake()
{
	TPacketGCHandshake kPacketHandshake;
	if (!Recv(sizeof(kPacketHandshake), &kPacketHandshake))
		return false;

	TPacketCGMarkLogin kPacketMarkLogin;

	kPacketMarkLogin.header = HEADER_CG_MARK_LOGIN;
	kPacketMarkLogin.handle = m_dwHandle;
	kPacketMarkLogin.random_key = m_dwRandomKey;

	if (!Send(sizeof(kPacketMarkLogin), &kPacketMarkLogin))
		return false;

	return true;
}

bool CGuildMarkDownloader::__LoginState_RecvPing()
{
	TPacketGCPing kPacketPing;

	if (!Recv(sizeof(kPacketPing), &kPacketPing))
		return false;

	TPacketCGPong kPacketPong;
	kPacketPong.bHeader = HEADER_CG_PONG;

	if (!Send(sizeof(TPacketCGPong), &kPacketPong))
		return false;

	return SendSequence();
}

bool CGuildMarkDownloader::__LoginState_RecvPhase()
{
	TPacketGCPhase kPacketPhase;

	if (!Recv(sizeof(kPacketPhase), &kPacketPhase))
		return false;

	if (kPacketPhase.phase == PHASE_LOGIN)
	{
		switch (m_dwTodo)
		{
			case TODO_RECV_NONE:
			{
				assert(!"CGuildMarkDownloader::__LoginState_RecvPhase - Todo type is none");
				break;
			}
			case TODO_RECV_MARK:
			{
				// MARK_BUG_FIX
				if (!__SendMarkIDXList())
					return false;
				// END_OF_MARK_BUG_FIX
				break;
			}
			case TODO_RECV_SYMBOL:
			{
				if (!__SendSymbolCRCList())
					return false;
				break;
			}
		}
	}

	return true;
}

// MARK_BUG_FIX
bool CGuildMarkDownloader::__SendMarkIDXList()
{
	TPacketCGMarkIDXList kPacketMarkIDXList;
	kPacketMarkIDXList.header = HEADER_CG_MARK_IDXLIST;
	if (!Send(sizeof(kPacketMarkIDXList), &kPacketMarkIDXList))
		return false;

	return true;
}

bool CGuildMarkDownloader::__LoginState_RecvMarkIndex()
{
	TPacketGCMarkIDXList kPacketMarkIndex;

	if (!Peek(sizeof(kPacketMarkIndex), &kPacketMarkIndex))
		return false;

	// uint32_t bufSize = sizeof(uint16_t) * 2 * kPacketMarkIndex.count;

	if (!Peek(kPacketMarkIndex.bufSize))
		return false;

	Recv(sizeof(kPacketMarkIndex));

	uint16_t guildID, markID;

	for (uint32_t i = 0; i < kPacketMarkIndex.count; ++i)
	{
		Recv(sizeof(uint16_t), &guildID);
		Recv(sizeof(uint16_t), &markID);

		// 길드ID -> 마크ID 인덱스 등록
		CGuildMarkManager::Instance().AddMarkIDByGuildID(guildID, markID);
	}

	// 모든 마크 이미지 파일을 로드한다. (파일이 없으면 만들어짐)
	CGuildMarkManager::Instance().LoadMarkImages();

	m_currentRequestingImageIndex = 0;
	__SendMarkCRCList();
	return true;
}

bool CGuildMarkDownloader::__SendMarkCRCList()
{
	TPacketCGMarkCRCList kPacketMarkCRCList;

	if (!CGuildMarkManager::Instance().GetBlockCRCList(m_currentRequestingImageIndex, kPacketMarkCRCList.crclist))
		__CompleteState_Set();
	else
	{
		kPacketMarkCRCList.header = HEADER_CG_MARK_CRCLIST;
		kPacketMarkCRCList.imgIdx = m_currentRequestingImageIndex;
		++m_currentRequestingImageIndex;

		if (!Send(sizeof(kPacketMarkCRCList), &kPacketMarkCRCList))
			return false;
	}
	return true;
}

bool CGuildMarkDownloader::__LoginState_RecvMarkBlock()
{
	TPacketGCMarkBlock kPacket;

	if (!Peek(sizeof(kPacket), &kPacket))
		return false;

	if (!Peek(kPacket.bufSize))
		return false;

	Recv(sizeof(kPacket));

	uint8_t posBlock;
	uint32_t compSize;
	char compBuf[SGuildMarkBlock::MAX_COMP_SIZE];

	for (uint32_t i = 0; i < kPacket.count; ++i)
	{
		Recv(sizeof(uint8_t), &posBlock);
		Recv(sizeof(uint32_t), &compSize);

		if (compSize > SGuildMarkBlock::MAX_COMP_SIZE)
		{
			TraceError("RecvMarkBlock: data corrupted");
			Recv(compSize);
		}
		else
		{
			Recv(compSize, compBuf);
			// 압축된 이미지를 실제로 저장한다. CRC등 여러가지 정보가 함께 빌드된다.
			CGuildMarkManager::Instance().SaveBlockFromCompressedData(kPacket.imgIdx, posBlock, (const uint8_t*)compBuf, compSize);
		}
	}

	if (kPacket.count > 0)
	{
		// 마크 이미지 저장
		CGuildMarkManager::Instance().SaveMarkImage(kPacket.imgIdx);

		// 리소스 리로딩 (재접속을 안해도 본인것은 잘 보이게 함)
		std::string imagePath;

		if (CGuildMarkManager::Instance().GetMarkImageFilename(kPacket.imgIdx, imagePath))
		{
			CResource* pResource = CResourceManager::Instance().GetResourcePointer(imagePath.c_str());
			if (pResource->IsType(CGraphicImage::Type()))
			{
				CGraphicImage* pkGrpImg = static_cast<CGraphicImage*>(pResource);
				pkGrpImg->Reload();
			}
		}
	}

	// 더 요청할 것이 있으면 요청하고 아니면 이미지를 저장하고 종료
	if (m_currentRequestingImageIndex < CGuildMarkManager::Instance().GetMarkImageCount())
		__SendMarkCRCList();
	else
		__CompleteState_Set();

	return true;
}
// END_OF_MARK_BUG_FIX

bool CGuildMarkDownloader::__SendSymbolCRCList()
{
	for (uint32_t i = 0; i < m_kVec_dwGuildID.size(); ++i)
	{
		TPacketCGSymbolCRC kSymbolCRCPacket;
		kSymbolCRCPacket.header = HEADER_CG_GUILD_SYMBOL_CRC;
		kSymbolCRCPacket.dwGuildID = m_kVec_dwGuildID[i];

		std::string strFileName = GetGuildSymbolFileName(m_kVec_dwGuildID[i]);
		kSymbolCRCPacket.dwCRC = GetFileCRC32(strFileName.c_str());
		kSymbolCRCPacket.dwSize = GetFileSize(strFileName.c_str());
#ifdef _DEBUG
		printf("__SendSymbolCRCList [GuildID:%d / CRC:%u]\n", m_kVec_dwGuildID[i], kSymbolCRCPacket.dwCRC);
#endif
		if (!Send(sizeof(kSymbolCRCPacket), &kSymbolCRCPacket))
			return false;
	}

	return true;
}

bool CGuildMarkDownloader::__LoginState_RecvSymbolData()
{
	TPacketGCBlankDynamic packet;
	if (!Peek(sizeof(TPacketGCBlankDynamic), &packet))
		return true;

#ifdef _DEBUG
	printf("__LoginState_RecvSymbolData [%d/%d]\n", GetRecvBufferSize(), packet.size);
#endif
	if (packet.size > GetRecvBufferSize())
		return true;

	//////////////////////////////////////////////////////////////

	TPacketGCGuildSymbolData kPacketSymbolData;
	if (!Recv(sizeof(kPacketSymbolData), &kPacketSymbolData))
		return false;

	uint16_t wDataSize = kPacketSymbolData.size - sizeof(kPacketSymbolData);
	uint32_t dwGuildID = kPacketSymbolData.guild_id;
	uint8_t* pbyBuf = new uint8_t[wDataSize];

	if (!Recv(wDataSize, pbyBuf))
	{
		delete[] pbyBuf;
		return false;
	}

	MyCreateDirectory(g_strGuildSymbolPathName.c_str());

	std::string strFileName = GetGuildSymbolFileName(dwGuildID);

	FILE* File = fopen(strFileName.c_str(), "wb");
	if (!File)
	{
		delete[] pbyBuf;
		return false;
	}
	fwrite(pbyBuf, wDataSize, 1, File);
	fclose(File);

#ifdef _DEBUG
	printf("__LoginState_RecvSymbolData(filename:%s, datasize:%d, guildid:%d)\n", strFileName.c_str(), wDataSize, dwGuildID);
#endif

	delete[] pbyBuf;
	return true;
}
