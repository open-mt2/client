#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonApplication.h>
#include <Neoeter/Debug/Debug.h>
#include <Neoeter/Render/Camera.h>
#include <Neoeter/Render/SdlPlatform.h>

void CPythonApplication::OnCameraUpdate()
{
	if (m_pyBackground.IsMapReady())
	{
		CCamera* pkCameraMgr = CCameraManager::Instance().GetCurrentCamera();
		if (pkCameraMgr)
		{
			if (m_iCameraBaseWidth > 0 && m_iCameraBaseHeight > 0 && m_dwWidth > 0 && m_dwHeight > 0)
			{
				pkCameraMgr->SetMouseDragScale(
					float(m_iCameraBaseWidth) / float(m_dwWidth),
					float(m_iCameraBaseHeight) / float(m_dwHeight));
			}

			pkCameraMgr->Update();
		}
	}
}

void CPythonApplication::OnUIUpdate()
{
	UI::CWindowManager& rkUIMgr = UI::CWindowManager::Instance();
	rkUIMgr.Update();
}

void CPythonApplication::OnUIRender()
{
	UI::CWindowManager& rkUIMgr = UI::CWindowManager::Instance();
	rkUIMgr.Render();
}

void CPythonApplication::OnSizeChange(int width, int height)
{
	if (m_iCameraBaseWidth <= 0 || m_iCameraBaseHeight <= 0)
	{
		m_iCameraBaseWidth = width;
		m_iCameraBaseHeight = height;
	}

	m_dwWidth = static_cast<DWORD>(width);
	m_dwHeight = static_cast<DWORD>(height);

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.SetResolution(width, height);

	if (rkWndMgr.GetScreenWidth() <= 0 || rkWndMgr.GetScreenHeight() <= 0)
		rkWndMgr.SetScreenSize(width, height);
}

void CPythonApplication::OnMouseMiddleButtonDown(int x, int y)
{
	if (Neoeter::Debug::WantsMouseAt(x, y))
	{
		m_isDebugMiddleButtonCaptured = true;
		return;
	}

	CCameraManager& rkCmrMgr = CCameraManager::Instance();
	CCamera* pkCmrCur = rkCmrMgr.GetCurrentCamera();
	if (pkCmrCur)
		pkCmrCur->BeginDrag(x, y);

	if (!m_pyBackground.IsMapReady())
		return;

	SetCursorNum(CAMERA_ROTATE);
	if (CURSOR_MODE_HARDWARE == GetCursorMode())
		SetCursorVisible(FALSE, true);
}

void CPythonApplication::OnMouseMiddleButtonUp(int x, int y)
{
	if (m_isDebugMiddleButtonCaptured)
	{
		m_isDebugMiddleButtonCaptured = false;
		return;
	}

	CCameraManager& rkCmrMgr = CCameraManager::Instance();
	CCamera* pkCmrCur = rkCmrMgr.GetCurrentCamera();
	if (pkCmrCur)
		pkCmrCur->EndDrag();

	if (!m_pyBackground.IsMapReady())
		return;

	SetCursorNum(NORMAL);
	if (CURSOR_MODE_HARDWARE == GetCursorMode())
		SetCursorVisible(TRUE);
}

void CPythonApplication::OnMouseWheel(int nLen)
{
	if (Neoeter::Debug::WantsMouse())
		return;

	CCameraManager& rkCmrMgr = CCameraManager::Instance();
	CCamera* pkCmrCur = rkCmrMgr.GetCurrentCamera();
	if (pkCmrCur)
		pkCmrCur->Wheel(nLen);
}

