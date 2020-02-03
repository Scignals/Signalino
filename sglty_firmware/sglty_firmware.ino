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
// This software has been inspired by the work of many people:
// Chris Rorden (original ADS1298 driver), see http://www.mccauslandcenter.sc.edu/crnl/open-source-eegecgemg 
// Adam Feuer (Author of HackEEGShield), see https://github.com/adamfeuer 
// People of the Open-BCI project
// JA Barios, ILSB technologies, Spain.
// And, of course, the Arduino team
//
// This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
//
/*
* Author: JA Barios, ILSB technologies, Spain., agosto 2016 - enero 2017 - ...
* Modified: 
* Company: ILSB technologies, Spain. 
* 
* Dependencies: DueTimer library
*
* ---------------------------------------------
*/

//#define ARDUINO_SAM_DUE
#define TEENSYDUINO

#include "signalino.h"

#define INTERVALO_LEESERIAL 1*16
#define INTERVALO_LEELUZ 1*16


SIGNALINO_maquina_estados sg_estado=SENAL_REAL_ADS;
SIGNALINO_serial_interprete sg_interprete=INTERPRETE_SIGNALINO;

unsigned long tick;
boolean gSerialPrinting = true;
boolean gLUX_ON = false;
long luz=0;

luxometro *gLUX;
acelerometro *gACC;



void setup(){
 tick=0; 
 inicia_signalino(sg_estado);
 
 gLUX=new luxometro();
 gLUX->iniciar();

 gACC=new acelerometro();
 gACC->iniciar();

}

void loop()
{

  // la lectura de datos se hace por interrupciones
  
  if(gHayLectura && gisReadingDataNow ){
         gHayLectura=0;
         tick++;
         if(gLUX_ON){    
            // copia en serialBytes la ultima lectura de los perifericos
            // en serialBytes se copia lo q sale del ads (1+nchannel*3): 
            // metemos la luz, de momento, en canal 1
            // loa canales, del 2 al 7. Y la temperatura en el 8
            // lux1 los pone y lux0 saca EEG
            to_3bytes((long)(10*gLUX->get_ultima_luz_calibrada()), &(serialBytes[1]));
            to_3bytes((long)(100*gACC->AcX), &(serialBytes[4]));
            to_3bytes((long)(100*gACC->AcY), &(serialBytes[7]));
            to_3bytes((long)(100*gACC->AcZ), &(serialBytes[10]));
            to_3bytes((long)(10*gACC->GyX), &(serialBytes[13]));
            to_3bytes((long)(10*gACC->GyY), &(serialBytes[16]));
            to_3bytes((long)(10*gACC->GyZ), &(serialBytes[19]));
            to_3bytes((long)(10*gACC->temperature), &(serialBytes[22]));
            gACC->leer(); //siempre leemos
            if(tick%(INTERVALO_LEELUZ)==0)gLUX->get_luz_calibrada();
          }         
         if(tick%1==0)imprimeSerial_signalino(gFormatoSerial);
         if(tick%(INTERVALO_LEESERIAL)==0)leeSerial_signalino();
             
  }

}

///////////////////////////////////////////////////////////////////////////////////////////
