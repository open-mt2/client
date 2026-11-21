#pragma once

#include <Neoeter/Model/Grn/ThingInstance.h>
#include <Client/Map/MapType.h>

#include <cstdint>

class CActorInstance;

class CActorInstanceRender
{
	public:
		explicit CActorInstanceRender(CActorInstance& rkOwner);

		void OnRender();

		void BeginAddRender();
		void BeginBlendRender();
		void BeginDiffuseRender();
		void BeginModulateRender();
		void BeginOpacityRender();
		void EndAddRender();
		void EndBlendRender();
		void EndDiffuseRender();
		void EndModulateRender();
		void EndOpacityRender();
		static bool IsDirLine();
		void RenderCollisionData();
		void RenderToShadowMap();
		void RestoreRenderMode();
		void SetAddColor(const D3DXCOLOR& c_rColor);
		void SetAddRenderMode();
		void SetMaterialAlpha(uint32_t dwAlpha);
		void SetMaterialColor(uint32_t dwColor);
		void SetModulateRenderMode();
		void SetRenderMode(int iRenderMode);
		static void ShowDirectionLine(bool isVisible);

	private:
		CActorInstance& m_rkOwner;
};
