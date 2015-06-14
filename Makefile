SPICEDIR:=/opt/cspice

CFLAGS:=-I$(SPICEDIR)/include
CXXFLAGS:=$(CFLAGS) -std=c++11
LDFLAGS:=$(SPICEDIR)/lib/cspice.a -lm
PROGRAMS:= BT GHF TBOS TKV

all: $(PROGRAMS)

BT: BT.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

GHF: GHF.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

TBOS: TBOS.c
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

TKV: TKV.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean: $(PROGRAMS)
	rm -f $^
