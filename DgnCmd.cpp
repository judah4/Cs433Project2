#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <string.h>

#define MAXLINE 80 /* The maximum length command */


void Process(std::string check, char* args[])
{
	pid_t  pid;
	int    status;
	printf("processing...");
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

void  parse(char *line, char **argv)
{
	while (*line != '\0') {       /* if not the end of line ....... */
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';     /* replace white spaces with 0    */
		*argv++ = line;          /* save the argument position     */
		while (*line != '\0' && *line != ' ' &&
			*line != '\t' && *line != '\n')
			line++;             /* skip the argument until ...    */
	}
	*argv = '\0';                 /* mark the end of argument list  */
}

int main(void)
{
	char* args[MAXLINE / 2 + 1]; /* command line arguments */
	bool shouldrun = true; /* flag to determine when to exit program */
	std::vector<std::string> history;
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
		char * checkArgs = strdup(check.c_str());
		parse(checkArgs, args);

		Process(check, args);

	}
	return 0;
}