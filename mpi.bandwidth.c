#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This program requires at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    // Number of message exchanges and message size in bytes (adjust as needed)
    int num_exchanges = 1e6;
    int message_size_bytes = 1e6;
    int message_size_bits = message_size_bytes * 8;
    char* message = (char*)malloc(message_size_bytes * sizeof(char));

    double start_time, end_time;
    double total_throughput = 0.0;

    for (int exchange = 0; exchange < num_exchanges; exchange++) {
        if (rank == 0) {
            // Process 0 sends a message to Process 1
            start_time = MPI_Wtime();
            MPI_Send(message, message_size_bytes, MPI_CHAR, 1, exchange, MPI_COMM_WORLD);
            end_time = MPI_Wtime();

            double throughput_gbps = (message_size_bits / 1e9) / (end_time - start_time);
            total_throughput += throughput_gbps;
        } else if (rank == 1) {
            // Process 1 receives the message from Process 0
            start_time = MPI_Wtime();
            MPI_Recv(message, message_size_bytes, MPI_CHAR, 0, exchange, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end_time = MPI_Wtime();

            double throughput_gbps = (message_size_bits / 1e9) / (end_time - start_time);
            total_throughput += throughput_gbps;
        }
    }

    // Calculate the mean throughput in Gbps
    double mean_throughput = total_throughput / num_exchanges;
    if (rank == 0) {
        printf("Mean throughput for %d exchanges: %f Gbps\n", num_exchanges, mean_throughput);
    }

    free(message);

    MPI_Finalize();
    return 0;
}
