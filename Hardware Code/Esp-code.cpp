#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN D3          
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN A0

const char* ssid = "Zaheer";
const char* password = "12345678";
const char* serverName = "https://script.google.com/macros/s/AKfycbyrVa_ikbSfVHGwoyCv6_bVTUGI1meqiuQTNWMvdWLHoMp6_Qyz5yryMlpDFHteA0GENA/exec";





DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  
  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClientSecure client;

    // Ignore SSL verification it is complsry for making a connection to google script
    client.setInsecure();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    String url = String(serverName) + "?temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&soilMoisture=" + String(soilMoistureValue);
    Serial.println("Sending GET request to: " + url);
    http.begin(client, url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  
  delay(10000); 
}
