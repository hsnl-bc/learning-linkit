# learning-linkit

MCS's public page for DEMO purpose: [https://mcs.mediatek.com/public/devices/DOuHxFwk](https://mcs.mediatek.com/public/devices/DOuHxFwk)

## Requirement
* Linkit 7697
* Arduino IDE
### DHTWithOLED
* Grove-Temperature&Humidity Sensor Pro
* Grove-OLED Display 0.96"
### LBLEService
Scan LBLE's characteristic of Temperature&Humidity service, and then request (POST)  to mcs api.
* Windows 10 & Linux with BlueZ >= 5.43
* Bluetooth (use [Bleak](https://bleak.readthedocs.io))
* Python 3
```sh
pip install bleak requests
```
Replace the device id and characteristic uuid with yours.
### WIFI
Set your SSID and password.
### MotionTracking
* IMU 9DOF
* Grove-OLED Display 0.96"
