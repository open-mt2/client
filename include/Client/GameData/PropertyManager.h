#pragma once

class CPropertyManager : public CSingleton<CPropertyManager>
{
	public:
		CPropertyManager();
		virtual ~CPropertyManager();

		void Clear();

		bool LoadReservedCRC(const char* c_pszFileName);
		void ReserveCRC(uint32_t dwCRC);
		uint32_t GetUniqueCRC(const char* c_szSeed);

		void Initialize();
		bool Register(const char* c_pszFileName, CProperty** ppProperty = NULL);

		bool Get(uint32_t dwCRC, CProperty** ppProperty);
		bool Get(const char* c_pszFileName, CProperty** ppProperty);

	protected:
		typedef std::map<uint32_t, CProperty*> TPropertyCRCMap;
		typedef std::set<uint32_t> TCRCSet;

		TPropertyCRCMap m_PropertyByCRCMap;
		TCRCSet m_ReservedCRCSet;
};
