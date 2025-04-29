#include <stdio.h>
#include <omp.h>
#include <math.h>

// Function to check if a number is prime
int isPrime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int main() {
    int start = 1, end = 100; // Range of numbers to check for primes
    int totalPrimes = 0;

    #pragma omp parallel for reduction(+:totalPrimes)
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            totalPrimes++;
            printf("Thread %d found prime: %d\n", omp_get_thread_num(), i);
        }
    }

    printf("Total prime numbers in range [%d, %d]: %d\n", start, end, totalPrimes);

    return 0;
}