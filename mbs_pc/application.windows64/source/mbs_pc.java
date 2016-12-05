import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import java.util.*; 
import java.util.ArrayList; 
import java.util.Arrays; 
import java.io.FileWriter; 
import processing.serial.*; 
import javax.swing.JOptionPane; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class mbs_pc extends PApplet {

///////////////////////////////////////////////
//
// GUI for controlling the ADS1299-based signalino
//
//
/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

* Author: JABarios, agosto 2016 - enero 2017
* Modified:
* Company: ILSB, Spain. 
* ---------------------------------------------
* Requires ControP5 library
*/

///////////////////////////////////////////////









 

//For user input dialogs

int BAUD_RATE = 115200;//57600 115200 230400;//921600;//460800;//921600; //921600 for Teensy2/Teensy3/Leonardo 460800
int numCanales = 8; //number of channels to report
int anchoPantalla=1000;
int altoPantalla=600;

boolean modo_conectado = false;
boolean modo_test      = false;
boolean gGrabando      = false;
boolean gui_running    = true;

int serialPortNumber = 0; //set to 0 for automatic detection


// global vars containing the gui
ControlP5 gui;
// and the chart recorder
Chart ADS4ch;
// and the serial port
Serial port;      


//create a file first for recording signal    
File file;
FileWriter fw;
PrintWriter outputfile;

//global, usada por varias funciones (creo) 
int[]  lectura;   // Datos leidos en el puerto serie
  
    
public void setup() {
  
  rectMode(CENTER);

  println("Signalino, signal visor 0.3 (c) 2017 ILSB ");

  lectura=new int[numCanales];

  modo_conectado=false;
  if (!modo_test){
     serie_inicia();
     //pone el ads en modo 6 ( bytes openBCI ). antes manda un "oka" xq si no el arduino no responde
     sendComando("oka",port);
     sendComando("frm6",port);
  }   
  ADS4ch = new Chart(anchoPantalla/2-50,altoPantalla/2,anchoPantalla-100,altoPantalla-50,numCanales,0.01f);
  gui = new ControlP5(this);
  try {
    file = new File("./signalino_rawdata.txt");
    fw = new FileWriter(file);
    outputfile = new PrintWriter(fw);
  } catch (IOException e) {
    println("File error. Sorry...");
    e.printStackTrace();
  } 
  iniciaGui(gui);
  sendComando("sim1",port);
  println("llegamos");
  
}

public void draw() {
     if(modo_conectado)serDecode(ADS4ch.bf);
     else serRand(ADS4ch.bf);
     if(gui_running)ADS4ch.update();
     
     // esto hay q meterlo como metodo de gui2
       fill(0,0,0);
       rect(810,585,60,20);
       fill(255,255,255);
       text(fm_calculada+"  ",800,590); 
      
     
}
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
//  Copyright \u00a9 2016 JA Barios
//  This file is part of project: SCIGNALS: a chart recorder
//
class arr_int {
  int[] v1;
  int nelem;
  public arr_int(int n) {
    v1=new int[n];
    nelem=n;
  }
  
  public arr_int(int[] n) {
    v1=n;
    nelem=n.length;
  }

  public void set(int nn, int valor) {
    v1[nn]=valor;
  };
  public int  get(int nn) {
    return(v1[nn]);
  };
  public int length() {
    return(v1.length);
  };
  public int[] toVInt() {
    return(v1);
  };

  public int sum_x1() {
    int suma=0;
    for (int i=0; i<nelem; i++)suma += v1[i];
    return(suma);
  }
  
  public int sum_x1(int num_elem, int num_puntos) {
    int suma=0;
        for (int i=0; i<num_elem; i++)suma += v1[i];
        if(num_elem-num_puntos<0){
           for (int i=nelem+(num_elem-num_puntos); i<nelem; i++)suma += v1[i];
        }
        return(suma);
  }
  
  
  public int sum_x2() {
    int suma=0;
    for (int i=0; i<nelem; i++)suma += (v1[i]*v1[i]);
    return(suma);
  }
  public int avg() {
    return(sum_x1()/nelem);
  };
  public int std() {
    int mm=avg();
    return((int)sqrt(PApplet.parseInt(sum_x2() / nelem) - (mm * mm)));
  }
  
  public int var() {
    //algoritmo de welford
    //no testado
    int M=0;
    int S = 0;
    int x;
    int oldM;
    for( int k=1;k<nelem;k++) {
      x = v1[k];
      oldM = M;
      M += (x-M)/k;
      S += (x-M)*(x-oldM);
    }  
    return S/(nelem-1); 
  }
  
  
  
  public int[] offset() {
    int mm=avg();
      for (int i=0; i<nelem; i++)v1[i] -= mm;
    return(v1);
  }
 
   public int[] zscore() {
    int mm=avg();
    for (int i=0; i<nelem; i++)v1[i] -= mm;
    mm=std();
    if(mm>0)
    for (int i=0; i<nelem; i++)v1[i] /= mm;
    
    return(v1);
  }
 
  public int[] mov_avg(int npuntos) {
    double suma;
    suma=0;
    for (int i=nelem-npuntos; i>npuntos; i--){
       for (int j=0; j<npuntos; j++){
         suma+=v1[i-j];
       }
       v1[i]=(int)((suma)/npuntos);
       suma=0;
    }
    return(v1);
  }
 
 public int[] mov_avg_r(int npuntos) {
    double suma;
    int i; //esta vez se usa fuera del bucle, y la reusamos con cuidadito
    suma=0;
    for (i=nelem-1; i>nelem-npuntos; i--){
         suma+=v1[i];
       }
    v1[i]=(int)((suma)/npuntos);
    for (i=nelem-npuntos-1; i>npuntos; i--){
       v1[i]=v1[i+1]-(int)(v1[i-npuntos]/npuntos);
    }
    return(v1);
  }


  public int[] trocito(int principio, int fin) {
    int [] salida=new int[fin-principio+1];
    for (int i=0; i<(fin-principio+1); i++) {
      salida[i]=v1[principio+i];
    }
    return(salida);
  }

  // findpeaks simplificado, nivel sd*4/5, pone marcador en el maximo tras  pasar el nivel  y espera
  // para marcar otro hasta que la onda baje de nivel*2/3, pasado al menos "paso"
  // ahora funciona bien tambi\u00e9n con ondas cuadradas, y con sinusoidal lenta
  // apunta los tiempos en marcadores, que se pasa como parametro
  public int busca_picos(int paso, int[] marcadores) {
    int npicos=0;
    for (int x = 0; x < marcadores.length; x++) { 
      marcadores[x]=0;
    }
    int nivel=avg()+std()*4/5;
    for (int x = 0; x < length() && npicos<200; x++) { 
      if (v1[x]>nivel) {
        int maximo=v1[x];
        // ahora xfin avanza desde x+paso (paso es un "periodo refractario") hasta que encuentre un valor bajito, o hasta que se acabe el buffer
        int xfin=x+paso; 
        while(xfin<length() && v1[xfin%length()]>nivel*2/3){xfin++;};
        // en dicho intervalo, buscamos el maximo
        for (int y=x; y<xfin; y++) {
          if (v1[y%length()]>maximo) {
            maximo=v1[y%length()];
            x=y;
          }
        }  
        if(x<length())marcadores[npicos++]=x;
        x=xfin;
      }
    }
    return(npicos);
  }


  public double[] fft() {
    double[] vv1=new double[nelem]; // solo funciona si nelem es potencia de 2
    double[] vvceros=new double[nelem];
    // vamos a restar la media, es mejor en esta aplicacion
    int suma=0;
    for (int i=0; i<v1.length; i++) suma += v1[i];
    for (int i=0; i<nelem; i++)vv1[i]=(double)(v1[i]-(suma/v1.length));
    //fftpot2((vv1), (vvceros));
    fft512((vv1), (vvceros));

    return(vv1);
  }
  public double[] espectro() {
    // log del cuadrado de la fft, con la mitad de puntos
    double[] vv1;
    double[] vv2=new double[nelem/2];
    vv1=fft();   
    for (int i=0; i<vv1.length/2; i++) {
      vv2[i]=(double)Math.log((double)Math.pow((float)vv1[i], 2.0f));
    }
    return(vv2);
  }
}


/* 
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector's length must be a power of 2. Uses the Cooley-Tukey decimation-in-time radix-2 algorithm.
 * sacada del internet
 */
public static void fftpot2(double[] real, double[] imag) {
  // Initialization
  if (real.length != imag.length)
    throw new IllegalArgumentException("Mismatched lengths");
  int n = real.length;
  int levels = 31 - Integer.numberOfLeadingZeros(n);  // Equal to floor(log2(n))
  if (1 << levels != n)
    throw new IllegalArgumentException("Length is not a power of 2");
  double[] cosTable = new double[n / 2];
  double[] sinTable = new double[n / 2];
  for (int i = 0; i < n / 2; i++) {
    cosTable[i] = Math.cos(2 * Math.PI * i / n);
    sinTable[i] = Math.sin(2 * Math.PI * i / n);
  }

  // Bit-reversed addressing permutation
  for (int i = 0; i < n; i++) {
    int j = Integer.reverse(i) >>> (32 - levels);
    if (j > i) {
      double temp = real[i];
      real[i] = real[j];
      real[j] = temp;
      temp = imag[i];
      imag[i] = imag[j];
      imag[j] = temp;
    }
  }

  // Cooley-Tukey decimation-in-time radix-2 FFT
  for (int size = 2; size <= n; size *= 2) {
    int halfsize = size / 2;
    int tablestep = n / size;
    for (int i = 0; i < n; i += size) {
      for (int j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
        double tpre =  real[j+halfsize] * cosTable[k] + imag[j+halfsize] * sinTable[k];
        double tpim = -real[j+halfsize] * sinTable[k] + imag[j+halfsize] * cosTable[k];
        real[j + halfsize] = real[j] - tpre;
        imag[j + halfsize] = imag[j] - tpim;
        real[j] += tpre;
        imag[j] += tpim;
      }
    }
    if (size == n)  // Prevent overflow in 'size *= 2'
      break;
  }
}

int fft_iniciada;
double [] cosTable;
double [] sinTable;

// la fft de arriba, optimizada para buffer de 512. No comprueba tama\u00f1os de arrays, y precalcula las tablas. 
// usa un global fft_iniciada, que probablemente deberia ser un static, pero no controlo c\u00f3mo se hace
// esto deberia ser un metodo estatico, pero tampoco lo controlo, y da error de compilaci\u00f3n
// funciona con imag todo ceros
public  void fft512(double[] real, double[] imag) {
  int n = 512;
  int levels=9;
  if (fft_iniciada==0) {
    for (int i = 0; i < n / 2; i++) {
      cosTable[i] = Math.cos(2 * Math.PI * i / n);
      sinTable[i] = Math.sin(2 * Math.PI * i / n);
    }
    fft_iniciada=1;
  }  

  // Bit-reversed addressing permutation
  double temp=0;
  for (int i = 0; i < n; i++) {
    int j = Integer.reverse(i) >>> (32 - levels);
    if (j > i) {
      temp = real[i];
      real[i] = real[j];
      real[j] = temp;
      temp = imag[i];
      imag[i] = imag[j];
      imag[j] = temp;
    }
  }

  // Cooley-Tukey decimation-in-time radix-2 FFT
  for (int size = 2; size <= n; size *= 2) {
    int halfsize = size / 2;
    int tablestep = n / size;
    for (int i = 0; i < n; i += size) {
      for (int j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
        double tpre =  real[j+halfsize] * cosTable[k] + imag[j+halfsize] * sinTable[k];
        double tpim = -real[j+halfsize] * sinTable[k] + imag[j+halfsize] * cosTable[k];
        real[j + halfsize] = real[j] - tpre;
        imag[j + halfsize] = imag[j] - tpim;
        real[j] += tpre;
        imag[j] += tpim;
      }
    }
    if (size == n)  // Prevent overflow in 'size *= 2'
      break;
  }
}
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
//  Copyright \u00a9 2016 JA Barios
//  This file is part of project: SCIGNALS: a chart recorder
//
class Buffer {
  
  int [][] datos;
  int num_canales;
  int max_buffer;
  int puntero;
  int cte_tiempo;
  int p_cte_tiempo;
  int [] umbral_max;
  int [] umbral_min;
  int [][] offset;
  int [] sum_offset;
  
  
  
  Buffer(int nc , int mb ) {
        datos=new int [nc][mb] ;
        umbral_max=new int[nc];
        umbral_min=new int[nc];
        cte_tiempo=(int)gCteTiempo;  //numero magico, de momento. es mas o menos 1 segundo.
        offset=new int[nc][cte_tiempo];
        sum_offset=new int[nc];
        
        
        
        puntero=0;
        p_cte_tiempo=0;
        
        num_canales=nc;
        max_buffer=mb;        

        for(int i=0;i<nc;i++)umbral_max[i]=1000000000;
        for(int i=0;i<nc;i++)sum_offset[i]=0;
        
   
}
  
  public void graba(int[] x1) {
      
             int i=0;
             for (i = 0; i < numCanales-1; i++){
               outputfile.print(x1[i]);
               outputfile.print(',');
             }          
             outputfile.println(x1[i]);
  }  
  
  public void apunta(int[] x1) {
   
    if((++puntero)>=max_buffer)puntero=0;
    if((++p_cte_tiempo)>=cte_tiempo)p_cte_tiempo=0;
    int previo_offset;
    
    for(int i=0;i<num_canales;i++){
               //calculamos offset de los ultimos cte_tiempo puntos
               previo_offset=offset[i][p_cte_tiempo];               
               offset[i][p_cte_tiempo]=x1[i];
               sum_offset[i]+=(x1[i]-previo_offset);
               //apuntamos el dato con el offset ya restado
               datos[i][puntero]=x1[i]-sum_offset[i]/cte_tiempo;
             }      
   }


  public int [] lee_canal(int canal,int longitud, int decimando) {

    int longitud2=longitud*decimando;
    int [] c=new int [longitud2];
    int [] d=new int [longitud];
  
    if(puntero>longitud2)
        System.arraycopy(datos[canal], puntero-longitud2, c, 0, longitud2 );
    else{
        System.arraycopy(datos[canal], max_buffer+puntero-longitud2, c, 0, longitud2-puntero );
        System.arraycopy(datos[canal], 0, c, longitud2-puntero, puntero );
    }

    for(int i=0;i<longitud;i++)d[i]=c[i*decimando];
    
    //moving average de 3 puntos (despues de decimar)
    double suma,suma2;
    int npuntos=5;
    suma=0;suma2=0;
    if(1==0){
      //algoritmo "oficial", ineficaz, creo que se podr\u00e1 borrar
          for (int i=longitud-npuntos; i>npuntos; i--){
             for (int j=0; j<npuntos; j++){
                  suma+=d[i-j];
             }
             d[i]=(int)((suma)/npuntos);
             suma2+=d[i];
             suma=0;
          }
      } else {
      //algoritmo recursivo, operativo. muy chulo. 
      // adaptado de http://www.dspguide.com/CH15.PDF
      // el mio va marcha atras, y asi no hace falta un segundo buffer
      // i99=i98+i97+i96+i5 (moving avr de 4 puntos) --> i=((i+1)*n-i+(i-npuntos))/npuntos;
      // una formula para calcular frecuencia de corte aproximada es frec=fm*0.443/sqr(n\u00b2-1)
        int ii;
        for (ii=longitud-2; ii>longitud-npuntos-2; ii--){
             suma+=d[ii];
           }
        d[longitud-1]=(int)((suma)/npuntos);
        for (ii=longitud-3; ii>npuntos; ii--){
           d[ii+1]=(int)((npuntos*d[ii+2]-d[ii]+d[ii-npuntos+1])/npuntos);
           suma+=d[ii+1];
        }
    }
    
    return(d);    
     
  }

  public void limpia(){
        datos      = new int [num_canales][max_buffer] ;
        umbral_max = new int[num_canales];
        umbral_min = new int[num_canales];
        sum_offset = new int[num_canales];
        
        for(int i=0;i<num_canales;i++)sum_offset[i]=0;
        for(int i=0;i<num_canales;i++)for(int j=0;j<cte_tiempo;j++)offset[i][j]=0;
        
 }

  public void calcula_umbrales() {
    for(int i=0;i<num_canales;i++){
       for(int j=0;j<max_buffer;j++){
           umbral_max[i]+=abs(datos[i][j]);
      }
    }
    
    for(int i=0;i<num_canales;i++){
           umbral_max[i]/=max_buffer;
    }
  }
    
}  
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
//  Copyright \u00a9 2016 JA Barios
//  This file is part of project: SCIGNALS: a chart recorder
//
class Canal {
  
  int x;
  int y;
  int ancho;
  ventana v;
  float escala;  
  int decimando;
  Canal(int x,int y,int w,int h,float e) {
        v=new ventana(x,y,w,h);
        escala=e;
        decimando=64;
  }
  public void pinta(int[] x1) {
  //    v.pintaVector(new arr_int((new arr_int(x1)).mov_avg(3)).zscore(),escala);
        v.pintaVector(x1,escala);
    //   v.pintaVector((new arr_int(x1)).mov_avg(3),escala);
 
}
  
  public void xpinta(arr_int x1) {
      v.pintaVector(x1.mov_avg_r(3),escala);
  }
}  
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
//  Copyright \u00a9 2016 JA Barios
//  This file is part of project: SCIGNALS: a chart recorder
//
int MAX_DECIMANDO=64;
class Chart { 
  int ancho, alto, cx, cy;
  ArrayList<Canal> canales;
  int n_canales,ancho_canal, alto_canal;
  int puntero_canales;
  ventana v;
  int[][] datos;
  int[] visibles;

  Buffer bf;

  Chart (int x, int y, int w, int h, int n, float escala) {  
    ancho = w; 
    alto = h;
    cx=x;
    cy=y;
    v=new ventana(cx,cy,ancho,alto);
    n_canales=n;
    puntero_canales=0;

    canales = new ArrayList<Canal>();
    visibles= new int[n_canales];
    for(int i=0;i<n_canales;i++)visibles[i]=1;
    
    int alto_canal=alto/n_canales;
    ancho_canal=ancho-10;
    for(int i=0;i<n;i++){
      canales.add(new Canal(cx,v.mrgS()+3+alto_canal/2+(alto_canal)*i,ancho-10, alto_canal, escala));
    }
    
    bf=new Buffer(n,w*MAX_DECIMANDO);
    
    
  
   }
   
   public void set(int [] valores){
      bf.apunta(valores);
   }
 
  public void reset(){
      bf.limpia();
   }
 
  public void update() {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        if(visibles[i]==1)
          chx.pinta(bf.lee_canal(i,ancho,chx.decimando));
     }
//     bf.calcula_umbrales();

  } 
  public void setEscala(float e) {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        chx.escala=e;
     }
  }
  public void setDecimando(int e) {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        chx.decimando=e;
     }
  }
  
  public void setVisible(int e, int vis){
     visibles[e]=vis;       
  }
  
  
    

} 
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
//  Copyright \u00a9 2016 JA Barios
//  This file is part of project: SCIGNALS: a chart recorder
//
float escala_multiplicador=1;
float escala_base=0.00001f; //valor para q se vea simul
float gCteTiempo=64;
float escala_valor=1;
String nombre_archivo="signalino_raw";

