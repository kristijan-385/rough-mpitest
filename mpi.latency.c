#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MESSAGE_SIZE 1 // Set message size to 1 for latency testing

int main(int argc, char *argv[]) {
    int rank, size;
    double start_time, end_time;
    double total_time = 0.0;
    int num_messages = 1e3; // Adjust the number of messages as needed

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "This program requires at least two MPI processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    char *message = (char *)malloc(MESSAGE_SIZE * sizeof(char));

    if (rank == 0) {
        // Warm-up
        MPI_Send(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        start_time = MPI_Wtime();

        for (int i = 0; i < num_messages; ++i) {
            MPI_Send(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        end_time = MPI_Wtime();
        total_time = end_time - start_time;
    } else if (rank == 1) {
        // Warm-up
        MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message, MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

        for (int i = 0; i < num_messages; ++i) {
            MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        double mean_latency = (total_time / (num_messages * 2)) * 1e6; // Convert to microseconds
        printf("Mean Latency: %.2f microseconds\n", mean_latency);
    }

    free(message);

    MPI_Finalize();
    return 0;
}
