#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_SIGNALS 3


void handleSignal(int signal);
void connectPipe(pid_t pid);


char * categories[NUM_SIGNALS] = {"Student", "Azeri", "French"};


int main() {

    pid_t pid = getpid();  // get the current process ID (to create connection with client)
    struct sigaction action;
    
    // write current process ID to the pipe
    connectPipe(pid);

    // initializes an empty signal set for sa_mask 
    // (set of signals to be blocked during the execution of the signal handler)
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handleSignal;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    // pausing after each signal received
    while (1) {
        sleep(1);
    }
}


void connectPipe(pid_t pid){
    
    int fd;

    // Create a named pipe
    mkfifo("server_pipe", 0666);
    // Open the pipe for writing
    fd = open("server_pipe", O_WRONLY);
    // Write the server process ID to the pipe (to be read by client)
    write(fd, &pid, sizeof(pid));
    // Close the pipe
    close(fd);
}


void handleSignal(int signal) {

    int categoryIndex;
    // receive signal and set the type of recipe according to the signal received
    switch(signal) {
        case SIGINT:
            categoryIndex = 0; // Student
            break;
        case SIGQUIT:
            categoryIndex = 1; // Azeri
            break;
        case SIGTERM:
            categoryIndex = 2; // French
            break;
        default:
            return;
    }
    printf("Recipe for category %s\n", categories[categoryIndex]);
}

