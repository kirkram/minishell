/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:38:21 by clundber          #+#    #+#             */
/*   Updated: 2024/03/11 11:23:17 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_exp(t_utils *utils, int fd)

{
	int	i;

	i = 0;
	if (!utils->export)
		return ;
	while (utils->export[i])
	{
		ft_putendl_fd(utils->export[i], fd);
		i++;
	}
}

void	sort_export(t_utils *utils)

{
	int		i;
	char	*temp;
	bool	sorted;

	sorted = false;
	while (sorted == false)
	{
		sorted = true;
		i = 0;
		while (utils->export[i])
		{
			if (utils->export[i +1] && ft_strncmp (utils->export[i], utils->export[i +1], -1) > 0)
			{
				sorted = false;
				temp = utils->export[i];
				utils->export[i] = utils->export[i +1];
				utils->export[i +1] = temp;
			}
			i++;
		}
	}
}



int	export(t_utils *utils, char **arg)

{
	int		i;
	int		x;
	bool	quote;
	bool	dquote;
	char	*temp;


	i = 1;
	if (!utils)
		return (1);
	if (!arg || !arg[0])
		return (0);
	if (!arg[1])
	{
		print_exp(utils, 1);
		return (0);
	}
	while (arg[0] && arg[i])
	{
		quote = false;
		dquote = false;
		x = 0;
		while (arg[i][x])
		{
			if (arg[i][x] == '\'')
				quote_status(&quote);
			else if (arg[i][x] == '\"')
				quote_status(&dquote);
			if (arg[i][x] == '=' && quote == false && dquote == false)
			{
				temp = jointhree("declare -x ", ft_substr(arg[i], 0, (x +1)), "\"");
				temp = jointhree(temp, ft_substr(arg[i], (x +1), (ft_strlen(arg[i]) - x)), "\"");
				change_env_var(&utils, ft_substr(arg[i], 0, (x +1)), arg[i]);
				change_exp_var(&utils, ft_strjoin("declare -x ", ft_substr(arg[i], 0, (x +1))), temp);
				break ;
			}
			x++;
			if (arg[i][x] == '\0')
				add_exp_var(&utils,ft_strjoin("declare -x ", arg[i]));
		}
		i++;
	}
	sort_export(utils);
	return (0);
}

int	env(t_utils *utils)

{
	int	i;

	i = 0;
	if (!utils->envp)
		return (0);
	while (utils->envp[i])
	{
		ft_putendl_fd(utils->envp[i], 1);
		i++;
	}
	return (0);
}

int	pwd(t_utils *utils)

{
	int		i;
	char	*temp;

	temp = NULL;
	i = 0;
	if (!utils->envp)
		return (0);
	while (utils->envp[i])
	{
		if (strncmp(utils->envp[i], "PWD=", 4) == 0)
		{
			temp = ft_substr(utils->envp[i], 4, ft_strlen(utils->envp[i]) -4);
			printf("%s\n", temp);
			free (temp);
		}
	}
	return (0);
}

/*  int	unset(t_utils *utils, char **arg)

{
	int		i;
	char	*temp;

	temp = ft_strjoin(arg[1], "=");
	if (!temp)
		error_func("malloc error");
	i = 0;
	while (arg[1] && utils->envp[i])
	{
		if (strncmp(arg[1], utils->envp[i], )) //next has to be = or '\0' for EXP
		if (arg[1][i] == '=')
		{
			remove_env(utils, ft_substr(arg[1][i], 0, i +1));
			remove_exp(utils, ft_strjoin("declare -x " , ft_substr(arg[1][i], 0, i +1)));
			return (0);
		}
		i++;
	}
	remove_exp(utils, arg[1][i])

} */