DropdownList d1;
 StringList l;

public void iniciaGui(ControlP5 gui) { //assuming Arduino is only transfering active channels

  gui.addRadioButton("Senal")
   .setPosition(910,300)
   .setSize(10,27)
   .setValue(3)
   .addItem("senal",3)
   .addItem("test",2)
   .addItem("simul",1)
   
   ;

  gui.addTextlabel("Signalino")
   .setText("Signalino visor 0.3")
   .setPosition(10,2)
   .setSize(10,27)
   .setColorValue(0xffffff00)
   .setFont(createFont("Georgia",20))
   ;

   
  gui.addSlider("VisorGanancia")
   .setPosition(910,10)
   .setRange(1, 20)
   .setSize(10, 60)
   .setNumberOfTickMarks(10)
   .setValue(10)
   .setCaptionLabel("Ganancia")
     ;

  gui.addRadioButton("Escala")
   .setPosition(950,30)
   .setSize(27,10)
   .addItem("x1",1)
   .addItem("x10",2)
   .addItem("x100",3)
   .setValue(1)
   
   ;

 
  gui.addSlider("Tiempo")
   .setPosition(910,100)
   .setRange(1, 20)
   .setSize(80, 10)
   .setNumberOfTickMarks(10)
   .setValue(1)
   .setCaptionLabel("barrido")
     ;

  gui.addTextlabel("lab1")
   .setText("<esc> exit")
   .setPosition(10,580)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;

  gui.addTextlabel("lab2")
   .setText("<+/-> Ganancia")
   .setPosition(90,580)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;

  gui.addTextlabel("lab3")
   .setText("<space> Pausa")
   .setPosition(170,580)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;
   

 gui.addTextfield("NombreArchivo")
   .setPosition(905,520)
   .setSize(90,20)
//   .setFocus(true)
   .setFont(createFont("Georgia",12))
   .setColor(color(255,255,255))
   .setAutoClear(false)
   .setCaptionLabel("")
   ;

  gui.addToggle("grabando")
     .setCaptionLabel("Grabando")
     .setPosition(925,550)
     .setSize(50,20)
     .setValue(gGrabando)
     .setMode(ControlP5.SWITCH)
     .setColorActive(color(64,64,64))
     .setColorBackground(color(122,122,122))
     .setColorForeground(color(122,122,122))
     ;
     

  gui.addRadioButton("ADSGanancia")
                .setPosition(910, 150)
                .setColorForeground(color(120))
                .setColorActive(color(255))
                .setColorLabel(color(255))
                .setSize(8,10)
                .addItem("1", 0)
                .addItem("2", 1)
                .addItem("4", 2)
                .addItem("6", 3)
                .addItem("8", 4)
                .addItem("12", 5)
                .addItem("24", 6)
                
                ;

  gui.addRadioButton("Protocolo")
                .setPosition(950, 120)
                .setColorForeground(color(120))
                .setColorActive(color(255))
                .setColorLabel(color(255))
                .setSize(8,10)
                .setVisible(false)
                .addItem("p1", 0)
                .addItem("p2", 1)
                .addItem("p3", 2)
                .addItem("p4", 3)
                .addItem("p5", 4)
                .addItem("p6", 5)
                
                ;
  
   l=new StringList();
   l.append("Cyberamp 80"); //SRB2 min ganancia
   l.append("EMG");         //bipolar min gaancia
   l.append("EEG");         //bipolar max ganancia
   l.append("PSG");
   gui.addScrollableList("Entrada")
       .setPosition(200, 1)
      // .setSize(200, 20)
       .setBarHeight(20)
       .setItemHeight(20)
       .addItems(l.array())
       .setValue(0)
       .setType(ScrollableList.DROPDOWN) // currently supported DROPDOWN and LIST
       ;

   
 }
    

