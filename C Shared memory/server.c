#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
//#include <sys/ipc.h> 
//#include <sys/types.h> 
/**/
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define PROJECT_ID 1444

static struct sembuf dec[1] = {{ 0, -3, 0 }};
static struct sembuf inc[1] = {{ 0, 1, 0 }};
static struct sembuf dec2[1] = {{ 0, -6, 0 }};


int main(void){
	typedef struct mem_msg {
        char mtext[8192];
		//char msgContent[2048];
    } message;
	
	
	
	
	union semun {
        int val;                // Value for SETVAL
        struct semid_ds *buf;   // Buffer for IPC_STAT, IPC_SET
        unsigned short *array;  // Array for GETALL, SETALL
        struct seminfo *__buf;  // Buffer for IPC_INFO (Linux specific)
    };
	
	// IPC key
	key_t IPC_key;
    if ((IPC_key = ftok("/media/sf_GitHub", PROJECT_ID)) == -1) {
        perror("ftok");
        return 1;
    }
	
    // shmget + semget
    int shmid = shmget(IPC_key, sizeof(message), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }
	int semid = semget(IPC_key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        return 1;
    }

	// shmat
    message *temp;
	if ((temp = (message*)shmat(shmid, NULL, 0)) == -1) {
        perror("shmat");
        return 1;
    }
	
	// ps shenanigans (shell)
	FILE *output;
	char PS_OUTPUT[8192], buffer[8192];
	output = popen("ps -U huhh -o pid=,time=,tty=,priority= --sort -time", "r");
	if (output == NULL) {
		perror("popen");
		return 1;
	}
	else {
		while (fgets(buffer, 8192-1, output) !=NULL) {
			strcpy(PS_OUTPUT+strlen(PS_OUTPUT), buffer);
		}
	}
	pclose(output);
	strcpy(temp->mtext, PS_OUTPUT);
	// debug
	printf("Debug:\n{\"mtext\": \n\"%s\"}\n", temp->mtext);
	
	
	
	// setup semaphore - 1
	if (semop(semid, &inc[0], 1) == -1) {
        perror("semop");
        return 1;
    }
	// waiting - until 0 (2->3)
	printf("Waiting for 2 clients to finish....\n");
    if (semop(semid, &dec[0], 1) == -1) {
        perror("semop");
        return 1;
    }
	memset(temp->mtext,0,8192);
	
	union semun arg;
    arg.val = 4;
	semctl(semid, 0, SETVAL, arg);
	// wait - 6-6
	printf("Waiting for 2 clients to respond....\n");
	if (semop(semid, &dec2[0], 1) == -1) {
        perror("semop");
        return 1;
    }
	printf("Resuming...\nWhat clients told us....:\n\n%s",temp->mtext);
	// Clean up
	// shm
	if (shmdt(temp) == -1) {
            perror("shmdt");
            return 1;
	}
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            return 1;
	}
	// sem
	if (semctl(semid, 1, IPC_RMID) == -1) {
            perror("semctl");
            return 1;
	}
    return 0;
}