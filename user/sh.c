#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/parser.h"
#include "user/commands.h"

#define MAXBUF 100

int
getcmd(char *buf, int nbuf)
{
  write(2, "$ ", 2);
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if (buf[0] == 0)
    return -1;
  return 0;
}

int
has_pipe(char *buf)
{
  for (char *p = buf; *p; p++) {
    if (*p == '|')
      return 1;
  }
  return 0;
}

int
main(void)
{
  static char buf[MAXBUF];
  char *argv[MAXARGS];
  char *infile;
  char *outfile;
  char *commands[MAXCMDS][MAXARGS];
  int ncommands;

  while (getcmd(buf, sizeof(buf)) >= 0) {
    int len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
      buf[len - 1] = 0;

    if (buf[0] == 0)
      continue;  // linea vacia

    if (has_pipe(buf)) {
      parse_pipeline(buf, commands, &ncommands);
      run_pipeline(commands, ncommands);
    } else {
      int argc = parse_line(buf, argv, &infile, &outfile);
      if (argc == 0)
        continue;

      if (strcmp(argv[0], "exit") == 0) {
        exit(0);
      }

      run_command(argv, infile, outfile);
    }
  }

  exit(0);
}