public void controlEvent(ControlEvent theEvent) {
//Is called whenever a GUI Event happened
}

public void Senal(int value){
  println("cambiando modo senal a "+value);
  switch(value){
     case 1:
       sendComando("sim1;",port); 
       break;
     case 2:
       sendComando("sim2;",port);
       break;
     case 3:
       sendComando("sim3;",port);
       break;  

  }
  ADS4ch.reset();
}

public void Escala(int value){
  println("cambiando modo escala a "+value);
  switch(value){
     case 1:
       escala_multiplicador=1;
       break;
     case 2:
       escala_multiplicador=10;
       break;
     case 3:
       escala_multiplicador=100;
       break;  

  }
   float valor = gui.getController("VisorGanancia").getValue();
     ADS4ch.setEscala(valor*escala_base*escala_multiplicador);

}

public void VisorGanancia(float value){
   ADS4ch.setEscala(value*escala_base*escala_multiplicador);
 // ADS4ch.setEscala(value*0.0001);
  // 0.0001 test, /10 simul, 
  
}




public void Tiempo(int value){
  ADS4ch.setDecimando(value);
}

public void ADSGanancia(int value){
       sendComando("gan"+(value+1),port); 
       println("gan"+(value+1));
}

public void Entrada(int value){
  
  println("cambiando Perif\u00e9rico de entrada a "+value);
  switch(value){
     case 0:
      sendComando("inp1",port); 
       break;
     case 1:
      sendComando("inp2",port); 
       break;
     case 2:
      sendComando("inp3",port); 
       break;  

  }
 
}

