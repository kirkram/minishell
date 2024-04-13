/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_findpath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:00:28 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/12 18:42:14 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//If there is absolute path (scmd starts with '/')
//then check only once then break
//otherwise look through env_paths and try to access
extern int	g_signal;

char	*assign_scmd_path(char *scmd, char **envp, t_ms *ms)
{
	char	**env_paths;
	char	*cmd_path;
	int		i;

	if (!scmd || scmd[0] == '\0')
		return (NULL);
	env_paths = find_path_and_pwd(envp, scmd, ms);
	if (!env_paths)
		return (NULL);
	i = -1;
	while (env_paths[++i])
	{
		if (scmd[0] == '/')
			cmd_path = ft_strdup(scmd);
		else
			cmd_path = jointhree(env_paths[i], "/", scmd);
		if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0 && \
		free_and_1(env_paths, NULL))
			return (cmd_path);
		free (cmd_path);
		if (scmd[0] == '/')
			break ;
	}
	free_and_1(env_paths, NULL);
	return (NULL);
}

//if there is a './' then skip adding pwd to the path
char	**find_path_and_pwd(char **envp, char *scmd, t_ms *ms)
{
	t_paths	vars;

	ft_bzero(&vars, sizeof(t_paths));
	if (!ft_strnstr(scmd, "./", -1))
		vars.skip_pwd = true;
	while (envp[++vars.i])
	{
		if (ft_strncmp(envp[vars.i], "PATH=", 5) == 0)
			vars.path = envp[vars.i] + 5;
	}
	getcwd(vars.pwd, 4096);
	vars.bigpath = jointhree(vars.path, ":", vars.pwd);
	if (vars.path)
		malloc_check(&vars.bigpath, ms);
	if (vars.bigpath && !vars.skip_pwd)
		vars.paths = ft_split(vars.bigpath, ':');
	else if (vars.path)
		vars.paths = ft_split(vars.path, ':');
	else if (!vars.skip_pwd)
		vars.paths = ft_split(vars.pwd, ':');
	free (vars.bigpath);
	if (((vars.bigpath && !vars.skip_pwd) || (!vars.bigpath && vars.path) || \
	(!vars.path && !vars.skip_pwd)) && !vars.paths)
		malloc_check(vars.paths, ms);
	return (vars.paths);
}
