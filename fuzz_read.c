#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "work1.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // fuzz 데이터를 임시 파일로 저장
    char filename[] = "/tmp/fuzz_mbr_file";
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return 0;
    }
    fwrite(data, 1, size, file);
    fclose(file);

    // MBR 파일을 읽어들이는 함수 호출
    ReadFile_MBR(filename);

    // 임시 파일 삭제
    remove(filename);

    return 0;
}

