#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
/*
18.Написать программу копирования содержимого одного файла в другой, создавая последний в случае необходимости,
и оформить ее в виде команды интерпретатора shell ОС UNIX с выдачей сообщения о количестве блоков копируемого файла (блок = 512 байт) и байтов.
Предусмотреть аномальные ситуации и возможность прерывания от клавиатуры.
*/
//Первый аргумент - имя файла-источника. Второй аргумент - конечный файл.

static volatile int src = 0;

// Прерывание
void interrupter(int dummy) {
    printf("\n----Прерывание----\n");
    if (src==0) printf("\nВведите имя файла-источника: ");
    if (src==1) printf("\nВведите имя konechnogo file: ");
printf("\n");
}

int main(int argc, char *argv[]) {
    int fileSource = -1, fileTarget = -1;
    char buffer[1024], temp[128];
    long int read_bytes = 1, write_bytes;
    struct stat sb;

    //Прерывание
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sigprocmask(0,0,&sa.sa_mask);
    sa.sa_handler = interrupter;
    sa.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &sa, NULL);
    // Прерывания old
    //signal(SIGINT, interrupter);

    // 2 Аргумента: оба файла
    if (argc==3){
        fileSource = open(argv[1], O_RDONLY);
        // Check
        if (fileSource == -1) {
            printf("Не удалось открыть файл-источник.\n");
            while(1) {
                printf("Введите имя файла-источника: ");
                scanf("%s", temp);
                fileSource = open(temp, O_RDONLY);
                if (fileSource == -1) printf("Недопустимый файл. Попробуйте ещё раз.\n");
                else {
                    src = 1;
                    printf("----\nИмя файла источника: %s\n----\n", temp);
                    break;
                }
            }
        }
        else {printf("----\nИмя файла источника: %s\n----\n",argv[1]); src=1;}

        fileTarget = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0777);
        if (fileTarget == -1) {
            printf("Не удалось открыть файл-источник.\n");
            while(1) {
                printf("Введите имя конечного файла: ");
                scanf("%s", temp);
                fileTarget = open(temp, O_CREAT | O_WRONLY | O_TRUNC, 0777);
                if (fileTarget == -1) printf("Недопустимый файл. Попробуйте ещё раз.\n");
                else {
                    src = 2;
                    printf("----\nИмя конечного файла: %s\n----\n", temp);
                    break;
                }
            }
        }
        else {printf("----\nИмя конечного файла: %s\n----\n",argv[2]); src = 2;}



        if((fileSource == -1) || (fileTarget == -1)) {
            printf("Не удалось открыть файл-источник или конечный файл.\n");
            if((fileSource == -1) && (fileTarget != -1)) unlink(argv[2]);
            exit(2);
        }
        

    }
    // 1 Аргумент: только источник
    else if (argc==2) {
        // Источник
        fileSource = open(argv[1], O_RDONLY);
        if (fileSource == -1) {
            printf("Не удалось открыть файл-источник.\n");
            while(1) {
                printf("Введите имя файла-источника: ");
                scanf("%s", temp);
                fileSource = open(temp, O_RDONLY);
                if (fileSource == -1) printf("Недопустимый файл. Попробуйте ещё раз.\n");
                else {
                    printf("----\nИмя файла источника: %s\n----\n", temp);
                    src = 1;
                    break;
                }
          printf("----\nИмя файла-источника: %s\nИмя конечного файла: %s\n----\n",argv[1],argv[2]);  }
        }

        // конечный
        printf("Не было предоставлено название конечного файла.\n");
        while(1) {
            printf("Введите имя конечного файла: ");
            scanf("%s", temp);
            fileTarget = open(temp, O_CREAT | O_WRONLY | O_TRUNC, 0777);
            if (fileTarget == -1) printf("Недопустимый файл. Попробуйте ещё раз.\n");
            else {
                printf("----\nИмя конечного файла: %s\n", temp);
                src=2;
                break;
            }
        }
    }
    else {
        printf("Не было предоставлено названий файлов из 2х требуемых.\n");
        while(1) {
            printf("Введите имя файла-источника: ");
            scanf("%s", temp);
            fileSource = open(temp, O_RDONLY);
            if (fileSource == -1) printf("Недопустимый файл. Попробуйте ещё раз.\n");
            else {
                printf("----\nИмя файла источника: %s\n----\n", temp);
                src =1;
                break;
            }
        }
        while(1) {
            printf("Введите имя конечного файла: ");
            scanf("%s", temp);
            fileTarget = open(temp, O_CREAT | O_WRONLY | O_TRUNC, 0777);
            if (fileTarget == -1) printf("Недопустимый файл. Попробуйте ещё раз.\n");
            else {
                printf("----\nИмя конечного файла: %s\n----\n", temp);
                src =2;
                break;
            }
        }
    }
    //printf("Source fd %d Target fd %d debug\n",fileSource,fileTarget);
    // Сообщение о копировании
    if (fstat(fileSource,&sb) != -1) {
        printf("Копируемый файл имеет %ld байтов и %ld блоков размером 512 байт.\n",sb.st_size,sb.st_blocks);
    }
    else exit(3);
    // Копирование
    while (read_bytes > 0) {
        read_bytes = read(fileSource, buffer, 1024);
        write_bytes = write(fileTarget, buffer, read_bytes);
        if( write_bytes != read_bytes){
            exit(3);
        }
    }
    close(fileSource);
    close(fileTarget);
    exit(0);
}
