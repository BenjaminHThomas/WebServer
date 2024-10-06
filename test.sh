#!/bin/bash

set -e

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

run_test() {
    local config_file="$1"
    local config_name=$(basename "$config_file" .json)
    local test_file="./test/${config_name}.hurl"

    if [ ! -f "$test_file" ]; then
        echo -e "${RED}❌ Error: Test file not found: $test_file${NC}"
        return 1
    fi

    echo -e "${BLUE}🚀 Starting webserv for $config_name...${NC}"
    ./webserv "$config_file" &> /dev/null &
    local webserv_pid=$!

    echo -e "${CYAN}🧪 Running test for $config_name...${NC}"
    if ./test/hurl --test --repeat 1 "$test_file"; then
      echo -e "${GREEN}✅ Test passed for $config_name${NC}"
    else
      echo -e "${RED}❌ Test failed for $config_name${NC}"
    fi

    echo -e "${YELLOW}🛑 Stopping webserv for $config_name...${NC}"

    kill $webserv_pid 2>/dev/null || true

    echo -e "${MAGENTA}🏁 Tests completed for $config_name.${NC}"
}

run_make() {
    echo -e "${CYAN}🛠️  Running make...${NC}"
    if make fclean &> /dev/null && make -j &> /dev/null; then
        echo -e "${GREEN}✅ Make completed successfully.${NC}"
    else
        echo -e "${RED}❌ Error: Make failed.${NC}"
        exit 1
    fi
}

if [ "$1" = "all" ]; then
    run_make
    for config_file in ./config/*.json; do
        if [ -f "$config_file" ]; then
            run_test "$config_file"
            sleep 1
        else
            echo -e "${YELLOW}⚠️  Warning: No config files found in ./config/${NC}"
            break
        fi
    done
elif [ -z "$1" ]; then
    echo -e "${YELLOW}ℹ️  Usage: $0 <config_file> or $0 all${NC}"
    exit 1
else
    if [ ! -f "$1" ]; then
        echo -e "${RED}❌ Error: Config file not found: $1${NC}"
        exit 1
    fi
    run_make
    run_test "$1"
fi

echo -e "${GREEN}🎉 All tests completed.${NC}"