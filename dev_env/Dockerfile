FROM ubuntu:24.04

RUN bash <<EOF
    apt-get update -y
    apt-get --assume-yes install vim git openssh-client ripgrep wget neovim
EOF

ARG ENABLE_CPP
ARG ENABLE_PYTHON
ARG ENABLE_BASH

RUN bash <<EOF
    set -euo pipefail
    [ "${ENABLE_CPP:-true}" == "true" ] || exit 0
    apt-get update -y
    apt-get --assume-yes install cmake clang clangd clang-format
EOF

RUN bash <<EOF
    set -euo pipefail
    [ "${ENABLE_PYTHON:-true}" == "true" ] || exit 0

    apt-get update -y
    apt-get --assume-yes install python3.12 python3-pip
    pip install --break-system-packages pyright
EOF

COPY setup_nvim.sh /root
RUN /root/setup_nvim.sh

ARG GITHUB_USERNAME
ARG GITHUB_EMAIL

COPY ssh/config /root/.ssh/
RUN bash <<EOF
    set -eou pipefail
    git config --global user.name "$GITHUB_USERNAME"
    git config --global user.email "$GITHUB_EMAIL"
    git config --global --add safe.directory '*'

    echo "$GITHUB_USERNAME" >> /root/gitconfig
    echo "$GITHUB_EMAIL" >> /root/gitconfig
EOF

COPY bashrc /root/.bashrc
RUN bash <<EOF
    set -euo pipefail

    echo '#!/bin/bash' >> /root/.bash_logout
    echo 'find /files -user root | xargs -I{} chown \$REAL_USER:\$REAL_USER {}' >> /root/.bash_logout
    chmod u+x /root/.bash_logout
    echo 'trap /root/.bash_logout EXIT' >> /root/.bashrc
EOF

COPY requirements.txt .
RUN bash <<EOF
    set -euo pipefail
    [ "${ENABLE_PYTHON:-true}" == "true" ] || exit 0

    pip install --break-system-packages -r requirements.txt
EOF

WORKDIR /files
