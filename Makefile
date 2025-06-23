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
CFLAGS		:=	-Wall -Wextra -Werror -g3
LDFLAGS		:=	-lreadline -lncurses

SRCPATH		:=	src
BUILDPATH	:=	build
CONFIGPATH	:=	$(SRCPATH)/config

LIBFT		:=	libs/libft
LIBFT_A		:=	$(LIBFT)/libft.a

INCLUDES	:=	-Iincludes -I$(LIBFT)/includes

SRCS		:=	$(SRCPATH)/minishell.c

SRCS		+=	$(CONFIGPATH)/build_prompt.c \
				$(CONFIGPATH)/colors.c \
				$(CONFIGPATH)/create_config.c \
				$(CONFIGPATH)/keywords.c \
				$(CONFIGPATH)/load_config.c

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
