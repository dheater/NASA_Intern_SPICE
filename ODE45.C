/*

   ODE45.C	MEX file implementation of:

	[tout, yout] = ode45(F, t0, tfinal, y0, tol)

   Integrate a system of ordinary differential equations.

   INPUT:
   F     - String containing name of user-supplied problem description.
         Call: yprime = fun(t,y) where F = 'fun'.
         t      - Time (scalar).
         y      - Solution column-vector.
         yprime - Returned derivative column-vector; yprime(i) = dy(i)/dt.
   t0    - Initial value of t.
   tfinal- Final value of t.
   y0    - Initial value column-vector.
   tol   - The desired accuracy. (Default: tol = 1.e-6).

   OUTPUT:
   tout  - Returned integration time points (row-vector).
   yout  - Returned solution, one solution column-vector per tout-value.

   C.B. Moler, 3-25-87.
   Marc Ullman  June 23, 1987
   Copyright (C) 1984-93  The Mathworks Inc.
   All Rights Reserved
*/

#include <math.h>
#include "mex.h"

/* Input Arguments */

#define	F_IN	prhs[0]
#define	T0_IN	prhs[1]
#define	TF_IN	prhs[2]
#define	Y0_IN	prhs[3]
#define	TOL_IN	prhs[4]


/* Output Arguments */

#define	T_OUT	plhs[0]
#define	Y_OUT	plhs[1]


/* Temporary Variables */

#define	OLD_T_OUT	old_plhs[0]
#define	OLD_Y_OUT	old_plhs[1]

#define	T_TEMP		tmp_prhs[0]
#define	Y_TEMP		tmp_prhs[1]
#define	Y_OUT_TEMP	tmp_plhs[0]


#define	MAX(A, B)	((A) > (B) ? (A) : (B))
#define	MIN(A, B)	((A) < (B) ? (A) : (B))

/* Fehlberg coefficients */

static double alpha[5] = { 1.0/4.0,  3.0/8.0,  12.0/13.0,  1.0,  1.0/2.0 };

static double beta[5][5] = {
    { 1.0/4.0, 3.0/32.0,  1932.0/2197.0,   8341.0/4104.0,  -6080.0/20520.0 },
    {     0.0, 9.0/32.0, -7200.0/2197.0, -32832.0/4104.0,  41040.0/20520.0 },
    {     0.0,      0.0,  7296.0/2197.0,  29440.0/4104.0, -28352.0/20520.0 },
    {     0.0,      0.0,            0.0,   -845.0/4104.0,   9295.0/20520.0 },
    {     0.0,      0.0,            0.0,             0.0,  -5643.0/20520.0 }
};

static double gama[6][2] = {
	{   902880.0/7618050.0,   -2090.0/752400.0 },
	{                  0.0,                0.0 },
	{  3953664.0/7618050.0,   22528.0/752400.0 },
	{  3855735.0/7618050.0,   21970.0/752400.0 },
	{ -1371249.0/7618050.0,  -15048.0/752400.0 },
	{   277020.0/7618050.0,  -27360.0/752400.0 }
};

#ifdef __STDC__
static double inf_norm(
	double	y[],
	unsigned int M
	)
#else
static double inf_norm(y,M)
double	y[];
unsigned int M;
#endif
{
	register unsigned int m;
	double	temp_max,yabs;

	temp_max = fabs(y[0]);

	for (m = 1; m < M; m++) {
		yabs = fabs(y[m]);
		temp_max = MAX(temp_max,yabs);
	}

	return(temp_max);
}



#ifdef __STDC__
void mexFunction(
	int		nlhs,
	Matrix	*plhs[],
	int		nrhs,
	Matrix	*prhs[]
	)
