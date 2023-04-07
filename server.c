#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define NUM_RECIPES 2
#define NUM_SIGNALS 3

char *recipes[NUM_SIGNALS][NUM_RECIPES] = {
    {"Sosiski", "Hechne, pul yoxdu"}, // Student
    {"Plov", "Dolma"},     // Azeri
    {"oui oui", "non non"} // French
};

void print_recipe(int category) {
    int recipe_idx = rand() % NUM_RECIPES;
    printf("Recipe for category %d: %s\n", category, recipes[category][recipe_idx]);
}

void handle_signal(int signal) {
    int category;
    switch(signal) {
        case SIGINT:
            category = 0; // Student
            break;
        case SIGQUIT:
            category = 1; // Azeri
            break;
        case SIGTERM:
            category = 2; // French
            break;
        default:
            return;
    }
    print_recipe(category);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error SIGINT ");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("Error SIGQUIT");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Error SIGTERM");
        exit(EXIT_FAILURE);
    }

    printf("Server running..\n");
    while(1) {}
    return 0;
}
