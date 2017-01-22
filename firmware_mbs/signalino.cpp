#include "signalino.h"

void inicia_signalino(int p_gtestCONTINUO,int p_gtestSignal){
  crea_tabla_seno();
  inicia_serial_pc();
  due_inicia_hw();
 
  if(!p_gtestCONTINUO){
      mensaje_inicio();
      while(1); //nos quedamos colgados para terminar con un mensaje
  }
  
  if( p_gtestSignal )
      ads9_misetup_ADS1299(MODE_SENAL_TEST);
  else
      ads9_misetup_ADS1299(MODE_SENAL_REAL_24x);
  
}

void imprimeSerial_signalino(int p_modo_salida){
    switch(p_modo_salida){
              case 1: imprime_linea(MODO_HEX);break;
              case 2: imprime_linea(MODO_DEC);break;
              case 3: imprime_openEEG_p2(1);break;
              case 4: imprime_openEEG_p2(2);break;
              case 9: imprime_openEEG_p2(3);break;
              case 5: imprime_openBCI_V3(1);break;
              case 6: imprime_openBCI_V3(2);break;
              case 7: imprime_openBCI_V3(3);break;
              case 8: imprime_linea(MODO_NADA);break;
    }
  
}


         
