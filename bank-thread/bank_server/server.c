#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "thread_functions.c"




// global variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int max = 8;
int cl_sock; // client_sock
int curr_desks = 0; // number of desks in use

// Struct for a thread that has its id and number
typedef struct{
	int thread_num;
	pthread_t thread_id;
} thread_info;

thread_info *threads;


static void* thread_start(void* arg){
	
	pthread_t thread_id = pthread_self();
	int thread_num = *((int*) arg);
	pthread_detach(thread_id);
	printf("Thread %d created,  ID:  %ld\n", thread_num, thread_id);
	while(1){
		
		pthread_mutex_lock(&mutex);
		while(1){
			if(pthread_cond_wait(&cond,&mutex) == 0)
				break;
		}
		printf("Client at thread/desk %d \n", thread_num);

		int cl_fd = cl_sock;

		// start listening to client commands
		pthread_mutex_unlock(&mutex);
		listenClient(cl_fd);
		pthread_mutex_lock(&mutex);
		curr_desks--;
		pthread_mutex_unlock(&mutex);	
	}
	return NULL;
}



int main(int argc, char *argv[])
{
	// Open the log file
	FILE* logfile;
	logfile = fopen("server.log","w");
	fprintf(logfile,"Started running the program.. \n");

	// Check user input
	if (argc != 2){
        	fprintf(stderr, "Give arguments in format: ./server amountofdesks  \nFor example : ./server 4 \n");
        	exit(EXIT_FAILURE);
	}

	int threads_len = (int) strtol(argv[1], NULL, 10);
	if (threads_len > max){
        	fprintf(stderr, "You entered too many desks. Max is 8. \n");
        	exit(EXIT_FAILURE);
	}

	// Create socket for listening to client sockets
	int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(listen_fd == -1) fprintf(stderr,"Error creating socket %s \n",strerror(errno));


	// reset socketpath
	unlink("/tmp/bankSocket");
	
	// Address struct
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "/tmp/bankSocket", sizeof(addr.sun_path) - 1);

	// Bind listening socket to the address
	if(bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) fprintf(stderr,"Error binding socket %s \n",strerror(errno));


   	// make the main socket to listen for connections
   	// passive socket, backlog of 10
	if(listen(listen_fd, 10) == -1) fprintf(stderr,"Error making socket listen %s \n",strerror(errno));
    	
    	// reserve space for the thread structs
	threads = calloc(threads_len, sizeof(thread_info));

	
	printf("Created the requested %d threads.\n", threads_len);
	for(int i = 0; i < threads_len; i++){
		threads[i].thread_num = i;
		
		if (pthread_create(&threads[i].thread_id, NULL, &thread_start, (void *)&threads[i].thread_num) != 0){
		    fprintf(stderr,"Error creating thread %s \n",strerror(errno));
		}
	}

	// Loop for accepting connections
	printf("Bank is now open.\n");
	printf("Press CTRL-C to close bank.\n");
	// logfile
	fprintf(logfile,"Bank opened successfully. \n");
	fclose(logfile);
	int client_fd;
	while(1){
		client_fd = accept(listen_fd, NULL, NULL);
		if(client_fd == -1){
		   fprintf(stderr,"Error accepting connection %s \n",strerror(errno));
		}
		
		printf("Socket: %d connected.\n", client_fd);
		// mutex	
		pthread_mutex_lock(&mutex);
		
		// if space
		if(curr_desks < threads_len){
			cl_sock = client_fd;
			curr_desks = curr_desks + 1;
		}
		// if all threads are full
		else{   
			
			// QUEUE HERE
			// try to implement queue if time
			printf("Implement queue (maybe FIFO pipes..)\n");
			
			// currently respond to client
			char response[50];
			strcpy(response,"No available threads/desks. \n");			
			if(write(client_fd, response, sizeof(response)) ==  -1){
		        	fprintf(stderr,"Write error %s \n",strerror(errno));
			}
			close(client_fd);
			printf("Socket: %d disconnected, no spaces.\n",client_fd); 
		}

		// signal thread
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	

	//close main socket and delete the socket path
	if(close(listen_fd)){
        	fprintf(stderr,"Error closing listening socket %s \n",strerror(errno));
	}

	if(unlink("/tmp/bankSocket") == -1){
        	fprintf(stderr,"Error unlinking socket path %s \n",strerror(errno));
	}
	return 0;
}










