#!/bin/sh

echo "Running multi_command.sh"

(echo "echo \"echo hello; echo world\""
echo "echo hello; echo world"
sleep 1
echo "echo \"echo hello || echo world\""
echo "echo hello || echo world"
sleep 1
echo "echo \"echo hello && echo world\""
echo "echo hello && echo world"
sleep 1
echo "echo \"echo hello && || echo world\""
echo "echo hello && || echo world\""
sleep 1
echo "echo \"echo hello || && echo world\""
echo "echo hello || && echo world"
sleep 1
echo "echo \"echo hello; cd || echo world\""
echo "echo hello; cd || echo world"
sleep 1
echo "echo \"echo hello; echo world && echo neat\""
echo "echo hello; echo world && echo neat\""
sleep 1
echo "echo \"exit\""
echo "exit") | ../bin/rshell
