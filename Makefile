NAME = minishell
LIBFT_PATH = ./lib/libft
LIBFT = $(LIBFT_PATH)/libft.a
CFLAGS = -Wall -Wextra -Werror
HEADERS	= -I ./include -L/usr/local/lib -L${HOME}/.brew/opt/readline/lib -I/usr/local/include #-lreadline
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
SRCS = \
 ./srcs/parsing/lexer.c ./srcs/parsing/lexer2.c ./srcs/parsing/parse.c \
 ./srcs/parsing/parsing.c ./srcs/parsing/ms_split.c ./srcs/parsing/parsing_utils.c \
 ./srcs/parsing/here_doc.c ./srcs/parsing/syntax_check.c ./srcs/parsing/parse_env.c \
 ./srcs/parsing/tokens.c ./srcs/parsing/parsing_init.c \
 ./srcs/initializing/init_main.c \
 ./srcs/signal/signal.c \
 ./srcs/executing/exec_main.c \
 ./lib/get_next_line/get_next_line.c ./lib/get_next_line/get_next_line_utils.c \
 ./srcs/builtins/env.c ./srcs/builtins/builtin.c ./srcs/builtins/export.c \
 ./srcs/builtins/export_utils.c ./srcs/builtins/unset.c \
 ./srcs/utils/malloc_error.c
OBJCTS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(SRCS)
	cc $(CFLAGS) $(HEADERS) $(SRCS)  $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

debug: .debug

.debug: $(LIBFT) $(SRCS) 
	cc $(DEBUGFLAGS) $(HEADERS) -lreadline $(SRCS) $(LIBFT) -o minishell
	touch .debug

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) .debug
	$(RM) -r minishell.dSYM


re: fclean all

.PHONY: all clean fclean re bonus both debug