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

// #define ARDUINO_SAM_DUE
#define TEENSYDUINO
#include "signalino.h"

#define INTERVALO_LEESERIAL 1*16

SIGNALINO_maquina_estados sg_estado=SENAL_REAL_ADS;
SIGNALINO_serial_interprete sg_interprete=INTERPRETE_SIGNALINO;

unsigned long tick;


#define cs 10   //10
#define start1 6
#define reset1 2
#define pwdn 3
#define drdy 4
#define cs_low digitalWrite(cs, LOW)
#define cs_high digitalWrite(cs, HIGH)
#define start_off digitalWrite(start1, LOW)
#define start_on digitalWrite(start1, HIGH)
#define reset_off digitalWrite(reset1, LOW)
#define reset_on digitalWrite(reset1, HIGH)
#define pwdn_off digitalWrite(pwdn, LOW)
#define pwdn_on digitalWrite(pwdn, HIGH)


char data[3];
char i;
char j;
byte data2[27];
signed long out[9];




void setup(){



 tick=0; 
 gFormatoSerial=1; 
 
 while(teensy_cuenta_ch()<8);
 teensy_configini();
 
 inicia_signalino(sg_estado);

}

 
void loop()
{
  int i;
  int cont=0;
// teensy_inicia_hw();

//return;
while(0){
		if(digitalRead(drdy) == LOW)
		{

			cs_low;
			SPI.transfer(0x12);
			for (i=0;i<27;i++)
			{
				data2[i]= SPI.transfer(0x00);
			}
			cs_high;

			for (i=0;i<9;i++)
			{
				j=i*3;
				out[i]=0x00;

				out[i]=data2[(j)];

				out[i]=(out[i]<<8) | data2[(j+1)];
				out[i]=(out[i]<<8) | data2[(j+1)];
			}

			for (i=1;i<8;i++)
			{

				Serial.print(out[i]);
				Serial.print(" ");

			}

			Serial.println(out[8]);

		}
	}
  
  // la lectura de datos se hace por interrupciones
  
  if(gHayLectura && gisReadingDataNow ){
         gHayLectura=0;
         tick++;    
         if(tick%1==0)imprimeSerial_signalino(gFormatoSerial);
         if(tick%(INTERVALO_LEESERIAL)==0)leeSerial_signalino();
  }
//  if((++cont)%80==0)Serial.println('.');
//  else Serial.print(cont);
  

}

///////////////////////////////////////////////////////////////////////////////////////////
