#include <stdio.h>
#include <stdlib.h> 
#include <sys/msg.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "writer.h"
#include "reader.h"


char * categories[NUM_OF_CATEGORIES] = {"Student", "Azeri", "French"};
struct recipe recipes[NUM_OF_RECIPES*NUM_OF_CATEGORIES];


void write_recipe_to_queue(int msgQueueID, char * recipeContent);


// function to implement writing logic
void Cooking_recipe_writer() {
    for(int i=0;i<3;i++){
        int msgQueueID = get_message_queue_access(i+1);
        struct msqid_ds buf;
        // firstly, it checks whether each category (queue type) entails an entry
        if (msgctl(msgQueueID, IPC_STAT, &buf) == -1) {
            perror("Error getting queue info");
        }
        if(buf.msg_qnum == 0){
            // if it doesn't, writer writes 2 recipes to the queue
            write_recipe_to_queue(msgQueueID, recipes[i*2].recipeContent);
            write_recipe_to_queue(msgQueueID, recipes[i*2+1].recipeContent);
        }
    }
}
 

int create_message_queue(int queueType){
// function to create message queue of a given type (category)
    key_t k ;
    int id ;
    // choosing different keys for each category
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
    
    if (k == -1){ perror("Error in ftok");}
    // creating the message queue
    id = msgget (k, IPC_CREAT | 0666) ;
    if (id == -1){ perror("Error creating the queue");}
    return id ;
}



void write_recipe_to_queue(int msgQueueID, char *recipeContent){
// function to send the given recipe to the given message queue
    int r;
    struct message msg;
    size_t content_length = strlen(recipeContent);

    msg.messageType = 1;
    
    // as recipes are long and determining their size is difficult
    // it was decided to split messages into parts and send each part as a separate message
    for (size_t offset = 0; offset < content_length; offset += MAX_CHUNK_SIZE) {
        // size of chunk is either the fixed maximum
        // or the leftover length if the chunk is last
        msg.chunkSize = (content_length - offset > MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : content_length - offset;
        // copying chunkSize length of the recipe starting from the offset to the message
        memcpy(msg.chunk, recipeContent + offset, msg.chunkSize);
        // sending the message via message queue
        r = msgsnd(msgQueueID, &msg, sizeof(msg), 0);
        if (r == -1) {
            perror("Error sending the message");
            break;
        }
    }
}



void write_to_array(){
// function to read all recipes from files and store them in the array

    // counter of recipes array entries
    int insertionCounter = 0;

    // iterating through each directory (created for each category)
    for(int i=0; i<NUM_OF_CATEGORIES; i++){
        DIR *directory;
        struct dirent *dir_read;

        directory = opendir(categories[i]);
        if(directory==NULL){ perror("No such directory");}    

        // going through each recipe in the given directory
        while((dir_read=readdir(directory))!=NULL){

            if(dir_read->d_name[0] != '.'){

                char filename[300];
                // creating the correct filepath to be able to open it
                snprintf(filename, sizeof(filename), "%s/%s", categories[i], dir_read->d_name);
                int fd;

                // defining recipes properties
                recipes[insertionCounter].categoryIndex = i+1;
                recipes[insertionCounter].categoryName = (char *)malloc(sizeof(dir_read->d_name));

                // getting the name of the recipe 
                for(int k=0; dir_read->d_name[k]!='.';k++){
                    // reading until ".txt"
                    recipes[insertionCounter].categoryName[k] =  dir_read->d_name[k];
                }

                // opening the file for reading only
                fd = open(filename, O_RDONLY);
                if(fd == -1) perror("Error opening file");

                // determining the size of the file
                off_t fileSize = lseek(fd, 0, SEEK_END);

                // allocating a buffer to store the contents of the file
                recipes[insertionCounter].recipeContent = (char*)malloc(fileSize);

                // resetting the file pointer to the beginning of the file
                lseek(fd, 0, SEEK_SET);

                // reading the contents of the file into the buffer
                read(fd, recipes[insertionCounter].recipeContent, fileSize);

                insertionCounter++;
                close(fd);
            }
        }
        closedir(directory);
    }
}