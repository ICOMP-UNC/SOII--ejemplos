#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>

/*   !!!!!!!!!!!!!!!!!!  */
#define QUEUEPATH "" /* <----- add path to file used to represent queue(must be an existing file) */

#define MSGTYPE1   22 // ID for massages from parent to child
#define MSGTYPE2   23 // ID for messages from child to parent

/* custom message */
struct usrmsg {
        int id;
        char info;
};


struct msgbuf {
        long mtype; /* mandatory queue message member*/
        struct usrmsg msg;
};

int main(){

        struct msgbuf parent_msg = {MSGTYPE1, {10,'P'}};

        key_t qkey = ftok(QUEUEPATH, 66);

        if (qkey == -1){
                perror("Obtaining token");
                exit(EXIT_FAILURE);
        }

        /* queue creation */
        int   qid = msgget(qkey, 0666 | IPC_CREAT); 
        if(qid == -1){
                perror("Creating queue");
                exit(EXIT_FAILURE);
        }

        pid_t chpid = fork();
        if (chpid == -1){
                perror("Creating child process");
                goto delete;
        }
        else if (chpid){
                if (msgsnd(qid, &parent_msg, sizeof parent_msg.msg, 0 ) == -1){
                        perror("Sending message");
                        goto delete;
                }
                wait(NULL); //wait for child to finish
                if (msgrcv(qid, &parent_msg, sizeof parent_msg.msg, MSGTYPE2, 0) == -1){
                        perror("Receiving message");
                        goto delete;
                }
                printf("Mensaje received in parent process! \n");
                printf("Id    : %d\n", parent_msg.msg.id);
                printf("Info  : %c\n", parent_msg.msg.info);
                /* Just so you can watch and explore the queue from ipcs command in your console :) */
                printf("Press enter to terminate process\n");
                getchar();
        }
        else {
                /* We already have the queue identifier, but if you fork + execv child process should 
                 * call ftok() + msgget() without IPC_CREAT flag to obtain the identifier
                 */
                struct msgbuf child_msg = {MSGTYPE1,{0}};
                if (msgrcv(qid, &child_msg, sizeof child_msg.msg, MSGTYPE1, 0) == -1){
                        perror("Receiving message");
                        exit(EXIT_FAILURE);
                }
                printf("Mensaje received in child process! \n");
                printf("Id    : %d\n", child_msg.msg.id);
                printf("Info  : %c\n", child_msg.msg.info);
                /* format child response */
                child_msg.mtype = MSGTYPE2;
                child_msg.msg.id = 33;
                child_msg.msg.info = 'C';
                if (msgsnd(qid, &child_msg, sizeof child_msg.msg, 0 ) == -1){
                        perror("Sending message from child");
                        exit(EXIT_FAILURE); 
                }
                exit(EXIT_SUCCESS);
        }

delete:
        if (msgctl(qid, IPC_RMID, NULL) == -1)
                perror("Eliminando cola : ");
        return 0;

}

