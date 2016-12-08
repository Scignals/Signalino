///////////////////////////////////////////////
// SIGNALINO: a bioamplifier
// firmware for controlling a ADS1299-based shield for Arduino Due
//
//
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
//  This file is part of project: SIGNALINO, a ADS1299-based bioamplifier.
//
/*
* Author: JA Barios, ILSB technologies, Spain., agosto 2016 - enero 2017
* Modified:
* Company: ILSB technologies, Spain. 
* ---------------------------------------------
* Requires ControP5 library
*/


#include <stdlib.h>     /* strtoul */
#include <SPI.h>  // include the SPI library:
#include "ads1298.h"
#include "adsCMD.h"
#include "util.h"

#include "firmware_mbs.h"
#include "sig_simuladas.h"
#include "version.h"
#include "serieCMD.h"


long contador_muestras=0;


#define INTERVALO_LEESERIAL 1*64


// variables modificaables  durante debugging
// protocolo openEEG firmware P2, util para openvibe
// solo registra 6 canales

int     gSenal_obtenida=TABLA_SENO;
boolean gtestSignal=false;
boolean gtestHEX=false;
boolean gtestCONTINUO=true;
boolean gserialVerbose=true;
boolean gBluetooth=false;
volatile int gHayLectura=0;

// 1-hex 2-numeros 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex 8-no imprime nada      
// define el modo de salida por defecto, 
// está en 2 porque lo lee el monitor del IDE de arduino
// lo paso a 6 porque no hace mas que colgarse al inicio
int modo_salida=6;



int minComando=1;
int maxComando=8;
int ultimo_modo=8;
unsigned long tick=0;



// globales
int gMaxChan = 0; //maximum number of channels supported by ads1299 = 8
int gIDval = 0; //Device ID : lower 5 bits of  ID Control Register 
int gNumActiveChan = 0;
boolean gActiveChan [9]; // reports whether channels 1..9 are active
boolean isRDATAC = false;


char *gLetra; // buffer usado en to_hex, inicializado en inicia_serial()
// velocidad de los puertos, 0 si cerrados
int gWired_speed=0;
int gBT_speed=0;

//donde se apunta lo que vuelve de la interrupcion
//es superimportante!
 volatile unsigned char serialBytes[80];
int numSerialBytes=0;


#define MAX_LEN_PACKET 33        //32+1, openbci V3
unsigned char txBuf[MAX_LEN_PACKET];  //17 en openeeg   32 en openbci V3, 8 canales+3acelerometros
unsigned long indice_paquete=0;


void setup(){
  crea_tabla_seno();
  inicia_serial_pc();
  due_inicia_hw();
 
  if(!gtestCONTINUO){
      mensaje_inicio();
      while(1); //nos quedamos colgados para terminar con un mensaje
  }
  
  if( gtestSignal )
      ads9_misetup_ADS1299(MODE_SENAL_TEST);
  else
      ads9_misetup_ADS1299(MODE_SENAL_REAL_24x);
  
}


void loop()
{
  // la lectura de datos se hace por interrupciones
  if(gHayLectura && gtestCONTINUO && isRDATAC ){
         gHayLectura=0;
         tick++;    
         if(tick%1==0){
             switch(modo_salida){
              case 1: imprime_linea(MODO_HEX);break;
              case 2: imprime_linea(MODO_DEC);break;
              case 3: imprime_openEEG_p2(1);break;
              case 4: imprime_openEEG_p2(2);break;
              case 9: imprime_openEEG_p2(3);break;
              case 5: imprime_openBCI_V3(1);break;
              case 6: imprime_openBCI_V3(2);break;
              case 7: imprime_openBCI_V3(3);break;
              case 8: imprime_linea(MODO_NADA);break;
             }
         }
         if(tick%(INTERVALO_LEESERIAL)==0)leeSerial();
  }
  
     
}




