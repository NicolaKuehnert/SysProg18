
## Compiler and general flags
CC=gcc
CFLAGS=-ansi -pedantic -Wall
CFLAGS+=-fPIC
CFLAGS+=-g
CFLAGS+=-DNDEBUG
CFLAGS+=-I$(INCL_V) -I$(INCL_M) -I.

CXX=g++
CXXFLAGS=-Wall
CXXFLAGS+=-fPIC
CXXFLAGS+=-g
CXXFLAGS+=-DNDEBUG
CXXFLAGS+=-I$(INCL_V) -I$(INCL_M) -I.

CMOCKFLAGS=-I/usr/local/include/
CMOCKLDFLAGS:=-L/usr/local/lib/

LDFLAGS=-L. -Wl,-rpath=.
LDLIBS=-lwiringPi -lm -pthread -lgtest -lgtest_main

# path to includes in "Vorgabe" (and solution of students)
INCL_V?=$(shell realpath "$(shell pwd)/../../include/")
SRC_V?=$(shell realpath "$(shell pwd)/../../src/")
# path to includes in "Muster" (i.e. our own solution)
INCL_M?=$(INCL_V)
SRC_M?=$(SRC_V)


###############################################################################
##
## Note 1: This makefile is not aware of changes to any test headerfile, i.e.
##         changes to .h files in the tests/ subdirectory are not detected
##         and taken care of.
##         (Changes to source files (test as well as sources under test) and
##         dependencies to the sources under test are detected as always.)
##
## Note 2: When switching from test to abgabe or vice versa please make sure
##         to call make clean. Otherwise the shared libraries and/or binaries
##         may be not be recompiled using the correct flags!
##
## Note 3: Tests should be run as sudo, i.e. `sudo make ...`
##
##
## This makefile deals with test targets with different test sources
## and an additional compiler flag like -DABGABE or -DMALLOCSPLIT.
##

## Sources under test (SUT)
## TODO: Project specific: Adapt accordingly!
SUTSRC = $(addprefix $(SRC_V), /ledanzeige/TM1637.c) $(addprefix $(SRC_M), /ledanzeige/segmentanzeige.c)

## Extra flags for compiling the SUT (as defined in assignment sheet)
## TODO: Project specific: Adapt accordingly!
FLAG =  ## no extra flags in P1

## Test binaries
## TODO: Project specific: Adapt accordingly!
TESTS      = segmentanzeige  							## names of test executables
TESTS_FLAG = ## not used in P1                          ## names of test executables (used with flag)

## Test sources
## TODO: Project specific: Adapt accordingly!
segmentanzeige: segmentanzeige_tests.cpp
###############################################################################


## Common definitions
ALL_TESTS              = $(TESTS) $(TESTS_FLAG)         ## all test executables
TEST_TARGETS           = $(ALL_TESTS:%=test_%)          ## names of test targets
TEST_TARGETS_VALGRIND  = $(TEST_TARGETS:%=%_valgrind)   ## same, but for Valgrind tests
TEST_TARGETS_LEAKCHECK = $(TEST_TARGETS:%=%_leakcheck)  ## same, but for LeakCheck tests


## Main targets
.DEFAULT_GOAL=abgabe  ## otherwise the definition of test sources would count as default target

.Phony: $(TEST_TARGETS)
$(TEST_TARGETS): test_%: %$
	-./$*

.Phony: $(TEST_TARGETS_VALGRIND)
$(TEST_TARGETS_VALGRIND): test_%_valgrind: %$
	-valgrind --leak-check=yes ./$*

.Phony: $(TEST_TARGETS_LEAKCHECK)
$(TEST_TARGETS_LEAKCHECK): test_%_leakcheck: %$  libleakcheck.so
	LD_PRELOAD=libleakcheck.so ./$*

libleakcheck.so:
	cd ../leakcheck/ && $(MAKE) all
	cp ../leakcheck/libleakcheck.so .
	cd ../leakcheck/ && $(MAKE) clean

# Abgabe-Target:
# 1. Tests mit -DABGABE-Flag (normale Ausgabe)
# 2. Tests mit Valgrind (Ausgabe bzgl. Memory Leaks)
# Probleme:
# * Valgrind: "still reachable"; siehe Ticket 518 im Redmine (C++, Mocking)
# * Valgrind laeuft auf dem Raspi nicht 100%. Es gibt einen Fehler wg.
#   einer "illegal instruction". Dies stellt kein ernstes Problem dar, die
#   Berechnung der Speicherloecher funktioniert dennoch.
# Das `-` vor den Befehlen unterdrueckt das Abbrechen des Make-Runs
# bei einem Error. Der Error wird natuerlich dennoch kommuniziert.
.Phony: abgabe
abgabe: CFLAGS+=-DABGABE
abgabe: CXXFLAGS+=-DABGABE
abgabe: $(TEST_TARGETS_LEAKCHECK)


## Auxiliary variables and dependencies of sources under test
SUTLIBS  = libsut.so libsut_flag.so
SUTDEPS  = $(SUTLIBS:.so=.d)
$(SUTDEPS): %.d: $(SUTSRC)
	$(CC) $(CFLAGS) -MM $^ > $@ -MT $*.so
-include $(SUTDEPS)

## Extra flags: Compile with `-DABGABE`
libsut_flag.so: CFLAGS+=$(FLAG)
$(TESTS_FLAG): CXXFLAGS+=$(FLAG)

## Extra dependencies: Libs and test targets
$(TESTS): LDLIBS+=-lsut
$(TESTS): libsut.so
$(TESTS_FLAG): LDLIBS+=-lsut_flag
$(TESTS_FLAG): libsut_flag.so

## Do something :-)
$(ALL_TESTS):
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(CMOCKFLAGS) $(CMOCKLDFLAGS) $^ $(LDLIBS)

$(SUTLIBS): $(SUTSRC)
	$(CC) -shared $(CFLAGS) -o $@ $^


## ... and clean.
.Phony: clean
clean:
	rm -rf *.d *.o *.so $(ALL_TESTS)





