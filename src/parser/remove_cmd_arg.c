#include "parser.h"

void remove_cmd_arg(t_pipeline *node, int ind)
{
	free(node->cmd.args[ind]);
	while (node->cmd.args[ind])
	{
		node->cmd.args[ind] = node->cmd.args[ind + 1];
		ind++;
	}
}
