#include <vecManip.h>
#include <vector>
#include <iostream>
#include <fstream>

#define GM (-6.67384e-11*5.972e24)
#define moon_rad 373333
#define earth_rad 933333

using namespace std;

vector<double> accelFunc(vector<double> pos, vector<double> vel, double t){
	
	vector<double> dvdt(3);
	
	for(int i(0);i<3;++i) dvdt[i]=0;
	
	double r = mag(pos);
	double fNet = GM/(r*r*r);
	
	dvdt[0] = fNet * pos[0];
	dvdt[1] = fNet * pos[1];
	dvdt[2] = fNet * pos[2];
	
	return dvdt;
}

void evaluate(
	vector<double> &posIn,
	vector<double> &velIn,
	double t,
	double dt,
	vector<double> &posOut,
	vector<double> &velOut)
	{
		double halfDt = dt * (double)(0.5);
		double tMid = t + halfDt;
		vector<double> dVelIn = accelFunc(posIn, velIn, t);
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
		velOut = VecAdd(velIn, VecScaleMulti(VecScaleMulti(VecAdd(dVelD, VecAdd(dVelIn, VecScaleMulti(VecAdd(dVelB, dVelC), (double)(2.0)))),(double)(1.0/6.0)), dt));
		
		//posOut = posIn + dt * double(1.0 / 6.0) * (velIn + double(2.0) * (velB + velC) + velD);
		posOut = VecAdd(posIn, VecScaleMulti(VecScaleMulti(VecAdd(velD, VecAdd(velIn, VecScaleMulti(VecAdd(velB, velC),(double)(2.0)))), (double)(1.0/6.0)), dt));
	}

int main(){
	
	vector<double> pos(3), vel(3), posNew(3), velNew(3);
	double t = 0, dt = 0.1;
	
	long double counter(0.0);
	
	ofstream fout("MoonPos.txt");
	
	pos = { 7e6, 0, 0 };
	vel = { 0, 7545.6867982406, 0};
	
/* 	7545.6867982406  */
	
	while(1){
	
 		if(counter >= 1000000){
  			fout.close();
  			break;
  		}
 
		
		if(mag(pos) <= moon_rad+earth_rad){
			cout << "\tMoon has struck earth\n";
			break;
		}

		evaluate(pos, vel, t, dt, posNew, velNew);
	
		//cout << "First Pos: <" << pos[0] << ", " << pos[1] << ", " << pos[2] << "> \n";
		
		//cout << "New Pos: <" << posNew[0] << ", " << posNew[1] << ", " << posNew[2] << "> \n";
	
		fout <<  posNew[0] << " " << posNew[1] << " " << posNew[2] << " 0 0 0 " << velNew[0] << " " << velNew[1] <<  " " << velNew[2] << '\n';
	
		for(int i(0); i<3;++i){
			pos[i] = posNew[i];
			vel[i] = velNew[i];
		}
	
		counter++;
	}
	
	return 0;
	
}