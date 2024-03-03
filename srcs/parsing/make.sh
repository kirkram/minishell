#!/usr/bin/bash
cc parse.c pipex_split.c pipex_split_helper.c parse_main.c preparse.c ../../libft/libft.a
#cc -g -fsanitize=address,undefined,integer parse.c pipex_split.c pipex_split_helper.c parse_main.c preparse.c ../../libft/libft.a