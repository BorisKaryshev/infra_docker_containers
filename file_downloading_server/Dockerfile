FROM ubuntu:24.10

RUN bash <<EOF
    apt-get update
    apt-get install --assume-yes python3 python3-pip php
EOF

COPY requirements.txt .
RUN python3.12 -m pip install --break-system-packages -r requirements.txt

WORKDIR /root
COPY file_server /root/file_server

ENTRYPOINT python3.12 -m file_server --directory /root/files --port 5000
