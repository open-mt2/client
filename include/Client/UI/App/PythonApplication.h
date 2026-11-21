#pragma once

#include <Neoeter/Render/MSApplication.h>
#include <Neoeter/Render/Input.h>
#include <Neoeter/Render/Profiler.h>
#include <Neoeter/Render/GrpDevice.h>
#include <Neoeter/Render/NetDevice.h>
#include <Neoeter/Render/GrpLightManager.h>
#include <Neoeter/Effect/EffectManager.h>
#include <Client/Game/RaceManager.h>
#include <Client/GameData/ItemManager.h>
#include <Client/Game/FlyingObjectManager.h>
#include <Client/Game/GameEventManager.h>
#include <Neoeter/Audio/SoundManager.h>

#include <Client/Feature/Event/PythonEventManager.h>
#include <Client/Player/PythonPlayer.h>
#include <Client/Feature/NonPlayer/PythonNonPlayer.h>
#include <Client/Feature/Minimap/PythonMiniMap.h>
#include <Client/Feature/Ime/PythonIME.h>
#include <Client/Feature/Item/PythonItem.h>
#include <Client/Feature/Shop/PythonShop.h>
#include <Client/Feature/Exchange/PythonExchange.h>
#include <Client/Feature/Chat/PythonChat.h>
#include <Client/Feature/TextTail/PythonTextTail.h>
#include <Client/Feature/Skill/PythonSkill.h>
#include <Client/Feature/System/PythonSystem.h>
// #include "PythonNetworkDatagram.h"
#include <Client/Session/PythonNetworkStream.h>
#include <Client/Feature/Character/PythonCharacterManager.h>
#include <Client/Feature/Quest/PythonQuest.h>
#include <Client/Feature/Messenger/PythonMessenger.h>
#include <Client/Feature/SafeBox/PythonSafeBox.h>
#include <Client/Feature/Guild/PythonGuild.h>

#include <Client/Feature/Guild/GuildMarkDownloader.h>
#include <Client/Feature/Guild/GuildMarkUploader.h>

#include <Client/UI/AccountConnector.h>

#include <Client/UI/ServerStateChecker.h>
#include <Client/UI/AbstractApplication.h>
#include <Client/UI/Camera/ApplicationCamera.h>
#include <Client/UI/Cursor/ApplicationCursor.h>

class CPythonApplication : public CMSApplication, public CInputKeyboard, public IAbstractApplication
{
	public:
		enum EDeviceState
		{
			DEVICE_STATE_FALSE,
			DEVICE_STATE_SKIP,
			DEVICE_STATE_OK,
		};

		enum EInfo
		{
			INFO_ACTOR,
			INFO_EFFECT,
			INFO_ITEM,
			INFO_TEXTTAIL,
		};

	public:
		CPythonApplication();
		virtual ~CPythonApplication();

	public:
		void NotifyHack(const char* c_szFormat, ...);
		void GetInfo(uint32_t eInfo, std::string* pstInfo);
		void GetMousePosition(POINT* ppt);

		static CPythonApplication& Instance()
		{
			assert(ms_pInstance != NULL);
			return *ms_pInstance;
		}

		void Loop();
		void Destroy();
		void Clear();
		void Exit();
		void Abort();

		void SetMinFog(float fMinFog);
		void SetFrameSkip(bool isEnable);
		void SkipRenderBuffering(uint32_t dwSleepMSec);

		bool Create(PyObject* poSelf, const char* c_szName, int width, int height, int Windowed);
		bool CreateStartupWindow();
		bool CreateDevice(int width, int height, int Windowed, int bit = 32, int frequency = 0);

		void UpdateGame();
		void RenderGame();

		bool Process();

		void UpdateClientRect();

		void SafeSetCapture();
		void SafeReleaseCapture();
		void ResetMouseCapture();

		CApplicationCursor& Cursor();

		void SetMouseHandler(PyObject* poMouseHandler);

		int GetWidth();
		int GetHeight();

		CApplicationCamera& Camera();

		void SetCenterPosition(float fx, float fy, float fz);

		void SetFPS(int iFPS);
		void SetServerTime(time_t tTime);
		time_t GetServerTime();
		time_t GetServerTimeStamp();
		float GetGlobalTime();
		float GetGlobalElapsedTime();

		float GetFaceSpeed() { return m_fFaceSpd; }
		float GetAveRenderTime() { return m_fAveRenderTime; }
		uint32_t GetCurRenderTime() { return m_dwCurRenderTime; }
		uint32_t GetCurUpdateTime() { return m_dwCurUpdateTime; }
		uint32_t GetUpdateFPS() { return m_dwUpdateFPS; }
		uint32_t GetRenderFPS() { return m_dwRenderFPS; }
		uint32_t GetLoad() { return m_dwLoad; }
		uint32_t GetFaceCount() { return m_dwFaceCount; }

		void SetConnectData(const char* c_szIP, int iPort);
		void GetConnectData(std::string& rstIP, int& riPort);

		void RunIMEUpdate();
		void RunIMETabEvent();
		void RunIMEReturnEvent();
		void RunPressExitKey();

		void RunIMEChangeCodePage();
		void RunIMEOpenCandidateListEvent();
		void RunIMECloseCandidateListEvent();
		void RunIMEOpenReadingWndEvent();
		void RunIMECloseReadingWndEvent();

