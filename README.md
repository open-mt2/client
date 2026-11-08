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

## Documentation

https://open-mt2.github.io/docs/client/
