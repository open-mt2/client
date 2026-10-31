# GameLib (`pkg_gamelib`)

[![GameLib CI](https://github.com/open-mt2/pkg_gamelib/actions/workflows/ci.yml/badge.svg)](https://github.com/open-mt2/pkg_gamelib/actions)

`GameLib` is the core game-world gameplay and entity rendering engine of the OpenMT2 client. It orchestrates actors (`CActorInstance`), race and character data (`CRaceManager`, `CRaceData`, `CRaceMotionData`), items (`CItemManager`, `CItemData`), flying objects and projectiles (`CFlyingManager`, `CFlyingInstance`), dungeon blocks (`CDungeonBlock`), terrain / outdoor maps (`CMapOutdoor`, `CMapManager`, `CArea`, `CTerrain`), properties (`CPropertyManager`, `CProperty`), and collision/physics detection.

## Architecture & Linux Porting Notes

- **64-Bit & Cross-Platform Math**:
  - Added portable 3D/4D math implementations to `DirectXMathCompat.h`: `D3DXVec3Scale`, `D3DXVec3Add`, `D3DXVec3Subtract`, `D3DXVec3Lerp`, `D3DXVec3Transform` (outputting 4D homogeneous vector), and `D3DXQuaternionRotationYawPitchRoll`.
  - Added D3D9 texture interface stubs to `DirectXMathCompat.h`: `IDirect3DBaseTexture9::GetLevelCount()`.
  - Fixed 64-bit pointer truncation in `CMapManager::ApplyLight` where pointer was cast to 32-bit `DWORD` (`(DWORD)(uintptr_t)mc_pcurEnvironmentData`).
- **Standard C++ Conformance**:
  - Removed illegal nested in-class qualification `FFindMapName::FFindMapName` in `MapManager.h`.
  - Fixed non-const reference binding to temporary return values of `std::for_each` in `MapOutdoorUpdate.cpp` (`rPushTerrainToDeleteVector`, `rPushAreaToDeleteVector`).
  - Added missing `<cfloat>` include for `FLT_EPSILON` in `MapUtil.cpp`.
  - Fixed forward declaration of `CFlyTarget` in `FlyTarget.h` for enclosing template instantiation in `IFlyTargetableObject`.
  - Cleaned up pool cleanup lifecycle in `CRaceManager::Destroy` (`CRaceData::DestroySystem()`, `CRaceMotionData::DestroySystem()`) and `CItemManager::Destroy` (`CItemData::DestroySystem()`).
  - Normalized case-sensitive `#include` paths (`StdAfx.h`, `<EterGrnLib/Util.h>`).

## Directory Layout (Pitchfork Standard)

- `include/GameLib/` — Public headers (`ActorInstance.h`, `RaceManager.h`, `RaceData.h`, `ItemManager.h`, `ItemData.h`, `MapOutdoor.h`, `MapManager.h`, `PropertyManager.h`, `StdAfx.h`, etc.)
- `include/Platform/` — Bundled cross-platform compatibility headers (`Platform.h`, `DirectXMathCompat.h`)
- `src/` — Implementation files (`ActorInstance*.cpp`, `RaceManager.cpp`, `RaceData.cpp`, `ItemManager.cpp`, `MapOutdoor*.cpp`, `PropertyManager.cpp`, etc.)
- `tests/` — CTest unit & smoke test suite (`test_gamelib.cpp`)

## Building & Testing Standalone

```bash
cmake -B build -G Ninja -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```
