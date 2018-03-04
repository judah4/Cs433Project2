#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/wait.h>

#define MAXLINE 80 /* The maximum length command */


void Process(std::string check, char* args[])
{
	pid_t  pid;
	int    status;

	if ((pid = fork()) < 0) {     /* fork a child process           */
		printf("*** ERROR: forking child process failed\n");
		exit(1);
	}
	else if (pid == 0) {          /* for the child process:         */
		printf("child>");
		if (execvp(args[0], args) < 0) {     /* execute the command  */
			printf("*** ERROR: exec failed\n");
			exit(1);
		}

	}
	else {                                  /* for the parent:      */
		while (wait(&status) != pid)       /* wait for completion  */
			;
	}

}

int main(void)
{
	char* args[MAXLINE / 2 + 1]; /* command line arguments */
	bool shouldrun = true; /* flag to determine when to exit program */

	while (shouldrun) {
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

		if(check == "exit")
		{
			shouldrun = false;
			continue;
		}

		Process(check, args);

	}
	return 0;
}