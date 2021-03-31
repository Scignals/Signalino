#include "signalino.h"
#include "serialPC.h"
#include "eegADS1299.h"


bool hayLectura {false};
#define INTERVALO_LEESERIAL 1*16
const unsigned int intervalo_leeserial {INTERVALO_LEESERIAL};

Signalino     *SG;
EEGADS1299    *ads;
SerialPC      *puertos;


void setup() {
  puertos->iniciar();
  WiredSerial.println("toygnalino 2.0");

  SG = new Signalino(); 
  SG->setFormatoSerial(formatos_eeg::DECIM);
  SG->setUsandoDriver(true);
  SG->iniciar();

  ads = new EEGADS1299();
  ads->iniciar(modos_sg::TEST);

  hayLectura=false;

}

void loop()
{
  String *linea_salida;

  unsigned long tick=0;

  if(digitalRead(pin_drdy) == LOW)/// wait for ready state of the ads
      {
        tick++;
        ads->leerDatos();
        SG->apuntarLecturaEEG(ads->getUltimaLectura());
        if (SG->usando_driver){
            WiredSerial.write(SG->getDireccionEEG(),TRAMA::longitud);
        } else {    
            linea_salida=SG->getLinea();
            WiredSerial.println(*linea_salida);
        }  
      }  

    //if(tick%intervalo_leeserial==0)
      //SG->leeComandoSerial();
 
   

}
