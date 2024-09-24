#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "work1.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < sizeof(pe_t)) return 0;

    // fuzz 데이터를 파티션 구조체로 해석
    pe_t partition;
    memcpy(&partition, data, sizeof(pe_t));

    // 파티션을 출력하는 함수 호출
    print_partition(&partition);

    return 0;
}

