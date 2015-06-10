#include "Integrators.h"
#include <vecManip.h>
#include <vector>

#define GM (-6.67384e-11*5.972e24)

using namespace std;

template<typename Vector, typename Scalar>
struct DnCA {
	const Vector accel;
	const Scalar drag;

DnCA(const Vector &accel, double drag) :
	accel(accel), drag(drag){
}
Vector operator()(const Vector &xv, const Vector &dxvdt, Scalar t){
		for(int i(0);i<6;++i){
			vel[i]=0;
		}
		
		// Pos[1]  =  Vel[0]
		dxvdt[0] = xv[3];
		dxvdt[1] = xv[4];
		dxvdt[2] = xv[5];
		
		double r = mag(xv);
		double fNet = GM/(r*r*r);
		
		dxvdt[3] = fNet * xv[0];
		dxvdt[4] = fNet * xv[1];
		dxvdt[5] = fNet * xv[2];
	}
};
 
void simLoop() {
    IntegratorRK4 integrator;
 
    // 1D particle states (positions and velocities) & buffers for next state
    std::vector<double> poss, vels, nextPoss, nextVels;
 
    // system time state and timestep size
    double t = 0.f, dt = 1.f / 100;
	double vect = {0,0,0}
    while (true) {
        // integrate accelerations to velocities & velocities to positions
        for (size_t i = 0; i < poss.size(); i++) {
            integrator.evaluate(poss[i], vels[i],DnCA<vector<double>, vector<double> >(1,0.25), t, dt, nextPoss[i], nextVels[i]);
        }
        // take a timestep
        t += dt;
        // advance state to the next state
        poss.swap(nextPoss);
        vels.swap(nextVels);
    }
}


int main(){
	
	
	return 0;
	
}