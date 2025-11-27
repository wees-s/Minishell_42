# minishell

A simple shell implementation inspired by bash, featuring command execution, pipes, redirections, and built-in commands as part of the 42 School curriculum.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Architecture](#architecture)
- [Implementation Details](#implementation-details)
- [Project Structure](#project-structure)
- [Compilation](#compilation)
- [Testing](#testing)
- [Key Concepts](#key-concepts)
- [Authors](#authors)

## Description

`minishell` is a minimalist shell that replicates core bash functionality. The project teaches fundamental Unix concepts including process management, file descriptors, pipes, signal handling, and command parsing. It provides an interactive command-line interface with support for command history, variable expansion, and proper signal handling.

## Features

- ✅ Interactive prompt with command history
- ✅ Command execution from PATH or absolute paths
- ✅ Pipes (`|`) for chaining commands
- ✅ Input/output redirections (`<`, `>`, `>>`)
- ✅ Here-document (`<<`) support
- ✅ Environment variable expansion (`$VAR`, `$?`)
- ✅ Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)
- ✅ Quote handling (single `'` and double `"`)
- ✅ Seven built-in commands
- ✅ Proper exit codes and error handling

## Installation

1. Clone the repository:
```bash
git clone https://github.com/be-dantas/42_Milestone3_MiniShell.git
cd 42_Milestone3_MiniShell
```

2. Install readline library (if needed):
```bash
# Ubuntu/Debian
sudo apt-get install libreadline-dev

# macOS
brew install readline
```

3. Compile the project:
```bash
make
```

This will create the `minishell` executable.

## Usage

### Starting the Shell

```bash
./minishell
```

You'll see the prompt:
```
minishell$
```

### Basic Commands

```bash
# Simple command execution
minishell$ ls -la
minishell$ echo "Hello, World!"
minishell$ pwd

# Command with pipes
minishell$ ls | grep minishell | wc -l

# Input/output redirection
minishell$ cat < input.txt
minishell$ echo "test" > output.txt
minishell$ echo "append" >> output.txt

# Here-document
minishell$ cat << EOF
> Hello
> World
> EOF

# Variable expansion
minishell$ echo $USER
minishell$ echo $?
minishell$ export MY_VAR="test"
minishell$ echo $MY_VAR
```

### Exit the Shell

```bash
minishell$ exit
```

Or press `Ctrl+D`.

## Built-in Commands

### `echo [-n] [args...]`
Prints arguments to standard output.

```bash
minishell$ echo Hello World
Hello World
minishell$ echo -n "No newline"
No newline$
```

### `cd [path]`
Changes the current directory.

```bash
minishell$ cd /tmp
minishell$ cd ..
minishell$ cd ~
minishell$ cd          # goes to HOME
```

### `pwd`
Prints the current working directory.

```bash
minishell$ pwd
/home/user/minishell
```

### `export [VAR=value]`
Sets or displays environment variables.

```bash
minishell$ export PATH="/usr/bin"
minishell$ export MY_VAR="hello"
minishell$ export           # displays all variables
```

### `unset [VAR]`
Removes environment variables.

```bash
minishell$ unset MY_VAR
minishell$ echo $MY_VAR
                        # empty
```

### `env`
Displays all environment variables.

```bash
minishell$ env
PATH=/usr/bin:/bin
USER=student
HOME=/home/student
...
```

### `exit [code]`
Exits the shell with optional exit code.

```bash
minishell$ exit
minishell$ exit 42
```

## Architecture

### System Flow Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                         ENTRADA                             │
│  • Herança envp                                             │
│  • Sinais (SIGINT, SIGQUIT)                                │
│  • Readline (histórico)                                     │
│  • Limpeza de saída (SIGINT, EXIT)                         │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│                          MAIN                               │
│  envp (herança) + signal (d, \, c) + histórico + readline  │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│                       COMMANDS                              │
│  • Lista, modifica, env (EXPORT, UNSET, EXPANDER)         │
│  • Determina execução de comandos                          │
│  • Detecta heredoc                                         │
│  • Validação de input, expansão simples, completa         │
│  • Tokenização, redirecionamento, identificação pipes      │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│                         PIPES                               │
│  • Redirecionamento de saída para o próximo                │
│  • Criação de processo filho (FORK)                        │
│  • Execução de comando (EXECVE)                            │
│  • Resultado no FD e descritor esperado (DUP2)            │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│                        RESULT                               │
└─────────────────────────────────────────────────────────────┘
```

### Core Components

```
┌──────────────────────────────────────────────────────────┐
│                        CORE                              │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │  Export  │  │   Echo   │  │ Expander │             │
│  └──────────┘  └──────────┘  └────┬─────┘             │
│  ┌──────────┐  ┌──────────┐       │                   │
│  │  Unset   │  │   Exit   │       │                   │
│  └──────────┘  └──────────┘       │                   │
│  ┌──────────┐  ┌──────────┐  ┌────▼─────┐            │
│  │   Env    │  │    Cd    │  │   Pwd    │            │
│  └──────────┘  └──────────┘  └──────────┘            │
│                                                        │
│  ┌──────────┐             ┌──────────────┐           │
│  │ Heredoc  │             │  BUILT-INs   │           │
│  └──────────┘             └──────────────┘           │
└──────────────────────────────────────────────────────────┘
```

## Implementation Details

### Main Loop

```c
int main(int argc, char **argv, char **envp)
{
    char *line;
    t_shell shell;
    
    // Initialize shell structure with environment
    init_shell(&shell, envp);
    setup_signals();
    
    while (1)
    {
        line = readline("minishell$ ");
        
        if (!line)  // Ctrl+D
            break;
            
        if (*line)
            add_history(line);
            
        // Process and execute command
        process_line(line, &shell);
        free(line);
    }
    
    cleanup_shell(&shell);
    return (shell.exit_code);
}
```

### Core Structures

```c
// Token types
typedef enum e_token_type
{
    T_WORD,           // Regular word
    T_PIPE,           // |
    T_REDIRECT_IN,    // <
    T_REDIRECT_OUT,   // >
    T_APPEND,         // >>
    T_HEREDOC,        // <<
} t_token_type;

// Token structure
typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

// Command structure
typedef struct s_cmd
{
    char    **args;         // Command and arguments
    char    *input_file;    // Input redirection file
    char    *output_file;   // Output redirection file
    int     append;         // Append mode flag
    int     heredoc;        // Heredoc flag
    char    *delimiter;     // Heredoc delimiter
    int     pipe_in;        // Has pipe input
    int     pipe_out;       // Has pipe output
} t_cmd;

// Shell state
typedef struct s_shell
{
    char    **envp;         // Environment variables
    int     exit_code;      // Last exit code ($?)
    t_token *tokens;        // Parsed tokens
    t_cmd   *commands;      // Parsed commands
} t_shell;
```

### Processing Pipeline

#### 1. Lexer (Tokenization)

Splits input into tokens respecting quotes:

```c
t_token *lexer(char *line)
{
    t_token *tokens;
    
    // Split by spaces, respecting quotes
    // Identify operators (|, <, >, >>, <<)
    // Store in linked list
    
    return (tokens);
}
```

**Example:**
```
Input:  echo "hello | world" | grep hello > out.txt
Tokens: [echo] ["hello | world"] [|] [grep] [hello] [>] [out.txt]
```

#### 2. Parser

Converts tokens into structured commands:

```c
t_cmd *parser(t_token *tokens)
{
    t_cmd *commands;
    
    // Group tokens into commands
    // Identify redirections
    // Set pipe flags
    
    return (commands);
}
```

**Example:**
```
cmd[0]:
  args: ["echo", "hello | world"]
  pipe_out: true
  
cmd[1]:
  args: ["grep", "hello"]
  output_file: "out.txt"
  pipe_in: true
```

#### 3. Expander

Replaces variables before execution:

```c
void expander(t_cmd *cmd, t_shell *shell)
{
    // Replace $VAR with getenv("VAR")
    // Replace $? with exit_code
    // Don't expand inside single quotes
    // Expand inside double quotes
}
```

**Example:**
```
Input:  echo $USER works in $PWD
After:  echo student works in /home/student/minishell
```

#### 4. Executor

Executes commands with proper redirections:

```c
void executor(t_cmd *commands, t_shell *shell)
{
    int pipe_fd[2];
    pid_t pid;
    
    // Create pipes if needed
    for each command:
        if (is_builtin(cmd))
            exec_builtin(cmd, shell);
        else
        {
            pid = fork();
            if (pid == 0)  // Child
            {
                setup_redirections(cmd, pipe_fd);
                execve(cmd->args[0], cmd->args, shell->envp);
            }
        }
    
    // Wait for all children
    waitpid(-1, &status, 0);
}
```

### Signal Handling

```c
volatile sig_atomic_t g_signal = 0;

void signal_handler(int signum)
{
    if (signum == SIGINT)  // Ctrl+C
    {
        g_signal = SIGINT;
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void setup_signals(void)
{
    struct sigaction sa;
    
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &sa, NULL);   // Ctrl+C
    sigaction(SIGQUIT, SIG_IGN, NULL);  // Ctrl+\ (ignore)
}
```

**Signal Behavior:**
- `Ctrl+C`: Interrupts current command, displays new prompt
- `Ctrl+D`: Exits shell (EOF)
- `Ctrl+\`: Ignored (no action)

### Pipe Implementation

```c
void execute_pipeline(t_cmd *commands, t_shell *shell)
{
    int pipe_fd[2];
    int prev_fd = STDIN_FILENO;
    pid_t pid;
    
    for (int i = 0; commands[i]; i++)
    {
        if (commands[i].pipe_out)
            pipe(pipe_fd);
            
        pid = fork();
        if (pid == 0)  // Child process
        {
            // Setup input from previous pipe
            if (prev_fd != STDIN_FILENO)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            
            // Setup output to next pipe
            if (commands[i].pipe_out)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            
            // Handle redirections
            setup_redirections(&commands[i]);
            
            // Execute
            execve(cmd_path, commands[i].args, shell->envp);
            exit(127);
        }
        
        // Parent closes and updates
        if (prev_fd != STDIN_FILENO)
            close(prev_fd);
            
        if (commands[i].pipe_out)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
    }
    
    // Wait for all children
    while (wait(&status) > 0)
        ;
}
```

### Heredoc Implementation

```c
void handle_heredoc(t_cmd *cmd)
{
    int pipe_fd[2];
    char *line;
    
    pipe(pipe_fd);
    
    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, cmd->delimiter) == 0)
        {
            free(line);
            break;
        }
        
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);
}
```

## Project Structure

```
minishell/
├── src/
│   ├── main.c                 # Main loop and initialization
│   ├── prompt/
│   │   ├── prompt.c          # Prompt display
│   │   └── signals.c         # Signal handling
│   ├── parser/
│   │   ├── lexer.c           # Tokenization
│   │   ├── parser.c          # Command structure building
│   │   ├── expander.c        # Variable expansion
│   │   └── quotes.c          # Quote handling
│   ├── executor/
│   │   ├── exec.c            # Main execution logic
│   │   ├── pipes.c           # Pipeline handling
│   │   ├── redirections.c    # File redirections
│   │   ├── builtins.c        # Built-in dispatcher
│   │   └── env.c             # Environment management
│   ├── builtins/
│   │   ├── echo.c            # Echo implementation
│   │   ├── cd.c              # Directory change
│   │   ├── pwd.c             # Print working directory
│   │   ├── export.c          # Export variables
│   │   ├── unset.c           # Unset variables
│   │   ├── env.c             # Display environment
│   │   └── exit.c            # Exit shell
│   └── utils/
│       ├── ft_split_mod.c    # Modified split for parsing
│       ├── free_utils.c      # Memory cleanup
│       ├── error.c           # Error handling
│       └── path.c            # PATH resolution
├── include/
│   └── minishell.h           # Header with all prototypes
├── libft/                    # Custom C library
├── Makefile                  # Compilation rules
└── README.md                # This file
```

## Compilation

The project uses a Makefile with the following targets:

- `make` or `make all`: Compiles the minishell program
- `make clean`: Removes object files
- `make fclean`: Removes object files and executable
- `make re`: Performs fclean followed by all

### Compilation Flags

```
-Wall -Wextra -Werror -lreadline
```

## Testing

### Basic Command Tests

```bash
# Simple execution
./minishell
minishell$ /bin/ls
minishell$ pwd
minishell$ echo hello

# Built-ins
minishell$ cd /tmp
minishell$ pwd
minishell$ export TEST=123
minishell$ echo $TEST
minishell$ env | grep TEST
minishell$ unset TEST
```

### Pipe Tests

```bash
minishell$ ls | grep mini
minishell$ cat file.txt | grep pattern | wc -l
minishell$ echo "test" | cat | cat | cat
```

### Redirection Tests

```bash
minishell$ echo "hello" > test.txt
minishell$ cat < test.txt
minishell$ echo "world" >> test.txt
minishell$ cat < test.txt > output.txt
```

### Heredoc Tests

```bash
minishell$ cat << EOF
> Line 1
> Line 2
> EOF

minishell$ grep hello << END
> hello world
> test hello
> END
```

### Variable Expansion Tests

```bash
minishell$ echo $USER
minishell$ echo $PWD
minishell$ echo $?
minishell$ export VAR="test"
minishell$ echo $VAR
minishell$ echo '$VAR'        # not expanded
minishell$ echo "$VAR"        # expanded
```

### Quote Tests

```bash
minishell$ echo 'hello | world'
minishell$ echo "hello | world"
minishell$ echo "$USER works here"
minishell$ echo '$USER works here'
```

### Signal Tests

```bash
# Test Ctrl+C during command
minishell$ sleep 100
^C
minishell$

# Test Ctrl+D
minishell$ 
^D
exit

# Test Ctrl+\ (should be ignored)
minishell$ sleep 100
^\minishell$
```

### Comparison Script

```bash
#!/bin/bash

test_cmd() {
    echo "Testing: $1"
    
    bash -c "$1" > bash_out.txt 2>&1
    bash_exit=$?
    
    echo "$1" | ./minishell > mini_out.txt 2>&1
    mini_exit=$?
    
    if diff bash_out.txt mini_out.txt && [ $bash_exit -eq $mini_exit ]; then
        echo "✓ PASS"
    else
        echo "✗ FAIL"
        diff bash_out.txt mini_out.txt
    fi
}

test_cmd "echo hello"
test_cmd "ls | wc -l"
test_cmd "echo \$USER"
test_cmd "cat < /etc/passwd | grep root"
```

## Key Concepts

### Process Management

**Fork**: Creates a child process
- Returns 0 in child
- Returns child PID in parent
- Returns -1 on error

**Execve**: Replaces current process with new program
- Never returns on success
- Returns -1 on error

**Wait/Waitpid**: Waits for child process termination

### File Descriptors

Standard file descriptors:
- **0 (STDIN)**: Standard input
- **1 (STDOUT)**: Standard output
- **2 (STDERR)**: Standard error

**dup2(oldfd, newfd)**: Duplicates file descriptor for redirection

### Pipes

A pipe connects the output of one command to the input of another:

```c
int pipe_fd[2];
pipe(pipe_fd);  // Create pipe
// pipe_fd[0] = read end
// pipe_fd[1] = write end
```

### Environment Variables

Managed through:
- `getenv(name)`: Get variable value
- Custom env array: For export/unset

### Readline Functions

- `readline(prompt)`: Read line with editing support
- `add_history(line)`: Add to history
- `rl_clear_history()`: Clear history
- `rl_on_new_line()`: Cursor to new line
- `rl_replace_line(text, clear)`: Replace current line
- `rl_redisplay()`: Redraw prompt

## Development Roadmap

### Phase 1: Foundation (Days 1-4)
- Set up project structure
- Implement main loop with readline
- Add signal handling (Ctrl+C, Ctrl+D)
- Test basic prompt functionality

### Phase 2: Parsing (Days 5-9)
- Implement lexer (tokenization)
- Create parser (command structures)
- Add quote handling
- Test with various inputs

### Phase 3: Execution (Days 10-14)
- Implement simple executor (no pipes/redirs)
- Add PATH resolution
- Test with basic commands

### Phase 4: Built-ins (Days 15-18)
- Implement all 7 built-ins
- Test each individually
- Test built-ins in pipelines

### Phase 5: Advanced Features (Days 19-23)
- Add pipe support
- Implement redirections (<, >, >>)
- Add heredoc support
- Test complex combinations

### Phase 6: Polish (Days 24-26)
- Variable expansion ($VAR, $?)
- Refine signal handling
- Memory leak checks
- Final testing and norminette

## Common Pitfalls

### ❌ Don't Mix Parsing and Execution
Keep them separate for cleaner code and easier debugging.

### ❌ Don't Forget to Close FDs
Every opened file descriptor must be closed to avoid leaks.

### ❌ Don't Fork for Built-ins Alone
Built-ins don't need fork when executed alone, only in pipelines.

### ❌ Don't Use Prohibited Functions
Never use `system()`, `popen()`, or other forbidden functions.

### ✅ Do Test Incrementally
Test each feature before moving to the next.

### ✅ Do Compare with Bash
Use `strace bash` to understand expected behavior.

### ✅ Do Check Memory Leaks
Run `valgrind` regularly during development.

## Authors

**wedos-sa && be-dantas** - Cadets at 42 School

---

*This project demonstrates fundamental Unix system programming concepts including process creation, inter-process communication, signal handling, and shell command interpretation.*