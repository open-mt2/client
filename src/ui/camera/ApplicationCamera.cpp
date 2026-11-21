#include <Client/UI/StdAfx.h>
#include <Client/UI/Camera/ApplicationCamera.h>
#include <Client/UI/App/PythonApplication.h>
#include <Client/Feature/Background/PythonBackground.h>

#include <Neoeter/Audio/SoundManager.h>
#include <Neoeter/Platform/Base/Timer.h>
#include <Neoeter/Render/Camera.h>

namespace
{
	float c_fDefaultCameraRotateSpeed = 1.5f;
	float c_fDefaultCameraPitchSpeed = 1.5f;
	float c_fDefaultCameraZoomSpeed = 0.05f;

	float BlendValueByLinear(float fElapsedTime, float fDuration, float fBeginValue, float fEndValue)
	{
		if (fElapsedTime >= fDuration)
			return fEndValue;

		return (fEndValue - fBeginValue) * (fElapsedTime / fDuration) + fBeginValue;
	}
}

CApplicationCamera::CApplicationCamera(CPythonApplication& rkApp) : m_rkApp(rkApp)
{
	m_v3CenterPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_iCameraMode = CAMERA_MODE_NORMAL;
	m_isSpecialCameraMode = FALSE;
	m_fBlendCameraStartTime = 0.0f;
	m_fBlendCameraBlendTime = 0.0f;
	m_fRotationSpeed = 0.0f;
	m_fPitchSpeed = 0.0f;
	m_fZoomSpeed = 0.0f;
	m_fCameraRotateSpeed = c_fDefaultCameraRotateSpeed;
	m_fCameraPitchSpeed = c_fDefaultCameraPitchSpeed;
	m_fCameraZoomSpeed = c_fDefaultCameraZoomSpeed;
}

