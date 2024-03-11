/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:29:34 by clundber          #+#    #+#             */
/*   Updated: 2024/03/11 15:03:46 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "./minishell.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

//# include </usr/include/readline/readline.h> //needed for Linux
//# include </usr/include/readline/history.h> // needed for Linux
typedef struct s_paths
{
	int		i;
	char	*path;
	char	*pwd;
	char	*bigpath;
	char	**paths;

}	t_paths;

typedef struct s_pipe
{
	char	**noio_args;
	char	**args;
	char	*cmd_with_path;
	int		*tokens;
	char	*infile;
}	t_pipe;

typedef struct s_utils
{
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


// LEXER

int			lexer(char *argv, t_pipe ***pipe, t_utils *utils);
void		error_func(char *str);
void		var_substitution(char **array, char *envp[]);
char		*env_variable(char *str, char **envp);
char		**array_copy(char **array);
char		*get_variable(char *temp, char **envp);
int			*tokenizer(char **array);
int			get_token(char *str);

// PARSER

char		**get_cmd(char **cmds, int start, int end);
void		pre_parse(char **array, t_pipe ***pipe);
int			parser(char **array, t_pipe ***pipe, int *err_code);

//EXECUTE

char	*find_scmd_path(char *scmd, char **envp);
int		execute(t_utils *utils, t_pipe **_pipe);
char	*jointhree(char const *s1, char const *s2, char const *s3);
int		handle_execve_errors(char *failed_cmd);
int		msg_stderr(char *message, char *cmd, int err_code);
char	**find_path(char **envp);
int		user_cmd_path(char **args, char *arg_cmd, char **paths);
void	delete_pwd_path(char **paths);
int		free_and_1(char **paths, int **end);

// BUILTINS

int			exec_builtin(t_pipe **_pipe, t_utils *utils, int i);
int			change_env_var(t_utils **utils, char *env_name, char *newstr);
int			echo_builtin(char **noio_args);
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

//PPX SPLIT
int			skip_chars(int *x, char *str, char stop_char, char c);
char		**return_and_nullterm(char ***array, int i);
void		skip_esc_and_c(char stop_char, int *x, char *str, char c);
char		**free_reverse(int i, char **array);
char		**ppx_split(char const *str, char c);
char		**splitter(char *str, char c, char **array, int i);
int			splitlen(char *str, char c);
int			countstrings(char *str, char c);
void		quote_status(bool *quote);

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

#endif
