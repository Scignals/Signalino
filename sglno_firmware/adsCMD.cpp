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

/* adsCMD.cpp
 *  
 */
#include "Arduino.h"   // use: Wprogram.h for Arduino versions prior to 1.0
#include "adsCMD.h"
#include "ads1298.h"
#include <SPI.h>  // include the SPI library:
#include "dueCMD.h"
#include "firmware_mbs.h"

// variables modificaables  durante debugging

//int     gSenal_obtenida=TABLA_SENO;
int     gSenal_obtenida=SENAL_REAL;

boolean gtestSignal=false;
boolean gtestHEX=false;
boolean gtestCONTINUO=true;
boolean gserialVerbose=true;
boolean gBluetooth=false;
volatile int gHayLectura=0;


//donde se apunta la lectra a a vuelta de la interrupcion
//es superimportante!
// son 80 por exceso (24 deberian bastar)
volatile unsigned char serialBytes[80];
int numSerialBytes=0;

long contador_muestras=0;


// globales
int gMaxChan = 0; //maximum number of channels supported by ads1299 = 8
int gIDval = 0; //Device ID : lower 5 bits of  ID Control Register 
int gNumActiveChan = 0;
boolean gActiveChan [9]; // reports whether channels 1..9 are active
boolean isRDATAC = false;



void ads9_send_command(int cmd)
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE1));
  	digitalWrite(IPIN_CS, LOW);
  	SPI.transfer(cmd);
  	delayMicroseconds(1);
  	digitalWrite(IPIN_CS, HIGH);
  SPI.endTransaction();

}

void ads9_wreg(int reg, int val)
{
  //see pages 40,43 of datasheet - 
	SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE1));
    digitalWrite(IPIN_CS, LOW);
  	SPI.transfer(ADS1298::WREG | reg);
  	SPI.transfer(0);	// number of registers to be read/written – 1
  	SPI.transfer(val);
  	delayMicroseconds(1);
  	digitalWrite(IPIN_CS, HIGH);
  SPI.endTransaction();

}

int ads9_rreg(int reg){
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE1));
      int out = 0;
      digitalWrite(IPIN_CS, LOW);
      SPI.transfer(ADS1298::RREG | reg);
      delayMicroseconds(5);
      SPI.transfer(0);	// number of registers to be read/written – 1
      delayMicroseconds(5);
      out = SPI.transfer(0);
      	delayMicroseconds(1);
      digitalWrite(IPIN_CS, HIGH);
  SPI.endTransaction();

  return(out);
}


void ads9_misetup_ADS1299(int opciones) {
   using namespace ADS1298;
   ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
   delay(10);

   switch(opciones){
     case MODE_SENAL_TEST:
       ads9_wreg(GPIO, char(0));
       ads9_wreg(CONFIG1, HIGH_RES_250_SPS);
       ads9_wreg(CONFIG2, INT_TEST_4HZ_2X);  // generate internal test signals
       ads9_wreg(CONFIG3,char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
            ads9_wreg(char(CHnSET + i), char(TEST_SIGNAL | GAIN_1X ));
        } 
        break;
     case MODE_SENAL_REAL_1x:
       ads9_wreg(GPIO, char(0));
       ads9_wreg(CONFIG1, HIGH_RES_250_SPS);
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
               ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_1X & ~SRB2_INPUT )); //report this channel with x12 gain
        } 
        break;       
      case MODE_SENAL_REAL_12x:
       ads9_wreg(GPIO, char(0));
       ads9_wreg(CONFIG1, HIGH_RES_250_SPS);
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
               ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_12X & ~SRB2_INPUT )); //report this channel with x12 gain
        } 
        break;       

       case MODE_SENAL_SRB1:
       // set mode SRB1, util en EEG, inutil ahora xq la placa no tiene la srb1 disponible
       // gaancia a 1
       ads9_wreg(GPIO, char(0));
       ads9_wreg(PACE, char(0x20)); //set SRB1. Es un electrodo q internamente se une a todas las entradas negativas
       ads9_wreg(CONFIG1, HIGH_RES_250_SPS);
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
               ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_12X & ~SRB2_INPUT  )); //report this channel with x12 gain
        } 
        break;
       case MODE_SENAL_SRB2:
       // set mode SRB2, util en chart
       // ganancia a 1
       ads9_wreg(GPIO, char(0));
       ads9_wreg(CONFIG1, HIGH_RES_250_SPS);
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
               ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_1X | SRB2_INPUT   )); //SRB2 y ganancia 1
        } 
        break;
            
   }
  //start streaming data
    ads9_detectActiveChannels(); 
    isRDATAC = true;
    ads9_send_command(RDATAC); 
    ads9_send_command(START); 
}

