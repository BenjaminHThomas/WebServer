#!/bin/bash

# set -e
set -e

exit_code=0

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

run_test() {
    local test_file="$1"
    local repeat_count="${2:-1}" # Default to 1 if not provided
    local config_name=$(basename "$test_file" .hurl)
    local config_file="./config/${config_name}.json"

    if [ ! -f "$config_file" ]; then
        echo -e "${RED}❌ Error: Config file not found: $config_file${NC}"
        return 1
    fi

    echo -e "${BLUE}🚀 Starting webserv for $config_name...${NC}"
    ./webserv "$config_file" &> /dev/null &
    local webserv_pid=$!

    echo -e "${CYAN}🧪 Running test for $config_name ($repeat_count times)...${NC}"
    if ./test/hurl --connect-timeout 15s --test --repeat "$repeat_count" "$test_file"; then
        echo -e "${GREEN}✅ Test passed for $config_name${NC}"
    else
        echo -e "${RED}❌ Test failed for $config_name${NC}"
        exit_code=1
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
    for test_file in ./test/*.hurl; do
        if [ -f "$test_file" ]; then
            if [ -n "$2" ]; then
                run_test "$test_file" "$2"
            else
                run_test "$test_file"
            fi
            sleep 1
        else
            echo -e "${YELLOW}⚠️  Warning: No test files found in ./test/${NC}"
            break
        fi
    done
elif [ -z "$1" ]; then
    echo -e "${YELLOW}ℹ️  Usage: $0 <test_file> [repeat_count] or $0 all [repeat_count]${NC}"
    exit 1
else
    if [ ! -f "$1" ]; then
        echo -e "${RED}❌ Error: Test file not found: $1${NC}"
        exit 1
    fi
    run_make
    if [ -n "$2" ]; then
        run_test "$1" "$2"
    else
        run_test "$1"
    fi
fi

echo -e "${GREEN}🎉 All tests completed.${NC}"
exit $exit_code