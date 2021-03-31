#include "signalino.h"


void Signalino::setFormatoSerial(formatos_eeg p){formatoSerial=p;};
void Signalino::iniciar(){
  pinMode(pin_cs, OUTPUT);
  pinMode(pin_start1, OUTPUT);
  pinMode(pin_reset1, OUTPUT);
  pinMode(pin_pwdn, OUTPUT);
  pinMode(pin_drdy, INPUT);
  pinMode(pin_red, OUTPUT);
  pinMode(pin_green, OUTPUT);
  pinMode(pin_blue, OUTPUT);
  pinMode(pin_key, OUTPUT);
  pinMode(pin_blkey, OUTPUT);
  delay(200); //desde Power up, esperar 0.2 segundos para mover nada
  bluetooth_on;
  ledred_on;
  ledgreen_off;
  ledblue_off;
  pwdn_on;
  start_on;
  cs_high;

  for(int i=0;i<80;i++)ultimaLecturaEEG[i]=0;
};


void Signalino::leeComandoSerial(){};

void Signalino::apuntarLecturaEEG(volatile unsigned char *bytes)
{
    ultimaLecturaEEG[0]=TRAMA::EEG_header;
    ++ultimaLecturaEEG[1];
    for(int i=0;i<num_bytes;i++)ultimaLecturaEEG[2+i]=*(bytes+i+1);
    ultimaLecturaEEG[32]=TRAMA::EEG_footer;
};



String *Signalino::getLinea()
{
  buffer_linea="";  
  imprimirLecturaEEG(getDireccionEEG(),formatoSerial); 
  return(&buffer_linea);
};


void Signalino::imprimirLecturaEEG( unsigned char *bytes, formatos_eeg formato){
    char letras[5];
    long numero=0;
    for (int i = 0, ii = 2; i < num_canales; i++, ii+=3)
    {
      numero=toInt32(bytes+ii);
      switch(formato){
        case formatos_eeg::HEXAD:
          sprintf(letras, "%03lX ",numero);
       	  buffer_linea += letras;
          break;
        case formatos_eeg::DECIM:
          sprintf(letras, "%ld",numero);
          buffer_linea += letras;
          if(i<(num_canales-1)) buffer_linea  += ',';
          break;
      }
    }
};

long Signalino::toInt32(  volatile unsigned char* byteArray) {  
// long desde 3 bytes leidos en el ads1299   
 long newInt = (  
     ((0xFF & byteArray[0]) << 16) |  
     ((0xFF & byteArray[1]) << 8) |   
     (0xFF & byteArray[2])  
   );   
 if ((newInt & 0x00800000) > 0) {  
   newInt |= 0xFF000000;  
 } else {  
   newInt &= 0x00FFFFFF;  
 }  
return newInt;  
}  
