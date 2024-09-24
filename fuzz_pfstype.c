#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "work1.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    // 첫 번째 바이트를 파일 시스템 타입으로 사용
    uint8_t type = data[0];
    print_fstype(type);

    return 0;
}

