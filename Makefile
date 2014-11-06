SHELL        = /bin/bash
CXX          = g++
 
FLAGS        = -march=native -mtune=native -std=c++0x -Wall -Wextra -Wshadow -Werror -fopenmp -O3 -DNDEBUG
DEBUGFLAGS   = 
RELEASEFLAGS =
INCPATH      = 
LIBPATH      = 
LIBS         = 

# likwid
FLAGS       += -DUSE_LIKWID -pthread
INCPATH     += -I/usr/local/likwid-3.1.2/include/
LIBPATH     += -L/usr/local/likwid-3.1.2/lib/
LIBS        += -llikwid
 
COMMON       = Timer.h
 
all: rbgs

%.o: %.cpp $(COMMON)
	$(CXX) -c $(FLAGS) $(INCPATH) $<
 
rbgs: rbgs.cpp
	$(CXX) $(FLAGS) $(INCPATH) -o rbgs rbgs.cpp $(LIBPATH) $(LIBS)

documentation: report/setup.tex report/main.tex
	cd report && pdflatex main.tex
	cd report && pdflatex main.tex
	cd report && pdflatex main.tex

clean:
	rm -f *.o rgbs

.PHONY : all clean
