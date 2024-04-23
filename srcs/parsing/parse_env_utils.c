/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:51:37 by clundber          #+#    #+#             */
/*   Updated: 2024/04/23 14:53:49 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	quote_str_count(char *str)
{
	int		count;
	int		x;

	x = 0;
	count = 1;
	while (str[x])
	{
		while (str[x] == ' ')
			x++;
		if (str[x])
			count++;
		while (str[x] && str[x] != ' ')
			x++;
	}
	count *= 2;
	count += x +1;
	return (count);
}

static char	*quote_add(char **str, t_ms *ms)
{
	int		i;
	char	*temp;
	int		x;

	x = 0;
	temp = NULL;
	i = 0;
	temp = malloc(sizeof(char *) * quote_str_count((*str)) + 1);
	if (!temp)
	{
		ft_nullfree(str);
		malloc_check(NULL, ms);
	}
	while ((*str)[i])
	{
		while ((*str) && (*str)[i] == ' ')
			temp[x++] = (*str)[i++];
		temp[x++] = '\"';
		while ((*str)[i] && (*str)[i] != ' ')
			temp[x++] = (*str)[i++];
		temp[x++] = '\"';
	}
	temp[x] = '\0';
	ft_nullfree(str);
	return (temp);
}

char	*get_variable(t_ms *ms)
{
	int		i;
	char	*env_var;

	env_var = NULL;
	i = 0;
	if (env_error(&ms->temp2, ms) == 1)
		return (ft_itoa(ms->utils->err_code));
	while (ms->utils->envp[i])
	{
		if (ft_strnstr(ms->utils->envp[i], ms->temp2, ft_strlen(ms->temp2)))
		{
			env_var = ft_substr(ms->utils->envp[i], ft_strlen(ms->temp2),
					ft_strlen(ms->utils->envp[i]) - ft_strlen(ms->temp2));
			malloc_check(&env_var, ms);
			break ;
		}
		i++;
	}
	if (ms->temp2)
		ft_nullfree(&ms->temp2);
	if (!env_var)
		return (ft_strdup(""));
	env_var = quote_add(&env_var, ms);
	return (env_var);
}
