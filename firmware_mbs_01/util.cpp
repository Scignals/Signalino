#include "util.h"

void to_hex(unsigned char byte, char *buf)
// coge char ("byte") y devuelve como cadena ("buf")  
{
	int i;
	char nibbles[2];

	nibbles[0] = (byte & 0xF0) >> 4;
	nibbles[1] = (byte & 0x0F);

	for (i = 0; i < 2; i++) {
		if (nibbles[i] < 10) {
			buf[i] = '0' + nibbles[i];
		} else {
			buf[i] = 'A' + nibbles[i] - 10;
		}
	}
	buf[2] = '\0';
}


long to_long(char b1, char b2, char b3)
{
  long i = b1 << 24 + b2 << 16 + b3 << 8;
  i = i >> 8;

 // sprintf(buf, "%Lu", i);
  return i;
}


struct dos_int m1to_10bit(unsigned char *b)
{
  dos_int salida;
  byte sync0 = 0xA5;
  byte foo=0;
  
  unsigned int val_u16;  //16-bit
  
  long i = to_Int32(b);
  
  i = i / 32;
  i = constrain(i,min_int16,max_int16);  //constrain to fit in 16 bits
  val_u16 = (unsigned int) (i & (0x0000FFFF));  //truncate and cast
  if (val_u16 > 1023) val_u16 = 1023;
  
  foo = (byte)((val_u16 >> 8) & 0x00FF); //high byte
  if (foo == sync0) foo--;
  salida.high=foo;

  foo = (byte)(val_u16 & 0x00FF); //high byte
  if (foo == sync0) foo--;
  salida.low=foo;

  return salida;
}


struct dos_int to_10bit(unsigned char* byteArray)
{
  dos_int salida;
  byte sync0 = 0xA5;
  byte foo=0;
  
  unsigned int val_u16;  //16-bit
  
  long i = to_Int32(byteArray);
  i = i / 8192.0;

  val_u16 = (unsigned int) (i & (0x0000FFFF));  //truncate and cast
  if (val_u16 > 1023) val_u16 = 1023;
  
  foo = (byte)((val_u16 >> 8) & 0x00FF); //high byte
  if (foo == sync0) foo--;
  salida.high=foo;

  foo = (byte)(val_u16 & 0x00FF); //high byte
  if (foo == sync0) foo--;
  salida.low=foo;

  return salida;
}

void dos_hex(int num, int *buf)
{
  int c1=(num<<4);
  buf[1]=num - c1*num;
  int c2=(c1<<4);
  buf[0]= c1- c2*c1;
  
}

long to_Int32( unsigned char* byteArray) {     
 long newInt = (  
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

void to_Int16( unsigned char* byteArray, unsigned char* byteArray2) {     
   long newInt = to_Int32( byteArray);
 //  newInt += 0xFFFFFF; 
   int ss = (int) (newInt >> 0);
   byteArray2[0] = (ss & 0xFF00) >> 8;
   byteArray2[1] =  ss & 0x00FF;
}  


void to_3bytes(long i, unsigned char *j)
{
  j[0]=( i & 0x00FF0000 ) >> 16;
  j[1]=( i & 0x0000FF00 ) >> 8;
  j[2]=( i & 0x000000FF );
 
}


