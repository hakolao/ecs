# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/17 13:09:18 by ohakola           #+#    #+#              #
#    Updated: 2020/09/12 15:33:13 by ohakola          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = libecs.a
LIBFT = ./libft
DIR_SRC = ./
DIR_TESTS = ./tests
DIR_OBJ = temp
SOURCES = entity.c

SRCS = $(addprefix $(DIR_SRC)/,$(SOURCES))
OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))
INCLUDES = -I ./include -I $(LIBFT)/include
FLAGS = -Wall -Wextra -Werror

all: $(DIR_OBJ) $(NAME)

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)

$(DIR_OBJ):
	@mkdir -p temp

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

TEST_SRCS =$(wildcard $(DIR_TESTS)/*.c)

test: all
	@make -C $(LIBFT)
	$(CC) -o test_run $(TEST_SRCS) -L$(LIBFT) -lft $(FLAGS) $(INCLUDES) $(NAME)
	./test_run
	@/bin/rm -f main.o
	@/bin/rm -f test_run
	@make fclean
	@make -C $(LIBFT) fclean

clean:
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)
	@make -C $(LIBFT) clean

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C $(LIBFT) fclean

re: fclean all

.PHONY: all, $(DIR_OBJ), clean, fclean, re
