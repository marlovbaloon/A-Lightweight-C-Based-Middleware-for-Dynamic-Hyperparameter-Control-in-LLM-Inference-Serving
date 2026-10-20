#!/bin/bash

set -e

echo "=== setup C LLM Middleware ==T"


sudo apt-get update
sudo apt-get install -y build-essential libcurl4-openssl-dev pkg-config

echo "=== install dependencies success ==="

make clean
make