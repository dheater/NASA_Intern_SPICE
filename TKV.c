#include <stdio.h>
#include <SpiceUsr.h>

int main() {

    ConstSpiceChar *versn;
    versn = tkvrsn_c("TOOLKIT");
    printf("Toolkit version: %s\n", versn);
    return 0;
}
