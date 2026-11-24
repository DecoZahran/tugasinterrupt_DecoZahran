# Tugas 3 - Timer & Interrupt Bare-Metal STM32F401

| Keterangan | Data |
| :--- | :--- |
| **Nama** | Deco Zahraan Saputra |
| **NIM** | 101032400150 |
| **Kelas** | TK-48-05 |

## Konfigurasi Pin & Perangkat Keras

Sistem berjalan menggunakan sumber *clock* internal default **HSI 16 MHz** dengan jalur *clock* `RCC` aktif pada `GPIOA`, `TIM2`, `TIM3`, `TIM4`, dan `SYSCFG`.

| Pin | Komponen | Mode GPIO | Fungsi |
| :--- | :--- | :--- | :--- |
| `PA10` | LED Eksternal 1 + Resistor | Output Push-Pull | Indikator `TIM2` (1 Hz, Poin 1) & LED uji (Poin 2–3) |
| `PA11` | LED Eksternal 2 + Resistor | Output Push-Pull | Indikator `TIM3` (2 Hz) — Poin 1 |
| `PA12` | LED Eksternal 3 + Resistor | Output Push-Pull | Indikator `TIM4` (5 Hz) — Poin 1 |
| `PA0` | Push Button Bawaan (`KEY`) | Input Pull-Up | Input interupsi `EXTI0` & tombol kontrol (Poin 2–3) |

## Penjelasan Program (`Source_Codes/`)

### 1. `poin1_multiple_leds.c`
Mengoperasikan tiga *timer* secara paralel (*non-blocking*) untuk mengedipkan tiga LED eksternal dengan frekuensi berbeda melalui interupsi *update*:
* **`TIM2` (`PA10`):** `PSC = 15999`, `ARR = 499` tiap `500 ms` (**1 Hz**)
* **`TIM3` (`PA11`):** `PSC = 15999`, `ARR = 249` tiap `250 ms` (**2 Hz**)
* **`TIM4` (`PA12`):** `PSC = 15999`, `ARR = 99` tiap `100 ms` (**5 Hz**)

### 2. `poin2_debouncing.c`
Mengatasi efek pantulan mekanis (*bouncing*) pada tombol bawaan **`KEY` (`PA0`)** menggunakan interupsi eksternal **`EXTI0`** (`SYSCFG->EXTICR[0]` bit `[3:0]`, vektor `EXTI0_IRQn`, *falling edge*), jeda waktu **`TIM2` selama 50 ms** (`PSC = 15999`, `ARR = 49`), dan variabel `flag` untuk mengubah status LED `PA10` satu kali setiap kali tombol ditekan.

### 3. `poin3_push_to_blink.c`
Memantau kondisi tombol bawaan **`KEY` (`PA0`)** di dalam `while(1)`. Selama tombol ditekan (`LOW`), **`TIM3`** diaktifkan (`PSC = 15999`, `ARR = 99`) sehingga LED `PA10` berkedip tiap `100 ms` (**5 Hz**). Saat tombol dilepas, `TIM3` dimatikan, nilai `CNT` di-reset ke `0`, dan LED `PA10` langsung padam.
