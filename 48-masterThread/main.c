#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int threadID = omp_get_thread_num();
        int numThreads = omp_get_num_threads();

        if (threadID == 0) {
            // Master thread (thread 0) counts the total number of threads
            printf("Master thread: Total number of threads = %d\n", numThreads);
        } else {
            // Other threads print their thread numbers
            printf("Thread %d: Hello from worker thread\n", threadID);
        }
    }

    return 0;
}