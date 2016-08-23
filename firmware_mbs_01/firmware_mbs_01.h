
// variables modificables  durante debugging
boolean gtestSignal=false;
boolean gSimuladaSignal=false;
boolean gtestHEX=false;
boolean gtestCONTINUO=true;
int modo_salida=1;
// 1-hex 2-numeros 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex      

#include "Arduino.h"
#define isDUE  //Detect Arduino Due
#define SERIAL_SPEED 57600  //57600
#define SERIAL_SPEED_BT 115200
#define SEPARADOR_SERIAL ','
#define FINLINEA " "
// tenemos un modulo HC06 en serial3, y por tanto en pines pins 15 (RX) and 14 (TX)
// y prefiero el programing port, parece mas estable
#define HC06 Serial3  
#define WiredSerial Serial


// #include <Cmd.h>
/* 
The Arduino Due has three additional 3.3V TTL serial ports: Serial1 on pins 19 (RX) and 18 (TX); 
Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX). 
Pins 0 and 1 are also connected to the corresponding pins of the ATmega16U2 USB-to-TTL Serial chip,
which is connected to the USB debug port. 
Additionally, there is a native USB-serial port on the SAM3X chip, SerialUSB. 
y el Serial es el programming port
*/


/*  pines utilizados  del arduino due
  2 -- 
  3 --
  4 --
  5 --
  6 --
  7 -- clksel
  8 -- reset
  10 -- CS
  11 -- spi out
  12 -- spi in 
  13 -- spi luz led (preasignado) 
  14 -- HC06
  15 -- HC06
*/



// globales
int gMaxChan = 0; //maximum number of channels supported by ads1299 = 8
int gIDval = 0; //Device ID : lower 5 bits of  ID Control Register 
int gNumActiveChan = 0;
boolean gActiveChan [9]; // reports whether channels 1..9 are active
boolean isRDATAC = false;


char *gLetra; // buffer usado en to_hex, inicializado en setup()
int numSerialBytes=0;
unsigned char serialBytes[80];

int minComando=1;
int maxComando=7;

// protocolo openEEG firmware P2, util para openvibe
// solo registra 6 canales

unsigned char txBuf[33];  //17 en openeeg   32 enopenbci
unsigned long indice_paquete=0;


const int kPIN_LED = 13;//pin con luz led, en Teensy3, is ALSO spi clock!
const int kPIN_RESET = 8; //Reset en Pin 8
const int kPIN_CLKSEL = 7; //ClkSel en Pin 7


