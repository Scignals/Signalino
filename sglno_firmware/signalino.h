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

enum SIGNALINO_maquina_estados {
    QUIETO_PARADO,
    SENAL_CUADRADA_ADS,
    SENAL_REAL_ADS,
    MIDIENDO_IMPEDANCIAS

};


enum SIGNALINO_serial_interprete {
    INTERPRETE_SIGNALINO,
    INTERPRETE_OPENBCI   // aun no implementado
};


void inicia_signalino(SIGNALINO_maquina_estados p_estado);
void imprimeSerial_signalino(int p_modo_salida);



#endif

