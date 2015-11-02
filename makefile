FLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir bin
	g++ $(FLAGS) rshell.cc

rshell: rshell.cc
	g++ $(FLAGS) rshell.cc
