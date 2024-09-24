#include <stdint.h>
#include <stddef.h>
#include <string.h>  // memcpy 사용을 위해 필요
#include "work1.h"   // 필요한 구조체 및 함수 선언 포함

#define SIGNATURE 0xAA55

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 입력 데이터가 MBR 구조체 크기보다 작은 경우 처리하지 않음
    if (size < sizeof(MBR_t)) {
        return 0;
    }

    // 메모리에 데이터를 로드하여 처리
    MBR_t mbr;
    memcpy(&mbr, data, sizeof(MBR_t));

    // MBR 서명을 확인
    if (mbr.signature == SIGNATURE) {
        for (int i = 0; i < 4; i++) {
            pe_t partition = mbr.partitions[i];
            if (partition.type == 0x05 || partition.type == 0x0F) {
                read_ebr("dummy_file", partition.lba_start, partition.lba_start, 0);
            } else {
                print_partition(&partition);
            }
        }
    }

    return 0;
}

