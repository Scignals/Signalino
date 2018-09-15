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
/* due_CMD.h */
#ifndef DUE_CMD_H
#define DUE_CMD_H
#include "Arduino.h"
#include "ads1298.h"
#include <SPI.h>  // include the SPI library:
#include "adsCMD.h"
#include "version.h"



/*  pines utilizados  del arduino due
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

// no se si usamos el 13 pa a

/*  pines utilizados  del teensy 3.2
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
  26 -- HC06
  31 -- HC06
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
 * en teensy, los puertos serial 1 a 6 van por otros pines, el Serial3 son los pines 7(RX) y 8(TX), que estan usados
 * mejor pasarlo al serial2 que son 26 y 31, lo que pasa es que estan por detras
 * el Serial es el USB, como el due
 */




// tenemos un modulo HC06 en serial3, y por tanto en pines pins 15 (RX) and 14 (TX)
// como serial usamos (xq lo prefiero) el programing port (Serial), parece mas estable


#if defined(BOARD_DUE)

const int PIN_START = 4;
const int IPIN_DRDY = 5;
const int IPIN_CS = 10;//10
//const int PIN_DOUT = 11;//SPI out
//const int PIN_DIN = 12;//SPI in
//const int PIN_SCLK = 13;//SPI clock
const int kPIN_LED = 13;//pin con luz led, en Teensy3, is ALSO spi clock!
const int kPIN_RESET = 8; //Reset en Pin 8
const int kPIN_CLKSEL = 7; //ClkSel en Pin 7

#elif defined(BOARD_TS3x2)

const int PIN_START = 4;
const int IPIN_DRDY = 5;
const int IPIN_CS = 10;//10
//const int PIN_DOUT = 11;//SPI out
//const int PIN_DIN = 12;//SPI in
//const int PIN_SCLK = 13;//SPI clock
const int kPIN_LED = 13;//pin con luz led, en Teensy3, is ALSO spi clock!
const int kPIN_RESET = 8; //Reset en Pin 8
const int kPIN_CLKSEL = 7; //ClkSel en Pin 7

#endif



void due_inicia_hw();
void parpadea(int intervalo);


#endif 



