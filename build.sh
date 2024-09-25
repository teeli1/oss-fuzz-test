# 컴파일 및 링크 단계
$CC $CFLAGS -c work1.c -o work1.o
$CC $CFLAGS -c fuzz_work.c -o fuzz_work.o

# 컴파일된 객체 파일들을 링크하여 최종 fuzzer 바이너리 생성
$CC $CFLAGS $LIB_FUZZING_ENGINE \
    fuzz_work.o \
    work1.o \
    -o $OUT/simple-fuzzer
