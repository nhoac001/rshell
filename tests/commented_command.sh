#!/bin/sh

echo "Running commented_command.sh"

(echo "echo \"ls #-a\""
echo "ls #-a"
sleep 1
echo "echo \"echo \"hello \# world\"; #ls -a"
echo "echo \"hello \# world\"; #ls -a"
sleep 1
echo "echo \"exit\""
echo "exit") | ../bin/rshell
