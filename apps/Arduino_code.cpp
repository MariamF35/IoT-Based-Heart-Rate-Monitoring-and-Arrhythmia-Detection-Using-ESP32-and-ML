#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

String apiKey = "YOUR_THINGSPEAK_WRITE_KEY";
const char* server = "http://api.thingspeak.com/update";

int pulsePin = 34;   // Analog pin
int threshold = 550;

unsigned long lastBeat = 0;
int BPM = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int signal = analogRead(pulsePin);

  if (signal > threshold) {
    unsigned long now = millis();
    if (lastBeat > 0) {
      BPM = 60000 / (now - lastBeat);
      Serial.print("BPM: ");
      Serial.println(BPM);
      sendToThingSpeak(BPM);
    }
    lastBeat = now;
    delay(1000);
  }
}

void sendToThingSpeak(int bpm) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server + String("?api_key=") + apiKey + "&field1=" + bpm;
    http.begin(url);
    http.GET();
    http.end();
  }
}
