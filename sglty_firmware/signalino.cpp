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
#include "signalino.h"

// hay tres modos: pararse, señal cuadrada, señal real




void inicia_signalino(SIGNALINO_maquina_estados p_estado){
  crea_tabla_seno();
  inicia_serial_pc();
  #if defined(ARDUINO_SAM_DUE)
    due_inicia_hw();
  #elif defined(TEENSYDUINO)
    while(teensy_cuenta_ch()<8);
    teensy_configini();  
  #endif


  switch(p_estado) {
    case QUIETO_PARADO: 
        mensaje_inicio();
        while(1); //nos quedamos "colgados" para terminar con un mensaje en pantalla
        break;
    case SENAL_CUADRADA_ADS: 
        ads9_misetup_ADS1299(MODE_SENAL_TEST);
        break;
    case SENAL_REAL_ADS:
        ads9_misetup_ADS1299(MODE_SENAL_REAL_24x);
        break;
    case MIDIENDO_IMPEDANCIAS:  //no existe aun...
        ads9_misetup_ADS1299(MODE_IMPEDANCIAS);
        break;
        
    }

}
// protocolo openEEG firmware P2, es util para openvibe pero creo que no funciona aun
// solo registra 6 canales
// 1-numeros 2-hex 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex 8-no imprime nada      
// el modo de salida por defecto, 
// está en 2 porque así lee el monitor del IDE de arduino

void imprimeSerial_signalino(int p_formato_salida){
    switch(p_formato_salida){

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


         
