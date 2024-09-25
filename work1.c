#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SIGNATURE 0xAA55
#define SECTOR_SIZE 512

#pragma pack(1)

// MBR 파티션 엔트리
typedef struct {
    uint8_t status;
    uint8_t chs_first[3];
    uint8_t type;
    uint8_t chs_last[3];
    uint32_t lba_start;
    uint32_t size_sectors;
} pe_t;

// MBR 구조
typedef struct {
    uint8_t bootloader[446];
    pe_t partitions[4];
    uint16_t signature;
} MBR_t;

#pragma pack()

// Reads MBR from file and returns the MBR struct.
int ReadFile_MBR(const char *filename, MBR_t *mbr) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return -1;
    }

    size_t size = fread(mbr, 1, sizeof(MBR_t), file);
    if (size != sizeof(MBR_t)) {
        fprintf(stderr, "Error: reading file %s, incomplete MBR\n", filename);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

// Reads EBR from a given LBA (sector) address.
int ReadFile_EBR(const char *filename, uint32_t ebr_start_addr, MBR_t *ebr) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        return -1;
    }

    // Move the file pointer to the start of the EBR.
    if (fseek(file, ebr_start_addr * SECTOR_SIZE, SEEK_SET) != 0) {
        fprintf(stderr, "Error: unable to seek to LBA %u in file %s\n", ebr_start_addr, filename);
        fclose(file);
        return -1;
    }

    size_t size = fread(ebr, 1, sizeof(MBR_t), file);
    if (size != sizeof(MBR_t)) {
        fprintf(stderr, "Error: reading EBR at LBA %u in file %s\n", ebr_start_addr, filename);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

void print_partition(const pe_t* partition) {
    const char *type_str = NULL;
    switch (partition->type) {
        case 0x00: type_str = "No file system"; break;
        case 0x01: type_str = "FAT12"; break;
        case 0x04: type_str = "FAT16 (16-32MB)"; break;
        case 0x05: type_str = "EBR"; break;
        case 0x06: type_str = "FAT16 (32MB-2GB)"; break;
        case 0x07: type_str = "NTFS"; break;
        case 0x0B: type_str = "FAT32 (CHS)"; break;
        case 0x0C: type_str = "FAT32 (LBA)"; break;
        case 0x0E: type_str = "FAT16 (LBA)"; break;
        case 0x0F: type_str = "EBR"; break;
        default: type_str = "Unknown"; break;
    }
    printf("Partition type: %s, LBA Start: %u, Size (sectors): %u\n",
           type_str, partition->lba_start, partition->size_sectors);
}

void read_ebr(const char *filename, uint32_t ebr_start_addr, uint32_t total_lba_addr) {
    MBR_t ebr;
    if (ReadFile_EBR(filename, ebr_start_addr, &ebr) != 0) {
        fprintf(stderr, "Error reading EBR at LBA %u\n", ebr_start_addr);
        return;
    }

    if (ebr.signature != SIGNATURE) {
        fprintf(stderr, "EBR signature invalid at LBA %u\n", ebr_start_addr);
        return;
    }

    pe_t partition = ebr.partitions[0];
    if (partition.type == 0x05 || partition.type == 0x0F) {
        read_ebr(filename, partition.lba_start, total_lba_addr + partition.lba_start);
    } else {
        print_partition(&partition);
    }

    // Check if there's a chain of extended partitions (next EBR).
    partition = ebr.partitions[1];
    if (partition.type == 0x05 || partition.type == 0x0F) {
        read_ebr(filename, partition.lba_start, total_lba_addr + partition.lba_start);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    MBR_t mbr;
    if (ReadFile_MBR(argv[1], &mbr) != 0) {
        return EXIT_FAILURE;
    }

    if (mbr.signature != SIGNATURE) {
        fprintf(stderr, "Invalid MBR signature\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 4; i++) {
        pe_t partition = mbr.partitions[i];
        if (partition.type == 0x05 || partition.type == 0x0F) {
            read_ebr(argv[1], partition.lba_start, partition.lba_start);
        } else {
            print_partition(&partition);
        }
    }

    return EXIT_SUCCESS;
}

