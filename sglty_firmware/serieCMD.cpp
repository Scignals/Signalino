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
#include "serieCMD.h"
#include "command_parser.h"


#define MAX_LEN_PACKET 33        //32+1, openbci V3
unsigned char txBuf[MAX_LEN_PACKET];  //17 en openeeg   32 en openbci V3, 8 canales+3acelerometros
unsigned long indice_paquete=0;

// velocidad de los puertos, 0 si cerrados
int gWired_speed=0;
int gBT_speed=0;

char buffer_comentaserial[MAX_COMENTARIO_SERIAL];
char *gLetra; // buffer usado en to_hex, inicializado en inicia_serial()
int gFormatoSerial; 

// intervalo variable cuando se esta leyendo el puerto serie para modificar su velocidad de respuesta 
unsigned int intervalo_leeserial=1;
elapsedMillis ultimo_acceso_serie=0;

int minComando=1;
int maxComando=8;
int ultimo_modo=8;

int canales_extra=0;


void inicia_serial_pc(){
  gLetra=new char[80];  
  WiredSerial.begin(SERIAL_SPEED);
  while (WiredSerial.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
  gWired_speed=SERIAL_SPEED;
  
  HC06.begin(SERIAL_SPEED_BT); 
  while (HC06.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
  gBT_speed=SERIAL_SPEED_BT;
 
}

void comentaSerial(String texto)
{
   if(gserialVerbose){
     WiredSerial.print('#');
     WiredSerial.println(texto);
  }
}


/* PROTOCOLOS POR PUERTO SERIE   */
//--------------------------------
// cambio a Stream y referencia porque ahora funciona bien con los dos tipos de puertos
// (serie y bluetooth)
// leido en http://forum.arduino.cc/index.php?topic=155961.0

void imprime_linea2( int modo_hex, Stream &port_serial ){
   for (int i = 1; i < numSerialBytes; i+=3)
   {
    long numero = to_Int32(serialBytes+i);
    switch(modo_hex) {
    case 0:
    	break;
    case 2:
    	port_serial.print(numero,HEX);
    	break;
    default:
                  port_serial.print(numero);
                  if(i<numSerialBytes-3){
                      port_serial.print(SEPARADOR_SERIAL );
                  }
            }      
   }
   //añadimos un ; al final 
   port_serial.println(FINLINEA);    
}


// protocolo simple, solo numeros
void imprime_linea( int modo_hex){
  if(modo_hex){
     imprime_linea2( modo_hex, WiredSerial );
     if(gBluetooth) imprime_linea2( modo_hex, HC06 ); //ponia modo_hex
  }
}



//protocolo openEEG, no funciona aun bien
void imprime_openEEG_p2(int modo_openeeg_protocolo){
// modo 1; numeros. Modo 2: bytes . Modo 3; hexadecimal
   int ind;
   static int count = -1;

   ind=0;
   txBuf[ind++]=0xA5;
   txBuf[ind++]=0x5A;
   txBuf[ind++]=2;
   txBuf[ind++]=indice_paquete++;
      
   for (int i = 1; i < numSerialBytes-6; i+=3)
   {
     long numero = to_Int32(serialBytes+i) ;
     numero >>= 13; 
     //LSB - HSB
     txBuf[ind++] = (byte) (numero & 0x000000FF)  ;     
     txBuf[ind++] = (byte) ((numero & 0x0000FF00) >> 8)  ;
   }
   byte switches = 0x07;
   count++; if (count >= 18) count=0;
   if (count >= 9) {
      switches = 0x0F;
   } 
   txBuf[ind]=switches; 
   ind=0;
   if(modo_openeeg_protocolo==1){    
     for(int m=0;m<4;m++){
        WiredSerial.print(txBuf[m]);
        if(gBluetooth)HC06.print(txBuf[m]);
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<6;m++){
        int vv=((int)txBuf[ind++] << 8);
        vv += (int)txBuf[ind++] ;
        WiredSerial.print(vv );
        if(gBluetooth)HC06.print(vv );
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
     }
     WiredSerial.print(txBuf[ind]  );
     if(gBluetooth)HC06.print(txBuf[ind]  );
//añadimos un ; al final 
    WiredSerial.println(FINLINEA);
    if(gBluetooth)HC06.println(FINLINEA);

   } else {
        if(modo_openeeg_protocolo==2) {
               WiredSerial.write(txBuf,17);
               
   } else {
   // protocolo open_eeg , pero en hexadecimal, a implementar, ahora sale en dec
     for(int m=0;m<4;m++){
        WiredSerial.print(txBuf[m]);
        if(gBluetooth)HC06.print(txBuf[m]);
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<6;m++){
        int vv=((int)txBuf[ind++] << 8);
        vv += (int)txBuf[ind++] ;
        WiredSerial.print(vv );
        if(gBluetooth)HC06.print(vv );
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
     }
     WiredSerial.print(txBuf[ind]  );
     if(gBluetooth)HC06.print(txBuf[ind]  );
   //añadimos un ; al final 
    WiredSerial.println(FINLINEA);
    if(gBluetooth)HC06.println(FINLINEA);

   }
 }
}

// protocolo openbci V3, el que usamos por defecto, modo 2;
void imprime_openBCI_V3(int modo_bci_protocolo){
// protocolo interesante, descrito en 
// https://github.com/OpenBCI/OpenBCI-V2hardware-DEPRECATED/wiki/Data-Format-for-OpenBCI-V3 
// el ultimo byte es 0xCX, y según X se reinterpretan los últimos 6 bytes (acelerometro, fecha, o user defined)
// modo 1; numeros. Modo 2: bytes . Modo 3; hexadecimal. 
  
   int ind;
   byte timestamp[3];
   byte luz_percibida[3];

   if(gLUX_BOTH_ON)canales_extra=0;
   else canales_extra=0;
   
   txBuf[0]=0xA0;
   txBuf[1]=indice_paquete++;
   
   ind=2;


   for (int i = 1; i < numSerialBytes+canales_extra; i+=3)
   {
     txBuf[ind++] = serialBytes[i];
     txBuf[ind++] = serialBytes[i+1];
     txBuf[ind++] = serialBytes[i+2];
   }
   
   //los 2 acelerometros, no los tenemos asi que lo dejamos a cero
   //pero metemos un timestamp en el primero
   //y el luxometro en el segundo
   to_3bytes(micros(),timestamp);
   to_3bytes((long)gLUX->get_ultima_luz_calibrada(),luz_percibida);
   txBuf[ind++] = timestamp[0];
   txBuf[ind++] = timestamp[1];
   txBuf[ind++] = timestamp[2];
   txBuf[ind++] = luz_percibida[0];
   txBuf[ind++] = luz_percibida[1];
   txBuf[ind++] = luz_percibida[2];
   
   // este ultimo es C0 para EEG y acelerometros
   // ahora es C1 para lux y acelerometros
   // en su lugar podian ir por ejemplo un registro de  booleanos: luz, bluetooh si/no,
   // pero entonces hay q cambiar el C0 por CX
   txBuf[ind]=footer; 

   ind=0;
   if(modo_bci_protocolo==1){ 
    // protocolo open_bci V3, pero con mumeros ascii   
     for(int m=0;m<2;m++){
        WiredSerial.print(txBuf[ind]);
        if(gBluetooth)HC06.print(txBuf[ind]);
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<10+canales_extra;m++){
        long vv = to_Int32(txBuf+ind);
        WiredSerial.print(vv );
        if(gBluetooth)HC06.print(vv );
        if(m<9+canales_extra){
           WiredSerial.print(SEPARADOR_SERIAL );
           if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        }
        ind+=3;
     }

    //añadimos un ; al final 
    WiredSerial.println(FINLINEA);
    if(gBluetooth)HC06.println(FINLINEA);

  } else {
    if(modo_bci_protocolo==2){
    // protocolo open_bci V3, pero con bytes (es el que se usa en el visor_pc)
        WiredSerial.write(txBuf,33+canales_extra*3);
        if(gBluetooth)HC06.write(txBuf,33+canales_extra*3);
        
//        if(gBluetooth)HC06.println(" y probando...");
        
       
    } else {
    // protocolo open_bci V3, pero en hexadecimal
         char letras[5];
         for(int jj=0;jj<33+canales_extra*3;jj++){
          sprintf(letras, "%02X",txBuf[jj]);
          WiredSerial.print(letras);
          if(gBluetooth)HC06.print(letras);
         }
         WiredSerial.println("");
         if(gBluetooth)HC06.println("");

    }
  }
}






void _lee_Comando_Stream(Stream &puerto){
	if(puerto.available()!=0){
		String comando;
      intervalo_leeserial=1;
      ultimo_acceso_serie=0;
		while( (comando = puerto.readStringUntil(';')).length()>0){
			comando.toLowerCase();
			comando.trim();
			procesaComando(comando);
		}
   
	} else {
      if(intervalo_leeserial==1){
         if(ultimo_acceso_serie>1000*5) 
            intervalo_leeserial=16;
      }
   }
}

void lee_Comando_Serial_signalino(){
  _lee_Comando_Stream(WiredSerial);
  _lee_Comando_Stream(HC06);
}


// Comandos de control:
// gan: ganancia del amplificador
// inp: input mode (CHART, EEG, EMG)
// sim: señal de salida del signalino (real 1x, onda cuadrada del ads, señal simulada por software, real 12x )
// frm: formato de salida de datos ()
// rec: rec0 apaga salida EEG, rec1 la restaura
// hlp: mensaje de inicio y espera un enter por wiredsignal
// oka: mensaje de inicio y sigue corriendo
// blt: bluetooth 1:on/0:off
// sdc: grabando/no grabando en la SD card
// sfm: cambia frecuencia de muestreo


void procesaComando(String texto){
     comentaSerial(texto);
     comentaSerial("-->ok");

     comandos_parser  parcom;
     comandos_parser::cmd_control p1=parcom.extraeComando(texto);
     comandos_parser::codigos_cmd p2=parcom.traduceComando(p1);

     switch(p2){
      case comandos_parser::codigos_cmd::HLP:
           mensaje_inicio();          
           while(WiredSerial.available()==0 && HC06.available()==0 );
           return;
          break;
      case comandos_parser::codigos_cmd::SIM:
         if(gImpedanciasActivas==true){  
               ads9_misetup_ADS1299(MODE_IMPEDANCIAS_OFF); 
               gImpedanciasActivas=false;  
         }
         
         switch(p1.param){
           // 1 normal  2 test 3 simulada 4 max ganancia
            case 1:
              gSenal_obtenida=SENAL_REAL;
              ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
              break;
            case 2:
              gSenal_obtenida=SENAL_TEST;
              ads9_misetup_ADS1299(MODE_SENAL_TEST);
              break;
            case 3:
              gSenal_obtenida=TABLA_SENO;
              /* no activamos ads9_misetup porque... (vamos, que ni idea) */
              break;
           case 4:
              gSenal_obtenida=SENAL_REAL;
              ads9_misetup_ADS1299(MODE_SENAL_REAL_24x);
              break;
           case 5:
              gSenal_obtenida=SENAL_TEST;
              ads9_misetup_ADS1299(MODE_SENAL_TEST_24x);
              break;
          case 6: // reset
            #if defined(ARDUINO_SAM_DUE)
               ads9_misetup_ADS1299(MODE_SENAL_TEST);
               due_inicia_hw();
            #elif defined(TEENSYDUINO)
               ads9_misetup_ADS1299(MODE_SENAL_TEST);
               teensy_inicia_hw();
//             teensy_configini();
            #endif
               break;
         }       
         sprintf(buffer_comentaserial,"Signal mode changed to %d",p1.param);
         comentaSerial(buffer_comentaserial);
         break;
     
      case comandos_parser::codigos_cmd::FRM:
        switch(p1.param){
            case 0:
               if(++gFormatoSerial>maxComando)gFormatoSerial=minComando;
               else { if (p1.param<=maxComando)gFormatoSerial=p1.param;}
               break;
            default:  
               if (p1.param<=maxComando) gFormatoSerial=p1.param;
               break;
         }           
         sprintf(buffer_comentaserial,"Signal format changed to %d",p1.param);
         comentaSerial(buffer_comentaserial);

         break;

      case comandos_parser::codigos_cmd::REC:
        switch(p1.param){
           case 0:
            // rec0 calla la salida de numeros por puerto serie
            ultimo_modo=gFormatoSerial;
            gFormatoSerial=8;          
            break;  
           case 1:
            gFormatoSerial=ultimo_modo;
         }
         sprintf(buffer_comentaserial,"Data format restored to  %d",p1.param);
         comentaSerial(buffer_comentaserial);
         break;
         // return;
      case comandos_parser::codigos_cmd::RST:
         gSenal_obtenida=SENAL_REAL;
         ads9_misetup_ADS1299(MODE_SENAL_TEST);
         delay(500);
         ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
         comentaSerial("Reset de la entrada");
         break;
 

      case comandos_parser::codigos_cmd::GAN:
         gSenal_obtenida=SENAL_REAL;
        	// 1..7 1-2-4-6-8-12-24
         ads9_setGanancia(p1.param);
         sprintf(buffer_comentaserial,"Gain changed to %d",p1.param);
         comentaSerial(buffer_comentaserial);
         break;

/*
      case comandos_parser::codigos_cmd::SFM:
         gSenal_obtenida=SENAL_REAL;
         // HIGH_RES_[16k,8k,4k,2k,1k,500,250]_SPS
         ads9_set_fm(p1.param);
         sprintf(buffer_comentaserial,"fm changed to %d",p1.param);
         comentaSerial(buffer_comentaserial);
         break;
*/
      case comandos_parser::codigos_cmd::INP:
         comentaSerial(buffer_comentaserial);

            switch(p1.param){
               case 1:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_12x);
                ads9_misetup_ADS1299(MODE_SENAL_SRB1);
                comentaSerial("All channels to SRB1, 12x)");
                break;
               case 2:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
                ads9_misetup_ADS1299(MODE_SENAL_BIP);
                ads9_setGanancia(4);
                comentaSerial("EMG/EKG mode (bipolar channels, 4x)");
                break;
               case 3: //eeg, serian 8 monopolares 
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_24x);
                ads9_misetup_ADS1299(MODE_SENAL_SRB2);                
                ads9_setGanancia(24);
                comentaSerial("EEG mode (todos a erb2 channels, 24x)");
                break;
               case 4:  //psg, serian 6 monopolares y 2 bipolares
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_PSG);
                comentaSerial("PSG mode (6 srb2 2 bipolar channels, 24x)");
                break;
               case 5:  //psg, serian 6 monopolares y 2 bipolares
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_BIP);
                ads9_setGanancia(24);
                comentaSerial("Bipolares x24");
                break;
               default:
                  comentaSerial("Solo hay modos 1-2-3-4-5");
                  break;
             }
             break;
      case comandos_parser::codigos_cmd::BLT:
              switch(p1.param){
                case 0:
                  gBluetooth=false;
                  comentaSerial("BT off");
                  break;
                case 1:
                  gBluetooth=true;
                  comentaSerial("BT on");
                  break;
             }
             break;
      case comandos_parser::codigos_cmd::IMP:
              switch(p1.param){
                case 0:
                  ads9_misetup_ADS1299(MODE_IMPEDANCIAS_OFF); 
                  gImpedanciasActivas=false;  
                  comentaSerial("Impedancias OFF");
                  break;
                case 1:
                  ads9_misetup_ADS1299(MODE_IMPEDANCIAS_ON);   
                  gImpedanciasActivas=true;  
                  comentaSerial("Impedancias ON");
                  break;
             }
             break;
      case comandos_parser::codigos_cmd::LUX:
              switch(p1.param){
                case 0:
                  gLUX_ON=false;
                  gLUX_BOTH_ON=false;
                  canales_extra=0;                  
                  comentaSerial("EEG on IMU/LUX off");
                  break;
                case 1:
                  gLUX_ON=true;
                  gLUX_BOTH_ON=false;                  
                  canales_extra=0;                  
                  comentaSerial("IMU/LUX on");
                  break;
                case 2:
                  gLUX_ON=true;
                  gLUX_BOTH_ON=true;                  
                  canales_extra=0;                  
                  comentaSerial("IMU/LUX & EEG on");
                  break;

             }
             break;
             
      case comandos_parser::codigos_cmd::IPD:
           switch(p1.param){
                case 0:
                  gCRD_ON=false;
                  if(gCRD){
                     gCRD->cierra_archivo();
                     comentaSerial("SD off");
                  }   
                  break;
                case 1:
                  if(gCRD){
                     gCRD->abre_archivo("signalino_file.csv");
                     gCRD_ON=true;
                     comentaSerial("SD on");
                  }   
                  break;
             }
             break;

      case comandos_parser::codigos_cmd::OKA:
          mensaje_inicio();
          break;
      default:
          comentaSerial("Comando desconocido");
          break;
          
      }       
               
      return;

}


void procesaComando_estilo_openBCI(String texto){
      // no implementado, tal vez algun dia...
      return;

}
