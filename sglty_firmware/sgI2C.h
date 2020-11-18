#pragma once
#include <Wire.h>

class i2c {
public:
    void get_twobytes( int16_t &mivar);
    void prepare_registro(int dispositivo, int registro, int num_variables_request);

};





