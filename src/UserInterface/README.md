# UserInterface (`pkg_userinterface`)

[![UserInterface CI](https://github.com/open-mt2/pkg_userinterface/actions/workflows/ci.yml/badge.svg)](https://github.com/open-mt2/pkg_userinterface/actions)

`UserInterface` is the top-level application and UI layer of the OpenMT2 client, compiling both the static library `UserInterface` and the master client executable `metin2client`. It integrates the game loop, network protocol stream (`CPythonNetworkStream`), high-level Python game controllers (`CPythonApplication`, `CPythonPlayer`, `CPythonCharacterManager`, `CPythonItem`, `CPythonNonPlayer`, `CPythonSkill`, `CPythonGuild`, `CPythonChat`, `CPythonMiniMap`, `CPythonBackground`), network actor management (`CNetworkActorManager`), text tails, IME input handling, and security checkers (`CInsultChecker`).

## Architecture & Linux Porting Notes

- **Entry Point & Platform Translation**:
  - Implemented standard Linux `main(int argc, char** argv)` converting CLI arguments to Win32-compatible command line strings and invoking `WinMain`.
  - Added compatibility stubs for Windows window styles, message pump messages (`WM_INPUTLANGCHANGE`, `WM_IME_*`, `WM_MOUSEWHEEL`, etc.), mutexes (`CreateMutex`, `ReleaseMutex`), and module path resolution via `/proc/self/exe`.
- **DirectShow COM Separation**:
  - Replaced legacy DirectShow / DirectDraw COM playback filters in `MovieMan.cpp` and `PythonApplicationLogo.cpp` with portable Linux no-op stubs (`ClearToBlack`, `PlayLogo`, `PlayIntro`, `PlayTutorial`, `OnLogoOpen`, etc.), cleanly decoupling video playback without impacting gameplay.
- **64-Bit & Cross-Platform Math**:
  - Added `D3DXColorModulate` to `DirectXMathCompat.h` for minimap mark color modulation.
  - Normalized packet coordinate types in `Packet.h` from 64-bit platform `long` to 32-bit wire-compatible `LONG` (`int32_t`), ensuring strict packet struct alignment across 32-bit and 64-bit systems.
  - Replaced 32-bit pointer integer truncations with `(intptr_t)` and `PyTuple_GetLong` in `PythonApplicationModule.cpp` text file loader APIs.
  - Corrected `PyDict_Next` iteration position type to `Py_ssize_t` in `PythonPlayerModule.cpp`.
- **Standard C++ Conformance**:
  - Fixed float-to-`LONG` narrowing in `PythonChat.cpp` `RECT` initialization.
  - Fixed `CPythonCharacterManager::CreateInstance` format argument passing `m_dwVID`.
  - Added space separators to hex constants in `PythonNetworkStreamPhaseGame.cpp` and `PythonNetworkStreamPhaseLoading.cpp` to prevent C++11 user-defined literal suffix misparsing.
  - Normalized case-sensitive headers (`PythonCharacterManager.h`, `PythonItem.h`, `PythonNonPlayer.h`, `Packet.h`, `<IL/il.h>`).
  - Extracted string table handling to `ApplicationStringTable.cpp` for clean linkage across library and test targets.

## Directory Layout (Pitchfork Standard)

- `include/UserInterface/` — Public headers (49 headers: `PythonApplication.h`, `PythonNetworkStream.h`, `PythonPlayer.h`, `PythonCharacterManager.h`, `PythonItem.h`, `Packet.h`, `StdAfx.h`, etc.)
- `include/Platform/` — Bundled cross-platform compatibility headers (`Platform.h`, `DirectXMathCompat.h`)
- `src/` — Implementation files (89 source files: `PythonApplication*.cpp`, `PythonNetworkStream*.cpp`, `PythonPlayer*.cpp`, `UserInterface.cpp`, `ApplicationStringTable.cpp`, etc.)
- `tests/` — CTest unit & smoke test suite (`test_userinterface.cpp`)

## Building & Testing Standalone

```bash
cmake -B build -G Ninja -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The full client Python 2.7 surface is pinned by `test_python_client_api`
(`tests/data/python_api_client.txt`), which boots the interpreter headless and
imports every module registered by `RunMainScript()`. After adding, removing or
renaming a binding, regenerate the golden file deliberately:

```bash
OPENMT2_UPDATE_PYTHON_API=1 ctest --test-dir build -R python --output-on-failure
```
