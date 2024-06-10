int get_split_size(char **split)
{
	int i;

	if (!split)
		return (0);
	i = 0;
	while (split[i])
		i++;
	return (i);
}