void CApplicationCamera::Update()
{
	CCamera* pMainCamera = CCameraManager::Instance().GetCurrentCamera();
	if (!pMainCamera)
		return;

	if (CAMERA_MODE_BLEND == m_iCameraMode)
	{
		float fcurTime = CTimer::Instance().GetCurrentSecond();
		float fElapsedTime = fcurTime - m_fBlendCameraStartTime;

		float fxCenter = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.v3CenterPosition.x, m_kEndBlendCameraSetting.v3CenterPosition.x);
		float fyCenter = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.v3CenterPosition.y, m_kEndBlendCameraSetting.v3CenterPosition.y);
		float fzCenter = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.v3CenterPosition.z, m_kEndBlendCameraSetting.v3CenterPosition.z);

		float fDistance = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.fZoom, m_kEndBlendCameraSetting.fZoom);
		float fPitch = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.fPitch, m_kEndBlendCameraSetting.fPitch);
		float fRotation = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.fRotation, m_kEndBlendCameraSetting.fRotation);

		float fUpDir = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.kCmrPos.m_fUpDir, m_kEndBlendCameraSetting.kCmrPos.m_fUpDir);
		float fViewDir = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.kCmrPos.m_fViewDir, m_kEndBlendCameraSetting.kCmrPos.m_fViewDir);
		float fCrossDir = BlendValueByLinear(fElapsedTime, m_fBlendCameraBlendTime, m_kEventCameraSetting.kCmrPos.m_fCrossDir, m_kEndBlendCameraSetting.kCmrPos.m_fCrossDir);

		// Temporary. Have to fix that this work in camera class. - [levites]
		pMainCamera->Unlock();
		m_rkApp.m_pyGraphic.SetPositionCamera(fxCenter, fyCenter, fzCenter, fDistance, fPitch, fRotation);
		pMainCamera->MoveVertical(fUpDir);
		pMainCamera->MoveFront(fViewDir);
		pMainCamera->MoveAlongCross(fCrossDir);
		pMainCamera->Lock();
	}
	else if (CAMERA_MODE_STAND == m_iCameraMode)
	{
		float fDistance, fPitch, fRotation, fHeight;
		Get(&fDistance, &fPitch, &fRotation, &fHeight);
		m_rkApp.m_pyGraphic.SetPositionCamera(m_kEventCameraSetting.v3CenterPosition.x,
											  m_kEventCameraSetting.v3CenterPosition.y,
											  m_kEventCameraSetting.v3CenterPosition.z + pMainCamera->GetTargetHeight(),
											  fDistance, fPitch, fRotation);
	}
	else if (CAMERA_MODE_NORMAL == m_iCameraMode)
	{
		float fDistance, fPitch, fRotation, fHeight;
		Get(&fDistance, &fPitch, &fRotation, &fHeight);
		m_rkApp.m_pyGraphic.SetPositionCamera(m_v3CenterPosition.x, m_v3CenterPosition.y, m_v3CenterPosition.z + pMainCamera->GetTargetHeight(), fDistance, fPitch, fRotation);
	}

	if (0.0f != m_fRotationSpeed)
		pMainCamera->Roll(m_fRotationSpeed);
	if (0.0f != m_fPitchSpeed)
		pMainCamera->Pitch(m_fPitchSpeed);
	if (0.0f != m_fZoomSpeed)
		pMainCamera->Zoom(m_fZoomSpeed);

	if (0.0f != m_kCmrSpd.m_fViewDir)
		m_kCmrPos.m_fViewDir += m_kCmrSpd.m_fViewDir;
	if (0.0f != m_kCmrSpd.m_fCrossDir)
		m_kCmrPos.m_fCrossDir += m_kCmrSpd.m_fCrossDir;
	if (0.0f != m_kCmrSpd.m_fUpDir)
		m_kCmrPos.m_fUpDir += m_kCmrSpd.m_fUpDir;

	if (0.0f != m_kCmrPos.m_fViewDir)
		pMainCamera->MoveFront(m_kCmrPos.m_fViewDir);
	if (0.0f != m_kCmrPos.m_fCrossDir)
		pMainCamera->MoveAlongCross(m_kCmrPos.m_fCrossDir);
	if (0.0f != m_kCmrPos.m_fUpDir)
		pMainCamera->MoveVertical(m_kCmrPos.m_fUpDir);

	if (pMainCamera->IsDraging())
		m_rkApp.SkipRenderBuffering(3000);

	const D3DXVECTOR3& c_rv3CameraDirection = pMainCamera->GetView();
	const D3DXVECTOR3& c_rv3CameraUp = pMainCamera->GetUp();
	m_rkApp.m_SoundManager.SetPosition(m_v3CenterPosition.x, m_v3CenterPosition.y, m_v3CenterPosition.z);
	m_rkApp.m_SoundManager.SetDirection(c_rv3CameraDirection.x, c_rv3CameraDirection.y, c_rv3CameraDirection.z, c_rv3CameraUp.x, c_rv3CameraUp.y, c_rv3CameraUp.z);
	m_rkApp.m_SoundManager.Update();
}

void CApplicationCamera::OnUpdate()
{
	if (CPythonBackground::Instance().IsMapReady())
	{
		CCamera* pkCameraMgr = CCameraManager::Instance().GetCurrentCamera();
		if (pkCameraMgr)
		{
			if (m_rkApp.m_iCameraBaseWidth > 0 && m_rkApp.m_iCameraBaseHeight > 0 && m_rkApp.m_dwWidth > 0 && m_rkApp.m_dwHeight > 0)
			{
				pkCameraMgr->SetMouseDragScale(
					float(m_rkApp.m_iCameraBaseWidth) / float(m_rkApp.m_dwWidth),
					float(m_rkApp.m_iCameraBaseHeight) / float(m_rkApp.m_dwHeight));
			}

			pkCameraMgr->Update();
		}
	}
}

void CApplicationCamera::SetViewDirSpeed(float fSpeed)
{
	if (IsLocked())
		return;

	m_kCmrSpd.m_fViewDir = fSpeed;
}

void CApplicationCamera::SetCrossDirSpeed(float fSpeed)
{
	if (IsLocked())
		return;

	m_kCmrSpd.m_fCrossDir = fSpeed;
}

void CApplicationCamera::SetUpDirSpeed(float fSpeed)
{
	if (IsLocked())
		return;

	m_kCmrSpd.m_fUpDir = fSpeed;
}

