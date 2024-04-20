/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:10:47 by clundber          #+#    #+#             */
/*   Updated: 2024/04/20 12:03:27 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	check_equal(t_ms *ms, char *arg)
{
	char	*temp;
	int		i;
	int		len;
	int		x;

	x = 0;
	len = 0;
	i = 0;
	temp = NULL;
	temp = ft_strjoin("declare -x ", arg);
	while (temp[len] && temp[len] != '=')
		len++;
	while (ms->utils->export[i])
	{
		if ((ft_strncmp(temp, ms->utils->export[i], len) == 0)
			&& ms->utils->export[i][len] == '\0')
		{
			remove_exp(ms, i, x, len);
			ft_nullfree(&temp);
			return ;
		}
		i++;
	}
	ft_nullfree(&temp);
}

static void	export_loop2(char *arg, t_ms *ms, int *i)
{
	check_equal(ms, arg);
	ms->temp2 = ft_substr(arg, 0, ((*i) + 1));
	malloc_check(&ms->temp2, ms);
	change_var(&ms->utils->envp, ms->temp2, arg, ms);
	ms->temp = jointhree("declare -x ", ms->temp2, "\"", ms);
	ft_nullfree(&ms->temp2);
	malloc_check(&ms->temp, ms);
	ms->temp2 = ft_free_strjoin(ms->temp, ft_substr(arg,
				((*i) + 1), (ft_strlen(arg) - (*i))));
	malloc_check(&ms->temp2, ms);
	ms->temp = ft_strjoin(ms->temp2, "\"");
	ft_nullfree(&ms->temp2);
	malloc_check(&ms->temp, ms);
	ms->temp2 = ft_strdup("declare -x ");
	malloc_check(&ms->temp2, ms);
	ms->temp2 = ft_free_strjoin(ms->temp2, ft_substr(arg, 0, ((*i) + 1)));
	malloc_check(&ms->temp2, ms);
	change_var(&ms->utils->export, ms->temp2, ms->temp, ms);
	ft_nullfree(&ms->temp);
	ft_nullfree(&ms->temp2);
}

static void	export_loop(char *arg, t_ms *ms)
{
	int		i;
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
	i = 0;
	while (arg[i])
	{
		quote_status2(&quote, &dquote, arg[i]);
		if (arg[i] == '=' && quote == false && dquote == false)
		{
			export_loop2(arg, ms, &i);
			break ;
		}
		if (arg[++i] == '\0')
		{
			ms->temp = ft_strjoin("declare -x ", arg);
			malloc_check(&ms->temp, ms);
			add_exp_var(&ms->utils, ms->temp, ms);
			ft_nullfree(&ms->temp);
		}
	}
}

int	export(t_utils *utils, char **arg, t_ms *ms)
{
	int		i;
	int		ret;

	ret = 0;
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
		if (export_error(arg[i], 1) == 0)
			export_loop(arg[i], ms);
		else
			ret = 1;
		i++;
	}
	sort_export(utils);
	return (ret);
}
