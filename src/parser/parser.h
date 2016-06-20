#ifndef parser_h
#define parser_h

#include "../structures/structures.h"

typedef struct {
    String *program;
    Vector *arguments;
    FILE *fin;
    FILE *fout;
    FILE *ferr;
} Command;

Vector * parse_cmd(String *command);
char **genargv(Command *cmd);

#endif
