/*
 *  Copyright (C) 2012 Xo Wang
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL XO
 *  WANG BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 *  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  Except as contained in this notice, the name of Xo Wang shall not be
 *  used in advertising or otherwise to promote the sale, use or other dealings
 *  in this Software without prior written authorization from Xo Wang.
 */

#ifndef INTEGRATORS_H_
#define INTEGRATORS_H_

template<typename Derived>
class Integrator {
public:
    virtual ~Integrator(void) {
    }

    /**
     * Interface method for numerical integration of physics. "Vector" template
     * type does not need to actually be a vector type; in fact a float works
     * just fine. It does need to support the + and * operators for element-wise
     * addition bzw. scalar multiplication, however.
     *
     * This is an interface declaration in CRTP style:
     *   http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
     *
     * @param posIn input positions
     * @param velIn input velocities
     * @param accelFunc force evaluation function/functor matching the concept
     *      Vector accelFunc(const Vector &pos, const Vector &vel, Scalar t);
     * @param posOut output positions
     * @param velOut output velocities
     */
    template<typename Vector, typename Scalar, typename ForceFunc>
    void evaluate(const Vector &posIn,
            const Vector &velIn,
            ForceFunc accelFunc,
            Scalar t,
            Scalar dt,
            Vector &posOut,
            Vector &velOut) const {
        static_cast<Derived *>(this)->evaluate(posIn, velIn, accelFunc, posOut, velOut);
    }
};

class IntegratorEuler: Integrator<IntegratorEuler> {
public:
    template<typename Vector, typename Scalar, typename ForceFunc>
    void evaluate(const Vector &posIn,
            const Vector &velIn,
            ForceFunc accelFunc,
            Scalar t,
            Scalar dt,
            Vector &posOut,
            Vector &velOut) const {
        velOut = velIn + accelFunc(posIn, velIn, t) * dt;
        posOut = posIn + velIn * dt;
    }
};

class IntegratorMidpoint: Integrator<IntegratorMidpoint> {
public:
    template<typename Vector, typename Scalar, typename ForceFunc>
    void evaluate(const Vector &posIn,
            const Vector &velIn,
            ForceFunc accelFunc,
            Scalar t,
            Scalar dt,
            Vector &posOut,
            Vector &velOut) const {
        const Scalar halfDt = dt * Scalar(0.5);
        const Scalar tMid = t + halfDt;
        const Vector dVelIn = accelFunc(posIn, velIn, t);
        const Vector velMid = velIn + dVelIn * halfDt;
        const Vector posMid = posIn + velIn * halfDt;
        velOut = velIn + accelFunc(posMid, velMid, tMid) * dt;
        posOut = posIn + velMid * dt;
    }
};

class IntegratorRK4: Integrator<IntegratorRK4> {
public:
    template<typename Vector, typename Scalar, typename ForceFunc>
    void evaluate(
	    const Vector &posIn,
            const Vector &velIn,
            ForceFunc accelFunc,
            Scalar t,
            Scalar dt,
            Vector &posOut,
            Vector &velOut) 
	const {
        const Scalar halfDt = dt * Scalar(0.5);
        const Scalar tMid = t + halfDt;
        const Vector dVelIn = accelFunc(posIn, velIn, t);
        const Vector velB = velIn + dVelIn * halfDt;
        const Vector posB = posIn + velIn * halfDt;
        const Vector dVelB = accelFunc(posB, velB, tMid);
        const Vector velC = velB + dVelB * halfDt;
        const Vector posC = posB + velB * halfDt;
        const Vector dVelC = accelFunc(posC, velC, tMid);
        const Vector velD = velC + dVelC * dt;
        const Vector posD = posC + velC * dt;
        const Vector dVelD = accelFunc(posD, velD, t + dt);
        velOut = velIn + dt * Scalar(1.0 / 6.0) * (dVelIn + Scalar(2.0) * (dVelB + dVelC) + dVelD);
        posOut = posIn + dt * Scalar(1.0 / 6.0) * (velIn + Scalar(2.0) * (velB + velC) + velD);
    }
};

#endif /* INTEGRATORS_H_ */
