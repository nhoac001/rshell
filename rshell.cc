#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"

using namespace std;

void execute(vector<char*> ok, int pass) {
	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if(c_pid < 0) {
		perror("fork failed");
		exit(1);
	}
	else if(c_pid == 0) {
		pass = execvp(ok[0], &(ok[0]));
		perror("execvp failed");
	}
	else {
		if((pid = wait(&status)) < 0) {
			perror("Error in child");
			exit(1);
		}
	}
}

int main() {
	//vector to temp hold commands
	vector<string> ls;
	//takes user input
	string input;
	//stores commands to be run
	vector<char*> argv;
	//stores what connectors we find
	vector<string> connectors;
	//2-d vector to store list of commands
	vector< vector< char* > > commands;

	typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
	string separator1("");
	string separator2(" ");
	string separator3("\"\'");
	boost::escaped_list_separator<char> sep(separator1, separator2, separator3);	

	while(true) {
		cout << "$ ";
		cout.flush();

		//get input and prepare to tokenize
		getline(cin, input);
		boost::trim(input);

		//tokenize input
		tokenizer tok(input, sep);

		//use tokenized input to fill argv
		for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {

			//skip comments, only works if begins with '#'
			if(*beg == "#" || strncmp(&beg->at(0), "#", 1) == 0) {
				break;
			}
			else if(*beg == "exit") {
				return 0;
			}
			else if(*beg == "||" || strncmp(&beg->at(0), "||", 2) == 0) {
				if(argv.size() != 0) {
					connectors.push_back("||");
					commands.push_back(argv);
					argv.clear();
				}
				
			}
			else if(*beg == ";" || strncmp(&beg->at(beg->size()-1), ";", 1) == 0) {
				connectors.push_back(";");
				string temp = beg->substr(0, beg->size()-1);
				ls.push_back(temp);
				argv.push_back(const_cast<char*>(ls.back().c_str()));
				argv.push_back(NULL);
				commands.push_back(argv);
				argv.clear();
			}
			else {
				ls.push_back(*beg);
				argv.push_back(const_cast<char*>(ls.back().c_str()));
			}
		}

		//push argv into commands if argv isn't empty
		if(argv.size() != 0) {
			argv.push_back(NULL);
			commands.push_back(argv);
			argv.clear();
		}
/*		for(unsigned i = 0; i < commands.size(); i++) {
			for(unsigned j = 0; j < commands.at(i).size(); j++) {
				cout << commands[i][j];
			}
			cout << endl;
		}

	
		pid_t pid;
		int status;

		pid = fork();


		if(pid < 0) {
			cout << "ERROR: Fork child failed\n";
			exit(1);
		}
		else if(pid == 0) {
			for(unsigned i = 0; i < commands.size(); i++) {	
				int result = execvp(commands[i][0], &(commands[i][0]));
				cout << result << endl;
			}	
		}
		else {
			if(wait(&status) < 0) {
				perror("Error in child");
				exit(1);
			}
		}*/
		for(unsigned i = 0; i < commands.size(); i++) {
			int state = 0;
			execute(commands[i], state);
			if(state)
			{}
		}
		commands.clear();
	}

}


