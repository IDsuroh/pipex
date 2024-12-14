# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: suroh <suroh@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/10 21:12:22 by suroh             #+#    #+#              #
#    Updated: 2024/12/04 10:12:48 by suroh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -g -Wall -Wextra -Werror

SRC = pipex.c utils.c helpers.c errors.c
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
OBJDIR = ./obj_files
LIB = ./libft/
LIBFT = ./libft/libft.a

all: $(NAME)


$(NAME): $(OBJ) $(LIBFT)
	@echo "\033[33m------Compiling LIBFT------"
	@$(CC) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "\033[32m------Compilation Complete------"

$(LIBFT):
	@make makeall -C $(LIB)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
	@echo "\033[35mCleaned Object Files."

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(LIB)
	@find . -type f ! -name "*.c" ! -name "*.h" ! -name "Makefile" ! -path "./libft/*" -exec rm -f {} +
	@find . -type d ! -path "./libft" ! -path "." -exec rm -rf {} +
	@echo "\033[34mCleaned Everything."

re: fclean all

.PHONY: all clean fclean re
