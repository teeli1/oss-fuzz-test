$CC $CFLAGS work1.c -c -o work1.o
$CC $CFLAGS fuzz_work1.c -c -o fuzz_work1.o
$CC $CFLAGS $LIB_FUZZING_ENGINE fuzz_work1.o work1.o -o $OUT/simple-fuzzer
