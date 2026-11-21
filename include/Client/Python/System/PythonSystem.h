#pragma once
#include <Neoeter/Platform/Base/Singleton.h>
#include <Python.h>

class CPythonSystem : public CSingleton<CPythonSystem>
{
	public:
		enum EWindow
		{
			WINDOW_STATUS,
			WINDOW_INVENTORY,
			WINDOW_ABILITY,
			WINDOW_SOCIETY,
			WINDOW_JOURNAL,
			WINDOW_COMMAND,

			WINDOW_QUICK,
			WINDOW_GAUGE,
			WINDOW_MINIMAP,
			WINDOW_CHAT,

			WINDOW_MAX_NUM,
		};

		enum
		{
			FREQUENCY_MAX_NUM = 20,
			RESOLUTION_MAX_NUM = 100
		};

		typedef struct SResolution
		{
				uint32_t width;
				uint32_t height;
				uint32_t bpp; // bits per pixel (high-color = 16bpp, true-color = 32bpp)

				uint32_t frequency[20];
				uint8_t frequency_count;
		} TResolution;

		typedef struct SWindowStatus
		{
				int isVisible;
				int isMinimized;

				int ixPosition;
				int iyPosition;
				int iHeight;
		} TWindowStatus;

		typedef struct SConfig
		{
				uint32_t width;
				uint32_t height;
				uint32_t bpp;
				uint32_t frequency;

				bool is_software_cursor;
				bool is_object_culling;
				int iDistance;
				int iShadowLevel;

				float music_volume;
				uint8_t voice_volume;

				int gamma;

				int isSaveID;
				char SaveID[20];

				bool bWindowed;
				bool bDecompressDDS;
				bool bNoSoundCard;
				bool bUseDefaultIME;
				uint8_t bSoftwareTiling;
				bool bViewChat;
				bool bAlwaysShowName;
				bool bShowDamage;
				bool bShowSalesText;
		} TConfig;

	public:
		CPythonSystem();
		virtual ~CPythonSystem();

		void Clear();
		void SetInterfaceHandler(PyObject* poHandler);
		void DestroyInterfaceHandler();

		// Config
		void SetDefaultConfig();
		bool LoadConfig();
		bool SaveConfig();
		void ApplyConfig();
		void ReloadInterface();
		void SetConfig(TConfig* set_config);
		TConfig* GetConfig();
		void ChangeSystem();

		// Interface
		bool LoadInterfaceStatus();
		void SaveInterfaceStatus();
		bool isInterfaceConfig();
		const TWindowStatus& GetWindowStatusReference(int iIndex);

		uint32_t GetWidth();
		uint32_t GetHeight();
		uint32_t GetBPP();
		uint32_t GetFrequency();
		bool IsSoftwareCursor();
		bool IsWindowed();
		bool IsViewChat();
		bool IsAlwaysShowName();
		bool IsShowDamage();
		bool IsShowSalesText();
		bool IsUseDefaultIME();
		bool IsNoSoundCard();
		bool IsAutoTiling();
		bool IsSoftwareTiling();
		void SetSoftwareTiling(bool isEnable);
		void SetViewChatFlag(int iFlag);
		void SetAlwaysShowNameFlag(int iFlag);
		void SetShowDamageFlag(int iFlag);
		void SetShowSalesTextFlag(int iFlag);

		// Window
		void SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight);

		// SaveID
		int IsSaveID();
		const char* GetSaveID();
		void SetSaveID(int iValue, const char* c_szSaveID);

		/// Display
		void GetDisplaySettings();

		int GetResolutionCount();
		int GetFrequencyCount(int index);
		bool GetResolution(int index, OUT uint32_t* width, OUT uint32_t* height, OUT uint32_t* bpp);
		bool GetFrequency(int index, int freq_index, OUT uint32_t* frequncy);
		int GetResolutionIndex(uint32_t width, uint32_t height, uint32_t bpp);
		int GetFrequencyIndex(int res_index, uint32_t frequency);
		bool isViewCulling();

		// Sound
		float GetMusicVolume();
		int GetSoundVolume();
		void SetMusicVolume(float fVolume);
		void SetSoundVolumef(float fVolume);

		int GetDistance();
		int GetShadowLevel();
		void SetShadowLevel(unsigned int level);

	protected:
		TResolution m_ResolutionList[RESOLUTION_MAX_NUM];
		int m_ResolutionCount;

		TConfig m_Config;
		TConfig m_OldConfig;

		bool m_isInterfaceConfig;
		PyObject* m_poInterfaceHandler;
		TWindowStatus m_WindowStatus[WINDOW_MAX_NUM];
};
