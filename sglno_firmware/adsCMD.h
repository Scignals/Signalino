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
//
//  Copyright © 2016 JA Barios, ILSB technologies, Spain.
//  This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
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

extern int gSenal_obtenida;
extern boolean gtestSignal;
extern boolean gtestHEX;
extern boolean gtestCONTINUO;
extern boolean gserialVerbose;
extern boolean gBluetooth;
extern volatile int gHayLectura;

#define SENAL_TEST 2 
#define SENAL_REAL 1 

#define MODE_SENAL_TEST 2 
#define MODE_SENAL_REAL_1x 1 
#define MODE_SENAL_REAL_12x 3
#define MODE_SENAL_SRB1 4
#define MODE_SENAL_SRB2 5
#define MODE_SENAL_BIP 6

extern volatile int numSerialBytes;
extern volatile unsigned char serialBytes[];

extern int gMaxChan;
extern int gIDval; //Device ID : lower 5 bits of  ID Control Register 
extern int gNumActiveChan;
extern boolean gActiveChan[]; // reports whether channels 1..9 are active
extern boolean isRDATAC;

//function prototypes
void ads9_wreg(int reg, int val); //write register
void ads9_send_command(int cmd); //send command
int ads9_rreg(int reg); //read register
void ads9_misetup_ADS1299(int opciones); // tambien inicializa
void ads9_setGanancia(int valor); // eso
void ads9_detectActiveChannels(); // usa vars del otro lado, no se si es de aqui
void ads9_lee_datos(void);  // esta va como interrupcion
int ads9_getCanales(void);

