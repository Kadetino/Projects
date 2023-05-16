#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h> 
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
	
	// Occupy
	if (semop(semid, &dec[0], 1) == -1) {
        perror("semop");
        return 1;
    }
	
	// shenanigans
	char buffer[8192], answer_msg[2048], str_temp[10], other_temp[10];
	strcpy(buffer, temp->mtext);
	
	int x, lowest_priority=100;
	char *ptr = strtok(buffer, "\n");

	while(ptr != NULL) {
		//printf("%s\n",ptr+(strlen(ptr)-2));
		sscanf(ptr+(strlen(ptr)-3), "%d", &x);
		//printf("\nThe integer value of x is %d\n", x);
		if (x < lowest_priority) {
			lowest_priority = x;
		}
		ptr = strtok(NULL, "\n");
	}
	sprintf(str_temp, "%d", lowest_priority);
	strcpy(answer_msg+strlen(answer_msg), str_temp);
	strcpy(answer_msg+strlen(answer_msg), " <-- Lowest priority around here! -  - the other thingy: ");
	printf("%d <------------ Lowest priority around here!\n", lowest_priority);
	
	
	struct semid_ds ds;
	msgctl(semid, IPC_STAT, &ds);
	printf("the other thingy: %d\n", ds.sem_nsems);
	sprintf(other_temp, "%d", ds.sem_nsems);
	strcpy(answer_msg+strlen(answer_msg), other_temp);
	strcpy(answer_msg+strlen(answer_msg), "\n");
	
	printf("%s",answer_msg);
	
	// Release
	if (semop(semid, &inc[0], 1) == -1) {
        perror("semop");
        return 1;
    }
	
	// #2 Occupy - 4-4
	if (semop(semid, &decquad[0], 1) == -1) {
        perror("semop");
	return 1;}
	strcpy(temp->mtext+strlen(temp->mtext), answer_msg);
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