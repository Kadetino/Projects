#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>

/**/
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define PROJECT_ID 1444

static struct sembuf inc[1] = {{ 0, 2, 0 }};
static struct sembuf dec[1] = {{ 0, -1, 0 }};
static struct sembuf decquad[1] = {{ 0, -4, 0 }};
static struct sembuf inc2[1] = {{ 0, 5, 0 }};

int main(void){
	typedef struct mem_msg {
        char mtext[8192];
		//char msgContent[2048];
    } message;
	
	// IPC key
	key_t IPC_key;
    if ((IPC_key = ftok("/media/sf_GitHub", PROJECT_ID)) == -1) {
        perror("ftok");
        return 1;
    }
	
    // shmget + semget
    int shmid;
	while(1){
        shmid = shmget(IPC_key, 0, 0);
        if(shmid == -1){
            perror("shmget");
            sleep(3);
        } else 
            break;
    }
    /*if (shmid == -1) {
        perror("shmget");
        return 1;
    }*/
	int semid;
    /*if (semid == -1) {
        perror("semget");
        return 1;
    }*/
	while(1){
        semid = semget(IPC_key, 0, 0);
        if(semid == -1){
            perror("semget");
            sleep(3);
        } else 
            break;
    }

	// shmat
    message *temp;
	if ((temp = (message*)shmat(shmid, NULL, 0)) == -1) {
        perror("shmat");
        return 1;
    }
	// Occupy - 1->0|2->1
	if (semop(semid, &dec[0], 1) == -1) {
        perror("semop");
        return 1;
    }
	// ps shenanigans (shell)
	FILE *output;
	char PS_OUTPUT[8192], buffer[8192], command[100], STR_PID[50];
	strcpy(command,"ps -o state= -p ");
	strcpy(buffer, temp->mtext);
	
	int i = 0;
	while (buffer[i] == ' '){
			i++;
		}
	while (buffer[i] != ' '){
			//printf("%c", buffer[i]);
			i++;
		}
	strncpy(STR_PID,buffer,i);
	strcpy(command+strlen(command), STR_PID);
	output = popen(command, "r");
	
	if (output == NULL) {
		perror("popen");
		return 1;
	}
	else {
		memset(buffer, 0, 8192);
		while (fgets(buffer, 8192-1, output) !=NULL) {
			//printf("Client 1 result: %s", buffer);
			strcpy(PS_OUTPUT, "Client 1 result:");
			strcpy(PS_OUTPUT+strlen(PS_OUTPUT), buffer);
			break;
		}
	}
	pclose(output);
	
	printf("%s",PS_OUTPUT);
	

	// Release 0->2|1->3
    if (semop(semid, &inc[0], 1) == -1) {
        perror("semop");
        return 1;
    }
	// #2 Occupy - 4-4
	if (semop(semid, &decquad[0], 1) == -1) {
        perror("semop");
        return 1;
	}
	strcpy(temp->mtext+strlen(temp->mtext), PS_OUTPUT);
	
	// Release+5
    if (semop(semid, &inc2[0], 1) == -1) {
        perror("semop");
        return 1;
    }
		
// Clean up
	// shm
	if (shmdt(temp) == -1) {
            perror("shmdt");
            return 1;
	}

    return 0;
}