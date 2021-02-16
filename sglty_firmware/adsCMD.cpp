
// This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
//
/* adsCMD.cpp
 *  Lectura, ganancias, muestreo, canales activos... del ADS1299
 */
 // cosas para hacer: 
 // en mi setup, creo q los canales se ponen a srb2 cuando cambias la ganancia
 // no es logico
 // gSenalObtenida deberia ser un enum
 // aqui no deberia haber mas que un adscmd.h y el resto de includes ahi
 // firmware_mbs.h contiene definiciones duplicadas, deberia desaparecer, creo
 // RDATAC -- read data continuous  
 // hay que revisar atentamente https://github.com/starcat-io/hackeeg-driver-arduino/blob/master/hackeeg_driver/adsCommand.cpp

#include "Arduino.h"   // use: Wprogram.h for Arduino versions prior to 1.0
#include "dueCMD.h"
#include "adsCMD.h"
#include "ads1298.h"
#include <SPI.h>  // include the SPI library:
#include "firmware_mbs.h"


//	int     gSenal_obtenida=TABLA_SENO;
int gSenal_obtenida 	= SENAL_REAL;
boolean gtestHEX 		= false;
boolean gserialVerbose 	= true; //activa/desactiva comentaserial
boolean gBluetooth 		= true;
boolean gImpedanciasActivas = true;

volatile int gHayLectura = 0;
CHIP_EEG gChip_EEG_instalado = AMP_ADS1299; //o al menos eso esperamos


// donde se apunta la lectura a la vuelta de la interrupcion
// es superimportante!
// son 80 por exceso (24 deberian bastar, suponiendo 8 canales, pero eso podria cambiar...)
volatile unsigned char serialBytes[MAX_SERIALBYTES];
volatile int numSerialBytes = 0;

long contador_muestras = 0;

// globales
int gMaxChan 		= 0; //maximum number of channels supported by ads1299 = 8
int gIDval 			= 0; //Device ID : lower 5 bits of  ID Control Register
int gNumActiveChan 	= 0;
boolean gisReadingDataNow 	= false;
boolean gActiveChan[MAX_CANALES_HARDWARE]; // reports whether channels 1..9 are active

void ads9_send_command(int cmd) {
	using namespace ADS1298;
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
		cs_low;
		SPI.transfer(cmd);
		delayMicroseconds(1);
		cs_high;
	SPI.endTransaction();

}

