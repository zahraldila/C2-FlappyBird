# 🐦 Flappy Bird - Proyek Pengembangan Aplikasi Berbasis Library (C2)

[![Raylib Badge](https://img.shields.io/badge/Made%20with-Raylib-blue.svg)](https://www.raylib.com/)
[![C Language Badge](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

## 🌟 Tentang Proyek Ini

Selamat datang di proyek Flappy Bird kami! Ini adalah implementasi klasik game Flappy Bird yang dikembangkan menggunakan bahasa pemrograman C dan didukung oleh library **Raylib**.

Tujuan utama proyek ini adalah untuk mengaplikasikan konsep-konsep pemrograman game, pengelolaan state, deteksi tabrakan, dan integrasi library eksternal dalam konteks pembelajaran. Kami telah berfokus pada pengalaman bermain yang responsif dan intuitif, lengkap dengan berbagai fitur tambahan untuk meningkatkan keseruan.

Proyek ini adalah bagian dari mata kuliah Pengembangan Aplikasi Berbasis Library, Program Studi D3 - Teknik Informatika, Jurusan Teknik Komputer dan Informatika.

## 📸 Tampilan Game

![Gameplay Flappy Bird](assets/tampilan1.png)
![Gameplay Flappy Bird](assets/tampilan2.png)

## ✨ Fitur Utama

* **Gameplay Klasik Flappy Bird:** Rasakan keseruan asli game ini dengan kontrol yang sederhana dan tantangan yang adiktif.
* **Gravitasi Realistis:** Gerakan burung yang responsif terhadap gravitasi dan input pemain.
* **Rintangan Pipa Dinamis:** Pipa-pipa yang bergerak secara horizontal sebagai rintangan utama.
* **Deteksi Tabrakan Akurat:** Penanganan tabrakan antara burung dengan pipa atau tanah untuk menentukan Game Over.
* **Sistem Skor:** Lacak skor tertinggi dan bersaing dengan pemain lain.
* **Leaderboard:** Catat skor tertinggi dan bersaing dengan teman-teman.
* **Menu Utama Interaktif:** Navigasi mudah ke berbagai opsi game.
* **Tampilan Game Over:** Layar informatif saat permainan berakhir.
* **Efek Suara dan Musik:** Pengalaman audio yang imersif untuk menambah keseruan.
* **Pilihan Latar Belakang:** Kustomisasi tampilan game Anda dengan berbagai pilihan background.

## 🎮 Cara Bermain

Meskipun terlihat sederhana, Flappy Bird membutuhkan ketepatan dan timing yang baik!

* **Lompat/Terbang:** Tekan tombol **SPACE BAR** untuk membuat burung melompat dan terbang lebih tinggi. Kendalikan frekuensi lompatan Anda untuk menjaga burung tetap di udara.
* **Menghindari Rintangan:** Tujuan utama adalah melewati **pipa-pipa** yang bergerak dari kanan ke kiri tanpa menabraknya.
* **Mencetak Skor:** Setiap kali burung berhasil melewati sepasang pipa, skor Anda akan bertambah 1.
* **Game Over:** Permainan berakhir jika burung menyentuh pipa, menabrak tanah, atau terbang terlalu tinggi hingga keluar layar.
* **Pause/Resume:** Saat bermain, tekan tombol **'P'** untuk menjeda atau melanjutkan permainan.
* **Restart Game:** Di layar Game Over, tekan **ENTER** untuk memulai permainan baru.
* **Kembali ke Menu:** Dari layar Game Over atau Bantuan, tekan **BACKSPACE** untuk kembali ke menu utama.
* **Input Nama:** Sebelum memulai permainan baru, Anda akan diminta memasukkan nama untuk dicatat di Leaderboard.

## 🚀 Instalasi dan Cara Menjalankan Game

Untuk dapat memainkan game ini dari repositori GitHub, Anda perlu mengkompilasi kode sumbernya. Proyek ini dibangun menggunakan **Raylib**, jadi Anda perlu menyiapkan lingkungan pengembangan yang sesuai.

### Langkah-langkah Cepat (Disarankan: Menggunakan VS Code)

#### 1. Clone Repositori & Inisialisasi Submodul

Buka terminal atau Git Bash Anda, lalu clone repositori dan inisialisasi submodul Raylib:

```bash
git clone [https://github.com/zahraldila/c2-flappybird.git](https://github.com/zahraldila/c2-flappybird.git)
cd c2-flappybird
git submodule update --init --recursive
```

#### 2. Buka Proyek di Visual Studio Code
1. Buka folder proyek c2-flappybird di VS Code (File > Open Folder...).
2. Pastikan Anda telah menginstal ekstensi C/C++ dari Microsoft (jika belum, VS Code biasanya akan merekomendasikannya).
3. Buka Command Palette (Ctrl+Shift+P atau Cmd+Shift+P).
4. Ketik "Tasks: Run Build Task" dan pilih opsi build release.
5. VS Code akan secara otomatis menjalankan perintah kompilasi yang diperlukan. Anda akan melihat prosesnya di panel "Terminal".
6. Setelah kompilasi selesai (tanpa error), Anda akan menemukan file executable (flappybird.exe di Windows atau flappybird di Linux/macOS) di direktori utama proyek.

#### 3. Jalankan Game
Dari terminal di direktori proyek, atau langsung dari VS Code jika executable sudah dibuat:

**Untuk Windows:**
```bash
.\flappybird.exe
```

**Untuk Linux/macOS:**
```bash
./flappybird
```

## 👥 Anggota Tim C2

| Nama                      | Tugas                                         | NIM         |
| :------------------------ | :-------------------------------------------- | :---------- |
| Alexandrio Vega Bonito    | Pembuatan dan Pergerakan Pipa                 | 241511067   |
| Dava Ramadhan             | Logika Burung (Gravitasi, Lompat) & Input Key | 241511070   |
| Qlio Amanda               | Integrasi Sound dan Sistem Skor               | 241511087   |
| Zahra Aldila              | Deteksi Tabrakan dan State Game               | 241511094   |
| Zakky Zhillan M.I         | Pengembangan Main Menu dan Integrasi Proyek   | 241511095   |

### Manager Proyek: Djoko Cahyo Utomo
