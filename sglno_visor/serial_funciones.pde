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
//
//  Copyright © 2016 JA Barios
//  This file is part of project: SCIGNALS, a chart recorder.
//
int   v1=0;
int   gUltimoTimeStamp;
float fm_calculada;
boolean debugeando=false;

void serDecode(Buffer bf) { 
  //assuming Arduino is only transfering active channels
  //assuming que cada paquete termina en 0xC0 (discutible)
  //assuming que el formato es openbci3 (6)

  int bytes_por_paquete = 33; // modo openbci3
  int longitud_cabecero=2; // dos bytes: A0 y el timestamp? 
  byte[] rawData = new byte[10*bytes_por_paquete*200];
  byte[] localAdsByteBuffer = {0,0,0};
  int bytes_available = port.available();
  
  if (bytes_available < bytes_por_paquete) return;
  if (bytes_available > 50*bytes_por_paquete*2){
                 println("buffer overrun...");
                 port.clear();
                 return;
  }

  while(port.available()>=bytes_por_paquete){
           int nn=port.readBytesUntil(0xC0, rawData);

           //si no coincide la longitud, debe ser un error de lectura
           if(nn!=bytes_por_paquete) 
              continue;
 
           for (int i = 0; i < numCanales; i++){
                     localAdsByteBuffer[0]=rawData[longitud_cabecero+0+(i*3)];
                     localAdsByteBuffer[1]=rawData[longitud_cabecero+1+(i*3)];
                     localAdsByteBuffer[2]=rawData[longitud_cabecero+2+(i*3)];
                     buffer_lectura[i] = interpret24bitAsInt32(localAdsByteBuffer);
           }   

           localAdsByteBuffer[0]=rawData[longitud_cabecero+0+(numCanales*3)];
           localAdsByteBuffer[1]=rawData[longitud_cabecero+1+(numCanales*3)];
           localAdsByteBuffer[2]=rawData[longitud_cabecero+2+(numCanales*3)];
           int timestamp = interpret24bitAsInt32(localAdsByteBuffer);
           fm_calculada=floor(1000000/(timestamp-gUltimoTimeStamp+1));
           gUltimoTimeStamp=timestamp;
           //println(fm_calculada);
           bf.apunta(buffer_lectura);
           if(gGrabando)bf.graba(buffer_lectura);

       }
             
} //void serDecode

int interpret24bitAsInt32(byte[] byteArray) {     
    //little endian
    int newInt = ( 
      ((0xFF & byteArray[0]) << 16) |
      ((0xFF & byteArray[1]) << 8) | 
      (0xFF & byteArray[2])
      );
    if ((newInt & 0x00800000) > 0) {
      newInt |= 0xFF000000;
    } else {
      newInt &= 0x00FFFFFF;
    }
    return newInt;
  }

void serLSL(Buffer bf) { //numeros aleatorios
  int timestamp=0;
  for(int jj=1;jj<2;jj++){
      for(int i=0;i<numCanales;i++){
               buffer_lectura[i]=1000*(int) random(1,100);
      }
      timestamp = millis()*1000;
      gUltimoTimeStamp=timestamp;
              
      bf.apunta(buffer_lectura);
      if(gGrabando)bf.graba(buffer_lectura);
  }
  fm_calculada=floor(1000000/(timestamp-gUltimoTimeStamp+1));

} //void serRand


void serRand(Buffer bf) { //numeros aleatorios
  int timestamp=0;
  for(int jj=1;jj<2;jj++){
      for(int i=0;i<numCanales;i++){
               buffer_lectura[i]=1000*(int) random(1,100);
      }
      timestamp = millis()*1000;
      gUltimoTimeStamp=timestamp;
              
      bf.apunta(buffer_lectura);
      if(gGrabando)bf.graba(buffer_lectura);
  }
  fm_calculada=floor(1000000/(timestamp-gUltimoTimeStamp+1));

} //void serRand

void serEco() { //copia en la consola lo que entra por el puerto
    int bytes_available = port.available();
    if(bytes_available<1)return;
      String texto=port.readString();
      print(texto);
} //void serEco


