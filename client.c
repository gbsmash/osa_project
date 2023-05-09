#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


pid_t getServerPID();
void sendSignal(int category, pid_t serverPID);


int main() {

    int category;
    pid_t serverPID = getServerPID();
    printf("Enter 1,2,3 to get recipes, or 0 to exit");

    while(category!=0){
        printf("\nEnter the category : ");
        scanf("%i",&category);
        sendSignal(category, serverPID);
    }

    return 0;
}


void sendSignal(int category, pid_t serverPID){
// sending signal of according type based on the category number entered by the user
    switch (category) {
        case 1:
            kill(serverPID, SIGINT);
            printf("\nSent SIGINT for recipe category 1 : Student\n");
            break;
        case 2:
            kill(serverPID, SIGQUIT);
            printf("\nSent SIGQUIT for recipe category 2 : Azeri\n");
            break;
        case 3:
            kill(serverPID, SIGTERM);
            printf("\nSent SIGTERM for recipe category 3 : French\n");
            break;
        default:
            printf("Unknown signal chosen\n");
    }
}


pid_t getServerPID(){
// function to connect to server via reading server PID from pipe
    int fd;
    pid_t server_pid;
    srand(time(NULL));

    // open the named pipe for reading the process id of server
    fd = open("server_pipe", O_RDONLY);
    // read the server process ID from the pipe
    read(fd, &server_pid, sizeof(server_pid));
    // Close the pipe
    close(fd);

    return server_pid;

}