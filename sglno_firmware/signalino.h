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
//  Copyright © 2016 JA Barios, ILSB Technologies, Spain
//  This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
//
/* firmware_signalino.h */
#ifndef SIGNALINO_H
#define SIGNALINO_H

#include "Arduino.h"
#include "ads1298.h"
#include "adsCMD.h"
#include "util.h"

#include "firmware_mbs.h"
#include "sig_simuladas.h"
#include "version.h"
#include "serieCMD.h"



void inicia_signalino(int p_gtestCONTINUO, int p_gtestSignal);
void imprimeSerial_signalino(int p_modo_salida);



#endif

