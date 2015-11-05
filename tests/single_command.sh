#!/bin/sh

echo "Running single_command.sh"


(echo "echo ls"
echo "ls"
sleep 1
echo "echo \"ls -a\""
echo "ls -a"
sleep 1
echo "echo \"echo hello\""
echo "echo hello"
sleep 1
echo "echo \"mkdir -p test\""
echo "mkdir -p test"
sleep 1
echo "exit") | ../bin/rshell 
