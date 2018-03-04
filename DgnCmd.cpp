#include <stdio.h>
#include "DgnShell.h"

#define MAXLINE 80 /* The maximum length command */
int main(void)
{
	char *args[MAXLINE / 2 + 1]; /* command line arguments */
	int shouldrun = 1; /* flag to determine when to exit program */
	DgnShell* shell = new DgnShell();


	while (shell->getExitStatus() == false) {
		printf("osh>");
		fflush(stdout);
		/**
		* After reading user input, the steps are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if command included &, parent will invoke wait()
		*/
		std::string check = "";
		std::getline(std::cin, check);


		shell->SendArgs(check, args);
	}
	return 0;
}