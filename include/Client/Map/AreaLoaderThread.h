// AreaLoaderThread.h: interface for the CAreaLoaderThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREALOADERTHREAD_H__E43FBE42_42F4_4F0E_B9DA_D7B7C5EA0753__INCLUDED_)
#define AFX_AREALOADERTHREAD_H__E43FBE42_42F4_4F0E_B9DA_D7B7C5EA0753__INCLUDED_

#include <Neoeter/Platform/PortTypes.h>
#include <Neoeter/Render/Mutex.h>

#include <atomic>

class CTerrain;
class CArea;

class TEMP_CAreaLoaderThread
{
	public:
		TEMP_CAreaLoaderThread();
		virtual ~TEMP_CAreaLoaderThread();

		bool Create(void* arg);
		void Shutdown();

		void Request(CTerrain* pTerrain);

		bool Fetch(CTerrain** ppTerrian);

		void Request(CArea* pArea);

		bool Fetch(CArea** ppArea);

	protected:
		static uint32_t EntryPoint(void* pThis);
		uint32_t Run(void* arg);

		void* Arg() const { return m_pArg; }
		void Arg(void* arg) { m_pArg = arg; }

		PlatformThreadHandle m_hThread;

	private:
		void* m_pArg;

	protected:
		uint32_t Execute(void* pvArg);
		void Destroy();
		void ProcessTerrain();
		void ProcessArea();

	private:
		std::deque<CTerrain*> m_pTerrainRequestDeque;
		Mutex m_TerrainRequestMutex;

		std::deque<CTerrain*> m_pTerrainCompleteDeque;
		Mutex m_TerrainCompleteMutex;

		std::deque<CArea*> m_pAreaRequestDeque;
		Mutex m_AreaRequestMutex;

		std::deque<CArea*> m_pAreaCompleteDeque;
		Mutex m_AreaCompleteMutex;

		PlatformSemaphoreHandle m_hSemaphore;
		std::atomic<bool> m_bShutdowned;
};

#endif // !defined(AFX_AREALOADERTHREAD_H__E43FBE42_42F4_4F0E_B9DA_D7B7C5EA0753__INCLUDED_)
