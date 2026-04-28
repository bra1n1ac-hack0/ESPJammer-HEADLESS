# ESPJAMMER HEADLESS

<img src="https://img.shields.io/badge/ESP32-WROOM-000000?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP32">
<img src="https://img.shields.io/badge/PlatformIO-000000?style=for-the-badge&logo=platformio&logoColor=white" alt="PlatformIO">

**A minimal, headless ESP32-WROOM firmware for WiFi deauthentication and BLE spam.**

Designed strictly for **educational purposes** and **isolated lab environments**.

---

## Features

- Automatic WiFi Deauthentication (broadcast deauth attacks)
- BLE Advertisement Spam (disruption / noise generation)
- Fully headless — runs immediately on power-up
- No web server, no buttons, no display
- Optimized for continuous operation
- Easy flashing via browser (WebSerial)

## Disclaimer

> **⚠️ For educational and authorized lab use only.**  
> Do not use this firmware on networks or devices you do not own or have explicit permission to test.  
> Misuse may violate local laws.

---

## Quick Flash (Recommended)

Open the web flasher in **Chrome or Edge**:

→ **[Flash Firmware](https://bra1n1ac-hack0.github.io/ESPJammer-HEADLESS/)**

Just plug in your ESP32 and click **Connect**.

---

## Hardware

- ESP32-WROOM (any 4MB flash variant recommended)
- USB connection for flashing

*(nRF24L01+ support prepared but currently unused)*

---

## How It Works

- **WiFi Deauth**: Continuously scans for access points and sends raw 802.11 deauthentication frames.
- **BLE Spam**: Floods BLE advertisements to create noise and disrupt nearby BLE scanners/devices.
- Runs on dual FreeRTOS tasks for better performance.

---

## Build Locally

```bash
# Clone the repo
git clone https://https://github.com/bra1n1ac-hack0/ESPJammer-HEADLESS
cd esp32-deauther-headless

# Build firmware
pio run

# Flash via USB
pio run -t upload
