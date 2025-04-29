/* shared_memory_basics.c
 * This program demonstrates basic IPC using shared memory with two processes:
 * - A sender process that sends characters A-Z or numbers 1-100 to shared memory
 * - A receiver process that reads the data from shared memory
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
 
 #define SHM_SIZE 1024  // Size of shared memory segment in bytes
 #define SHM_KEY 1234   // Key for the shared memory segment
 
 // Structure to store our data in shared memory
 typedef struct {
     int data_ready;      // Flag to indicate if data is ready to be read
     int data_type;       // 1 for A-Z, 2 for 1-100
     int data_count;      // Number of elements in the data array
     char data[100];      // Array to store characters or numbers
 } SharedData;
 
 // Function prototypes
 void sender_process();
 void receiver_process();
 
 int main() {
     pid_t pid;
     
     // Create a child process
     pid = fork();
     
     if (pid < 0) {
         // Fork failed
         perror("Fork failed");
         exit(1);
     } else if (pid == 0) {
         // Child process - will be the receiver
         receiver_process();
     } else {
         // Parent process - will be the sender
         sender_process();
         
         // Wait for child to finish
         wait(NULL);
     }
     
     return 0;
 }
 
 // Sender process function
 void sender_process() {
     int shmid;
     SharedData *shared_data;
     int choice, i;
     
     // Create the shared memory segment
     shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
     if (shmid < 0) {
         perror("shmget failed in sender");
         exit(1);
     }
     
     // Attach the shared memory segment
     shared_data = (SharedData *)shmat(shmid, NULL, 0);
     if (shared_data == (SharedData *)-1) {
         perror("shmat failed in sender");
         exit(1);
     }
     
     // Initialize shared memory
     shared_data->data_ready = 0;
     
     // Ask the user what to send
     printf("What would you like to send?\n");
     printf("1. Alphabets (A-Z)\n");
     printf("2. Numbers (1-100)\n");
     printf("Enter your choice (1 or 2): ");
     scanf("%d", &choice);
     
     // Set the data type in shared memory
     shared_data->data_type = choice;
     
     if (choice == 1) {
         // Send A-Z
         printf("Sending alphabets A-Z to shared memory...\n");
         for (i = 0; i < 26; i++) {
             shared_data->data[i] = 'A' + i;
         }
         shared_data->data_count = 26;
     } else if (choice == 2) {
         // Send 1-100
         printf("Sending numbers 1-100 to shared memory...\n");
         // We can only send up to 100 values
         int count = 100;
         for (i = 0; i < count; i++) {
             // We store numbers as their ASCII values
             shared_data->data[i] = i + 1;
         }
         shared_data->data_count = count;
     } else {
         printf("Invalid choice. Exiting.\n");
         // Detach from shared memory
         if (shmdt(shared_data) == -1) {
             perror("shmdt failed in sender");
         }
         exit(1);
     }
     
     // Set the data_ready flag to signal receiver
     shared_data->data_ready = 1;
     
     // Wait for receiver to process (simple sleep for demonstration)
     printf("Sender: Waiting for receiver to process data...\n");
     sleep(2);
     
     // Detach from shared memory
     if (shmdt(shared_data) == -1) {
         perror("shmdt failed in sender");
         exit(1);
     }
     
     // Mark shared memory segment for deletion
     if (shmctl(shmid, IPC_RMID, NULL) == -1) {
         perror("shmctl failed in sender");
         exit(1);
     }
     
     printf("Sender: Shared memory detached and marked for deletion.\n");
 }
 
 // Receiver process function
 void receiver_process() {
     int shmid;
     SharedData *shared_data;
     int i;
     
     // Wait briefly to ensure sender has created the segment
     sleep(1);
     
     // Get the shared memory segment
     shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
     if (shmid < 0) {
         perror("shmget failed in receiver");
         exit(1);
     }
     
     // Attach the shared memory segment
     shared_data = (SharedData *)shmat(shmid, NULL, 0);
     if (shared_data == (SharedData *)-1) {
         perror("shmat failed in receiver");
         exit(1);
     }
     
     // Wait for data to be ready
     printf("Receiver: Waiting for data...\n");
     while (shared_data->data_ready == 0) {
         usleep(100000);  // Sleep for 100ms
     }
     
     // Read and display the data
     printf("Receiver: Data received from shared memory:\n");
     
     if (shared_data->data_type == 1) {
         // Received A-Z
         printf("Alphabets: ");
         for (i = 0; i < shared_data->data_count; i++) {
             printf("%c ", shared_data->data[i]);
         }
         printf("\n");
     } else if (shared_data->data_type == 2) {
         // Received 1-100
         printf("Numbers: ");
         for (i = 0; i < shared_data->data_count; i++) {
             printf("%d ", (unsigned char)shared_data->data[i]);
         }
         printf("\n");
     }
     
     // Detach from shared memory
     if (shmdt(shared_data) == -1) {
         perror("shmdt failed in receiver");
         exit(1);
     }
     
     printf("Receiver: Shared memory detached.\n");
 }