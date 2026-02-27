#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// The following items are actually defined via build flags
// #define DHT22_PIN 21
// #define DHT11_PIN 2 
// #define OLED_RESET_PIN -1

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C

DHT_Unified dht22(DHT22_PIN, DHT22);
DHT_Unified dht11(DHT11_PIN, DHT11);

uint32_t delayMS;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

void setup() {
  Serial.begin(115200);
  delay(5000);  
  Serial.println("Initializing...");
  Wire.begin(SDA_PIN, SCL_PIN);


  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Initializing..."));
  display.display(); 

  dht22.begin();
  dht11.begin();
  sensor_t dht22_sensor;
  sensor_t dht11_sensor;
  delay(1000);

  dht22.temperature().getSensor(&dht22_sensor);
  Serial.println(F("DHT22:"));
  Serial.print(F("Sensor: ")); Serial.println(dht22_sensor.name);
  Serial.print(F("Driver Ver: ")); Serial.println(dht22_sensor.version);
  Serial.print(F("Unique ID: ")); Serial.println(dht22_sensor.sensor_id);
  Serial.print(F("Max Value: ")); Serial.print(dht22_sensor.max_value); Serial.println(F("°C"));
  Serial.print(F("Min Value: ")); Serial.print(dht22_sensor.min_value); Serial.println(F("°C"));
  Serial.print(F("Resolution: ")); Serial.print(dht22_sensor.resolution); Serial.println(F("°C"));
  Serial.print(F("Min Delay: ")); Serial.print(dht22_sensor.min_delay); Serial.println(F("ms"));
  Serial.println();

  dht11.temperature().getSensor(&dht11_sensor);
  Serial.println(F("DHT11:"));
  Serial.print(F("Sensor: ")); Serial.println(dht11_sensor.name);
  Serial.print(F("Driver Ver: ")); Serial.println(dht11_sensor.version);
  Serial.print(F("Unique ID: ")); Serial.println(dht11_sensor.sensor_id);
  Serial.print(F("Max Value: ")); Serial.print(dht11_sensor.max_value); Serial.println(F("°C"));
  Serial.print(F("Min Value: ")); Serial.print(dht11_sensor.min_value); Serial.println(F("°C"));
  Serial.print(F("Resolution:   ")); Serial.print(dht11_sensor.resolution); Serial.println(F("°C"));
  Serial.print(F("Min Delay: ")); Serial.print(dht11_sensor.min_delay); Serial.println(F("ms"));
  Serial.println();
  

}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("Dev Temp(F) Humi(%)"));
  display.println();

  sensors_event_t dht22Event;
  dht22.temperature().getEvent(&dht22Event);
  if (isnan(dht22Event.temperature)) {
    Serial.println("Error reading DHT22 temperature!");
  } else {
    float tempF22 = (dht22Event.temperature * 9/5) + 32;
    display.println("DHT22  " + String(tempF22) + "  "  + String(dht22Event.relative_humidity));
  }

  sensors_event_t dht11Event;
  dht11.temperature().getEvent(&dht11Event);
  if (isnan(dht11Event.temperature)) {
    Serial.println("Error reading DHT11 temperature!"); 
  } else {
    float tempF11 = (dht11Event.temperature * 9/5) + 32;
    display.println("DHT11  " + String(tempF11) + "  " + String(dht11Event.relative_humidity));
  }
  display.display();
  // DHT11 can't be read more than once every 2 seconds
  Serial.println("Waiting 5 seconds before next read...");
  delay(5000);
}
