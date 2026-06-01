#!/bin/bash

#Test script for 'hello_world_extended' kernel module
# Arg 1 - String to be written into the module

PARAM_FOLDER="/sys/module/hello_world_extended/parameters"
if [ ! -d "$PARAM_FOLDER" ]; then
    echo "Error: module not loaded or sysfs entry missing."
    exit 1
fi

if [ $# -lt 1 ]; then
    TARGET="Hello, World!"
    else
    TARGET="$1"
fi

LEN=${#TARGET}

for (( i=0; i<LEN; i++ )); do
    char="${TARGET:$i:1}"
    echo "$i" > "$PARAM_FOLDER/idx"
    echo "$char" > "$PARAM_FOLDER/ch_val"
done

RESULT=$(cat "$PARAM_FOLDER/my_str")
echo "Result: $RESULT"
if [ "$RESULT" = "$TARGET" ]; then
    echo "SUCCESS: string matches '$TARGET'"
    exit 0
else
    echo "FAIL: expected '$TARGET', got '$RESULT'"
    exit 1
fi