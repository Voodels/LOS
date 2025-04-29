/* shared_memory_sorting.c
 * This program demonstrates IPC using shared memory with three processes:
 * - Process 1 (parent): Takes numbers as input from user
 * - Process 2 (first child): Sorts the numbers
 * - Process 3 (second child): Displays the sorted numbers
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
 #include <sys/wait.h>
 #include <signal.h>
 
 #define SHM_SIZE 4096   // Size of shared memory segment
 #define SHM_KEY 9876    // Key for shared memory segment
 #define MAX_NUMBERS 100 // Maximum number of integers to sort
 
 // Structure to store our data in shared memory
 typedef struct {
     int input_ready;    // Flag to indicate input is ready for sorting
     int sort_ready;     // Flag to indicate sorting is complete
     int count;          // Number of integers in the array
     int numbers[MAX_NUMBERS]; // Array of integers
 } SharedData;
 
 // Function prototypes
 void input_process(int shmid);
 void sort_process(int shmid);
 void display_process(int shmid);
 
 // Comparison function for qsort
 int compare_ints(const void *a, const void *b) {
     return (*(int *)a - *(int *)b);
 }
 
 int main() {
     int shmid;
     pid_t pid1, pid2;
     
     // Create the shared memory segment
     shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
     if (shmid < 0) {
         perror("shmget failed in main");
         exit(1);
     }
     
     // Initialize shared memory
     SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
     if (shared_data == (SharedData *)-1) {
         perror("shmat failed in main");
         exit(1);
     }
     
     // Initialize flags
     shared_data->input_ready = 0;
     shared_data->sort_ready = 0;
     
     // Detach from shared memory (children will reattach)
     if (shmdt(shared_data) == -1) {
         perror("shmdt failed in main");
         exit(1);
     }
     
     // Create first child process (sorter)
     pid1 = fork();
     
     if (pid1 < 0) {
         // Fork failed
         perror("First fork failed");
         exit(1);
     } else if (pid1 == 0) {
         // First child process - will be the sorter
         sort_process(shmid);
         exit(0);
     }
     
     // Create second child process (display)
     pid2 = fork();
     
     if (pid2 < 0) {
         // Fork failed
         perror("Second fork failed");
         kill(pid1, SIGTERM); // Kill first child
         exit(1);
     } else if (pid2 == 0) {
         // Second child process - will display results
         display_process(shmid);
         exit(0);
     }
     
     // Parent process - will take input
     input_process(shmid);
     
     // Wait for both children to finish
     waitpid(pid1, NULL, 0);
     waitpid(pid2, NULL, 0);
     
     // Delete the shared memory segment
     if (shmctl(shmid, IPC_RMID, NULL) == -1) {
         perror("shmctl failed in main");
         exit(1);
     }
     
     printf("Parent: Shared memory segment deleted.\n");
     return 0;
 }
 
 // Input process function
 void input_process(int shmid) {
     SharedData *shared_data;
     int num, i;
     
     // Attach the shared memory segment
     shared_data = (SharedData *)shmat(shmid, NULL, 0);
     if (shared_data == (SharedData *)-1) {
         perror("shmat failed in input process");
         exit(1);
     }
     
     // Get number count from user
     printf("How many numbers do you want to enter? (max %d): ", MAX_NUMBERS);
     scanf("%d", &num);
     
     if (num <= 0 || num > MAX_NUMBERS) {
         printf("Invalid number count. Must be between 1 and %d.\n", MAX_NUMBERS);
         shmdt(shared_data);
         exit(1);
     }
     
     shared_data->count = num;
     
     // Get numbers from user
     printf("Enter %d integers:\n", num);
     for (i = 0; i < num; i++) {
         printf("Number %d: ", i + 1);
         scanf("%d", &shared_data->numbers[i]);
     }
     
     printf("Input process: Numbers stored in shared memory.\n");
     
     // Set the input_ready flag to signal sorter
     shared_data->input_ready = 1;
     
     // Detach from shared memory
     if (shmdt(shared_data) == -1) {
         perror("shmdt failed in input process");
         exit(1);
     }
 }
 
 // Sort process function
 void sort_process(int shmid) {
     SharedData *shared_data;
     
     // Wait briefly to ensure parent has initialized the segment
     sleep(1);
     
     // Attach the shared memory segment
     shared_data = (SharedData *)shmat(shmid, NULL, 0);
     if (shared_data == (SharedData *)-1) {
         perror("shmat failed in sort process");
         exit(1);
     }
     
     // Wait for input to be ready
     printf("Sort process: Waiting for input...\n");
     while (shared_data->input_ready == 0) {
         usleep(100000);  // Sleep for 100ms
     }
     
     printf("Sort process: Sorting %d numbers...\n", shared_data->count);
     
     // Sort the numbers using qsort
     qsort(shared_data->numbers, shared_data->count, sizeof(int), compare_ints);
     
     printf("Sort process: Sorting complete.\n");
     
     // Set the sort_ready flag to signal display process
     shared_data->sort_ready = 1;
     
     // Detach from shared memory
     if (shmdt(shared_data) == -1) {
         perror("shmdt failed in sort process");
         exit(1);
     }
 }
 
 // Display process function
 void display_process(int shmid) {
     SharedData *shared_data;
     int i;
     
     // Wait a bit longer to ensure sorter has started
     sleep(2);
     
     // Attach the shared memory segment
     shared_data = (SharedData *)shmat(shmid, NULL, 0);
     if (shared_data == (SharedData *)-1) {
         perror("shmat failed in display process");
         exit(1);
     }
     
     // Wait for sorting to be complete
     printf("Display process: Waiting for sorting to complete...\n");
     while (shared_data->sort_ready == 0) {
         usleep(100000);  // Sleep for 100ms
     }
     
     // Display the sorted numbers
     printf("\nDisplay process: Sorted numbers from shared memory:\n");
     for (i = 0; i < shared_data->count; i++) {
         printf("%d ", shared_data->numbers[i]);
     }
     printf("\n");
     
     // Detach from shared memory
     if (shmdt(shared_data) == -1) {
         perror("shmdt failed in display process");
         exit(1);
     }
 }