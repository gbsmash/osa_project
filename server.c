#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "reader.h"
#include "writer.h"


void handleSignal(int signal);
void connectPipe(pid_t pid);


int main() {

// *********************** CONNECTION BETWEEN SERVER AND CLIENT ********************
    pid_t pid = getpid();  // get the current process ID (to create connection with client)
    struct sigaction action;
    // write current process ID to the pipe
    connectPipe(pid);

// ******* PREPARING SERVER, READING RECIPES FROM FILES AND CREATING QUEUES ***********
    // reading all recipes from files and storing them in the array
    write_to_array();

    // creating a message queue for each category
    for(int i=0;i<3;i++){
        create_message_queue(i+1);
    }
    
// ***************************** SIGNAL HANDLING *******************************
    // initializes an empty signal set for sa_mask 
    // (set of signals to be blocked during the execution of the signal handler)
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handleSignal;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    while (1) {
        sleep(1);
    }
}


void connectPipe(pid_t pid){
// function to establish connection with clients via writing server PID to pipe
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
// signal handler function
    int msg_id;
    int categoryIndex;
    // receive signal and set the type of recipe according to the signal received
    switch(signal) {
        case SIGINT:
            categoryIndex = 1; // Student
            break;
        case SIGQUIT:
            categoryIndex = 2; // Azeri
            break;
        case SIGTERM:
            categoryIndex = 3; // French
            break;
        default:
            return;
    }
    
    // launch writer
    Cooking_recipe_writer();

    // launch reader
    msg_id = get_message_queue_access(categoryIndex);
    // reading 2 recipes from the message queue
    read_cooking_recipe(msg_id);
}

