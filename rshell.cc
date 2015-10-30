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

	vector<string> ls;
	string input;
	vector<char*> argv;

	typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
	string separator1("");
	string separator2(" ");
	string separator3("\"\'");
	boost::escaped_list_separator<char> sep(separator1, separator2, separator3);	

	while(true) {
		cout << "$ ";
		cout.flush();

		getline(cin, input);
		boost::trim(input);

		tokenizer tok(input, sep);
		
		for(tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
			if(*beg == "#" || strncmp(&beg->at(0), "#", 1) == 0) {
				break;
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
		}
		else {
			if(wait(&status) < 0) {
				perror("Error in child");
				exit(1);
			}
		}
	
	}

}
