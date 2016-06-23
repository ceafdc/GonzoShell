#ifndef parser_h
#define parser_h

#include "../structures/structures.h"

typedef enum {
    FOREGROUND,
    BACKGROUND
} GroundMode;

typedef struct {
    Vector *commands;
    FILE *fin;
    FILE *fout;
    FILE *ferr;
    GroundMode ground;
} Pipeline;

typedef struct {
    String *program;
    Vector *arguments;
} Command;

Pipeline * parse_cmd(String *line);
char ** genargv(Command *cmd);

#endif
