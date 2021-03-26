#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

/* !!!!!!!!!!!!!!!!!!!!!!!!!! */
#define QUEUEPATH "" /* <--- add some name to que queue in the following format : /qname */


#define MSG_SIZE 5



int main(){
        
        struct mq_attr queue_atributes = {0};
        /* Setting queue atributes.
         *
         * Default and max values can be viewed/modified through /proc interfaces
         * explained in /proc interfaces section of man 7 mq_overview
         */
        queue_atributes.mq_maxmsg  = 10 ;
        queue_atributes.mq_msgsize = MSG_SIZE ;

        /* permission macros(the ones starting with S_) can vi viewed in man 2 open */
        mqd_t qd = mq_open(QUEUEPATH, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, &queue_atributes);
        if (qd == -1){
                perror("Creating queue");
                exit(EXIT_FAILURE);
        }
        char sent_msg[5] = "hola";
        if (  mq_send(qd, sent_msg, MSG_SIZE, (unsigned int) 1) == -1){
                perror("Sending");
                exit(EXIT_FAILURE);
        }
        
        pid_t chpid = fork();
        if (chpid == -1){
                perror("Creating child process");
                goto delete;
        }
        else if (chpid){
                printf("press enter to terminate process \n");
                getchar();
        }
        else {
                
                char msg[MSG_SIZE]= "";
                unsigned int prio = 1;
                if (mq_receive(qd, msg, MSG_SIZE, &prio) == -1 ){
                        perror("Receiving");
                        exit(EXIT_FAILURE);
                }
                if (mq_close(qd) == -1){
                        perror("Closing queue");
                }
                printf("Message received  : %s \n", msg);
                exit(EXIT_SUCCESS);
        }
delete:
        if (mq_unlink(QUEUEPATH) == -1){
                perror("Destroying queue");
        }
        return 0;

}

