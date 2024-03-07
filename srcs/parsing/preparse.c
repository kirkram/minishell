/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:46:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/07 12:04:42 by clundber         ###   ########.fr       */
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

char	*separator(char *str)

{
	int		i;
	int		x;
	char	*temp;
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
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
	{
		error_func("malloc failed\n");
		exit (1);
	}
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
	//free (str);
	return (temp);
}

void	var_substitution(char **str, char *envp[])

{
	*str = env_variable(*(str), envp);
	*str = separator(*(str));
}

char	*env_variable(char *str, char **envp)

{
	int		i;
	int		start;
	char	*new_str;
	char	*temp;
	char	*ptr;
	bool	quote;

	quote = false;
	ptr = NULL;
	temp = NULL;
	new_str = NULL;
	start = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			quote_status(&quote);
		if (str[i] == '$' && quote == false)
		{
			if (str[i +1] == '\0')
				i++;
			if (i > start)
			{
				if (!new_str)
					new_str = ft_substr(str, start, i);
				else
					new_str = ft_strjoin(new_str, ft_substr(str, start, i - start));
			}
			if (str[i] == '\0')
				break ;
			i++;
			start = i;
			while (str[i] != ' ' && str[i] != '\0' && str[i] != '$')
				i++;
			temp = ft_strjoin(ft_substr(str, start, i - start), "=");
			new_str = ft_strjoin(new_str, get_variable(temp, envp));
			free (temp);
			temp = NULL;
			start = i;
		}
		else
			i++;
	}
	if (!new_str)
		return (str);
	//free (str);
	return (new_str);
}

char	*get_variable(char *temp, char **envp)

{
	int		i;
	char	*env_var;

	env_var = NULL;
	i = 0;
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

