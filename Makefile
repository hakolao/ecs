# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/17 13:09:18 by ohakola           #+#    #+#              #
#    Updated: 2020/12/14 13:38:23 by ohakola          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
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
			systems_utils.c \
			systems_parallel.c \
			world.c

SRCS = $(addprefix $(DIR_SRC)/,$(SOURCES))
OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))
INCLUDES = -I ./include -I $(LIBFT)/include
FLAGS = -Wall -Wextra -Werror -O3 -flto

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
	$(CC) -lpthread -O3 -flto \
		-o test_run $(TEST_SRCS) -I$(DIR_TESTS)/include \
			 -L$(LIBFT) -lft $(FLAGS) $(INCLUDES) $(NAME)
	./test_run
	@/bin/rm -f main.o
	@/bin/rm -f test_run
	@make fclean
	@make -C $(LIBFT) fclean

# Demo

DIR_DEMO = ./demo
LIBSDL2 = $(DIR_DEMO)/SDL2
LIB3D = ./demo/lib3d
LIBGMATRIX = ./demo/libgmatrix
DEMO_SRCS = $(wildcard $(DIR_DEMO)/*.c)
DEMO_FLAGS =  -lpthread -O3 -flto \
				-rpath $(LIBSDL2) \
				-framework SDL2 -F$(LIBSDL2)/ \
				-framework SDL2_image -F$(LIBSDL2)/ \
				-framework SDL2_ttf -F$(LIBSDL2)/ \
				-L$(LIBFT) -lft
DEMO_INCLUDES = -I$(DIR_DEMO)/include \
				-I$(LIBSDL2)/SDL2.framework/Headers \
				-I$(LIBSDL2)/SDL2_image.framework/Headers \
				-I$(LIBSDL2)/SDL2_ttf.framework/Headers
DEMO_SQUARE_SRCS = $(wildcard $(DIR_DEMO)/demo_squares/*.c)
demo_squares: all
	@make -C $(LIBFT)
	@make -C $(LIBGMATRIX)
	@make -C $(LIB3D)
	$(CC) -o demo_squares $(DEMO_SRCS) $(DEMO_SQUARE_SRCS) \
		-I$(LIBGMATRIX)/include \
		-I$(LIB3D)/include \
		-L$(LIBGMATRIX) -lgmatrix \
		-L$(LIB3D) -l3d \
		-I$(DIR_DEMO)/demo_squares/include \
		$(FLAGS) $(INCLUDES) $(DEMO_INCLUDES) $(DEMO_FLAGS) $(NAME)
	./demo_squares
	@/bin/rm -f main.o
	@make clean

clean:
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)
	@make -C $(LIBFT) clean
	@make -C $(LIBGMATRIX) clean
	@make -C $(LIB3D) clean

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C $(LIBFT) fclean
	@make -C $(LIBGMATRIX) fclean
	@make -C $(LIB3D) fclean
	@if [ -a demo_squares ]; then rm demo_squares; fi;
	@if [ -a test_run ]; then rm test_run; fi;

re: fclean all

.PHONY: all, $(DIR_OBJ), clean, fclean, re
