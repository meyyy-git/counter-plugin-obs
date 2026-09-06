# Counter for OBS

**Language / Bahasa:** [English](README.md) · [Bahasa Indonesia](README.id.md)

A native OBS Studio plugin that shows simple counters on your stream — a death counter, a win/lose score, anything countable. No styling fuss: add a **Counter** source, bind 3 buttons, done.

```
Death Count: 12
W 3 / L 1
```

---

## How to Use (beginner friendly)

### 1. Install the plugin

See [Install](#install) below — download it, copy one folder, restart OBS.

### 2. Add a Counter source

1. Open OBS.
2. In the **Sources** panel (bottom left), click the **+** button.
3. Pick **Counter** from the list.
4. Name it anything you like (e.g. "Death Counter") and click **OK**.

### 3. Pick a Mode and style it

In the properties window that appears:

- **Mode** — choose:
  - **Death Counter** → displays `Death Count: 0` (the number is slot A)
  - **Win / Lose** → switches the template to `W {A} / L {B}` (wins in A, losses in B)
  - **Custom** → write your own template; use `{A}` and `{B}` as the numbers
- **Font** and **Color** — size and color of the text.
- Click **OK**. The counter text appears on your scene right away.

> The template is free text: write `Deaths: {A} so far` and that's what shows on screen, with `{A}` replaced by the number.

### 4. Bind the 3 hotkeys

The counter only changes through hotkeys, so this step is required:

1. Open **File → Settings → Hotkeys**.
2. Scroll to the section named after your source (e.g. "Death Counter").
3. There are 3 actions — click a field and press the key you want:

| Action | Death Counter mode | Win / Lose mode |
|---|---|---|
| **Increment / Win** | Deaths +1 | Wins +1 |
| **Decrement / Lose** | Deaths −1 (stops at 0) | Losses +1 |
| **Reset Counters** | all → 0 | all → 0 |

No F13 key? Use something games don't grab, like `Ctrl+Shift+1`. Click **Apply**.

### 5. Done!

Press your hotkeys while streaming — the number on screen changes instantly. Values are saved automatically, so an OBS crash won't lose your count.

---

## Stream Deck

The plugin has no Stream Deck-specific code — your Stream Deck simply triggers the same OBS hotkeys:

1. Bind the 3 actions in Settings → Hotkeys as above (pick keys nothing else uses, e.g. `Numpad 1/2/3`).
2. In the Stream Deck app, drag a **System → Hotkey** action onto a button and set the same key.

Alternative: install the official *OBS Studio* plugin for Stream Deck and use its **Trigger Hotkey** action.

> Win/Lose tip: button 1 = Win, button 2 = Lose, button 3 = Reset. Done.

## Auto-reset each session

The Counter properties have a **Reset counters when streaming starts** checkbox — enable it if the number must start at 0 every time you go live. There's also a **Reset Counters Now** button for a manual reset.

## Where is the data stored?

Every change is written immediately to a JSON file in OBS's config folder (`plugin_config/counter/counters.json`). A crash or power cut won't lose the number. To reset manually, use the Reset button/hotkey.

---

## Install

Download from the [Releases page](https://github.com/meyyy-git/counter-plugin-obs/releases) — grab the file for your OS.

**Windows** (`counter-x.y.z-windows-x64.zip`):
1. Extract the zip → you get a `counter` folder (containing `bin\64bit\counter.dll` and `data\`).
2. Copy that folder into `C:\ProgramData\obs-studio\plugins\` (create the folder if it doesn't exist).
3. Restart OBS.

**macOS** (`counter-x.y.z-macos-universal.pkg`): double-click the .pkg, follow the prompts, restart OBS.

**Ubuntu/Debian** (`counter-x.y.z-x86_64-linux-gnu.deb`): `sudo apt install ./counter-x.y.z-x86_64-linux-gnu.deb`, restart OBS.

Requires OBS Studio 30.0 or newer.

### Plugin doesn't show up?

- Make sure you **restarted OBS** after copying the files.
- Check **Tools → Plugin Manager** (OBS 31+) — "counter" should be checked.
- Check the OBS log (`%APPDATA%\obs-studio\logs`): if you see `obs_register_source failed`, you have an old plugin build — update to the latest release.

## Building (for developers)

The project is based on the official [OBS plugin template](https://github.com/obsproject/obs-plugintemplate); CI builds Windows, macOS, and Ubuntu automatically on every push.

```sh
git clone https://github.com/meyyy-git/counter-plugin-obs.git
cmake --preset windows-x64   # or macos / ubuntu-x86_64
cmake --build --preset windows-x64
```

Toolchain prerequisites: MSVC + CMake (Windows), Xcode (macOS), `build-essential` + `cmake` (Ubuntu). Full guide in the [template README](https://github.com/obsproject/obs-plugintemplate#readme).

## License

GPL-2.0 (inherited from the OBS plugin template).
