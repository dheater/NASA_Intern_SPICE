#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "/SPICE/cspice/include/SpiceUsr.h"
#include "/SPICE/cspice/src/cspice/furnsh_c.c"

#define SIZE 100
#define G -6.67384e-11

void loadKernel();
long double mag(long double vec[3]);
long double *toUnitVec(long double *dVec);


int main(){
	
	printf("\n\n\tTwo Body Orbital System Simulation\n\n");
	
	//loadKernel();
	
	long double 
	mE		= 5.97e24,
	mM		= 5000,
	
	radE		= 6371,
	radM	= 1737,
	
	pE[3]	= {0*mE,80*mE,20*mE},
	pM[3]	= {0*mM,51.512*mM,0*mM},
	
	posE[3]	= {0,0,0},
	posM[3]	= {1.5e11,0,0},
	
	distVec[3]= {posM[0]-posE[0], posM[1]-posE[1], posM[2]-posE[2]},
	
	fNet[3];
	
	long double *unitVec = (long double * )malloc(3*sizeof(long double));
	
	printf(" Earth Position: <%.2e,%.2e,%.2e>", posE[0], posE[1], posE[2]);
	printf(" Moon Position: <%.2e,%.2e,%.2e>", posM[0], posM[1], posM[2]);
	printf(" Distance: <%.2e,%.2e,%.2e>", distVec[0], distVec[1], distVec[2]);
	
	long double
	
	t = 1,
	
	dt = (86400*7);
	
	printf("\n mE\t%Lf \n radE\t%Lf \n mM\t%Lf \n radM\t%Lf \n radDif\t%Lf \n",mE,radE,mM,radM,mag(distVec));
	
	printf("\n Magnitude:\n\t%Lf",mag(distVec));
	
	printf("\n Net Force:\t");
	
	unitVec=toUnitVec(distVec);
	
	for(int i=0;i<3;++i){
		
		fNet[i] = ((G*mE*mM)/(mag(distVec)*mag(distVec)))*(unitVec[i]);
		
		printf("\n\tfNet[%d]: %Lf", i, fNet[i]);
		
	}
	
	FILE *fp;
	fp = fopen("MoonPos.txt", "w+");
	
	int userIn=0;
	
	printf("\n\nPlease input number of weeks to run the sim: ");
	scanf("%d",&userIn);
	
	while(t>0){
		
		for(int i=0;i<3;++i){
			fNet[i] = ((G*mE*mM)/(mag(distVec)*mag(distVec)))*(unitVec[i]);
			
			pE[i]+=(fNet[i]*dt);
			pM[i]+=(fNet[i]*dt);
			
			posE[i]+=(pE[i]/mE)*dt;
			posM[i]+=(pM[i]/mM)*dt;
			
			distVec[i] = posM[i] - posE[i];
			
			unitVec[i]=distVec[i]/mag(distVec);
		}
		
		t+=dt;
		
		//printf("\n\tMoon Pos: <%Lf, %Lf, %Lf> ", posM[0], posM[1], posM[2]);
		//printf("\n\tEarth Pos: <%Lf, %Lf, %Lf>", posE[0], posE[1], posE[2]);
		//printf("\n\tDistance: %Lf %Lf %Lf", distVec[0], distVec[1], distVec[2]);
		//printf("\n\tUnit Vec: %Lf %Lf %Lf", unitVec[0],unitVec[1],unitVec[2]);
		fprintf(fp, "%Lf %Lf %Lf %Lf %Lf %Lf\n", posM[0], posM[1], posM[2], posE[0], posE[1], posE[2]);
		//printf("\n\tFnet: %Lf %Lf %Lf", fNet[0], fNet[1], fNet[2]);
		//printf("\n\tMoon P: %Lf %Lf %Lf\n\n", pM[0], pM[1], pM[2]);
		
		
		if(t>dt*userIn){
			break;
		}
		
	}
	
	printf("\n UnitVector:\n");
	
	for(int i=0;i<3;++i){
		printf("\t%Lf \n",unitVec[i]);
	}
	
	fclose(fp);
	
	free(unitVec);
	
	return 0;
}

void loadKernel(){
	
	SpiceChar str[SIZE];
	
	prompt_c("Enter setup file name: ", SIZE, str);
	
	if(str[0]=='q'){
		printf("\n\n\tMeta-Kernel KF.txt is loading to kernel pool...\n");
		furnsh_c("KF.txt");
	}else{
		printf("\n\n\t%s is loading to kernel pool...\n\n", str);
		furnsh_c(str);
	}
	
	return;
	
}

long double mag(long double vec[3]){
	return (sqrt((vec[0]*vec[0])+(vec[1]*vec[1])+(vec[2]*vec[2])));
}

long double *toUnitVec(long double *dVec){
  long double *uVec = (long double*)malloc(3*sizeof(long double));
  
  uVec[0] = dVec[0] / mag(dVec);
  uVec[1] = dVec[1] / mag(dVec);
  uVec[2] = dVec[2] / mag(dVec);
  
  return uVec;
}

/*
while t >= 0:
    rate(100)
    fnet = ((-1*G*ME*MS)/((mag(earth.pos-sun.pos))**2))*((earth.pos-sun.pos)/mag(earth.pos-sun.pos))
    earth.p += fnet * dt
    sun.p += fnet * dt
    earth.pos += (earth.p/ME) * dt
    sun.pos += (sun.p/MS) * dt
    
    p.pos = earth.pos
    f.pos = earth.pos
    p.axis = earth.p * 4e-19
    f.axis = fnet * 2e-12
    
    earth.trail.append(pos = earth.pos)
    sun.trail.append(pos = sun.pos)
    print("Pos:", earth.pos)
    
    t += dt
*/