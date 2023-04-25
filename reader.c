#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>
#include<string.h>
#include <unistd.h>


#define NUM_OF_RECIPES 2
#define NUM_OF_CATEGORIES 3
#define MAX_CHUNK_SIZE 100


// struct for messages in message queue
struct message {
    long messageType;
    size_t chunkSize;
    char chunk[MAX_CHUNK_SIZE];
};


int get_message_queue_access(int queueType);
int read_cooking_recipe(int msgQueueID);


int main() {
    // getting the input from the user
    int category;
    printf("\nEnter the category : ");
    scanf("%i",&category);
    printf("\n\nHere are the recipes of category %i!\n\n",category);
    // accessing the message queue of the given type (category)
    int msg_id = get_message_queue_access(category);
    // reading 2 recipes from the message queue
    read_cooking_recipe(msg_id);
    return 0;
}

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
        // writing to the terminal
        fwrite(msg.chunk, 1, msg.chunkSize, stdout);
        // if it is the last chunk
        if (msg.chunkSize < MAX_CHUNK_SIZE) {
            printf("\n---\n");
            // it means 1 recipe has been read
            recipe_count++;
            if(recipe_count==2) exit(0);
        }
    }
}
