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
#ifndef SERIE_CMD_H
#define SERIE_CMD_H

#include "Arduino.h"
#include "util.h"
#include "sig_simuladas.h"
#include "firmware_mbs.h"
#include "adsCMD.h"
#include "version.h"



#define SERIAL_SPEED 115200  //57600 460800
#define SERIAL_SPEED_BT 115200
#define SEPARADOR_SERIAL ','
#define FINLINEA ";"
#define MODO_HEX 1
#define MODO_DEC 2
#define MODO_NADA 0
#define MAX_COMENTARIO_SERIAL 80


extern int gWired_speed;
extern int gBT_speed;
extern char *gLetra; // buffer usado en to_hex, inicializado en inicia_serial()
extern boolean gserialVerbose;
extern volatile int numSerialBytes;
extern volatile  unsigned char serialBytes[];
extern boolean gBluetooth;
extern unsigned char txBuf[];  //17 en openeeg   32 enopenbci
extern unsigned long indice_paquete;
extern int gSenal_obtenida;
extern int gFormatoSerial;
extern int minComando;
extern int maxComando;
extern int ultimo_modo;
extern char buffer_comentaserial[];



void inicia_serial_pc();
void leeSerial_signalino();
void comentaSerial(String texto);
void imprime_linea2( int modo_hex_dec_nada, Stream &port_serial );
void imprime_linea( int modo_hex_dec_nada);
void imprime_openEEG_p2(int modo);
void imprime_openBCI_V3(int modo_bci_protocolo);
void procesaComando(String texto);
void procesaComando_estilo_openBCI(String texto); // no implementado
int extrae_parametro(String texto);



#endif

