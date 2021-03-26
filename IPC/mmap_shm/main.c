#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

/*  !!!!!!!!!!!!!!!!  */
#define MPATH ""     /* <------ add a name in the format /segmentname */
#define CHILD_BIN "" /* <------ add name of your child process binary */
#define MEM_SIZE 1024
#define DATA_OFFSET 5

int main(){

        pid_t pid;
        int fd = shm_open(MPATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP );
        if (fd == -1){
                perror("Creating shared memory");
                exit(EXIT_FAILURE);
        }

        if (ftruncate(fd, MEM_SIZE) == -1){
                perror("Setting memory size");
                exit(EXIT_FAILURE);
        }


        int *data = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if ((caddr_t) data == (caddr_t)(-1)){
                perror("mmap :");
                exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1){
                perror("Creating child");
        }
        else if (pid){
                int status;
                wait(&status);
                printf("Child process wrote %d at offset %u\n", data[DATA_OFFSET], DATA_OFFSET);
        }
        else{
                execl(CHILD_BIN, "", "", (char *)0);
        }
        
        munmap((caddr_t) data, MEM_SIZE);
        shm_unlink(MPATH);
        return 0;
}
