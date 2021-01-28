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
// JA Barios, Marco , ILSB technologies, Spain.
// And, of course, the Arduino team
//
// This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
//

// Rutinas dependientes de board (teensy, arduino Due)

#include "dueCMD.h"
#include "firmware_mbs.h"
#include "version.h"
#include "sdtarjeta.h"

#if defined(TEENSYDUINO)

//byte cmd_read[23];

byte data1;
byte teensy_cuenta_canales_EEG(void);

byte teensy_inicia_hw() {
  // inicia teensy, reconoce chip, saca gmaxchan,parpadea, activa interrupcion

  // ** inicia teensy
  using namespace ADS1298;
 
  pinMode(cs, OUTPUT);
  pinMode(start1, OUTPUT);
  pinMode(reset1, OUTPUT);
  pinMode(pwdn, OUTPUT);
  pinMode(drdy, INPUT);
// falta un misterioso pin 9 para bluetooth
// nuevos pines en teensyII
  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);
  pinMode(pin_key, OUTPUT);
  pinMode(pin_blkey, OUTPUT);


  
  delay(800); //desde Power up, esperar 1 segundo para mover nada

//para teensyII
  bluetooth_on;
  digitalWrite(pin_tx, HIGH);
  digitalWrite(pin_rx, HIGH);
  pinMode(pin_tx, OUTPUT);
  pinMode(pin_rx, INPUT);

  ledred_on;
  ledgreen_off;
  ledblue_off;
  

  pwdn_on;
  start_on;
  cs_high;
  SPI.begin();

  //al empezar, reset del ADS1299
//  pwdn_off;delay(10); pwdn_on;delay(300);
  reset_off;delay(100);reset_on;delay(260);

  // cuenta canales a gMaxChan y activa interrupcion
  if(teensy_cuenta_canales_EEG()>0){
    // si añado la rutina parpadea, se para el programa. que cosas...
    attachInterrupt(digitalPinToInterrupt(drdy), ads9_lee_datos, FALLING);
    delay(100);
    ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
  } else {
      gMaxChan = 8;
      gChip_EEG_instalado=AMP_NONE;
      gSenal_obtenida=TABLA_SENO;
      Serial.println("No hay ADS1299!!!");
      // y activamos la interrupcion "dummy"
      // que, de momento al menos, no funciona a nivel hardware
      // asi que la llamamos como un timer
          Timer1.initialize(4000);
          Timer1.attachInterrupt(ads9_solo_datos_sin_eeg); 
  }

  return gMaxChan;  

}



void teensy_sdcard_info(void)
{
  gCRD=new sdtarjeta();
  if(!gCRD->info()) return;
  gCRD=nullptr;


}  


byte teensy_cuenta_canales_EEG()
{
//  Serial.print("Counting device channels...");
  byte revid;
  byte ch;
  byte dev_id;
  byte num_ch;

  // identifica el chip
  // calcula el numero de canales
  // ** no se que hace, tal vez identificar 
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
    cs_low;
    delayMicroseconds(1000);
    SPI.transfer(0x11);
    delayMicroseconds(2);
    SPI.transfer(0x20);
    SPI.transfer(0x00);
    data1=SPI.transfer(0x00);
    cs_high;
  SPI.endTransaction();
  
  ch=((data1>>4) & 1);
  revid=((data1>>5) & 3); 
  dev_id=((data1>>2) & 3);
  num_ch=(data1 & 3);
  num_ch=(num_ch*2)+4;
  if(ch==1)
  {
    if(dev_id==3)
    {
      Serial.println(num_ch);
      Serial.print("Device ID: ");
      Serial.println("ADS1299-X");
      gMaxChan = (int) num_ch; //ads1299
      gChip_EEG_instalado=AMP_ADS1299;
      Serial.print("Numero de canales: ");
    }
    else
    {
      Serial.println();
      Serial.print("Device ID: ");
      Serial.println("unknown"); 
      Serial.print("Datos obtenidos: ");
      Serial.println(data1,BIN);
    }

  }
  else
  {
    Serial.println();
    Serial.println("Error de conexión con el dispositivo. ");
    Serial.print("Datos obtenidos: ");
    Serial.println(data1,BIN);
  }
  return num_ch;
}

