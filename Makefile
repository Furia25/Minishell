# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/13 23:20:17 by val               #+#    #+#              #
#    Updated: 2025/03/12 18:02:59 by vdurand          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
RESET = \033[0m
BG_GREEN = \033[42m

NAME = minishell

SRC_DIR = src
MAIN_DIR = src_main
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = libs/libft

VERBOSE = 0

ifeq ($(VERBOSE),1)
    SILENT =
else
    SILENT = @
endif

ifeq ($(VERBOSE),1)
    DUMP_OUT =
else
    DUMP_OUT = > /dev/null 2>&1
endif

SRC_FILES = \
	ast_fonctions.c \
	main.c \

SRC = $(patsubst %.c, $(SRC_DIR)/%.c, $(SRC_FILES))
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEP = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SRC))

CC = cc
CFLAGS = -Werror -Wextra #-Wall
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)

all: makelibft $(NAME)

$(NAME): $(OBJ)
	$(SILENT)$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "$(BG_GREEN)>>> Program $(NAME) compiled!$(RESET)"

makelibft:
	@echo "$(BLUE)>>> Compiling Libft...$(RESET)"
	$(SILENT)$(MAKE) -C $(LIBFT_DIR) $(DUMP_OUT)
	$(SILENT)$(MAKE) bonus -C $(LIBFT_DIR) $(DUMP_OUT)
	@echo "$(GREEN)>>> Compilation achieved!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile $(INC_DIR)/*.h $(LIBFT_DIR)/libft.a | $(OBJ_DIR) 
	@echo "$(BLUE)>>> Compiling $<...$(RESET)"
	$(SILENT)$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	@echo "$(YELLOW)>>> Directory '$(OBJ_DIR)' created!$(RESET)"
	$(SILENT)mkdir -p $(OBJ_DIR)

cleanlibs:
	@echo "$(YELLOW)>>> Cleaning libs...$(RESET)"
	$(SILENT)$(MAKE) fclean -C $(LIBFT_DIR) $(DUMP_OUT)

clean:
	@echo "$(YELLOW)>>> Cleaning objects$(RESET)"
	$(SILENT)rm -rf $(OBJ_DIR) $(DUMP_OUT)

fclean: clean cleanlibs
	@echo "$(YELLOW)>>> Cleaning executable...$(RESET)"
	$(SILENT)rm -f $(NAME) $(DUMP_OUT)

re: fclean all

-include $(DEP)

.PHONY: all cleanlibs clean fclean re makelibft