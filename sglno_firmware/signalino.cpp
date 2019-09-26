#include "signalino.h"


void inicia_signalino(int p_gtestCONTINUO,int p_gtestSignal){
  crea_tabla_seno();
  inicia_serial_pc();
  due_inicia_hw();
 
  if(!p_gtestCONTINUO){
      mensaje_inicio();
      while(1); //nos quedamos "colgados" para terminar con un mensaje en pantalla
  }
  
  if( p_gtestSignal )
      ads9_misetup_ADS1299(MODE_SENAL_TEST);
  else
      ads9_misetup_ADS1299(MODE_SENAL_REAL_24x);
  
}


// protocolo openEEG firmware P2, es util para openvibe pero creo que no funciona aun
// solo registra 6 canales
// 1-numeros 2-hex 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex 8-no imprime nada      
// el modo de salida por defecto, 
// está en 2 porque así lee el monitor del IDE de arduino

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


         
