NAME = minishell
SOURCES = src/main.c \
		  src/builtins/ft_cd.c \
		  src/builtins/ft_echo.c \
		  src/builtins/ft_exit.c \
		  src/builtins/ft_pwd.c \
		  src/builtins/builtin.c \
		  src/utils/ft_atoi.c \
		  src/utils/ft_isdigit.c \
		  src/utils/ft_putstr_fd.c \
		  src/utils/ft_strcmp.c \
		  src/utils/ft_strlen.c \
		  src/redirection/handle_redir.c \
		  src/execute/exe_cmd.c \
		  src/pipe/pipe_it.c \
		  src/builtins/ft_env.c \
		  src/builtins/ft_export.c \
		  src/builtins/ft_unset.c \
		  src/utils/ft_strdup.c \
		  src/utils/ft_calloc.c \
		  src/parser/parser.c \
		  src/utils/ft_split.c \
		  src/utils/ll_free.c \
		  src/parser/heredoc_parse.c \
		  src/environment/env_handle.c \
		  src/utils/env_strcmp.c \
		  src/parser/tokenizer.c \
		  src/utils/debug_menu.c \
		  src/parser/parserV3.c

CC = cc
CFLAGS = -Werror -Wextra -Wall -g3
OBJS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

help:
		@echo "Available rules:"
		@echo "all       - Build the library."
		@echo "clean     - Remove object files."
		@echo "fclean    - Remove object files and the library."
		@echo "re        - Rebuild the library."
		@echo "help      - Display this help."

.PHONY:	all clean fclean re help