void CApplicationCamera::Set(float fDistance, float fPitch, float fRotation, float fDestinationHeight)
{
	if (IsLocked())
		return;

	CCamera* pCurrentCamera = CCameraManager::Instance().GetCurrentCamera();
	if (!pCurrentCamera)
		return;

	D3DXVECTOR3 v3Target = pCurrentCamera->GetTarget();
	m_rkApp.m_pyGraphic.SetPositionCamera(v3Target.x, v3Target.y, v3Target.z, fDistance, fPitch, fRotation);

	CCamera* pMainCamera = CCameraManager::Instance().GetCurrentCamera();
	if (pMainCamera)
		pMainCamera->SetTargetHeight(fDestinationHeight);
}

void CApplicationCamera::Get(float* pDistance, float* pPitch, float* pRotation, float* pDestinationHeight)
{
	CCamera* pCurrentCamera = CCameraManager::Instance().GetCurrentCamera();
	*pDistance = pCurrentCamera->GetDistance();
	*pPitch = pCurrentCamera->GetPitch();
	*pRotation = pCurrentCamera->GetRoll();
	*pDestinationHeight = pCurrentCamera->GetTarget().z;
}

void CApplicationCamera::Rotate(int iDirection)
{
	if (IsLocked())
		return;

	float fDegree = m_fCameraRotateSpeed * float(iDirection);
	m_fRotationSpeed = fDegree;
}

void CApplicationCamera::Pitch(int iDirection)
{
	if (IsLocked())
		return;

	float fDegree = m_fCameraPitchSpeed * float(iDirection);
	m_fPitchSpeed = fDegree;
}

void CApplicationCamera::Zoom(int iDirection)
{
	if (IsLocked())
		return;

	float fRatio = 1.0f + m_fCameraZoomSpeed * float(iDirection);
	m_fZoomSpeed = fRatio;
}

void CApplicationCamera::MovieRotate(int iDirection)
{
	if (IsLocked())
		return;

	float fDegree = m_fCameraRotateSpeed * float(iDirection);
	if (m_isSpecialCameraMode)
	{
		if (GetKeyState(VK_SCROLL) & 1)
		{
			SetCrossDirSpeed(-fDegree * 6.0f);
			return;
		}
	}
	m_fRotationSpeed = fDegree;
}

void CApplicationCamera::MoviePitch(int iDirection)
{
	if (IsLocked())
		return;

	float fDegree = m_fCameraPitchSpeed * float(iDirection);
	if (m_isSpecialCameraMode)
	{
		if (GetKeyState(VK_SCROLL) & 1)
		{
			SetViewDirSpeed(-fDegree * 6.0f);
			return;
		}
	}
	m_fPitchSpeed = fDegree;
}

void CApplicationCamera::MovieZoom(int iDirection)
{
	if (IsLocked())
		return;

	float fRatio = 1.0f + m_fCameraZoomSpeed * float(iDirection);
	if (m_isSpecialCameraMode)
	{
		if (GetKeyState(VK_SCROLL) & 1)
		{
			SetUpDirSpeed((1.0f - fRatio) * 200.0f);
			return;
		}
	}
	m_fZoomSpeed = fRatio;
}

void CApplicationCamera::MovieReset()
{
	if (IsLocked())
		return;

	if (m_isSpecialCameraMode)
	{
		SetCrossDirSpeed(0.0f);
		SetViewDirSpeed(0.0f);
		SetUpDirSpeed(0.0f);

		m_kCmrPos.m_fViewDir = 0;
		m_kCmrPos.m_fCrossDir = 0;
		m_kCmrPos.m_fUpDir = 0;
	}
}

float CApplicationCamera::GetRotation()
{
	return CCameraManager::Instance().GetCurrentCamera()->GetRoll();
}

float CApplicationCamera::GetPitch()
{
	return CCameraManager::Instance().GetCurrentCamera()->GetPitch();
}

bool CApplicationCamera::IsLocked()
{
	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (!pCamera)
		return false;

	return pCamera->IsLock();
}

