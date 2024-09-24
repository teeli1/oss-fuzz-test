#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "work1.h"  // MBR 및 EBR 파서 코드가 정의된 헤더 파일

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 입력 데이터가 MBR 구조체 크기보다 작은 경우 유효하지 않음
    if (size < sizeof(MBR_t)) {
        return 0;
    }

    // 입력 데이터를 파일이 아닌 메모리에서 처리하도록 가정
    MBR_t mbr;
    memcpy(&mbr, data, sizeof(MBR_t));

    // MBR 서명 확인
    if (mbr.signature == SIGNATURE) {
        for (int i = 0; i < 4; i++) {
            pe_t partition = mbr.partitions[i];
            if (partition.type == 0x05 || partition.type == 0x0F) {
                // 가상 EBR 읽기 (EBR 처리 생략)
                // read_ebr 함수는 실제 파일을 필요로 하므로 간단한 테스트에서는 생략
            } else {
                print_partition(&partition);
            }
        }
    }

    return 0; // 항상 0을 반환하여 에러가 없음을 알림
}

