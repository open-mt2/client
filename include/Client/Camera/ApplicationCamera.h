#pragma once

#include <Client/Map/MapType.h>
#include <Client/Application/AbstractApplication.h>

class CPythonApplication;

class CApplicationCamera
{
	public:
		enum
		{
			CAMERA_MODE_NORMAL = 0,
			CAMERA_MODE_STAND = 1,
			CAMERA_MODE_BLEND = 2,

			EVENT_CAMERA_NUMBER = 101,
		};

		enum EDirection
		{
			CAMERA_TO_POSITIVE = 1,
			CAMERA_TO_NEGITIVE = -1,
			CAMERA_STOP = 0,
		};

		struct SCameraSpeed
		{
				float m_fUpDir;
				float m_fViewDir;
				float m_fCrossDir;

				SCameraSpeed() : m_fUpDir(0.0f), m_fViewDir(0.0f), m_fCrossDir(0.0f) {}
		};

		explicit CApplicationCamera(CPythonApplication& rkApp);

		void SetGlobalCenterPosition(int32_t x, int32_t y);
		void SetCenterPosition(float fx, float fy, float fz);
		void GetCenterPosition(TPixelPosition* pPixelPosition);

		void Set(float fDistance, float fPitch, float fRotation, float fDestinationHeight);
		void Get(float* pDistance, float* pPitch, float* pRotation, float* pDestinationHeight);
		void Rotate(int iDirection);
		void Pitch(int iDirection);
		void Zoom(int iDirection);
		void MovieRotate(int iDirection);
		void MoviePitch(int iDirection);
		void MovieZoom(int iDirection);
		void MovieReset();
		void SetViewDirSpeed(float fSpeed);
		void SetCrossDirSpeed(float fSpeed);
		void SetUpDirSpeed(float fSpeed);
		float GetRotation();
		float GetPitch();
		bool IsLocked();
		void EnableSpecialMode();
		void SetSpeed(int iPercentage);

		void SetEvent(const IAbstractApplication::SCameraSetting& c_rCameraSetting);
		void BlendEvent(const IAbstractApplication::SCameraSetting& c_rCameraSetting, float fBlendTime);
		void SetDefault();
		void SetSetting(const IAbstractApplication::SCameraSetting& c_rCameraSetting);
		void GetSetting(IAbstractApplication::SCameraSetting* pCameraSetting);
		void SaveSetting(const char* c_szFileName);
		bool LoadSetting(const char* c_szFileName);

		void Update();
		void OnUpdate();

	private:
		CPythonApplication& m_rkApp;

		IAbstractApplication::SCameraSetting m_DefaultCameraSetting;
		IAbstractApplication::SCameraSetting m_kEventCameraSetting;
		IAbstractApplication::SCameraSetting m_kEndBlendCameraSetting;

		IAbstractApplication::SCameraPos m_kCmrPos;
		SCameraSpeed m_kCmrSpd;

		D3DXVECTOR3 m_v3CenterPosition;

		int m_iCameraMode;
		int32_t m_isSpecialCameraMode;

		float m_fBlendCameraStartTime;
		float m_fBlendCameraBlendTime;

		float m_fRotationSpeed;
		float m_fPitchSpeed;
		float m_fZoomSpeed;
		float m_fCameraRotateSpeed;
		float m_fCameraPitchSpeed;
		float m_fCameraZoomSpeed;
};
