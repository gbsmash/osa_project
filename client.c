#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define NUM_SIGNALS 3

void send_random_signal() {
    int signal_idx = rand() % NUM_SIGNALS;
    switch(signal_idx) {
        case 0:
            printf("SIGINT\n");
            kill(getpid(), SIGINT);
            break;
        case 1:
            printf("SIGQUIT\n");
            kill(getpid(), SIGQUIT);
            break;
        case 2:
            printf("SIGTERM\n");
            kill(getpid(), SIGTERM);
            break;
        default:
            break;
    }
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        send_random_signal();
        sleep(1);
    }

    return 0;
}
