#1/bin/sh

echo "Running exit.sh"

(echo "echo \"exit\""
echo "exit") | ../bin/rshell
