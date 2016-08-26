import java.util.ArrayList;
import java.util.Arrays;

import processing.serial.*;
import javax.swing.JOptionPane;//For user input dialogs

int BAUD_RATE = 57600;//230400;//921600;//460800;//921600; //921600 for Teensy2/Teensy3/Leonardo 460800
int numCanales = 8; //number of channels to report
int serialPortNumber = 0; //set to 0 for automatic detection

boolean primera_vuelta=true;

Serial port;      // Create object from Serial class
int[]  lectura;              // Datos leidos en el puerto serie



int anchoPantalla=1000;
int altoPantalla=600;
Chart ADS4ch;


boolean modo_conectado = false;
boolean modo_test = false;




void setup() {
  size(1000, 600);
  rectMode(CENTER);

  lectura=new int[numCanales];

  modo_conectado=false;
  if (!modo_test){
     serie_inicia();
     // port.buffer(1);  // no se si hace falta
     //pone el ads en modo 6, bytes openBCI
     sendComando("frm6",port);
     sendComando("sim3",port);
     
  }   
  ADS4ch = new Chart(anchoPantalla/2,altoPantalla/2,anchoPantalla-10,altoPantalla-50,numCanales);

}

int n=1;
void draw() {

     if(modo_conectado)serDecode();
     else serRand();
   //  serEco();
     ADS4ch.set(lectura);
     ADS4ch.update();

}