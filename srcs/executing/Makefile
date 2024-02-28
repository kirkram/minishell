# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/28 14:45:41 by klukiano          #+#    #+#              #
#    Updated: 2024/02/28 14:50:16 by klukiano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test_exec
LIBFT_PATH = ../../lib/libft-kir
LIBFT = $(LIBFT_PATH)/libft.a
CFLAGS = -Wall -Wextra -Werror
HEADERS	= -I ../include -I ../include/kirtemp
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
SRCS =./exec_main.c ./ppx_split/pipex_split.c ./ppx_split/pipex_split_helper.c
OBJCTS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(SRCS) $(PRINTF_SRCS)
	cc $(CFLAGS) $(HEADERS) $(SRCS) $(PRINTF_SRCS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

debug: .debug

.debug: $(LIBFT) $(SRCS) $(PRINTF_SRCS) 
	cc $(DEBUGFLAGS) $(HEADERS) $(SRCS)  $(PRINTF_SRCS) $(LIBFT)   -o debug.out
	touch .debug

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ $(HEADERS)

# inout:
# 	$(RM) ./testfilesshell/infile 
# 	$(RM) ./testfilesshell/outfile
# 	touch ./testfilesshell/infile
# 	touch ./testfilesshell/outfile

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) debug.out
	$(RM) .debug
	$(RM) .bonus
	$(RM) -r debug.out.dSYM


re: fclean all

.PHONY: all clean fclean re bonus both debug