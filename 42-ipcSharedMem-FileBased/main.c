/* shared_memory_files.c
 * This program demonstrates IPC using shared memory with file I/O:
 * - A sender process reads data from an input file and sends it via shared memory
 * - A receiver process reads from shared memory and writes to an output file
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
 #include <sys/wait.h>
 #include <fcntl.h>
 
 #define SHM_SIZE 4096  // Increased size for larger file contents
 #define SHM_KEY 5678   // Different key from previous example
 #define MAX_FILENAME 256
 
 // Structure to store our data in shared memory
 typedef struct {
     int data_ready;        // Flag to indicate if data is ready to be read
     int data_size;         // Size of data in the buffer
     char filename[MAX_FILENAME]; // Original filename for reference
     char buffer[SHM_SIZE - 512]; // Buffer for file data (leaving space for metadata)
 } SharedData;
 
 // Function prototypes
 void sender_process(const char *input_file);
 void receiver_process(const char *output_file);
 
 int main() {
     pid_t pid;
     char input_file[MAX_FILENAME];
     char output_file[MAX_FILENAME];
     
     // Get input and output filenames
     printf("Enter input filename: ");
     scanf("%255s", input_file);
     
     printf("Enter output filename (must be different from input): ");
     scanf("%255s", output_file);
     
     // Check that the filenames are different
     if (strcmp(input_file, output_file) == 0) {
         printf("Error: Input and output filenames must be different.\n");
         exit(1);
     }
     
     // Create a child process
     pid = fork();
     
     if (pid < 0) {
         // Fork failed
         perror("Fork failed");
         exit(1);
     } else if (pid == 0) {
         // Child process - will be the receiver
         receiver_process(output_file);
     } else {
         // Parent process - will be the sender
         sender_process(input_file);
         
         // Wait for child to finish
         wait(NULL);
     }
     
     return 0;
 }
 
 // Sender process function
 void sender_process(const char *input_file) {
     int shmid;
     SharedData *shared_data;
     FILE *fp;
     size_t bytes_read;
     
     // Open input file
     fp = fopen(input_file, "r");
     if (fp == NULL) {
         perror("Failed to open input file");
         exit(1);
     }
     
     // Create the shared memory segment
     shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
     if (shmid < 0) {
         perror("shmget failed in sender");
         fclose(fp);
         exit(1);
     }
     
     // Attach the shared memory segment
     shared_data = (SharedData *)shmat(shmid, NULL, 0);
     if (shared_data == (SharedData *)-1) {
         perror("shmat failed in sender");
         fclose(fp);
         exit(1);
     }
     
     // Initialize shared memory
     shared_data->data_ready = 0;
     strcpy(shared_data->filename, input_file);
     
     // Read from file into shared memory
     printf("Sending file contents to shared memory...\n");
     bytes_read = fread(shared_data->buffer, 1, sizeof(shared_data->buffer) - 1, fp);
     
     if (ferror(fp)) {
         perror("Error reading from file");
         fclose(fp);
         shmdt(shared_data);
         exit(1);
     }
     
     // Null-terminate the buffer (for text files)
     shared_data->buffer[bytes_read] = '\0';
     shared_data->data_size = bytes_read;
     
     // Close the input file
     fclose(fp);
     
     printf("Sender: Read %zu bytes from '%s'\n", bytes_read, input_file);
     
     // Set the data_ready flag to signal receiver
     shared_data->data_ready = 1;
     
     // Wait for receiver to process
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
 void receiver_process(const char *output_file) {
     int shmid;
     SharedData *shared_data;
     FILE *fp;
     
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
     
     // Open output file
     fp = fopen(output_file, "w");
     if (fp == NULL) {
         perror("Failed to open output file");
         shmdt(shared_data);
         exit(1);
     }
     
     // Write shared memory contents to output file
     size_t bytes_written = fwrite(shared_data->buffer, 1, shared_data->data_size, fp);
     
     if (ferror(fp)) {
         perror("Error writing to file");
         fclose(fp);
         shmdt(shared_data);
         exit(1);
     }
     
     // Close the output file
     fclose(fp);
     
     printf("Receiver: Wrote %zu bytes to '%s'\n", bytes_written, output_file);
     printf("Receiver: Original file was '%s'\n", shared_data->filename);
     
     // Detach from shared memory
     if (shmdt(shared_data) == -1) {
         perror("shmdt failed in receiver");
         exit(1);
     }
     
     printf("Receiver: Shared memory detached.\n");
 }