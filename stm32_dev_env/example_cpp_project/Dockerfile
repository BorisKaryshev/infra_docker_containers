FROM gcc:14.2.0 as build

RUN apt-get update -y && apt-get install --assume-yes libncurses-dev gcc-arm-none-eabi cmake

WORKDIR /root/files

RUN bash <<EOF
    set -euo pipefail
    gcc_arm=$(mktemp -d)
    cd $gcc_arm
    apt-get install --assume-yes wget bzip2
    wget https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
    bzip2 -d gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
    tar -xf gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar
    echo "\$PATH='$gcc_arm/gcc-arm-none-eabi-5_4-2016q3-20160926-linux/bin:\$PATH'" >> ~/.bashrc
EOF

RUN bash <<EOF
    apt-get --assume-yes install stlink-tools gcc-multilib g++-multilib
EOF

COPY CMakeLists.txt CMakeLists.txt
COPY configs configs
COPY include include
COPY src src
COPY libs libs
COPY linker/STM32F103X6_FLASH.ld LINKER_SCRIPT.ld

ENTRYPOINT cmake -S . -B build -DBUILD_MODE=DEBUG && cmake --build build -j 4 -t exe
