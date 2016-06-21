#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <limits.h>

#include "parser/parser.h"

String *
genhead() {
    String *head = str_init();

    struct passwd *p = getpwuid(getuid());
    str_append(head, p->pw_name);

    char hostname[HOST_NAME_MAX+1];
    gethostname(hostname, sizeof(hostname));  // Check the return value!
    str_append(head, "@");
    str_append(head, hostname);

    str_append(head, ":");

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    str_append(head, cwd);

    str_append(head, "$ ");

    return head;
}
int
main (int argc, char ** argv) {
    while (1) {
        String *head = genhead();

        printf("%s", head->string);
        String *line = str_from_stdin();
        Vector *commands = parse_cmd(line);

        for (int i = 0; i < commands->count; i++) {
            Command *cmd = (Command *)commands->objs[i];
            printf("\"%s\"\n", cmd->program->string);
            for (int i = 0; i < cmd->arguments->count; i++) {
                String *argument = (String *)cmd->arguments->objs[i];
                printf("\t\"%s\"\n", argument->string);
            }
        }
        Command *cmd = (Command *)commands->objs[0];

        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "fork error\n");
            return EXIT_FAILURE;
        } else if (pid == 0) { // child
            char **cmdargv = genargv(cmd);
            execvp(cmdargv[0], cmdargv);
            fprintf(stderr, "Command %s not found\n", cmdargv[0]);
            release(cmd);
            free(cmdargv);
        } else { // parent
            int status;
            wait(&status);
        }

        release(line);
        release(commands);
    }
    return EXIT_SUCCESS;
}
