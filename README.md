RSHELL
================

General Description
--------------------------
Rshell is an executable that mimics terminal. You are able to run commands that
interact with the current directory, but cannot change the current directory.

For example:

	* ls will list the items in the specified directory (default is current)
		you can include flags like -a
		ls -a
		ls ./src/

	* echo will print input. If input is longer than one word, use " " to group
		echo hello
		echo "hello world"

The program first takes in a line of input. The input is then tokenized using
boost::tokenizer. An iterator is used to read through the tokenized input.
Commands are separated and pushed into the commands vector while the connector
vector keeps track of what connectors separate them. '#' will cut off 
everything after it. After the input has been processed, we run through the
commands vector executing each command. If there is more than one command, the
program checks the connector and decides to run the command or not. Afterwards
the commands and connectors vectors are cleared to prepare for the next line of
input.


Connectors
--------------------------
This program also allows the use of connectors:

	*  ;	allows for multiple commands in one line 
			i.e. ls -a; echo hello 

			known bugs: 
				- inputting ; alone will return an error as execute tries 
					run execvp on empty char* with NULL
				- ; && ls 
					first an error will be printed, but ls will still run 
					because ; separates everything before it from what 
					comes after

	* &&	will run command after if command before succeeds
			i.e. ls && echo hello
					will list current dir and then print hello

			     cd && echo hello
					will print error and won't echo hello

				 lol && echo hello
					will print erro and won't echo hello

	* ||	will run command after, if command before fails
			i.e. ls || echo hello
					will list curr dir and won't echo hello

			     cd || echo hello
					will return error on cd, then echo hello

				 lol || echo hello
				    will return error, then echo hello

Comments
-------------------------
Use # for comments. Everything after '#' is considered a comment and the
program will not execute it.
i.e. echo hello # world
		will echo hello but not world

	echo hello # ls -a
		will echo hello, but will not run ls -a

Exit
-------------------------
Use "exit" to exit the program. It is case-sensitive.

To Note
-------------------------
For logic connectors && and ||, if inputted consecutively
	the first connector is used, second is ignored.
		i.e. ls && || echo hello
			is equivalent to
			ls && echo hello
