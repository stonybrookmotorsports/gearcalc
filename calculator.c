#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <termios.h>


double NDptoPd(int N, double D){
  return (double)N/D;
}

double plspeed(double D, double n){
  return M_PI * D * n / 12;
}

double torque(double P, double n){
  return 63000*P/n;
}

double tanforce(double P, double v){
  return 33000 * P / v;
}

double sizefactor(double Pd){
  if(Pd >= 5){return 1;}
  else if(Pd >= 4){return 1.05;}
  else if(Pd >= 3){return 1.15;}
  else if(Pd >= 2){return 1.25;}
  else{return 1.40;}
}

double pinionpropfactor(double F, double D){
  double ratio = F/D;
  if(ratio < 0.5){ratio = 0.5;}
  if(F <= 1){return ratio/10 - 0.025;}
  else{return ratio/10 - 0.0375 + 0.0125*F;}
}

double meshalignfactor(double F){
  return 0.0675 + 0.0128*F - 0.0000926 * pow(F,2.0);//PRECISION ENCLOSED
}

double loaddistfactor(double cpf, double cma){
  return 1+cpf+cma;
}

double rimfactor(double tR, double Pd){
  double ratio = tR/(2.25/Pd);
  if(ratio<1.2){return 1.6 * log(2.242/ratio);}
  else{return 1;}
}

double dynfactor(double a, double v){
  double B = 0.25 * pow(a-5,0.667);
  double C = 50 + 56 * (1.0 - B);
  return pow(C/(C+pow(v,0.5)),-B);  
}

double bsn(double Wt, double Pd, double F, double J, double Ko, double Ks, double Km, double KB, double Kv){
  return Ko*Ks*Km*KB*Kv*Wt*Pd/(F*J);
}

double reliabilityfactor(double re){
  if(re>=0.9999){return 1.5;}
  else if(re>=0.999){return 1.25;}
  else if(re>=0.99){return 1.00;}
  else{return 0.85;}
}

double numcycles(int L, double n, int q){
  return 60*L*n*q;
}

double cyclefactor(double Nc){
  return 1.3558*pow(Nc,-0.0178);
}

double abstress(double HB){
  return (0.102*HB+16.40) * 1000; 
}

double rabs(double abs, double Yn, double SF, double KR){
  return abs*Yn/(SF*KR);
}

int main(){
  //printf("GOOD MAWNING\n");

  //===========INPUTS============
  int N1 = 29;
  int N2 = 48;
  double D1 = 2.41038961;
  double D2 = 3.98961039;
  double n1 = 573.916;
  double n2 = 346.741;
  double P = 5;
  double fw = 0.5;
  int L = 1052;
  int q = 1;
  int A = 6;
  double re = 0.99;
  
  //==========TABLE VALUES/CONSTANTS===========
  double jp = 0.375;
  double jg = 0;
  double Ko = 2.75;
  double Ks = 0;
  double cpf = 0;
  double cma = 0;
  double Km = 0;
  double KB = 0;
  double B = 0;
  double C = 0;
  double Kv = 0;
  double stp = 0;
  double stg = 0;
  double Sf = 1;
  double KR = 0;
  double ncp = 0;
  double ncg = 0;
  double ynp = 0;
  double yng = 0;
  double HBguess = 375;  
  double sat = 0;
  double spatp = 0;
  double spatg = 0;
  
  double Pd=NDptoPd(N1, D1);
  double v=plspeed(D1, n1);
  double T=torque(P, n1);
  double Wt=tanforce(P,v);
  Ks = sizefactor(Pd);
  cpf = pinionpropfactor(fw,D1);
  cma = meshalignfactor(fw);
  Km = loaddistfactor(cpf,cma);
  KB = rimfactor(D1/2-1.25/Pd,Pd);
  Kv = dynfactor(A, v);
  stp = bsn(Wt, Pd, fw, jp, Ko, Ks, Km, KB, Kv);
  KR = reliabilityfactor(re);
  ncp = numcycles(L,n1,q);
  ynp = cyclefactor(ncp);
  sat = abstress(HBguess);
  spatp = rabs(sat, ynp, Sf, KR);
  
  printf("Diametral Pitch:%f\n", Pd);
  printf("Pitch Line Speed:%f\n", v);
  printf("Torque:%f\n", T);
  printf("Wt:%f\n", Wt);
  printf("Ks:%f\n", Ks);
  printf("cpf:%f\n", cpf);
  printf("cma:%f\n", cma);
  printf("Km:%f\n", Km);
  printf("KB:%f\n", KB);
  printf("Kv:%f\n", Kv);
  printf("stp:%f\n", stp);
  printf("KR:%f\n", KR);
  printf("ncp:%f\n", ncp);
  printf("ynp:%f\n", ynp);
  printf("sat:%f\n", sat);
  printf("spatp:%f\n", spatp);  
}
