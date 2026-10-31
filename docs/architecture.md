# Architecture — OpenMT2 client

## Role

The game, in the sense the engine is not: data formats of Metin2 and the
application that turns content into a running client.

| Package | Responsibility |
|---|---|
| `GameLib` | item/race/skill tables, network packet structs, game-side data managers |
| `UserInterface` | application loop, scene setup, 38 Python game modules, `metin2client` |
| `CWebBrowser` | no-op stub keeping the UI's web-page entry points linkable |

## Dependency boundaries

```
client ──► neoeter-python ──► neoeter
   └──────────────────────────►
```

- the client may include `<EterBase/…>`, `<EterLib/…>`, `<ScriptLib/…>` — the
  historic include roots are preserved by the move;
- the client may **not** include another game-internal implementation detail
  through a different repository: everything game-side is here;
- `neoeter` and `neoeter-python` never include a client header. If a change
  needs that direction, it belongs in `neoeter` as a generic service.

## Module registration

`UserInterface` owns the game modules. Each is a flat C function table
(`Py_InitModule("player", …)`, `"item"`, `"skill"`, `"chrmgr"`, `"quest"`, …),
registered after `neoeter-python` has created the interpreter. This is the seam
where a future C# host can take over one module at a time
(`../../neoeter/docs/scripting-hosts.md`).

## Assets

Not in this repository. `bin/` lives in the devkit (asset fetcher) and is
mounted/copied at dist assembly time. Code and content have different
provenance and licenses; keeping them apart is deliberate.

## Executable

`UserInterface` defines the `metin2client` target. The devkit pins its binary
directory to `build/src/UserInterface`, so `build.sh`, `smoke.sh` and the dist
layout are unchanged by the move.
