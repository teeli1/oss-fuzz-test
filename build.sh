# 컴파일 및 링크 단계
$CC $CFLAGS -c work1.c -o work1.o
$CC $CFLAGS -c fuzz_pfstype.c -o fuzz_pfstype.o
$CC $CFLAGS -c fuzz_read_ebr.c -o fuzz_read_ebr.o
$CC $CFLAGS -c fuzz_read.c -o fuzz_read.o
$CC $CFLAGS -c fuzz_ppart.c -o fuzz_ppart.o
$CC $CFLAGS -c fuzz_read_lba.c -o fuzz_read_lba.o

# 컴파일된 객체 파일들을 링크하여 최종 fuzzer 바이너리 생성
$CC $CFLAGS $LIB_FUZZING_ENGINE \
    fuzz_work1.o \
    fuzz_pfstype.o \
    fuzz_read_ebr.o \
    fuzz_read.o \
    fuzz_ppart.o \
    fuzz_read_lba.o \
    work1.o \
    -o $OUT/simple-fuzzer
