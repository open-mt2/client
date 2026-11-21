// AreaLoaderThread.h: interface for the CAreaLoaderThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREALOADERTHREAD_H__E43FBE42_42F4_4F0E_B9DA_D7B7C5EA0753__INCLUDED_)
#define AFX_AREALOADERTHREAD_H__E43FBE42_42F4_4F0E_B9DA_D7B7C5EA0753__INCLUDED_

#include <Neoeter/Render/Mutex.h>

#include <condition_variable>
#include <mutex>
#include <thread>

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
		uint32_t Run(void* arg);

		void* Arg() const { return m_pArg; }
		void Arg(void* arg) { m_pArg = arg; }

		std::thread m_thread;

	private:
		void* m_pArg;

	protected:
		uint32_t Execute(void* pvArg);
		void Destroy();
		void ProcessTerrain();
		void ProcessArea();
		bool HasQueuedWork();

	private:
		std::deque<CTerrain*> m_pTerrainRequestDeque;
		Mutex m_TerrainRequestMutex;

		std::deque<CTerrain*> m_pTerrainCompleteDeque;
		Mutex m_TerrainCompleteMutex;

		std::deque<CArea*> m_pAreaRequestDeque;
		Mutex m_AreaRequestMutex;

		std::deque<CArea*> m_pAreaCompleteDeque;
		Mutex m_AreaCompleteMutex;

		std::mutex m_waitMutex;
		std::condition_variable m_workAvailable;
		bool m_workPending;
		bool m_bShutdowned;
		bool m_finished;
};

#endif // !defined(AFX_AREALOADERTHREAD_H__E43FBE42_42F4_4F0E_B9DA_D7B7C5EA0753__INCLUDED_)
