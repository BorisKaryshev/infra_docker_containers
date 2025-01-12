FROM gcc:14.2.0 as build

RUN apt-get update -y && apt-get install --assume-yes libncurses-dev gcc-arm-none-eabi cmake

WORKDIR /root/files

COPY CMakeLists.txt CMakeLists.txt
COPY configs configs
COPY include include
COPY src src
COPY libs libs
COPY linker/STM32F103X6_FLASH.ld LINKER_SCRIPT.ld

ENTRYPOINT cmake -S . -B build -DBUILD_MODE=DEBUG && cmake --build build -j 4 -t exe
