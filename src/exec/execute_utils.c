/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:25:20 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/19 21:26:23 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char	**env_list_to_array(t_env *env)
{
	int		count;
	char	**env_array;
	t_env	*e;
	int		i;

	e = env;
	count = get_env_size(env);
	env_array = malloc((count + 1) * sizeof(char *));
	if (env_array == NULL)
		return (NULL);
	i = 0;
	while (e != NULL)
	{
		env_array[i] = malloc(strlen(e->key) + strlen(e->value) + 2);
		if (env_array[i] == NULL)
			return (free_split(env_array), NULL);
		ft_strlcat(env_array[i], e->key, ft_strlen(e->key) + 1);
		ft_strlcat(env_array[i], "=", 2);
		ft_strlcat(env_array[i], e->value, ft_strlen(e->value) + 1);
		i++;
		e = e->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*find_sys_cmd(char *cmd, t_env *env)
{
	char	*path;
	char	**path_dirs;
	char	*sys_cmd;
	int		i;

	path = get_env_val(env, "PATH");
	if (path == NULL)
		return (NULL);
	path_dirs = ft_split(path, ':');
	if (path_dirs == NULL)
		return (NULL);
	i = -1;
	while (path_dirs[++i] != NULL)
	{
		sys_cmd = ft_strjoin(path_dirs[i], "/");
		if (sys_cmd == NULL)
			return (free_split(path_dirs), NULL);
		sys_cmd = ft_strjoin(sys_cmd, cmd);
		if (sys_cmd == NULL)
			return (free_split(path_dirs), NULL);
		if (access(sys_cmd, F_OK) == 0)
			return (sys_cmd);
		free(sys_cmd);
	}
	return (free_split(path_dirs), NULL);
}
