#ifndef COMMANDS_H
#define COMMANDS_H

#include "user/parser.h"

// Ejecuta un comando simple, redirigiendo stdin desde infile y/o
// stdout hacia outfile (cualquiera de los dos puede ser NULL).
void run_command(char *argv[], char *infile, char *outfile);

// Ejecuta un pipeline de ncommands comandos conectados por pipes.
void run_pipeline(char *commands[MAXCMDS][MAXARGS], int ncommands);

#endif