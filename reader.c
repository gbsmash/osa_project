#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>
#include<string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>

#include "reader.h"


int get_message_queue_access(int queueType) {
// function to get access to the message queue of the given type
    key_t k;
    int id;
    // choosing key according to the type of queue
    switch (queueType){
    case 1:
        k = ftok("file1", 'A');
        break;
    case 2:
        k = ftok("file2", 'B');
        break;
    case 3:
        k = ftok("file3", 'C');
        break;
    }

    if (k == -1) { perror("Error in ftok"); }
    // accessing the queue
    id = msgget(k, 0);
    if (id == -1) { perror("Error getting the message"); }
    return id;
}


int read_cooking_recipe(int msgQueueID) {
// function to read data from the given message queue
    int r;
    struct message msg;
    int recipe_count = 0;

    // until all 2 recipes are read
    while (recipe_count < 2) {
        // getting chunks of recipes
        r = msgrcv(msgQueueID, &msg, sizeof(msg), 1, 0);
        if (r == -1) {
            perror("Error receiving the message");
            break;
        }
        // Add a null terminator to the end of the received chunk before printing
        char temp_chunk[MAX_CHUNK_SIZE + 1];
        memcpy(temp_chunk, msg.chunk, msg.chunkSize);
        temp_chunk[msg.chunkSize] = '\0';
        printf("%s", temp_chunk);

        // if it is the last chunk
        if (msg.chunkSize < MAX_CHUNK_SIZE) {
            // it means 1 recipe has been read
            printf("\n---\n");
            recipe_count++;
        }
    }
    return 0;
}
