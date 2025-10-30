DICIONÁRIO DE FUNÇÕES PERMITIDAS:

**readline** – lê uma linha digitada no terminal com suporte a edição e histórico; retorna uma string alocada dinamicamente.
**rl_clear_history** – limpa toda a memória usada pelo histórico do `readline`.
**rl_on_new_line** – avisa ao `readline` que o cursor deve ir para uma nova linha (geralmente após sinal).
**rl_replace_line** – substitui o conteúdo atual da linha sendo editada por outro texto.
**rl_redisplay** – redesenha o prompt e o texto atual da linha no terminal.
**add_history** – adiciona uma linha ao histórico de comandos do `readline`.

**printf** – imprime texto formatado na saída padrão.
**malloc** – aloca dinamicamente uma área de memória e retorna seu ponteiro.
**free** – libera a memória previamente alocada com `malloc`.
**write** – escreve dados em um descritor de arquivo (como stdout).
**access** – verifica permissões de acesso a um arquivo (leitura, escrita, execução).
**open** – abre ou cria um arquivo e retorna seu descritor.
**read** – lê bytes de um descritor de arquivo.
**close** – fecha um descritor de arquivo.

**fork** – cria um novo processo, duplicando o processo atual.
**wait** – aguarda a finalização de um processo filho.
**waitpid** – aguarda um processo filho específico terminar.
**wait3 / wait4** – variantes de `wait` que também retornam informações de uso de recursos.

**signal** – define uma função que será executada quando um sinal específico for recebido.
**sigaction** – versão mais robusta e configurável de `signal`.
**sigemptyset** – inicializa um conjunto de sinais vazio.
**sigaddset** – adiciona um sinal a um conjunto de sinais.
**kill** – envia um sinal a um processo (ex: terminar, parar, continuar).
**exit** – encerra o processo atual liberando recursos e retornando um código de status.

**getcwd** – obtém o diretório de trabalho atual.
**chdir** – muda o diretório de trabalho atual.
**stat / lstat / fstat** – obtém informações sobre um arquivo (tamanho, permissões, tipo).
**unlink** – remove (deleta) um arquivo do sistema.
**execve** – substitui o processo atual por outro programa executável.

**dup** – duplica um descritor de arquivo.
**dup2** – duplica um descritor de arquivo para um número específico (ex: redirecionamento).
**pipe** – cria um canal de comunicação entre processos (leitura/escrita).

**opendir** – abre um diretório para leitura.
**readdir** – lê a próxima entrada de um diretório aberto.
**closedir** – fecha um diretório aberto.

**strerror** – retorna uma string descritiva de um código de erro (`errno`).
**perror** – imprime automaticamente a mensagem de erro associada ao último erro.

**isatty** – verifica se um descritor de arquivo é um terminal interativo.
**ttyname** – retorna o nome do terminal associado a um descritor.
**ttyslot** – retorna o número de slot do terminal atual.
**ioctl** – realiza operações de controle direto sobre dispositivos (como terminais).

**getenv** – obtém o valor de uma variável de ambiente.
**tcsetattr / tcgetattr** – configuram ou obtêm atributos do terminal (modo canônico, eco, etc.).

**tgetent** – inicializa o acesso à base de dados termcap (capacidade do terminal).
**tgetflag** – lê uma flag booleana da base termcap.
**tgetnum** – lê um valor numérico da base termcap.
**tgetstr** – lê uma string de controle da base termcap.
**tgoto** – gera uma string de controle para posicionar o cursor em uma coordenada.
**tputs** – envia uma string de controle ao terminal (normalmente usada com `tgetstr`/`tgoto`).



# 42_Milestone3_MiniShell
# Estrutura

# 🧱 1. Estrutura de Pastas

Organize desde o começo — isso te poupa dor de cabeça no fim:

```bash
minishell/
│
├── src/
│   ├── main.c
│   ├── prompt/
│   │   ├── prompt.c
│   │   └── signals.c
│   ├── parser/
│   │   ├── lexer.c
│   │   ├── parser.c
│   │   ├── expander.c
│   │   └── quotes.c
│   ├── executor/
│   │   ├── exec.c
│   │   ├── pipes.c
│   │   ├── redirections.c
│   │   ├── builtins.c
│   │   └── env.c
│   └── utils/
│       ├── ft_split_mod.c
│       ├── free_utils.c
│       └── error.c
│
├── include/
│   └── minishell.h
│
├── libft/
│   └── ...
│
├── Makefile
└── README.md
```

Essa estrutura já separa responsabilidades:

