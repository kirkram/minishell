/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:29:34 by clundber          #+#    #+#             */
/*   Updated: 2024/02/29 13:04:38 by clundber         ###   ########.fr       */
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

void		error_func(char *str);
void		var_substitution(char **array, char *envp[]);
char		*env_variable(char *str, char **envp);
char		**array_copy(char **array);
// lst functions

t_bigcmd	*ms_lstlast(t_bigcmd *lst);
t_bigcmd	*ms_lstnew(char **cmds);
int			ms_lstadd_back(t_bigcmd **lst, t_bigcmd *new);
int			ms_lstsize(t_bigcmd **lst);
void		lst_clear(t_bigcmd **stack);
# define CMD 1
# define PIPE 2
# define IN_FD 3
# define IN_HD 4
# define RED 5
# define RED_AP 6
# define BUILT_IN 7
#endif