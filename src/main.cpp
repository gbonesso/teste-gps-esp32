/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete instructions at https://RandomNerdTutorials.com/esp32-neo-6m-gps-module-arduino/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

 // Define the RX and TX pins for Serial 2
#include <HardwareSerial.h>
#include <TinyGPS++.h>
//#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define RXD2 16
#define TXD2 17

#define GPS_BAUD 9600

//Pin for ESP32
// #define TFT_CS         5  //case select connect to pin 5
// #define TFT_RST        15 //reset connect to pin 15
// #define TFT_DC         32 //AO connect to pin 32  (not sure that this is really used)  try pin 25
// #define TFT_MOSI       23 //Data = SDA connect to pin 23
// #define TFT_SCLK       18 //Clock = SCK connect to pin 18

#define TFT_DC 12 //A0
#define TFT_CS 13 //CS
#define TFT_MOSI 14 //SDA
#define TFT_CLK 27 //SCK
#define TFT_RST 2 // RST
#define TFT_LED 25 // LED

// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

//powering the tft display...

//GND is GND  that is the easy one
//LED pin on TFT will control brightness.  Using DAC on the ESP32 can control brightness by connecting
//to this pin.  See code below.  If this pin is high, VCC on the TFT does not need to be connected, but
//things seem to work better when it is connected.  VCC can go to either the 3.3 volt or the 5 volt (Vin)
//of the ESP32.  There does not appear to be any appreciable brightness difference when 5v is used instead of 3.3
//But the screen is brightest when 3.3 volts for VCC and 5volts for LED.

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

TinyGPSPlus gps;

void setup(){
  // Serial Monitor
  Serial.begin(9600);
  
  // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 started at 9600 baud rate");
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.fillScreen(ST77XX_BLACK);
  int w = tft.width();
  int h = tft.height();
  Serial.print("width =");
  Serial.println(w);
  Serial.print("height =");
  Serial.println(h);
  //expecting 128 x 160

  // Set TFT_LED to output DAC and start in 255
  dacWrite(TFT_LED, 255);
  // pinMode(TFT_RST, OUTPUT);
  // digitalWrite(TFT_RST, LOW);
}

/**
 Possible colors that are predefined...
 
ST7735_BLACK ST77XX_BLACK
ST7735_WHITE ST77XX_WHITE
ST7735_RED ST77XX_RED
ST7735_GREEN ST77XX_GREEN
ST7735_BLUE ST77XX_BLUE
ST7735_CYAN ST77XX_CYAN
ST7735_MAGENTA ST77XX_MAGENTA
ST7735_YELLOW ST77XX_YELLOW
ST7735_ORANGE ST77XX_ORANGE

*/


void loop(){
  while (gpsSerial.available() > 0){
    // get the byte data from the GPS
    char gpsData = gpsSerial.read();
    Serial.print(gpsData);
    gps.encode(gpsData);
    if (gps.location.isUpdated()) {
      Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("LONG: "); 
      Serial.println(gps.location.lng(), 6);
      Serial.print("SPEED (km/h) = "); 
      Serial.println(gps.speed.kmph()); 
      Serial.print("ALT (min)= "); 
      Serial.println(gps.altitude.meters());
      Serial.print("HDOP = "); 
      Serial.println(gps.hdop.value() / 100.0); 
      Serial.print("Satellites = "); 
      Serial.println(gps.satellites.value()); 
      Serial.print("Time in UTC: ");
      Serial.println(String(gps.date.year()) + "/" + String(gps.date.month()) + "/" + String(gps.date.day()) + "," + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
      Serial.println("");
      // a single pixel
      //tft.fillScreen(ST77XX_BLACK);   
      tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK); // Setting background collor clear the previous text
      tft.setTextSize(1);
      tft.setCursor(5, 5);
      tft.print("LAT: ");
      tft.println(gps.location.lat(), 6);
      tft.setCursor(5, 15);
      tft.print("LON: "); 
      tft.println(gps.location.lng(), 6);
      tft.setCursor(5, 25);
      tft.print("SPEED (km/h): "); 
      tft.println(gps.speed.kmph()); 
      tft.setCursor(5, 35);
      tft.print("ALT (min): "); 
      tft.println(gps.altitude.meters());
      tft.setCursor(5, 45);
      tft.print("HDOP: "); 
      tft.println(gps.hdop.value() / 100.0); 
      tft.setCursor(5, 55);
      tft.print("Satellites: "); 
      tft.println(gps.satellites.value()); 
      tft.setCursor(5, 75);
      tft.print("Time UTC: ");
      tft.setCursor(5, 85);
      tft.println(String(gps.date.year()) + "/" + String(gps.date.month()) + "/" + String(gps.date.day()));
      tft.setCursor(5, 95);
      tft.println(String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
    }
  }
  delay(1000);
  Serial.println("-------------------------------");

  


}