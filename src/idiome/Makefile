all: pimpl raii

run: raii pimpl
	./raii
	./pimpl

pimpl: pimpl.o
	g++ -o pimpl pimpl.o

pimpl.o: pimpl.cpp
	g++ -c -g pimpl.cpp

raii: raii.o
	g++ -o raii raii.o

raii.o: raii.cpp
	g++ -c -g raii.cpp

clean:
	rm *.o
	rm raii
	rm pimpl
