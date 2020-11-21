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
// JA Barios, Marco ILSB technologies, Spain.
// And, of course, the Arduino team
//
// This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
//
#include "version.h"

const char * build_version = "0.7";
const char * build_fecha = "noviembre 18, 2020";
#if defined(ARDUINO_SAM_DUE)
const char * build_board = "Arduino Due";
#elif defined(TEENSYDUINO)
const char * build_board = "Teensy 3.6";
#endif


extern boolean gBluetooth;



void _mensaje_inicio(Stream &port_serial){
   port_serial.println("");
   port_serial.print(F("SIGNALINO "));
   port_serial.print(F("["));
   port_serial.print(build_board);
   port_serial.println("]");    
   port_serial.print(F("(c) Scignals; v "));
   port_serial.print(build_version);   
   port_serial.print(F("(build "));
   port_serial.print(build_fecha);  
   port_serial.println(")");    
   // si hay 8 canales, es q esta vivo...
   port_serial.print(F("canales: "));
   port_serial.print(gMaxChan);
   port_serial.print(F("; bps: "));
   port_serial.print(gWired_speed);
   port_serial.print(F("; format: "));
   port_serial.println(gFormatoSerial);
   
   port_serial.println(F("Comandos: (separados por punto y coma)"));
   port_serial.println(F("recN -- chorro datos EEG 0:off 1:on")); 
   port_serial.println(F("simN -- N1:1X N2:cuadrada(ADS) N3:seno(arduino) N4:12x N5:test-24x")); 
   port_serial.println(F("ganN -- ganancia del ads1299 N=1..7 --> 1,2,4,6,8,12,24x"));
   port_serial.println(F("frmN -- data format: N=0..8(6)"));
   port_serial.println(F("inpN -- input parameters: N1:ref2x12 N2:bipx4 N3:refx24 N4:PSG N5:bipolarx24"));
   port_serial.println(F("bltN -- bluetooth 1=on 0=off"));
   port_serial.println(F("luxN -- luxometro (dec format) 1=on 0=off"));
   port_serial.println(F("hlp  -- mensaje de ayuda y espera 'enter' "));
   port_serial.println(F("oka  -- mensaje de ayuda y sigue corriendo"));
   
   
    
// Comandos de control:
// gan: ganancia del amplificador
// inp: input mode (CHART, EEG, EMG)
// sim: señal de salida del signalino (real 1x, onda cuadrada del ads, señal simulada por software, real 12x )
// frm: formato de salida de datos ()
// rec: rec0 apaga salida, rec1 la restaura
// hlp: mensaje de inicio y espera un enter por wiredsignal
// oka: mensaje de inicio y sigue corriendo
// blt: bluetooth 1:on/0:off
// lux: lux 1:on/0:off, recomendado: rec0;lux1 



}
void mensaje_inicio(void){
  _mensaje_inicio(WiredSerial);
  if(gBluetooth)_mensaje_inicio(HC06);  
}
