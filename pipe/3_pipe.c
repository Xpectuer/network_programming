#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void) {
    int pipefds[2];
    char buffer[5];
    char *pin;

    if(pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);    
    }
        
    pid_t pid = fork();    
    if(pid == 0) { // in child process
        pin = "4821\0";
        close(pipefds[0]); // close read fd
        write(pipefds[1], pin, 5); // write PIN to pipe
        
        printf("Generating PIN in child and sending to parent...\n");
        sleep(2);
        exit(EXIT_SUCCESS);
    }
    if (pid > 0) { // in main process
        wait(NULL);
        close(pipefds[1]);
        read(pipefds[0], buffer, 5);
        close(pipefds[0]);
        
        printf("Parent received PIN '%s'\n", buffer);
    }
    return EXIT_SUCCESS;
}

