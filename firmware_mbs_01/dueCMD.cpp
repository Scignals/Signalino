#include "dueCMD.h"

void due_inicia_hw() {
  using namespace ADS1298;



  pinMode(IPIN_CS, OUTPUT);
  pinMode(PIN_START, OUTPUT);
  pinMode(IPIN_DRDY, INPUT);
  pinMode(kPIN_LED, OUTPUT);
  pinMode(kPIN_RESET, OUTPUT);
  pinMode(kPIN_CLKSEL, OUTPUT);

    //al empezar, reset
   delay(800); //desde Power up, esperar 1 segundo para mover nada
   digitalWrite(kPIN_RESET, LOW);
   delay(100);
   digitalWrite(kPIN_RESET, HIGH);
   delay(260); //deberia bastar, tiempo en datasheet es 240 ms

  digitalWrite(PIN_START, LOW);
  digitalWrite(IPIN_CS, HIGH);
  digitalWrite(kPIN_CLKSEL, HIGH); // el reloj sea el interno
    
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

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

  // aqui podia ir una secuencia de leds para ver cuanto vale gmaxchan

}
