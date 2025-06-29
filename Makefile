NAME = minishell
SOURCES = src/main.c \
		  src/builtins/ft_cd.c \
		  src/builtins/ft_echo.c \
		  src/builtins/ft_exit.c \
		  src/builtins/ft_pwd.c \
		  src/builtins/builtin.c \
		  src/utils/ft_atoi.c \
		  src/utils/ft_isalnum.c \
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
		  src/utils/ft_split.c \
		  src/environment/env_handle.c \
		  src/utils/env_strcmp.c \
		  src/parser/tokenizer.c \
		  src/utils/debug_menu.c \
		  src/parser/parser.c \
		  src/utils/ft_itoa.c \
		  src/utils/create_filename.c \
		  src/utils/ft_substr.c \
		  src/parser/tokenizer_utils.c \
		  src/utils/cleaner.c \
		  src/parser/parser_utils.c \
		  src/utils/ft_strjoin.c \
		  src/utils/error_messages.c \
		  src/execute/signal_handler.c \
		  src/utils/ft_strchr.c \
		  src/parser/validation.c \
		  src/builtins/ft_export_help.c \
		  src/utils/ft_memcpy.c \
		  src/utils/ft_strncmp.c \
		  src/utils/ft_strstr.c \
		  src/utils/looking_path.c \
		  src/utils/cleaner_utils.c \
		  src/parser/parser_len_counter.c \
		  src/builtins/cd_utils.c

CC = cc
CFLAGS = -Werror -Wextra -Wall -O2 -g
OBJS = $(SOURCES:.c=.o)

GREEN = \033[1;32m
RED = \033[1;31m
BLUE = \033[1;34m
CYAN = \033[1;36m
RESET = \033[0m
CHECK = ✓
CROSS = ✗

all: banner $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline
	@printf "$(GREEN)[$(CHECK)] Build successful: $(NAME)$(RESET)\n"
	@$(MAKE) --no-print-directory minihell

banner:
	@printf "$(CYAN)🚀 Building $(NAME)...$(RESET)\n"

%.o: %.c
	@printf "$(BLUE)[*] Compiling $<...$(RESET)\n"
	@$(CC) $(CFLAGS) -c $< -o $@

minihell:
	@echo "$(RED)"
	@echo "███╗   ███╗██╗███╗   ██╗██╗██╗  ██╗███████╗██╗     ██╗     "
	@echo "████╗ ████║██║████╗  ██║██║██║  ██║██╔════╝██║     ██║     "
	@echo "██╔████╔██║██║██╔██╗ ██║██║███████║█████╗  ██║     ██║     "
	@echo "██║╚██╔╝██║██║██║╚██╗██║██║██╔══██║██╔══╝  ██║     ██║     "
	@echo "██║ ╚═╝ ██║██║██║ ╚████║██║██║  ██║███████╗███████╗███████╗"
	@echo "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "                   $(BLUE)by $(RED)ishchyro $(BLUE)& $(RED)aorth $(CYAN)"
	@echo "$(RESET)"

clean:
	@rm -rf $(OBJS)
	@clear
	@printf "$(RED)[$(CROSS)] Object files removed.$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(RED)[$(CROSS)] Executable removed.$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re banner
