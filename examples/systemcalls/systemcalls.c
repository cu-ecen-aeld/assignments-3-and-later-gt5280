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

	pid_t pid;	
	pid = fork();	
		
	if(pid == -1){		
		perror ("fork");
	
	}	
	
	else if (pid == 0){
		
		int ret;
		printf("I'm here\n");
		
		if ((ret = execv(command[0], command)) == -1  ) 
				return false;
	}

		int status;
	
		if (waitpid (pid, &status, 0) == -1)
			return -1;    
			else if (WIFEXITED (status))        
			return WEXITSTATUS (status);
	
		va_end(args);

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

	pid_t pid;	
	pid = fork();	
		
	if(pid != 0)
	return false;

	else if (pid == 0){

		int ret;
		
		if( outputfile != NULL ) {
                int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);     // write this open command
                if (fd < 0)
					return false;

                if (dup2(fd, 1) < 0)         // write this command
					return false;

                close(fd);
            }
            if ((ret = execv(command[0], command)) != 0)  
				return false;

	}

		int status;
	
		if (waitpid (pid, &status, 0) == -1)
			return false;    
			else if (WIFEXITED (status))        
			return WEXITSTATUS (status);

    va_end(args);
    
    return true;
}


 
int main (){

	bool i;
	
	i= do_exec(2, "echo", "Testing execv implementation with echo");
	
	//i= do_exec(2, "test/test_fork", "test_fork");
	
	//i= do_exec_redirect("two.txt", 3, "/bin/sh", "-c", "cat one.txt > test/two.txt");
	
	if (i = false)
		printf("do_exec returned false\n");
	else
		printf("do_exec returned true\n");

	return 0;
	
	}
