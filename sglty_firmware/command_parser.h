#ifndef com_par_h
#define com_par_h

#include "Arduino.h"


class comandos_parser {

 public:
  typedef struct  {
    char cmd[40];
    int param;
  } cmd_control;

// Comandos de control:
// gan: ganancia del amplificador
// inp: input mode (CHART, EEG, EMG)
// sim: señal de salida del signalino (real 1x, onda cuadrada del ads, señal simulada por software, real 12x )
// frm: formato de salida de datos ()
// rec: rec0 apaga salida, rec1 la restaura
// hlp: mensaje de inicio y espera un enter por wiredsignal
// oka: mensaje de inicio y sigue corriendo
// blt: bluetooth 1:on/0:off
// lux: lux 1:on/0:off


 typedef enum {ERR,GAN,INP,SIM,FRM,REC,HLP,OKA,BLT,LUX} codigos_cmd ; // empezamos en 1 para q 0 sea el ERR
 const static int numero_comandos=9;

 typedef struct {
	char cmd[4];
        codigos_cmd codigo;
 } tabla_comandos;

  tabla_comandos tabla_cmd [numero_comandos]={{"GAN",GAN},{"INP",INP},{"SIM",SIM},
	{"FRM",FRM},{"REC",REC},{"HLP",HLP},
	{"OKA",OKA},{"BLT",BLT},{"LUX",LUX}};
public:
 cmd_control extraeComando(String);
 codigos_cmd traduceComando(cmd_control);
 comandos_parser::codigos_cmd run_and_compare(String);
 

};


#endif