void ads9_wreg(int reg, int val) {
	//see pages 40,43 of datasheet -
	using namespace ADS1298;
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

int ads9_rreg(int reg) {
	using namespace ADS1298;
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

void ads9_misetup_ADS1299(MODOS_ADS1299 estado_ads1299) {
	// configura los parametros de ganancia y electrodos de referencia
	using namespace ADS1298;
	ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
	delay(1); // dicen que hay que esperar un poquito aqui, aunque tanto como 1 segundo... lo bajo a 1ms y parece que no pasa nada
	ads9_wreg(GPIO, char(0));
	ads9_wreg(CONFIG1, HIGH_RES_250_SPS);
	ads9_wreg(CONFIG2, 0xC0);  // no generate internal test signals
	delay(1); // dicen que hay que esperar un poquito aqui

	switch (estado_ads1299) {
	case MODE_SENAL_TEST:
		ads9_wreg(CONFIG2, INT_TEST_4HZ_2X);  // generate internal test signals
		ads9_wreg(CONFIG3, char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i), 
				char(TEST_SIGNAL | GAIN_1X));
		}
		break;
	case MODE_SENAL_TEST_24x:
		ads9_wreg(CONFIG2, INT_TEST_8HZ);  // generate internal test signals
		ads9_wreg(CONFIG3, char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i), 
				char(TEST_SIGNAL | GAIN_24X));
		}
		break;
	case MODE_SENAL_REAL_1x:
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i),
				char(ELECTRODE_INPUT | GAIN_1X )); 
		}
		break;
	case MODE_SENAL_REAL_12x:
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i),
					char(ELECTRODE_INPUT | GAIN_12X )); 
		}
		break;
	case MODE_SENAL_REAL_24x:
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i),
					char(ELECTRODE_INPUT | GAIN_24X )); 
		}
		break;
	// senal entraria por entradas P si SRB1 es referencia (srb1 une las entradas negativas)
	// senal entraria por entradas N si SRB2 es referencia
	// PACE	= 0x15, //misc1 en ADS1299
	case MODE_SENAL_SRB1:
		// referencial: set mode SRB1, util en EEG, 
		ads9_wreg(PACE, char(0x20)); //set SRB1. Es un electrodo q internamente se une a todas las entradas negativas
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i),
					char((ELECTRODE_INPUT ) & ~SRB2_INPUT)); //SRB1 
		}
		break;
	case MODE_SENAL_BIP:
		// TODOS bipolarES: srb1  y srb2 desactivados
		ads9_wreg(PACE, char(0x00)); //unset SRB1, switches open. 
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i),
					char((ELECTRODE_INPUT) & ~SRB2_INPUT)); 
					//desactivados SRB1 y SRB2 y ganancia 1
		}
		break;
	case MODE_SENAL_SRB2:
		// set mode SRB2, util en chart para coexistir referenciales y bipolares
		// senal entraria por entradas N 
		ads9_wreg(R1299::XMISC1, char(0x00)); //unset SRB1. 
		delay(150);
		for (int i = 1; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i),
					char(ELECTRODE_INPUT | SRB2_INPUT)); //SRB2 y ganancia 1
		}
		break;
	case MODE_SENAL_PSG:
		// CANALES 1..6 REFERENCIA A SRB2, GANANCIA 24
		// CANALES 7..8 BIPOLARES, GANANCIA 8
		ads9_wreg(R1299::XMISC1, char(0x00)); //unset SRB1. 
		delay(150);
		for (int i = 1; i <= 6; i++) {
			ads9_wreg(char(CHnSET + i),
					char(ELECTRODE_INPUT | SRB2_INPUT  | GAIN_24X)); //SRB2 y ganancia 1
		}
		for (int i = 7; i <= gMaxChan; i++) {
			ads9_wreg(char(CHnSET + i),
					char(ELECTRODE_INPUT | GAIN_8X & ~SRB2_INPUT)); //SRB2 y ganancia 1
		}
		break;
	case MODE_IMPEDANCIAS_ON:
		// activa el AC lead-off 
		ads9_wreg(R1299::XLOFF, 		0b10101110 ); // Setup register 4; Current and frequency for Lead_off. 
		ads9_wreg(R1299::XLOFF_SENSP, 	0b11111111 ); //Setup register 15; lead off enable . 
		ads9_wreg(R1299::XLOFF_SENSN, 	0b11111111 ); // Setup register 16, lead off detection in negative signal
		ads9_wreg(R1299::XCONFIG4, 		0b00000010 );//Setup register 23; lead off comparator enable
		gImpedanciasActivas=true;
		break;
	case MODE_IMPEDANCIAS_OFF:
		// desactiva el AC lead-off 
		ads9_wreg(R1299::XLOFF, 		0b00000000 ); // Setup register 4; Current and frequency for Lead_off. 
		ads9_wreg(R1299::XLOFF_SENSP, 	0b00000000 ); //Setup register 15; lead off enable . 
		ads9_wreg(R1299::XLOFF_SENSN, 	0b00000000 ); // Setup register 16, lead off detection in negative signal
		ads9_wreg(R1299::XCONFIG4, 		0b00000000 );//Setup register 23; lead off comparator enable
		gImpedanciasActivas=false;
		break;	
	}
	//start streaming data
	ads9_detectActiveChannels();
	gisReadingDataNow = true;
	ads9_send_command(RDATAC);
	ads9_send_command(START);
}

