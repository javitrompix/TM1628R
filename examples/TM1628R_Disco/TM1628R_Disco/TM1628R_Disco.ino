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

  cargaCD(); 
  delay(500);     
  vueltaCD();
  delay(500);     
  vueltaCDf();
  delay(500);   
}


void cargaCD(void){
  for(int i=0;i<14;i++){
    dvdLED.setLEDon(i);
    delay(20);    
  }
}


void vueltaCD(void){

  for(int i=0;i<14;i++){
    dvdLED.setLEDoff(i);
  }
  
  for(int i=0;i<14;i++){
    dvdLED.setLEDon(i);
    
    if(i!=0){
    dvdLED.setLEDoff(i-1);      
    }
 
    delay(20);

    if(i>=13){
    dvdLED.setLEDoff(i);      
    } 
   
  }   
}


void vueltaCDf(void){

  for(int i=0;i<14;i++){
    dvdLED.setLEDoff(i);
  }
  
  for(int i=0;i<14;i=i+2){
    dvdLED.setLEDon(i);
    dvdLED.setLEDon(i+1);    
    if(i!=0){
    dvdLED.setLEDoff(i-1); 
    dvdLED.setLEDoff(i-2);    
    }
 
//    delay(20);

    if(i>=12){
    dvdLED.setLEDoff(i);
    dvdLED.setLEDoff(i+1);    
    }  
  
  }   
}
