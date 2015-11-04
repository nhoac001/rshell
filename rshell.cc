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
				commands.push_back(argv);
				argv.clear();
			}
			else {
				ls.push_back(*beg);
				argv.push_back(const_cast<char*>(ls.back().c_str()));
			}
		}

		//if just one command, commands vector is empty
		if(commands.size() == 0) {
			commands.push_back(argv);
			argv.clear();
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
				int result = execvp(commands.at(i).at(0), &(commands.at(i).at(0)));
				cout << result << endl;
			}	
		}
		else {
			if(wait(&status) < 0) {
				perror("Error in child");
				exit(1);
			}
		}
		commands.clear();
	}

}
