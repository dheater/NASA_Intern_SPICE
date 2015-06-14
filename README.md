## Dependencies
This code depends on the SPICE, cspice tools and libraries avaialble from the [JPL NAIF website](http://naif.jpl.nasa.gov/naif/index.html).

## Source
GHF.cpp - Main C++ code -- Integrates, and outputs moon positions in vector form to MoonPos.txt
VPyTBS.py - vPython visualizer -- reads in data from MoonPos.txt and plots accordingly.
RK4.hpp - Header for Integrator -- takes two 1x3 vectors
TBOS.c - Current C code for basic simulation (non-integral)
GHV.cpp - WIP C++ code for integration routine simulation (not working)
vecManip.h - Header containing vector manipulation methods -- Used for add, subtract, etc. C++ is not kind to vectors...

## Building
Type `make`

The Makefile assumes that SPICE is installed `/opt/cspice`
If SPICE is installed in another directory, override the `SPICEDIR` variable.
For example if SPICE is installed at /SPICE/csprice, the command would be

    make SPICEDIR=/SPICE/cspice
