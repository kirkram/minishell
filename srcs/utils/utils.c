/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 18:11:38 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/13 15:51:24 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//if exit code is -42 then it doesn't exit
void	free_all_and_exit(t_pipe ***_pipe, t_utils **utils, int ex_code)
{
	int	i;

	i = 0;
	if (_pipe && *_pipe)
	{
		while ((*_pipe)[i])
		{
			ft_arrfree((*_pipe)[i]->args);
			ft_arrfree((*_pipe)[i]->noio_args);
			free((*_pipe)[i]->tokens);
			free((*_pipe)[i]->cmd_with_path);
			free((*_pipe)[i]);
			i ++;
		}
		free((*_pipe)[i]);
		free(*_pipe);
		(*_pipe) = NULL;
	}
	if (utils && *utils)
	{
		ft_arrfree((*utils)->envp);
		ft_arrfree((*utils)->export);
	}
	if (ex_code != -42)
		exit (ex_code);
}

int	msg_stderr(char *cmd, char *message, int err_code)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
		ft_putstr_fd(cmd, 2);
	if (message)
		ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	return (err_code);
}

char	*jointhree(char const *s1, char const *s2, char const *s3)
{
	char	*newstr;

	if (s1 && s2 && s3)
	{
		newstr = malloc(((ft_strlen(s1) + ft_strlen(s2) \
		+ ft_strlen(s3)) + 1) * sizeof(char));
		if (newstr == NULL)
			malloc_error(1);
		ft_strlcpy(newstr, (char *)s1, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s2, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s3, -1);
		return (newstr);
	}
	return (NULL);
}

int	free_and_1(char **paths, int **end)
{
	int	i;

	i = 0;
	if (paths && *paths)
	{
		while (paths[i])
		{
			free (paths[i]);
			i ++;
		}
		*paths = NULL;
		free (paths);
	}
	if (end)
	{
		free (*end);
		(*end) = NULL;
	}
	return (1);
}

int	is_only_digits_and_signs(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
		{
			if (*str != '-' && *str != '+')
				return (0);
		}
		str ++;
	}
	return (1);
}
