FROM ubuntu:24.04

RUN bash <<EOF
  apt-get update -y
  apt-get --assume-yes install  vim                 \
                                git                 \
                                openssh-client      \
                                ripgrep wget        \
                                libncurses-dev      \
                                gcc-arm-none-eabi   \
                                gdb                 \
                                gdb-multiarch       \
                                cmake               \
                                gcc                 \
                                clangd              \
                                clang-format
EOF


COPY setup_nvim.sh /root
RUN /root/setup_nvim.sh

COPY ssh/config /root/.ssh/
RUN bash <<EOF
    set -eou pipefail
    git config --global user.name "$GITHUB_USERNAME"
    git config --global user.email "$GITHUB_EMAIL"
    git config --global --add safe.directory '*'
EOF

COPY bashrc /root/.bashrc

RUN bash <<EOF
    set -eou pipefail
    echo '#!/bin/bash' >> /root/.bash_logout
    echo 'find /files -user root | xargs -I{} chown \$REAL_USER:\$REAL_USER {}' >> /root/.bash_logout
    chmod u+x /root/.bash_logout
    echo 'trap /root/.bash_logout EXIT' >> /root/.bashrc
EOF

WORKDIR /files
