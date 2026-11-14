#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonApplication.h>
#include <Client/UI/resource.h>
#include <Client/UI/SdlCursor.h>

#include <Neoeter/IO/EterPackManager.h>

namespace
{
	struct SCursorCell
	{
			int x;
			int y;
			int hotX;
			int hotY;
	};

	WORD ReadWord(const BYTE* p)
	{
		return WORD(p[0] | (p[1] << 8));
	}

	void DecodeDXT3Block(const BYTE* pSrc, DWORD* pDst, int dstPitch)
	{
		unsigned long long alphaBits = 0;
		for (int i = 0; i < 8; ++i)
			alphaBits |= (unsigned long long)pSrc[i] << (i * 8);

		const WORD color0 = ReadWord(pSrc + 8);
		const WORD color1 = ReadWord(pSrc + 10);

		const BYTE r0 = BYTE(((color0 >> 11) & 0x1f) * 255 / 31);
		const BYTE g0 = BYTE(((color0 >> 5) & 0x3f) * 255 / 63);
		const BYTE b0 = BYTE((color0 & 0x1f) * 255 / 31);
		const BYTE r1 = BYTE(((color1 >> 11) & 0x1f) * 255 / 31);
		const BYTE g1 = BYTE(((color1 >> 5) & 0x3f) * 255 / 63);
		const BYTE b1 = BYTE((color1 & 0x1f) * 255 / 31);

		const BYTE colors[4][3] =
			{
				{r0, g0, b0},
				{r1, g1, b1},
				{BYTE((2 * r0 + r1) / 3), BYTE((2 * g0 + g1) / 3), BYTE((2 * b0 + b1) / 3)},
				{BYTE((r0 + 2 * r1) / 3), BYTE((g0 + 2 * g1) / 3), BYTE((b0 + 2 * b1) / 3)},
			};

		DWORD indices = 0;
		for (int i = 0; i < 4; ++i)
			indices |= DWORD(pSrc[12 + i]) << (i * 8);

		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				const int pixel = y * 4 + x;
				const int colorIndex = (indices >> (pixel * 2)) & 0x3;
				const BYTE alpha = BYTE(((alphaBits >> (pixel * 4)) & 0xf) * 17);
				const BYTE* color = colors[colorIndex];

				pDst[y * dstPitch + x] = (DWORD(alpha) << 24) | (DWORD(color[0]) << 16) | (DWORD(color[1]) << 8) | color[2];
			}
		}
	}
}

bool CPythonApplication::CreateOfficialCursors()
{
	static const SCursorCell s_cells[CURSOR_COUNT] =
		{
			{0, 0, 0, 0},
			{32, 0, 0, 0},
			{32, 0, 0, 0},
			{96, 32, 0, 0},
			{0, 32, 0, 0},
			{32, 32, 0, 0},
			{160, 0, 0, 0},
			{128, 0, 0, 0},
			{128, 0, 0, 0},
			{64, 0, 0, 0},
			{64, 32, 0, 0},
			{96, 0, 0, 0},
			{192, 0, 16, 16},
			{128, 32, 16, 16},
			{224, 0, 16, 16},
		};

	CEterPackManager::TPackDataPtr data;
	if (!CEterPackManager::Instance().Get("d:/ymir work/ui/cursor.dds", data))
		return false;

	if (!data || data->size() < 128)
		return false;

	const BYTE* pFile = reinterpret_cast<const BYTE*>(data->data());
	if (memcmp(pFile, "DDS ", 4) != 0 || memcmp(pFile + 84, "DXT3", 4) != 0)
		return false;

	int width = 0;
	int height = 0;
	memcpy(&height, pFile + 12, sizeof(height));
	memcpy(&width, pFile + 16, sizeof(width));

	if (width < 256 || height < 64 || (width % 4) != 0 || (height % 4) != 0)
		return false;

	const int blockCount = (width / 4) * (height / 4);
	const size_t requiredSize = 128 + size_t(blockCount) * 16;
	if (data->size() < requiredSize)
		return false;

	std::vector<DWORD> atlas(size_t(width) * height, 0);
	const BYTE* pBlocks = pFile + 128;

	for (int blockY = 0; blockY < height / 4; ++blockY)
	{
		for (int blockX = 0; blockX < width / 4; ++blockX)
		{
			DWORD pixels[4 * 4] = {};
			DecodeDXT3Block(pBlocks, pixels, 4);
			pBlocks += 16;

			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; ++x)
					atlas[size_t(blockY * 4 + y) * width + blockX * 4 + x] = pixels[y * 4 + x];
			}
		}
	}

	m_CursorHandleMap.clear();

	for (int i = 0; i < CURSOR_COUNT; ++i)
	{
		SDL_Surface* pSurface = SDL_CreateRGBSurface(0, 32, 32, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		if (!pSurface)
		{
			DestroyCursors();
			return false;
		}

		bool hasAlpha = false;
		for (int y = 0; y < 32; ++y)
		{
			DWORD* pDst = reinterpret_cast<DWORD*>(static_cast<BYTE*>(pSurface->pixels) + y * pSurface->pitch);
			const DWORD* pSrc = &atlas[size_t(s_cells[i].y + y) * width + s_cells[i].x];

			for (int x = 0; x < 32; ++x)
			{
				pDst[x] = pSrc[x];
				if ((pSrc[x] >> 24) != 0)
					hasAlpha = true;
			}
		}

		if (!hasAlpha)
		{
			SDL_FreeSurface(pSurface);
			DestroyCursors();
			return false;
		}

		SDL_Cursor* pCursor = SDL_CreateColorCursor(pSurface, s_cells[i].hotX, s_cells[i].hotY);
		SDL_FreeSurface(pSurface);

		if (!pCursor)
		{
			DestroyCursors();
			return false;
		}

		m_CursorHandleMap.insert(TCursorHandleMap::value_type(i, reinterpret_cast<HANDLE>(pCursor)));
	}

	return true;
}

