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
int v1=0;
boolean debugeando=false;
int gUltimotimestamp;
float fm_calculada;

void serDecode(Buffer bf) { //assuming Arduino is only transfering active channels

  int packetBytes = 33; // modo openbci3
  int cabecero=2;
  byte[] rawData = new byte[10*packetBytes*2];
  
  byte[] localAdsByteBuffer = {0,0,0};

  int newlen = port.available();
  if (newlen < packetBytes) return;

      while(port.available()>=packetBytes){
           int nn=port.readBytesUntil(0xC0, rawData);
           if(nn!=packetBytes)continue;
 
           for (int i = 0; i < numCanales; i++){
                     localAdsByteBuffer[0]=rawData[cabecero+0+(i*3)];
                     localAdsByteBuffer[1]=rawData[cabecero+1+(i*3)];
                     localAdsByteBuffer[2]=rawData[cabecero+2+(i*3)];
                     lectura[i] = interpret24bitAsInt32(localAdsByteBuffer);
           }   
           localAdsByteBuffer[0]=rawData[cabecero+0+(numCanales*3)];
           localAdsByteBuffer[1]=rawData[cabecero+1+(numCanales*3)];
           localAdsByteBuffer[2]=rawData[cabecero+2+(numCanales*3)];
           int timestamp = interpret24bitAsInt32(localAdsByteBuffer);
           fm_calculada=floor(1000000/(timestamp-gUltimotimestamp+1));
           gUltimotimestamp=timestamp;
           //println(fm_calculada);
           bf.apunta(lectura);
           if(gGrabando)bf.graba(lectura);

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



void serRand(Buffer bf) { //numeros aleatorios
  int timestamp=0;
  for(int jj=1;jj<2;jj++){
      for(int i=0;i<numCanales;i++){
               lectura[i]=1000*(int) random(1,100);
      }
      timestamp = millis()*1000;
      gUltimotimestamp=timestamp;
              
      bf.apunta(lectura);
      if(gGrabando)bf.graba(lectura);
  }
  fm_calculada=floor(1000000/(timestamp-gUltimotimestamp+1));

} //void serRand

void serEco() { //numeros aleatorios
    int newlen = port.available();
    if(newlen<1)return;
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
   //   modo_conectado=false;
      port = new Serial(this, Serial.list()[serialPortNumber], BAUD_RATE);    
      //vaciamos el buffer del puerto y leemos hasta un 0xC0
     while (port.available() > 0)  port.read(); 
     // aqui es donde sospecho que se cuelga, cuando no encuentra un signalino
     // no se si realmente hace algo
       // int retorno=port.readBytesUntil(0xC0,basura);
      basura=port.readBytesUntil(0xC0);
      modo_conectado=true;
        
   //     print("retorno:");
   //     println(retorno);
   //     if(retorno>-1) modo_conectado=true;

  } catch (Exception e){ 
          javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Scignals v 0.3 (c) 2016</div>"+
           "<p>No devices detected: please check Arduino power and drivers. Offline mode...</p></html>");  
          modo_conectado=false;
    //exit();
  }
}

void setPortNum() 
{
  String[] portStr = Serial.list();
  int nPort = portStr.length;
  serialPortNumber=-1;
  
  
  
  
   
//    List<String> new_portStr = Arrays.asList(portStr);
//    new_portStr.add("Test signal");
//    String[] temp_portStr = new String[new_portStr.size()];
//    portStr = new_portStr.toArray(temp_portStr);

   if (nPort < 1) {
      javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Scignals v 0.3 (c) 2016</div>"+
           "<p>No devices detected: please check Arduino power and drivers. Exiting program...</p></html>");  
      exit();    
      return;
   }
   int index = 0;
   for (int i=0; i<nPort; i++) {
     // esta linea parece una fineza, apareceria resaltado si se llamara cu.us. El problema es q en windows se llama com1 , y en mi linux se llama acm1, y ...
     // pero la mantengo, por si algun dia se me ocurre algo mejor
     if (match(portStr[i], "cu.us") != null) index = i; //Arduino/Teensy names like /dev/cu.usbmodem*" and our BlueTooth is  /dev/cu.us922k0000bt 
     portStr[i] =  i+ " "+portStr[i] ;
   }
   
  String[] array2 = new String[]{portStr.length+" SIMULATED noisy Signal"};
  String[] array = new String[portStr.length + array2.length];
  System.arraycopy(portStr, 0, array, 0, portStr.length);
  System.arraycopy(array2, 0, array, portStr.length, array2.length);
  nPort=nPort+1;
  
   String respStr = (String) JOptionPane.showInputDialog(null,
      "<html><div align='center'>Scignals v 0.3 (c) 2016</div>"+
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