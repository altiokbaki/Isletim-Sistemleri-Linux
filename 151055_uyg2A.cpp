/**
 * @file 152120151055_uyg2A.cpp
 * @author Osman Baki ALTIOK
 * Contact: osmanbakialtiok@gmail.com
 */

#include<iostream>			//c++ (cout,cin etc)
#include<unistd.h>			//to getopt()
#include <sys/types.h>
#include <sys/wait.h>		//to use wait(seconds) function.
#include <sstream> 			//to use ostringstream
#include <string>			//to use string operations
#include <time.h>			//to format and print time/date
#include <fstream>

using namespace std;

bool findString(string s1, string s2) {						//String parsing for arguments(getopt())
	if (s1.find(s2) != std::string::npos) {
		return true;
	}
	else
		return false;
}

int createTree(int depth, int x, int getParentPid) {		//Dynamic fork() function.
	if (depth == 0 && x != 0 && getParentPid != 0) {		
		return getParentPid;
	}
	int pid = fork();
	if (x == 0) {

		if (pid > 0) {

			cout << "PID:\t" << getpid() << endl;			
			getParentPid = getpid();
			string log = " >>log";
			ostringstream pstreeStream;
			pstreeStream << "pstree -p " << getParentPid << " >>log";
			string k = pstreeStream.str();
			const char* a = k.c_str();
			system("mkdir -p logs;cd logs/;date>>logAll.log");
			createTree(depth, 1, getParentPid);
			sleep(1);
			if (getParentPid != 0) {

				system("cd logs/;");
				system(a);
			}
		}
		else {

			cout << "PID:\t" << getpid() << "\t" << "PPID:\tforked..." << getppid() << endl;
			sleep(2.1);
		}

	}
	if (pid == 0 && x != 0) {

		sleep(2);
		cout << "PID:\t" << getpid() << "\t" << "PPID:\tforked..." << getppid() << endl;
		x++;
		createTree(--depth, x, getParentPid);
	}
	return getParentPid;
}

int main(int argc, char** argv) {							//Main function

	int depth, x = 0, getParentPid = 0, q = 0;				//q is used for doubling depth,getParentPid is a holder of Parents PID used in pstree, x holds the childs queue just stabilizer.
	int c = 0;
	cout << "Chosen arguments are:" << endl;
	for (int i = 0; i < argc; i++) {

		depth = atoi(argv[i]);
		if (findString(argv[i], "-i")) {

			cout << "Info" << endl;
			system("echo 152120151055 @altiokbaki");
		}
		else if (findString(argv[i], "-d")) {

			cout << "Double the depth" << endl;
			q = 1;
		}
		else if (findString(argv[i], "-t")) {

			cout << "Time" << endl;
			system("date");
		}
	}
	if (depth < 2) {

		cout << "Depth must be <=2" << endl;
		return -1;
	}
	if (q == 1)
		depth *= 2;
	c = createTree(depth - 1, x, getParentPid);

	sleep(4);

	return 0;
}
