CFLAGS = -c -Wall -pedantic -g
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

LED_BIB = all_libledanzeige.so
LIB_FLAG = -L. -l$(LED_LIB)

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

MAIN_C = $(SRC_SPEICHER)main.c

O_FILES = demo.o segmentanzeige.o


##default target
all: all_libledanzeige.so install_libledanzeige demo studi pruef speicher

##ledanzeige

demo: $(O_FILES) lib/$(LED_BIB)
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

pruef: pruefungen.o $(LED_BIB)
	gcc -o pruef pruefungen.o TM1637.o segmentanzeige.o $(OFLAGS) 

pruefungen.o: $(PRUEF_C) $(PRUEF_H) $(SEG_H)
	gcc $(CFLAGS) $(INCLUDE) $(PRUEF_C)

TM1637.o: $(TM_C) $(TM_H) $(TM_INT_H)
	gcc $(CFLAGS) $(INCLUDE) $(TM_C)


##speicher

speicher: speicherverwaltung.o main.o
	gcc -o speicher speicherverwaltung.o main.o

speicherverwaltung.o: $(SPEICHER_C) $(SPEICHER_H)
	gcc $(CFLAGS) $(INCLUDE) $(SPEICHER_C) -DMALLOCSPLIT

main.o: $(MAIN_C) $(SPEICHER_H)
	gcc $(CFLAGS) $(INCLUDE) $(MAIN_C)


##Bibliothek aus LED-Anzeige

all_libledanzeige.so: $(SRC_LED)segmentanzeige.c $(SRC_LED)TM1637.c
	gcc -c -fpic $(SRC_LED)segmentanzeige.c $(INCLUDE)
	gcc -c -fpic $(SRC_LED)TM1637.c $(INCLUDE)
	gcc -shared -o all_libledanzeige.so segmentanzeige.o TM1637.o
	
install_libledanzeige: all_libledanzeige.so
	mkdir -p lib/
	cp all_libledanzeige.so lib/




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
	rm lib/all_libledanzeige.so
	rmdir lib/
	rm all_libledanzeige.so
	rm main.o

##Documentation

doc:
	doxygen doxygen.config

doc-remove: 
	rm -r ./html ./latex
