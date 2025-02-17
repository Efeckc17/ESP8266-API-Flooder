# ESP8266-API-Flooder


A lightweight API stress testing tool using ESP8266, capable of sending high-speed login requests to any specified endpoint.

## Features
- Rapid API request flooder for login stress testing.
- Support for multiple target APIs (up to 10).
- Dynamically add target APIs via Serial Monitor.
- Commands to add URLs, clear all URLs, start, and stop the flood.
- Overheating protection that pauses requests if too many are sent in a short period.
- Easily modify login credentials (default: `test2` as username and `test1` as password).
- Clean and organized Serial Monitor output for better monitoring.

## Usage

### 1️⃣ Flash the Code to ESP8266
Upload the `ESP8266-API-Flooder.ino` file to your ESP8266 board.

### 2️⃣ Set the Target API
Open Serial Monitor and enter:
```
url https://yourapi.com/login
```

### 3️⃣ Start the Flood
```
start
```
ESP8266 will begin sending requests rapidly.

### 4️⃣ Stop the Flood
```
stop
```

### 5️⃣ Change Login Credentials
Modify the code in `sendFloodRequest()` to set custom username & password.
Alternatively, you can update the credentials dynamically via Serial Monitor in a future update.

## Example Serial Output
```
Request Sent: test2 / test1 Response: 200
Request Sent: test2 / test1  Response: 401
Device overheating. Cooling down for 5 seconds.
```

## ⚠️ Disclaimer
This project is intended for **educational and security research purposes only.**
Use only on systems you own or have explicit permission to test.

## 🚀 Contributions
Feel free to submit pull requests or issues to improve the project.

## 📜 License
MIT License – Free to use, but at your own risk.

