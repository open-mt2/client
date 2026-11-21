#include <Client/UI/StdAfx.h>
#include <Client/Instance/InstanceBase.h>
#include <Client/Instance/InstanceBaseTransform.h>
#include <Client/Python/Background/PythonBackground.h>

CInstanceBaseTransform::CInstanceBaseTransform(CInstanceBase& rkOwner) : m_rkOwner(rkOwner)
{
}

void CInstanceBaseTransform::SCRIPT_SetPixelPosition(float fx, float fy)
{
	float fz = m_rkOwner.__GetBackgroundHeight(fx, fy);
	NEW_SetPixelPosition(TPixelPosition(fx, fy, fz));
}

void CInstanceBaseTransform::NEW_SetPixelPosition(const TPixelPosition& c_rPixelPosition)
{
	m_rkOwner.m_GraphicThingInstance.SetCurPixelPosition(c_rPixelPosition);
}

void CInstanceBaseTransform::NEW_GetPixelPosition(TPixelPosition* pPixelPosition)
{
	*pPixelPosition = m_rkOwner.m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
}

void CInstanceBaseTransform::SetRotation(float fRotation)
{
	m_rkOwner.m_GraphicThingInstance.SetRotation(fRotation);
}

void CInstanceBaseTransform::BlendRotation(float fRotation, float fBlendTime)
{
	m_rkOwner.m_GraphicThingInstance.BlendRotation(fRotation, fBlendTime);
}

void CInstanceBaseTransform::NEW_LookAtFlyTarget()
{
	m_rkOwner.m_GraphicThingInstance.LookAtFlyTarget();
}

void CInstanceBaseTransform::NEW_LookAtDestPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	m_rkOwner.m_GraphicThingInstance.LookAt(c_rkPPosDst.x, -c_rkPPosDst.y);
}

void CInstanceBaseTransform::NEW_LookAtDestInstance(CInstanceBase& rkInstDst)
{
	m_rkOwner.m_GraphicThingInstance.LookAt(&rkInstDst.m_GraphicThingInstance);
}

float CInstanceBaseTransform::GetRotation()
{
	return m_rkOwner.m_GraphicThingInstance.GetRotation();
}

float CInstanceBaseTransform::GetAdvancingRotation()
{
	return m_rkOwner.m_GraphicThingInstance.GetAdvancingRotation();
}

void CInstanceBaseTransform::SetDirection(int dir)
{
	float fDegree = GetDegreeFromDirection(dir);
	SetRotation(fDegree);
	m_rkOwner.m_GraphicThingInstance.SetAdvancingRotation(fDegree);
}

void CInstanceBaseTransform::BlendDirection(int dir, float blendTime)
{
	m_rkOwner.m_GraphicThingInstance.BlendRotation(GetDegreeFromDirection(dir), blendTime);
}

float CInstanceBaseTransform::GetDegreeFromDirection(int dir)
{
	if (dir < 0)
		return 0.0f;

	if (dir >= CInstanceBase::DIR_MAX_NUM)
		return 0.0f;

	static float s_dirRot[CInstanceBase::DIR_MAX_NUM] =
		{
			+45.0f * 4,
			+45.0f * 3,
			+45.0f * 2,
			+45.0f,
			+0.0f,
			360.0f - 45.0f,
			360.0f - 45.0f * 2,
			360.0f - 45.0f * 3,
		};

	return s_dirRot[dir];
}
