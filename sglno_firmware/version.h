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


#define BOARD_DUE
//#define BOARD_TS3x2

#define HC06 Serial3  
#define WiredSerial Serial


extern const char * build_version;
extern const char * build_fecha;
extern int gMaxChan;
extern int gWired_speed;

void mensaje_inicio(void);




#endif /* VERSION_H_ */
