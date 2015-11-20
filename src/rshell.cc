#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void execute(vector<char*> ok, int &pass) {
	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if(c_pid < 0) {
		perror("fork failed");
		exit(1);
	}
	else if(c_pid == 0) {
		// incase command is valid, but fails. pass = 1
		pass = execvp(ok[0], &(ok[0]));
		perror("execvp failed");
		exit(errno);
	}
	else if (c_pid > 0) {
		pid = wait(&status);
		
		if (WIFEXITED(status)) {
			// pass = 2 if execvp fails for invalid command
			pass = WEXITSTATUS(status);
		}
	}
}

void test(vector<char*> ok, int &pass, string flag) {
	struct stat buf;
	stat(ok[0], &buf);
	cout << "flag: " << flag << endl
		<< "ISREG: " << S_ISREG(buf.st_mode) << endl
		<< "ISDIR: " << S_ISDIR(buf.st_mode) << endl;
	if (flag == "e") {
		cout << "testing e: ";
		if ((S_ISREG(buf.st_mode) == 1) || (S_ISDIR(buf.st_mode) == 1)) {
			cout << "passed e" << endl;
			pass = 0;
		}
		else {
			cout << "failed e" << endl;
		}
	}
	else if (flag == "d") {
		if(S_ISDIR(buf.st_mode) == 1) {
			cout << "passed d" << endl;
			pass = 0;
		}
	}
	else if (flag == "f") {
		if(S_ISREG(buf.st_mode) == 1) {
			cout << "passed f" << endl;
			pass = 0;
		}
	}
	else {
		perror("unrecognized flag");
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

	//vector to hold flags
	vector<string> test_flag;

	typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
	string separator1("");
	string separator2(" ");
	string separator3("\"\'");
	boost::escaped_list_separator<char> sep(separator1, separator2, separator3);	

	while(true) {
/*		string test_flag;
		test_flag = "e";
*/

		cout << "$ ";
		cout.flush();

		//get input and prepare to tokenize
		getline(cin, input);
		boost::trim(input);

		//tokenize input
		tokenizer tok(input, sep);

		//use tokenized input to fill argv
		for (tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {

			//skip comments, only works if begins with '#'
			if (*beg == "#" || strncmp(&beg->at(0), "#", 1) == 0) {
				break;
			}
			//test condition
			else if (*beg == "test" || strncmp(&beg->at(0), "test", 4) == 0) {
				ls.push_back(*beg);
				argv.push_back(const_cast<char*>(ls.back().c_str()));
  				beg++;
				
				if (beg == tok.end()) {
					break;
				}

				if (strncmp(&beg->at(0), "-", 1) == 0) {
					if (beg->size() > 1) {
						test_flag.push_back(beg->substr(1, beg->size()));
					}
					else {
						test_flag.push_back("x");
					}
					beg++;
					if (beg == tok.end()) {
						break;
					}
				}
				else {
					test_flag.push_back("e");
				}
				if (strncmp(&beg->at(0), "/", 1) == 0) {
					string tempo = beg->substr(1, beg->size());
					ls.push_back(tempo);
				}
				else {
					ls.push_back(*beg);
				}
				argv.push_back(const_cast<char*>(ls.back().c_str()));
				argv.push_back(NULL);
				commands.push_back(argv);
				argv.clear();
			}
			// [ ] test
			else if (*beg == "[") {
				ls.push_back("test");
				argv.push_back(const_cast<char*>(ls.back().c_str()));
				beg++;

				if (beg == tok.end()) {
					break;
				}

				if (strncmp(&beg->at(0), "-", 1) == 0) {
					if (beg->size() > 1) {
						test_flag.push_back(beg->substr(1, beg->size()));
					}
					else {
						test_flag.push_back("x");
					}
					beg++;
					if (beg == tok.end()) {
						break;
					}
				}
				else {
					test_flag.push_back("e");
				}
				while (*beg != "]") {
					if (strncmp(&beg->at(0), "/", 1) == 0) {
						string tempa = beg->substr(1, beg->size());
						ls.push_back(tempa);
					}
					else {
						ls.push_back(*beg);
					}
					argv.push_back(const_cast<char*>(ls.back().c_str()));
					beg++;
					if (beg == tok.end()) {
						break;
					}
				}
				argv.push_back(NULL);
				commands.push_back(argv);
				argv.clear();
			}
			//or condition
			else if (*beg == "||" || strncmp(&beg->at(0), "||", 2) == 0) {
				// check if argv has been filled. If not, skip the "||"
				if (argv.size() != 0) {
					//push back "OR" for when you run commands
					connectors.push_back("OR");

					//end with NULL or you'll have problems with processes
					argv.push_back(NULL);
					commands.push_back(argv);
					argv.clear();
				}
				if (commands.size() != 0) {
					connectors.push_back("OR");
				}	
			}
			//and condition - same as OR above, just change "or" with "and"
			else if (*beg == "&&" || strncmp(&beg->at(0), "&&", 2) == 0) {
				//check if argv has been filled. If not, skip the "&&"
				if (argv.size() != 0)  {
					//push back "AND" for you run commmands
					connectors.push_back("AND");

					//end with NULL, push into commands
					argv.push_back(NULL);
					commands.push_back(argv);
					argv.clear();
				}
				if (commands.size() != 0) {
					connectors.push_back("AND");
				}
			}
			//; condition
			else if (*beg == ";" || strncmp(&beg->at(beg->size() - 1), ";", 1) == 0) {
				connectors.push_back(";");
				//cut off the ';' at the end, push into commands
				string temp = beg->substr(0, beg->size() - 1);
				ls.push_back(temp);
				argv.push_back(const_cast<char*>(ls.back().c_str()));
				argv.push_back(NULL);
				commands.push_back(argv);
				argv.clear();
			}
			//else, keep addinng words till end or connector reached
			else {
				//convert to string
				ls.push_back(*beg);
				//convert to const char* and push back
				argv.push_back(const_cast<char*>(ls.back().c_str()));
			}
		}

		//push argv into commands if argv isn't empty
		if (argv.size() != 0) {
			argv.push_back(NULL);
			commands.push_back(argv);
			argv.clear();
		}

		for (unsigned i = 0; i < commands.size(); i++) {
			for (unsigned j = 0; j < commands[i].size() - 1; j++) {
				cout << commands[i][j];
			}
			cout << endl;
		}

		//store the value returned by execvp
		int state = 1;
		vector<string>::iterator conex = connectors.begin();
		vector<string>::iterator flagit = test_flag.begin();
		for (unsigned i = 0; i < commands.size(); i++) {
			//exit check
			if (string(commands[i][0]) == "exit") {
				return 0;
			}
			//connectors only show up after one command has run
			if (i > 0) {
				// and condition, run command if previous succeeded
				if ((*conex == "AND") && ((state < 1))) {
					if(string(commands[i][0]) == "test") {
						commands[i].erase(commands[i].begin());
						test(commands[i], state, *flagit);
						flagit++;
					}
					else {
						execute(commands[i], state);
						conex++;
					}
				}
				// or condition, run command if previous failed
				else if ((*conex == "OR") && ((state >= 1))) {
					if(string(commands[i][0]) == "test") {
						commands[i].erase(commands[i].begin());
						test(commands[i], state, *flagit);
						flagit++;
					}
					else {
						execute(commands[i], state);
						conex++;
					}
				}
				// ; condition, run command regardless
				else if (*conex == ";") {
					if(string(commands[i][0]) == "test") {
						commands[i].erase(commands[i].begin());
						test(commands[i], state, *flagit);
						flagit++;
					}
					else {
						execute(commands[i], state);
						conex++;
					}
				}
				//else, failed all tests, skip the command
				else 
				{}
			}
			// i = 0. first command
			else {
				if(string(commands[i][0]) == "test") {
					commands[i].erase(commands[i].begin());
					test(commands[i], state, *flagit);
					flagit++;
				}
				else {
					execute(commands[i], state);
				}
			}
		}
		//empty commands vector
		commands.clear();
		//empty connectors vector
		connectors.clear();
		//empty flag vector
		test_flag.clear();
	}

}
