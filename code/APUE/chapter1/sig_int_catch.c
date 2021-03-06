#include "apue.h"
#include <sys/wait.h>


static void sig_int(int);	/* our signal-catching function */


int
main(void)
{
	char buf[MAXLINE];	/* constant from apue.h */
	pid_t	pid;		/* For forked process   */
	int 	status;
	
	/*
	*	Part of the code to set a method for catching
	*	a signal
	*/
	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal error");


	printf("APUE Simple Shell\n");

	printf("%% "); /* print prompt (printf requires %% to print % because it's used for formats) */
	while (fgets(buf, MAXLINE-1, stdin) != NULL)
	{
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf)-1] = '\0'; /* replace newline with null */

		pid = fork(); // fork the process
		
		// from here, this code will be executed from
		// parent and son process
		if (pid < 0)
		{
			err_sys("fork error");
		}
		else if (pid == 0) // value returned from for for child process
		{
			printf("[CHILD] Executing: \"%s\"\n", buf);
			execlp(buf, buf, (char *)0);
			err_ret("couldn't execute: %s", buf);
			exit(127);
		}
		else // for parent, pid returns son's pid
		{
			printf("[PARENT] waiting for %ld\n", (long)pid);

			if ((pid = waitpid(pid, &status, 0)) < 0)
				err_sys("waitpid error");
			printf("%% "); // again prompt
		}
	}

	exit(0);
}

void
sig_int(int signo)	/* Method to catch the interruptions */
{
	printf("interrupt CTRL+C pressed\n%% ");
}
