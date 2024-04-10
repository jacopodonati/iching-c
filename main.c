#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include "iching.h"

#define NUM_THREADS 3

typedef struct {
    int thread_id;
    int result;
} ThreadData;

void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int random_number = rand() % 2 + 2;
    data->result = random_number;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int no_wait = 0;
    int verbose = 0;
    int option;

    static struct option long_options[] = {
        {"no-wait", no_argument, 0, 'w'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    while ((option = getopt_long(argc, argv, "wv", long_options, NULL)) != -1) {
        switch (option) {
            case 'w':
                no_wait = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case '?':
                fprintf(stderr, "Opzione non riconosciuta\n");
                exit(EXIT_FAILURE);
            default:
                abort();
        }
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    srand(time(NULL));

    for (int i = 0; i < 6; i++) {
        if (!no_wait)
        {
            getchar();
        }
        
        int sum = 0;

        for (int j = 0; j < NUM_THREADS; j++) {
            thread_data[j].thread_id = j;
            pthread_create(&threads[j], NULL, thread_function, (void *)&thread_data[j]);
        }

        for (int j = 0; j < NUM_THREADS; j++) {
            pthread_join(threads[j], NULL);
            sum += thread_data[j].result;
        }
        printf("%d ", sum);
    }

    return 0;
}

