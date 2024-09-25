FROM gcr.io/oss-fuzz-base/base-builder
RUN mkdir $SRC/oss-fuzz-test
COPY build.sh $SRC/
COPY *.c *.h $SRC/oss-fuzz-test
WORKDIR $SRC/oss-fuzz-test
