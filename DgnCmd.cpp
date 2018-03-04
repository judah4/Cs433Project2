#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <string>

#define MAXLINE 80 /* The maximum length command */


void Process(std::string check, char * args[])
{
	pid_t pid = fork();
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		//return 1;
	}
	else if (pid == 0) { /* child process */

		execvp(args[0], args);
		exit(EXIT_FAILURE);  //if the child process fails, exit the child process.
	}
	else if (check != "&") {/* parent will wait for the child to complete */

		wait(NULL);

	}
	else
	{
		return;
	}
}

int main(void)
{
	char *args[MAXLINE / 2 + 1]; /* command line arguments */
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



	}
	return 0;
}