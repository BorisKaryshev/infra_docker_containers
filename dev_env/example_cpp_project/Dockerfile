FROM gcc:14.2.0 as build

RUN apt-get update -y && apt-get install --assume-yes clang-format cmake wget

WORKDIR /files
RUN bash <<EOF
    mkdir tmp
    cd tmp
    wget https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
    tar -xzf boost_1_87_0.tar.gz && cd boost_1_87_0
    ./bootstrap.sh --prefix=/usr/lib/boost
    ./b2 install -j 12
EOF

COPY CMakeLists.txt CMakeLists.txt
COPY configs configs
COPY include include
COPY src src

RUN cmake -S . -B build && cmake --build build -j 4 -t corutine_test

FROM ubuntu:24.04

COPY --from=build /files/build/corutine_test .
ENTRYPOINT ./corutine_test
