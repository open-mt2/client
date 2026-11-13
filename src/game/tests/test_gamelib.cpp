#include <Client/Game/StdAfx.h>
#include <Client/Game/GameUtil.h>
#include <Client/Game/Lighting.h>
#include <Client/Game/ItemManager.h>
#include <Client/Game/RaceManager.h>
#include <Client/Game/PropertyManager.h>
#include <Client/Game/ActorInstance.h>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace
{

	void test_game_math_and_util()
	{
		// Rotation from position (OpenMT2 uses (0, -1, 0) as standard 0-degree vector)
		float degSouth = GetDegreeFromPosition(0.0f, -10.0f);
		assert(std::fabs(degSouth - 0.0f) < 1.0f || std::fabs(degSouth - 360.0f) < 1.0f);

		float degEast = GetDegreeFromPosition(10.0f, 0.0f);
		assert(std::fabs(degEast - 90.0f) < 1.0f);

		float degNorth = GetDegreeFromPosition(0.0f, 10.0f);
		assert(std::fabs(degNorth - 180.0f) < 1.0f);

		float degWest = GetDegreeFromPosition(-10.0f, 0.0f);
		assert(std::fabs(degWest - 270.0f) < 1.0f);

		// Rotating direction
		int dirRight = GetRotatingDirection(80.0f, 10.0f);
		assert(dirRight == DEGREE_DIRECTION_RIGHT);

		int dirLeft = GetRotatingDirection(10.0f, 80.0f);
		assert(dirLeft == DEGREE_DIRECTION_LEFT);

		std::printf("[PASS] test_game_math_and_util\n");
	}

	void test_item_manager()
	{
		static_assert(sizeof(CItemData::TItemTable) == 156, "TItemTable stride must match item_proto stride (156 bytes)");
		static_assert(sizeof(CItemData::TItemLimit) == 5, "TItemLimit size must be 5 bytes");
		static_assert(sizeof(CItemData::TItemApply) == 5, "TItemApply size must be 5 bytes");

		CItemManager itemMgr;

		CItemData* pItem1 = itemMgr.MakeItemData(1001);
		assert(pItem1 != nullptr);

		CItemData* pFound = nullptr;
		BOOL foundOk = itemMgr.GetItemDataPointer(1001, &pFound);
		assert(foundOk && pFound == pItem1);

		// Make another item
		CItemData* pItem2 = itemMgr.MakeItemData(2002);
		assert(pItem2 != nullptr);
		assert(pItem2 != pItem1);

		// Select item
		BOOL selectOk = itemMgr.SelectItemData(2002);
		assert(selectOk);
		assert(itemMgr.GetSelectedItemDataPointer() == pItem2);

		// Non-existent item
		CItemData* pMissing = nullptr;
		assert(!itemMgr.GetItemDataPointer(99999, &pMissing));

		std::printf("[PASS] test_item_manager\n");
	}

	void test_race_manager()
	{
		CRaceManager raceMgr;

		raceMgr.RegisterRaceName(1, "warrior_m");
		raceMgr.RegisterRaceName(2, "assassin_w");

		raceMgr.CreateRace(1);
		raceMgr.CreateRace(2);

		CRaceData* pRaceData1 = nullptr;
		BOOL ok = raceMgr.GetRaceDataPointer(1, &pRaceData1);
		assert(ok && pRaceData1 != nullptr);

		raceMgr.SelectRace(2);
		assert(raceMgr.GetSelectedRaceDataPointer() != nullptr);

		std::printf("[PASS] test_race_manager\n");
	}

	void test_property_manager()
	{
		CPropertyManager propMgr;

		// Verify singleton creation and state
		assert(CPropertyManager::InstancePtr() != nullptr);

		std::printf("[PASS] test_property_manager\n");
	}

	void test_actor_instance_basics()
	{
		CActorInstance actor;

		actor.SetPosition(100.0f, 200.0f, 50.0f);
		const D3DXVECTOR3& pos = actor.GetPosition();
		assert(std::fabs(pos.x - 100.0f) < 1e-4f);
		assert(std::fabs(pos.y - 200.0f) < 1e-4f);
		assert(std::fabs(pos.z - 50.0f) < 1e-4f);

		actor.SetRotation(135.0f);
		assert(std::fabs(actor.GetRotation() - 135.0f) < 1e-4f);

		std::printf("[PASS] test_actor_instance_basics\n");
	}

	void test_lighting_modes()
	{
		assert(Lighting_SetModeByName("full"));
		assert(Lighting_GetMode() == LIGHTING_MODE_FULL);
		assert(Lighting_IsEnabled());
		assert(Lighting_IsTerrainEnabled());
		assert(std::strcmp(Lighting_GetModeName(), "full") == 0);

		assert(Lighting_SetModeByName("models"));
		assert(Lighting_IsEnabled());
		assert(!Lighting_IsTerrainEnabled());

		assert(Lighting_SetModeByName("off"));
		assert(!Lighting_IsEnabled());
		assert(!Lighting_IsTerrainEnabled());

		assert(!Lighting_SetModeByName("bogus"));
		assert(std::strcmp(Lighting_GetModeName(), "off") == 0);

		std::printf("[PASS] test_lighting_modes\n");
	}

	void test_day_night_curve()
	{
		SDayNightTint noon;
		DayNight_ComputeTint(12.0f, noon);
		assert(std::fabs(noon.sun.r - 1.0f) < 1e-4f);
		assert(std::fabs(noon.ambient.g - 1.0f) < 1e-4f);
		assert(std::fabs(noon.fog.b - 1.0f) < 1e-4f);
		assert(std::fabs(noon.sky.r - 1.0f) < 1e-4f);
		assert(std::fabs(noon.flare - 1.0f) < 1e-4f);

		SDayNightTint night;
		DayNight_ComputeTint(22.0f, night);
		assert(night.sun.r < noon.sun.r);
		assert(night.ambient.g < noon.ambient.g);
		assert(night.fog.b < noon.fog.b);
		assert(night.sky.r < noon.sky.r);
		assert(night.flare < 0.01f);

		SDayNightTint dusk;
		DayNight_ComputeTint(18.0f, dusk);
		assert(dusk.sun.r > dusk.sun.b);

		TEnvironmentData base;
		base.DirLights[ENV_DIRLIGHT_BACKGROUND].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		base.DirLights[ENV_DIRLIGHT_CHARACTER].Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		base.Material.Diffuse = D3DCOLORVALUE{1.0f, 1.0f, 1.0f, 1.0f};
		base.Material.Ambient = D3DCOLORVALUE{0.88f, 0.85f, 0.78f, 1.0f};
		base.Material.Emissive = D3DCOLORVALUE{0.32f, 0.32f, 0.41f, 1.0f};
		base.FogColor = D3DXCOLOR(0.8f, 0.9f, 1.0f, 1.0f);
		base.LensFlareBrightnessColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		base.fLensFlareMaxBrightness = 1.0f;

		TEnvironmentData out;
		DayNight_Apply(noon, base, out);
		assert(std::fabs(out.DirLights[ENV_DIRLIGHT_BACKGROUND].Diffuse.r - 1.0f) < 1e-4f);
		assert(std::fabs(out.DirLights[ENV_DIRLIGHT_CHARACTER].Ambient.g - 0.5f) < 1e-4f);
		assert(std::fabs(out.FogColor.b - 1.0f) < 1e-4f);
		assert(std::fabs(out.fLensFlareMaxBrightness - 1.0f) < 1e-4f);

		DayNight_Apply(night, base, out);
		assert(out.DirLights[ENV_DIRLIGHT_BACKGROUND].Diffuse.r < 1.0f);
		assert(out.FogColor.b < 1.0f);
		assert(out.Material.Ambient.r < 0.88f);
		assert(out.Material.Emissive.r < 0.32f);
		assert(out.fLensFlareMaxBrightness < 0.01f);

		TEnvironmentData skyBase = base;
		skyBase.bSkyBoxTextureRenderMode = FALSE;
		skyBase.bySkyBoxGradientLevelUpper = 1;
		skyBase.SkyBoxGradientColorVector.resize(2);
		skyBase.SkyBoxGradientColorVector[1].m_FirstColor = TColor(0.8f, 0.3f, 0.2f, 1.0f);
		skyBase.SkyBoxGradientColorVector[1].m_SecondColor = TColor(0.9f, 0.4f, 0.3f, 1.0f);

		TEnvironmentData skyOut;
		DayNight_Apply(noon, skyBase, skyOut);
		assert(std::fabs(skyOut.FogColor.r - 0.8f) < 1e-4f);
		assert(std::fabs(skyOut.FogColor.g - 0.3f) < 1e-4f);
		assert(std::fabs(skyOut.FogColor.b - 0.2f) < 1e-4f);

		DayNight_SetHour(18.5f);
		assert(DayNight_IsHourFrozen());
		assert(std::fabs(DayNight_GetHour() - 18.5f) < 1e-4f);

		std::printf("[PASS] test_day_night_curve\n");
	}

} // namespace

int main()
{
	test_game_math_and_util();
	test_item_manager();
	test_race_manager();
	test_property_manager();
	test_actor_instance_basics();
	test_lighting_modes();
	test_day_night_curve();

	std::printf("All GameLib smoke tests passed successfully!\n");
	return 0;
}
