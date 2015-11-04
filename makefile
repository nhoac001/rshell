FLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir -p bin
	g++ $(FLAGS) ./src/rshell.cc -o rshell
	mv rshell ./bin/

rshell: rshell.cc
	g++ $(FLAGS) ./src/rshell.cc -o rshell
