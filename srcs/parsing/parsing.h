/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:29:34 by clundber          #+#    #+#             */
/*   Updated: 2024/03/01 13:56:40 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../../libft/include/libft.h"

// PIPEX SPLITTER

int			skip_chars(int *x, char *str, char stop_char, char c);
char		**return_and_nullterm(char ***array, int i);
void		skip_esc_and_c(char stop_char, int *x, char *str, char c);
char		**free_reverse(int i, char **array);
char		**ppx_split(char const *str, char c);
char		**splitter(char *str, char c, char **array, int i);
int			splitlen(char *str, char c);
int			countstrings(char *str, char c);

// LEXER

void		lexer(char *argv, char **envp);
void		error_func(char *str);
void		var_substitution(char **array, char *envp[]);
char		*env_variable(char *str, char **envp);
char		**array_copy(char **array);

// PARSER

char		**get_cmd(char **cmds, int start, int end);

# define CMD 1
# define PIPE 2
# define IN_FD 3
# define IN_HD 4
# define OUT 5
# define OUT_AP 6
# define SKIP_IN 7
# define BUILTIN 8
#endif