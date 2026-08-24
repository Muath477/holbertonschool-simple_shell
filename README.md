# holbertonschool-simple_shell

This is our Simple Shell project for the Holberton low-level programming
track. We're building a basic UNIX command line interpreter in C, called
`hsh`, that works kind of like a stripped-down `/bin/sh`.

Team: Sarah Ali Alshahrani and Muath Hassan Almutairi.

## What it does

`hsh` reads a line, runs it as a command, and goes back to waiting for
the next one. It works in two ways:

- If you run it directly in a terminal, it prints a prompt (`($)`) and
  waits for you to type something.
- If input is piped in (like `echo "/bin/ls" | ./hsh`), it just reads
  line by line and runs each one, no prompt shown.

Each line gets split into words. The first word is the command, and
everything after it gets passed as arguments to that command. Right now
you have to give the full path to the program, like `/bin/ls`, since we
haven't implemented PATH searching yet, and there are no built-ins like
`cd` or `exit`.

## How to compile

```sh
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Examples

Interactive mode:

```sh
$ ./hsh
($) /bin/ls
hsh  main.c  shell.h
($) /bin/echo hello world
hello world
($)
$
```

(Ctrl+D on an empty line ends it.)

Piped mode:

```sh
$ echo "/bin/ls" | ./hsh
hsh  main.c  shell.h
```

## Files

- `README.md` - this file
- `man_1_simple_shell` - man page for the shell
- `AUTHORS` - list of people who contributed

## Authors

- Sarah Ali Alshahrani
- Muath Hassan Almutairi
