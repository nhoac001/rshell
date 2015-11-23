#!/bin/sh

echo "Running test_command.sh"

(echo "echo \"test src && echo exists\""
echo "test src && echo exists"
sleep 1
echo "echo \"test /src && echo exists\""
echo "test /src && echo exists"
sleep 1
echo "echo \"test -f src || echo unexistant\""
echo "test -f src || echo unexistant"
sleep 1
echo "echo \"test -d src && echo exists|"
echo "test -d src && echo exists"
sleep 1
echo "echo \"[ src ] && echo exists\""
echo "[ src ] && echo exists"
sleep 1
echo "echo \"[ -f src ] || echo unexistant\""
echo "[ -f src ] || echo unexistant"
sleep 1
echo "echo \"[ -d src ] && echo exists\""
echo "[ -d src ] && echo exists"
echo "exit") | ./bin/rshell
