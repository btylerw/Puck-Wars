CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: puckwars

puckwars: puckwars.o log.o avu2.cpp amartinez2.cpp adiazperez.cpp tbrown.cpp abotello.cpp abotello.h amartinez2.h tbrown.h avu2.h
	g++ puckwars.o log.o avu2.cpp amartinez2.cpp adiazperez.cpp tbrown.cpp abotello.cpp libggfonts.a $(LFLAGS) -o puckwars
puckwars.o:	puckwars.cpp
	g++ -c puckwars.cpp

log.o:	log.cpp
	g++ -c log.cpp

clean:
	rm -f puckwars
	rm -f *.o

