# ESP8266-API-Flooder

A lightweight API stress testing tool using ESP8266, capable of sending high-speed login requests to any specified endpoint.

## Features
- Rapid API request flooder for login stress testing.
- Mini DOS Mode for sending GET requests to specified URLs or IP addresses.
- Support for multiple target APIs.
- Dynamically add target APIs via Serial Monitor.
- Commands to add URLs, clear all URLs, start, and stop the flood.
- Overheating protection that pauses requests if too many are sent in a short period.
- Easily modify login credentials dynamically via Serial Monitor.
- Adjustable request burst rate and delay.
- Clean and organized Serial Monitor output for better monitoring.
- LED indicator with different blink patterns during operation.
- Splash screen disclaimer with IP and MAC address display.
- Dual help menus for Flood Request Mode and Mini DOS Mode.

## Usage

### 1️⃣ Flash the Code to ESP8266
Upload the `ESP8266-API-Flooder.ino` file to your ESP8266 board.

### 2️⃣ Splash Screen and Mode Selection
After flashing, open Serial Monitor. The program displays a disclaimer along with the device's IP and MAC address. Type:
```
I accept
```
to proceed.

Then select the mode:
```
mode flood
```
or
```
mode minidos
```

### 3️⃣ Set the Target API or IP
```
url https://yourapi.com/login
```
or for Mini DOS Mode:
```
url 192.168.1.1
```

### 4️⃣ Adjust Speed Settings
```
rate 20
```
```
delay 5
```

### 5️⃣ Set Login Credentials (Flood Mode Only)
```
user yourUsername
```
```
pass yourPassword
```

### 6️⃣ Start the Flood
```
start
```

### 7️⃣ Stop the Flood
```
stop
```

### 8️⃣ Help Menus
Type `help flood` for Flood Request Mode help or `help minidos` for Mini DOS Mode help.

## Example Serial Output
```
Flood Request: user123 / pass123 | Payload: 32 bytes | HTTP Code: 200 | Response: OK
Mini DOS Request | HTTP Code: 404 | Response: Not Found
Device overheating. Cooling down for 5 seconds.
```

## ⚠️ Disclaimer
This project is intended for **educational and security research purposes only.**
Use only on systems you own or have explicit permission to test.

## 🚀 Contributions
Feel free to submit pull requests or issues to improve the project.

## 📜 License
MIT License – Free to use, but at your own risk.

---
✨ Developed by [toxi360](https://github.com/Efeckc17)

