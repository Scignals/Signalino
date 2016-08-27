import controlP5.*;

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


ControlP5 gui;

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
  ADS4ch = new Chart(anchoPantalla/2-50,altoPantalla/2,anchoPantalla-100,altoPantalla-50,numCanales,0.0001);

  gui = new ControlP5(this);
  gui.addRadioButton("Senal")
   .setPosition(910,40)
   .setSize(10,27)
   .setValue(1)
   .addItem("senal",1)
   .addItem("test",2)
   .addItem("simul",3)
   ;

}

int n=1;
void draw() {
     if(modo_conectado)serDecode();
     else serRand();
   //  serEco();
     ADS4ch.set(lectura);
     ADS4ch.update();
}

public void controlEvent(ControlEvent theEvent) {
//Is called whenever a GUI Event happened
}
public void Senal(int value){
  println("cambiando modo senal a "+value);
  switch(value){
     case 1:
       sendComando("sim1;",port); 
       break;
     case 2:
       sendComando("sim2;",port);
       break;
     case 3:
       sendComando("sim3;",port);
       break;  
  }
}