public void grabando(boolean value){
    gGrabando=value;
    println("grabando:"+gGrabando);
    nombre_archivo = gui.get(Textfield.class,"NombreArchivo").getText();
    println("grabando:"+nombre_archivo);
   
    if(gGrabando){
      if(nombre_archivo.length()<1)return;
       gui.getController("grabando").setCaptionLabel("grabando");
       gui.getController("grabando").setColorActive(color(0,255,0));
       try {
          file = new File(nombre_archivo);
          fw = new FileWriter(file);
          outputfile = new PrintWriter(fw);
       } catch (IOException e) {
          println("File error. Sorry...");
          e.printStackTrace();
      } 
    }
    
    else {
       gui.getController("grabando").setColorActive(color(255,0,0));
       gui.getController("grabando").setCaptionLabel("no");
    }
   
  }
  
public void Protocolo(int value){
  println("cambiando protocolo senal a "+value);
  if(value>0 && value<7){
       sendComando("frm"+(value),port); 
       println("frm"+(value+1));
  }
}

public void keyPressed() {
 if(key=='+') {
     float value=gui.getController("VisorGanancia").getValue();
     gui.getController("VisorGanancia").setValue(value+3);
 } 
if(key=='-') {
     float value=gui.getController("VisorGanancia").getValue();
     gui.getController("VisorGanancia").setValue(value-3);
 }
 if(key==' ') {
     gui_running=!gui_running;
 
 }
}
int v1=0;
boolean debugeando=false;
int gUltimotimestamp;
float fm_calculada;

