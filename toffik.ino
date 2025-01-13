#include <Adafruit_NeoPixel.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

#define PIN_UART_TX 0
#define PIN_UART_RX 1
#define PIN_LEDS_SIG 6 //6

#define PIN_SDA A4 //A0
#define PIN_SCL A5 //A1
#define PIN_TOF_OUTPUT A3 // pin 22 out1

#define NUM_LEDS 16
#define SETTINGS_CHECK_TIME_MS 1000
#define SETTINGS_PULSE_TIME_MS 1500

Adafruit_NeoPixel pixels(NUM_LEDS, PIN_LEDS_SIG, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  pinMode(PIN_TOF_OUTPUT,OUTPUT);
  digitalWrite(PIN_TOF_OUTPUT, HIGH);

  pixels.begin();
  pixels.clear();
 
  for(int i=0; i<NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, 255, 255, 255); // Dioda "i" oraz skladowe R=255 G=0 B=0
    pixels.show(); // Wysylamy dane do lancucha
    delay(100); // Opoznienie 500ms przed zaswieceniem kolejnej diody
  }

  while (!lox.begin()) { // TOF not detected
    
    //Serial.println("Failed to detect and initialize sensor!");
    
    for(int i=0; i<NUM_LEDS; i++)
    {
      pixels.setPixelColor(i, 255, 0, 0); // Dioda "i" oraz skladowe R=255 G=0 B=0
      pixels.show(); // Wysylamy dane do lancucha
    }delay (200);

    for(int i=0; i<NUM_LEDS; i++)
    {
      pixels.setPixelColor(i, 0, 0, 0); // Dioda "i" oraz skladowe R=255 G=0 B=0
      pixels.show(); // Wysylamy dane do lancucha
    }delay (200);
  }
}

void loop() {
  
  // TOF init
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  
  // TOF Measurement
  int range = measure.RangeMilliMeter;
  
  if (measure.RangeStatus != 4) { //TOF didn't fail
    if(300 < range && range <= 400){
      
      for(int i=0; i<(int)((NUM_LEDS*(range)-100)/400); i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 255));
          pixels.show();
      }
      for(int i=(int)(int)((NUM_LEDS*(range)-100)/400); i<NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          pixels.show();
      }
      
    }else if(200 < range && range <= 300){
      
      for(int i=0; i<(int)((NUM_LEDS*(range)-100)/400); i++) {
          pixels.setPixelColor(i, pixels.Color(0, 255, 0));
          pixels.show();
      }
      for(int i=(int)((NUM_LEDS*(range)-100)/400); i<NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          pixels.show();
      }
      
    }else if(100 < range && range <= 200){
      
      for(int i=0; i<(int)((NUM_LEDS*(range)-100)/400); i++) {
          pixels.setPixelColor(i, pixels.Color(255, 0, 0));
          pixels.show();
      }
      for(int i=(int)((NUM_LEDS*(range)-100)/400); i<NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          pixels.show();
      }
      
    }else if(range <= 100){
      
      for(int i=0; i<NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(255, 0, 0));
          pixels.show();
       }delay(100);
      for(int i=0; i<NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          pixels.show();
       }delay(100);
      
    }else{                                       //Hand not detected
      long z = (long) (millis()/SETTINGS_PULSE_TIME_MS);
      
      if(z%2){
        for(int i=0; i<NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, (millis()%SETTINGS_PULSE_TIME_MS)*255/SETTINGS_PULSE_TIME_MS));
          pixels.show();
        }
      }else{
        for(int i=0; i<NUM_LEDS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, (255-((millis()%SETTINGS_PULSE_TIME_MS)*255)/SETTINGS_PULSE_TIME_MS)));
          pixels.show();
        }
      }             
    }
  }
  else{                          //Range measurment failed    
    for(int i=0; i<NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();
    }              
  }
}
