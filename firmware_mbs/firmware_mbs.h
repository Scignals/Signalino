// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
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
//  Copyright © 2016 JABarios
//  This file is part of project: signalino
//
/* util.h */
#ifndef FIRMWARE_MBS_01
#define FIRMWARE_MBS_01

#include "Arduino.h"


#define MODE_SENAL_TEST 2 
#define MODE_SENAL_REAL_1x 1 
#define MODE_SENAL_REAL_12x 3
#define MODE_SENAL_SRB1 4
#define MODE_SENAL_SRB2 5
#define MODE_SENAL_BIP 6

 

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
// de hecho, no logro hacer funcionar el SerialUSB




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
