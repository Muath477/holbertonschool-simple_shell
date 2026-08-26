# Simple Shell - UNIX Command Line Interpreter

## Description
This is a custom, lightweight UNIX command-line interpreter written in C (C89 standard) as part of the Software Engineering curriculum at Holberton School.

The program mimics the core functionality of the standard /bin/sh shell, handling command execution, path resolution via the PATH environment variable, error messaging, dynamic line parsing, and built-in command handling.

## Features Implemented
- Interactive & Non-Interactive Mode: Works seamlessly with direct user input or piped commands.
- Command Resolution (PATH): Resolves relative and absolute executable paths dynamically.
- Built-in Commands:
  - exit: Terminate the shell with status propagation.
  - env: Print the current environment variables.
- Robust Memory Management: Freeing memory dynamically allocated by getline and string tokenization to prevent memory leaks.
- Error Handling: Standardized error reporting matching POSIX/sh formats (including command line numbering).
- Betty Compliance: Fully compliant with Holberton School's Betty coding and style guidelines.

## Allowed Functions & System Calls Used
read, write, malloc, free, exit, fork, execve, wait, strtok, getline, isatty, stat, access, perror

## Compilation & Usage

Compilation:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

Interactive Mode:
./hsh

Non-Interactive Mode:
echo "ls -l" | ./hsh

## File Structure
- main.h - Header file containing function prototypes, macros, and standard headers.
- main.c - Entry point, main interactive loop, and command parsing logic.
- builtins.c - Implementation of internal commands (exit, env) and _getenv.
- AUTHORS - Contributors list.
- man_1_simple_shell - Manual page for the shell.

## Authors
- Sara Alshahrani - sara12.shahrani@gmail.com
- Muath Almuatiri - moathalmuatiri@gmail.com
