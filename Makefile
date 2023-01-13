all : main paperfish

CXXFLAGS = -std=c++17 -Wall -MMD -Wextra -pedantic -g -Ofast -flto
OBJECTS = position.o move.o board.o square.o table.o
DEPENDS = ${OBJECTS:.o=.d}

main : ${OBJECTS} main.o
	${CXX} ${OBJECTS} main.o -o main -pg

paperfish : ${OBJECTS} paperfish.o
	${CXX} ${OBJECTS} paperfish.o -o paperfish

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} main.o main.d main paperfish.o paperfish.d paperfish