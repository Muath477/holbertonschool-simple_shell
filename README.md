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

## How we built it

`main.c` has the main loop. It reads a line with `getline`, sends it to
`split_line` to break it into words, and if there's actually something
there (not just an empty line), it calls `execute`.

`execute` does the classic fork/exec/wait pattern: `fork()` a child
process, and in the child replace it with the requested program using
`execve()`. The parent just calls `waitpid()` and waits for the child to
finish before going back to read the next line.

One thing we had to fix along the way: when a command doesn't exist,
`sh` doesn't just print the normal "No such file or directory" message,
it prints something like `sh: 1: qwerty: not found`, with a line
number. So we made `hsh` do the same thing, just with our own program
name instead of `sh`. We check the file with a small helper function in
`execute.c` and format the error to match. `hsh` also exits with the
same status code as the last command it ran when it hits end-of-file,
same as `sh` does.

## Files

- `shell.h` - header file, has the includes and prototypes, include
  guarded
- `main.c` - the main loop: read, split, execute, handle EOF
- `split_line.c` - splits a line into a NULL-terminated array of words
- `execute.c` - forks, execs, waits, and handles the "not found" error
- `man_1_simple_shell` - man page for the shell
- `AUTHORS` - list of people who contributed

## Testing

We tested this against `/bin/sh` directly, comparing output line by
line for stuff like: a single command, a command with arguments, a
command that doesn't exist, blank lines, and Ctrl+D / piped EOF. We also
ran it through `valgrind --leak-check=full` and got no leaks, and it
passes `betty-style.pl` / `betty-doc.pl` with nothing flagged.

## Authors

- Sarah Ali Alshahrani
- Muath Hassan Almutairi
