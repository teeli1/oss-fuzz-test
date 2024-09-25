#ifndef WORK1_H
#define WORK1_H

#include <stdint.h>

#define SIGNATURE 0xAA55
#define SECTOR_SIZE 512

#pragma pack(1)

// MBR 파티션 엔트리 (Partition Entry)
typedef struct {
    uint8_t status;
    uint8_t chs_first[3];
    uint8_t type;
    uint8_t chs_last[3];
    uint32_t lba_start;
    uint32_t size_sectors;
} pe_t;

// MBR 구조 (MBR Structure)
typedef struct {
    uint8_t bootloader[446];
    pe_t partitions[4];
    uint16_t signature;
} MBR_t;

#pragma pack()

// Function Prototypes

// Reads MBR from a file and returns the MBR struct.
// Returns 0 on success and -1 on failure.
int ReadFile_MBR(const char *filename, MBR_t *mbr);

// Reads EBR from a given LBA (sector) address.
// Returns 0 on success and -1 on failure.
int ReadFile_EBR(const char *filename, uint32_t ebr_start_addr, MBR_t *ebr);

// Prints partition information to the console.
void print_partition(const pe_t* partition);

// Recursively reads and processes extended partitions.
void read_ebr(const char *filename, uint32_t ebr_start_addr, uint32_t total_lba_addr);

#endif // WORK1_H

