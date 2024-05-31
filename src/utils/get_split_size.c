int get_split_size(char **split)
{
	int i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}
