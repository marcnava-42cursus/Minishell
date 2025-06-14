# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 02:21:54 by marcnava          #+#    #+#              #
#    Updated: 2025/04/24 02:47:55 by marcnava         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# ********************************* VARIABLES ******************************** #

# Your program name
NAME				:=

# Compiler, you can change it to use cc, gcc or clang
CC					:=	cc
# Compilation flags
CFLAGS				:=	-Wall -Wextra -Werror
# Debug flags, comment this line if you don't want to use it
DFLAGS				:=	-g3
# Compilation alias with flags
COMPILER			:=	$(CC) $(CFLAGS) $(DFLAGS)

#! Command for delete any file or folder WARNING DANGEROUS COMMAND
RM					:=	rm -rf

# You can add your custom variables here
# ==============================================================================

# ==============================================================================

# **************************************************************************** #
# *********************************** FILES ********************************** #

# The paths to source files and auto-generated object files
SRCPATH				:=	src
BUILDPATH			:=	build

# Path to header files
INCLUDES			:=	-Iincludes

# Here you have to put all your .c files
SRCS				:=

# Auto-generated object files using source and build paths
OBJS				:=	$(SRCS:$(SRCPATH)/%.c=$(BUILDPATH)/%.o)

# You can add your custom files and file paths here
# ==============================================================================

# ==============================================================================

# **************************************************************************** #
# *********************************** RULES ********************************** #

#! If you want to hide the logs of the rules, you have to put @ before        !#
#! any command                                                                !#

all:				$(NAME)

# If you are using libft or any lib, you have to put -L<lib path> before -o flag
$(NAME):			$(OBJS)
	$(CC) $(INCLUDES) $(OBJS) -o $(NAME)

$(BUILDPATH)/%.o:	$(SRCPATH)/%.c
	mkdir -p $(@D)
	$(COMPILER) -o $@ -c $<

clean:
	$(RM) $(BUILDPATH)

fclean:				clean
	$(RM) $(NAME)

re:					fclean all

bonus:
	echo "Optional rule"

# You can add your custom rules here
# ==============================================================================

# ==============================================================================

# **************************************************************************** #
# *********************************** PHONY ********************************** #

.PHONY: all
.PHONY: clean
.PHONY: fclean
.PHONY: re
.PHONY: bonus

# You can add your custom .PHONY rules here
# ==============================================================================

# ==============================================================================