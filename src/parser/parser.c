#include "parser.h"

void cmd_free(void *obj);

typedef enum {
    PROGRAM = 0,
    ARGUMENTS,
} ParseState;

Vector *
parse_cmd(String *line) {
    Vector *vector = vector_init();
    Command *cmd = alloc(sizeof(Command), cmd_free);
    vector_append(vector, cmd);
    release(cmd);

    int inString = 0;
    int isEscaped = 0;

    ParseState state = 0; // program, arguments

    cmd = (Command*)vector->objs[vector->count-1];
    cmd->program = str_init();
    cmd->arguments = vector_init();
    for (int i = 0; i < line->len; i++) {
        char c = line->string[i];
        if (c == ' ' && !inString && !isEscaped) {
            if (state == PROGRAM) {
                state = ARGUMENTS;
            }
            if (state == ARGUMENTS) {
                String *s = str_init();
                vector_append(cmd->arguments, s);
                release(s);
            }
            continue;
        }
        if ((c == '\"' || c == '\'') && !isEscaped) {
            inString = !inString;
            continue;
        }
        if (c == '\\' && !isEscaped) {
            isEscaped = 1;
            continue;
        }
        switch (state) {
            case PROGRAM: {
                str_append_char(cmd->program, c);
                continue;
            }
            case ARGUMENTS: {
                String *argument = (String *)cmd->arguments->objs[cmd->arguments->count - 1];
                str_append_char(argument, c);
                break;
            }
        }
    }
    return vector;
}


char **
genargv(Command *cmd) {
    char **argv = (char **)calloc(cmd->arguments->count + 2, sizeof(char *));

    argv[0] = (char *)malloc(sizeof(char *) * (cmd->program->len + 1));
    strcpy(argv[0], cmd->program->string);
    for (int i = 0; i < cmd->arguments->count; i++) {
        String *argument = (String *)cmd->arguments->objs[i];
        argv[i + 1] = (char *)malloc(sizeof(char *) * (argument->len + 1));
        strcpy(argv[i + 1], argument->string);
    }

    return argv;
}

void
cmd_free(void *obj) {
    Command *cmd = (Command *)obj;
    release(cmd->program);
    release(cmd->arguments);
}
