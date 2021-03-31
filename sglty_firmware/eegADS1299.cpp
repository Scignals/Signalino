#include "eegADS1299.h"
#include "signalino.h"
#include <SPI.h>  // include the SPI library:


volatile unsigned char buffer_ads1299[80];

EEGADS1299::EEGADS1299(){
	serialBytes=buffer_ads1299;
}


void EEGADS1299::iniciar(int modo){

  switch(modo){	
	case modos_sg::IMPEDANCIAS:
	//intentamos ir a impedancias
		using namespace ADS1299;
		SPI.begin(); 

		reset_off;delay(10);reset_on;delay(100);

		send_command(SDATAC); // dejamos el modo READ para emitir comandos
		delay(2); // dicen que hay que esperar un poquito aqui, aunque tanto como 1 segundo... lo bajo a 1ms y parece que no pasa nada
		wreg(GPIO, char(0));
		wreg(CONFIG1, HIGH_RES_250_SPS);
		wreg(CONFIG2, 0xC0);  // no generate internal test signals
		delay(2); // dicen que hay que esperar un poquito aqui


		wreg(CONFIG3, char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
		wreg(0x01, 		    0b11010110); // Setup register 1
		wreg(0x02, 		    0b11010011); // Setup register 2
		wreg(0x03, 		   0b11101100); // Setup register 3
		wreg(0x11, 		   0b00000000); // Setup register 3
		wreg(R1299::XLOFF, 		    0b10101110 ); // Setup register 4; Current and frequency for Lead_off. 
		wreg(R1299::XLOFF_SENSP, 	0b11111111 ); //Setup register 15; lead off enable . 
		wreg(R1299::XLOFF_SENSN, 	0b11111111 ); // Setup register 16, lead off detection in negative signal
		wreg(R1299::XCONFIG4, 		0b00000010 );//Setup register 23; lead off comparator enable

		
		wreg(PACE, char(0x00)); //unset SRB1, switches open. 
		delay(150);
		for (int i = 1; i <= 8; i++) {
			wreg(char(CHnSET + i),
					char((ELECTRODE_INPUT) & ~SRB2_INPUT)); 
					//desactivados SRB1 y SRB2 y ganancia 1
		}

		send_command(RDATAC);
		delay(2);
		send_command(START);
		break;

case modos_sg::BIPOLAR:
	//intentamos ir a registro normal, todos bipolares
		using namespace ADS1299;
		SPI.begin(); 
		reset_off;delay(10);reset_on;delay(100);

		send_command(SDATAC); // dejamos el modo READ para emitir comandos
		delay(2); // dicen que hay que esperar un poquito aqui, aunque tanto como 1 segundo... lo bajo a 1ms y parece que no pasa nada
		wreg(GPIO, char(0));
		wreg(CONFIG1, HIGH_RES_250_SPS);
		wreg(CONFIG2, 0xC0);  // no generate internal test signals
		delay(2); // dicen que hay que esperar un poquito aqui

		wreg(PACE, char(0x00)); //unset SRB1, switches open. 
		delay(150);
		for (int i = 1; i <= 8; i++) {
			wreg(char(CHnSET + i),
					char((ELECTRODE_INPUT) & ~SRB2_INPUT)); 
					//desactivados SRB1 y SRB2 y ganancia 1
		}

		send_command(RDATAC);
		delay(2);
		send_command(START);
		break;

case modos_sg::TEST:
	//intentamos ir a modo test
		using namespace ADS1299;
		SPI.begin(); 
		reset_off;delay(10);reset_on;delay(100);

		wreg(CONFIG2, INT_TEST_8HZ);  // generate internal test signals
		delay(10); 
		wreg(CONFIG3, char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
		delay(150);
		for (int i = 1; i <= 8; i++) {
			wreg(char(CHnSET + i), 
				char(TEST_SIGNAL | GAIN_1X));
		}
		send_command(RDATAC);
		delay(2);
		send_command(START);
		break;
  }		

//------------------

//		for (int i = 1; i <= 8; i++) {
//			wreg(char(CHnSET + i),
//				char(ELECTRODE_INPUT | GAIN_1X )); 
//		}



//-----------------
/*
		wreg(CONFIG2, INT_TEST_8HZ);  // generate internal test signals

		delay(10); 
		wreg(CONFIG3, char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
		delay(150);
		for (int i = 1; i <= 8; i++) {
			wreg(char(CHnSET + i), 
				char(TEST_SIGNAL | GAIN_1X));
		}

*/
//-------------------

};


void EEGADS1299::reset(){};

 volatile unsigned char *EEGADS1299::getUltimaLectura(){
    return(serialBytes);
}







void EEGADS1299::send_command(int cmd) {
	using namespace ADS1299;
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
		cs_low;
		SPI.transfer(cmd);
		delayMicroseconds(1);
		cs_high;
	SPI.endTransaction();

}

void EEGADS1299::wreg(int reg, int val) {
	//see pages 40,43 of datasheet -
	using namespace ADS1299;
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
    cs_low;
		SPI.transfer(WREG | reg);
		delayMicroseconds(2);
		SPI.transfer(0);	// number of registers to be read/written – 1
		delayMicroseconds(2);
		SPI.transfer(val);
		//en datasheet dice q modificar config1 es como hacer un reset
		//luego habra que esperar 18 clocks al menos
		delayMicroseconds(1); 
    cs_high;
	SPI.endTransaction();

}

int EEGADS1299::rreg(int reg) {
	using namespace ADS1299;
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
		int out = 0;
    	cs_low;
		SPI.transfer(RREG | reg);
		delayMicroseconds(5);
		SPI.transfer(0);	// number of registers to be read/written – 1
		delayMicroseconds(5);
		out = SPI.transfer(0);
		delayMicroseconds(1);
    	cs_high;
	SPI.endTransaction();
	return (out);
}


void EEGADS1299::leerDatos(void) {
	int i = 0;
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
        cs_low; // cs a 0, empezamos a leer el ads1299
		SPI.transfer(0x12);// Send  RDATA for SPI
	  	delayMicroseconds(2);
		serialBytes[i++] = SPI.transfer(0); //get 1st byte of header
		SPI.transfer(0); //skip 2nd byte of header
		SPI.transfer(0); //skip 3rd byte of header
		for (int canal = 1; canal <= 8; canal++) {
				serialBytes[i++] = SPI.transfer(0);
				serialBytes[i++] = SPI.transfer(0);
				serialBytes[i++] = SPI.transfer(0);
		}
		// cs a 1, terminamos de leer el ads1299
		delayMicroseconds(1);
		cs_high;
	SPI.endTransaction();
	hayLectura = 1;
}
