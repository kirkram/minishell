# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/26 11:59:53 by clundber          #+#    #+#              #
#    Updated: 2024/02/26 16:52:04 by clundber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_BLUE=\033[0;34m
COLOUR_END=\033[0m

# ------------ PROJECT -------#
NAME = libft.a

# ------------ DIRECTORIES ---#
SRC_DIR = srcs
LIBFT_DIR = srcs/libft
FT_PRINTF_DIR = srcs/ft_printf
GNL_DIR = srcs/get_next_line
OBJ_DIR = temp

#------------- SOURCE FILES ------#

SRC_FLS = $(LIBFT_FLS) $(FT_PRINTF_FLS) $(GNL_FLS)

LIBFT_FLS = $(LIBFT_DIR)/ft_atoi.c $(LIBFT_DIR)/ft_strlen.c $(LIBFT_DIR)/ft_bzero.c $(LIBFT_DIR)/ft_calloc.c $(LIBFT_DIR)/ft_isalnum.c \
		$(LIBFT_DIR)/ft_isalpha.c $(LIBFT_DIR)/ft_isascii.c $(LIBFT_DIR)/ft_isdigit.c $(LIBFT_DIR)/ft_isprint.c $(LIBFT_DIR)/ft_memchr.c \
		$(LIBFT_DIR)/ft_memcmp.c $(LIBFT_DIR)/ft_memcpy.c $(LIBFT_DIR)/ft_memmove.c $(LIBFT_DIR)/ft_memset.c $(LIBFT_DIR)/ft_putchar_fd.c \
		$(LIBFT_DIR)/ft_putendl_fd.c $(LIBFT_DIR)/ft_putnbr_fd.c $(LIBFT_DIR)/ft_putstr_fd.c $(LIBFT_DIR)/ft_strchr.c $(LIBFT_DIR)/ft_strdup.c \
		$(LIBFT_DIR)/ft_strjoin.c $(LIBFT_DIR)/ft_strlcat.c $(LIBFT_DIR)/ft_strlcpy.c $(LIBFT_DIR)/ft_strncmp.c \
		$(LIBFT_DIR)/ft_strnstr.c $(LIBFT_DIR)/ft_strrchr.c $(LIBFT_DIR)/ft_strtrim.c $(LIBFT_DIR)/ft_substr.c $(LIBFT_DIR)/ft_tolower.c \
		$(LIBFT_DIR)/ft_toupper.c $(LIBFT_DIR)/ft_itoa.c $(LIBFT_DIR)/ft_strmapi.c $(LIBFT_DIR)/ft_split.c $(LIBFT_DIR)/ft_striteri.c \
		$(LIBFT_DIR)/ft_lstnew_bonus.c $(LIBFT_DIR)/ft_lstadd_front_bonus.c $(LIBFT_DIR)/ft_lstsize_bonus.c $(LIBFT_DIR)/ft_arrfree.c \
		$(LIBFT_DIR)/ft_lstlast_bonus.c $(LIBFT_DIR)/ft_lstadd_back_bonus.c $(LIBFT_DIR)/ft_lstdelone_bonus.c \
		$(LIBFT_DIR)/ft_lstclear_bonus.c $(LIBFT_DIR)/ft_lstiter_bonus.c $(LIBFT_DIR)/ft_lstmap_bonus.c $(LIBFT_DIR)/ft_trijoin.c

FT_PRINTF_FLS = $(FT_PRINTF_DIR)/ft_printf.c $(FT_PRINTF_DIR)/ft_printhex.c $(FT_PRINTF_DIR)/ft_putnbr_return.c \
		$(FT_PRINTF_DIR)/ft_putstr_return.c $(FT_PRINTF_DIR)/ft_putnbr_u_return.c $(FT_PRINTF_DIR)/ft_putchar_return.c

GNL_FLS = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c

OFILES = $(SRC_FLS:.c=.o)

#--------- FLAGS ----------#

CC = @cc
CFLAGS = -Wall -Wextra -Werror -I include

all: $(NAME)

$(NAME): $(OFILES)
	@echo "$(COLOUR_BLUE)compiling libft$(COLOUR_END)"
	@ar rcs $(NAME) $(OFILES)
	@mkdir -p $(OBJ_DIR)
	@mv $(OFILES) $(OBJ_DIR)	
	@echo "$(COLOUR_GREEN)libft compiled successfully$(COLOUR_END)"

clean:
	@echo "$(COLOUR_GREEN)cleaning libft$(COLOUR_END)"
	@rm -r -f $(OBJ_DIR) 
	 

fclean: clean
	@rm -f $(NAME) 
	
re: fclean $(NAME) 

.PHONY: all clean fclean re