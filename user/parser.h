#ifndef PARSER_H
#define PARSER_H

#define MAXARGS 32
#define MAXCMDS 10

int parse_line(char *buf, char *argv[], char **infile, char **outfile);
int parse_pipeline(char *buf, char *commands[MAXCMDS][MAXARGS], int *ncommands);

#endif