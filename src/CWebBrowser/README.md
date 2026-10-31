# CWebBrowser (`pkg_cwebbrowser`)

Portable web view lifecycle and state interface for the Metin2 client engine.

## Overview

Replaces the legacy Windows-only Internet Explorer ActiveX (`IWebBrowser2`) control with a clean, portable implementation of the `WebBrowser_*` C API (`WebBrowser_Startup`, `WebBrowser_Show`, `WebBrowser_Move`, `WebBrowser_Hide`, `WebBrowser_IsVisible`, `WebBrowser_GetRect`, `WebBrowser_Destroy`, `WebBrowser_Cleanup`).

## Directory Layout (Pitchfork Standard)

```text
CWebBrowser/
├── CMakeLists.txt
├── include/
│   ├── CWebBrowser/
│   │   └── CWebBrowser.h
│   └── Platform/
│       └── Platform.h
├── src/
│   └── CWebBrowser.cpp
└── tests/
    ├── CMakeLists.txt
    └── test_webbrowser.cpp
```

## Building & Testing Standalone

```bash
cmake -B build -G Ninja -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```
