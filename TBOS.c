
/*
This program calculates net force of two bodies,
updating their respective positions and velocities,
then outputing their position vectors to a file to be read by vPython simulation
(VPyTBS.py).
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Spice-specific headers; Not important for this program --yet--  */
#include <SpiceUsr.h>

/* SIZE is for SpiceChar array-size prompt function. Again, unimportant at the
 * moment.  */
#define SIZE 100

#define G -6.67384e-11

void loadKernel(); // Loads spice kernels

long double mag(long double vec[3]);       // Magnitude func
long double *toUnitVec(long double *dVec); // Unit-vec calc func

int main() {

    printf("\n\n\tTwo Body Orbital System Simulation\n\n");

    // loadKernel();

    long double mE = 5.97e24, // Mass earth
        mM = 5000,            // Mass moon

        radE = 6371, // Radius earth
        radM = 1737, // Radius moon

        pE[3] = { 0 * mE, 0 * mE, 0 * mE },      // Momentum (not vel) earth
        pM[3] = { 0 * mM, 51.512 * mM, 0 * mM }, // Momentum moon

        posE[3] = { 0, 0, 0 },      // Position earth
        posM[3] = { 1.5e11, 0, 0 }, // Pos moon

        distVec[3] = { posM[0] - posE[0], posM[1] - posE[1],
                       posM[2] - posE[2] }, // Distance between earth & moon

        fNet[3]; // Net force

    long double *unitVec = (long double *)malloc(3 * sizeof(long double));

    printf(" Earth Position: <%.2Le,%.2Le,%.2Le>", posE[0], posE[1], posE[2]);
    printf(" Moon Position: <%.2Le,%.2Le,%.2Le>", posM[0], posM[1], posM[2]);
    printf(" Distance: <%.2Le,%.2Le,%.2Le>", distVec[0], distVec[1], distVec[2]);

    long double

        t = 1, // Time counter

        dt = (.01); // Time step

    printf("\n mE\t%Lf \n radE\t%Lf \n mM\t%Lf \n radM\t%Lf \n radDif\t%Lf \n",
           mE, radE, mM, radM, mag(distVec));

    printf("\n Magnitude:\n\t%Lf", mag(distVec));

    printf("\n Net Force:\t");

    unitVec = toUnitVec(distVec);

    for(int i = 0; i < 3; ++i) {

        fNet[i] = ((G * mE * mM) / (mag(distVec) * mag(distVec))) *
                  (unitVec[i]); // Net force calculation (iterates
                                // per-dimension)

        printf("\n\tfNet[%d]: %Lf", i, fNet[i]);
    }

    FILE *fp;
    fp = fopen("MoonPos.txt", "w+"); // Output file

    int userIn = 0;

    printf("\n\nPlease input number of weeks to run the sim: ");
    scanf("%d", &userIn);

    while(t > 0) {

        for(int i = 0; i < 3; ++i) {
            fNet[i] = ((G * mE * mM) / (mag(distVec) * mag(distVec))) *
                      (unitVec[i]); // Update Netforce

            pE[i] += (fNet[i] * dt); // Updating momentum
            pM[i] += (fNet[i] * dt);

            posE[i] += (pE[i] / mE) * dt; // Updating pos
            posM[i] += (pM[i] / mM) * dt;

            distVec[i] = posM[i] - posE[i]; // Updating dist

            unitVec[i] = distVec[i] / mag(distVec); // Updating unitVec
        }

        t += dt; // Incr

        // printf("\n\tMoon Pos: <%Lf, %Lf, %Lf> ", posM[0], posM[1],
        // posM[2]);
        // printf("\n\tEarth Pos: <%Lf, %Lf, %Lf>", posE[0], posE[1],
        // posE[2]);
        // printf("\n\tDistance: %Lf %Lf %Lf", distVec[0], distVec[1],
        // distVec[2]);
        // printf("\n\tUnit Vec: %Lf %Lf %Lf",
        // unitVec[0],unitVec[1],unitVec[2]);
        // printf("\a");
        fprintf(fp, "%Lf %Lf %Lf %Lf %Lf %Lf\n", posM[0], posM[1], posM[2],
                posE[0], posE[1], posE[2]);
        // printf("\n\tFnet: %Lf %Lf %Lf", fNet[0], fNet[1], fNet[2]);
        // printf("\n\tMoon P: %Lf %Lf %Lf\n\n", pM[0], pM[1], pM[2]);

        if(t > userIn) {
            break;
        }
    }

    printf("\n UnitVector:\n");

    for(int i = 0; i < 3; ++i) {
        printf("\t%Lf \n", unitVec[i]);
    }

    fclose(fp);

    free(unitVec);

    return 0;
}

void loadKernel() { // This is for SPICE kernel load ; unimportant

    SpiceChar str[SIZE];

    prompt_c("Enter setup file name: ", SIZE, str);

    if(str[0] == 'q') {
        printf("\n\n\tMeta-Kernel KF.txt is loading to kernel pool...\n");
        furnsh_c("KF.txt");
    } else {
        printf("\n\n\t%s is loading to kernel pool...\n\n", str);
        furnsh_c(str);
    }

    return;
}

long double mag(long double vec[3]) {
    return (sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2])));
}

long double *toUnitVec(long double *dVec) {
    long double *uVec = (long double *)malloc(3 * sizeof(long double));

    uVec[0] = dVec[0] / mag(dVec);
    uVec[1] = dVec[1] / mag(dVec);
    uVec[2] = dVec[2] / mag(dVec);

    return uVec;
}
