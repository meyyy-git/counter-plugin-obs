# Counter Plugin

Plugin OBS (native C++) untuk menampilkan counter sederhana di scene — misalnya death counter atau skor Win/Lose — yang di-increment lewat hotkey atau Stream Deck.

## Language

**Counter**:
Satu instance source di OBS yang menampilkan pasangan angka dan punya hotkey-nya sendiri.
_Avoid_: death counter (itu hanya salah satu pemakaian), widget, overlay

**Slot**:
Salah satu dari dua bilangan bulat independen di dalam sebuah Counter, disebut Slot A dan Slot B. Slot tidak punya makna bawaan; maknanya ditentukan oleh Template.
_Avoid_: counter A/B, angka, field

**Template**:
Teks format milik Counter dengan placeholder `{A}` dan `{B}` yang menentukan apa yang dirender, mis. `Death Count: {A}` atau `W {A} / L {B}`.
_Avoid_: format, layout, label

**Increment / Decrement**:
Menambah atau mengurangi nilai satu Slot tepat satu per satu per penekanan hotkey.
_Avoid_: add, minus, klik

**Reset**:
Mengembalikan kedua Slot ke 0.
_Avoid_: clear, restart

**Auto-reset**:
Opsi per-Counter (checkbox) untuk menjalankan Reset otomatis saat sesi streaming baru dimulai.
_Avoid_: reset on start

**Persist File**:
File JSON di folder config OBS tempat nilai Slot setiap Counter disimpan, ditulis pada setiap perubahan.
_Avoid_: save file, cache, database

**Hotkey Binding**:
Pemetaan aksi Counter (Increment/Decrement/Reset per Slot) ke tombol keyboard yang didaftarkan per-Counter di Settings → Hotkeys OBS. Stream Deck memicu aksi yang sama dengan memanggil hotkey ini.
_Avoid_: shortcut, keybind
