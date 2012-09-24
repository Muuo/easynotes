proj:main.o pa_io.o
	gcc -Wall main.o pa_io.o -lportaudio -lfftw3 -o build/proj

main.o:main.c pa_io.h
	gcc -Wall -c main.c main.h pa_io.h

pa_io.o:pa_io.c pa_io.h
	gcc -Wall -c pa_io.c pa_io.h

clean:
	rm build/proj main.o main.h.gch pa_io.o pa_io.h.gch
