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
			else if(*beg == ";" || strncmp(&beg->at(beg->size()-1), ";", 1) == 0) {
				connectors.push_back(";");
				ls.push_back(*beg);
				argv.push_back(const_cast<char*>(ls.back().c_str()));
			}
			else {
				ls.push_back(*beg);
				argv.push_back(const_cast<char*>(ls.back().c_str()));
			}
			
		}
		pid_t pid;
		int status;

		pid = fork();


		if(pid < 0) {
			cout << "ERROR: Fork child failed\n";
			exit(1);
		}
		else if(pid == 0) {
			int result = execvp(argv[0], &argv[0]);
			cout << result << endl;
			
		}
		else {
			if(wait(&status) < 0) {
				perror("Error in child");
				exit(1);
			}
		}
	
	}

}
