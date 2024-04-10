/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:29:34 by clundber          #+#    #+#             */
/*   Updated: 2024/04/09 00:07:07 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "./minishell.h"
# include "./readline/readline.h"
# include "./readline/history.h"
//# include <readline/readline.h>
//# include <readline/history.h>
# include <signal.h>
# include <dirent.h>

extern int	g_signal;

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
	int		hd_fd[2];
}	t_pipe;

typedef struct s_utils
{
	bool	syntax_err;
	int		err_code;
	bool	was_prev_line_null;
	char	**envp;
	char	**export;
}	t_utils;

typedef struct s_ms
{
	t_pipe	**pipe;
	t_utils	*utils;
	char	*line;
	char	*temp;
	char	*temp2;
}	t_ms;

// SIGNAL
void	signal_handler(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
// INIT

int		interactive_mode_loop(char **envp);
char	*rl_gets(char *line_read, t_utils *utils);
void	intialize_utils(char **sys_envp, t_utils **utils);

// LEXER

void	lex_merror(t_utils *utils, char **str);
int		check_quote(char **str, bool quote, bool dquote, int *err_code);
//int		lexer(char **str, t_utils *utils);
int		lexer(t_ms *ms);
void	env_variable(char **str, t_utils *utils, bool quote, bool dquote);
char	**array_copy(char **array);
char	*get_variable(char *temp, char **envp, int err_code);
int		*tokenizer(char **array);
int		get_token(char *str);

char	*separator(char *str, bool quote, bool dquote, int i);
void	remove_space(char **str, int i, t_utils *utils);

void	init_token(t_pipe *pipe);
void	remove_in(t_pipe *pipe, int i, int hd);
void	remove_out(t_pipe *pipe, int i, int app);
int		make_tokens(t_pipe *pipe, int i);
int		env_error(char **temp);
void	combine_str(char **new_str, char *temp);

void	pipeline_init(char **array, t_pipe ***pipe);
void	init_tokenarr(int **tokens, char **array);
void	quote_remover(t_pipe *pipe);
// PARSER

//int		parsing(char **line_read, t_pipe ***pipe, t_utils *utils);
int		parsing(t_ms *ms);
char	**get_cmd(char **cmds, int start, int end);
void	pre_parse(char **array, t_pipe ***pipe);
int		parser(char **array, t_pipe ***pipe, int *err_code);
int		syntax_check(int *tokens, int *err_code, char **array);
void	here_doc_open(char *eof, t_pipe *_pipe, t_utils *utils);
void	here_doc(t_pipe ***pipe, t_utils *utils);
int		final_args(t_pipe *pipe, int i);
void	remove_red(t_pipe *pipe, int i);

// LEXER/PARSER UTILS
int			is_builtin(char *str);
int			syntax_err(char **array, int *err_code, int i);
void		malloc_error(int err);
void		quote_status2(bool *quote, bool *dquote, char c);
void		quote_status(bool *quote);
int			not_ms(char **array, int *err_code, int i);
int			pipe_error(int *tokens, int *err_code, int i);

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
int		exec_assign_redirections(t_pipe *_pipe_i, int (*fd)[2], char **infile, char **outfile);
void	free_pipes_utils_and_exit(t_pipe ***_pipe, t_utils **utils, int child_exit_code);
int		waitpid_and_close_exec(t_pipe **_pipe, pid_t (*pid)[256], int savestdio[2], t_utils *utils, int has_fd_failed);

// BUILTINS
int		exec_builtin(t_pipe **_pipe, t_utils *utils, int i);
int		change_env_var(t_utils **utils, char *env_name, char *newstr);
int		echo_builtin(char **noio_args, t_utils *utils);
int		add_exp_var(t_utils **utils, char *newstr);
int		change_exp_var(t_utils **utils, char *env_name, char *newstr);
void	print_exp(t_utils *utils, int fd);
void	sort_export(t_utils *utils);
int		export(t_utils *utils, char **arg);
int		env(t_utils *utils);
int		pwd(t_utils *utils);
int		remove_env(t_utils *utils, int i, int x, int y);
int		remove_exp(t_utils *utils, int i, int x, int y);
int		unset(t_utils *utils, char **arg);
int		cd_builtin(t_pipe **_pipe, t_utils *utils, int i);
int		exit_builtin(t_pipe **_pipe, t_utils *utils, int i);

int		update_pwd_oldpwd_env_exp(t_utils *utils, char *cwd);
int		update_pwd_oldpwd_env(t_utils *utils, char *cwd);
int		export_error(char *arg);
void	export_loop(char *arg, t_utils *utils, bool quote, bool dquote);
void	unset_exp(t_utils *utils, char **arg, int j, int i);
void	unset_env(t_utils *utils, char **arg);
//MS SPLIT

int		str_count(char *str);
char	**ms_splitter(char *str, char **array, bool quote, bool dquote);
char	**ms_split(char *str);
char	**free_reverse(int i, char **array);
char	*remove_quote(char *str, int i);
int		quote_count(char *str);

//HELPER
void		ft_nullfree(char **str);
void		malloc_error2(char **str);
int			is_only_digits_and_signs(char *str);
char		*ft_free_strjoin(char *s1, char *s2);

// UTILS
void	malloc_check(char **str, t_ms *ms);

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
# define NOT_DEF 13 // undefined token

#endif
