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
// lux: lux+acelerometro 1:on/0:off
// imp: activar medición de impedancias
// rst: reset
// sdc: grabando en SD



 typedef enum {ERR,GAN,INP,SIM,FRM,REC,RST,HLP,OKA,BLT,LUX,IMP,SDC,IPD} codigos_cmd ; // empezamos en 1 para q 0 sea el ERR
 const static int numero_comandos=13; // el ERR no cuenta

 typedef struct {
	char cmd[4];
        codigos_cmd codigo;
 } tabla_comandos;

  tabla_comandos tabla_cmd [numero_comandos]={{"GAN",GAN},{"INP",INP},{"SIM",SIM},
	{"FRM",FRM},{"REC",REC},{"HLP",HLP},
	{"OKA",OKA},{"BLT",BLT},{"LUX",LUX},{"IMP",IMP},{"SDC",SDC}, {"IPD",IPD}, {"RST",RST} };
public:
 cmd_control extraeComando(String);
 codigos_cmd traduceComando(cmd_control);
 comandos_parser::codigos_cmd run_and_compare(String);
 

};


#endif


