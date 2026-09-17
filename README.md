# CycloneDDS Console Simulator

Simulasi konsol C++ berbasis **Eclipse CycloneDDS** yang terintegrasi dengan Docker dan Docker Compose. Proyek ini menerapkan pemisahan arsitektur modul IDL (*Separation of Concerns*), penanganan *input/output* interaktif, serta *QoS Profile Factory* terpusat (`src/config/qos_profiles.h`).

---

## 🏗 Arsitektur & Topik DDS

Sistem terbagi menjadi dua grup QoS sesuai dengan kebutuhan taktis jaringan:

| Topik | Reliability | Durability | History | Deskripsi & Dampak Taktis |
| :--- | :--- | :--- | :---: | :--- |
| `Ship/MissionRoute` | `Reliable` | `TransientLocal` | 5 | Input manual rute pelayaran (*late-joiner* otomatis menerima sinkronisasi). |
| `Weapon/RCWSCommand` | `Reliable` | `TransientLocal` | 10 | Input manual perintah kendali senjata (*zero packet loss*). |
| `Weapon/RCWSStatus` | `Reliable` | `Volatile` | 1 | Status *safety interlock* dan amunisi RCWS. |
| `Ship/Telemetry` | `BestEffort` | `Volatile` | 1 | Transmisi telemetri otomatis (interval 3s, memprioritaskan latensi rendah). |
| `Tactical/TargetTrack` | `BestEffort` | `Volatile` | 1 | Transmisi data jejak target otomatis (interval 3s). |

---

## 📋 Prasyarat

* **Docker Engine** & **Docker Compose**
* **GCC/G++ (C++17/20)** & **CMake** (v3.16+) jika ingin dikompilasi secara lokal tanpa Docker
* Sertifikat DDS (CA, Node Certificate, & Private Key) pada direktori host (jika fitur *Security* CycloneDDS diaktifkan)

---

## 🚀 Konfigurasi & Persiapan

### 1. File Environment (`.env`)
Buat file `.env` pada direktori utama proyek untuk menentukan variabel identitas kapal dan path lokasi sertifikat:

```env
SHIP_ID=KRI-601
CERTS_PATH=/home/username/.config/certs
```
> *(Ganti `/home/username/...` dengan path absolut lokasi sertifikat di komputer Anda)*

### 2. File Konfigurasi DDS (`cyclonedds.xml`)

Salin templat `cyclonedds.xml.example` menjadi `cyclonedds.xml`:

```bash
cp cyclonedds.xml.example cyclonedds.xml
```

> ⚠️ **Catatan:** File  `cyclonedds.xml` perlu dikonfigurasi menyesuaikan kebutuhan.

---

## 🛠 Langkah Build & Eksekusi (Docker)

### 1. Build Docker Image

Kompilasi image menggunakan fitur *multi-stage build* (mengompilasi CycloneDDS C & C++ binding secara otomatis):

```bash
docker compose build
```

### 2. Menjalankan Simulasi Real-Time

Buka dua jendela terminal terpisah untuk mensimulasikan komunikasi interaktif antar-node:

**Terminal 1: Master Subscriber**

```bash
docker compose run --rm dds_simulator
```

> Pilih menu `1` (Subscriber). Terminal ini akan mendengarkan seluruh data dari 5 topik secara simultan.

**Terminal 2: Publisher**

```bash
docker compose run --rm dds_simulator
```

> Pilih menu `2` (Publisher), lalu pilih topik yang ingin dipublikasikan (misalnya `4` untuk Telemetry Data atau `1` untuk Mission Route).

---

## 🕹 Navigasi & Kontrol Console

* **Kembali ke Menu Utama:** Saat berada dalam *looping* transmisi (Telemetry / TargetTrack / Master Subscriber), tekan **[ENTER]** kapan saja untuk kembali ke menu awal tanpa mematikan program.
* **Menghentikan Kontainer Secara Paksa:** Tekan `Ctrl + C` atau jalankan perintah `docker compose kill` dari terminal terpisah jika program tidak merespons.
