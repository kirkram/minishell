/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:46:03 by clundber          #+#    #+#             */
/*   Updated: 2024/03/06 17:57:38 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* void	var_substitution(char **array, char *envp[])

{
	int	i;

	i = 0;
	while (array[i])
	{
		if (ft_strrchr(array[i], '$'))
			array[i] = env_variable(array[i], envp);
		i++;
	}

} */

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
		if (i > 0 && (str[i] == '>' || str[i] == '|' || str[i] == '<'))
		{
			if (str[i -1] != str[i] && (quote == false && dquote == false))
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
	//printf("count nbr = %d\n", count);
	return (temp);

/* 	while (str[i])
	{
		if (str[i] == '\'' && quote == false)
			quote = true;
		else if (str[i] == '\'' && quote == true)
			quote = false;
		if (str[i] == '\"' && dquote == false)
			dquote = true;
		else if (str[i] == '\"' && dquote == true)
			dquote = false;
		if (str[i] == '>' && str[i +1] != '>' && str[i +1] != '\0')
		{
			if (start == 0)

		}


	} */


} 

void	var_substitution(char **str, char *envp[])

{
	//*str = env_variable(*(str), envp); // this cannot be tested before working env in minishell
	*str = separator(*(str)); // for splitting operatiors from other strings
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
	i = -1;
	while (i == -1 || str[i])
	{
		i++;
		if (str[i] == '\'' && quote == true)
			quote = false;
		else if (str[i] == '\'' && quote == false)
			quote = true;
		if (str[i] == '$' && start == 0 && i > start && quote == false)
		{
			if (str[i +1] == '\0')
				return (str);
			new_str = ft_substr(str, start, i);
			if (!new_str)
			{
				error_func("malloc failed\n" ,1);
				exit (1);
			}
			start = i + 1;
		}
		else if ((str[i] == '$' || str[i] == '\0') && i > start && quote == false)
		{
			temp = ft_strjoin(ft_substr(str, start, i), "=");
			ptr = new_str;
			new_str = get_variable(temp, envp, new_str);
			free (ptr);
			free (temp);
			start = i +1;
		}
		else if (str[i] == '\0' && i > start && new_str)
		{
			ptr = new_str;
			new_str = ft_strjoin(new_str, ft_substr(str, start, i));
			//free (ptr);
		}
		else if (str[i] == '$' && str[i +1] == '\0')
		{
			if (!new_str)
				return (str);
			ptr = new_str;
			new_str = ft_strjoin(new_str, "$");
			//free (ptr);
			break ;
		}
	}
	if (!new_str)
		return (str);
	//free (str);
	return (new_str);
}

char	*get_variable(char *temp, char **envp, char *new_str)

{
	int		i;
	char	*ptr;

	ptr = new_str;
	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], temp, ft_strlen(temp)))
		{
			new_str = ft_strjoin(new_str, ft_substr(envp[i], ft_strlen(temp), ft_strlen(envp[i] - ft_strlen(temp))));
			break ;
		}
		i++;
	}
	free(ptr);
	return (new_str);
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

