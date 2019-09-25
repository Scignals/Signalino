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
/* sig_simuladas.h */
#ifndef SIG_SIMULADAS_H
#define SIG_SIMULADAS_H

#include "Arduino.h"

extern float samples_seno[];
extern float phaseIncrement;


#define TABLE_SIZE 48*8*4*8
#define AMPLITUD_SENAL 500
//#define TWO_PI (3.14159 * 2)

#define TABLA_SENO 3 
#define TABLA_LINEAL 4
#define TABLA_RAMPA 5


void crea_tabla_seno(void);

#endif 
