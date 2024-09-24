#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define SIGNATURE 0xAA55

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

MBR_t ReadFile_MBR(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    MBR_t mbr;
    size_t size = fread(&mbr, 1, sizeof(MBR_t), file);

    if (size != sizeof(MBR_t)) {
        fprintf(stderr, "Error reading file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    fclose(file);
    return mbr;
}

MBR_t ReadFile_EBR(char *filename, uint32_t ebr_start_addr) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, ebr_start_addr * 512, SEEK_SET);
    MBR_t ebr;
    size_t size = fread(&ebr, 1, sizeof(MBR_t), file);
    if (size != sizeof(MBR_t)) {
        fprintf(stderr, "Error: reading file %s failed\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return ebr;
}

void print_partition(const pe_t* partition);

void read_ebr(char *filename, uint32_t ebr_start_addr, uint32_t total_lba_addr, uint32_t cur_lba_addr) {
    MBR_t ebr = ReadFile_EBR(filename, ebr_start_addr + cur_lba_addr);

    if (ebr.signature != SIGNATURE) {
        fprintf(stderr, "EBR not found!!\n");
        exit(EXIT_FAILURE);
    }

    pe_t partition = ebr.partitions[0];
    if (partition.type == 0x05 || partition.type == 0x0F) {
        read_ebr(filename, ebr_start_addr, total_lba_addr + partition.lba_start, partition.lba_start);
    } else {
        print_partition(&partition);
    }

    // 다음 EBR을 가리키는 파티션이 존재할 경우
    partition = ebr.partitions[1];
    if (partition.type == 0x05 || partition.type == 0x0F) {
        read_ebr(filename, ebr_start_addr, total_lba_addr + partition.lba_start, partition.lba_start);
    }
}

// Partition Type(0x1C2, 1Byte) -> 파일 시스템 종류
const char* print_fstype(uint8_t type) {
    switch (type) {
        case 0x00: return "No file system";
        case 0x01: return "FAT12";
        case 0x04: return "FAT16 (16-32MB)";
        case 0x05: return "EBR";
        case 0x06: return "FAT16 (32MB-2GB)";
        case 0x07: return "NTFS";
        case 0x0B: return "FAT32 (CHS)";
        case 0x0C: return "FAT32 (LBA)";
        case 0x0E: return "FAT16 (LBA)";
        case 0x0F: return "EBR";
        default: return "Unknown";
    }
}

void print_partition(const pe_t* partition) {
    printf("%s ", print_fstype(partition->type));
    printf("%u ", partition->lba_start);
    printf("%u ", partition->size_sectors);
    printf("\n");
}
   
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    MBR_t mbr = ReadFile_MBR(argv[1]);

    if (mbr.signature != SIGNATURE) {
        fprintf(stderr, "Invalid MBR signature\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 4; i++) {
        pe_t partition = mbr.partitions[i];
        if (partition.type == 0x05 || partition.type == 0x0F) {
            read_ebr(argv[1], partition.lba_start, partition.lba_start, 0);
        } else {
            print_partition(&partition);
        }
    }

    return EXIT_SUCCESS;
}