bool CPythonApplication::CreateCursors()
{
	m_bCursorVisible = TRUE;
	m_bLiarCursorOn = false;

	if (CreateOfficialCursors())
		return true;

	TraceError("CreateOfficialCursors failed, falling back to SDL system cursors");

	// The Win32 cursor resources are not part of the source tree, so the SDL
	// window/input path uses SDL system cursors on every target (T3.2).
	static const SDL_SystemCursor s_systemCursors[CURSOR_COUNT] =
		{
			SDL_SYSTEM_CURSOR_ARROW,	 // NORMAL
			SDL_SYSTEM_CURSOR_ARROW,	 // ATTACK
			SDL_SYSTEM_CURSOR_CROSSHAIR, // TARGET
			SDL_SYSTEM_CURSOR_HAND,		 // TALK
			SDL_SYSTEM_CURSOR_NO,		 // CANT_GO
			SDL_SYSTEM_CURSOR_CROSSHAIR, // PICK

			SDL_SYSTEM_CURSOR_HAND,	 // DOOR
			SDL_SYSTEM_CURSOR_ARROW, // CHAIR
			SDL_SYSTEM_CURSOR_ARROW, // MAGIC
			SDL_SYSTEM_CURSOR_HAND,	 // BUY
			SDL_SYSTEM_CURSOR_HAND,	 // SELL

			SDL_SYSTEM_CURSOR_SIZEALL,	// CAMERA_ROTATE
			SDL_SYSTEM_CURSOR_SIZEWE,	// HSIZE
			SDL_SYSTEM_CURSOR_SIZENS,	// VSIZE
			SDL_SYSTEM_CURSOR_SIZENWSE, // HVSIZE
		};

	m_CursorHandleMap.clear();

	for (int i = 0; i < CURSOR_COUNT; ++i)
	{
		HANDLE hCursor = Platform_CreateSystemCursor(s_systemCursors[i]);

		if (NULL == hCursor)
			return false;

		m_CursorHandleMap.insert(TCursorHandleMap::value_type(i, hCursor));
	}

	return true;
}

void CPythonApplication::DestroyCursors()
{
	TCursorHandleMap::iterator itor;
	for (itor = m_CursorHandleMap.begin(); itor != m_CursorHandleMap.end(); ++itor)
	{
		Platform_DestroyCursor(itor->second);
	}
}

void CPythonApplication::SetCursorVisible(BOOL bFlag, bool bLiarCursorOn)
{
	m_bCursorVisible = bFlag;
	m_bLiarCursorOn = bLiarCursorOn;

	if (CURSOR_MODE_HARDWARE == m_iCursorMode)
	{
		// SDL_ShowCursor returns the previous state (0/1) instead of the
		// Win32 reference counter, so it must be called once, not in the
		// Win32 do/while loop.
		Platform_ShowCursorFlag(m_bCursorVisible);
	}
}

BOOL CPythonApplication::GetCursorVisible()
{
	return m_bCursorVisible;
}

bool CPythonApplication::GetLiarCursorOn()
{
	return m_bLiarCursorOn;
}

int CPythonApplication::GetCursorMode()
{
	return m_iCursorMode;
}

BOOL CPythonApplication::__IsContinuousChangeTypeCursor(int iCursorNum)
{
	switch (iCursorNum)
	{
		case CURSOR_SHAPE_NORMAL:
		case CURSOR_SHAPE_ATTACK:
		case CURSOR_SHAPE_TARGET:
		case CURSOR_SHAPE_MAGIC:
		case CURSOR_SHAPE_BUY:
		case CURSOR_SHAPE_SELL:
			return TRUE;
			break;
	}

	return FALSE;
}

BOOL CPythonApplication::SetCursorNum(int iCursorNum)
{
	if (CURSOR_SHAPE_NORMAL == iCursorNum)
	{
		if (!__IsContinuousChangeTypeCursor(m_iCursorNum))
		{
			iCursorNum = m_iContinuousCursorNum;
		}
	}
	else
	{
		if (__IsContinuousChangeTypeCursor(m_iCursorNum)) // 현재 커서가 지속 커서일때만
		{
			m_iContinuousCursorNum = m_iCursorNum; // 현재의 커서를 저장한다.
		}
	}

	if (CURSOR_MODE_HARDWARE == m_iCursorMode)
	{
		TCursorHandleMap::iterator itor = m_CursorHandleMap.find(iCursorNum);
		if (m_CursorHandleMap.end() == itor)
			return FALSE;

		HANDLE hCursor = itor->second;

		Platform_SetCursorHandle(hCursor);
		m_hCurrentCursor = hCursor;
	}

	m_iCursorNum = iCursorNum;

	PyCallClassMemberFunc(m_poMouseHandler, "ChangeCursor", Py_BuildValue("(i)", m_iCursorNum));

	return TRUE;
}

void CPythonApplication::SetCursorMode(int iMode)
{
	switch (iMode)
	{
		case CURSOR_MODE_HARDWARE:
			m_iCursorMode = CURSOR_MODE_HARDWARE;
			Platform_ShowCursorFlag(TRUE);
			break;

		case CURSOR_MODE_SOFTWARE:
			m_iCursorMode = CURSOR_MODE_SOFTWARE;
			Platform_SetCursorHandle(NULL);
			Platform_ShowCursorFlag(FALSE);
			break;
	}
}
