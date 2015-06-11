#include "Integrators.h"
#include <vecManip.h>
#include <vector>

#define GM (-6.67384e-11*5.972e24)

using namespace std;

template<typename Vector, typename Scalar>
struct DragAndConstAccel {
    const Vector accel;
    const Scalar drag;
 
    DragAndConstAccel(const Vector &accel, double drag) :
            accel(accel), drag(drag) {
    }
 
    Vector operator()(const Vector &pos, const Vector &vel, Scalar t) {
        return accel - vel * drag;
    }
};

/* DragAndConstAccel(const Vector &pos, const Vector &vel, Scalar t) {
 * 	return accel - vel * drag
 * }
 */
 
void simLoop() {
    IntegratorRK4 integrator;
    vector<double > poss, vels, nextPoss, nextVels;
    double t = 0.0, dt = 1.0 / 100;
    while (true) {
        for (size_t i = 0; i < poss.size(); i++) {
            integrator.evaluate(poss[i],
                    vels[i],
                    DragAndConstAccel<vector<double>, vector<double> >(1.0, 0.25),
                    t,
                    dt,
                    nextPoss[i],
                    nextVels[i]);
        }
        t += dt;
        poss.swap(nextPoss);
        vels.swap(nextVels);
    }
}

/* 
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
*/


int main(){
	
	DragAndConstAccel<vector<double>, vector<double> > DACA;
	
	
	simLoop();
	return 0;
	
}