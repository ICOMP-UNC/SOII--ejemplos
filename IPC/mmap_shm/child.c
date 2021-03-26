#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* !!!!!!!!!!!!!!! */
#define MPATH "" /* <---- add name in format /segmentname (same as parent of course) */
#define MEM_SIZE 1024
#define DATA_OFFSET 5

int main(){

        int fd = shm_open(MPATH, O_RDWR, 0 );
        if (fd == -1){
                perror("Obtaining shared memory descriptor");
                exit(EXIT_FAILURE);
        }


        int *data = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if ((caddr_t) data == (caddr_t)(-1)){
                perror("child mmap :");
                exit(EXIT_FAILURE);
        }
        
        data[DATA_OFFSET] = 56; 
        munmap((caddr_t) data, MEM_SIZE);
        return 0;
}
