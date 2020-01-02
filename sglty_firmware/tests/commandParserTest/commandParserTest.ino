#include <AUnit.h>
using namespace aunit;

#include <command_parser.h>

comandos_parser  parser1;





void setup() {
  Serial.begin(9600);
  while (!Serial) {  }
  delay(1500);
  
 
}

void loop() {
    TestRunner::run();
    delay(1);         
}


test(evaluaComando) {
struct {String linea;comandos_parser::codigos_cmd cod;bool respuesta;} datos_test[8]={
  {"frm",comandos_parser::codigos_cmd::FRM,1},
  {"frm",comandos_parser::codigos_cmd::GAN,0},
  {"gan",comandos_parser::codigos_cmd::FRM,0},
  {"gan",comandos_parser::codigos_cmd::GAN,1},
  {""   ,comandos_parser::codigos_cmd::ERR,1},
  {"F"  ,comandos_parser::codigos_cmd::ERR,1},
  {"FR" ,comandos_parser::codigos_cmd::ERR,1},
  {"FRX",comandos_parser::codigos_cmd::ERR,1}};
  
  
  for(int i=0;i<8;i++){
    if(datos_test[i].respuesta)
      assertEqual(parser1.run_and_compare(datos_test[i].linea),datos_test[i].cod);
    else
      assertNotEqual(parser1.run_and_compare(datos_test[i].linea),datos_test[i].cod);
      
    }  
    
}
