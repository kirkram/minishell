/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:29:34 by clundber          #+#    #+#             */
/*   Updated: 2024/04/13 15:55:12 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "./minishell.h"
# include "./readline/readline.h"
# include "./readline/history.h"

# include <signal.h>
# include <dirent.h>

extern int	g_signal;


typedef struct s_paths
{
	int		i;
	char	*path;
	char	pwd[4096];
	char	*bigpath;
	char	**paths;
	bool	skip_pwd;
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
	bool	was_prev_line_null;
	char	**envp;
	char	**export;
	int		err_code;
}	t_utils;

typedef struct s_ms
{
	t_pipe	**pipe;
	t_utils	*utils;
	char	*line;
	char	*temp;
	char	*temp2;
}	t_ms;

typedef struct s_exec
{
	int		fd[2];
	int		savestdio[2];
	pid_t	pid[256];
	int		pipefd[2];
	int		tempfd_0;
	int		num_of_pipes;
	int		fd_failed;
	int		i;
}	t_exec;

// SIGNAL
void	signal_handler(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

// INIT
void	intialize_utils(char **sys_envp, t_utils **utils, t_ms *ms);
int		interactive_mode_loop(char **envp);
char	*rl_gets(char *line_read, t_utils *utils);
void	copy_utils_change_shellvars(char **sys_envp, t_utils **ut, t_ms *ms);
char	*init_exp(char *str1, char *str2, t_ms *ms);
char	*shell_level(char *str, t_ms *ms);

// LEXER
int		check_quote(char **str, bool quote, bool dquote, int *err_code);
int		lexer(t_ms *ms);
void	env_variable(t_ms *ms, bool quote, bool dquote, char **str);
char	**array_copy(char **array);
char	*get_variable(t_ms *ms);
int		*tokenizer(char **array);
int		get_token(char *str);

char	*separator(t_ms *ms, bool quote, bool dquote, int i);
void	remove_space(int i, t_ms *ms);
void	init_token(t_pipe *pipe, t_ms *ms);
void	remove_in(t_pipe *pipe, int i, int hd);
void	remove_out(t_pipe *pipe, int i, int app);
void	make_tokens(t_pipe *pipe, int i);
int		env_error(char **temp, t_ms *ms);
void	combine_str(char **new_str, char *temp);

void	pipeline_init(char **array, t_pipe ***pipe, t_ms *ms);
void	init_tokenarr(int **tokens, char **array, t_ms *ms);
void	quote_remover(t_pipe *pipe, t_ms *ms);

// PARSER
int		parsing(t_ms *ms);
char	**get_cmd(char **cmds, int start, int end, t_ms *ms);
void	pre_parse(char **array, t_pipe ***pipe, t_ms *ms);
void	parser(char **array, t_pipe ***pipe, t_ms *ms);
int		syntax_check(int *tokens, int *err_code, char **array);
void	here_doc_open(char *eof, t_pipe *_pipe, t_utils *utils, t_ms *ms);
void	here_doc(t_pipe ***pipe, t_utils *utils, t_ms *ms);
void	final_args(t_pipe *pipe, int i, t_ms *ms);
void	remove_red(t_pipe *pipe, int i, t_ms *ms);

// LEXER/PARSER UTILS
int			is_builtin(char *str);
int			syntax_err(char **array, int *err_code, int i);
void		malloc_error(int err); // DELETE ONCE NOT IN USE AT EXECUTING
void		quote_status2(bool *quote, bool *dquote, char c);
void		quote_status(bool *quote);
int			not_ms(char **array, int *err_code, int i);
int			pipe_error(int *tokens, int *err_code, int i);

//EXECUTE
char	*assign_scmd_path(char *scmd, char **envp, t_ms *ms);
int		execute(t_utils *utils, t_pipe **_pipe, t_ms *ms);
char	*jointhree(char const *s1, char const *s2, char const *s3);
int		handle_execve_errors(char *failed_cmd);
int		msg_stderr(char *message, char *cmd, int err_code);
char	**find_path_and_pwd(char **envp, char *scmd, t_ms *ms);
int		user_cmd_path(char **args, char *arg_cmd, char **paths);
void	delete_pwd_path(char **paths);
int		free_and_1(char **paths, int **end);
int		exec_assign_redirections(t_pipe *_pipe_i, int (*fd)[2]);
int		exec_redir_in(t_pipe *_pipe_i, int (*fd)[2], int j, int *fd_failed);
int		exec_redir_out(t_pipe *_pipe_i, int (*fd)[2], int j, int *fd_failed);
void	free_pipes_utils_and_exit(t_pipe ***_pipe, t_utils **utils, int child_exit_code);
int		waitpid_and_close_exec(t_ms *ms, t_exec *xx);
void	exec_child_system_function(t_pipe **_pipe, t_utils *utils, int i, t_exec *xx);
void	exec_child_builtin_function(t_ms *ms, int i, t_exec *xx);
void	exec_builtin_no_pipes(t_ms *ms, int i, t_exec *xx);
void	exec_fd_fail_pass_pipe(t_pipe **_pipe, int i, t_exec *xx);
void	dup_and_close_child_process(int i, t_exec *xx);
void	pipe_readend_and_close_parent(int i, t_pipe **_pipe, t_exec *xx);

// BUILTINS
int		exec_builtin(t_pipe **_pipe, t_utils *utils, int i, t_ms *ms);

// export
int		export(t_utils *utils, char **arg, t_ms *ms);
void	print_exp(t_utils *utils, int fd);
void	sort_export(t_utils *utils);
//static int	print_exp_error(char *arg);
//static int	export_error(char *arg);
//static void	export_loop2(char *arg, t_ms *ms, int *i);
//static void export_loop(char *arg, t_ms *ms, bool quote, bool dquote);
int		add_exp_var(t_utils **utils, char *newstr, t_ms *ms);

// unset
int		remove_env(t_ms *ms, int i, int x, int y);
int		remove_exp(t_ms *ms, int i, int x, int y);
int		unset(t_utils *utils, char **arg, t_ms *ms);
void	unset_exp(t_ms *ms, char **arg, int j, int i);
void	unset_env(t_utils *utils, char **arg, t_ms *ms);

// env
int		env(t_utils *utils, char **noio_arg);
int		change_var(char ***array, char *env_name, char *newstr, t_ms *ms);

// pwd
int		pwd(t_utils *utils);
int		update_pwd_oldpwd_env_exp(t_utils *utils, t_ms *ms, char cwd[4096]);
int		update_pwd_oldpwd_env(t_utils *utils, char *cwd, t_ms *ms);

// cd
int		cd_builtin(t_pipe **_pipe, t_utils *utils, int index, t_ms *ms);
int		cd_home_chdir_fail(char *home_path, t_utils *utils);
int		cd_chdir_fail(t_pipe **_pipe, int index);
char	*find_home_env(t_utils *utils);

// exit
int		exit_builtin(t_pipe **_pipe, t_utils *utils, int i);

// echo
int		echo_builtin(char **noio_args, t_utils *utils);

//MS SPLIT

int		str_count(char *str);
int		ms_splitter(char *str, char ***array, bool quote, bool dquote);
char	**ms_split(char *str, t_ms *ms);
char	**free_reverse(int i, char **array);
char	*remove_quote(char *str, int i, int x, t_ms *ms);
int		quote_count(char *str);

//HELPER
void	ft_nullfree(char **str);
int		is_only_digits_and_signs(char *str);
char	*ft_free_strjoin(char *s1, char *s2);

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

# define YEL "\e[0;33m"
# define CRESET "\e[0m"

#endif
