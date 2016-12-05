// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
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
//  Copyright © 2016 JABarios
//  This file is part of project: signalino
//
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
