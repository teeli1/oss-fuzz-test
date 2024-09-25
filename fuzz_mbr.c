#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "work1.h"

// A simple function to process and fuzz MBR data from a buffer.
int FuzzProcessMBR(const uint8_t* data, size_t size) {
    if (size < sizeof(MBR_t)) {
        return 0;  // If the input size is too small, return early.
    }

    MBR_t mbr;
    // Copy the data into the MBR structure.
    memcpy(&mbr, data, sizeof(MBR_t));

    // Validate the MBR signature.
    if (mbr.signature == SIGNATURE) {
        // Iterate through the partitions and print their details.
        for (int i = 0; i < 4; i++) {
            pe_t partition = mbr.partitions[i];
            print_partition(&partition);
        }
    }

    return 0;  // Return 0 to indicate success.
}

// The entry point for libFuzzer.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Process the MBR data using the fuzzing data.
    FuzzProcessMBR(data, size);

    // If needed, you can add more specific fuzzing logic for EBR here.

    return 0;
}

