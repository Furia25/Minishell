# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alpayet <alpayet@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2025/05/30 02:52:15 by alpayet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#	ANSI TEXT COLORS
BLACK = \033[30m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
WHITE = \033[37m

#	ANSI BACKGROUND COLORS
BG_BLACK = \033[40m
BG_RED = \033[41m
BG_GREEN = \033[42m
BG_YELLOW = \033[43m
BG_BLUE = \033[44m
BG_MAGENTA = \033[45m
BG_CYAN = \033[46m
BG_WHITE = \033[47m

#	STYLES
BOLD = \033[1m
DIM = \033[2m
ITALIC = \033[3m
UNDERLINE = \033[4m
BLINK = \033[5m
REVERSE = \033[7m
HIDDEN = \033[8m

RESET = \033[0m
##################

READLINE_SUPP_DIR = ignore_readline.supp
DEBUG_VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=all --track-origins=yes --suppressions=$(READLINE_SUPP_DIR) -s

SRC_DIR = src
MAIN_DIR = src_main
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = libft

NAME = minishell

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
	main/main.c \
	main/main_script_handlers.c \
	main/main_shell_handler.c \
	main/parsing_exec.c \
	builtins/cd_builtin.c \
	builtins/echo_builtin.c \
	builtins/env_builtin.c \
	builtins/export_builtin.c \
	builtins/pwd_builtin.c \
	builtins/unset_builtin.c \
	builtins/exit_builtin.c \
	builtins/builtins.c \
	data_structures/create_node_ast.c \
	data_structures/hashmap_managing.c \
	data_structures/hashmap_methods.c \
	data_structures/hashmap_utils.c \
	data_structures/lst_functions.c \
	environment/env_managing.c \
	environment/env_methods.c \
	environment/env_populate.c \
	errors_and_debug/errors.c \
	errors_and_debug/not_interpret_error.c \
	errors_and_debug/print_str.c \
	errors_and_debug/print_argv.c \
	errors_and_debug/print_ast.c \
	errors_and_debug/print_cmd.c \
	errors_and_debug/print_lst.c \
	errors_and_debug/syntax_error.c \
	exec/evaluate_ast.c \
	exec/evaluate_logical_op.c \
	exec/evaluate_pipe_op.c \
	exec/execute_cmd.c \
	exec/exec.c \
	exec/exec_utils.c \
	garbage_collector/garbage_functions.c \
	garbage_collector/garbage_managing.c \
	lexing/quotes_token.c \
	lexing/tokenization.c \
	lexing/word_token.c \
	parsing/ev_expand_cmd_substitution/ev_expand.c \
	parsing/ev_expand_cmd_substitution/ev_expand_in_here_doc.c \
	parsing/ev_expand_cmd_substitution/ev_expand_cmd_substitution_in_cmd.c \
	parsing/ev_expand_cmd_substitution/ev_expand_cmd_substitution_in_par_cmd.c \
	parsing/ev_expand_cmd_substitution/cmd_substitution.c \
	parsing/ev_expand_cmd_substitution/ev_expand_cmd_substitution_utils.c \
	parsing/redirect/redirections_in_cmd.c \
	parsing/redirect/redirections_in_par_cmd.c \
	parsing/redirect/here_doc_write.c \
	parsing/redirect/here_doc_utils.c \
	parsing/redirect/here_docs.c \
	parsing/redirect/redis_except_here_doc.c \
	parsing/merge_joined_tokens_in_cmd.c \
	parsing/merge_joined_tokens_in_par_cmd.c \
	parsing/command_table.c \
	parsing/parse_cmd.c \
	parsing/create_ast.c \
	parsing/tokens_to_argv.c \
	parsing/tokens_to_str.c \
	utils/memory_utils.c \
	utils/strjoin_alt.c \
	utils/misc_utils.c \
	utils/parentheses_utils.c \
	utils/errors_utils.c \
	wildcards/wildcards_search.c \
	wildcards/wildcards.c \
	wildcards/wildcards_in_cmd.c \
	wildcards/wildcards_in_par_cmd.c \
	signals/signals_setups.c

SRC = $(patsubst %.c, $(SRC_DIR)/%.c, $(SRC_FILES))
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEP = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SRC))

BASH_DEFAULT_PATH := $(shell \
    env -i bash --posix --noprofile --norc -c 'printf "%s" "$$PATH"' \
)

CC = cc -g3
CFLAGS = -Werror -Wextra -Wall
# CHANGE MINISHELL_HARD_PATH TO BASH DEFAULT HARD PATH IF NOT EMPTY
CFLAGS += $(if $(strip $(BASH_DEFAULT_PATH)),\
            -DMINISHELL_HARD_PATH=\"$(BASH_DEFAULT_PATH)\",\
            )
READLINE_INC = -I/usr/local/include
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR) $(READLINE_INC)
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
FLAGS = $(CFLAGS)

all: makelibft $(NAME)

$(NAME): $(OBJ)
	$(SILENT)$(CC) $(FLAGS) $(OBJ) $(LDFLAGS) -o $@
	@echo "$(BG_GREEN)>>> Program $(NAME) compiled!$(RESET)"

makelibft:
	@echo "$(BLUE)>>> Compiling Libft...$(RESET)"
	$(SILENT)$(MAKE) -C $(LIBFT_DIR) $(DUMP_OUT)
	$(SILENT)$(MAKE) bonus -C $(LIBFT_DIR) $(DUMP_OUT)
	@echo "$(GREEN)>>> Compilation achieved!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile $(LIBFT_DIR)/libft.a | $(OBJ_DIR)
	$(SILENT) mkdir -p $(dir $@)
	@echo "$(BLUE)>>> Compiling $<...$(RESET)"
	$(SILENT)$(CC) $(FLAGS) -MMD -MP $(INCLUDES) -c $< -o $@

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

debug: all
	$(DEBUG_VALGRIND) ./$(NAME) $(ARGS)

debug_env: all
	env -i $(DEBUG_VALGRIND) ./$(NAME) $(ARGS)

debug_trueclean: all
	$(DEBUG_VALGRIND) env -i ./$(NAME) $(ARGS)

-include $(DEP)

.PHONY: all cleanlibs clean fclean re makelibft debug debug_env debug_trueclean
