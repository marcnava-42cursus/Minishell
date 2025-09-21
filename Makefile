# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 02:21:54 by marcnava          #+#    #+#              #
#    Updated: 2025/09/18 22:05:17 by marcnava         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# ********************************* VARIABLES ******************************** #

NAME		:=	minishell

CC			:=	cc
CFLAGS		:=	-Wall -Wextra
DFLAGS		:=	-g3 -fsanitize=address
LDFLAGS		:=	-lreadline -lncurses

SRCPATH		:=	src
BUILDPATH	:=	build

CONFIG		:=	$(SRCPATH)/config
EXEC		:=	$(SRCPATH)/exec
BUILTINS	:=	$(EXEC)/builtins
FORKERMAN	:=	$(SRCPATH)/forkerman
PARSER		:=	$(SRCPATH)/parser
SIGNALS		:=	$(SRCPATH)/signals
STRUCTS		:=	$(SRCPATH)/structs
SUGGESTIONS	:=	$(SRCPATH)/suggestions
UTILS		:=	$(SRCPATH)/utils
WILDCARDS	:=	$(SRCPATH)/wildcards

LIBFT		:=	libs/libft
LIBFT_A		:=	$(LIBFT)/libft.a

INCLUDES	:=	-Iincludes -I$(LIBFT)/includes

HEADERS		:=	includes/config.h \
				includes/exec.h \
				includes/exec_builtins.h \
				includes/forkerman.h \
				includes/minishell.h \
				includes/parser.h \
				includes/signals.h \
				includes/structs.h \
				includes/suggestions.h \
				includes/utils.h \
				includes/wildcards.h

SRCS		:=	$(SRCPATH)/minishell.c

SRCS		+=	$(CONFIG)/build_prompt.c \
				$(CONFIG)/colors.c \
				$(CONFIG)/create_config.c \
				$(CONFIG)/keywords.c \
				$(CONFIG)/keywords_system.c \
				$(CONFIG)/load_config.c \
				$(CONFIG)/config_utils.c \
				$(CONFIG)/config_parser.c \
				$(CONFIG)/prompt_utils.c

SRCS		+=	$(BUILTINS)/cd.c \
				$(BUILTINS)/echo.c \
				$(BUILTINS)/env.c \
				$(BUILTINS)/envp_helpers.c \
				$(BUILTINS)/exit.c \
				$(BUILTINS)/exit_utils.c \
				$(BUILTINS)/exit_numparse.c \
				$(BUILTINS)/export.c \
				$(BUILTINS)/pwd.c \
				$(BUILTINS)/unset.c

SRCS		+=	$(EXEC)/exec.c \
				$(EXEC)/exec_commands.c \
				$(EXEC)/exec_utils.c \
				$(EXEC)/exec_path.c \
				$(EXEC)/exec_env.c \
				$(EXEC)/exec_quotes.c \
				$(EXEC)/exec_command_utils.c \
				$(EXEC)/exec_external.c \
				$(EXEC)/exec_pipeline_utils.c \
				$(EXEC)/exec_pipeline_run.c \
				$(EXEC)/exec_pipeline.c \
				$(EXEC)/exec_errors.c \
				$(EXEC)/exec_logic.c \
				$(EXEC)/exec_builtin_utils.c

SRCS		+=	$(FORKERMAN)/bombs.c \
				$(FORKERMAN)/game.c \
				$(FORKERMAN)/map.c \
				$(FORKERMAN)/player.c \
				$(FORKERMAN)/render.c

SRCS		+=	$(PARSER)/expansion.c \
				$(PARSER)/expansion_core.c \
				$(PARSER)/parser_common.c \
				$(PARSER)/tokenizer.c \
				$(PARSER)/tokenizer_ops.c \
				$(PARSER)/tokenizer_ctx.c \
				$(PARSER)/tokenizer_read.c \
				$(PARSER)/parser_tree_helpers.c \
				$(PARSER)/parser_cmd.c \
				$(PARSER)/parser_cmd_redirs.c \
				$(PARSER)/parser_cmd_heredoc.c \
				$(PARSER)/parser_cmd_words.c \
				$(PARSER)/parser_cmd_utils.c \
				$(PARSER)/parser_subshell.c \
				$(PARSER)/heredoc.c \
				$(PARSER)/heredoc_utils.c \
				$(PARSER)/heredoc_flow.c \
				$(PARSER)/parser_main_helpers.c \
				$(PARSER)/parser_main_run.c \
				$(PARSER)/expansion_helpers.c \
				$(PARSER)/expansion_dquote.c \
				$(PARSER)/expansion_dollar.c \
				$(PARSER)/parser_utils.c \
				$(PARSER)/parser.c \
				$(PARSER)/parser_tree.c \
				$(PARSER)/save_envp.c

SRCS		+=	$(SIGNALS)/signal_handler.c

SRCS		+=	$(STRUCTS)/envp_manager.c \
				$(STRUCTS)/envp_utils.c \
				$(STRUCTS)/tree_manager.c

SRCS		+=	$(SUGGESTIONS)/sug_string_utils.c \
				$(SUGGESTIONS)/cmd_list.c \
				$(SUGGESTIONS)/path_loader.c \
				$(SUGGESTIONS)/terminal.c \
				$(SUGGESTIONS)/suggestion_main.c \
				$(SUGGESTIONS)/suggestion_utils.c \
				$(SUGGESTIONS)/suggestion_prompt.c

SRCS		+=	$(UTILS)/print_tree.c \
				$(UTILS)/matrix_utils.c \
				$(UTILS)/termios_utils.c \
				$(UTILS)/io_utils.c

SRCS		+=	$(WILDCARDS)/pattern.c \
				$(WILDCARDS)/pattern_utils.c \
				$(WILDCARDS)/path.c \
				$(WILDCARDS)/expand_utils.c \
				$(WILDCARDS)/expand.c \
				$(WILDCARDS)/utils.c

OBJS		:=	$(SRCS:$(SRCPATH)/%.c=$(BUILDPATH)/%.o)
DIRS		:=	$(sort $(dir $(OBJS)))

RM			:=	rm -rf

# **************************************************************************** #
# *********************************** RULES ********************************** #

check:
	@make -q all && echo "✅ Todo OK" || make all

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
		$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $(NAME)

$(BUILDPATH)/%.o: $(SRCPATH)/%.c $(HEADERS) $(LIBFT)/Makefile | dirs
		@$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES) -c $< -o $@

clean:
		$(RM) $(BUILDPATH)
		@if [ -d $(LIBFT) ]; then \
			$(MAKE) -C $(LIBFT) clean; \
		fi

fclean: clean
	$(RM) $(NAME)
	$(RM) minishell.config
	$(RM) libs

re: fclean all

.PHONY: check all clean fclean re dirs

# Create all build directories once
dirs:
		@mkdir -p $(DIRS)

$(LIBFT_A): $(LIBFT)/Makefile
		$(MAKE) -C $(LIBFT) all

$(LIBFT)/Makefile:
	git clone https://github.com/marcnava-42cursus/libft.git libs/libft
