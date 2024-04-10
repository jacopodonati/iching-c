#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <termios.h>
#include <fcntl.h>
#include "iching.h"

#define NUM_THREADS 3
#define ITERATIONS 6

typedef struct {
    int no_wait;
    int verbose;
} Options;

typedef struct {
    int thread_id;
    int result;
    time_t timestamp;
} ThreadData;

typedef struct {
    ThreadData thread_data;
    Options options;
} ThreadParams;

int kbhit() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void *thread_function(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    ThreadData *data = &(params->thread_data);
    Options *options = &(params->options);
    
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    data->timestamp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    int random_number = rand() % 2 + 2;

    if (options->verbose) {
        printf("%lld: %d\n", (long long)data->timestamp, random_number);
    }

    data->result = random_number;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    Options options = {0, 0};
    int option;

    static struct option long_options[] = {
        {"no-wait", no_argument, 0, 'w'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    while ((option = getopt_long(argc, argv, "wv", long_options, NULL)) != -1) {
        switch (option) {
            case 'w':
                options.no_wait = 1;
                break;
            case 'v':
                options.verbose = 1;
                break;
            case '?':
                fprintf(stderr, "Opzione non riconosciuta\n");
                exit(EXIT_FAILURE);
            default:
                abort();
        }
    }

    pthread_t threads[NUM_THREADS];
    ThreadParams thread_params[NUM_THREADS];

    srand(time(NULL));
    int result[ITERATIONS];

    for (int i = 0; i < ITERATIONS; i++) {
        if (!options.no_wait)
        {
            while (!kbhit()) {}
            getchar();
        }
        
        int sum = 0;

        for (int j = 0; j < NUM_THREADS; j++) {
            thread_params[j].thread_data.thread_id = j;
            thread_params[j].options = options;
            pthread_create(&threads[j], NULL, thread_function, (void *)&thread_params[j]);
        }

        for (int j = 0; j < NUM_THREADS; j++) {
            pthread_join(threads[j], NULL);
            sum += thread_params[j].thread_data.result;
        }

        result[i] = sum;

        if (options.verbose)
        {
            printf("La somma è: %d\n\n", sum);
        }
        else
        {
            printf("%d", sum);
        }
    }

    printf("\n");
    return 0;
}