public void serDecode(Buffer bf) { //assuming Arduino is only transfering active channels

  int packetBytes = 33; // modo openbci3
  int cabecero=2;
  byte[] rawData = new byte[10*packetBytes*2];
  
  byte[] localAdsByteBuffer = {0,0,0};

  int newlen = port.available();
  if (newlen < packetBytes) return;

      while(port.available()>=packetBytes){
           int nn=port.readBytesUntil(0xC0, rawData);
           if(nn!=packetBytes)continue;
 
           for (int i = 0; i < numCanales; i++){
                     localAdsByteBuffer[0]=rawData[cabecero+0+(i*3)];
                     localAdsByteBuffer[1]=rawData[cabecero+1+(i*3)];
                     localAdsByteBuffer[2]=rawData[cabecero+2+(i*3)];
                     lectura[i] = interpret24bitAsInt32(localAdsByteBuffer);
           }   
           localAdsByteBuffer[0]=rawData[cabecero+0+(numCanales*3)];
           localAdsByteBuffer[1]=rawData[cabecero+1+(numCanales*3)];
           localAdsByteBuffer[2]=rawData[cabecero+2+(numCanales*3)];
           int timestamp = interpret24bitAsInt32(localAdsByteBuffer);
           fm_calculada=floor(1000000/(timestamp-gUltimotimestamp+1));
           gUltimotimestamp=timestamp;
           //println(fm_calculada);
           bf.apunta(lectura);
           if(gGrabando)bf.graba(lectura);

       }
             
} //void serDecode

