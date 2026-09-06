# Counter for OBS

A native OBS Studio plugin that shows simple counters on your stream — a death counter, a win/lose score, anything countable. No styling, no dependencies: add a **Counter** source, bind hotkeys, go live.

```
Death Count: 12
W 3 / L 1
```

## How it works

Each **Counter** source has two independent slots (**A** and **B**) and a text template with `{A}` and `{B}` placeholders. The default template is `Death Count: {A}`; change it to `W {A} / L {B}` for a win/lose counter.

Every source registers three hotkeys in **Settings → Hotkeys** (under the source's name):

| Hotkey | Death Counter mode | Win / Lose mode |
|---|---|---|
| Increment / Win | `A + 1` | `W + 1` |
| Decrement / Lose | `A - 1` (stops at 0) | `L + 1` |
| Reset Counters | both → 0 | both → 0 | Font, color, template, and a **Reset counters when streaming starts** option are in the source's properties, along with a **Reset Counters Now** button.

## Stream Deck

The plugin has no Stream Deck-specific code — your Stream Deck triggers the same hotkeys OBS registers:

1. Bind the actions you want in OBS **Settings → Hotkeys** (e.g. `F13` for Increment Counter A, `F14` for Reset).
2. In the Stream Deck app, add a **System → Hotkey** action and set the same key.

Or use Elgato's official *OBS Studio* Stream Deck plugin: it can trigger OBS hotkeys directly from a button.

## Persistence

Slot values are written to a JSON file (`config/obs-studio/plugin_config/counter/counters.json`) on every change, so a crash mid-stream can't lose your count. If **Reset counters when streaming starts** is enabled, the counter resets each time you go live.

## Install

Download the installer for your OS from the [releases page](https://github.com/plugin-counter/counter/releases) (produced by CI: `.exe` for Windows, `.pkg` for macOS, `.deb`/`.tar.xz` for Ubuntu) or copy a build manually:

- **Windows:** `counter.dll` → `%APPDATA%\obs-studio\plugins\counter\bin\64bit\`, `data\locale\` → `%APPDATA%\obs-studio\plugins\counter\data\locale\`
- **macOS:** bundle → `~/Library/Application Support/obs-studio/plugins/`
- **Linux:** `counter.so` → `~/.config/obs-studio/plugins/counter/bin/64bit/`, data → `~/.config/obs-studio/plugins/counter/data/`

Requires OBS Studio 30.0 or newer.

## Building

The project is based on the official [OBS plugin template](https://github.com/obsproject/obs-plugintemplate). CI builds Windows, macOS, and Ubuntu automatically on every push.

Locally (all platforms):

```sh
git clone https://github.com/plugin-counter/counter.git
cmake --preset windows-x64   # or macos, ubuntu-x86_64
cmake --build --preset windows-x64
```

See the [template's build guide](https://github.com/obsproject/obs-plugintemplate#readme) for toolchain prerequisites (MSVC + CMake on Windows, Xcode on macOS, `build-essential` + `cmake` on Ubuntu).

## License

GPL-2.0 (inherited from the OBS plugin template).
