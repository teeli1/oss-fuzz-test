#include <stdint.h>
#include <stddef.h>
#include "work1.h"  // Include the header for your functions

// Fuzzing entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Create an MBR structure to hold the parsed data
    MBR_t mbr;
    
    // If the data is too small to be an MBR, skip processing
    if (size < sizeof(MBR_t)) {
        return 0;
    }

    // Process the MBR data from the buffer
    memcpy(&mbr, data, sizeof(MBR_t));

    // Check the signature and process partitions if valid
    if (mbr.signature == SIGNATURE) {
        for (int i = 0; i < 4; i++) {
            print_partition(&mbr.partitions[i]);
        }
    }

    return 0;  // Return 0 on success
}