public int interpret24bitAsInt32(byte[] byteArray) {     
    //little endian
    int newInt = ( 
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



public void serRand(Buffer bf) { //numeros aleatorios
  int timestamp=0;
  for(int jj=1;jj<2;jj++){
      for(int i=0;i<numCanales;i++){
               lectura[i]=1000*(int) random(1,100);
      }
      timestamp = millis()*1000;
      gUltimotimestamp=timestamp;
              
      bf.apunta(lectura);
      if(gGrabando)bf.graba(lectura);
  }
  fm_calculada=floor(1000000/(timestamp-gUltimotimestamp+1));

} //void serRand

public void serEco() { //numeros aleatorios
    int newlen = port.available();
    if(newlen<1)return;
      String texto=port.readString();
      print(texto);
} //void serEco


public void serie_inicia()
{
  if (serialPortNumber == 0) {
    setPortNum();
  } else {
    print("Will attempt to open port "); println(serialPortNumber); 
    println(", this should correspond to the device number in this list:");
    println(Serial.list());
    println("Hint: if you set serialPortNumber=0 the program will allow the user to select from a drop down list of available ports");
  }
  try{
      port = new Serial(this, Serial.list()[serialPortNumber], BAUD_RATE);    
      //vaciamos el buffer del puerto y leemos hasta un 0xC0
     while (port.available() > 0)  port.read(); 
     // aqui es donde sospecho que se cuelga, cuando no encuentra un signalino
     byte[] basura=port.readBytesUntil(0xC0);
        modo_conectado=true;

  } catch (Exception e){ 
          javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Signalino v 0.3 (c) 2016</div>"+
           "<p>No devices detected: please check Arduino power and drivers. Offline mode...</p></html>");  
          modo_conectado=false;
    //exit();
  }
}

public void setPortNum() 
{
  String[] portStr = Serial.list();
  int nPort = portStr.length;
  serialPortNumber=-1;
  
  
  
  
   
//    List<String> new_portStr = Arrays.asList(portStr);
//    new_portStr.add("Test signal");
//    String[] temp_portStr = new String[new_portStr.size()];
//    portStr = new_portStr.toArray(temp_portStr);

   if (nPort < 1) {
      javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Signalino v 0.3 (c) 2016</div>"+
           "<p>No devices detected: please check Arduino power and drivers. Exiting program...</p></html>");  
      exit();    
      return;
   }
   int index = 0;
   for (int i=0; i<nPort; i++) {
     // esta linea parece una fineza, apareceria resaltado si se llamara cu.us. El problema es q en windows se llama com1 , y en mi linux se llama acm1, y ...
     // pero la mantengo, por si algun dia se me ocurre algo mejor
     if (match(portStr[i], "cu.us") != null) index = i; //Arduino/Teensy names like /dev/cu.usbmodem*" and our BlueTooth is  /dev/cu.us922k0000bt 
     portStr[i] =  i+ " "+portStr[i] ;
   }
   
  String[] array2 = new String[]{portStr.length+" SIMULATED noisy Signal"};
  String[] array = new String[portStr.length + array2.length];
  System.arraycopy(portStr, 0, array, 0, portStr.length);
  System.arraycopy(array2, 0, array, portStr.length, array2.length);
  nPort=nPort+1;
  
   String respStr = (String) JOptionPane.showInputDialog(null,
      "<html><div align='center'>Scignals v 0.3 (c) 2016</div>"+
      "<p>This program is free software; is distributed under GNU General Public License in the hope that it will be useful, but WITHOUT ANY WARRANTY</p>"+
      "<p align='center'>Choose COM port (if not listed: check drivers and power)</p></html>", "Select Signalino device",
      JOptionPane.PLAIN_MESSAGE, null,
      array, array[index]);
      println(respStr);
      try{
         if(respStr!=null) serialPortNumber = Integer.parseInt(respStr.substring(0, 1));
      } catch (Exception e){ 
         // no hacer nada, es por si acaso
      }    
       
} //setPortNum()

