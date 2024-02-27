/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:44:03 by clundber          #+#    #+#             */
/*   Updated: 2024/02/27 21:00:36 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* void	initiate_node(t_cmd *cmd_node)

{


	
} */

char	*env_variable(char *str, char **envp)

{

	
}

void	var_substitution(char **array[], char *envp[])

{
	int	i;

	i = 0;
	while (array[i])
	{
		if (ft_strrchr(array[i], '$'))
		{
			
		}

		i++;
	}

}
void	error_func(char *str)

{
	ft_putendl_fd(str, 2);
	exit (1);
}

char	**lexer(char *argv, char **envp)

{
	char **array;

	array = NULL;
	array = ppx_split(argv, ' ');
	if (!array)
		exit(1);
	var_substitution(&array, envp);
	return (array);
}

int	main(int argc, char *argv[], char *envp[])

{
	t_cmd	*cmd_head;
	char	**array;

	array = NULL;
	cmd_head = NULL;
	if (argc < 2 || !argv[1][0])
		return(0);
	array = lexer(argv[1], envp);

	int	i = 0;
	while (array[i])
		printf("%s\n", array[i++]);
}
