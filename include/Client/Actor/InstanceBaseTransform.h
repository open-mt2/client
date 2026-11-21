#pragma once

#include <Client/Map/MapType.h>

class CInstanceBase;

class CInstanceBaseTransform
{
	public:
		explicit CInstanceBaseTransform(CInstanceBase& rkOwner);

		void SCRIPT_SetPixelPosition(float fx, float fy);
		void NEW_SetPixelPosition(const TPixelPosition& c_rPixelPosition);
		void NEW_GetPixelPosition(TPixelPosition* pPixelPosition);

		void SetRotation(float fRotation);
		void BlendRotation(float fRotation, float fBlendTime = 0.1f);
		void NEW_LookAtFlyTarget();
		void NEW_LookAtDestPixelPosition(const TPixelPosition& c_rkPPosDst);
		void NEW_LookAtDestInstance(CInstanceBase& rkInstDst);

		float GetRotation();
		float GetAdvancingRotation();

		void SetDirection(int dir);
		void BlendDirection(int dir, float blendTime);
		float GetDegreeFromDirection(int dir);

	private:
		CInstanceBase& m_rkOwner;
};
