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
		openlog("writer", 0, LOG_USER);
  
    if(argc==3)
    {
		ssize_t nr;
		const char *buf = argv[2];
		
		fd = open ( argv[1], O_WRONLY | O_CREAT, 0760 );
		if (fd == -1)
			syslog(LOG_ERR, "open");
		else{
			nr = write (fd, buf, strlen(buf));
			syslog(LOG_DEBUG, "Writing %s to %s", buf, argv[1]);
		}
		if (nr == -1)
			syslog(LOG_ERR, "write");
		
		if (close (fd) == -1)
			syslog(LOG_ERR, "close");
    }
	else
			syslog(LOG_DEBUG, "incorrect number of arguments");

		closelog();
		
    return 0;
}