#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include"unistd.h"
#include<syslog.h>
  
int main(int argc,char* argv[])
{
	    int fd;
		//setlogmask (LOG_UPTO (LOG_DEBUG));
		openlog("writer", 0, LOG_USER);
        
        

    if(argc==3)
    {
        syslog(LOG_DEBUG, "Number Of Arguments Passed: %d\n", argc);
		
		ssize_t nr;
		const char *buf = argv[2];
		fd = open ( argv[1], O_WRONLY | O_CREAT, 0760 );
		if (fd == -1)
			//perror("open");
			syslog(LOG_ERR, "open");
		else
			nr = write (fd, buf, strlen(buf));
		if (nr == -1)
			//perror("write");
			syslog(LOG_ERR, "write");
		
		if (close (fd) == -1)
			//perror("close");
			syslog(LOG_ERR, "close");
    }
	else
			syslog(LOG_DEBUG, "incorrect number of arguments");
		
		//printf("%s\n", argv[1]);

		closelog();
		
    return 0;
}