void ads9_setGanancia(int valor) {
       using namespace ADS1298;
       ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
       delay(10);
       ads9_wreg(GPIO, char(0));
       ads9_wreg(CONFIG2, INT_TEST_4HZ_2X);  // generate internal test signals
       ads9_wreg(CONFIG3,char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
           switch(valor){
               case 1:  ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_1X )); break;
               case 2:  ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_2X )); break;
               case 3:  ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_4X )); break;
               case 4:  ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_6X )); break;
               case 5:  ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_8X )); break;
               case 6:  ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_12X )); break;
               case 7:  ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_24X ));break;
           }
        } 
        ads9_detectActiveChannels(); 
        isRDATAC = true;
        ads9_send_command(RDATAC); 
        ads9_send_command(START); 
}

void ads9_set_fm(int p_samplefm) {
//     p_samplefm = HIGH_RES_[16k,8k,4k,2k,1k,500,250]_SPS
       using namespace ADS1298;
       ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
       delay(10);
       ads9_wreg(GPIO, char(0));
       ads9_wreg(CONFIG1, p_samplefm);
       isRDATAC = true;
       ads9_send_command(RDATAC); 
       ads9_send_command(START); 
}

void ads9_detectActiveChannels() {  
  //actualiza gActiveChan y gNumActiveChan
  using namespace ADS1298; 
  gNumActiveChan = 0;
  for (int i = 1; i <= gMaxChan; i++) {
     delayMicroseconds(1); 
     int chSet = ads9_rreg(CHnSET + i);
     gActiveChan[i] = ((chSet & 7) != SHORTED);
     if ( (chSet & 7) != SHORTED) gNumActiveChan ++;   
  }
}

void ads9_lee_datos(void) { 
// hardware puro, 
// se utiliza por interrupciones
// lee el ads y lo pone en serialBytes[]--numSerialBytes
    int i = 0;
    numSerialBytes = 1 + (3 * gNumActiveChan); //8-bits header plus 24-bits per ACTIVE channel

      
// cs a 0, empezamos a leer el ads1299    
      SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE1));
          digitalWrite(IPIN_CS, LOW);
          contador_muestras++;
          serialBytes[i++] =SPI.transfer(0); //get 1st byte of header
          SPI.transfer(0); //skip 2nd byte of header
          SPI.transfer(0); //skip 3rd byte of header
          for (int ch = 1; ch <= gMaxChan; ch++) {
               switch(gSenal_obtenida){
                  case SENAL_REAL:
                    serialBytes[i++] = SPI.transfer(0);
                    serialBytes[i++] = SPI.transfer(0);
                    serialBytes[i++] = SPI.transfer(0);
                    break;
                  case TABLA_SENO:
                    // señal seno, creada al inicio 
                    SPI.transfer(0);
                    SPI.transfer(0);
                    SPI.transfer(0);
                    to_3bytes(samples_seno[contador_muestras%TABLE_SIZE]*100,serialBytes+i);
                    i+=3;
                  break;
            }
          }
                        
          // cs a 1, terminamos de leer el ads1299    
          delayMicroseconds(1);
          digitalWrite(IPIN_CS, HIGH);
      SPI.endTransaction();
      gHayLectura=1;
}


