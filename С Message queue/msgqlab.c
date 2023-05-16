#include <stdio.h> // FILE
#include <sys/msg.h> // Macros, msgsnd(), msgrcv(), msgget(), msgctl(), struct msqid_ds
//#include <sys/wait.h> //waitpid()
//#include <string.h> // strlen(), strcpy()
//#include <unistd.h> // fork()

#define BUFFER 100

/* Сообщение */
struct message {
    long mtype;
    char mtext[8192];
};

int main(void){
    /* Создать очередь сообщений. */
    int msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (msqid == -1) {
        perror("msgget");
        return 1;
    }

    /* fork */
    int pid = fork();
    if (pid == 0) {
        /* child - Клиент */
		printf("***CLIENT***\n\n");
		/* Достать имя файла текущего каталога, который был модифицирован последним. */
		FILE *output;
		char filename[BUFFER], buffer[BUFFER];
		
		//output = popen("ls -t -a -p /media/sf_GitHub/lab-junk/| grep -v / -m 1", "r");
		output = popen("ls -t -a -p | grep -v / -m 1", "r");
		
		if (output == NULL) {
			perror("popen");
			return 1;
		}
		else {
			while (fgets(buffer, BUFFER-1, output) !=NULL) {
				//printf("output[%d]: %s", 1, buffer);
				strcpy(filename, buffer);
				break;
			}
		}
		pclose(output);
		printf("Last modified file is: %s\n", filename);
		
		/* Создать сообщение. */
        struct message message;
        memset(&(message.mtext), 0, 8192);

        /* Отправить сообщение с другим текстом.*/
		message.mtype = 14;
		strcpy(message.mtext, "main.py");
        if (msgsnd(msqid, &message, strlen(message.mtext), 0) == -1) {
            perror("msgsnd");
            return 1;
        }
		printf("(another) Message sent.\n\n\n");
		
        /* Отправить сообщение. */
		message.mtype = 13;
        strcpy(message.mtext, filename);
        if (msgsnd(msqid, &message, strlen(message.mtext), 0) == -1) {
            perror("msgsnd");
            return 1;
        }
		printf("Message sent.\n\n\n");
    }
	else {
        /* parent */
        /* Ждём child */
        waitpid(pid, NULL, 0);
		printf("***SERVER***\n\n");
		
        /* Получить сообщение из очереди */
        struct message message;
        if (msgrcv(msqid, &message, 8192, -15, 0) == -1) {
            perror("msgrcv");
            return 1;
        }
        printf("Received message text: %s\n", message.mtext);
		
		/* Достать количество строк в указанном файле. */
		FILE *output;
		char wc_reply[BUFFER], buffer[BUFFER], command[BUFFER];
		
		strcpy(command, "wc -l ");
		strcpy(command+strlen(command),message.mtext);
		
		output = popen(command, "r");
		
		if (output == NULL) {
			perror("popen");
			return 1;
		}
		else {
			while (fgets(buffer, BUFFER-1, output) !=NULL) {
				strcpy(wc_reply, buffer);
				break;
			}
		}
		pclose(output);
		printf("Line count:");
		
		int i = 0;
		while (wc_reply[i] != ' '){
			printf("%c", wc_reply[i]);
			i++;
		}
		printf("\n");
		
		/* а также общее число сообщений в очереди. */
		struct msqid_ds ds;
		msgctl(msqid, IPC_STAT, &ds);
		printf("Count of messages left in queue: %d\n", ds.msg_qnum);
		
        /* Удалить очередь сообщений */
        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            return 1;
        }
    }

    return 0;
}
