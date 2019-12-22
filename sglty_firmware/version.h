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
//  This file is part of projec: SIGNALINO, a ADS1299-based bioamplifier
//
/*version.h*/
#ifndef VERSION_H_
#define VERSION_H_

#include "Arduino.h"
#include "serieCMD.h"


#if defined(TEENSYDUINO)
#define HC06 Serial3  
#define WiredSerial Serial   // funciona tb con serialUSB ¿en due?
#elif defined(ARDUINO_SAM_DUE)
#define HC06 Serial3  
#define WiredSerial Serial   // funciona tb con serialUSB ¿en due?
#endif

extern const char * build_board;
extern const char * build_version;
extern const char * build_fecha;
extern int gMaxChan;
extern int gWired_speed;
extern int gFormatoSerial;



void mensaje_inicio(void);





#endif /* VERSION_H_ */
