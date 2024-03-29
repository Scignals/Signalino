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

// hay CINCO estados: QUIETO_PARADO, señal cuadrada, IMPEDANCIAS, señal real


const SIGNALINO_formatos_salida formatos[]={
            RHEX, RDEC, OPENeeg_1, OPENeeg_2, OPENeeg_3,
              OPENbci_1, OPENbci_2, OPENbci_3,
               ROFF };



void inicia_signalino(SIGNALINO_maquina_estados p_estado){
  crea_tabla_seno();
  inicia_serial_pc();
  #if defined(ARDUINO_SAM_DUE)
    due_inicia_hw();
  #elif defined(TEENSYDUINO)
    teensy_inicia_hw();
  //  teensy_configini();
    teensy_sdcard_info();  
  #endif
  inicia_serial_pc();
  
 gFormatoSerial       = 2; // default para usarlo con arduino gui 
 gBluetooth           = true;
 gSerialPrinting      = true;
 gLUX_ON              = false;
 gLUX_BOTH_ON         = false;
 gImpedanciasActivas  = false;
 
// gFormatoSerial_code = OUT_DEC;


  switch(p_estado) {
    case QUIETO_PARADO: 
        mensaje_inicio();
        while(1); //nos quedamos "colgados" para terminar con un mensaje en pantalla
        break;
    case SENAL_CUADRADA_ADS: 
        ads9_misetup_ADS1299(MODE_SENAL_TEST);
        break;
    case SENAL_REAL_ADS:
        ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
        break;
    case IMPEDANCIAS:
        ads9_misetup_ADS1299(MODE_IMPEDANCIAS_ON);
        break;
    }

}
// Protocolos de salida
// solo es util openBCI_2 (binario), usado por el driver. Los otros son dec y hex para debug 
// es un formato parecido pero no igual 
// el modo de salida por defecto está en 2, 
// porque así lee el serial monitor del IDE de arduino
// openEEG firmware P2, podria ser util para openvibe pero creo que no funciona aun
// solo registra 6 canales
// 1-numeros 2-hex 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex 8-no imprime nada      

void imprimeSerial_signalino(int p_formato_salida){
    switch(formatos[p_formato_salida]){
              case RHEX:      imprime_linea(MODO_HEX);break;
              case RDEC:      imprime_linea(MODO_DEC);break;
              case ROFF:      imprime_linea(MODO_NADA);break;
              case OPENeeg_1: imprime_openEEG_p2(1);break;
              case OPENeeg_2: imprime_openEEG_p2(2);break;
              case OPENeeg_3: imprime_openEEG_p2(3);break;
              case OPENbci_2: imprime_openBCI_V3(2);break; //default
              case OPENbci_1: imprime_openBCI_V3(1);break;
              case OPENbci_3: imprime_openBCI_V3(3);break; 
    }
  
}


         
