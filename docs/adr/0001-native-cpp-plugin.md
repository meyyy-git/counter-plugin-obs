# 0001 — Native C++ plugin, bukan script Lua atau browser source

Fiturnya sederhana (dua angka + hotkey), sehingga script Lua adalah kandidat wajar. Kami memilih plugin native C++ karena target distribusi adalah publik: plugin native bisa masuk ke OBS plugin store/registry, dipasang sebagai installer resmi, dan dibangun dengan obs-plugintemplate yang menyediakan CI multi-platform (Windows/macOS/Linux) out-of-the-box. Lua script akan lebih cepat dibuat tetapi distribusinya berupa file mentah tanpa pipeline rilis, dan browser source menuntut setup obs-websocket + hosting HTML di sisi user.
