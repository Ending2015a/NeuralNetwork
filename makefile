CC = g++
CFLAGS = -std=c++11

all: Solver Tester

Solver: source/Solver.o
	${CC} $< -o $@
Tester: source/Tester.o
	${CC} $< -o $@
%.o: %.cpp
	${CC} ${CFLAGS} $< -c -o $@

clean:
	@rm -rf source/*.o