public void sendComando(String comando, Serial puerto)
{
   if(puerto!=null){ 
     delay(200); // incomprensiblemente imprescindible, debe ser un bug de alguna libreria. Y me ha costado un huevo descubrirlo (windows 10, pc de elche).
     puerto.write(comando+";");
   }  
 
}
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
//  Copyright \u00a9 2016 JA Barios
//  This file is part of project: SCIGNALS: a chart recorder
//
class ventana {
  int _ctro_x;public int cx(){return(_ctro_x);}
  int _ctro_y;public int cy(){return(_ctro_y);}
  int _mrgI;public int mrgI(){return(_mrgI);}
  int _mrgD;public int mrgD(){return(_mrgD);}
  int _mrgS; public int mrgS(){return(_mrgS);}
  int _mrgF;public int mrgF(){return(_mrgF);}
  int _ancho;public int ancho(){return(_ancho);}
  int _alto;public int alto(){return(_alto);}
  int _fondo;public int fondo(){return(_fondo);}
  

  // constructor, calcula coordenadas y pinta fondo de color
  ventana(int p_cx, int p_cy, int p_ancho, int p_alto) {
    _ctro_x=p_cx; _ctro_y=p_cy;
    _ancho=p_ancho;_alto=p_alto;
    _mrgI=_ctro_x-(_ancho/2);
    _mrgD=_ctro_x+_ancho/2;
    _mrgS=_ctro_y-_alto/2;
    _mrgF=_ctro_y+_alto/2;
    _fondo = color(255, 253, 248);

    dibuja_fondo();
    }
  
