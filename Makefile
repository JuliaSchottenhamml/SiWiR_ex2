SHELL        = /bin/bash
CXX          = g++
 
FLAGS        = -march=native -mtune=native -std=c++0x -Werror -Wall -Wextra -Wshadow -fopenmp -O3 -fopt-info-optall -DNDEBUG
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

run: rbgs
	./rbgs ${ARGS}
	gnuplot gnuplotSolution.plt

documentation: report/setup.tex report/main.tex
	cd report && pdflatex main.tex
	cd report && pdflatex main.tex
	cd report && pdflatex main.tex

clean:
	rm -f *.o rgbs

.PHONY : all clean
