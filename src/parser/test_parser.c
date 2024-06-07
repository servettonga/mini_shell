#include "parser.h"

// mock function
char *get_varval(char *name)
{
    (void)name;
    return ("my_var_val");
}

// helper functions
int my_strcmp(char *s1, char *s2)
{
    if (s1 == NULL && s2 == NULL)
        return 0;
    if (s1 == NULL || s2 == NULL)
        return 1;
    return strcmp(s1, s2);
}

void cmdcmp(t_command c1, t_command c2, int n)
{
    int i = 0;
    while (c1.args[i] || c2.args[i])
    {
        if (my_strcmp(c1.args[i], c2.args[i])){
           printf("E %d.1\n", n);
           break;
        }
        i++;
    }
    if (c1.connection_type != c2.connection_type)
        printf("E %d.2\n", n);
    if (c1.is_heredoc != c2.is_heredoc
        || my_strcmp(c1.limiter, c2.limiter)
        || my_strcmp(c1.infile, c2.infile)
        || my_strcmp(c1.outfile, c2.outfile)
        || c1.outfile_append_mode != c2.outfile_append_mode)
        printf("E %d.3\n", n);
    free_split(c1.args);
    free(c1.limiter);
    free(c1.infile);
    free(c1.outfile);
}

void set_next(t_pipeline **node)
{
    t_pipeline *tmp = *node;
    *node = tmp->next;
    free(tmp);
}

/*
** Test cases
*/
// Base case
void test_case_1()
{
    char *line = "ls -l | grep -e .c | wc -l";
    t_pipeline *res = parse(line);
    // 0
    char *cmd0_args[] = {"ls", "-l", NULL};
    t_command cmd0 = {cmd0_args, CON_NONE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd0, 0);
    // 1
    set_next(&res);
    if (!res){
        printf("E 1.0\n"); exit(1);}
    char *cmd1_args[] = {"grep", "-e", ".c", NULL};
    t_command cmd1 = {cmd1_args, CON_PIPE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd1, 1);
    // 2
    set_next(&res);
    if (!res){
        printf("E 3.0\n"); exit(1);}
    char *cmd2_args[] = {"wc", "-l", NULL};
    t_command cmd2 = {cmd2_args, CON_PIPE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd2, 0);
    // 3
    set_next(&res);
    if (res){
        printf("E 4.0\n"); exit(1);}
}

// Redirections and connection types
void test_case_2()
{
    char *line = "<infile ls && grep 1 >outfile || << LIM >> outfile wc -l";
    t_pipeline *res = parse(line);
    // 0
    char *cmd0_args[] = {"ls", NULL};
    t_command cmd0 = {cmd0_args, CON_NONE, 0, NULL, "infile", NULL, 0};
    cmdcmp(res->cmd, cmd0, 0);
    // 1
    set_next(&res);
    if (!res){
        printf("E 1.0\n"); exit(1);}
    char *cmd1_args[] = {"grep", "1", NULL};
    t_command cmd1 = {cmd1_args, CON_AND, 0, NULL, NULL, "outfile", 0};
    cmdcmp(res->cmd, cmd1, 1);
    // 2
    set_next(&res);
    if (!res){
        printf("E 3.0\n"); exit(1);}
    char *cmd2_args[] = {"wc", "-l", NULL};
    t_command cmd2 = {cmd2_args, CON_OR, 1, "LIM", NULL, "outfile", 1};
    cmdcmp(res->cmd, cmd2, 0);
    // 3
    set_next(&res);
    if (res){
        printf("E 4.0\n"); exit(1);}
}

// variables
void test_case_3()
{
    char *line = "echo $VAR $? \"$?\" '$?' '\"$?\"' \"'$VAR'\"";
    t_pipeline *res = parse(line);
    // 0
    char *cmd0_args[] = {"echo", "my_var_val", "my_var_val", "my_var_val", "$?", "\"$?\"", "'my_var_val'", NULL};
    t_command cmd0 = {cmd0_args, CON_NONE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd0, 0);
    // 1
    set_next(&res);
    if (res){
        printf("E 1.0\n"); exit(1);}
}

// wildcards
// expected order of objects in readdir: https://stackoverflow.com/a/8984803
// order of files is changing if you modify the files, so if you face Error message double check the expected order with `ls -f`
void test_case_4()
{
    char *line = "echo replace* SEP *parser.c SEP split*.c";
    t_pipeline *res = parse(line);
    // 0
    char *cmd0_args[] = {"echo", "replace_vars.c", "replace_wildcards.c", "SEP", "parser.c", "test_parser.c", "SEP", "split_line.c", "split_tokens_per_command.c", NULL};
    t_command cmd0 = {cmd0_args, CON_NONE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd0, 0);
    // 1
    set_next(&res);
    if (res){
        printf("E 1.0\n"); exit(1);}
}


int main()
{
    printf("Runnung test 1...\n");
    test_case_1();
    printf("Runnung test 2...\n");
    test_case_2();
    printf("Runnung test 3...\n");
    test_case_3();
    printf("Runnung test 4...\n");
    test_case_4();
}
