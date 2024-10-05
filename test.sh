#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <config_file>"
  exit 1
fi

config_file="$1"

# Extract the configuration name from the file path
config_name=$(basename "$config_file" .json)

make fclean &> /dev/null && make -j &> /dev/null

./webserv "$config_file" &> /dev/null &
webserv_pid=$!

./test/hurl --test --repeat 10 "./test/${config_name}.hurl"

kill "$webserv_pid"

echo "Tests completed."
