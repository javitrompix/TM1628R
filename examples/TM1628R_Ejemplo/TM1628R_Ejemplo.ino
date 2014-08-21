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
     
}

void loop() {

     ShowLEDs();  

     dvdLED.clear();
     delay(500);

     ShowTime();
     delay(500);
     dvdLED.clear();
     
     ShowPrint();
        
     dvdLED.clear();
}

void ShowTime(){
  dvdLED.setTime(hour(), minute(), second());
  
  delay(50);
  dvdLED.setLED(z++%8);
  
  if (millis()/500 & 0x01){
      dvdLED.setLEDon(LED_CL1);
      dvdLED.setLEDon(LED_CL2);
    } else {
      dvdLED.setLEDoff(LED_CL1);
      dvdLED.setLEDoff(LED_CL2);
    }
}

void ShowLEDs(){
  for(int i=0;i<27;i++){
    dvdLED.setLEDon(i);
    delay(100);    
  }
  
  for(int i=0;i<27;i++){
    dvdLED.setLEDoff(i);
    delay(25);    
  }
}

void ShowPrint(){
int z=0;
    dvdLED.clear();
    dvdLED.print("HOLA");
    dvdLED.setLED(z++);
    delay(500);
    
// пример взят из reference/Serial_Print.html
    dvdLED.clear();
    dvdLED.print(78);//DEC default
    dvdLED.setLED(z++);
    delay(500);

    dvdLED.clear();
    dvdLED.write(78);//Para mostar el caracter ASCI 78 "N"
    dvdLED.setLED(z++);
    delay(500);

    dvdLED.clear();
    dvdLED.print(78, BIN);
    dvdLED.setLED(z++);
    delay(500);
    
    dvdLED.clear();
    dvdLED.print(78, OCT);
    dvdLED.setLED(z++);
    delay(500);
    
    dvdLED.clear();
    dvdLED.print(78, HEX);
    dvdLED.setLED(z++);
    delay(500);
    
    dvdLED.clear();
    dvdLED.print(-1.2345678);
    dvdLED.setLED(z++);
    delay(500);
    
    dvdLED.clear();
    dvdLED.print(-1.2345678, 4);
    dvdLED.setLED(z++);
    delay(500);
}
