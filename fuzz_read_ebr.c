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

    // 임의의 EBR 시작 주소 (퍼저가 제공한 데이터를 사용하지 않으므로 임의의 값 사용)
    uint32_t ebr_start_addr = 0;

    // EBR 파일을 읽어들이는 함수 호출
    ReadFile_EBR(filename, ebr_start_addr);

    // 임시 파일 삭제
    remove(filename);

    return 0;
}

