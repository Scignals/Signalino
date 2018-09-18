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
#include "dueCMD.h"
#include "firmware_mbs.h"

void due_inicia_hw() {
  using namespace ADS1298;


  //al empezar, reset
  // en charmander, el orden de este trozo es critico. Primero reset, y luego set pin mode. Y ni idea de porque. En la anterior, daba igual.

  delay(800); //desde Power up, esperar 1 segundo para mover nada
  digitalWrite(kPIN_RESET, LOW);
  delay(100);
  digitalWrite(kPIN_RESET, HIGH);
  delay(260); //deberia bastar, tiempo en datasheet es 240 ms



  pinMode(IPIN_CS, OUTPUT);
  pinMode(PIN_START, OUTPUT);
  pinMode(IPIN_DRDY, INPUT);
  pinMode(kPIN_LED, OUTPUT);
  pinMode(kPIN_RESET, OUTPUT);
  pinMode(kPIN_CLKSEL, OUTPUT);


  digitalWrite(PIN_START, LOW);
  digitalWrite(IPIN_CS, HIGH);
  digitalWrite(kPIN_CLKSEL, HIGH); // el reloj sea el interno

  SPI.begin(); //ojo, es imprescindible o se para el prog
  ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
  delay(10);

  // Determine model number and number of channels available
  gIDval = ads9_rreg(ID); //lower 5 bits of register 0 reveal chip type
  switch (gIDval & B00011111 ) { //least significant bits reports channels
    case  B10000: //16
      gMaxChan = 4; //ads1294
      break;
    case B10001: //17
      gMaxChan = 6; //ads1296
      break;
    case B10010: //18
      gMaxChan = 8; //ads1298
      break;
    case B11110: //30
      gMaxChan = 8; //ads1299
      break;
    default:
      gMaxChan = 0;
  }
  // parpadeo del led 13 q avisa si hemos leido los canales adecuados
  // esto segun esta no funciona en teensy, por lo del pin 13 que lleva el reloj SPI
  // creo q habria q usar la rutina parpadea, es lo q falta aqui
  for (int i = 0; i < gMaxChan; i++) {
    digitalWrite(kPIN_LED, HIGH); // el reloj sea el interno
    delay(400 / (1 + gMaxChan));
    digitalWrite(kPIN_LED, LOW); // el reloj sea el interno
    delay(400 / (1 + gMaxChan));
  }
  // y por fin nos preparamos para leer
  attachInterrupt(digitalPinToInterrupt(IPIN_DRDY), ads9_lee_datos, FALLING);
#if defined(BOARD_DUE)
  Serial.println("Due transmiting");
#elif defined(BOARD_TS3x2)
  Serial.println("Teensy transmiting");
#endif
}

void parpadea(int intervalo)
{
#if defined(BOARD_TS3x2)
  // SCK pin goes to 14 in teensy, play with led, and then back
  //https://forum.pjrc.com/threads/25727-an-FYI-on-remapping-the-Teensy-3-LED-and-using-SPI
  // be careful, not sure if more complex things are needed here
  SPI.setSCK(14);
#endif
  const int ledPin = 13;
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(intervalo / 2);                // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(intervalo / 2);                // wait for a second
#if defined(BOARD_TS3x2)
  SPI.setSCK(13);
#endif

}