void serie_inicia()
{
  byte[] basura = new byte[1000]; //tiene que ser grande, a veces se cuelga
  if (serialPortNumber == 0) {
    setPortNum();
  } else {
    print("Will attempt to open port "); println(serialPortNumber); 
    println(", this should correspond to the device number in this list:");
    println(Serial.list());
    println("Hint: if you set serialPortNumber=0 the program will allow the user to select from a drop down list of available ports");
  }
  try{
      modo_conectado=false;
      port = new Serial(this, Serial.list()[serialPortNumber], BAUD_RATE);    
      port.readBytesUntil(0xC0,basura);
      modo_conectado=true;
  } catch (Exception e){ 
          javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Scignals v "+version_software+" (c) 2016</div>"+
           "<p>No devices detected: please check Arduino power and drivers. Going to Offline mode...</p></html>");  
          modo_conectado=false;
    //exit();
  }
}

void LSL_inicia()
{
  byte[] basura = new byte[1000]; //tiene que ser grande, a veces se cuelga
  if (serialPortNumber == 0) {
    setPortNum();
  } else {
    print("Will attempt to open port "); println(serialPortNumber); 
    println(", this should correspond to the device number in this list:");
    println(Serial.list());
    println("Hint: if you set serialPortNumber=0 the program will allow the user to select from a drop down list of available ports");
  }
  try{
      modo_LSL=false;
//      port = new Serial(this, Serial.list()[serialPortNumber], BAUD_RATE);    
//      port.readBytesUntil(0xC0,basura);
      modo_LSL=true;
  } catch (Exception e){ 
          javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Scignals v "+version_software+" (c) 2016</div>"+
           "<p>No LSL streams detected: please check communications. Going to Offline mode...</p></html>");  
          modo_LSL=false;
    //exit();
  }
}


void setPortNum() 
{
  String[] portStr = Serial.list();
  int nPort = portStr.length;
  serialPortNumber=-1;

   if (nPort < 1) {
      javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Scignals v "+version_software+" (c) 2016</div>"+
           "<p>No serial devices detected: please check Arduino power and drivers. Exiting program...</p></html>");  
      exit();    
      return;
   }
   int index = 0;
   for (int i=0; i<nPort; i++) {
     // esta linea parece una fineza, apareceria resaltado si se llamara cu.us. 
     // El problema es q en windows se llama com1 , y en mi linux se llama acm1, y ...
     // pero la mantengo, por si algun dia se me ocurre algo mejor
     if (match(portStr[i], "cu.us") != null) index = i; //Arduino/Teensy names like /dev/cu.usbmodem*" and our BlueTooth is  /dev/cu.us922k0000bt 
     portStr[i] =  i+ " "+portStr[i] ;
   }
   
  String[] array2 = new String[]{portStr.length+" SIMULATED noisy Signal"};
  String[] array3 = new String[]{portStr.length+" LSLstream"};
  String[] array = new String[portStr.length + array2.length];
  System.arraycopy(portStr, 0, array, 0, portStr.length);
  System.arraycopy(array2, 0, array, portStr.length, array2.length);
  nPort=nPort+1;
  // no veo como hacer que aparezca al final del array, ahora se pone en el ultimo lugar pisando al noisy
//  array = new String[portStr.length + array3.length];
//  System.arraycopy(portStr, 0, array, 0, portStr.length);
//  System.arraycopy(array3, 0, array, portStr.length, array3.length);
//  nPort=nPort+1;

  
   String respStr = (String) JOptionPane.showInputDialog(null,
      "<html><div align='center'>Scignals v "+version_software+" (c) 2016</div>"+
      "<p>This program is free software, distributed under GNU General Public License (v3) in the hope that it will be useful, but WITHOUT ANY WARRANTY</p>"+
      "<p align='center'>Choose COM port (if not listed: check drivers and power)</p></html>", "Select device",
      JOptionPane.PLAIN_MESSAGE, null,
      array, array[index]);
      println(respStr);
      try{
         if(respStr!=null) serialPortNumber = Integer.parseInt(respStr.substring(0, 1));
      } catch (Exception e){ 
         // no hacer nada, es por si acaso
      }    
       
} //setPortNum()

void sendComando(String comando, Serial puerto)
{
   if(puerto!=null){ 
     delay(200); // incomprensiblemente imprescindible, debe ser un bug de alguna libreria. Y me ha costado un huevo descubrirlo (windows 10, pc de elche).
     puerto.write(comando+";");
   }  
 
}
