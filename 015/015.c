#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i*i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Thread function for printing even numbers
void* printEven(void* arg) {
    printf("Even numbers:\n");
    for (int i = 1; i <= 20; i++) {
        if (i % 2 == 0)
            printf("%d ", i);
    }
    printf("\n");
    pthread_exit(NULL);
}

// Thread function for printing odd numbers
void* printOdd(void* arg) {
    printf("Odd numbers:\n");
    for (int i = 1; i <= 20; i++) {
        if (i % 2 != 0)
            printf("%d ", i);
    }
    printf("\n");
    pthread_exit(NULL);
}

// Thread function for printing prime numbers
void* printPrime(void* arg) {
    printf("Prime numbers:\n");
    for (int i = 1; i <= 50; i++) {
        if (isPrime(i))
            printf("%d ", i);
    }
    printf("\n");
    pthread_exit(NULL);
}

// Main function
int main() {
    pthread_t thread1, thread2, thread3;

    // Create threads
    pthread_create(&thread1, NULL, printEven, NULL);
    pthread_create(&thread2, NULL, printOdd, NULL);
    pthread_create(&thread3, NULL, printPrime, NULL);

    // Wait for all threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("All threads have finished execution.\n");
    return 0;
}
