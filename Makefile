CC = mpicc
CFLAGS = -Wall
INSTALL_DIR = /usr/local/bin

all: mpi.bandwidth mpi.latency

mpi.bandwidth: mpi.bandwidth.c
	$(CC) $(CFLAGS) mpi.bandwidth.c -o mpi.bandwidth

mpi.latency: mpi.latency.c
	$(CC) $(CFLAGS) mpi.latency.c -o mpi.latency

install:
	mkdir -p $(INSTALL_DIR)
	cp mpi.bandwidth $(INSTALL_DIR)/mpi.bandwidth
	cp mpi.latency $(INSTALL_DIR)/mpi.latency

clean:
	rm -f mpi.bandwidth mpi.latency

.PHONY: all install clean
