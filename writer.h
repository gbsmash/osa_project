#define NUM_OF_RECIPES 2
#define NUM_OF_CATEGORIES 3
#define MAX_CHUNK_SIZE 100
#define NUM_SIGNALS 3


struct recipe {
    int categoryIndex;
    char * categoryName;
    char * recipeContent;
};


extern char * categories[NUM_OF_CATEGORIES];
extern struct recipe recipes[NUM_OF_RECIPES*NUM_OF_CATEGORIES];


void write_to_array();
void Cooking_recipe_writer();
int create_message_queue(int queueType);