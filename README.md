# Minishell - README Completo y Detallado

## ✨ Objetivo General

Crear un shell sencillo al estilo Bash. Aprenderás a gestionar:

- Procesos (fork, exec, wait)
- Redirecciones de ficheros
- Pipes
- Variables de entorno
- Manejo de señales
- Lectura de input interactivo (readline)
- Builtins (comandos internos)

---

# ✅ FUNCIONALIDADES OBLIGATORIAS

### Entrada del usuario

- Mostrar un prompt mientras espera comandos
- Leer la línea de comandos (usando `readline()`)
- Almacenar el historial de comandos (`add_history()`)

### Ejecución de comandos

- Identificar si es un builtin o un binario externo
- Buscar el binario usando la variable de entorno `PATH`
- Gestionar rutas relativas o absolutas
- Gestionar el último status de salida (`$?`)

### Builtins requeridos

- `echo` (soportar `-n`)
- `cd` (solo rutas absolutas o relativas)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### Redirecciones

- `<` Redireccionar entrada
- `>` Redireccionar salida (truncar)
- `>>` Redireccionar salida (append)
- `<<` Here-Document

### Pipes

- Soportar pipelines: `cmd1 | cmd2 | cmd3`

### Variables de entorno

- Expansiones de `$VAR` y `$?`
- Variables dentro de comillas:
  - Comillas simples: no expanden nada
  - Comillas dobles: expanden variables

### Señales