void CApplicationCamera::SetEvent(const IAbstractApplication::SCameraSetting& c_rCameraSetting)
{
	if (CCameraManager::DEFAULT_PERSPECTIVE_CAMERA == CCameraManager::Instance().GetCurrentCameraNum())
	{
		GetSetting(&m_DefaultCameraSetting);
	}

	CCameraManager::Instance().SetCurrentCamera(EVENT_CAMERA_NUMBER);
	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (!pCamera)
		return;

	SetSetting(c_rCameraSetting);
	m_kEventCameraSetting = c_rCameraSetting;
	m_iCameraMode = CAMERA_MODE_STAND;
}

void CApplicationCamera::BlendEvent(const IAbstractApplication::SCameraSetting& c_rCameraSetting, float fBlendTime)
{
	m_iCameraMode = CAMERA_MODE_BLEND;
	m_fBlendCameraStartTime = CTimer::Instance().GetCurrentSecond();
	m_fBlendCameraBlendTime = fBlendTime;
	m_kEndBlendCameraSetting = c_rCameraSetting;

	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (pCamera)
		pCamera->Lock();
}

void CApplicationCamera::SetDefault()
{
	m_iCameraMode = CAMERA_MODE_NORMAL;
	m_fBlendCameraStartTime = 0.0f;
	m_fBlendCameraBlendTime = 0.0f;

	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (pCamera)
		pCamera->Unlock();

	if (CCameraManager::DEFAULT_PERSPECTIVE_CAMERA != CCameraManager::Instance().GetCurrentCameraNum())
	{
		CCameraManager::Instance().SetCurrentCamera(CCameraManager::DEFAULT_PERSPECTIVE_CAMERA);
		SetSetting(m_DefaultCameraSetting);
	}
}

void CApplicationCamera::SetSetting(const IAbstractApplication::SCameraSetting& c_rCameraSetting)
{
	CCamera* pCamera = CCameraManager::Instance().GetCurrentCamera();
	if (!pCamera)
		return;

	m_rkApp.m_pyGraphic.SetPositionCamera(c_rCameraSetting.v3CenterPosition.x,
										  c_rCameraSetting.v3CenterPosition.y,
										  c_rCameraSetting.v3CenterPosition.z,
										  c_rCameraSetting.fZoom,
										  c_rCameraSetting.fPitch,
										  c_rCameraSetting.fRotation);

	if (0.0f != c_rCameraSetting.kCmrPos.m_fViewDir)
		pCamera->MoveFront(c_rCameraSetting.kCmrPos.m_fViewDir);
	if (0.0f != c_rCameraSetting.kCmrPos.m_fCrossDir)
		pCamera->MoveAlongCross(c_rCameraSetting.kCmrPos.m_fCrossDir);
	if (0.0f != c_rCameraSetting.kCmrPos.m_fUpDir)
		pCamera->MoveVertical(c_rCameraSetting.kCmrPos.m_fUpDir);

	m_kCmrPos.m_fUpDir = 0.0f;
	m_kCmrPos.m_fViewDir = 0.0f;
	m_kCmrPos.m_fCrossDir = 0.0f;
	m_kCmrSpd.m_fUpDir = 0.0f;
	m_kCmrSpd.m_fViewDir = 0.0f;
	m_kCmrSpd.m_fCrossDir = 0.0f;
	m_fZoomSpeed = 0.0f;
	m_fPitchSpeed = 0.0f;
	m_fRotationSpeed = 0.0f;
}

void CApplicationCamera::GetSetting(IAbstractApplication::SCameraSetting* pCameraSetting)
{
	pCameraSetting->v3CenterPosition = m_v3CenterPosition;
	pCameraSetting->kCmrPos = m_kCmrPos;

	if (CCameraManager::Instance().GetCurrentCamera())
		pCameraSetting->v3CenterPosition.z += CCameraManager::Instance().GetCurrentCamera()->GetTargetHeight();

	float fHeight;
	Get(&pCameraSetting->fZoom, &pCameraSetting->fPitch, &pCameraSetting->fRotation, &fHeight);
}

