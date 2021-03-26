#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FPATH "" /* name of file inside your filesystem */

int main(){


        int fd = open(FPATH, O_RDWR);
        if (fd == -1){
                perror("Opening file");
                exit(EXIT_FAILURE);
        }


        struct stat fs;
        if (fstat(fd, &fs) == -1){
                perror("Obtaining file information");
                exit(1);
        }

        char *data = mmap(NULL, fs.st_size , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if ((caddr_t) data == (caddr_t)(-1)){
                perror("mmap :");
                exit(EXIT_FAILURE);
        }

        /* mmap add a new reference to the file, so we can close() 
         * to eliminate the one added through open()
         */

        close(fd);
        
        /* convert every other character to uppercase */
        for(int i = 0; i < fs.st_size; i+= 2)
                data[i] &= ~32;
 
        /* write modified data to permanent storage */
        if(msync(data, fs.st_size , MS_SYNC)){
                perror("Sync");
        }

        munmap((caddr_t) data, fs.st_size);
        return 0;
}
