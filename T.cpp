#include <iostream>
#include <cmath>
#include <ODE_Boost.hpp>

#define GM (-6.67384e-11 * 5.972e24)

using namespace std;
using namespace boost::numeric::odeint;

/* we solve the simple ODE x' = 3/(2t^2) + x/(2t)
 * with initial condition x(1) = 0.
 * Analytic solution is x(t) = sqrt(t) - 1/t
 */

void rhs(const double x, double &dxdt, const double t) {
    dxdt = 3 / (2 * t * t) + x / (2 * t);
}

void write_cout(const double &x, const double t) {
    cout << t << '\t' << x << endl;
}

double mag(double vec[6]) {
    return (sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2])));
}

void dxvdt(double *xv, double *dxvdt) {

    // Zeroes dxvdt
    for(int i(0); i < 6; ++i) {
        dxvdt[i] = 0;
    }

    // Pos[1]  =  Vel[0]
    dxvdt[0] = xv[3];
    dxvdt[1] = xv[4];
    dxvdt[2] = xv[5];

    double r = mag(xv);
    double fNet = GM / (r * r * r);

    dxvdt[3] = fNet * xv[0];
    dxvdt[4] = fNet * xv[1];
    dxvdt[5] = fNet * xv[2];
}

// state_type = double
typedef runge_kutta_dopri5<double> stepper_type;

int main() {
    double x = 0.0;
    double hold[6], HATE[6] = { 1, 2, 3, 4, 5, 6 };
    dxvdt(HATE, hold);

    cout << "hold:\n\t< " << hold[0] << " , " << hold[1] << " , " << hold[2]
         << endl;
    cout << "\t  " << hold[3] << " , " << hold[4] << " , " << hold[5] << endl;

    integrate_adaptive(make_controlled(1E-12, 1E-12, stepper_type()), rhs, x,
                       1.0, 10.0, 0.1, write_cout);

    // integrate_adaptive( make_controlled( 1E-12 , 1E-12 , stepper_type() )
    // ,
    // dxvdt , HATE, 1.0 , 10.0 , 0.1 , write_cout );
}