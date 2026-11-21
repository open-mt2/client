#pragma once

#include <Neoeter/Platform/PortTypes.h>

#include <map>

class CPythonApplication;

class CApplicationCursor
{
	public:
		enum EMode
		{
			MODE_HARDWARE,
			MODE_SOFTWARE,
		};

		enum EShape
		{
			SHAPE_NORMAL,
			SHAPE_ATTACK,
			SHAPE_TARGET,
			SHAPE_TALK,
			SHAPE_CANT_GO,
			SHAPE_PICK,

			SHAPE_DOOR,
			SHAPE_CHAIR,
			SHAPE_MAGIC,
			SHAPE_BUY,
			SHAPE_SELL,

			SHAPE_CAMERA_ROTATE,
			SHAPE_HSIZE,
			SHAPE_VSIZE,
			SHAPE_HVSIZE,

			SHAPE_COUNT,

			NORMAL = SHAPE_NORMAL,
			ATTACK = SHAPE_ATTACK,
			TARGET = SHAPE_TARGET,
			CAMERA_ROTATE = SHAPE_CAMERA_ROTATE,
			CURSOR_COUNT = SHAPE_COUNT,
		};

		explicit CApplicationCursor(CPythonApplication& rkApp);

		bool Create();
		void Destroy();
		int32_t SetNum(int iCursorNum);
		void SetVisible(int32_t bFlag, bool bLiarCursorOn = false);
		int32_t GetVisible();
		bool GetLiarCursorOn();
		void SetMode(int iMode);
		int GetMode();
		int GetNum();

		void RestoreHandle();

	private:
		bool CreateOfficial();

		bool __IsContinuousChangeType(int iCursorNum);

		typedef std::map<int, PlatformHandle> TCursorHandleMap;

		CPythonApplication& m_rkApp;

		TCursorHandleMap m_kHandleMap;
		PlatformHandle m_hCurrentCursor;

		int32_t m_bVisible;
		bool m_bLiarCursorOn;
		int m_iMode;
		int m_iNum;
		int m_iContinuousNum;
};
