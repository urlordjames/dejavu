FROM alpine as builder

RUN mkdir -p /compile/build
WORKDIR /compile

RUN apk add --no-cache gcc g++ cmake ninja jsoncpp-dev zlib-dev git util-linux-dev openssl-dev

COPY CMakeLists.txt .
COPY src src

WORKDIR /compile/build

RUN cmake .. -DSTATIC_ROOT=/dejavu/static -GNinja && cmake --build . -j $(nproc)

FROM alpine

RUN apk add --no-cache jsoncpp util-linux
RUN adduser dejavu -H -D && mkdir /dejavu && chown dejavu:dejavu /dejavu

USER dejavu
WORKDIR /dejavu

COPY --chown=dejavu:dejavu --from=builder /compile/build/dejavu_server /dejavu/dejavu_server
COPY --chown=dejavu:dejavu static /dejavu/static

EXPOSE 8080

CMD ["/dejavu/dejavu_server"]
