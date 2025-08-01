/* suggest_inline_minimal.c */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc, free, exit */
#include <unistd.h>     /* read, write, access, STDIN_FILENO, STDOUT_FILENO */
#include <dirent.h>     /* opendir, readdir, closedir */
#include <limits.h>     /* PATH_MAX */
#include <termios.h>    /* tcgetattr, tcsetattr */
#include <term.h>       /* tgetent, tgetstr, tgoto, tputs */
#include <sys/stat.h>   /* stat */
#include <linux/limits.h>

#define PROMPT  "> "

/* ---- Funciones de cadena mínimas ---- */
static size_t my_strlen(const char *s) {
	size_t n = 0;
	while (s[n]) n++;
	return n;
}

static int is_prefix(const char *s, const char *p) {
	/* devuelve 1 si p es prefijo de s */
	size_t i = 0;
	while (p[i]) {
		if (s[i] != p[i]) return 0;
		i++;
	}
	return 1;
}

static char *my_strdup(const char *s) {
	size_t n = my_strlen(s);
	char *d = malloc(n+1);
	if (!d) return NULL;
	for (size_t i = 0; i < n; i++) d[i] = s[i];
	d[n] = '\0';
	return d;
}

/* ---- Lista dinámica de comandos ---- */
typedef struct {
	char **items;
	size_t  count, cap;
} CmdList;

static void cmdlist_init(CmdList *L) {
	L->items = NULL;
	L->count = L->cap = 0;
}

static void cmdlist_add(CmdList *L, const char *name) {
	/* No comprobamos duplicados para simplificar */
	if (L->count == L->cap) {
		size_t nc = (L->cap == 0 ? 256 : L->cap * 2);
		char **tmp = realloc(L->items, nc * sizeof(char*));
		if (!tmp) return;
		L->items = tmp;
		L->cap = nc;
	}
	L->items[L->count++] = my_strdup(name);
}

/* Carga todos los ejecutables de cada carpeta en $PATH */
static void load_commands(CmdList *L)
{
	char *path = getenv("PATH");
	if (!path) return;

	/* buffer para extraer cada directorio */
	char dirbuf[PATH_MAX];
	size_t dbi = 0;
	for (size_t i = 0; ; i++) {
		char c = path[i];
		if (c != ':' && c != '\0') {
			if (dbi + 1 < sizeof(dirbuf)) dirbuf[dbi++] = c;
		} else {
			/* fin de un directorio */
			dirbuf[dbi] = '\0';
			DIR *d = opendir(dirbuf);
			if (d) {
				struct dirent *e;
				char full[PATH_MAX];
				while ((e = readdir(d))) {
					/* construye ruta: dirbuf + "/" + e->d_name */
					size_t l1 = my_strlen(dirbuf);
					size_t l2 = my_strlen(e->d_name);
					if (l1 + 1 + l2 + 1 >= sizeof(full)) continue;
					/* copia */
					for (size_t k = 0; k < l1; k++) full[k] = dirbuf[k];
					full[l1] = '/';
					for (size_t k = 0; k <= l2; k++)
						full[l1+1+k] = e->d_name[k];
					/* si es ejecutable, agregar */
					if (access(full, X_OK) == 0) {
						cmdlist_add(L, e->d_name);
					}
				}
				closedir(d);
			}
			if (c == '\0') break;
			dbi = 0;
		}
	}
}

/* Busca la primera sugerencia que empiece por buf */
static const char *find_suggestion(CmdList *L, const char *buf)
{
	if (!buf[0]) return NULL;
	for (size_t i = 0; i < L->count; i++) {
		if (is_prefix(L->items[i], buf))
			return L->items[i];
	}
	return NULL;
}

/* ---- Terminal en modo raw ---- */
static struct termios orig_term;
static void enable_raw(void) {
	if (tcgetattr(STDIN_FILENO, &orig_term) == -1) {
		perror("tcgetattr");
		exit(1);
	}
	struct termios r = orig_term;
	r.c_lflag &= ~(ECHO | ICANON);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &r) == -1) {
		perror("tcsetattr");
		exit(1);
	}
}
static void disable_raw(void) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

/* para tputs */
static int puttc(int c) {
	char ch = c;
	return write(STDOUT_FILENO, &ch, 1);
}

int main(void)
{
	/* inicializa termcap */
	char *term = getenv("TERM");
	if (!term || tgetent(NULL, term) != 1) {
		/* no terminal válido */
	}
	char *ce = tgetstr("ce", NULL);   /* borrar hasta fin de línea */
	char *cm = tgetstr("cm", NULL);   /* cursor movement */

	CmdList cmds;
	cmdlist_init(&cmds);
	load_commands(&cmds);

	enable_raw();
	const size_t prompt_len = my_strlen(PROMPT);

	char buf[1024] = {0};
	size_t blen = 0;

	/* pinta prompt */
	write(STDOUT_FILENO, PROMPT, prompt_len);

	while (1) {
		char c;
		if (read(STDIN_FILENO, &c, 1) != 1) break;

		if (c == 3) {           /* Ctrl-C */
			disable_raw();
			write(STDOUT_FILENO, "\n", 1);
			exit(0);
		} else if (c == 127) {  /* Backspace */
			if (blen > 0) {
				blen--;
				buf[blen] = '\0';
			}
		} else if (c == '\r' || c == '\n') { /* Enter */
			disable_raw();
			/* borra resto y baja línea */
			if (ce) tputs(ce, 1, puttc);
			write(STDOUT_FILENO, "\n", 1);
			buf[blen] = '\0';
			printf("Ejecutar: %s\n", buf);
			break;
		} else if (c >= 32 && c <= 126) {
			if (blen + 1 < sizeof(buf)) {
				buf[blen++] = c;
				buf[blen] = '\0';
			}
		}

		/* redibuja la línea completa */
		if (cm) {
			/* mueve cursor al inicio de línea */
			tputs(tgoto(cm, 0, 0), 1, puttc);
		} else {
			write(STDOUT_FILENO, "\r", 1);
		}
		write(STDOUT_FILENO, PROMPT, prompt_len);
		write(STDOUT_FILENO, buf, blen);
		if (ce) tputs(ce, 1, puttc);

		/* busca sugerencia */
		const char *s = find_suggestion(&cmds, buf);
		if (s) {
			/* pinta la parte restante en gris */
			/* secuencia ANSI directa */
			write(STDOUT_FILENO, "\033[90m", 5);
			write(STDOUT_FILENO, s + blen, my_strlen(s + blen));
			write(STDOUT_FILENO, "\033[0m", 4);
		}
		/* coloca cursor tras lo escrito */
		if (cm) {
			tputs(tgoto(cm, prompt_len + blen, 0), 1, puttc);
		} else {
			/* sin termcap, dejamos el cursor después del buffer */
		}
	}

	/* libera memoria */
	for (size_t i = 0; i < cmds.count; i++)
		free(cmds.items[i]);
	free(cmds.items);

	return 0;
}
