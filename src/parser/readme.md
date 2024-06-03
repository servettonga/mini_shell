
## Parser

Section responsible for parsing line of prompt.
The result of the parser is the structure `t_pipeline` (located in minishell.h), which describes what and how should be executed.

Parsing steps:
- the line into tokens
- split tokens into commands
- for each command:
	- set redirection
	- replace variables with their values
	- replace wildcards with file names
	- drop utside quotation marks

### Outside API

`t_pipeline *parse(char *line)`

### Notes

- section contains tests (parser_tests.c, tests_makefile); these files should be ditched in 42 repo