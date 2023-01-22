/**  smsh1.c  small-shell version 1
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "smsh.h"

#define	DFL_PROMPT	"> "

void exit_sh(int);
char* cd(char*);

int main()
{
  char	*cmdline, *prompt, **arglist;
  int	result;
  void	setup();

  char cwd[256];
  getcwd(cwd, sizeof(cwd));

  prompt = strcat(cwd, DFL_PROMPT);
  setup();

  while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
    if ( (arglist = splitline(cmdline)) != NULL  ){
      if (strcmp(arglist[0], "exit") == 0)
      {
        // Handle exit
        if (arglist[1])
        {
          exit_sh(atoi(arglist[1]));
        }
        else
        {
          exit(0);
        }
      }
      else if (strcmp(arglist[0], "cd") == 0)
      {
        // Handle cd
        if (arglist[1])
        {
          chdir(arglist[1]);
        }
        else 
        {
          chdir(getenv("HOME"));
        }
        getcwd(cwd, sizeof(cwd));
        prompt = strcat(cwd, DFL_PROMPT);
      }
      else
      {
        result = execute(arglist);
      }  
      freelist(arglist);
    }
    free(cmdline);
  }
  return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
  fprintf(stderr,"Error: %s,%s\n", s1, s2);
  exit(n);
}

/*
  Exits the shell with the specified exit code
*/
void exit_sh(int exit_code)
{
  printf("Exit with code %d\n", exit_code);
  exit(exit_code);
}
