// Copyright 2019 jorge
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
/* firmware_signalino.h */
#ifndef FIRMWARE_MBS_01
#define FIRMWARE_MBS_01


#define SENAL_REAL 1 
#define SENAL_TEST 2 
#define TABLA_SENO 3 
#define TABLA_LINEAL 4
#define TABLA_RAMPA 5


// enum tipos_senales   { SENAL_REAL, SENAL_TEST, TABLA_SENO, TABLA_LINEAL, TABLA_RAMPA};
 enum MODOS_ADS1299     { MODE_SENAL_REAL_1x, MODE_SENAL_TEST, MODE_SENAL_TEST_24x, 
 MODE_SENAL_REAL_12x, MODE_SENAL_REAL_24x, MODE_SENAL_SRB1, MODE_SENAL_SRB2, MODE_SENAL_BIP,
 MODE_SENAL_PSG,  MODE_IMPEDANCIAS_ON, MODE_IMPEDANCIAS_OFF};

 
#define isDUE  //Detect Arduino Due
// tenemos un modulo HC06 en serial3, y por tanto en pines pins 15 (RX) and 14 (TX)
// y prefiero el programing port (Serial), parece mas estable
// de hecho, no logro hacer funcionar el SerialUSB




extern volatile int  gHayLectura;


extern char *gLetra; // buffer usado en to_hex, inicializado en setup()
extern long contador_muestras;
extern int  gSenal_obtenida;

extern unsigned char txBuf[];  
extern unsigned long indice_paquete;



#endif
