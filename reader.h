#define NUM_OF_RECIPES 2
#define NUM_OF_CATEGORIES 3
#define MAX_CHUNK_SIZE 100


struct message {
    long messageType;
    size_t chunkSize;
    char chunk[MAX_CHUNK_SIZE];
};


int get_message_queue_access(int queueType);
int read_cooking_recipe(int msgQueueID);
