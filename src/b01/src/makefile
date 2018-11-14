## Default-Target
all: run

## Baue "tollesProgramm"
limits: limits.c
	gcc -ansi -o limits limits.c -Wall -pedantic

sinus: sinus.c
	gcc  -o sinus sinus.c -Wall -pedantic -lm

sinus_while: sinus_while.c
	gcc  -o sinus_while sinus_while.c -Wall -pedantic -lm
	
pyramiede: pyramiede.c
		gcc -ansi -Wall -pedantic -o pyramiede pyramiede.c
		
bitOp: bit.c
	gcc -ansi -Wall -pedantic -o bit bit.c

## Lasse das Programm laufen
run: limits sinus sinus_while pyramiede bit
	./limits
	./sinus
	./sinus_while
	./pyramiede
	./bit

## Aufraeumen
clean:
	rm -rf limits
	rm -rf sinus
	rm -rf sinus_while
	rm -rf pyramiede
	rm -rf bit
