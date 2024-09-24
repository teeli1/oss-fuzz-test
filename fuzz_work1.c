#include <stdint.h>
#include <stddef.h>
#include <string.h>  // memcpy를 사용하기 위해 추가
#include "work1.h"   // 필요한 구조체 및 함수 선언 포함

#define SIGNATURE 0xAA55  // MBR 시그니처 정의

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 입력 데이터가 MBR 구조체 크기보다 작은 경우 유효하지 않음
    if (size < sizeof(MBR_t)) {
        return 0;
    }

    // 입력 데이터를 메모리에 복사하여 MBR 구조체로 변환
    MBR_t mbr;
    memcpy(&mbr, data, sizeof(MBR_t));

    // MBR 서명 확인
    if (mbr.signature == SIGNATURE) {
        for (int i = 0; i < 4; i++) {
            pe_t partition = mbr.partitions[i];
            if (partition.type == 0x05 || partition.type == 0x0F) {
                // 가상 EBR 읽기 (EBR 처리는 생략할 수 있음)
                // read_ebr 함수는 실제 파일을 필요로 하므로 생략됨
            } else {
                print_partition(&partition);
            }
        }
    }

    return 0;  // 정상 종료
}

