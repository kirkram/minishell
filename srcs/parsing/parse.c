/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/02/29 17:00:08 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "parsing.h"

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

char	*env_variable(char *str, char **envp)

{
	int		i;
	int		start;
	char	*new_str;
	char	*temp;
	char	*ptr;

	ptr = NULL;
	temp = NULL;
	new_str = NULL;
	start = 0;
	i = -1;
	while (i == -1 || str[i])
	{
		i++;
		if (str[i] == '$' && start == 0 && i > start)
		{
			if (str[i +1] == '\0')
				return (str);
			new_str = ft_substr(str, start, i);
			if (!new_str)
				error_func("malloc failed in lexer\n");
			start = i + 1;
		}
		else if ((str[i] == '$' || str[i] == '\0') && i > start)
		{
			temp = ft_strjoin(ft_substr(str, start, i), "=");
			new_str = get_variable(temp, envp, new_str);
			start = i +1;
		}
		if (str[i] == '$' && str[i +1] == '\0')
		{
			if (!new_str)
				return (str);
			ptr = new_str;
			new_str = ft_strjoin(new_str, "$");
			free (ptr);
			break ;
		}
	}
	free (str);
	return (new_str);
}

void	var_substitution(char **array, char *envp[])

{
	int	i;

	i = 0;
	while (array[i])
	{
		if (ft_strrchr(array[i], '$'))
			array[i] = env_variable(array[i], envp);
		i++;
	}

}

void	error_func(char *str)

{
	ft_putendl_fd(str, 2);
	exit (1);
}



int	get_token(char *str)

{
	if (ft_strncmp(str, "|", 2) == 0)
		return (2);
	else if (ft_strncmp(str, "<", 2) == 0)
		return (3);
	else if (ft_strncmp(str, "<<", 3) == 0)
		return (4);
	else if (ft_strncmp(str, ">", 2) == 0)
		return (5);
	else if (ft_strncmp(str, ">>", 3) == 0)
		return (6);
	else
		return (1);
}

int	*tokenizer(char **array)

{
	int	i;
	int	*tokens;

	tokens = NULL;
	i = 0;
	while (array[i])
		i++;
	tokens = malloc (sizeof(int *) * (i +1));
	if (!tokens)
		error_func("Token malloc failure\n");
	i = 0;
	while (array[i])
	{
		tokens[i] = get_token(array[i]);
		i++;
	}
	return (tokens);
}

char	**get_bigcmd(char **cmds, int *tokens, int start, int end)

{
	char	**temp_arr;
	int		i;

	i = 0;
	temp_arr = NULL;
	temp_arr = malloc(sizeof (char **) * ((end - start) + 2));
	while (cmds[start] && start <= end)
	{
		temp_arr[i] = ft_strdup(cmds[start]);
		if (!temp_arr[i])
			error_func("Malloc failure\n");
		i++;
		start++;
	}
	temp_arr[i] = NULL;
	return (temp_arr);
}

char	**array_copy(char **array)

{
	char	**new_array;
	int		i;

	i = 0;
	new_array = NULL;
	while (array[i])
		i++;
	new_array = malloc(sizeof(char *) * (i +1));
	if (!new_array)
		return (0);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			ft_arrfree(new_array);
			return (0);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

void	listmaker(t_pline **head, char **cmds, int *tokens)

{
	int		i;
	char	**temp_arr;
	int		start;

	start = 0;
	temp_arr = NULL;
	i = 0;

	while (cmds[i])
	{
		if (i == 0 && (tokens[i] == IN_FD || tokens[i] == IN_HD))
			i++;
		if (cmds[i +1] == '\0')
		{
			temp_arr = get_bigcmd(cmds, tokens, start, i);
			if (ms_lstadd_back(head, ms_lstnew(temp_arr)) == 0)
			{
				lst_clear(head); // more freeing etc.
				error_func("Malloc failed\n");
			}
			free(temp_arr);
			break ;
		}
		if (tokens[i] == IN_FD || tokens[i] == IN_HD)
		{
			temp_arr = get_bigcmd(cmds, tokens, start, i);
			if (ms_lstadd_back(head, ms_lstnew(temp_arr)) == 0)
			{
				lst_clear(head); // more freeing etc.
				error_func("Malloc failed\n");
			}
			free(temp_arr);
			start = i +1;
		}
		if (tokens[i] == OUT || tokens[i] == OUT_AP)
		{
			i++;
			//if statement for CMD?
			temp_arr = get_bigcmd(cmds, tokens, start, i);
			if (ms_lstadd_back(head, ms_lstnew(temp_arr)) == 0)
			{
				lst_clear(head); // more freeing etc.
				error_func("Malloc failed\n");
			}
			free(temp_arr);
			start = i +1;
		}
		i++;
	}
}

void	**lexer(char *argv, char **envp, t_pline *pipe)

{
	char	**array;
	int		*tokens;

	tokens = NULL;
	array = NULL;
	array = ppx_split(argv, ' ');
	if (!array)
		exit(1);
	var_substitution(array, envp);
	tokens = tokenizer(array);
	listmaker(pipe, array, tokens);
	ft_arrfree(array);
	free (tokens);
}
