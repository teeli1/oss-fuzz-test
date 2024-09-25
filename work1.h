#ifndef WORK1_H
#define WORK1_H

#include <stdint.h>
#include <string.h>
#define SIGNATURE 0xAA55
#define SECTOR_SIZE 512

#pragma pack(1)

// MBR 파티션 엔트리 구조체
typedef struct {
    uint8_t status;
    uint8_t chs_first[3];
    uint8_t type;
    uint8_t chs_last[3];
    uint32_t lba_start;
    uint32_t size_sectors;
} pe_t;

// MBR 구조체
typedef struct {
    uint8_t bootloader[446];
    pe_t partitions[4];
    uint16_t signature; 
} MBR_t;

#pragma pack()


// 함수 선언
MBR_t ReadFile_MBR(char *filename);
MBR_t ReadFile_EBR(char *filename, uint32_t ebr_start_addr);
void read_ebr(char *filename, uint32_t ebr_start_addr, uint32_t total_lba_addr, uint32_t cur_lba_addr);
void print_partition(const pe_t* partition);
const char* print_fstype(uint8_t type);

#endif // WORK1_H