#endif

void due_inicia_hw() {
  // inicia due, reconoce chip, saca gmaxchan,parpadea, activa interrupcion
  using namespace ADS1298;

  //al empezar, reset del ADS1299
  delay(800); //desde Power up, esperar 1 segundo para mover nada
  digitalWrite(kPIN_RESET, LOW);
  delay(100);
  digitalWrite(kPIN_RESET, HIGH);
  delay(260); //deberia bastar, tiempo en datasheet es 240 ms

  // ajustamos los pines
  pinMode(IPIN_CS, OUTPUT);
  pinMode(PIN_START, OUTPUT);
  pinMode(IPIN_DRDY, INPUT);
  pinMode(kPIN_LED, OUTPUT);
  pinMode(kPIN_RESET, OUTPUT);
  pinMode(kPIN_CLKSEL, OUTPUT);


  digitalWrite(PIN_START, LOW);
  digitalWrite(IPIN_CS, HIGH);
  digitalWrite(kPIN_CLKSEL, HIGH); // usa el reloj  interno

  // ojo, imprescindible o se para el prog:
  // arrancamos interfaz SPI
  SPI.begin(); 

  // identificamos chip y calculamos numero maximo de canales
  ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
  delay(10);
  // Determine model number and number of channels available
  gIDval = ads9_rreg(ID); //lower 5 bits of register 0 reveal chip type
  switch (gIDval & B00011111 ) { //least significant bits reports channels
    case  B10000: //16
      gMaxChan = 4; //ads1294
      gChip_EEG_instalado=AMP_ADS1294;
      break;
    case B10001: //17
      gMaxChan = 6; //ads1296
      gChip_EEG_instalado=AMP_ADS1296;
      break;
    case B10010: //18
      gMaxChan = 8; //ads1298
      gChip_EEG_instalado=AMP_ADS1298;
      break;
    case B11110: //30
      gMaxChan = 8; //ads1299
      gChip_EEG_instalado=AMP_ADS1299;
      break;
    default:
      // no hemos reconocido el hardware (o no esta...)
      // asi que implementamos una version simulada
      gMaxChan = 8;
      gChip_EEG_instalado=AMP_NONE;
      gSenal_obtenida=TABLA_SENO;
      for (int i = 0; i < 3*gMaxChan; i++) {
          parpadea(100 );
      }    
      Serial.println("No hay ADS1299!!!");
      // y activamos la interrupcion "dummy"
      // que, de momento al menos, no funciona a nivel hardware
      // asi que la llamamos como un timer

      #if defined(ARDUINO_SAM_DUE)
          Timer3.attachInterrupt(ads9_solo_datos_sin_eeg).start(4000);
      #elif defined(TEENSYDUINO)
          Timer1.initialize(4000);
          Timer1.attachInterrupt(ads9_solo_datos_sin_eeg); 
      #endif

      return;
  }

  
  // parpadeo del led 13 q avisa si hemos leido los canales adecuados
  for (int i = 0; i < gMaxChan; i++) {
    parpadea(1000 / (1 + gMaxChan));
  }
  // y por fin activamos la interrupcion
  attachInterrupt(digitalPinToInterrupt(IPIN_DRDY), ads9_lee_datos, FALLING);


  Serial.println((char*)sprintf("%s transmiting",build_board));
}

void parpadea(int intervalo)
{
#if defined(TEENSYDUINO)
  // SCK pin goes to 14 in teensy, play with led, and then back
  //https://forum.pjrc.com/threads/25727-an-FYI-on-remapping-the-Teensy-3-LED-and-using-SPI
  // be careful, not sure if more complex things are needed here
  SPI.setSCK(14);
#endif
  const int ledPin = 13;
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(intervalo / 2);                // wait for INTERVALO
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(intervalo / 2);                // wait for INTERVALO
#if defined(TEENSYDUINO)
  SPI.setSCK(13);
#endif

}
