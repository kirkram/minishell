/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:29:34 by clundber          #+#    #+#             */
/*   Updated: 2024/03/03 18:33:20 by clundber         ###   ########.fr       */
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
char	    *get_variable(char *temp, char **envp, char *new_str);
int	        *tokenizer(char **array);
int	        get_token(char *str);
// PARSER

char		**get_cmd(char **cmds, int start, int end);
void	    pre_parse(char **array, t_pipe ***pipe);
void	    parser(char **array, t_pipe ***pipe);

# define CMD 1 // 1st CMD is the acctual CMD, others are flags / arguments
# define PIPE 2
# define IN_FD 3 // Should be the FD to read from
# define IN_HD 4 // Should bee the FD to read from using Here_doc
# define OUT 5 // Output should go here
# define OUT_AP 6 // output should go here and append
# define SKIP_IN 7 // should be read and error given if unable to / but info not passed on
# define BUILTIN 8 // not in use at the moment
# define REMOVE 9 
# define SKIP_OUT 10 // should be created, but no information written to it
# define SKIP_HD 11 // should activate Here_doc, but information should not be passed on

#endif