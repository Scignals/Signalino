/*
 * adsCMD.h
 * Library header file for adsCMD library
 */
#include "Arduino.h"
#include "util.h"
#include "sig_simuladas.h"
#include "ads1298.h"
#include "dueCMD.h"

#define SENAL_TEST 2 
#define SENAL_REAL 1 

#define MODE_SENAL_TEST 2 
#define MODE_SENAL_REAL_1x 1 
#define MODE_SENAL_REAL_12x 3 


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


//function prototypes
void ads9_wreg(int reg, int val); //write register
void ads9_send_command(int cmd); //send command
int  ads9_rreg(int reg); //read register
void ads9_misetup_ADS1299(int opciones); // tambien inicializa
void ads9_setGanancia(int valor); // eso
void ads9_detectActiveChannels(); // usa vars del otro lado, no se si es de aqui
void ads9_lee_datos(void);
int  ads9_getCanales(void);