void CPythonApplication::OnMouseMove(int x, int y)
{
#if defined(_WIN32)
	int deltaX = 0;
	int deltaY = 0;
	SDL_GetRelativeMouseState(&deltaX, &deltaY);
#endif

	CCameraManager& rkCmrMgr = CCameraManager::Instance();
	CCamera* pkCmrCur = rkCmrMgr.GetCurrentCamera();

	if (pkCmrCur && pkCmrCur->IsDraging())
	{
		const Uint32 buttons = SDL_GetGlobalMouseState(NULL, NULL);
		if (!(buttons & (SDL_BUTTON_LMASK | SDL_BUTTON_MMASK | SDL_BUTTON_RMASK)))
		{
			pkCmrCur->EndDrag();

			if (m_pyBackground.IsMapReady())
			{
				SetCursorNum(NORMAL);
				if (CURSOR_MODE_HARDWARE == GetCursorMode())
					SetCursorVisible(TRUE);
			}
		}
#if defined(_WIN32)
		else
		{
			if (m_pyBackground.IsMapReady())
				pkCmrCur->DragRelative(deltaX, deltaY);

			RECT rcDrag;
			GetClientRect(&rcDrag);
			UI::CWindowManager::Instance().SetResolution(rcDrag.right - rcDrag.left, rcDrag.bottom - rcDrag.top);
			return;
		}
#endif
	}

	if (Neoeter::Debug::WantsMouseAt(x, y))
	{
		if (CURSOR_MODE_SOFTWARE == GetCursorMode())
			UI::CWindowManager::Instance().RunMouseMove(x, y);

		return;
	}

#if !defined(_WIN32)
	POINT Point;
	if (pkCmrCur)
	{
		if (CPythonBackground::Instance().IsMapReady() && pkCmrCur->Drag(x, y, &Point))
		{
			x = Point.x;
			y = Point.y;
			Platform_WarpMouse(Point.x, Point.y);
		}
	}
#endif

	RECT rcWnd;
	GetClientRect(&rcWnd);

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.SetResolution(rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	rkWndMgr.RunMouseMove(x, y);
}

void CPythonApplication::OnMouseLeftButtonDown(int x, int y)
{
	const bool debugWantsMouse = Neoeter::Debug::WantsMouseAt(x, y);
	if (debugWantsMouse)
	{
		m_isDebugLeftButtonCaptured = true;
		return;
	}

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunMouseLeftButtonDown(x, y);
}

void CPythonApplication::OnMouseLeftButtonUp(int x, int y)
{
	if (m_isDebugLeftButtonCaptured)
	{
		m_isDebugLeftButtonCaptured = false;
		return;
	}

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunMouseLeftButtonUp(x, y);
}

void CPythonApplication::OnMouseLeftButtonDoubleClick(int x, int y)
{
	if (m_isDebugLeftButtonCaptured || Neoeter::Debug::WantsMouseAt(x, y))
		return;

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunMouseLeftButtonDown(x, y);
	rkWndMgr.RunMouseLeftButtonDoubleClick(x, y);
}

void CPythonApplication::OnMouseRightButtonDown(int x, int y)
{
	const bool debugWantsMouse = Neoeter::Debug::WantsMouseAt(x, y);
	if (debugWantsMouse)
	{
		m_isDebugRightButtonCaptured = true;
		return;
	}

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunMouseRightButtonDown(x, y);
}

void CPythonApplication::OnMouseRightButtonUp(int x, int y)
{
	if (m_isDebugRightButtonCaptured)
	{
		m_isDebugRightButtonCaptured = false;
		return;
	}

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunMouseRightButtonUp(x, y);
}

void CPythonApplication::OnKeyDown(int iIndex)
{
#if defined(NEOETER_DEBUG_UI)
	if (DIK_F12 == iIndex)
	{
		Neoeter::Debug::Toggle();
		return;
	}
#endif

	if (Neoeter::Debug::WantsKeyboard())
		return;

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();

	if (DIK_ESCAPE == iIndex)
	{
		rkWndMgr.RunPressEscapeKey();
	}

	rkWndMgr.RunKeyDown(iIndex);
}

void CPythonApplication::OnKeyUp(int iIndex)
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunKeyUp(iIndex);
}

void CPythonApplication::RunIMEUpdate()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunIMEUpdate();
}
void CPythonApplication::RunIMETabEvent()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunIMETabEvent();
}
void CPythonApplication::RunIMEReturnEvent()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunIMEReturnEvent();
}
void CPythonApplication::OnIMEKeyDown(int iIndex)
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunIMEKeyDown(iIndex);
}

void CPythonApplication::RunIMEChangeCodePage()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunChangeCodePage();
}
void CPythonApplication::RunIMEOpenCandidateListEvent()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunOpenCandidate();
}
void CPythonApplication::RunIMECloseCandidateListEvent()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunCloseCandidate();
}
void CPythonApplication::RunIMEOpenReadingWndEvent()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunOpenReading();
}
void CPythonApplication::RunIMECloseReadingWndEvent()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunCloseReading();
}

void CPythonApplication::RunPressExitKey()
{
	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	rkWndMgr.RunPressExitKey();
}

void CPythonApplication::OnMouseUpdate()
{
#ifdef _DEBUG
	if (!m_poMouseHandler)
	{
		// assert(!" CPythonApplication::OnMouseUpdate - Mouse handler has not set!");
		return;
	}
#endif // _DEBUG

	UI::CWindowManager& rkWndMgr = UI::CWindowManager::Instance();
	long lx, ly;
	rkWndMgr.GetMousePosition(lx, ly);
	PyCallClassMemberFunc(m_poMouseHandler, "Update", Py_BuildValue("(ii)", lx, ly));
}

void CPythonApplication::OnMouseRender()
{
#ifdef _DEBUG
	if (!m_poMouseHandler)
	{
		// assert(!" CPythonApplication::OnMouseRender - Mouse handler has not set!");
		return;
	}
#endif // _DEBUG

	PyCallClassMemberFunc(m_poMouseHandler, "Render", Py_BuildValue("()"));
}
