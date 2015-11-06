#1/bin/sh

echo "Running exit.sh"

(echo "echo \"exit\""
echo "exit") | ./bin/rshell

echo "exit after ;"

(echo "echo \"ls; exit\""
echo "ls; exit") | ./bin/rshell

echo "exit after connectors"

(echo "echo \"ls && exit\""
echo "ls && exit") | ./bin/rshell

echo "exited"

(echo "echo \"ls || exit; ls; exit\""
echo "ls || exit; ls; exit") | ./bin/rshell

echo "exit before anything"

(echo "echo \"exit; ls\""
echo "exit; ls") | ./bin/rshell
