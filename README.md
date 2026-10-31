# OpenMT2 client

Game layer: `GameLib`, `UserInterface` and the `metin2client` executable.

**Status:** scaffold. The `pkg_*` sources are not imported yet (`TB.2`).

| Path | Package | Role target |
|---|---|---|
| `src/CWebBrowser/` | `pkg_cwebbrowser` | — |
| `src/GameLib/` | `pkg_gamelib` | `Client::Game` |
| `src/UserInterface/` | `pkg_userinterface` | `Client::UI` |

`layout.map` records the old repository → path mapping used by
`devkit/scripts/import-repo.sh`. See `docs/architecture.md`.

Game assets (`bin/`) are devkit-owned and are not committed here.
