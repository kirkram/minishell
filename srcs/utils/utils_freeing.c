/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_freeing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 16:02:11 by klukiano          #+#    #+#             */
/*   Updated: 2024/04/13 16:58:10 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//if exit code is -42 then it doesn't exit
void	free_and_exit(t_pipe ***_pipe, t_utils **utils, t_ms *ms, int ex_code)
{
	int	i;

	i = -1;
	if (_pipe && *_pipe)
	{
		while ((*_pipe)[++i])
			free_one_pipe((*_pipe)[i]);
		free(*_pipe);
		(*_pipe) = NULL;
	}
	if (utils && *utils)
	{
		ft_arrfree((*utils)->envp);
		ft_arrfree((*utils)->export);
		free (ms->utils);
	}
	if (ms)
	{
		free (ms->line);
		free (ms->temp);
		free (ms->temp2);
	}
	if (ex_code != -42)
		exit (ex_code);
}

void	free_one_pipe(t_pipe *_pipe)
{
	ft_arrfree(_pipe->args);
	ft_arrfree(_pipe->noio_args);
	free(_pipe->tokens);
	free(_pipe->cmd_with_path);
	free(_pipe);
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
