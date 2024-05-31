#include "parser.h"

void replace_wildcards(t_pipeline *node)
{
	int i;
	char *asterisk;
	t_list *args_list;
	char **ast_args;

	i = 0;
	while (node->cmd.args[i])
	{
		asterisk = ft_strchr(node->cmd.args[i], '*');
		if (asterisk && validate_asterisk(node->cmd.args[i], asterisk))
		{
			args_list = get_args_list(node->cmd.args[i], asterisk);
			if (ft_lstsize(args_list))
			{
				ast_args = malloc(sizeof(char *) * (ft_lstsize(args_list) + 1));
				fill_ast_args(ast_args, args_list);
				replace_asterisk(&node->cmd.args, i, ast_args);
				i += ft_lstsize(args_list) - 1;
			}
			ft_lstclear(&args_list, free);
		}
		i++;
	}
}

/*
Simple validation of asterisk.
Valid only if out of any type of quotes.
*/
int validate_asterisk(char *arg, char *asterisk)
{
	int sq;
	int dq;
	int i;

	i = 0;
	sq = 0;
	dq = 0;
	while (arg + i < asterisk)
	{
		if (arg[i] == '\'')
			sq++;
		else if (arg[i] == '"')
			dq++;
		i++;
	}
	if (!(sq % 2) || !(dq % 2))
		return (0);
	return (1);
}

// int count_dir_obj()
// {
// 	char *dirname;
// 	DIR *dirstream;
// 	int n_obj;
	
// 	dirname = getcwd(NULL, 0);
// 	dirstream = opendir(dirname);
// 	free(dirname);
// 	if (!dirstream)
// 		return (0);
// 	n_obj = 0;
// 	while (readdir(dirstream))
// 		n_obj++;
// 	closedir(dirstream);
// 	return (n_obj);
// }

t_list *get_args_list(char *arg, char *asterisk)
{
	t_list *res;
	char *dirname;
	DIR *dirstream;
	struct dirent *_dirent;

	res = NULL;
	dirname = getcwd(NULL, 0);
	dirstream = opendir(dirname);
	free(dirname);
	if (!dirstream)
		return (0);
	_dirent = readdir(dirstream);
	while (_dirent)
	{
		if (arg - asterisk <= ft_strlen(_dirent->d_name)
			&& !ft_memcmp(_dirent->d_name, arg, arg - asterisk)
			&& ft_strlen(asterisk + 1) <= ft_strlen(_dirent->d_name)
			&& !ft_memcmp(_dirent->d_name + ft_strlen(_dirent->d_name) - ft_strlen(asterisk + 1), asterisk + 1, ft_strlen(asterisk + 1)))
			ft_lstadd_back(&res, ft_lstnew(ft_strdup(_dirent->d_name)));
		_dirent = readdir(dirstream);
	}
	closedir(dirstream);
	return (res);
}

void fill_ast_args(char **ast_args, t_list *args_list)
{
	int i;

	i = 0;
	while (args_list)
	{
		ast_args[i] = ft_strdup(args_list->content);
		args_list = args_list->next;
		i++;
	}
}

// void replace_asterisk(char **old_args, int ast_pos, char **ast_args)
// {
// 	int i;
// 	int j;
// 	char **new_args;

// 	new_args = malloc(sizeof(char *) * (get_split_size(old_args) + get_split_size(ast_args)));
// 	i = 0;
// 	j = 0;
// 	while (old_args[i])
// 	{
// 		if (i != ast_args)
// 			new_args[i + j] = old_args[i];
// 		else
// 		{
// 			while (ast_args[j])
// 			{
// 				new_args[i + j] = ast_args[j];
// 				j++;
// 			}
// 			j--;
// 		}
// 		i++;
// 	}
// 	new_args[i + j] = 0;
// 	free(old_args[ast_pos]);
// 	free(old_args);
// 	free(ast_args);
// }

void replace_asterisk(char ***old_args_p, int ast_pos, char **ast_args)
{
    char **old_args;
	char **new_args;
	int s_old;
	int s_ast;

    old_args = *old_args_p;
	s_old = get_split_size(old_args);
	s_ast = get_split_size(ast_args);
	new_args = malloc(sizeof(char *) * (s_old + s_ast));
	ft_memcpy(new_args, old_args, sizeof(char *) * ast_pos);
	ft_memcpy(new_args + ast_pos, ast_args, sizeof(char *) * s_ast);
	ft_memcpy(new_args + ast_pos + s_ast, old_args + ast_pos + 1, sizeof(char *) * s_old - ast_pos - 1);
	new_args[s_old + s_ast - 1] = NULL;
	free(old_args[ast_pos]);
	free(old_args);
	free(ast_args);
    *old_args_p = new_args;
}
