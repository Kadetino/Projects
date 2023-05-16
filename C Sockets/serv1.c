#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/frfvfvfverereerrvevr.socket"

#define BUFFER_LENGTH 8192

int main() {
	int    serv_d=-1, fd1=-1, fd2=-1, rc;
	char   buffer[BUFFER_LENGTH], storage[BUFFER_LENGTH];
	memset(buffer, 0, sizeof(buffer));
	memset(storage, 0, sizeof(storage));
	struct sockaddr_un server_addr;
   
   // SOCKET
	serv_d = socket(AF_UNIX, SOCK_STREAM, 0);
	if (serv_d < 0) {
		perror("socket() failed");
		return 1;
	}
	
	// struct sockaddr_un
	unlink(SERVER_PATH);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SERVER_PATH);
	
	// BIND
	rc = bind(serv_d, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (rc < 0) {
		perror("bind() failed");
		return 1;
	}
	
	// LISTEN
	rc = listen(serv_d, 2);
	if (rc< 0) {
		perror("listen() failed");
		return 1;
	}
	
	printf("PREPARATION COMPLETED.\n");
	printf("ACCEPTING 1st CLIENT\n");
	// ACCEPT
	fd1 = accept(serv_d, NULL, NULL);
	if (fd1 < 0) {
		perror("accept() failed");
		return 1;
	}
	
	// RECEIVE
	rc = recv(fd1, buffer, sizeof(buffer), 0);
	if (rc < 0) {
		perror("recv() failed");
		return 1;
	} 
	
	//printf("Start of received data:\n%s\nEND OF DATA\n",buffer);
	strcpy(storage, buffer);
	memset(buffer, 0, sizeof(buffer));
	if (rc == 0) {
		printf("this line shoudln't be seen");
		return 1;
	}
	printf("ACCEPTING 2nd CLIENT\n");
	// ACCEPT
	fd2 = accept(serv_d, NULL, NULL);
	if (fd2 < 0) {
		perror("accept() failed");
		return 1;
	}
	
	// RECEIVE
	rc = recv(fd2, buffer, sizeof(buffer), 0);
	if (rc < 0) {
		perror("recv() failed");
		return 1;
	} 
	
	//printf("Start of received data2:\n%s\nEND OF DATA2\n",buffer);
	if (rc == 0) {
		printf("this line shoudln't be seen");
		return 1;
	}
	
	
	
	
	// Task
	char *pch1;
	char *pch2;
	pch1 = strtok(storage , "\n");
	while (pch1 != NULL) {
		//printf("%s\n",pch1);
		pch2 = strtok(buffer, "\n");
			while (pch2 != NULL) {
				
				if (strcmp(pch1, pch2) == 0) {
					printf("FOUND ONE: %s\n", pch1);
				}
				else{
					//printf("%s | %s | %d\n", pch1, pch2, strcmp(pch1, pch2));
				}
				pch2 = strtok(NULL, "\n");
			}
		pch1 = strtok(NULL, "\n");
	}
	
	
	//Clean up
   if (serv_d != -1)
      close(server_addr);

   if (fd1 != -1)
      close(fd1);
   if (fd2 != -1)
      close(fd2);
  
   unlink(SERVER_PATH);
   
   return 0;
}