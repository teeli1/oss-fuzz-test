# 컴파일 및 링크 단계
$CC $CFLAGS -c work1.c -o work1.o
$CC $CFLAGS -c fuzz_work1.c -o fuzz_work1.o
$CC $CFLAGS $LIB_FUZZING_ENGINE fuzz_work1.o work1.o -o $OUT/simple-fuzzer

