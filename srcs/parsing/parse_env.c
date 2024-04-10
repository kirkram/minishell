/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:44:51 by clundber          #+#    #+#             */
/*   Updated: 2024/04/09 23:30:07 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//void	env_loop(char **str, char **new_str, int *start, int *i)
void	env_loop(t_ms *ms, int *start, int *i)
{
	{
		if (!ms->temp)
			ms->temp = ft_substr(ms->line, (*start), (*i));
		else
			combine_str(&ms->temp, ft_substr(ms->line, (*start), (*i) - (*start)));
		malloc_check(&ms->temp, ms);
		(*i)++;
		(*start) = (*i);
		while (ms->line[(*i)] && ms->line[(*i)] != ' ' && ms->line[(*i)] != '\0'
			&& ms->line[(*i)] != '$' && ms->line[(*i)] != '\'' && ms->line[(*i)]
			!= '\"' && ms->line[(*i) - 1] != '?' && ms->line[(*i)] != '/'
			&& ms->line[(*i)] != ':')
			(*i)++;
	}
}

/* //void	env_loop2(char **temp, char **new_str, t_utils *utils, char **str)
void	env_loop2(t_ms *ms)
{
	combine_str(new_str, get_variable((*temp),
			utils->envp, utils->err_code));
	if (!(*new_str))
	{
		if (temp)
			free (temp);
		lex_merror(utils, str);
	//	malloc_error (1);
	}
} */

//void	env_util(char **new_str, char **str, int start, int i)
void	env_util(t_ms *ms, int start, int i)
{
	//printf("is this the place?\n");
	combine_str(&ms->temp, ft_substr(ms->line, start, i - start));
	malloc_check(&ms->temp, ms);
	ms->line = ft_strdup(ms->temp);
	malloc_check(&ms->line, ms);
	ft_nullfree(&ms->temp);
}

//void	env_variable(char **str, t_utils *utils, bool quote, bool dquote)
void	env_variable(t_ms *ms, bool quote, bool dquote)
{
	int		start;
	//char	*new_str;
	//char	*temp;
	int		i;

	i = 0;
	//temp = NULL;
	//new_str = NULL;
	start = 0;
	while (ms->line && ms->line[i])
	{
		quote_status2(&quote, &dquote, ms->line[i]);
		if (ms->line[i] == '$' && quote == false && ms->line[i +1]
			&& ms->line[i +1] != ' ' && ms->line[i +1] != '$'
			&& (!(dquote == true && (ms->line[i +1] == '\''
			|| ms->line[i +1] == '\"' || ms->line[i +1] == ' '))))
		{
			//env_loop(str, &new_str, &start, &i); // to do
			env_loop(ms, &start, &i);
			ms->temp2 = ft_substr(ms->line, start, i - start);
			malloc_check(&ms->temp2, ms);
			//env_loop2(&temp, &new_str, utils, str);
			combine_str(&ms->temp, get_variable(ms));
			malloc_check(&ms->temp, ms);
			start = i;
		}
		else
			i++;
	}
	if (ms->temp)
		env_util(ms, start, i);
		//env_util(&new_str, str, start, i);
}

//char	*get_variable(char *temp, char **envp, int err_code)
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
	return (env_var);
}
