alias b := build
alias r := run


# Build and run
build:
    gcc main.c -o main -Wall -Wextra -pedantic -std=c99

run: build
    ./main
