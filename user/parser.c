#include "kernel/types.h"
#include "user/user.h"
#include "user/parser.h"

// Funcion auxiliar interna: parte buf en palabras separadas por espacios
// y las guarda en argv (terminado en NULL). Devuelve la cantidad encontrada.
// No sabe nada de '<', '>' ni '|' -- solo tokeniza.
static int
tokenize(char *buf, char *argv[])
{
  int argc = 0;
  char *p = buf;

  while (*p) {
    while (*p == ' ')
      p++;

    if (*p == 0)
      break;

    if (argc >= MAXARGS - 1) {
      printf("shell: demasiados argumentos\n");
      break;
    }

    argv[argc] = p;
    argc++;

    while (*p != ' ' && *p != 0)
      p++;
    if (*p == ' ') {
      *p = 0;
      p++;
    }
  }

  argv[argc] = 0;
  return argc;
}

int
parse_line(char *buf, char *argv[], char **infile, char **outfile)
{
  int argc = 0;
  char *p = buf;

  *infile = 0;
  *outfile = 0;

  while (*p) {
    while (*p == ' ')
      p++;

    if (*p == 0)
      break;

    if (argc >= MAXARGS - 1) {
      printf("shell: demasiados argumentos\n");
      break;
    }

    if (*p == '<') {
      p++;
      while (*p == ' ')
        p++;

      if (*p == 0) {
        printf("shell: se esperaba un archivo despues de '<'\n");
        break;
      }

      *infile = p;
      while (*p != ' ' && *p != 0)
        p++;
      if (*p == ' ') {
        *p = 0;
        p++;
      }
      continue;
    }

    if (*p == '>') {
      p++;
      while (*p == ' ')
        p++;

      if (*p == 0) {
        printf("shell: se esperaba un archivo despues de '>'\n");
        break;
      }

      *outfile = p;
      while (*p != ' ' && *p != 0)
        p++;
      if (*p == ' ') {
        *p = 0;
        p++;
      }
      continue;
    }

    argv[argc] = p;
    argc++;

    while (*p != ' ' && *p != 0)
      p++;
    if (*p == ' ') {
      *p = 0;
      p++;
    }
  }

  argv[argc] = 0;
  return argc;
}

int
parse_pipeline(char *buf, char *commands[MAXCMDS][MAXARGS], int *ncommands)
{
  char *segs[MAXCMDS];
  int nsegs = 0;
  char *p = buf;

  segs[nsegs++] = p;
  while (*p) {
    if (*p == '|') {
      *p = 0;
      p++;
      if (nsegs >= MAXCMDS) {
        printf("shell: demasiados comandos en la tuberia\n");
        break;
      }
      segs[nsegs++] = p;
    } else {
      p++;
    }
  }

  for (int i = 0; i < nsegs; i++)
    tokenize(segs[i], commands[i]);

  *ncommands = nsegs;
  return nsegs;
}