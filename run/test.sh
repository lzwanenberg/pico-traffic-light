param_count=$#
tests_exec="./build/tests"

if [ $param_count -eq 0 ]; then
  "$tests_exec"
else
  "$tests_exec" -r compact -s "*$1*" "${@:2}"
fi
