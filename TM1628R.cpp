
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TM1628R.h"
byte _curpos = 0x00;
byte buffer[14] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//****************************************************************CONFIGURACION DEL DISPLAY
//Modificado pora el uso del display Redondo
const byte seg_addr[7]={0x02,0x03,0x04,0x05,0x06,0x0E,0x0F};//nº bit of digital segments

//                       SG0  SG1  SG2  SG3  SG4  SG5  SG6  SG7  SG8  SG9  SG10	SG11 SG12 SG13 PLY  PAU  VCD  DVD  CD   RET  DDD  DTS  MP4  MP3  CL1  CL2   //name   -|
const byte led_addr[26]={0x0D,0x00,0x02,0x0C,0x04,0x0A,0x08,0x0A,0x0B,0x08,0x07,0x0B,0x01,0x0D,0x01,0x00,0x03,0x02,0x03,0x06,0x04,0x06,0x09,0x09,0x05,0x05};//adress -| for the signs's and disc's leds
const byte led_val[26]= {0x02,0x02,0x02,0x02,0x02,0x02,0x80,0x80,0x02,0x02,0x01,0x01,0x01,0x01,0x02,0x80,0x01,0x80,0x02,0x80,0x80,0x02,0x01,0x02,0x01,0x02};//byte   -|

//****************************************************************FIN DE CONFIGURACION DEL DISPLAY

TM1628R::TM1628R(byte _dio_pin, byte _clk_pin, byte _stb_pin) {
  this->_dio_pin = _dio_pin;
  this->_clk_pin = _clk_pin;
  this->_stb_pin = _stb_pin;

  pinMode(_dio_pin, OUTPUT);
  pinMode(_clk_pin, OUTPUT);
  pinMode(_stb_pin, OUTPUT);

  digitalWrite(_stb_pin, HIGH);
  digitalWrite(_clk_pin, HIGH);

  sendCommand(0x40);
  sendCommand(0x80);

  digitalWrite(_stb_pin, LOW);
  send(0xC0);
  clear();
  digitalWrite(_stb_pin, HIGH);
}

void TM1628R::begin(boolean active = true, byte intensity = 1) {
  sendCommand(0x80 | (active ? 8 : 0) | min(7, intensity));
}

void TM1628R::update() {
  for (int i=0; i<14; i++)
    sendData(i, buffer[i]);
}

void TM1628R::setTime(int hour,int min,int sec) {

	if (hour >= 100) setSeg(0, (hour/100));
	if (hour >= 10) setSeg(1, (hour%100)/10);
	setSeg(2, (hour%100)%10);
	setSeg(3, (min/10));
	setSeg(4, (min%10));
	setSeg(5, (sec/10));
	setSeg(6, (sec%10));
	update();
}

void TM1628R::clear() {
	for (int i=0; i<14; i++)
		buffer[i]=0x00;
	_curpos=0x00;
	update();
}

void TM1628R::setLED(byte led){
	buffer[led_addr[led]] = buffer[led_addr[led]] ^ led_val[led];
	update();
}

void TM1628R::setLEDon(byte led){
	buffer[led_addr[led]] |= led_val[led];//= buffer[led_addr[led]] | led_val[led];
	update();
}

void TM1628R::setLEDoff(byte led){
	buffer[led_addr[led]] &= 0xFF - led_val[led];//= buffer[led_addr[led]] & (0xFF - led_val[led]);
	update();
}

void TM1628R::setDisc(boolean onoff){
	if (onoff) {
		for(int i=0;i<8;i++)// turn on all disc segments
			buffer[led_addr[i]] = buffer[led_addr[i]] | led_val[i];
	} else {
		for(int i=0;i<8;i++)
			buffer[led_addr[i]] = buffer[led_addr[i]] & (0xFF - led_val[i]);
	}
	update();
}

byte TM1628R::getButtons() {
  byte keys = 0;

  digitalWrite(_stb_pin, LOW);
  send(0x42);
  for (int i = 0; i < 5; i++) {
    keys |= receive() << i;
  }
  digitalWrite(_stb_pin, HIGH);

  return keys;
}

size_t TM1628R::write(byte chr){
	if(_curpos<0x07) {
		setChar(_curpos, chr);
		_curpos++;
	}
	
}

void TM1628R::setCursor(byte pos){
	_curpos = pos;
}
/*********** mid level  **********/
void TM1628R::sendData(byte addr, byte data) {
  sendCommand(0x44);
  digitalWrite(_stb_pin, LOW);
  send(0xC0 | addr);
  send(data);
  digitalWrite(_stb_pin, HIGH);
}

void TM1628R::sendCommand(byte data) {
  digitalWrite(_stb_pin, LOW);
  send(data);  
  digitalWrite(_stb_pin, HIGH);
}

void TM1628R::setSeg(byte addr, byte num) {
  for(int i=0; i<7; i++){
      bitWrite(buffer[i*2], seg_addr[addr], bitRead(NUMBER_FONT[num],i));
    }
}

void TM1628R::setChar(byte addr, byte chr) {
  for(int i=0; i<7; i++){
      bitWrite(buffer[i*2], seg_addr[addr], bitRead(FONT_DEFAULT[chr - 0x20],i));
    }
	update();
}
/************ low level **********/
void TM1628R::send(byte data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(_clk_pin, LOW);
    digitalWrite(_dio_pin, data & 1 ? HIGH : LOW);
    data >>= 1;     
    digitalWrite(_clk_pin, HIGH);
  }
}

byte TM1628R::receive() {
  byte temp = 0;

  // Pull-up on
  pinMode(_dio_pin, INPUT);
  digitalWrite(_dio_pin, HIGH);

  for (int i = 0; i < 8; i++) {
    temp >>= 1;

    digitalWrite(_clk_pin, LOW);

    if (digitalRead(_dio_pin)) {
      temp |= 0x80;
    }

    digitalWrite(_clk_pin, HIGH);
  }

  // Pull-up off
  pinMode(_dio_pin, OUTPUT);
  digitalWrite(_dio_pin, LOW);

  return temp;
}