void ads9_setGanancia(int valor) { // 1..7 1-2-4-6-8-12-24
	using namespace ADS1298;
	ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
	delay(10);
	ads9_wreg(GPIO, char(0));
	delay(150);
	for (int i = 1; i <= gMaxChan; i++) {
		switch (valor) {
		case 1:
			ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_1X));
			break;
		case 2:
			ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_2X));
			break;
		case 3:
			ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_4X));
			break;
		case 4:
			ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_6X));
			break;
		case 5:
			ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_8X));
			break;
		case 6:
			ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_12X));
			break;
		case 7:
			ads9_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_24X));
			break;
		}
	}
	ads9_detectActiveChannels();

	gisReadingDataNow = true;
	ads9_send_command(RDATAC);
	ads9_send_command(START);
}

void ads9_set_fm(int p_samplefm) { // HIGH_RES_[16k,8k,4k,2k,1k,500,250]_SPS
	using namespace ADS1298;
	ads9_send_command(SDATAC); // dejamos el modo READ para emitir comandos
	delay(10);
	ads9_wreg(GPIO, char(0));
	ads9_wreg(CONFIG1, p_samplefm);
	gisReadingDataNow = true;
	ads9_send_command(RDATAC);
	ads9_send_command(START);
}

void ads9_detectActiveChannels() {
	//actualiza gActiveChan y gNumActiveChan y numSerialBytes
	using namespace ADS1298;
	gNumActiveChan = 0;
	for (int i = 1; i <= gMaxChan; i++) {
		delayMicroseconds(1);
		int chSet = ads9_rreg(CHnSET + i);
		gActiveChan[i] = ((chSet & 7) != SHORTED);
		if ((chSet & 7) != SHORTED)
			gNumActiveChan++;
	}
	numSerialBytes = 1 + (3 * gNumActiveChan); //8-bits header plus 24-bits per ACTIVE channel
    numSerialBytes=25;
}

void ads9_lee_datos(void) {
// hardware puro, 
// se utiliza como funcion apuntada en el vector de interrupciones (al inicio, en setup() )
// lee el ads y lo pone en serialBytes[]--numSerialBytes
	int i = 0;
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
        cs_low; // cs a 0, empezamos a leer el ads1299
		contador_muestras++;
		serialBytes[i++] = SPI.transfer(0); //get 1st byte of header
		SPI.transfer(0); //skip 2nd byte of header
		SPI.transfer(0); //skip 3rd byte of header
		for (int ch = 1; ch <= gMaxChan; ch++) {
			switch (gSenal_obtenida) {
			case SENAL_REAL:
				serialBytes[i++] = SPI.transfer(0);
				serialBytes[i++] = SPI.transfer(0);
				serialBytes[i++] = SPI.transfer(0);
				break;
			case TABLA_SENO:
				// señal seno, creada al inicio
				SPI.transfer(0);
				SPI.transfer(0);
				SPI.transfer(0);
				to_3bytes(samples_seno[contador_muestras % TABLE_SIZE] * 100,
						serialBytes + i);
				i += 3;
				break;
			}
		}
		// cs a 1, terminamos de leer el ads1299
		delayMicroseconds(1);
		cs_high;
	SPI.endTransaction();
	gHayLectura = 1;
	// se me ocurre q si este fuese gHayLectura++ podria servir como indicador de q no se pudo leer el anterior, tal vez porque va lento (overflow)...
}

void ads9_solo_datos_sin_eeg(void) {
// hardware puro, basada ads9_leedatos 
// para usar un arduino sin ads1299 (como un emulador)
// se utiliza como funcion apuntada en el vector de interrupciones (al inicio, en setup() )
// NO lee el ads, PERO RELLENA igual serialBytes[]--numSerialBytes
	int i = 0;
    cs_low;
  	contador_muestras++;
  	serialBytes[i++] = 0; //get 1st byte of header
  	for (int ch = 1; ch <= gMaxChan; ch++) {
  			// señal seno, creada al inicio
  			to_3bytes(samples_seno[contador_muestras % TABLE_SIZE] * 100,
  					serialBytes + i);
  			i += 3;
  	}
  	// cs a 1, hacemos como si leyesemos el ads1299
  	delayMicroseconds(1);
    //parpadea(200);
    cs_high;
	gHayLectura = 1;
}
