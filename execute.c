/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
  int bg_flag = 0;
  int	pid;
  int	child_info = -1;

  // Check for '&' char in args
  int i = 0;
  while (argv[i] != NULL)
  {
    // If & arg is found, set flag and remove argument
    if (strcmp(argv[i], "&") == 0)
    {
      bg_flag = 1;
      argv[i] = '\0';
      break;
    }
    i++;
  }

  if ( argv[0] == NULL )		/* nothing succeeds	*/
    return 0;
  
  if ( (pid = fork())  == -1 )
    perror("fork");
  else if ( pid == 0 ){
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(argv[0], argv);
    perror("cannot execute command");
    exit(1);
  }
  else {
    if (!bg_flag)
    {
      if (waitpid(pid, &child_info, WCONTINUED) == -1)
      {
        perror("wait");
      }   
    }
    else
    {
      printf("Started background job with pid #%d\n", pid);
    }
  }

  // Check for finished background processes
  if (pid = waitpid(-1, &child_info, WNOHANG) > 0)
  {
    printf("Completed process #%d\n", pid);
  }

  // Return
  return child_info;
}
