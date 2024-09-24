#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "work1.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // fuzz 데이터를 임시 파일로 저장
    char filename[] = "/tmp/fuzz_ebr_file";
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return 0;
    }
    fwrite(data, 1, size, file);
    fclose(file);

    // 임의의 LBA 주소를 사용해 EBR을 읽는 함수 호출
    uint32_t ebr_start_addr = 0;
    uint32_t total_lba_addr = 0;
    uint32_t cur_lba_addr = 0;

    read_ebr(filename, ebr_start_addr, total_lba_addr, cur_lba_addr);

    // 임시 파일 삭제
    remove(filename);

    return 0;
}

