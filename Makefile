# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 02:21:54 by marcnava          #+#    #+#              #
#    Updated: 2025/09/05 15:16:15 by marcnava         ###   ########.fr        #
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

SRCS		:=	$(SRCPATH)/minishell.c

SRCS		+=	$(CONFIG)/build_prompt.c \
				$(CONFIG)/colors.c \
				$(CONFIG)/create_config.c \
				$(CONFIG)/keywords.c \
				$(CONFIG)/load_config.c

SRCS		+=	$(BUILTINS)/cd.c \
				$(BUILTINS)/echo.c \
				$(BUILTINS)/env.c \
				$(BUILTINS)/envp_helpers.c \
				$(BUILTINS)/exit.c \
				$(BUILTINS)/export.c \
				$(BUILTINS)/pwd.c \
				$(BUILTINS)/unset.c

SRCS		+=	$(EXEC)/exec.c \
				$(EXEC)/exec_commands.c \
				$(EXEC)/exec_utils.c \
				$(EXEC)/exec_helpers.c \
				$(EXEC)/exec_command_utils.c \
				$(EXEC)/exec_pipeline_utils.c \
				$(EXEC)/exec_builtin_utils.c

SRCS		+=	$(PARSER)/expansion.c \
				$(PARSER)/parser_utils.c \
				$(PARSER)/parser.c \
				$(PARSER)/parser_tree.c \
				$(PARSER)/save_envp.c


SRCS        +=  $(SIGNALS)/signal_handler.c

SRCS        +=  $(STRUCTS)/envp_manager.c \
				$(STRUCTS)/tree_manager.c

SRCS		+=	$(SUGGESTIONS)/sug_string_utils.c \
				$(SUGGESTIONS)/cmd_list.c \
				$(SUGGESTIONS)/path_loader.c \
				$(SUGGESTIONS)/terminal.c \
				$(SUGGESTIONS)/suggestion_main.c

SRCS		+=	$(UTILS)/print_tree.c \
					$(UTILS)/matrix_utils.c \
					$(UTILS)/termios_utils.c

# Wildcards (globbing '*')
SRCS		+=	$(WILDCARDS)/pattern.c \
					$(WILDCARDS)/path.c \
					$(WILDCARDS)/expand.c \
					$(WILDCARDS)/utils.c

SRCS		+=	$(FORKERMAN)/bombs.c \
				$(FORKERMAN)/game.c \
				$(FORKERMAN)/map.c \
				$(FORKERMAN)/player.c \
				$(FORKERMAN)/render.c

OBJS		:=	$(SRCS:$(SRCPATH)/%.c=$(BUILDPATH)/%.o)
DIRS		:=	$(sort $(dir $(OBJS)))

RM			:=	rm -rf

# **************************************************************************** #
# *********************************** RULES ********************************** #

check:
	@make -q all && echo "✅ Todo OK" || make all

all: libft $(NAME)

$(NAME): dirs $(OBJS)
		$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $(NAME)

$(BUILDPATH)/%.o: $(SRCPATH)/%.c
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
$(OBJS): | dirs libft

dirs:
		@echo "Preparing build directories..."
		@mkdir -p $(DIRS)

libft: $(LIBFT)/Makefile
		$(MAKE) -C $(LIBFT) all

$(LIBFT)/Makefile:
	git clone https://github.com/marcnava-42cursus/libft.git libs/libft

.PHONY: libft