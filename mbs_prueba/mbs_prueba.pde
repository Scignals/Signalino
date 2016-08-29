import java.util.ArrayList;
import java.util.Arrays;

import processing.serial.*;
import javax.swing.JOptionPane;//For user input dialogs

int BAUD_RATE = 57600;//230400;//921600;//460800;//921600; //921600 for Teensy2/Teensy3/Leonardo 460800
int numCanales = 8; //number of channels to report
int serialPortNumber = 0; //set to 0 for automatic detection

Serial port;      // Create object from Serial class
int[]  lectura;              // Datos leidos en el puerto serie



int anchoPantalla=1000;
int altoPantalla=600;
Chart ADS4ch;


boolean modo_conectado = false;
boolean modo_test = false;
boolean prima_volta=true;



void setup() {
  size(1000, 600);
  rectMode(CENTER);

  lectura=new int[numCanales];

     serie_inicia();
     port.buffer(1);
      // port.write("");
       delay(200);
       port.write("frm1;");
      // delay(1000);
  
    
}

int contador=0;

void draw() {
     serEco() ;
     if(prima_volta){
        //    port.write("frm1;");
        //    delay(1000);
        //    if(++contador>1)prima_volta=false;
     }
    delay(1);

}