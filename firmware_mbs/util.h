/* util.h */
#ifndef UTIL_H
#define UTIL_H

#include "Arduino.h"

#define max_int16 (32767)
#define min_int16 (-32767)

struct dos_int {
  int high;
  int low;
};

void to_hex(unsigned char byte, char *buf);
void dos_hex(int num, int *buf);
long to_Int32(unsigned char* byteArray);
long to_long(char b1, char b2, char b3);
struct dos_int to_10bit(unsigned char* byteArray);
void to_Int16( unsigned char* byteArray, unsigned char* byteArray2);

void to_3bytes(long i, unsigned char *j);
#endif /* UTIL_H */
