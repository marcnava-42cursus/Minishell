# Repository Guidelines

## Project Structure & Module Organization
- Source in `src/` grouped by domain: `config/`, `exec/` (and `exec/builtins/`), `parser/`, `signals/`, `structs/`, `suggestions/`, `utils/`, `wildcards/`. Entrypoint: `src/minishell.c`.
- Public headers in `includes/` mirror the module layout.
- Third‑party library: `libs/libft` (auto‑cloned/built by the Makefile).
- Build artifacts in `build/`; executable at `./minishell`.
- Tests in `tests/`; external suite in `minishell_tester/`.
- Runtime configuration: `minishell.config` (prompt/suggestions).

## Build, Test, and Development Commands
- `make` / `make all`: build `minishell` (clones and builds `libft` if missing).
- `make check`: no‑op if up‑to‑date; otherwise builds.
- `make clean | fclean | re`: clean objects, binary/config/libs, or rebuild.
- Run locally: `./minishell` (reads `minishell.config`).
- Dependencies: `readline` and `ncurses` dev libs. Example (Debian/Ubuntu): `sudo apt-get install libreadline-dev libncurses-dev`.

## Coding Style & Naming Conventions
- Follow 42 Norminette: functions ≤ 25 lines, one instruction per line, no assignments in control expressions.
- Indentation uses tabs for levels; avoid trailing spaces; keep lines short (<=80 cols).
- Files and functions use `snake_case`; module helpers may use `ms_` prefix.
- Start each file with the 42 header; header guards in UPPER_SNAKE_CASE.
- Include via `includes/…`; keep internal helpers `static` within module `.c` files.

## Testing Guidelines
- Full suite: `./tests/comprehensive_test.sh`.
- Categories: `./tests/run_specific_tests.sh <basic|builtin|pipeline|redirect|heredoc|error|signal|memory|performance|edge|integration>`.
- Bash parity: `./tests/compare_with_bash.sh`.
- Leaks: `./tests/valgrind_test_minishell.sh`.
- Add new tests as shell scripts under `tests/` (e.g., `feature_quotes.sh`) and ensure they execute `./minishell` from repo root.

## Commit & Pull Request Guidelines
- Commits: concise, imperative mood (e.g., `parser: handle odd quotes`). Group related changes.
- PRs: clear description, motivation, and risks; list commands used to test; link issues; include valgrind output for memory‑sensitive changes. Screenshots not required.

## Security & Configuration Tips
- Do not commit secrets or machine‑local paths.
- Adjust prompt/features via `minishell.config` (e.g., `SUGGESTIONS: enabled`).
