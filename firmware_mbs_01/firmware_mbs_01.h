/* util.h */
#ifndef FIRMWARE_MBS_01
#define FIRMWARE_MBS_01

#include "Arduino.h"


#define MODE_SENAL_TEST 2 
#define MODE_SENAL_REAL_1x 1 
#define MODE_SENAL_REAL_12x 3 

#define SENAL_TEST 2 
#define SENAL_REAL 1 
#define TABLA_SENO 3 
#define TABLA_LINEAL 4
#define TABLA_RAMPA 5


// variables modificables  durante debugging
int  gSenal_obtenida=TABLA_SENO;
boolean gtestSignal=false;
boolean gtestHEX=false;
boolean gtestCONTINUO=true;
// 1-hex 2-numeros 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex 8-no imprime nada      
int minComando=1;
int maxComando=8;
int modo_salida=2;
int ultimo_modo=8;


#include "Arduino.h"
#define isDUE  //Detect Arduino Due
#define SERIAL_SPEED 57600  //57600
#define SERIAL_SPEED_BT 115200
#define SEPARADOR_SERIAL ','
#define FINLINEA ";"
// tenemos un modulo HC06 en serial3, y por tanto en pines pins 15 (RX) and 14 (TX)
// y prefiero el programing port (Serial), parece mas estable



// globales
int gMaxChan = 0; //maximum number of channels supported by ads1299 = 8
int gIDval = 0; //Device ID : lower 5 bits of  ID Control Register 
int gNumActiveChan = 0;
boolean gActiveChan [9]; // reports whether channels 1..9 are active
boolean isRDATAC = false;


char *gLetra; // buffer usado en to_hex, inicializado en setup()
int numSerialBytes=0;
unsigned char serialBytes[80];


// protocolo openEEG firmware P2, util para openvibe
// solo registra 6 canales

unsigned char txBuf[33];  //17 en openeeg   32 enopenbci
unsigned long indice_paquete=0;


#endif
