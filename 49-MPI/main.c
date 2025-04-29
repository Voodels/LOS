#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank (ID) and size (total processes)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Print a message from each process
    printf("Hello from process %d out of %d processes\n", rank, size);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}


// mpicc hello_mpi.c -o hello_mpi
// mpirun -np 4 ./hello_mpi