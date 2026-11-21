#include <Client/UI/StdAfx.h>

#if defined(NEOETER_DEBUG_UI)

#include <Client/UI/Widget/GraphicsOptionsPanel.h>

#include <Client/Map/ScreenDepthOfField.h>
#include <Client/Feature/Background/PythonBackground.h>
#include <Client/Application/PythonApplication.h>

#include <Neoeter/Debug/Debug.h>

#include <imgui.h>

namespace
{
	void DrawGraphicsOptions(bool* pVisible)
	{
		if (!ImGui::Begin("Graphics Options", pVisible))
		{
			ImGui::End();
			return;
		}

		CScreenDepthOfField& rkDepthOfField = CScreenDepthOfField::Instance();
		CPythonBackground& rkBackground = CPythonBackground::Instance();

		bool bEnabled = rkDepthOfField.IsEnabled();
		if (ImGui::Checkbox("Horizon DOF", &bEnabled))
		{
			rkDepthOfField.SetEnabled(bEnabled);
			if (rkBackground.IsMapReady())
				rkBackground.GetMapOutdoorRef().RefreshEnvironmentSky();
		}

		float fBlur = rkDepthOfField.GetBlurRadius();
		if (ImGui::SliderFloat("DOF Blur", &fBlur, 0.0f, 16.0f, "%.2f"))
			rkDepthOfField.SetBlurRadius(fBlur);

		bool bAutoFocus = rkDepthOfField.IsAutoFocus();
		if (ImGui::Checkbox("DOF Auto Focus", &bAutoFocus))
			rkDepthOfField.SetAutoFocus(bAutoFocus);

		float fFocus = rkDepthOfField.GetWorldFocusDistance();
		if (ImGui::SliderFloat("DOF World Focus", &fFocus, 100.0f, 12000.0f, "%.0f"))
			rkDepthOfField.SetWorldFocusDistance(fFocus);

		float fRange = rkDepthOfField.GetWorldFocusRange();
		if (ImGui::SliderFloat("DOF World Range", &fRange, 100.0f, 12000.0f, "%.0f"))
			rkDepthOfField.SetWorldFocusRange(fRange);

		ImGui::Separator();

		bool bFog = rkDepthOfField.IsFogEnabled();
		if (ImGui::Checkbox("Fog", &bFog))
		{
			rkDepthOfField.SetFogEnabled(bFog);
			if (rkBackground.IsMapReady())
				rkBackground.GetMapOutdoorRef().RefreshEnvironmentSky();
		}

		float fNear = rkDepthOfField.GetFogNear();
		if (ImGui::SliderFloat("Fog Near", &fNear, 0.0f, 30000.0f, "%.0f"))
			rkDepthOfField.SetFogNear(fNear);

		const float fMaxFogFar = rkBackground.GetRenderDistance();
		float fFar = rkDepthOfField.GetFogFar();
		if (ImGui::SliderFloat("Fog Far", &fFar, 100.0f, 50000.0f, "%.0f"))
			rkDepthOfField.SetFogFar(fFar < fMaxFogFar ? fFar : fMaxFogFar);

		float fSkyFog = rkDepthOfField.GetSkyFogStrength();
		if (ImGui::SliderFloat("Sky Fog", &fSkyFog, 0.0f, 1.0f, "%.2f"))
		{
			rkDepthOfField.SetSkyFogStrength(fSkyFog);
			if (rkBackground.IsMapReady())
				rkBackground.GetMapOutdoorRef().RefreshEnvironmentSky();
		}

		ImGui::TextDisabled("Fog colour follows the map and time of day.");

		ImGui::Separator();

		float fRenderDistance = rkBackground.GetRenderDistance();
		if (ImGui::SliderFloat("Render Distance", &fRenderDistance, 2000.0f, 50000.0f, "%.0f"))
		{
			rkBackground.SetRenderDistance(fRenderDistance);
			const float fAppliedRenderDistance = rkBackground.GetRenderDistance();
			CPythonApplication::Instance().SetForceSightRange(int(fAppliedRenderDistance));
			if (rkDepthOfField.GetFogFar() > fAppliedRenderDistance)
				rkDepthOfField.SetFogFar(fAppliedRenderDistance);
		}

		ImGui::End();
	}
}

void GraphicsOptionsPanel_Initialize()
{
	Neoeter::Debug::AddPanel("Graphics", "Options", DrawGraphicsOptions, true);
}

#endif
