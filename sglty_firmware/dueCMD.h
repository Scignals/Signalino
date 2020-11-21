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
/* due_CMD.h */
#ifndef DUE_CMD_H
#define DUE_CMD_H
#include "Arduino.h"
#include "ads1298.h"
#include <SPI.h>  
#include "adsCMD.h"
#include "version.h"


#if defined(ARDUINO_SAM_DUE)
   #include <DueTimer.h>
#elif defined(TEENSYDUINO)
  #include <TimerOne.h>
#endif  



/*  pines utilizados  del arduino due ???
  2  -- 
  3  --
  4  -- START 
  5  -- DRDY
  6  --
  7  -- clksel
  8  -- reset
  10 -- CS
  11 -- spi out
  12 -- spi in 
  13 -- spi luz led (preasignado) 
  14 -- HC06
  15 -- HC06
*/

// Due: tenemos un modulo HC06 en serial3, y por tanto en pines pins 15 (RX) and 14 (TX)
// y prefiero el programing port (Serial), parece mas estable
// de hecho, no logro hacer funcionar el SerialUSB



/*  pines utilizados  del teensy 3.6
  2  -- RESET
  3  -- POWERDOWN
  4  -- DRDY 
  5  -- 
  6  -- START
  7  -- spi clksel SCLK
  8  -- 
  10 -- spi CS      
  11 -- spi out DOUT 
  12 -- spi in  DIN
  13 -- spi luz led (preasignado) 
  26 -- HC06
  31 -- HC06
*/


/*   segun marco
#define cs 10   
#define start1 6
#define reset1 2
#define pwdn 3
#define drdy 4
*/

/* registramos por srb2 asi que las entradas van por N  */


/* 
The Arduino Due has three additional 3.3V TTL serial ports: Serial1 on pins 19 (RX) and 18 (TX); 
Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX). 
Pins 0 and 1 are also connected to the corresponding pins of the ATmega16U2 USB-to-TTL Serial chip,
which is connected to the USB debug port. 
Additionally, there is a native USB-serial port on the SAM3X chip, SerialUSB. 
y el Serial es el programming port
*/

/* 
 * en teensy, los puertos serial 1 a 6 van por otros pines, el Serial3 son los pines 7(RX) y 8(TX), 
 * que estan usados,
 * mejor pasarlo al serial2 que son 26 y 31, lo que pasa es que estan por detras
 * el Serial es el USB, como el due
 * segun el esquema del dropbox hc06 esta en pines 9 y 10 que seria serial2 y usamos en el codigo serial3. 
 * no entiendo como funciona (serial3 son pines 7 y 8)
 */




// tenemos un modulo Bluetooth HC06 en serial3, y por tanto en pines pins 15 (RX) and 14 (TX)
// deberia cambiarse por un SMD que iria a los mismos pines
// como serial usamos (xq lo prefiero) el programing port (Serial), parece mas estable


enum CHIP_EEG {
    AMP_ADS1294,
    AMP_ADS1296,
    AMP_ADS1298,
    AMP_ADS1299,
    AMP_NONE
};

extern CHIP_EEG gChip_EEG_instalado; 


#if defined(ARDUINO_SAM_DUE)

const int PIN_START = 4;
const int IPIN_DRDY = 5;
const int IPIN_CS = 10;//10
//const int PIN_DOUT = 11;//SPI out
//const int PIN_DIN = 12;//SPI in
//const int PIN_SCLK = 13;//SPI clock
const int kPIN_LED = 13;//pin con luz led, en Teensy3, is ALSO spi clock!
const int kPIN_RESET = 8; //Reset en Pin 8
const int kPIN_CLKSEL = 7; //ClkSel en Pin 7

#define cs_low digitalWrite(IPIN_CS, LOW)
#define cs_high digitalWrite(IPIN_CS, HIGH)

#elif defined(TEENSYDUINO)

const int PIN_START = 6;
const int IPIN_DRDY = 4;
const int IPIN_CS = 10;//10
//const int PIN_DOUT = 11;//SPI out
//const int PIN_DIN = 12;//SPI in
//const int PIN_SCLK = 13;//SPI clock
const int kPIN_LED = 13;//pin con luz led, en Teensy3, is ALSO spi clock!
const int kPIN_RESET = 2; //Reset en Pin 2
const int kPIN_CLKSEL = 7; //ClkSel en Pin 7

// sacado de vteensy

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

#else
#error "Hay que definir TEENSYDUINO o ARDUINO_SAM_DUE"
#endif



void due_inicia_hw(void);
byte teensy_inicia_hw(void);
void teensy_configini(void);
void teensy_sdcard_info(void);



void parpadea(int intervalo);
byte identifica_chip_EEG(void); // no definida
void writereg(byte cant, byte numb);
void redreg(byte cant, byte numb);


#endif 
