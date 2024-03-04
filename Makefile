NAME = m_shell
LIBFT_PATH = ./lib/libft/
LIBFT = $(LIBFT_PATH)/libft.a
CFLAGS = -Wall -Wextra -Werror
HEADERS	= -I ./include -L/usr/local/lib -I/usr/local/include -lreadline
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
SRCS = \
 ./srcs/parsing/lexer.c ./srcs/parsing/parse_main.c ./srcs/parsing/parse.c ./srcs/parsing/pipex_split_helper.c \
 ./srcs/parsing/pipex_split.c ./srcs/parsing/preparse.c \
 ./srcs/initializing/init_main.c \
 ./srcs/executing/exec_main.c \
 ./lib/get_next_line/get_next_line.c ./lib/get_next_line/get_next_line_utils.c
OBJCTS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(SRCS)
	cc $(CFLAGS) $(HEADERS) $(SRCS)  $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

debug: .debug

.debug: $(LIBFT) $(SRCS) 
	cc $(DEBUGFLAGS) $(HEADERS) $(SRCS) $(LIBFT) -o debug.out
	touch .debug

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) debug.out
	$(RM) .debug
	$(RM) -r debug.out.dSYM


re: fclean all

.PHONY: all clean fclean re bonus both debug