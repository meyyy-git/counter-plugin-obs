# Counter for OBS

Plugin OBS Studio untuk menampilkan counter sederhana di stream — death counter, skor Win/Lose, atau apa pun yang bisa dihitung. Tanpa styling ribet: tambah source **Counter**, pasang 3 tombol, langsung jalan.

```
Death Count: 12
W 3 / L 1
```

---

## Cara Pakai (untuk pemula)

### 1. Install plugin

Lihat bagian [Install](#install) di bawah — unduh, copy satu folder, restart OBS.

### 2. Tambahkan source Counter

1. Buka OBS.
2. Di panel **Sources** (kiri bawah), klik tombol **+**.
3. Pilih **Counter** dari daftar.
4. Beri nama bebas (mis. "Death Counter"), lalu klik **OK**.

### 3. Pilih Mode dan atur tampilan

Di jendela properties yang muncul:

- **Mode** — pilih:
  - **Death Counter** → tampil `Death Count: 0` (angka di slot A)
  - **Win / Lose** → ganti template jadi `W {A} / L {B}` (menang di A, kalah di B)
  - **Custom** → tulis template sendiri; gunakan `{A}` dan `{B}` sebagai angka
- **Font** dan **Color** — atur ukuran/warna teks.
- Klik **OK**. Teks counter langsung tampil di scene.

> Template adalah teks bebas: tulis `Kalah: {A} kali` dan itu yang muncul di layar, dengan `{A}` diganti angka.

### 4. Pasang 3 hotkey

Counter hanya berubah lewat hotkey, jadi langkah ini wajib:

1. Buka **File → Settings → Hotkeys**.
2. Gulir / cari bagian bernama sama dengan nama source Anda (mis. "Death Counter").
3. Ada 3 aksi — klik kolomnya lalu tekan tombol keyboard yang diinginkan:

| Aksi | Mode Death Counter | Mode Win / Lose |
|---|---|---|
| **Increment / Win** | Deaths +1 | Wins +1 |
| **Decrement / Lose** | Deaths −1 (berhenti di 0) | Losses +1 |
| **Reset Counters** | semua → 0 | semua → 0 |

Contoh: bind `F13`… tidak punya F13? Pakai tombol yang tidak dipakai game, mis. `Ctrl+Shift+1`. Klik **Apply**.

### 5. Selesai!

Tekan hotkey saat streaming — angka di layar langsung berubah. Nilainya otomatis tersimpan, aman walau OBS crash.

---

## Stream Deck

Plugin ini tidak punya kode khusus Stream Deck — Stream Deck cukup memicu hotkey OBS yang sama:

1. Bind 3 aksi di Settings → Hotkeys seperti di atas (pilih tombol yang tidak bentrok, mis. `Numpad 1/2/3`).
2. Di aplikasi Stream Deck, drag action **System → Hotkey** ke tombol, isi key yang sama.

Alternatif: pakai plugin resmi *OBS Studio* di Stream Deck dan pilih aksi **Trigger Hotkey**.

> Tips Win/Lose: tombol 1 = Win, tombol 2 = Lose, tombol 3 = Reset. Selesai.

## Auto-reset tiap sesi

Di properties Counter ada checkbox **Reset counters when streaming starts** — aktifkan kalau angka harus mulai dari 0 setiap kali mulai live. Ada juga tombol **Reset Counters Now** untuk reset manual.

## Data tersimpan di mana?

Setiap perubahan langsung ditulis ke file JSON di folder config OBS (`plugin_config/counter/counters.json`). Crash atau mati lampu tidak menghilangkan angka. Reset manual? Pakai tombol/hotkey Reset.

---

## Install

Unduh dari [halaman Releases](https://github.com/meyyy-git/counter-plugin-obs/releases) — ambil file sesuai OS Anda.

**Windows** (`counter-x.y.z-windows-x64.zip`):
1. Extract zip → dapat folder `counter` (isi: `bin\64bit\counter.dll` dan `data\`).
2. Copy folder `counter` ke `C:\ProgramData\obs-studio\plugins\` (buat foldernya kalau belum ada).
3. Restart OBS.

**macOS** (`counter-x.y.z-macos-universal.pkg`): double-click file .pkg, ikuti petunjuk, restart OBS.

**Ubuntu/Debian** (`counter-x.y.z-x86_64-linux-gnu.deb`): `sudo apt install ./counter-x.y.z-x86_64-linux-gnu.deb`, restart OBS.

Butuh OBS Studio 30.0 atau lebih baru.

### Plugin tidak muncul?

- Pastikan sudah **restart OBS** setelah copy file.
- Cek **Tools → Plugin Manager** (OBS 31+) — "counter" harus tercentang.
- Cek log OBS (`%APPDATA%\obs-studio\logs`): kalau ada `obs_register_source failed`, versi plugin salah — update ke release terbaru.

## Building (untuk developer)

Proyek ini berbasis [obs-plugintemplate](https://github.com/obsproject/obs-plugintemplate) resmi; CI mem-build Windows, macOS, dan Ubuntu otomatis di setiap push.

```sh
git clone https://github.com/meyyy-git/counter-plugin-obs.git
cmake --preset windows-x64   # atau macos / ubuntu-x86_64
cmake --build --preset windows-x64
```

Prasyarat toolchain: MSVC + CMake (Windows), Xcode (macOS), `build-essential` + `cmake` (Ubuntu). Panduan lengkap di [template](https://github.com/obsproject/obs-plugintemplate#readme).

## License

GPL-2.0 (mewarisi obs-plugintemplate).
