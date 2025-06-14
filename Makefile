# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 02:21:54 by marcnava          #+#    #+#              #
#    Updated: 2025/06/14 18:01:06 by marcnava         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# ********************************* VARIABLES ******************************** #

NAME				:= minishell

CC					:=	cc
CFLAGS				:=	-Wall -Wextra -Werror
DFLAGS				:=	-g3

COMPILER			:=	$(CC) $(CFLAGS) $(DFLAGS)

RM					:=	rm -rf

# **************************************************************************** #
# *********************************** FILES ********************************** #

SRCPATH				:=	src
BUILDPATH			:=	build

INCLUDES			:=	-Iincludes

SRCS				:=	$(SRCPATH)/minishell.c

OBJS				:=	$(SRCS:$(SRCPATH)/%.c=$(BUILDPATH)/%.o)

# **************************************************************************** #
# *********************************** RULES ********************************** #

all:				$(NAME)

$(NAME):			$(OBJS)
	$(COMPILER) $(INCLUDES) $(OBJS) -o $(NAME)

$(BUILDPATH)/%.o:	$(SRCPATH)/%.c
	mkdir -p $(@D)
	$(COMPILER) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) $(BUILDPATH)

fclean:				clean
	$(RM) $(NAME)

re:					fclean all

$(LIBFT)/libft.a:
	@$(MAKE) --no-print-directory -s -C $(LIBFT)

# **************************************************************************** #
# *********************************** PHONY ********************************** #

.PHONY: all
.PHONY: clean
.PHONY: fclean
.PHONY: re
