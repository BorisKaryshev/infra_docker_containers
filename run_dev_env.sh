#!/usr/bin/bash
set -euo pipefail

env_name=$1
volume_path=$2

docker compose -f $env_name/docker-compose.yaml run --build -v "$volume_path:/root/files" $env_name bash
