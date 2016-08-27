int v1=0;
boolean debugeando=false;

void serDecode() { //assuming Arduino is only transfering active channels
  
  int newlen = port.available();
  if(newlen<1)return;
  
  
  int packetBytes = 33; // modo openbci3
  int cabecero=2;
  
  if (newlen < packetBytes) return;
  byte[] rawData = new byte[10*packetBytes*2];
  int nn=port.readBytesUntil(0xC0, rawData);
  if(nn!=packetBytes)return;
 
  for (int i = 0; i < numCanales; i++) //first byte is status, then 3 bytes per channel
             lectura[i] =  (rawData[cabecero+(i*3)]  << 24) + (rawData[cabecero+1+(i*3)]  << 16) + (rawData[cabecero+2+(i*3) ]<<8); //load as 32bit to capture sign bit
  for (int i = 0; i < numCanales; i++)
             lectura[i] = (lectura[i] >> 8); //convert 32-bit to 24-bit integer processing deals with the sign,           

  //chapucilla, pero salvado un pesadisimo bug. Solo leemos una vez por vuelta en draw, y borramos el resto. Si no, se almacena en algun sitio, y se acumula. 
  while(port.available()>0){
    int inByte = port.read();
   }
} //void serDecode

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
  port = new Serial(this, Serial.list()[serialPortNumber], BAUD_RATE);    
  //vaciamos el buffer del puerto y leemos hasta un 0xC0
 while (port.available() > 0)  port.read(); 
 byte[] basura=port.readBytesUntil(0xC0);
    modo_conectado=true;  
}

void setPortNum() 
{
   String[] portStr = Serial.list();
   int nPort = portStr.length;
   if (nPort < 1) {
      javax.swing.JOptionPane.showMessageDialog(frame,"No devices detected: please check Arduino power and drivers.");  
      exit();    
   }
   int index = 0;
   for (int i=0; i<nPort; i++) {
     if (match(portStr[i], "cu.us") != null) index = i; //Arduino/Teensy names like /dev/cu.usbmodem*" and our BlueTooth is  /dev/cu.us922k0000bt 
     portStr[i] =  i+ " "+portStr[i] ;
   }  
   String respStr = (String) JOptionPane.showInputDialog(null,
      "<html><div align='right'>Memboost v 0.1 (c) 2016</div>Choose COM port (if not listed: check drivers and power)</html>", "Select Arduino",
      JOptionPane.PLAIN_MESSAGE, null,
      portStr, portStr[index]);
   serialPortNumber = Integer.parseInt(respStr.substring(0, 1));  
} //setPortNum()

void sendComando(String comando, Serial puerto)
{
     delay(200); // incomprensiblemente imprescindible, debe ser un bug de alguna libreria. Y me ha costado un huevo descubrirlo (windows 10, pc de elche).
     puerto.write(comando+";");
 
}