# ESP8266-API-Flooder


A lightweight API stress testing tool using ESP8266, capable of sending high-speed login requests to any specified endpoint.

## Features
- Rapid API request flooder for login stress testing
- Set target API dynamically via Serial Monitor
- Modify login credentials easily (default: `test1` / `test2`)
- Start and stop attacks with Serial commands
- Overheating protection to prevent hardware damage

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

