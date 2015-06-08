#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long double mag(long double *dVec){
  return (sqrt(dVec[0]*dVec[0] + dVec[1]*dVec[1] + dVec[2]*dVec[2]));
}

long double *toUnitVec(long double *dVec){
  long double *uVec = (long double*)malloc(3*sizeof(long double));
  
  uVec[0] = dVec[0] / mag(dVec);
  uVec[1] = dVec[1] / mag(dVec);
  uVec[2] = dVec[2] / mag(dVec);
  
  return uVec;
}

int main(void){
  long double *dVec = (long double*)malloc(3*sizeof(long double));
  long double *uVec = (long double*)malloc(3*sizeof(long double));
  
  dVec[0] = 1;
  dVec[1] = 2;
  dVec[2] = 2;
  
  printf("\nOriginal Vector: <%Lf, %Lf, %Lf>\n", dVec[0], dVec[1], dVec[2]);
  
  printf("Magnitude: %Lf\n", mag(dVec));
  
  uVec = toUnitVec(dVec);
  printf("Unit Vector: <%Lf, %Lf, %Lf>\n", uVec[0], uVec[1], uVec[2]);
  
  free(dVec);
  free(uVec);
  
  return 0;
}