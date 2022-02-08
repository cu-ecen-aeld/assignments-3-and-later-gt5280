#include "systemcalls.h"
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include"unistd.h"
#include<syslog.h>
#include<stdint.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<errno.h>
#include<stdbool.h>



/**
 * @param cmd the command to execute with system()
 * @return true if the commands in ... with arguments @param arguments were executed 
 *   successfully using the system() call, false if an error occurred, 
 *   either in invocation of the system() command, or if a non-zero return 
 *   value was returned by the command issued in @param.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success 
 *   or false() if it returned a failure
*/

	int status;

		status= system(cmd);
		if (status < 0)
			return false;
		else if (WIFEXITED (status) == 0){	
			printf("system returned: %d\n", WIFEXITED (status) );
			return true;
		}
		else
			return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the 
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;

    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *   
*/
	int status;
	pid_t pid, wpid;	
	pid = fork();	

if (pid==0){
	execv(command[0], command);
		perror("ERROR execv");
		exit(EXIT_FAILURE);
}

else if (pid > 0){	
	wpid= waitpid (pid, &status, 0);
	if(wpid == -1)
		perror("ERROR waitpid");
}
	
else if (pid == -1)
	perror("ERROR fork");

if (WEXITSTATUS(status))
	return false;
else
	return true;

}

/**
* @param outputfile - The full path to the file to write with command output.  
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *   
*/
	pid_t pid, wpid;	
	int status;
	

     int fd = open( outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644 ); 
	if ( fd== -1 )
		return false;

	pid = fork();	
		
	if(pid == -1){
		perror ("ERROR fork");
		return false;
	}
	else if (pid == 0){

                if (dup2(fd, 1) < 0) {
					perror("ERROR dup2"); 
					return false;
				}
				
				execv(command[0], command);
				perror("ERROR execv");
                exit(EXIT_FAILURE);				
            }
	else{
				close(fd);
				wpid = waitpid (pid, &status, 0);

				if(wpid == -1){
					perror("ERROR waitpid");
					return false;
				}
				
				if (! WIFEXITED (status) || WEXITSTATUS(status))        
					return false;
	}
   
		va_end(args);
		
		return true;
}

/*

int main (){

	bool i;
	
	do_exec_redirect("~/myproject/assignment-1-gt5280/testfile.txt", 2, "/bin/echo", "home is $HOME");
	char *test_string = malloc( sizeof(char) * (25) );
	
	//i= do_exec(2, "echo", "Testing execv implementation with echo");
	
	//i= do_exec(3, "/usr/bin/test","-f","/bin/echo");
	
	//i= do_exec_redirect("two.txt", 3, "/bin/sh", "-c", "cat one.txt > test/two.txt");
	
	if (i == false){
		printf("do_exec returned false\n");
		return 0;
	}
	else{
		printf("do_exec returned true\n");

	return 0;
	}
	
	
	}
	
*/