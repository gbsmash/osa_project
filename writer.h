#define NUM_OF_RECIPES 2
#define NUM_OF_CATEGORIES 3
#define MAX_CHUNK_SIZE 100

struct recipe {
    int categoryIndex;
    char * categoryName;
    char * recipeContent;
};

extern char * categories[NUM_OF_CATEGORIES];
// array with all recipes of all categories
extern struct recipe recipes[NUM_OF_RECIPES*NUM_OF_CATEGORIES];



void write_to_array();
void Cooking_recipe_writer(int msgQueueID,char *recipeContent);
int create_message_queue(int queueType);
int check_message_queue();