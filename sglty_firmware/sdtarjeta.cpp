#include "sdtarjeta.h"


extern boolean gCRD_ON; // archivo abierto y escribible
 
int sdtarjeta::info() {
    // reinicia SD card y comprueba si se puede leer
    Serial.print("\nInitializing SD reader...");
    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    /*  Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
    */
        operativa=false; return -1;
    } else {
        Serial.println("Wiring is correct and a card is present.");
        operativa=true;
        //return 0;
    }

  // print the type of card
  String tipo_tarjeta = "\nCard type: ";
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      tipo_tarjeta += "SD1";
      break;
    case SD_CARD_TYPE_SD2:
      tipo_tarjeta += "SD2";
      break;
    case SD_CARD_TYPE_SDHC:
      tipo_tarjeta += "SDHC";
      break;
    default:
      tipo_tarjeta += "Unknown";
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    operativa=false; return -1;

  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  tipo_tarjeta += ". Voltype: FAT";
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  if (volumesize < 8388608ul) {
    Serial.print("Volume size (bytes): ");
    Serial.println(volumesize * 512);        // SD card blocks are always 512 bytes
  }
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 2;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
 // root.ls(LS_R | LS_DATE | LS_SIZE);
 // while(1){};
  return 0;
}


bool sdtarjeta::iniciar( ) {

  Serial.print("Initializing SD card...");

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("initialization failed!");
    return false;
  }
  Serial.println("initialization done.");
  return true;
}  


bool sdtarjeta::abre_archivo( const char *nombre_archivo) {

  
  
//    if(!operativa)return false;

    nombre_archivo="unfile.txt";
    archivo= SD.open(nombre_archivo, FILE_WRITE);
    if (!archivo) {
        Serial.print("error opening ");
        Serial.println(nombre_archivo);
        gCRD_ON=false;
        return false;
    } else {
        Serial.print("Aqui estamos en archivo ok!!");
        while(1){};
        gCRD_ON=true;
        return true;
    }

    archivo.close();

}

bool sdtarjeta::cierra_archivo( ) {
    if(!operativa)return false;
    archivo.close();
    gCRD_ON=false;
        
}

//void sdtarjeta::escribe(byte *fuente) {
   /*
    for(int i=0;i<num_canales;i++){
        long num=to_Int32(fuente+i*3):
        Serial.prinln(num);
        Serial.prinln(",");
    }
    Serial.print("");
    */
//    imprime_linea2(1,archivo);

//}



//void sdtarjeta::cerrar( const char *nombre_archivo) {
//    archivo.close();
    
//}

