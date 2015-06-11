#include "Integrators.h"
#include <vecManip.h>
#include <vector>
#include <iostream>
#include <fstream>

#define GM (-6.67384e-11*5.972e24)

using namespace std;

vector<double> accelFunc(vector<double> state, double t){
	
	vector<double> dxvdt(6);
	
	for(int i(0);i<6;++i){
		dxvdt[i]=0;
	}
	
	// Pos[1]  =  Vel[0]
	dxvdt[0] = state[3];
	dxvdt[1] = state[4];
	dxvdt[2] = state[5];
	
	double r = mag(state);
	double fNet = GM/(r*r*r);
	
	dxvdt[3] = fNet * state[0];
	dxvdt[4] = fNet * state[1];
	dxvdt[5] = fNet * state[2];
	
	return dxvdt;
}

void evaluate(
	vector<double> &stateIn,
	double t,
	double dt,
	vector<double> &stateOut)
	{
		double halfDt = dt * double(0.5);
		double tMid = t + halfDt;
		
		vector<double> dVelIn = accelFunc(posIn, velIn, t); //vector<double> dVelIn = accelFunc(stateIn, t)
		
		vector<double> velB = VecAdd(velIn, VecScaleMulti(dVelIn, halfDt)); // velB = velIn + dVelIn * halfDt;
		vector<double> posB = VecAdd(posIn, VecScaleMulti(velIn, halfDt)); //posIn + velIn * halfDt;
		
		
		
		vector<double> dVelB = accelFunc(posB, velB, tMid);
		
		vector<double> velC = VecAdd(velB, VecScaleMulti(dVelB, halfDt)); // velB + dVelB * halfDt;
		vector<double> posC = VecAdd(posB, VecScaleMulti(velB,halfDt)); // posB + velB * halfDt;
		
		
		
		vector<double> dVelC = accelFunc(posC, velC, tMid);
		
		vector<double> velD = VecAdd(velC, VecScaleMulti(dVelC, dt)); // velC + dVelC * dt;
		vector<double> posD = VecAdd(posC, VecScaleMulti(velC, dt)); // posC + velC * dt;
		
		
		
		vector<double> dVelD = accelFunc(posD, velD, t + dt);
		
		
		//velOut = velIn + dt * double(1.0 / 6.0) * (dVelIn + double(2.0) * (dVelB + dVelC) + dVelD);
		velOut = VecAdd(velIn, VecScaleMulti(VecScaleMulti(VecAdd(dVelD, VecAdd(dVelIn, VecScaleMulti(VecAdd(dVelB, dVelC), double(2.0)))), double(1.0/6.0)), dt));
		//posOut = posIn + dt * double(1.0 / 6.0) * (velIn + double(2.0) * (velB + velC) + velD);
		posOut = VecAdd(posIn, VecScaleMulti(VecScaleMulti(VecAdd(velD, VecAdd(velIn, VecScaleMulti(VecAdd(velB, velC), double(2.0)))), double(1.0/6.0)), dt));
	}

int main(){
	
	vector<double> stateNew(6);
	vector<double> state(6);
	double t=0,dt=1;
	
	long double counter(0.0);
	
	ofstream f("MoonPos.txt");
	
	state = { 7e6, 0, 0, 0, 7545.6867982406, 0};
	
	while(1){
	
 		if(counter>=86400.0*7){
 			f.close();
 			break;
 		}

		
	evaluate(state,t,dt,stateNew);
	
	//cout << "First Pos: <" << pos[0] << ", " << pos[1] << ", " << pos[2] << "> \n";
		
	//cout << "New Pos: <" << posNew[0] << ", " << posNew[1] << ", " << posNew[2] << "> \n";
	
	f <<  stateNew[0] << " " << stateNew[1] << " " << stateNew[2] << " 0 0 0\n";
	
	for(int i(0); i<6;++i){
		state[i] = stateNew[i];
	}
	
	
	
	counter++;
}
	return 0;
	
}