  // dibuja de color _fondo la ventana
  public void dibuja_fondo(){
      noStroke();
      fill(_fondo);  // color for the window background
      rect(_ctro_x, _ctro_y, _ancho, _alto);
      
  }
  // pinta cursor, si n<0, con valor relativo (-0.5, justo en la mitas), si n>0 en cx=n 
  public void cursor(float n){
     fill(255, 0, 0);  // color for the window background
     if(n<0){
       float cx=(float)_ancho*n;
       rect((int)cx,_mrgI, 1,_alto );
     } else {
       rect(n,_ctro_y, 1,_alto );
     }
     
  }
  
    // 4 funciones, cambiando el tipo de los parametros de entrada, que pintan un vector en la ventana
    // incluyen limpiar el fondo antes de dbujar
    public void pintaVector(int[] x1,float escala) {
      float cx1,cy1;
      dibuja_fondo();
 //     rectMode(CORNER);
      stroke(250, 0, 0);     //red                          
      noFill();
      beginShape();        
      cx1=0;
      for (int i = 0; i < x1.length; i++) {
        cx1=(float)map(i,0,x1.length,mrgI(),mrgD());
        cy1=cy()+escala*x1[i];
        cy1=cy1<mrgS()?mrgS():cy1;
        vertex(cx1,  cy1>mrgF()?mrgF():cy1);
        cy1=cy1<mrgS()?mrgS():cy1;
 //       vertex(cx1,  cy1);
       vertex(cx1,  cy1>=mrgF()-1?mrgF():cy1);
       // las lineas q apaecen abajo oblablemente tengan q ver con esto
    
  }
      endShape();
//      rectMode(CENTER);

  }



}
  public void settings() {  size(1000, 600); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "mbs_pc" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
