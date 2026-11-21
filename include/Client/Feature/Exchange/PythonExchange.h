#pragma once

#include <Client/Protocol/Packet.h>

/*
 *	교환 창 관련
 */
class CPythonExchange : public CSingleton<CPythonExchange>
{
	public:
		enum
		{
			EXCHANGE_ITEM_MAX_NUM = 12,
		};

		typedef struct trade
		{
				char name[CHARACTER_NAME_MAX_LEN + 1];

				uint32_t item_vnum[EXCHANGE_ITEM_MAX_NUM];
				uint8_t item_count[EXCHANGE_ITEM_MAX_NUM];
				uint32_t item_metin[EXCHANGE_ITEM_MAX_NUM][ITEM_SOCKET_SLOT_MAX_NUM];
				TPlayerItemAttribute item_attr[EXCHANGE_ITEM_MAX_NUM][ITEM_ATTRIBUTE_SLOT_MAX_NUM];

				uint8_t accept;
				uint32_t elk;
		} TExchangeData;

	public:
		CPythonExchange();
		virtual ~CPythonExchange();

		void Clear();

		void Start();
		void End();
		bool isTrading();

		// Interface

		void SetSelfName(const char* name);
		void SetTargetName(const char* name);

		char* GetNameFromSelf();
		char* GetNameFromTarget();

		void SetElkToTarget(uint32_t elk);
		void SetElkToSelf(uint32_t elk);

		uint32_t GetElkFromTarget();
		uint32_t GetElkFromSelf();

		void SetItemToTarget(uint32_t pos, uint32_t vnum, uint8_t count);
		void SetItemToSelf(uint32_t pos, uint32_t vnum, uint8_t count);

		void SetItemMetinSocketToTarget(int pos, int imetinpos, uint32_t vnum);
		void SetItemMetinSocketToSelf(int pos, int imetinpos, uint32_t vnum);

		void SetItemAttributeToTarget(int pos, int iattrpos, uint8_t byType, short sValue);
		void SetItemAttributeToSelf(int pos, int iattrpos, uint8_t byType, short sValue);

		void DelItemOfTarget(uint8_t pos);
		void DelItemOfSelf(uint8_t pos);

		uint32_t GetItemVnumFromTarget(uint8_t pos);
		uint32_t GetItemVnumFromSelf(uint8_t pos);

		uint8_t GetItemCountFromTarget(uint8_t pos);
		uint8_t GetItemCountFromSelf(uint8_t pos);

		uint32_t GetItemMetinSocketFromTarget(uint8_t pos, int iMetinSocketPos);
		uint32_t GetItemMetinSocketFromSelf(uint8_t pos, int iMetinSocketPos);

		void GetItemAttributeFromTarget(uint8_t pos, int iAttrPos, uint8_t* pbyType, short* psValue);
		void GetItemAttributeFromSelf(uint8_t pos, int iAttrPos, uint8_t* pbyType, short* psValue);

		void SetAcceptToTarget(uint8_t Accept);
		void SetAcceptToSelf(uint8_t Accept);

		bool GetAcceptFromTarget();
		bool GetAcceptFromSelf();

		bool GetElkMode();
		void SetElkMode(bool value);

	protected:
		bool m_isTrading;

		bool m_elk_mode; // 엘크를 클릭해서 교환했을때를 위한 변종임.
		TExchangeData m_self;
		TExchangeData m_victim;
};
