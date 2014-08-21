// Testing sketch for DVD LED module with TM1628 IC
// Written by Vasyl Yudin, oct 2012, public domain
// www.blockduino.org
#include <Time.h>
#include <TM1628R.h>
// Define - data pin D5, clock pin D4 and strobe pin D3
TM1628R dvdLED(5, 4, 3); 

int z=0;
int button;
int state=0;

void setup() {
  dvdLED.begin(ON, 2); 
  setTime(2, 12, 13, 24, 10, 12);
  
// Probar un byte de la memoria

  dvdLED.sendData(0,0x01);
  delay(500);  
  dvdLED.sendData(0,0x02);
  delay(500);
  dvdLED.sendData(0,0x04);
  delay(500);  
  dvdLED.sendData(0,0x08);
  delay(500);
  dvdLED.sendData(0,0x10);
  delay(500);
  dvdLED.sendData(0,0x20);
  delay(500);
  dvdLED.sendData(0,0x40);
  delay(500);
  dvdLED.sendData(0,0x80);
  delay(500);
   
}

void loop() {

}

