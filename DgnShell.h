#pragma once

#include <sstream>
#include <vector>
#include <unistd.h>
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

class DgnShell
{
private:
	
	std::string m_homedir;
	pid_t m_pid; //process ID for child process
	std::vector<std::string> m_history;
	void histArg(std::string check, char * args[]);
	void regularCheck(std::string check, char * args[]);
	std::string userName;
	std::string hostName;
	void initShell();//initialize the shell and display the cwd on the screen
	std::string concatCommand;
	bool exitStatus; /* flag to determine when to exit program */
	int argsIndex;
	int histIndex;
public:
	DgnShell();
	~DgnShell();
	void SendArgs(std::string check, char* args[]);
	bool getExitStatus();
};

