# 🌱 Smart Irrigation System using ESP32 (Web Dashboard Controlled)

This project monitors soil moisture and allows pump/valve control through a web dashboard hosted directly on the ESP32. No external cloud is required — works fully on your local Wi-Fi network.

## Features
- Live soil moisture percentage display
- Pump control with ON/OFF buttons
- Works on phone or PC browser
- REST API for automation (/status, /on, /off)
- Calibration support for accurate readings

## Hardware Connections
| ESP32 Pin | Component |
|----------|-----------|
| GPIO34 | Soil Moisture Sensor AOUT |
| GPIO26 | Relay IN |
| 3.3V | Sensor VCC |
| GND | Sensor GND + Relay GND |

## Setup
1. Update Wi-Fi Name & Password in code.
2. Upload to ESP32 using Arduino IDE.
3. Open Serial Monitor and note ESP32 IP.
4. Open browser → `http://<IP Address>/`

## Calibration
Adjust these values in code:
