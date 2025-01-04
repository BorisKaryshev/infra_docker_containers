# Repo with my dev env docker

Usage:
```
docker compose run --build -v <PATH_TO_WORKDIR>:/root/files dev_env bash
```

# Uploading code to MCU

Run stlink-gdb on host, connected with STLINK

To run (and install) stlink-gdb server:

### MacOS
```
brew install stlink
st-util
```

### Linux (Ubuntu)
```
sudo apt install --assume-yes stlink-tools
st-util
```

To build project:
```
cd stm32_example_project # or your folder
docker compose up --build
```

To upload code to MCU connect to gdb server via:
```
gdb-multiarch build/project_name.elf
(gdb) target extend host.g:4242
(gdb) load
(gdb) continue
```

# Has configured
- nvim
    - NVChad as base
    - clangd

- clang
- cmake
- clang-format

- ssh-client
- git (configured auth)
