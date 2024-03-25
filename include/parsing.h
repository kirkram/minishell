/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:29:34 by clundber          #+#    #+#             */
/*   Updated: 2024/03/25 15:48:46 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "./minishell.h"
# include <signal.h>
# include <dirent.h>
# include "readline/history.h"
# include "readline/rlstdc.h"
# include "readline/readline.h"

//# include </usr/include/readline/readline.h> //needed for Linux
//# include </usr/include/readline/history.h> // needed for Linux
typedef struct s_paths
{
	int		i;
	char	*path;
	char	*pwd;
	char	*bigpath;
	char	**paths;
	bool	should_skip_pwd;
}	t_paths;

typedef struct s_pipe
{
	char	**noio_args;
	char	**args;
	char	*cmd_with_path;
	int		*tokens;
	char	*infile;
	int		hd_fd[2];
}	t_pipe;

typedef struct s_utils
{
	bool	syntax_err;
	int		err_code;
	char	**envp;
	char	**export;
}	t_utils;

// INIT

char	*rl_gets(char *line_read, int hist_file);
int		rl_loop(int ac, char **av, char **sys_envp);
int		open_history_file(int hist_fd);
int		interactive_mode_loop(int hist_fd, char **envp);
void	intialize_utils(char **sys_envp, t_utils **utils);

int			parsing(char **line_read, t_pipe ***pipe, t_utils *utils);
// LEXER

int			check_quote(char **str, bool quote, bool dquote, int *err_code);
//int			lexer(char **array, char *envp[], int *err_code);
int			lexer(char **str, t_utils *utils);
void		env_variable(char **str, t_utils *utils, bool quote, bool dquote);
//char		*env_variable(char *str, char **envp, int err_code, bool quote, bool dquote);
char		**array_copy(char **array);
char		*get_variable(char *temp, char **envp, int err_code);
int			*tokenizer(char **array);
int			get_token(char *str);

char		*separator(char *str, bool quote, bool dquote);
void		remove_space(char **str, int i);

// PARSER

char		**get_cmd(char **cmds, int start, int end);
void		pre_parse(char **array, t_pipe ***pipe);
int			parser(char **array, t_pipe ***pipe, int *err_code);
int			syntax_check(int *tokens, int *err_code, char **array);
void		here_doc_open(char *eof, t_pipe *_pipe);
void		here_doc(t_pipe ***pipe);
// LEXER/PARSER UTILS

int			is_builtin(char *str);
int			syntax_err(char **array, int *err_code, int i);
void		malloc_error(int err);
void		quote_status2(bool *quote, bool *dquote, char c);
void		quote_status(bool *quote);
//EXECUTE

char	*assign_scmd_path(char *scmd, char **envp);
int		execute(t_utils *utils, t_pipe **_pipe);
char	*jointhree(char const *s1, char const *s2, char const *s3);
int		handle_execve_errors(char *failed_cmd);
int		msg_stderr(char *message, char *cmd, int err_code);
char	**find_path_and_pwd(char **envp, char *scmd);
int		user_cmd_path(char **args, char *arg_cmd, char **paths);
void	delete_pwd_path(char **paths);
int		free_and_1(char **paths, int **end);

// BUILTINS

int			exec_builtin(t_pipe **_pipe, t_utils *utils, int i);
int			change_env_var(t_utils **utils, char *env_name, char *newstr);
int			echo_builtin(char **noio_args, t_utils *utils);
int			add_exp_var(t_utils **utils, char *newstr);
int			change_exp_var(t_utils **utils, char *env_name, char *newstr);
void		print_exp(t_utils *utils, int fd);
void		sort_export(t_utils *utils);
int			export(t_utils *utils, char **arg);
int			env(t_utils *utils);
int			pwd(t_utils *utils);
int			remove_env(t_utils *utils, int i);
int			remove_exp(t_utils *utils, int i);
int			unset(t_utils *utils, char **arg);
int			cd_builtin(t_pipe **_pipe, t_utils *utils, int i);
int			exit_builtin(t_pipe **_pipe, t_utils *utils, int i);
int			update_pwd_oldpwd_env(t_utils *utils, char *cwd);

//MS SPLIT

int		str_count(char *str);
char	**ms_splitter(char *str, char **array, bool quote, bool dquote);
char	**ms_split(char *str);
char	**free_reverse(int i, char **array);
char	*remove_quote(char *str);
int		quote_count(char *str);

//HELPER
int			is_only_digits_and_signs(char *str);


# define CMD 1 // 1st CMD is the acctual CMD, others are flags / arguments
# define PIPE 2
# define IN_FD 3 // Should be the FD to read from
# define IN_HD 4 // Should bee the FD to read from using Here_doc
# define OUT 5 // Output should go here
# define OUT_AP 6 // output should go here and append
# define SKIP_IN 7 // should be read and error given if unable to / but info not passed on
# define BUILTIN 8 // builtin functions made by us
# define REMOVE 9
# define SKIP_OUT 10 // should be created, but no information written to it
# define SKIP_HD 11 // should activate Here_doc, but information should not be passed on
# define NOT_MS 12 // operator that is not implemented in minishell

#endif
