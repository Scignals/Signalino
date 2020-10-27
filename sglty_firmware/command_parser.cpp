#include "command_parser.h" 
#include "Arduino.h"


comandos_parser::cmd_control comandos_parser::extraeComando(String texto){
     comandos_parser::cmd_control p1={"ERR",-1}; 

     texto.toUpperCase();
     texto.trim();
     if(texto.length()>20 || texto.length()<3){
        String("ERR").toCharArray(p1.cmd,4);
        return p1;
     }	
     texto.substring(0,3);
     texto.toCharArray(p1.cmd,4);
     p1.param=texto.substring(3,4).toInt();
     return p1;
 }



 comandos_parser::codigos_cmd comandos_parser::traduceComando(comandos_parser::cmd_control p_cmd ){
    for(auto i=0;i<numero_comandos;i++){
       if(!strcmp(p_cmd.cmd,tabla_cmd[i].cmd))return tabla_cmd[i].codigo;
    }
    return comandos_parser::codigos_cmd::ERR;
 }

 comandos_parser::codigos_cmd  comandos_parser::run_and_compare(String LineaOrdenes){
    // comandos_parser::cmd_control p1=extraeComando(thisChar);
    // codigos_cmd p2=traduceComando(p1);
    // return p2;
    return(traduceComando(extraeComando(LineaOrdenes)));
 }








