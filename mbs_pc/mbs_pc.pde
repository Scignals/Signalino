///////////////////////////////////////////////
//
// GUI for controlling the ADS1299-based signalino
//
// Created: JA Barios, august 2016 - january 2017
// Modified: 
//
// Requires ControP5 library
//
// No warranty.  Use at your own risk.  Use for whatever you'd like.
// 

/*
* Author: JABarios, agosto 2016 - enero 2017
* Company: ILSB, Spain. 
* No warranty.  Use at your own risk.  Use for whatever you'd like.
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

///////////////////////////////////////////////

import controlP5.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.io.FileWriter;
 
import processing.serial.*;
import javax.swing.JOptionPane;//For user input dialogs

int BAUD_RATE = 115200;//57600 230400;//921600;//460800;//921600; //921600 for Teensy2/Teensy3/Leonardo 460800
int numCanales = 8; //number of channels to report
int serialPortNumber = 0; //set to 0 for automatic detection


Serial port;      // Create object from Serial class
int[]  lectura;              // Datos leidos en el puerto serie



int anchoPantalla=1000;
int altoPantalla=600;
Chart ADS4ch;


boolean modo_conectado = false;
boolean modo_test      = false;


// global var containing the gui
ControlP5 gui;


//create a file first for recording signal    
File file;
FileWriter fw;
PrintWriter outputfile;
  
    
void setup() {
  size(1000, 600);
  rectMode(CENTER);

  println("Signalino, signal visor 0.2 (c) 2017 ILSB ");

  lectura=new int[numCanales];

  modo_conectado=false;
  if (!modo_test){
     serie_inicia();
     //pone el ads en modo 6, bytes openBCI
     
     sendComando("frm6",port);
     sendComando("frm6",port);
     sendComando("frm6",port);
     
     sendComando("sim3",port);     
  }   
  ADS4ch = new Chart(anchoPantalla/2-50,altoPantalla/2,anchoPantalla-100,altoPantalla-50,numCanales,0.01);
  gui = new ControlP5(this);
  try {
    file = new File("./signalino_rawdata.txt");
    fw = new FileWriter(file);
    outputfile = new PrintWriter(fw);
  } catch (IOException e) {
    println("File error. Sorry...");
    e.printStackTrace();
  } 
  iniciaGui(gui);
  
}

void draw() {
     if(modo_conectado)serDecode(ADS4ch.bf);
     else serRand();
   //  serEco();
     ADS4ch.set(lectura);
     ADS4ch.update();
}
