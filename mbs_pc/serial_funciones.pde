int v1=0;
boolean debugeando=false;

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
           outputfile.println(lectura[2]);           
             bf.apunta(lectura);
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



void serRand() { //numeros aleatorios
  for(int i=0;i<numCanales;i++){
           lectura[i]=(int) random(1,100);
        }
} //void serRand

void serEco() { //numeros aleatorios
    int newlen = port.available();
    if(newlen<1)return;
      String texto=port.readString();
      print(texto);
} //void serEco


void serie_inicia()
{
  if (serialPortNumber == 0) {
    setPortNum();
  } else {
    print("Will attempt to open port "); println(serialPortNumber); 
    println(", this should correspond to the device number in this list:");
    println(Serial.list());
    println("Hint: if you set serialPortNumber=0 the program will allow the user to select from a drop down list of available ports");
  }
//  if(serialPortNumber>0){
      port = new Serial(this, Serial.list()[serialPortNumber], BAUD_RATE);    
      //vaciamos el buffer del puerto y leemos hasta un 0xC0
     while (port.available() > 0)  port.read(); 
     byte[] basura=port.readBytesUntil(0xC0);
        modo_conectado=true;  
 // } else exit();    
}

void setPortNum() 
{
   String[] portStr = Serial.list();
   int nPort = portStr.length;
   if (nPort < 1) {
      javax.swing.JOptionPane.showMessageDialog(frame,"No devices detected: please check Arduino power and drivers. Exiting program...");  
      exit();    
      return;
   }
   int index = 0;
   for (int i=0; i<nPort; i++) {
     if (match(portStr[i], "cu.us") != null) index = i; //Arduino/Teensy names like /dev/cu.usbmodem*" and our BlueTooth is  /dev/cu.us922k0000bt 
     portStr[i] =  i+ " "+portStr[i] ;
   }  
   String respStr = (String) JOptionPane.showInputDialog(null,
      "<html><div align='right'>Signalino v 0.2 (c) 2016</div>Choose COM port (if not listed: check drivers and power)</html>", "Select Signalino device",
      JOptionPane.PLAIN_MESSAGE, null,
      portStr, portStr[index]);
   serialPortNumber = Integer.parseInt(respStr.substring(0, 1));  
} //setPortNum()

void sendComando(String comando, Serial puerto)
{
   if(puerto!=null){ 
     delay(200); // incomprensiblemente imprescindible, debe ser un bug de alguna libreria. Y me ha costado un huevo descubrirlo (windows 10, pc de elche).
     puerto.write(comando+";");
   }  
 
}