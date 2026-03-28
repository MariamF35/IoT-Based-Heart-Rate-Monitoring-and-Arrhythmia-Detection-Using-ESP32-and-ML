#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Replace_with_your_wifi_name"; //wifi name
const char* password = "Wifi_Password"; //wifi password

String apiKey = "NACUMGJY33CHH6YL"; //You can replace with your READ API key
const char* server = "http://api.thingspeak.com/update";

int pulsePin = 23;
int threshold = 550;

unsigned long lastBeatTime = 0;
unsigned long lastSendTime = 0;
int BPM = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  int signal = analogRead(pulsePin);
  unsigned long now = millis();
  Serial.print("\nSignal: ");
  Serial.print(signal);
  Serial.print("\tMillis (now) = ");
  Serial.print(now);

  if (signal > threshold && (now - lastBeatTime) > 300) {
    BPM = 60000 / (now - lastBeatTime);
    lastBeatTime = now;
    Serial.println(BPM);
  }

  if (now - lastSendTime > 15000 && BPM > 0) {
    sendToThingSpeak(BPM);
    lastSendTime = now;
  }
}

void sendToThingSpeak(int bpm) {
  HTTPClient http;
  String url = String(server) + "?api_key=" + apiKey + "&field1=" + bpm;
  http.begin(url);
  http.GET();
  http.end();
}
