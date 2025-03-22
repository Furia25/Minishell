.PHONY : all clean fclean re
LIBFT_DIR = libft
SRC_DIR = srcs
OBJ_DIR = objs
DEPS_DIR = deps

SRC = ft_atoi.c ft_bzero.c ft_realloc.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_putchar_fd.c ft_memset.c ft_putendl_fd.c ft_putnbr_fd.c ft_split.c ft_putstr_fd.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strcmp.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c get_next_line.c ft_conversions.c printf.c

INC = incs

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
DEPS = $(addprefix $(DEPS_DIR)/, $(SRC:.c=.d) $(SRC_BONUS:.c=.d))

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC)
MAKE = @make --no-print-directory -j

all : $(NAME)

$(NAME) : $(OBJ)
	ar rcs $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c Makefile | $(OBJ_DIR) $(DEPS_DIR)
	$(CC) $(CFLAGS) -MMD -MP -MF $(DEPS_DIR)/$*.d -o $@ -c $<

$(OBJ_DIR) $(DEPS_DIR):
	@mkdir -p $@

-include $(DEPS)

clean :
	rm -rf $(OBJ_DIR) $(DEPS_DIR)

fclean : clean
	rm -f $(NAME) $(NAME_BONUS)

re : fclean all
