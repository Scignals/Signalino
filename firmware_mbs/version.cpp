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
#include "version.h"

const char * build_version = "0.4";
const char * build_fecha = "ago 4, 2017";

#if defined(BOARD_DUE)
const char * build_board = "Arduino Due";
#elif defined(BOARD_TS3x2)
const char * build_board = "Teensy 3.2";
#endif



void mensaje_inicio(){
   WiredSerial.println("");
   WiredSerial.print(F("SIGNALINO v "));
   WiredSerial.println(build_version);   
   WiredSerial.print(F("build "));
   WiredSerial.println(build_fecha);  
   WiredSerial.print(F("board "));
   WiredSerial.println(build_board);
   WiredSerial.println(F("(c) ILSB Technologies"));
  
   // si hay 8 canales, es q esta vivo...
   WiredSerial.print(F("canales activos: "));
   WiredSerial.println(gMaxChan);
   WiredSerial.print(F("speed com: "));
   WiredSerial.println(gWired_speed);
   WiredSerial.println(F("Comandos: (separados por punto y coma)"));
   WiredSerial.println(F("hlp  -- ayuda "));
   WiredSerial.println(F("rec  -- chorro datos on/off")); 
   WiredSerial.println(F("sim  -- señal simulada on/off"));    
   WiredSerial.println(F("simN -- N1, señal normal. N2, test. N3, simulada")); 
   WiredSerial.println(F("ganN -- ganancia del ads1299 N=1..8 --> 1,2,4,4,6,8,12,24x"));
   WiredSerial.println(F("frmN -- formatos de salida: N=0,va cambiando. Ahora hay 8. Por defecto, 6"));
   WiredSerial.println(F("inpN -- parametros de adquisición: N1:ref2x1 N2:bipx24 N3:refx24"));
   WiredSerial.println(F("bltN -- bluetooth 1=on 2=off"));
   
    
   
}

