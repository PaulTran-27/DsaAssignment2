clear
g++ -g -o main main.cpp -I .
valgrind -s --leak-check=full --show-leak-kinds=all --error-limit=no --track-origins=yes ./main
