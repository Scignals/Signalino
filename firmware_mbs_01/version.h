/*version.h*/
#ifndef VERSION_H_
#define VERSION_H_

#include "Arduino.h"

#define HC06 Serial3  
#define WiredSerial Serial

extern const char * build_version;
extern const char * build_fecha;
extern int gMaxChan;

void mensaje_inicio(void);




#endif /* VERSION_H_ */
