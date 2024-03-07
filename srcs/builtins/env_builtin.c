/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:38:21 by clundber          #+#    #+#             */
/*   Updated: 2024/03/07 17:23:50 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	export(t_utils *utils, char *str)

{
	int	i;
	bool	quote;
	bool	dquote;
	bool	equal;

	equal = false;
	quote = false;
	dquote = false;
	i = 0;
	if (!str)
	{
		//print the export?
		return (0);
	}
	while(str[i])
	{
		if(str[i] == '\'')
			quote_status(&quote);
		else if(str[i] == '\"')
			quote_status(&dquote);
		if (str[i] == '=')
			equal = true;


	i++;
	}


}

int	env(t_utils *utils)

{
	int	i;

	i = 0;
	if (!utils->envp)
		return (0);
	while (utils->envp[i])
	{
		printf("%s\n", utils->envp[i]);
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

int	unset(t_utils *utils, char *str)

{


}
