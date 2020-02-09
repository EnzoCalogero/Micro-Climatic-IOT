#include "DHT.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#define DHTPIN 3   //RX pin
//#define DHTPIN 4 // IMPORTANT D2 on NodeMCU is GPIO 4

#define DHTTYPE DHT11 // DHT 11


const char *ssid = "VM5519451"; // Your wifi ssid
const char *password = "Tf2rydsjyzrn"; //Your wifi password

unsigned long myChannelNumber = 852298;
const char * myWriteAPIKey = "1PY74AK38D9KC0HL";

const char* server = "api.thingspeak.com";

WiFiClient client;


// start
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



#define BACKLIGHT_PIN     13

// LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27,20,4);

//LiquidCrystal_I2C lcd(0x38, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address


// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }
   
};



// end

DHT dht(DHTPIN, DHTTYPE);

void updateThingSpeak(float t, float h){
    // fields 1 2 salotto
    // fields 3, 4 camera nostra
    // fields 5,6 camera giulio
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, h);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(30*1000);
}

void setup() {
  lcd.init();   // initializing the LCD
  lcd.backlight();
  
  Serial.begin(115200);
  dht.begin();
  ThingSpeak.begin(client);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("waiting for wifi to be connected");
        lcd.setCursor(0, 0);
        lcd.print("waiting for wifi to be connected");
    }

  Serial.println("");
  Serial.println("WiFi connected");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


 
}

void loop() {
    lcd.init();   // initializing the LCD
    lcd.backlight();
 
    
    float h = dht.readHumidity();
    //celcius
    float t = dht.readTemperature();
    //fahrenheit
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celcius
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");


   // lcd added
  lcd.setCursor(0, 0);
  String hs="Humidity: "+String((int)f)+" % ";
  String ts="Temp: "+String((int)t)+" C ";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ts);
  lcd.setCursor(0, 1);
  lcd.print(hs);
 // lcd finished

    if(client.connect(server,80)){
        updateThingSpeak(t,h);
    }
  delay(20000);
}