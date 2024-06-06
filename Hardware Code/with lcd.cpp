#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define LDR_PIN   13
#define BUZZER_PIN 12
#define DHTPIN 4  
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN 34  
#define RAIN_SENSOR_PIN 35
#define MOTOR_PIN 2

const char* ssid = "Robotics";
const char* password = "12345678";

const char* firebaseHost = "smart-agricultrure-default-rtdb.firebaseio.com";
const char* firebaseAuth = "AIzaSyD_B-2C-klfrOAPtiGpkAFa_KEGg8aiEvo";
const char* firebasePath = "/sensorData.json"; // Point directly to sensorData node

DHT dht(DHTPIN, DHTTYPE);

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS 4

LiquidCrystal_PCF8574 lcd(LCD_ADDRESS);
HTTPClient http; // Declare HTTPClient object globally

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

  Wire.begin();
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.setBacklight(255);
}

void loop() {
  lcd.setBacklight(0);
  delay(120);
  lcd.setBacklight(255); 
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Welcome");
  lcd.setCursor(1, 2);
  lcd.print("Developed By");
  delay(2000);

  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("Muhammad Asif");
  lcd.setCursor(4, 2);
  lcd.print("Arslan Rasheed");
  delay(2000);

  if (WiFi.status() == WL_CONNECTED) {
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

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print(" %");
    lcd.setCursor(0, 2);
    lcd.print("Soil Moisture: ");
    lcd.print(soilMoistureValue);
    lcd.setCursor(0, 3);
    lcd.print("Rain Status: ");
    lcd.print(rainStatus);

    String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + ", \"soil_moisture\": " + String(soilMoistureValue) + ", \"rain_status\": \"" + rainStatus + "\", \"pump_status\": \"" + pumpStatus + "\", \"buzzer_status\": \"" + buzzerStatus + "\"}";

    String url = "https://" + String(firebaseHost) + firebasePath + "?auth=" + String(firebaseAuth);
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.sendRequest("PATCH", jsonData); // Use PATCH method to update existing data

    if (httpCode > 0) {
      Serial.printf("[HTTP] PATCH request to Firebase successful, status code: %d\n", httpCode);
    } else {
      Serial.printf("[HTTP] PATCH request to Firebase failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    delay(5000);
  } else {
    Serial.println("WiFi Disconnected");
  }
}
