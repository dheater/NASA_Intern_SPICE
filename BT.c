#include <stdio.h>
#include <SpiceUsr.h>

#define SIZESTR 100

int main() {

    SpiceChar str[SIZESTR];

    printf("\n\n\tRunning...\n\n");
    prompt_c("Enter setup file name: ", SIZESTR, str);

    if(str[0] == 'q') {
        printf("\n\n\tKF.txt is loading to kernel pool...\n");
        furnsh_c("KF.txt");
    } else {
        printf("\n\n\t%s is loading to kernel pool...\n", str);
        furnsh_c(str);
    }

    printf("\n\n\tCompleted...\n\n");
}