- **prompt/** → interação com o usuário, histórico e sinais
- **parser/** → desmonta e interpreta a linha de comando
- **executor/** → cria processos, redireciona, executa
- **utils/** → utilitários, memória, erros, helpers

# 🧩 2. Etapas de Desenvolvimento (roadmap lógico)

## Fase 1 — Setup e prompt

- Crie o `main()` com loop de leitura usando `readline()`.
- Mostre o prompt (`minishell$` ) e armazene no histórico (`add_history()`).
- Se a linha for vazia, pule.
- Se for `Ctrl+D`, saia.
- Faça o shell sair limpo (`exit code` correto).

✅ **Dica:** já lide com `signal(SIGINT, handler)` pra não matar o shell com `Ctrl+C`.

## Fase 2 — Lexer (tokenização)

Transforma a linha em tokens (exemplo: `echo "oi | amigo"` → [echo] ["oi | amigo"]).

**Etapas:**

1. Separar palavras respeitando aspas `'` e `"`.
2. Identificar operadores (`|`, `&lt;`, `&gt;`, `&gt;&gt;`, `&lt;&lt;`).
3. Armazenar tokens em uma **lista ligada** ou **array de structs**.

**Estrutura base:**

```c
typedef enum e_token_type {
	T_WORD,
	T_PIPE,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_HEREDOC
}	t_token_type;

typedef struct s_token {
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;
```

## Fase 3 — Parser

Com os tokens prontos, monta comandos estruturados (com args, redirecionamentos, etc).

**Exemplo:**

```bash
echo hello | grep h > out.txt
```

vira algo como:

```
cmd[0]: "echo"
args: ["hello"]
pipe_out: yes

cmd[1]: "grep"
args: ["h"]
redir_out: "out.txt"
```

**Estrutura sugerida:**

```c
typedef struct s_cmd {
	char	**args;
	char	*input_file;
	char	*output_file;
	int		append;
	int		pipe_in;
	int		pipe_out;
}	t_cmd;
```

## Fase 4 — Expansão ($ e variáveis)

Antes de executar, substitua `$VAR`, `$?`, etc.

- `$VAR` → `getenv("VAR")`
- `$?` → último código de saída
- dentro de aspas simples `' '` → **não expande**
- dentro de aspas duplas `" "` → expande

## Fase 5 — Executor

Aqui entra o sangue e o caos (fork, dup2, execve).

**Processo:**

1. Crie **pipes** entre comandos.
2. Para cada comando:
    - `fork()`
    - No filho:
        - `dup2()` para redirecionamentos e pipes
        - `execve()` para executar o binário
    - No pai:
        - Fecha pipes que não usa
        - `waitpid()`

**Importante:** as builtins (`cd`, `echo`, etc.) **não** precisam de fork se forem executadas isoladamente.

Mas dentro de um pipeline, sim.

## Fase 6 — Builtins

Implemente um a um:

- **echo [-n]**
- **cd [path]**
- **pwd**
- **export**
- **unset**
- **env**
- **exit**

Crie uma função:

```c
int	is_builtin(char *cmd);
int	exec_builtin(t_cmd *cmd, t_env *env);
```

## Fase 7 — Redirecionamentos

- `&gt;` → dup2(fd_out, STDOUT_FILENO)
- `&lt;` → dup2(fd_in, STDIN_FILENO)
- `&gt;&gt;` → open com `O_APPEND`
- `&lt;&lt;` → heredoc (leitura até delimitador)

**Heredoc:**

- lê até encontrar o delimitador
- salva num pipe ou arquivo temporário
- redireciona `STDIN` do comando

## Fase 8 — Sinais

**Comportamento bash:**

- `Ctrl+C`: interrompe comando, mas não mata shell
- `Ctrl+\`: ignora
- `Ctrl+D`: sai

Use `sigaction()` e `tcgetattr()` / `tcsetattr()` para controle fino.

## Fase 9 — Testes e Validação

Crie scripts simples:

```bash
echo hello
ls | wc -l
cat < file | grep word > out.txt
echo $USER
export VAR=ok && echo $VAR
```

E valide saída comparando com bash.

# 🧠 3. Variável Global Permitida

Apenas uma:

```c
volatile sig_atomic_t g_signal;
```

Guarde o número do último sinal recebido.

Use-a pra modificar comportamento do prompt, não pra armazenar dados.

# 💡 4. Dicas Estratégicas

- Comece **sem redirecionamentos nem pipes**. Faça um shell que executa um comando simples primeiro.
- Use `strace bash` pra observar comportamento real.
- Evite misturar parsing com execução.
- Debugue com prints de tokens e comandos estruturados.
- **Nunca** use funções proibidas (como `system()` ou `popen()`).

# 🚀 5. Etapas de Entrega

**Mínimo funcional para não zerar:**

- Prompt + histórico
- Execução simples (sem pipes/redirs)
- Variáveis `$` e `$?`
- Builtins básicos
- Ctrl+C/D/\ funcionando

Depois vá expandindo para:

- Pipes
- Redirs
- Heredoc

Só depois disso pense em bônus (`&amp;&amp;`, `||`, `*`).

---

Se quiser, posso montar o **esqueleto de código inicial** (Makefile, headers e funções base) pra você começar com um ambiente pronto e norminette-safe.

**Quer que eu monte essa base pra você agora?**

# 📋 Lista de Ações

- [ ]  Criar estrutura de pastas do projeto minishell
- [ ]  Implementar main() com loop de leitura usando readline()
- [ ]  Adicionar prompt (minishell$ ) e histórico com add_history()
- [ ]  Lidar com signal(SIGINT, handler) para Ctrl+C
- [ ]  Implementar lexer para tokenização da linha de comando
- [ ]  Criar estrutura de dados para tokens (lista ligada ou array de structs)
- [ ]  Implementar parser para montar comandos estruturados
- [ ]  Implementar expansão de variáveis ($VAR, $?)
- [ ]  Implementar executor com fork, dup2 e execve
- [ ]  Criar pipes entre comandos
- [ ]  Implementar builtin: echo [-n]
- [ ]  Implementar builtin: cd [path]
- [ ]  Implementar builtin: pwd
- [ ]  Implementar builtin: export
- [ ]  Implementar builtin: unset
- [ ]  Implementar builtin: env
- [ ]  Implementar builtin: exit
- [ ]  Implementar redirecionamento de saída (>)
- [ ]  Implementar redirecionamento de entrada (<)
- [ ]  Implementar redirecionamento com append (>>)
- [ ]  Implementar heredoc (<<)
- [ ]  Configurar sinais com sigaction() e tcgetattr()/tcsetattr()
- [ ]  Criar scripts de teste e validar saída comparando com bash
- [ ]  Usar strace bash para observar comportamento real

# 📝 Resumo do Projeto

- **Objetivo:** Construir um shell simples inspirado no bash, com prompt, parsing, execução de comandos e redirecionamentos
- **Estrutura modular:** Separar código em prompt, parser, executor e utils para facilitar manutenção
- **Fluxo principal:** Ler linha → tokenizar → parsear → expandir variáveis → executar com fork/execve
- **Implementações-chave:** Pipes, redirecionamentos (>, <, >>, <<), builtins (echo, cd, pwd, export, unset, env, exit)
- **Tratamento de sinais:** Ctrl+C interrompe comando sem matar shell, Ctrl+D sai, Ctrl+\ ignorado
- **Expansão de variáveis:** $VAR via getenv(), $? para código de saída, não expandir em aspas simples
- **Estratégia de desenvolvimento:** Começar simples (um comando só), adicionar pipes/redirs progressivamente, testar contra bash
- **Variável global permitida:** Apenas uma (volatile sig_atomic_t g_signal) para sinais

# 📅 Cronograma de Desenvolvimento (26 dias)

## Semana 1 (Dias 1-7): Fundação

- **Dia 1-2:** Criar estrutura de pastas, Makefile, headers básicos e main() com readline()
- **Dia 3-4:** Implementar prompt, histórico e tratamento básico de sinais (Ctrl+C, Ctrl+D)
- **Dia 5-6:** Desenvolver lexer para tokenização da linha de comando
- **Dia 7:** Criar estrutura de dados para tokens e testar parsing básico

## Semana 2 (Dias 8-14): Parser e Executor Básico

- **Dia 8-9:** Implementar parser para montar comandos estruturados
- **Dia 10-11:** Criar executor simples (fork + execve) sem pipes nem redirecionamentos
- **Dia 12-13:** Implementar expansão de variáveis ($VAR, $?)
- **Dia 14:** Testar execução de comandos simples e validar contra bash

## Semana 3 (Dias 15-21): Builtins e Pipes

- **Dia 15-16:** Implementar builtins: echo, cd, pwd
- **Dia 17-18:** Implementar builtins: export, unset, env, exit
- **Dia 19-20:** Adicionar suporte a pipes entre comandos
- **Dia 21:** Testar pipelines e ajustar comportamento de builtins em pipes

## Semana 4 (Dias 22-26): Redirecionamentos e Finalização

- **Dia 22:** Implementar redirecionamentos básicos (>, <)
- **Dia 23:** Implementar append (>>) e heredoc (<<)
- **Dia 24:** Refinar tratamento de sinais com sigaction() e tcgetattr()
- **Dia 25:** Testes extensivos, correção de bugs e validação contra bash
- **Dia 26:** Revisão final de norminette, leaks de memória e entrega

### ⚠️ Dicas para manter o cronograma:

- Não pule etapas — cada fase depende da anterior
- Teste constantemente comparando com bash
- Use valgrind diariamente para evitar acúmulo de leaks
- Se atrasar, priorize: executor simples → builtins → pipes → redirecionamentos
