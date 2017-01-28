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


#include <SPI.h>  // include the SPI library:
#include "signalino.h"



#define INTERVALO_LEESERIAL 1*16


// protocolo openEEG firmware P2, util para openvibe
// solo registra 6 canales
// 1-hex 2-numeros 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex 8-no imprime nada      
// define el modo de salida por defecto, 
// está en 2 porque lo lee el monitor del IDE de arduino
// lo paso a 6 porque no hace mas que colgarse al inicio
// sigue siendo muy inestable la comunicacion hacia el signalino
int modo_salida=6;



unsigned long tick=0;


void setup(){
 inicia_signalino(gtestCONTINUO,gtestSignal);
  
}


void loop()
{
  // la lectura de datos se hace por interrupciones
  if(gHayLectura && gtestCONTINUO && isRDATAC ){
         gHayLectura=0;
         tick++;    
         if(tick%1==0)imprimeSerial_signalino(modo_salida);
         if(tick%(INTERVALO_LEESERIAL)==0)leeSerial_signalino();
  }
  
     
}




