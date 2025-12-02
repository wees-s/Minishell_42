# Milestone3 Minishell

A minimal shell implementation built as part of the 42 curriculum. This project reproduces essential behaviors of **bash**, excluding logical operators such as `||` and `&&`, focusing strictly on the mandatory requirements.

Authors: **Wesley Alexandre (wedos-sa)** and **Beatriz Dantas (bedantas)**

---

![alt text](https://i.imgur.com/7nyUwMp.png)

## Overview

This minishell provides a functional command interpreter capable of handling input reading, parsing, tokenization, environment variable expansion, redirections, pipelines, custom environment management, execution of built-in commands, and launching external programs via `execve`. Error handling follows the expected behavior of a real shell, including proper status codes retrievable through `$?`.

---

## Compilation and Execution

To build and run the shell:

```bash
make
./minishell
```

---

## Features

### Built-in Commands

The following built-ins are fully implemented:

* `echo` (supports `-n`)
* `pwd`
* `cd`
* `env`
* `export`
* `unset`
* `exit`
* heredoc support (`<<`)

Each built-in behaves correctly both inside and outside pipelines.

### Shell Capabilities

* Inherits and manages the system's `ENVP`
* Input handling through `readline`, including command history
* Proper handling of `SIGINT` and `SIGQUIT`, matching bash behavior in both interactive and execution contexts
* Full tokenization and parsing of commands, including quoted strings and operators
* Environment variable expansion (simple and full)
* Validation of malformed input (pipe errors, missing redirections, invalid tokens)
* Support for standard input/output redirections (`<`, `>`, `>>`, and `<<`) (`<`, `>`, `>>`) and heredoc (`<<`)
* Pipeline creation and execution
* Process creation with `fork` and execution with `execve`
* Correct use of file descriptors during redirections and pipes
* Error messages formatted in the same style expected from a POSIX shell

---

## Internal Architecture

The minishell is structured into clear stages and subsystems:

### Environment Handling

The inherited environment is stored in a modifiable structure that supports:

* `getenv`-like retrieval
* Insertions (`export`)
* Removals (`unset`)
* Use by the expansion subsystem

### Input and Readline

All user input is collected via `readline`, including history persistence across commands. Cleanup is performed properly when exiting through `exit` or via signals.

### Tokenizer

Splits the input into tokens while respecting:

* Quoted segments
* Operators
* Whitespace separation
* Edge cases identical to bash's behavior

### Parser

Constructs command structures containing:

* Command names
* Arguments
* Redirections
* Heredocs
* Pipeline chaining

### Expansion

Handles expansion of:

* `$VAR` using the internal environment list
* Quoted and unquoted contexts, accurately preserving literal content when required

### Redirection and Pipes

Implements all mandatory redirection operators and ensures correct file descriptor routing within a pipeline.

### Execution

* Applies redirections
* Executes built-ins directly
* Executes external commands using `execve`
* Manages children processes and exit statuses

---

## Example Usage

```
@minishell ~ echo "Hello World"
Hello World

@minishell ~ pwd
/home/user

@minishell ~ echo $USER
wedos-sa

@minishell ~ ls | wc -l
42

@minishell ~ cat << EOF
> line 1
> line 2
> EOF
line 1
line 2
```

---

## Project Structure

Adjust according to your repository layout:

* `src/` — core logic (parsing, execution, redirection, pipes)
* `utils/` — auxiliary functions
* `libft/` — 42's standard library
* `readline/` — integration with the readline library

---

## Exit and Cleanup

The shell performs proper cleanup on termination, including:

* Environment structures
* Readline state
* File descriptors

---

## License

Developed for academic purposes within the 42 curriculum.

---

## Authors

* **Wesley Alexandre** — [https://github.com/wees-s](https://github.com/wees-s)
* **Beatriz Dantas** — [https://github.com/be-dantas](https://github.com/be-dantas)
