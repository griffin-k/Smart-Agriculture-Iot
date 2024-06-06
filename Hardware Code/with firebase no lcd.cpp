#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define LDR_PIN   13     
#define BUZZER_PIN 12     
#define THRESHOLD 500  

#define DHTPIN 4  
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN 34 
#define RAIN_SENSOR_PIN 35  
#define MOTOR_PIN 2  

const char* ssid = "Robotics";
const char* password = "12345678";

// Replace with your Firebase Realtime Database URL and secret
const char* firebaseHost = "smart-agricultrure-default-rtdb.firebaseio.com";
const char* firebaseAuth = "AIzaSyD_B-2C-klfrOAPtiGpkAFa_KEGg8aiEvo";

// Path to the node you want to update
const char* firebasePath = "/sensorData.json";

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
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);  

  pinMode(BUZZER_PIN, OUTPUT);  
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
    int rainSensorValue = digitalRead(RAIN_SENSOR_PIN);
    int ldrValue = digitalRead(LDR_PIN);  
    
    String pumpStatus = "OFF";
    String rainStatus = "No";
    String buzzerStatus = "Safe";  

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

 
    if (soilMoistureValue < 1500) {
      digitalWrite(MOTOR_PIN, HIGH);  
      pumpStatus = "ON";
    } else {
      digitalWrite(MOTOR_PIN, LOW); 
      pumpStatus = "OFF";
    }
    
    if (rainSensorValue == 0) {
      rainStatus = "Yes";
    } else {
      rainStatus = "No";
    }

   
    if (ldrValue == 1) {
      digitalWrite(BUZZER_PIN, HIGH);  
      buzzerStatus = "Risk"; 
    } else {
      digitalWrite(BUZZER_PIN, LOW);   
      buzzerStatus = "Safe"; 
    }

    // Prepare JSON payload
    String jsonPayload = "{";
    jsonPayload += "\"temperature\":" + String(temperature) + ",";
    jsonPayload += "\"humidity\":" + String(humidity) + ",";
    jsonPayload += "\"soilMoisture\":" + String(soilMoistureValue) + ",";
    jsonPayload += "\"rainSensor\":\"" + rainStatus + "\",";
    jsonPayload += "\"pumpStatus\":\"" + pumpStatus + "\",";
    jsonPayload += "\"buzzerStatus\":\"" + buzzerStatus + "\"";
    jsonPayload += "}";

    // Prepare the URL for the PUT request
    String url = String("https://") + firebaseHost + firebasePath + "?auth=" + firebaseAuth;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.PUT(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error on sending PUT: ");
      Serial.println(httpResponseCode);
    }
    http.end();
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Soil Moisture: ");
    Serial.print(soilMoistureValue);
    Serial.print(", Rain Sensor: ");
    Serial.print(rainSensorValue == 0 ? "Yes" : "No");
    Serial.print(", Pump Status: ");
    Serial.println(pumpStatus);
    Serial.print(", LDR Value: ");
    Serial.println(ldrValue);  
    Serial.print(", Buzzer Status: ");
    Serial.println(buzzerStatus);  
  } else {
    Serial.println("WiFi Disconnected");
  }
  
  delay(500);  // Increase the delay to avoid too frequent requests
}
