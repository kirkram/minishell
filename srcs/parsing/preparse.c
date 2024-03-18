/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:46:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/15 14:26:37 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	quote_status(bool *quote)
{
	if ((*quote) == false)
		(*quote) = true;
	else if ((*quote) == true)
		(*quote) = false;
}

char	*separator(char *str, bool quote, bool dquote)
{
	int		i;
	int		x;
	char	*temp;

	i = 0;
	x = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			quote_status(&quote);
		else if (str[i] == '\"')
			quote_status(&dquote);
		if (quote == false && dquote == false)
		{
			if ((str[i] == '>' || str[i] == '|' || str[i] == '<') && (i == 0 || (i > 0 && str[i -1] != str[i])))
				x++;
		}
		i++;
	}
	temp = malloc(sizeof(char) * (i + (x *2) + 1));
	if (!temp)
		malloc_error(1);
	i = 0;
	x = 0;
	quote = false;
	dquote = false;
	while (str[i])
	{
		if (str[i] == '\'')
			quote_status(&quote);
		else if (str[i] == '\"')
			quote_status(&dquote);
		if (str[i] == '>' || str[i] == '|' || str[i] == '<')
		{
			if ((quote == false && dquote == false) && (i == 0 || str[i -1] != str[i]))
			{
				temp[x] = ' ';
				x++;
			}
			temp[x] = str[i];
			x++;
			if (str[i +1] != str[i] && (quote == false && dquote == false))
			{
				temp[x] = ' ';
				x++;
			}
		}
		else
		{
			temp[x] = str[i];
			x++;
		}
		i++;
	}
	temp[x] = '\0';
	return (temp);
}

void	var_substitution(char **str, char *envp[], int err_code)

{
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
	*str = env_variable(*(str), envp, err_code, quote, dquote);
	*str = separator(*(str), quote, dquote);
}

char	*env_variable(char *str, char **envp, int err_code, bool quote, bool dquote)
{
	int		i;
	int		start;
	char	*new_str;
	char	*temp;
	char	*ptr;

	temp = NULL;
	new_str = NULL;
	start = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' && quote == false)
			quote_status(&dquote);
		if (str[i] == '\'' && dquote == false)
			quote_status(&quote);
		if (str[i] == '$' && quote == false)
		{
			if (str[i +1] == '\0')
				i++;
			if (!new_str)
				new_str = ft_substr(str, start, i);
			else
			{
				ptr = new_str;
				new_str = ft_strjoin(new_str, ft_substr(str, start, i - start));
				free (ptr);
			}
			if (str[i] == '\0')
				break ;
			i++;
			start = i;
			while (str[i] != ' ' && str[i] != '\0' && str[i] != '$' && str[i] != '\'' && str[i] != '\"')
				i++;
			temp = ft_strjoin(ft_substr(str, start, i - start), "=");
			ptr = new_str;
			new_str = ft_strjoin(new_str, get_variable(temp, envp, err_code));
			free (ptr);
			free (temp);
			temp = NULL;
			start = i;
			if (str[i] == '\0')
				break;
		}
		else
			i++;
		if (str[i] == '\0')
		{
			ptr = new_str;
			new_str = ft_strjoin(new_str, ft_substr(str, start, i - start));
			free(ptr);
		}
	}
	if (!new_str)
		return (str);
	free (str); //hmm
	return (new_str);
}

char	*get_variable(char *temp, char **envp, int err_code)
{
	int		i;
	char	*env_var;

	env_var = NULL;
	i = 0;
	if (ft_strnstr("?=", temp, ft_strlen(temp)))
		 return (ft_itoa(err_code));
	while (envp[i])
	{
		if (ft_strnstr(envp[i], temp, ft_strlen(temp)))
		{
			env_var = ft_substr(envp[i], ft_strlen(temp), ft_strlen(envp[i]) - ft_strlen(temp));
			break ;
		}
		i++;
	}
	if (!env_var)
		return ("");
	return (env_var);
}

int	get_token(char *str)

{
	if (ft_strncmp(str, "|", 2) == 0)
		return (PIPE);
	else if (ft_strncmp(str, "<", 2) == 0)
		return (IN_FD);
	else if (ft_strncmp(str, "<<", 3) == 0)
		return (IN_HD);
	else if (ft_strncmp(str, ">", 2) == 0)
		return (OUT);
	else if (ft_strncmp(str, ">>", 3) == 0)
		return (OUT_AP);
	else
		return (CMD);
}

