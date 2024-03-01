#!/usr/bin/bash
cc -g -fsanitize=address,undefined,integer parse.c pipex_split.c pipex_split_helper.c parse_main.c ../../libft/libft.a