void CApplicationCamera::SaveSetting(const char* c_szFileName)
{
	IAbstractApplication::SCameraSetting CameraSetting;
	GetSetting(&CameraSetting);

	FILE* File = fopen(c_szFileName, "w");
	SetFileAttributes(c_szFileName, FILE_ATTRIBUTE_NORMAL);

	PrintfTabs(File, 0, "CenterPos %f %f %f\n", CameraSetting.v3CenterPosition.x, CameraSetting.v3CenterPosition.y, CameraSetting.v3CenterPosition.z);
	PrintfTabs(File, 0, "CameraSetting %f %f %f\n", CameraSetting.fZoom, CameraSetting.fPitch, CameraSetting.fRotation);
	PrintfTabs(File, 0, "CmrPos %f %f %f\n", CameraSetting.kCmrPos.m_fUpDir, CameraSetting.kCmrPos.m_fViewDir, CameraSetting.kCmrPos.m_fCrossDir);
	PrintfTabs(File, 0, "Line \"x;%d|y;%d|z;%d|distance;%d|pitch;%d|rot;%d|up;%d|view;%d|cross;%d\"\n",
			   int(CameraSetting.v3CenterPosition.x),
			   int(CameraSetting.v3CenterPosition.y),
			   int(CameraSetting.v3CenterPosition.z),
			   int(CameraSetting.fZoom),
			   int(CameraSetting.fPitch),
			   int(CameraSetting.fRotation),
			   int(CameraSetting.kCmrPos.m_fUpDir),
			   int(CameraSetting.kCmrPos.m_fViewDir),
			   int(CameraSetting.kCmrPos.m_fCrossDir));

	fclose(File);
}

bool CApplicationCamera::LoadSetting(const char* c_szFileName)
{
	CTextFileLoader TextFileLoader;
	if (!TextFileLoader.Load(c_szFileName))
		return false;

	TextFileLoader.SetTop();

	D3DXVECTOR3 v3CenterPosition;
	CTokenVector* pCameraSetting;
	CTokenVector* pCmrPos;
	if (TextFileLoader.GetTokenVector3("centerpos", &v3CenterPosition))
		if (TextFileLoader.GetTokenVector("camerasetting", &pCameraSetting))
			if (TextFileLoader.GetTokenVector("cmrpos", &pCmrPos))
				if (3 == pCameraSetting->size())
					if (3 == pCmrPos->size())
					{
						IAbstractApplication::SCameraSetting CameraSetting;
						CameraSetting.v3CenterPosition = v3CenterPosition;

						CameraSetting.fZoom = atof(pCameraSetting->at(0).c_str());
						CameraSetting.fPitch = atof(pCameraSetting->at(1).c_str());
						CameraSetting.fRotation = atof(pCameraSetting->at(2).c_str());

						CameraSetting.kCmrPos.m_fUpDir = atof(pCmrPos->at(0).c_str());
						CameraSetting.kCmrPos.m_fViewDir = atof(pCmrPos->at(1).c_str());
						CameraSetting.kCmrPos.m_fCrossDir = atof(pCmrPos->at(2).c_str());

						SetEvent(CameraSetting);
						return true;
					}

	return false;
}

void CApplicationCamera::SetGlobalCenterPosition(int32_t x, int32_t y)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	rkBG.GlobalPositionToLocalPosition(x, y);

	float z = CPythonBackground::Instance().GetHeight(x, y);

	SetCenterPosition(x, y, z);
}

void CApplicationCamera::SetCenterPosition(float fx, float fy, float fz)
{
	m_v3CenterPosition.x = +fx;
	m_v3CenterPosition.y = -fy;
	m_v3CenterPosition.z = +fz;
}

void CApplicationCamera::GetCenterPosition(TPixelPosition* pPixelPosition)
{
	pPixelPosition->x = +m_v3CenterPosition.x;
	pPixelPosition->y = -m_v3CenterPosition.y;
	pPixelPosition->z = +m_v3CenterPosition.z;
}

void CApplicationCamera::EnableSpecialMode()
{
	m_isSpecialCameraMode = TRUE;
}

void CApplicationCamera::SetSpeed(int iPercentage)
{
	m_fCameraRotateSpeed = c_fDefaultCameraRotateSpeed * float(iPercentage) / 100.0f;
	m_fCameraPitchSpeed = c_fDefaultCameraPitchSpeed * float(iPercentage) / 100.0f;
	m_fCameraZoomSpeed = c_fDefaultCameraZoomSpeed * float(iPercentage) / 100.0f;
}
