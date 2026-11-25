# OpenMT2 client

Game layer of the OpenMT2 client: game data, the UI and the `openmt2`
executable.

The executable is built at `build/openmt2` (CMake target `openmt2`). Headers are included as
`<Client/Game/...>`, `<Client/UI/...>` and `<Client/WebBrowser/...>`; the engine
and Python bridge keep their `<Neoeter/...>` roots.

Game assets (`bin/`) are devkit-owned and are not committed here.

## Build

```bash
cmake -S . -B build -G Ninja
cmake --build build --parallel
```

With no arguments the client builds standalone: it downloads the latest Neoeter
release and fetches `neoeter-python` from source.

| Option | Default | Purpose |
|---|---|---|
| `CLIENT_STANDALONE` | `ON` when the client is the top-level project, `OFF` under a superproject | Fetch `neoeter` / `neoeter-python` instead of relying on a parent project |
| `CLIENT_DOWNLOAD_NEOETER` | same as `CLIENT_STANDALONE` | Download a prebuilt Neoeter release instead of building it from source |
| `NEOETER_VERSION` | `latest` | Neoeter release tag, without the leading `v` |
| `NEOETER_SHA256` | empty | Expected archive hash; read from the release `.sha256` when empty |
| `NEOETER_ASSET_PLATFORM` | auto | Release asset suffix (`linux-x86_64`, `windows-x86_64`, `darwin-x86_64`) |
| `NEOETER_GIT_TAG` | `main` | Neoeter git ref when `CLIENT_DOWNLOAD_NEOETER=OFF` |
| `NEOETER_PYTHON_GIT_TAG` | `main` | `neoeter-python` git ref |
| `OPENMT2_SCRIPT_DOTNET` | `ON` | Build the .NET (C#) script runtime from `neoeter-dotnet` (needs the .NET 10 SDK) |
| `NEOETER_DOTNET_GIT_TAG` | `main` | `neoeter-dotnet` git ref |

`latest` resolves through the GitHub releases API. Pin `NEOETER_VERSION` for
reproducible builds; if no release exists yet, pass a tag explicitly.

Examples:

```bash
# Pin a Neoeter release
cmake -S . -B build -G Ninja -DNEOETER_VERSION=0.1.5

# Build Neoeter from source
cmake -S . -B build -G Ninja -DCLIENT_DOWNLOAD_NEOETER=OFF

# Build under a superproject that provides the Neoeter targets
cmake -S . -B build -G Ninja -DCLIENT_STANDALONE=OFF
```

## Script runtimes

The game scripts run on the embedded Python 2.7 runtime (`system.py`) or on
the .NET 10 runtime (`scripts/OpenMT2.Scripts.dll`). Both see the same native
modules (`app`, `chr`, `net`, `wndMgr`, …) plus a `scriptHost` module (`NAME`,
`DEBUG`, `COMMAND_LINE`), so scripts can move to C# gradually.

```bash
./openmt2 --script-runtime=auto     # default: .NET if scripts/OpenMT2.Scripts.dll exists, else Python
./openmt2 --script-runtime=python
OPENMT2_SCRIPT_RUNTIME=dotnet ./openmt2
```

The .NET runtime needs `<client>/dotnet` (`Neoeter.Scripting.dll`, staged by
the build) and a .NET 10 runtime: installed globally, through `DOTNET_ROOT`, or
bundled in `<client>/dotnet/runtime` (`devkit build --bundle-dotnet`).

`scripts/` holds a sample C# entry point (`OpenMT2.Scripts`). Build it with
`dotnet build scripts/OpenMT2.Scripts.slnx` and put the output under
`scripts/` in the packs (or next to the executable). C# can also redirect,
override or add pack files at run time through `Neoeter.Scripting.Packs.Pack`
(see `neoeter-dotnet/README.md`).

Migrating: port each Python module to C# calling the same native functions
through `Native.Import("<module>")`. Only one runtime is active per process;
`app.RunPythonFile` runs a `.py` file under Python and a script assembly under
.NET. Keep shipping `system.py` until the C# entry covers the whole game, then
add `OpenMT2.Scripts.dll` to the packs (`auto` switches over) or force a
runtime with `--script-runtime`.

## Documentation

https://open-mt2.github.io/docs/client/