		void SetEventCamera(const SCameraSetting& c_rCameraSetting);
		void BlendEventCamera(const SCameraSetting& c_rCameraSetting, float fBlendTime);
		void SetDefaultCamera();

		void SetForceSightRange(int iRange);

	protected:
		intptr_t WindowProcedure(PlatformWindow hWnd, uint32_t uiMsg, uintptr_t wParam, intptr_t lParam);

		void InitializeDebugRows();

		void OnUIUpdate();
		void OnUIRender();

		void OnMouseUpdate();
		void OnMouseRender();

		void OnMouseWheel(int nLen);
		void OnMouseMove(int x, int y);
		void OnMouseMiddleButtonDown(int x, int y);
		void OnMouseMiddleButtonUp(int x, int y);
		void OnMouseLeftButtonDown(int x, int y);
		void OnMouseLeftButtonUp(int x, int y);
		void OnMouseLeftButtonDoubleClick(int x, int y);
		void OnMouseRightButtonDown(int x, int y);
		void OnMouseRightButtonUp(int x, int y);
		void OnSizeChange(int width, int height);
		void OnKeyDown(int iIndex);
		void OnKeyUp(int iIndex);
		void OnIMEKeyDown(int iIndex);

		int CheckDeviceState();

		bool SetWindowFullscreen(bool enabled, int width, int height);
		void SetResolution(int width, int height);
		void MinimizeWindow();

	protected:
		CTimer m_timer;

		CLightManager m_LightManager;
		CSoundManager m_SoundManager;
		CFlyingManager m_FlyingManager;
		CRaceManager m_RaceManager;
		CGameEventManager m_GameEventManager;
		CItemManager m_kItemMgr;

		UI::CWindowManager m_kWndMgr;
		CEffectManager m_kEftMgr;
		CPythonCharacterManager m_kChrMgr;

		CServerStateChecker m_kServerStateChecker;
		CPythonGraphic m_pyGraphic;
		CPythonNetworkStream m_pyNetworkStream;
		// CPythonNetworkDatagram		m_pyNetworkDatagram;
		CPythonPlayer m_pyPlayer;
		CPythonIME m_pyIme;
		CPythonItem m_pyItem;
		CPythonShop m_pyShop;
		CPythonExchange m_pyExchange;
		CPythonChat m_pyChat;
		CPythonTextTail m_pyTextTail;
		CPythonNonPlayer m_pyNonPlayer;
		CPythonMiniMap m_pyMiniMap;
		CPythonEventManager m_pyEventManager;
		CPythonBackground m_pyBackground;
		CPythonSkill m_pySkill;
		CPythonResource m_pyRes;
		CPythonQuest m_pyQuest;
		CPythonMessenger m_pyManager;
		CPythonSafeBox m_pySafeBox;
		CPythonGuild m_pyGuild;

		CGuildMarkManager m_kGuildMarkManager;
		CGuildMarkDownloader m_kGuildMarkDownloader;
		CGuildMarkUploader m_kGuildMarkUploader;
		CAccountConnector m_kAccountConnector;

		CGraphicDevice m_grpDevice;
		CNetworkDevice m_netDevice;

		CPythonSystem m_pySystem;

		PyObject* m_poMouseHandler;

		unsigned int m_iFPS;
		bool m_isDebugLeftButtonCaptured;
		bool m_isDebugRightButtonCaptured;
		bool m_isDebugMiddleButtonCaptured;
		float m_fAveRenderTime;
		uint32_t m_dwCurRenderTime;
		uint32_t m_dwCurUpdateTime;
		uint32_t m_dwLoad;
		uint32_t m_dwWidth;
		uint32_t m_dwHeight;
		int m_iCameraBaseWidth;
		int m_iCameraBaseHeight;
		bool m_isUIResolutionReloadPending;
		int m_iPendingWidth;
		int m_iPendingHeight;

	protected:
		// Time
		uint32_t m_dwLastIdleTime;
		uint32_t m_dwStartLocalTime;
		time_t m_tServerTime;
		time_t m_tLocalStartTime;
		float m_fGlobalTime;
		float m_fGlobalElapsedTime;

		/////////////////////////////////////////////////////////////
		// Camera
		friend class CApplicationCamera;
		CApplicationCamera m_kCamera;
		// Camera
		/////////////////////////////////////////////////////////////

		float m_fFaceSpd;
		uint32_t m_dwFaceSpdSum;
		uint32_t m_dwFaceSpdCount;

		uint32_t m_dwFaceAccCount;
		uint32_t m_dwFaceAccTime;

		uint32_t m_dwUpdateFPS;
		uint32_t m_dwRenderFPS;
		uint32_t m_dwFaceCount;

		uint32_t m_dwLButtonDownTime;
		uint32_t m_dwLButtonUpTime;

		bool m_isWindowed;
		bool m_isFrameSkipDisable;

		// Connect Data
		std::string m_strIP;
		int m_iPort;

		static CPythonApplication* ms_pInstance;

		bool m_isMinimizedWnd;
		bool m_isActivateWnd;
		int32_t m_isWindowFullScreenEnable;

		uint32_t m_dwStickyKeysFlag;
		uint32_t m_dwBufSleepSkipTime;
		int m_iForceSightRange;

	protected:
		friend class CApplicationCursor;
		CApplicationCursor m_kCursor;
};
