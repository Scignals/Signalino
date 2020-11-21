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
boolean gLUX_BOTH_ON = false;

boolean gCRD_ON = false;
long luz=0;

luxometro *gLUX;
acelerometro *gACC;
sdtarjeta *gCRD;


void setup(){
 tick=0; 
 inicia_signalino(sg_estado);
 
 gLUX=new luxometro();
 gLUX->iniciar();

 gACC=new acelerometro();
 gACC->iniciar();

  // el new lo hacemos en teensy porque el due no tiene SD card
 if(gCRD) gCRD->iniciar(); 
  

}

void loop()
{

  // la lectura de datos se hace por interrupciones

File myFile;
volatile unsigned char *sbo;

if(gLUX_BOTH_ON) sbo=&(serialBytes[1]); // 25 para añadirse, 1 para solaparse
else sbo=&(serialBytes[1]);

  if(gHayLectura && gisReadingDataNow ){
         gHayLectura=0;
         tick++;
         if(gLUX_ON && gSenal_obtenida==SENAL_REAL ){
           // esto es un poco raro, pero mola
           // si gLUX_BOTH_ON hacemos que alternen acelerometro y eeg
           // asi casi no hay q tocar el codigo, solo complicamos el driver
           // if(gLUX_BOTH_ON && tick%2==1) goto toca_EEG;
            //const int sbo=1;    // si ponemos 26, y hacemos un formato nuevo, podriamos añadir canales 
            // copia en serialBytes la ultima lectura de los perifericos
            // en serialBytes se copia lo q sale del ads (1+nchannel*3): 
            // metemos la luz, de momento, en canal 1
            // loa canales, del 2 al 7. Y la temperatura en el 8
            // lux1 los pone y lux0 saca EEG
            // o char *buf_salida=serialBytes+24 permitiria añadir canales
            to_3bytes((long)(gLUX->get_ultima_luz_calibrada()), sbo); //sbo+0
            to_3bytes((long)(gACC->AcX), sbo+3); // sbo+3...
            to_3bytes((long)(gACC->AcY), sbo+6);
            to_3bytes((long)(gACC->AcZ), sbo+9);
            to_3bytes((long)(gACC->GyX), sbo+12);
            to_3bytes((long)(gACC->GyY), sbo+15);
            to_3bytes((long)(gACC->GyZ), sbo+18);
            to_3bytes((long)(gACC->temperature), sbo+21);
            gACC->leer(); //siempre leemos el acelerometro, frecuencia de muestreo como el EEG
            if(tick%(INTERVALO_LEELUZ)==0)gLUX->get_luz_calibrada();
          }         
         toca_EEG: 
         if(tick%1==0)imprimeSerial_signalino(gFormatoSerial);
        // if(gCRD_ON)imprime_linea2(1,gCRD->archivo);
         
         if(tick%(INTERVALO_LEESERIAL)==0)lee_Comando_Serial_signalino();


        if(0 && tick%(250)==0){ // en pruebas aun
          
           myFile = SD.open("test2.txt", FILE_WRITE);
  
            // if the file opened okay, write to it:
            if (myFile) {
              Serial.print("Writing to test.txt...");
              myFile.println("testing 1, 2, 3.");
            // close the file:
              myFile.close();
              Serial.println("done.");
            } else {
              // if the file didn't open, print an error:
              Serial.println("error opening test.txt");
            }
        }   
         // podria ser que se escaparan mensajes entrantes por serial, si no estamos leyendo siempre el serial
         // quiza el driver deberia esperar un oka y si no llega repetir el mensaje
             
  }

}

///////////////////////////////////////////////////////////////////////////////////////////
