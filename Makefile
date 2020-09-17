# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/17 13:09:18 by ohakola           #+#    #+#              #
#    Updated: 2020/09/17 18:30:34 by ohakola          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = libecs.a
LIBFT = ./libft
DIR_SRC = ./
DIR_TESTS = ./tests
DIR_OBJ = temp
SOURCES = entities.c \
			entity_utils_1.c \
			entity_utils_2.c \
			components.c \
			systems.c \
			world.c

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

# Tests

TEST_SRCS =$(wildcard $(DIR_TESTS)/*.c)
test: all
	@make -C $(LIBFT)
	$(CC) -o test_run $(TEST_SRCS) -L$(LIBFT) -lft $(FLAGS) $(INCLUDES) $(NAME)
	./test_run
	@/bin/rm -f main.o
	@/bin/rm -f test_run
	@make fclean
	@make -C $(LIBFT) fclean

# Demo

DIR_DEMO = ./demo
DEMO_SRCS =$(wildcard $(DIR_DEMO)/*.c)
LIBSDL2 = $(DIR_DEMO)/SDL2
DEMO_FLAGS = -rpath $(LIBSDL2) \
				-framework SDL2 -F$(LIBSDL2)/ \
				-framework SDL2_image -F$(LIBSDL2)/ \
				-framework SDL2_ttf -F$(LIBSDL2)/ \
				-L$(LIBFT) -lft
DEMO_INCLUDES = -I$(DIR_DEMO)/include \
				-I$(LIBSDL2)/SDL2.framework/Headers \
				-I$(LIBSDL2)/SDL2_image.framework/Headers \
				-I$(LIBSDL2)/SDL2_ttf.framework/Headers
demo: all
	@make -C $(LIBFT)
	$(CC) -o demo_run $(DEMO_SRCS) \
		$(FLAGS) $(INCLUDES) $(DEMO_INCLUDES) $(DEMO_FLAGS) $(NAME)
	./demo_run
	@/bin/rm -f main.o
	@/bin/rm -f demo_run
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
