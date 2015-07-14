make && valgrind --leak-check=full --show-reachable=yes --error-limit=no --suppressions=valgrind_ignore.supp ./bin/main
