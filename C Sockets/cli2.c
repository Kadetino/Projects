#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/frfvfvfverereerrvevr.socket"


int main(int argc, char *argv[]) {
	int    sd=-1, rc, bytesReceived;
	char   buffer[8192], SHELL_OUPUT[8192];
	struct sockaddr_un serveraddr;
	
	// SOCKET
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd < 0) {
       perror("socket() failed");
       return 1;
    }

	// struct sockaddr_un
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, SERVER_PATH);

	// CONNECT
    rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
    if (rc < 0) {
       perror("connect() failed");
       return 1;
    }

	// find . -maxdepth 1 -mtime -3 -type f
	// find . -maxdepth 1 -size 100c -type f 
						// find . -maxdepth 1 -size 1k -type f
	/* GNU  find meme: https://unix.stackexchange.com/questions/351532/print-out-list-of-files-less-than-specified-file-size
	* "Note that at least GNU find rounds the size up to full units before applying the less-than test if you give a test like -size -1M. 
	* Meaning that it will only match files that have a size of zero, unlike -size -1048576c which will match files smaller than a (binary) megabyte. "
	*/
	
	// shell shenanigans
	FILE *output;
	memset(buffer, 0, sizeof(buffer));
	memset(SHELL_OUPUT, 0, sizeof(SHELL_OUPUT));
	
	output = popen("find . -maxdepth 1 -mtime -3 -type f", "r");
	if (output == NULL) {
		perror("popen");
		return 1;
	}
	else {
		while (fgets(buffer, 8192, output) !=NULL) {
			strcpy(SHELL_OUPUT+strlen(SHELL_OUPUT), buffer);
		}
	}
	pclose(output);	
    
	// Send
	printf("SENDING:\n%s\nEND OF DATA\n",SHELL_OUPUT);
    rc = send(sd, SHELL_OUPUT, sizeof(SHELL_OUPUT), 0);
    if (rc < 0) {
       perror("send() failed");
       return 1;
    }

	// Close
	if (sd != -1)
		close(sd);
	return 0;
}