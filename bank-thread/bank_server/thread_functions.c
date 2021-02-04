#include "thread_functions.h"
#include "commands.c"


int listenClient(int sock){

	char buffer[100];
	char response[100];
	ssize_t read_ret,write_ret;
	while(1){
		// read from socket
		read_ret = read(sock, buffer, 100);
		// error reading
		if(read_ret < 0){ // error
			fprintf(stderr, "Error reading socket: %s \n",strerror(errno));
		}
		// client closes connection
		else if(read_ret == 0){
			printf("Client left the desk. Connection closed \n");
			close(sock);
			break;
		}
		// commands incoming
		else{
			// check if command was successfull and respond to client
			if(checkCommand(buffer) >= 0 ){
				strcpy(response,"Command successful.\n");
				write_ret = write(sock,response,sizeof(response));
			}
			else{
				strcpy(response,"Command failed. Invalid input.\n");
				write_ret = write(sock,response,sizeof(response));
			}
			
			
			if(write_ret == -1){
				fprintf(stderr, "Error writing to socket: %s \n",strerror(errno));			
			} 
		
	
		}
	
	}
	return 0;

}
