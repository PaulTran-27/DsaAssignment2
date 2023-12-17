clear
g++ -g -o main main.cpp -I .
valgrind -s --log-file=log.txt --leak-check=full --show-leak-kinds=all --error-limit=no --track-origins=yes ./main
./main >> output.txt