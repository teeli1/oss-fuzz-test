$CC $CFLAGS -o work1.o work1.c
$CC $CFLAGS -o fuzz_work1.o fuzz_work1.c
$CC $CFLAGS $LIB_FUZZING_ENGINE fuzz_work1.o work1.o -o $OUT/oss-fuzz-test

