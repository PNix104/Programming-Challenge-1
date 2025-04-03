#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 3
#define NUM_THREADS 9  // One thread per matrix element

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

int calculationCount = 0;                  // Step 3: Global count
pthread_mutex_t count_mutex;              // Step 3: Mutex

typedef struct {
    int row;
    int col;
} thread_data_t;

void *multiply(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int sum = 0;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        sum += matrixA[data->row][i] * matrixB[i][data->col];

        // Step 3: Protect this block with mutex
        pthread_mutex_lock(&count_mutex);
        calculationCount++;
        pthread_mutex_unlock(&count_mutex);
    }

    resultMatrix[data->row][data->col] = sum;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MATRIX_SIZE * MATRIX_SIZE];
    thread_data_t thread_data[MATRIX_SIZE * MATRIX_SIZE];

    pthread_mutex_init(&count_mutex, NULL);  // Step 3: Init mutex

    // Initialize matrices A and B
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // Create one thread per matrix element
    int thread_index = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_index].row = i;
            thread_data[thread_index].col = j;
            pthread_create(&threads[thread_index], NULL, multiply, (void *)&thread_data[thread_index]);
            thread_index++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    // Step 4: Print total calculation count
    printf("Total number of calculations: %d\n", calculationCount);

    pthread_mutex_destroy(&count_mutex);  // Clean up
    return 0;
}

