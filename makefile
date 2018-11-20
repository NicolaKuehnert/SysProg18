CFLAGS = -c -Wall -pedantic
OFLAGS = -lwiringPi -pthread -lcrypt -lm -lrt
INCLUDE = -I./include -I./src


SRC_LED = src/ledanzeige/
INC_LED = include/ledanzeige/
INC_STUDI = include/studiverwaltung/
SRC_STUDI = src/studiverwaltung/
INC_PRUEF = include/pruefungen/
SRC_PRUEF = src/pruefungen/
SRC_SPEICHER = src/speicherverwaltung/
INC_SPEICHER = include/speicherverwaltung/

DEMO = $(SRC_LED)demo.c
SEG_H = $(INC_LED)segmentanzeige.h
SEG_C = $(SRC_LED)segmentanzeige.c
TM_INT_H = $(INC_LED)TM1637_intern.h
TM_H = $(INC_LED)TM1637.h
TM_C = $(SRC_LED)TM1637.c

STUDI_C = $(SRC_STUDI)studiverwaltung.c
STUDI_H = $(INC_STUDI)studiverwaltung.h

PRUEF_C = $(SRC_PRUEF)pruefungen.c
PRUEF_H = $(INC_PRUEF)pruefungen.h

SPEICHER_C = $(SRC_SPEICHER)speicherverwaltung.c
SPEICHER_H = $(INC_SPEICHER)speicherverwaltung.h

O_FILES = demo.o segmentanzeige.o


##default target
all: demo studi pruef speicher

##ledanzeige

demo: $(O_FILES)
	gcc -o demo $(O_FILES) $(OFLAGS)

demo.o: $(DEMO) $(SEG_H)
	gcc $(CFLAGS) $(INCLUDE) $(DEMO)

segmentanzeige.o: $(SEG_C) $(SEG_H) $(TM_INT_H) 
	gcc $(CFLAGS) $(INCLUDE) $(SEG_C) $(TM_C)
	
##studikram

studi: studiverwaltung.o
	gcc -o studi studiverwaltung.o

studiverwaltung.o: $(STUDI_C) $(STUDI_H)
	gcc $(CFLAGS) $(INCLUDE) $(STUDI_C)

##prüfung

pruef: pruefungen.o TM1637.o segmentanzeige.o
	gcc -o pruef pruefungen.o TM1637.o segmentanzeige.o $(OFLAGS)

pruefungen.o: $(PRUEF_C) $(PRUEF_H) $(SEG_H)
	gcc $(CFLAGS) $(INCLUDE) $(PRUEF_C)

TM1637.o: $(TM_C) $(TM_H) $(TM_INT_H)
	gcc $(CFLAGS) $(INCLUDE) $(TM_C)


##speicher

speicher: speicherverwaltung.o
	gcc -o speicher speicherverwaltung.o

speicherverwaltung.o: $(SPEICHER_C) $(SPEICHER_H)
	gcc $(CFLAGS) $(INCLUDE) $(SPEICHER_C)



## Programme ausführen
run-speicher: all
	./speicher

run-pruef: all
	./pruef

run-studi: all
	./studi

run-demo: all
	./demo

## Programme testen
test-speicher: all
	make -C tests/speicherverwaltung

test-pruef: all
	make -C tests/pruefungen

test-studi: all
	make -C tests/studiverwaltung

test-demo: all
	make -C tests/ledanzeige

test: test-speicher test-pruef test-demo test-studi

##clean up
.PHONY: clean
clean:
	rm demo.o
	rm segmentanzeige.o
	rm demo
	rm pruefungen.o
	rm pruef
	rm studiverwaltung.o
	rm studi
	rm TM1637.o
	rm speicherverwaltung.o
	rm speicher

##Documentation

doc:
	doxygen doxygen.config

doc-remove: 
	rm -r ./html ./latex
