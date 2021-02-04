#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]){
	
	// create socket
	int sock;
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
    
	if(sock == -1) fprintf(stderr, "Error creating socket: %s \n",strerror(errno));
    			
	// create address
	struct sockaddr_un address;
	memset(&address, 0, sizeof(address));
	address.sun_family = AF_UNIX;
	strncpy(address.sun_path, "/tmp/bankSocket", sizeof(address.sun_path) - 1);

	// try to connect to the server
	if(connect(sock, (struct sockaddr *) &address, sizeof(address)) == -1) fprintf(stderr, "Error connecting to socket: %s \n",strerror(errno));
	
	int pid;
	char buf[80];
	char reply[40];
	int readRet, writeRet;
	
	// fork
	pid = fork();
    
        if(pid == -1) fprintf(stderr, "Error in forking: %s \n",strerror(errno));
                
        else if(pid == 0){
        // fork succeeded
            	while((readRet = read(STDIN_FILENO, buf, 80)) > 0)
            	{
            		// send contents the user wrote
		        writeRet = write(sock, buf, readRet);
		        if(writeRet == -1)
		        {
		        	fprintf(stderr, "Error writing to sock: %s \n",strerror(errno));
		        }
		        if(readRet == -1)
		        {
		         	fprintf(stderr, "Error reading sock: %s \n",strerror(errno));
		        }
            	}
        }
        else{	
        	// read the servers reply and display
        	while(1){
		        if(read(sock, reply, sizeof(reply)) == -1){
		            fprintf(stderr, "Error reading sock: %s \n",strerror(errno));
		        }

		        printf("%s", reply);
		        // reset the reply buffer to prevent errors.
		        memset(&reply, 0, sizeof(reply));
           	 }
        }

	// close connection
	close(sock);
	exit(EXIT_SUCCESS);
}