- `ctrl-C` (SIGINT): muestra prompt nuevo
- `ctrl-D` (EOF): termina el shell
- `ctrl-\` (SIGQUIT): no hacer nada

---

# 🌐 FUNCIONES PERMITIDAS (SOLO ESTAS)

```
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay,
add_history, printf, malloc, free, write, access, open, read, close, fork,
wait, waitpid, wait3, wait4, signal, sigaction, kill, exit, getcwd, chdir,
stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir,
closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv,
tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
```

Además, puedes usar tu `libft`.

---

# 🧪 DISEÑO GENERAL DEL PROYECTO

## 1. Estructura de ficheros sugerida

```
minishell/
|➔ includes/       # Headers
|➔ srcs/           # Código fuente
|➔ srcs/parser/   # Parsing del input
|➔ srcs/executor/ # Ejecución de comandos
|➔ srcs/builtins/  # Implementación de builtins
|➔ srcs/utils/     # Funciones auxiliares
|➔ libft/          # Tu libft
|➔ Makefile
```

## 2. Paso a paso de implementación

### Paso 1 - Setup inicial

- Crear `Makefile` con reglas: `all`, `clean`, `fclean`, `re`, `bonus`.
- Incluir la `libft` dentro del repo.

### Paso 2 - Leer input

- Usar `readline()` para obtener la línea del usuario.
- Controlar `EOF` (ctrl-D).
- Guardar en historial con `add_history()`.

### Paso 3 - Parsing del input

- Tokenizar la entrada:
  - Separar comandos, pipes, redirecciones, y argumentos.
  - Gestionar comillas simples y dobles.
  - Expansiones de variables (`$VAR`, `$?`)
- **Recomendado:** crear un lexer (tokenizer) y parser modular.

### Paso 4 - Abstract Syntax Tree (opcional pero recomendado)

- Crear una estructura tipo AST para representar los comandos y sus relaciones (pipes, redirecciones).

### Paso 5 - Redirecciones

- Antes de ejecutar el comando:
  - Usar `open()` para abrir ficheros.
  - Usar `dup2()` para redirigir `stdin` / `stdout`.
  - Cerrar descriptores antiguos (`close()`).

### Paso 6 - Pipes

- Crear pipes con `pipe()`
- Gestionar el fork de procesos en cada pipe
- Usar `dup2()` para redirigir los extremos de los pipes
- Controlar cierre de descriptores

### Paso 7 - Ejecución de comandos

- Comprobar si es builtin o externo:
  - Builtins: ejecutar en el mismo proceso o subshell según contexto
  - Externos:
    - Usar `fork()`
    - En el hijo: `execve()`
    - En el padre: `waitpid()`

### Paso 8 - Variables de entorno

- Crear copia interna del entorno (`char **envp`)
- Gestionar `export`, `unset`, `env`
- Gestionar la expansión de `$VAR` en el parser

### Paso 9 - Builtins

Implementar cada builtin según especificaciones POSIX:

- `echo -n`: imprimir argumentos separados por espacio, sin salto de línea final si `-n`.
- `cd`: cambiar directorio usando `chdir()`. Gestionar `HOME`, `OLDPWD`, errores.
- `pwd`: imprimir directorio actual usando `getcwd()`.
- `export`: modificar variables de entorno.
- `unset`: eliminar variables.
- `env`: mostrar todas las variables.
- `exit`: terminar el shell.

### Paso 10 - Señales

- Registrar handlers con `signal()` o `sigaction()`
- `SIGINT` (ctrl-C): mostrar prompt nuevo
- `SIGQUIT` (ctrl-): ignorar
- `EOF` (ctrl-D): terminar shell limpiamente

### Paso 11 - Control de memoria

- Todo `malloc()` debe tener su `free()`
- Controlar leaks: usar `valgrind` o `leaks` (en MacOS)

---

# 🔗 COMPORTAMIENTOS POSIX IMPORTANTES

- El shell por defecto siempre espera en modo interactivo (ver `isatty()`).
- Los pipelines crean procesos por cada comando.
- Los redireccionamientos afectan SOLO al comando actual.
- Las variables de entorno son heredadas por los hijos (ver `execve()`).
- El status de salida de un pipeline es el del último comando.
- Las variables solo existen en el shell actual si son exportadas.

---

# 🏆 TESTEO RECOMENDADO

- Crear un script de tests automáticos
- Testear:
  - Pipes anidados
  - Redirecciones combinadas
  - Comillas abiertas
  - Variables inexistentes
  - Builtins encadenados
  - Comandos inexistentes
  - Invalid file descriptors
  - Signals mientras ejecuta

---

# 🔧 FUNCIONES Y COMANDOS CLAVE

| Objetivo           | Funciones                                                        |
| ------------------ | ---------------------------------------------------------------- |
| Leer input         | `readline()`, `add_history()`                                    |
| Procesos           | `fork()`, `execve()`, `waitpid()`                                |
| Pipes              | `pipe()`, `dup2()`, `close()`                                    |
| Redirecciones      | `open()`, `dup2()`, `close()`                                    |
| Variables          | `getenv()`, `setenv()` (manualmente, no está permitido setenv()) |
| Directorios        | `chdir()`, `getcwd()`                                            |
| Señales            | `signal()`, `sigaction()`                                        |
| Gestion de errores | `perror()`, `strerror()`                                         |

---

# 📊 CONSEJOS DE DESARROLLO

- Empieza por el parser: es lo más complejo.
- Implementa primero solo comandos simples.
- Añade pipes y redirecciones progresivamente.
- Implementa signals desde el principio.
- Cada funcionalidad nueva, testéala exhaustivamente.
- Nunca asumas que el input es correcto: valida siempre.
- Usa herramientas como `valgrind`, `gdb`, `leaks`, `strace` para depurar.

---

# 🚀 BONUS (solo si la parte obligatoria está PERFECTA)

- Operadores `&&` y `||`
- Wildcards (`*`)
- Agrupación con paréntesis

---

# 📄 MAKEFILE MÍNIMO

```Makefile
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = $(wildcard srcs/**/*.c)
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

---

# 🚫 ERRORES QUE DESTRUYEN TU NOTA

- Cualquier segfault, double free, memory leak
- Violaciones de Norminette
- No gestionar signals correctamente
- Input mal parseado (comillas, escapes, etc)
- No liberar la memoria de cada input
- Variables de entorno mal gestionadas

---

# 🧹 RECURSOS POSIX RECOMENDADOS

- [POSIX Standard](https://pubs.opengroup.org/onlinepubs/9699919799/)
- `man 2 open`, `man 2 dup2`, `man 2 pipe`, `man 3 execve`, `man 2 fork`, etc.
- `man readline` (GNU Readline Library)

---

# 🔧 Este README es tu roadmap. Si sigues esto paso a paso, conseguirás un Minishell limpio, estable, portable, y que pasará cualquier evaluación de 42.

---

# Testers

alvamart (Alvaro Martin)