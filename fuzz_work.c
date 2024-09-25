#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "work1.h"

#define SIGNATURE 0xAA55
// Simple fuzz target
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < sizeof(MBR_t)) {
        return 0;
    }

    // Write the input to a temporary file to simulate reading from a disk image
    char filename[] = "/tmp/fuzz_mbrXXXXXX";
    int fd = mkstemp(filename);
    if (fd == -1) {
        return 0;
    }

    // Write data to the temporary file
    if (write(fd, data, size) != size) {
        close(fd);
        return 0;
    }
    close(fd);

    // Process the file as an MBR
    MBR_t mbr = ReadFile_MBR(filename);

    // Check MBR signature
    if (mbr.signature == SIGNATURE) {
        for (int i = 0; i < 4; i++) {
            pe_t partition = mbr.partitions[i];
            if (partition.type == 0x05 || partition.type == 0x0F) {
                // If it's an extended partition, process the EBR
                read_ebr(filename, partition.lba_start, partition.lba_start, 0);
            } else {
                // Otherwise, print the partition
                print_partition(&partition);
            }
        }
    }

    // Remove the temporary file
    unlink(filename);

    return 0;
}