#else
mexFunction(nlhs, plhs, nrhs, prhs)
int nlhs, nrhs;
Matrix *plhs[], *prhs[];
#endif
{

	double	*tout,*yout; 
	double	*toutp,*youtp;
	double	*t,*y;
	double	*y0;
	char	fcn_name[20];

	register unsigned int	m,k,kk;
	unsigned int	M,K;
	int		nr, nc;

	double	ts;
	double	*s0,*s1,*s2,*s3,*s4,*s5,*s6;
	double	t0;
	double	tfinal;
	double	tol,tau,delta,h,hmax;
	double	ymax;
	double	*ydel;

	static double powr = 1.0/5.0;

	Matrix	*tmp_plhs[1],*tmp_prhs[2];
	Matrix	*s_matptr[6];
	Matrix	*old_plhs[2];


	/* Check out the arguments */

	if ((nrhs < 4) || (nrhs > 6)) {
		mexErrMsgTxt("Wrong number of input arguments for ODE45");
	} else if (nlhs != 2) {
		mexErrMsgTxt("Wrong number of output arguments for ODE45");
	}


	/*
	 *  Get user function name
	 */

	if (!mxIsString(F_IN))
		mexErrMsgTxt("String argument expected for function name in ODE45.");
	mxGetString(F_IN, fcn_name, 20);

	/*
	 * Get Input Arguments
	 */

	nr = mxGetM(T0_IN);
	nc = mxGetN(T0_IN);
	if (!mxIsNumeric(T0_IN) || mxIsComplex(T0_IN) || 
		!mxIsFull(T0_IN)  || !mxIsDouble(T0_IN) || nr*nc != 1)
		mexErrMsgTxt("Bad t0 input for ODE45.");
	t0 = mxGetScalar(T0_IN);

	nr = mxGetM(TF_IN);
	nc = mxGetN(TF_IN);
	if (!mxIsNumeric(TF_IN) || mxIsComplex(TF_IN) || 
		!mxIsFull(TF_IN)  || !mxIsDouble(TF_IN) || nr*nc != 1)
		mexErrMsgTxt("Bad tfinal input for ODE45.");
	tfinal = mxGetScalar(TF_IN);

	M = MAX(mxGetM(Y0_IN),mxGetN(Y0_IN));

	if (!mxIsNumeric(Y0_IN) || mxIsComplex(Y0_IN) || 
		!mxIsFull(Y0_IN)  || !mxIsDouble(Y0_IN) || !M)
		mexErrMsgTxt("Bad y0 input for ODE45.");
	y0 = mxGetPr(Y0_IN);


	/*
	 * Create and Initialize Return Arguments
	 */

	T_OUT = mxCreateFull(1, 1, REAL);
	Y_OUT = mxCreateFull(mxGetM(Y0_IN),mxGetN(Y0_IN), REAL);

	tout = mxGetPr(T_OUT);
	yout = mxGetPr(Y_OUT);

	tout[0] = t0;
	for (m = 0; m <M; m++) {
		yout[m] = y0[m];
	}

	/*
	 * Create arguments for calling user function
	 */

	T_TEMP = mxCreateFull(1, 1, REAL);
	Y_TEMP = mxCreateFull(mxGetM(Y0_IN),mxGetN(Y0_IN), REAL);

	t = mxGetPr(T_TEMP);
	y = mxGetPr(Y_TEMP);

	/*
	 * Create an array for ydel
	 */

	ydel = (double *) mxCalloc(M, sizeof(double));

	/*
	 * Initialization
	 */

	if (nrhs < 5) {
		tol = 0.000001;
	} else {
		nr = mxGetM(TOL_IN);
		nc = mxGetN(TOL_IN);
		if (!mxIsNumeric(TOL_IN) || mxIsComplex(TOL_IN) || 
			!mxIsFull(TOL_IN)  || !mxIsDouble(TOL_IN) || nr*nc != 1)
			mexErrMsgTxt("Bad tol input for ODE45.");
		tol = mxGetScalar(TOL_IN);
	}

	hmax = (tfinal - t0)/16;
	h = hmax/8;

	tout = &t0; 
	yout = y0;

	*t = t0;

	k = 0;

	/*
	 * The main loop
	 */

	while ((*t < tfinal) && ((*t + h) >= *t)) {

		if ((*t + h) > tfinal) {
			h = tfinal - *t;
		}

		/*
		 * Compute the slopes
		 */


		/* ts = t  and  s0 = y */

		ts = tout[k];
		s0 = &yout[k*M];

		/* s1 = feval(F, t, y); */

		*t = ts;
		for  (m = 0; m < M; m++) {
			y[m] = s0[m];
		}

		mexCallMATLAB(1,tmp_plhs,2,tmp_prhs,fcn_name);
		if (mxGetM(Y_OUT_TEMP)*mxGetN(Y_OUT_TEMP) != M) {
			goto errorexit;
		}
		s1 = mxGetPr(Y_OUT_TEMP);
		s_matptr[0] = Y_OUT_TEMP;

		/* s2 = feval(F, t+h*alpha(1), y+h*s1*beta(1,1)); */

		*t = ts + h*alpha[0];
		for  (m = 0; m < M; m++) {
			y[m] = s0[m]+h*s1[m]*beta[0][0];
		}

		mexCallMATLAB(1,tmp_plhs,2,tmp_prhs,fcn_name);
		if (mxGetM(Y_OUT_TEMP)*mxGetN(Y_OUT_TEMP) != M) {
			goto errorexit;
		}
		s2 = mxGetPr(Y_OUT_TEMP);
		s_matptr[1] = Y_OUT_TEMP;

		/* s3 = feval(F, t+h*alpha(2), y+h*[s1*beta(2,1)+s2*beta(2,2)]; */

		*t = ts + h*alpha[1];
		for  (m = 0; m < M; m++) {
			y[m] = s0[m]+h*(s1[m]*beta[0][1] + s2[m]*beta[1][1]);
		}

		mexCallMATLAB(1,tmp_plhs,2,tmp_prhs,fcn_name);
		if (mxGetM(Y_OUT_TEMP)*mxGetN(Y_OUT_TEMP) != M) {
			goto errorexit;
		}
		s3 = mxGetPr(Y_OUT_TEMP);
		s_matptr[2] = Y_OUT_TEMP;

		/* s4 = feval(F, t+h*alpha(3), y+h*[s1*beta(3,1)+s2*beta(3,2)+...]; */

		*t = ts + h*alpha[2];
		for  (m = 0; m < M; m++) {
			y[m] = s0[m]+h*(s1[m]*beta[0][2] + s2[m]*beta[1][2]
					+s3[m]*beta[2][2]);
		}

		mexCallMATLAB(1,tmp_plhs,2,tmp_prhs,fcn_name);
		if (mxGetM(Y_OUT_TEMP)*mxGetN(Y_OUT_TEMP) != M) {
			goto errorexit;
		}
		s4 = mxGetPr(Y_OUT_TEMP);
		s_matptr[3] = Y_OUT_TEMP;

		/* s5 = feval(F, t+h*alpha(4), y+h*[s1*beta(4,1)+s2*beta(4,2)+...]; */

		*t = ts + h*alpha[3];
		for  (m = 0; m < M; m++) {
			y[m] = s0[m]+h*(s1[m]*beta[0][3] + s2[m]*beta[1][3]
					+s3[m]*beta[2][3] + s4[m]*beta[3][3]);
		}

		mexCallMATLAB(1,tmp_plhs,2,tmp_prhs,fcn_name);
		if (mxGetM(Y_OUT_TEMP)*mxGetN(Y_OUT_TEMP) != M) {
			goto errorexit;
		}
		s5 = mxGetPr(Y_OUT_TEMP);
		s_matptr[4] = Y_OUT_TEMP;

		/* s6 = feval(F, t+h*alpha(5), y+h*[s1*beta(5,1)+s2*beta(5,2)+...]; */

		*t = ts + h*alpha[4];
		for  (m = 0; m < M; m++) {
			y[m] = s0[m]+h*(s1[m]*beta[0][4] + s2[m]*beta[1][4]
					+s3[m]*beta[2][4] + s4[m]*beta[3][4]
					 +s5[m]*beta[4][4]);
		}

		mexCallMATLAB(1,tmp_plhs,2,tmp_prhs,fcn_name);
		if (mxGetM(Y_OUT_TEMP)*mxGetN(Y_OUT_TEMP) != M) {
			goto errorexit;
		}
		s6 = mxGetPr(Y_OUT_TEMP);
		s_matptr[5] = Y_OUT_TEMP;

		/*
		 * Estimate the error and the acceptable error
		 */

		for  (m = 0; m < M; m++) {
			ydel[m] = h*(s1[m]*gama[0][1] + s2[m]*gama[1][1]
				    + s3[m]*gama[2][1] + s4[m]*gama[3][1]
				     + s5[m]*gama[4][1] + s6[m]*gama[5][1]);
		}
		delta = inf_norm(ydel,M);

		ymax = inf_norm(&yout[k*M],M);
		tau = tol*MAX(ymax,1.0);

		/*
		 * Update the solution only if the error is acceptable
		 */

		if (delta <= tau) {

			K = k+1;

			OLD_T_OUT = T_OUT;
			OLD_Y_OUT = Y_OUT;
			T_OUT = mxCreateFull(1,K+1,REAL);
			Y_OUT = mxCreateFull(m,K+1,REAL);

			toutp = tout;
			youtp = yout;
			tout = mxGetPr(T_OUT);
			yout = mxGetPr(Y_OUT);

			for (kk = 0;kk < K; kk++) {
				tout[kk] = toutp[kk];
				for(m = 0; m < M; m++) {
					yout[kk*M+m] = youtp[kk*M+m];
				}
			}

			/* if k <> 0 Free OLD_T_OUT,OLD_Y_OUT */

			if (k) {
				mxFreeMatrix(OLD_T_OUT);
				mxFreeMatrix(OLD_Y_OUT);
			}

			tout[K] = *t = tout[k] + h;
			for (m = 0; m < M; m++) {
				yout[K*M+m] = yout[k*M+m]
				  + h*(s1[m]*gama[0][0] + s2[m]*gama[1][0]
				     + s3[m]*gama[2][0] + s4[m]*gama[3][0]
				     + s5[m]*gama[4][0] + s6[m]*gama[5][0]);
			}

			k++;
		}

		/* Free s_matptr[0], s_matptr[1], ... , s_matptr[5] */

		for (m = 0; m < 6; m++) {
			mxFreeMatrix(s_matptr[m]);
		}

		/*
		 * Update the step size
		 */

		if (delta) {
			h = MIN(hmax, 0.8*h*pow(tau/delta,powr));
		}


	}

	if (*t < tfinal) {
#ifdef THINK_C
		mexPrintf("Singularity likely at t = %f\r",*t);
#else
		mexPrintf("Singularity likely at t = %f\n",*t);
#endif
	}

	/*
	 * Transpose the outputs
	 */

	m = mxGetM(T_OUT);
	mxSetM(T_OUT, mxGetN(T_OUT));
	mxSetN(T_OUT, m);

	Y_TEMP = Y_OUT;
	mexCallMATLAB(1,&Y_OUT,1,&Y_TEMP,".'");

	return;

errorexit:
	mexPrintf("Function %s is not returing the correct number of state \
derivatives.",fcn_name);
	mexErrMsgTxt("Error in ODE45.");

}

