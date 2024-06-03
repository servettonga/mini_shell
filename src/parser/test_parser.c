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
}

/*
** Test cases
*/
void test_case_1()
{
    char *line = "ls -l | grep -e .c | wc -l";
    t_pipeline *res = parse(line);
    // 0
    char *cmd0_args[] = {"ls", "-l", NULL};
    t_command cmd0 = {cmd0_args, CON_NONE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd0, 0);
    // 1
    res = res->next;
    if (!res){
        printf("E 1.0\n"); exit(1);}
    char *cmd1_args[] = {"grep", "-e", ".c", NULL};
    t_command cmd1 = {cmd1_args, CON_PIPE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd1, 1);
    // 2
    res = res->next;
    if (!res){
        printf("E 3.0\n"); exit(1);}
    char *cmd2_args[] = {"wc", "-l", NULL};
    t_command cmd2 = {cmd2_args, CON_PIPE, 0, NULL, NULL, NULL, 0};
    cmdcmp(res->cmd, cmd2, 0);
    // 3
    res = res->next;
    if (res){
        printf("E 4.0\n"); exit(1);}
}

void test_case_2()
{
    char *line = "<out1 ls && grep 1 >infile || << LIM >> outfile wc -l";
    t_pipeline *res = parse(line);
    // 0
    char *cmd0_args[] = {"ls", NULL};
    t_command cmd0 = {cmd0_args, CON_NONE, 0, NULL, NULL, "out1", 0};
    cmdcmp(res->cmd, cmd0, 0);
    // 1
    res = res->next;
    if (!res){
        printf("E 1.0\n"); exit(1);}
    char *cmd1_args[] = {"grep", "1", NULL};
    t_command cmd1 = {cmd1_args, CON_AND, 0, NULL, "infile", NULL, 0};
    cmdcmp(res->cmd, cmd1, 1);
    // 2
    res = res->next;
    if (!res){
        printf("E 3.0\n"); exit(1);}
    char *cmd2_args[] = {"wc", "-l", NULL};
    t_command cmd2 = {cmd2_args, CON_OR, 1, "LIM", NULL, "outfile", 1};
    cmdcmp(res->cmd, cmd2, 0);
    // 3
    res = res->next;
    if (res){
        printf("E 4.0\n"); exit(1);}
}

int main()
{
    printf("Runnung test 1...\n");
    test_case_1();
    printf("Runnung test 2...\n");
    test_case_2();
}
