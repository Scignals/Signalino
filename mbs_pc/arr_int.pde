// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
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
//  Copyright © 2016 JA Barios
//  This file is part of project: SCIGNALS, a chart recorder.
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

  void set(int nn, int valor) {
    v1[nn]=valor;
  };
  int  get(int nn) {
    return(v1[nn]);
  };
  int length() {
    return(v1.length);
  };
  int[] toVInt() {
    return(v1);
  };

  int sum_x1() {
    int suma=0;
    for (int i=0; i<nelem; i++)suma += v1[i];
    return(suma);
  }
  
  int sum_x1(int num_elem, int num_puntos) {
    int suma=0;
        for (int i=0; i<num_elem; i++)suma += v1[i];
        if(num_elem-num_puntos<0){
           for (int i=nelem+(num_elem-num_puntos); i<nelem; i++)suma += v1[i];
        }
        return(suma);
  }
  
  
  int sum_x2() {
    int suma=0;
    for (int i=0; i<nelem; i++)suma += (v1[i]*v1[i]);
    return(suma);
  }
  int avg() {
    return(sum_x1()/nelem);
  };
  int std() {
    int mm=avg();
    return((int)sqrt(int(sum_x2() / nelem) - (mm * mm)));
  }
  
  int var() {
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
  
  
  
  int[] offset() {
    int mm=avg();
      for (int i=0; i<nelem; i++)v1[i] -= mm;
    return(v1);
  }
 
   int[] zscore() {
    int mm=avg();
    for (int i=0; i<nelem; i++)v1[i] -= mm;
    mm=std();
    if(mm>0)
    for (int i=0; i<nelem; i++)v1[i] /= mm;
    
    return(v1);
  }
 
  int[] mov_avg(int npuntos) {
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
 
 int[] mov_avg_r(int npuntos) {
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


  int[] trocito(int principio, int fin) {
    int [] salida=new int[fin-principio+1];
    for (int i=0; i<(fin-principio+1); i++) {
      salida[i]=v1[principio+i];
    }
    return(salida);
  }

  // findpeaks simplificado, nivel sd*4/5, pone marcador en el maximo tras  pasar el nivel  y espera
  // para marcar otro hasta que la onda baje de nivel*2/3, pasado al menos "paso"
  // ahora funciona bien también con ondas cuadradas, y con sinusoidal lenta
  // apunta los tiempos en marcadores, que se pasa como parametro
  int busca_picos(int paso, int[] marcadores) {
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


  double[] fft() {
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
  double[] espectro() {
    // log del cuadrado de la fft, con la mitad de puntos
    double[] vv1;
    double[] vv2=new double[nelem/2];
    vv1=fft();   
    for (int i=0; i<vv1.length/2; i++) {
      vv2[i]=(double)Math.log((double)Math.pow((float)vv1[i], 2.0));
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

// la fft de arriba, optimizada para buffer de 512. No comprueba tamaños de arrays, y precalcula las tablas. 
// usa un global fft_iniciada, que probablemente deberia ser un static, pero no controlo cómo se hace
// esto deberia ser un metodo estatico, pero tampoco lo controlo, y da error de compilación
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