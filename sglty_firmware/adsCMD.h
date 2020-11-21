// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA  02110-1301, USA.
//
// This software has been inspired by the work of many people:
// Chris Rorden (original ADS1298 driver), see http://www.mccauslandcenter.sc.edu/crnl/open-source-eegecgemg 
// Adam Feuer (Author of HackEEGShield), see https://github.com/adamfeuer 
// JA Barios, ILSB technologies, Spain.
// And, of course, the Arduino team
//
// This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
//
/*
 * adsCMD.h
 * Library header file for adsCMD library
 */
#include "Arduino.h"
#include "util.h"
#include "sig_simuladas.h"
#include "ads1298.h"
#include "dueCMD.h"
#include "firmware_mbs.h"

extern int gSenal_obtenida;
extern boolean gtestHEX;
extern boolean gtestCONTINUO;
extern boolean gserialVerbose;
extern boolean gBluetooth;
extern boolean gImpedanciasActivas;

extern boolean gSerialPrinting;
extern volatile int gHayLectura;


#define SENAL_TEST 2 
#define SENAL_REAL 1 


extern volatile int numSerialBytes;
extern volatile unsigned char serialBytes[];


#define MAX_CANALES_HARDWARE 32
#define MAX_SERIALBYTES 80

extern int gMaxChan;
extern int gIDval; //Device ID : lower 5 bits of  ID Control Register 
extern int gNumActiveChan;
extern boolean gActiveChan[]; // reports whether channels 1..9 are active
extern boolean gisReadingDataNow;

// "kernel" del ads
void    ads9_wreg(int reg, int val);    //write register
int     ads9_rreg(int reg);             //read register
void    ads9_send_command(int cmd);     //send command

// control del ads
void    ads9_misetup_ADS1299(MODOS_ADS1299 estado); // modifica estado interno del ADS (impedancias,entradas,...)
void    ads9_detectActiveChannels(); // usa vars del otro lado, no se si es de aqui
void    ads9_setGanancia(int valor); // 
void    ads9_set_fm(int p_samplefm);

// interrupciones
void    ads9_lee_datos(void);  // ejecutada durante interrupcion
void    ads9_solo_datos_sin_eeg(void); // esta tambien, TIMER, llamada cuando no hay ads

