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

#include "Arduino.h"
#define isDUE  //Detect Arduino Due
#define SERIAL_SPEED 115200  //57600
#define SERIAL_SPEED_BT 115200
#define SEPARADOR_SERIAL ','
#define FINLINEA ";"
// tenemos un modulo HC06 en serial3, y por tanto en pines pins 15 (RX) and 14 (TX)
// y prefiero el programing port (Serial), parece mas estable




extern int gMaxChan;
extern int gIDval; //Device ID : lower 5 bits of  ID Control Register 
extern int gNumActiveChan;
extern boolean gActiveChan[]; // reports whether channels 1..9 are active
extern boolean isRDATAC;


extern char *gLetra; // buffer usado en to_hex, inicializado en setup()
extern int numSerialBytes;
extern unsigned char serialBytes[];
extern long contador_muestras;
extern int  gSenal_obtenida;

extern unsigned char txBuf[];  
extern unsigned long indice_paquete;



#endif
