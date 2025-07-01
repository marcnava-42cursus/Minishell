# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 02:21:54 by marcnava          #+#    #+#              #
#    Updated: 2025/06/16 20:00:00 by marcnava         ###   ########.fr        #
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
PARSER		:=	$(SRCPATH)/parser
STRUCTS		:=	$(SRCPATH)/structs

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
				$(BUILTINS)/exit.c \
				$(BUILTINS)/export.c \
				$(BUILTINS)/pwd.c \
				$(BUILTINS)/unset.c

SRCS		+=	$(PARSER)/parser.c \
				$(PARSER)/save_envp.c

SRCS		+=	$(STRUCTS)/envp_manager.c \
				$(STRUCTS)/tree_manager.c

OBJS		:=	$(SRCS:$(SRCPATH)/%.c=$(BUILDPATH)/%.o)

RM			:=	rm -rf

# **************************************************************************** #
# *********************************** RULES ********************************** #

all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $(NAME)

$(BUILDPATH)/%.o: $(SRCPATH)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(BUILDPATH)
	$(MAKE) -C $(LIBFT) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT) fclean

re: fclean all

$(LIBFT_A):
	$(MAKE) -C $(LIBFT)

.PHONY: all clean fclean re
