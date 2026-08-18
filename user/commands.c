#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "user/commands.h"

void
run_command(char *argv[], char *infile, char *outfile)
{
  int pid;

  if (argv[0] == 0)
    return;

  pid = fork();

  if (pid < 0) {
    printf("shell: fork fallo\n");
    return;
  }

  if (pid == 0) {
    // Estamos en el HIJO

    if (infile != 0) {
      close(0);
      if (open(infile, O_RDONLY) < 0) {
        printf("shell: no se pudo abrir %s\n", infile);
        exit(1);
      }
    }

    if (outfile != 0) {
      close(1);
      if (open(outfile, O_WRONLY | O_CREATE | O_TRUNC) < 0) {
        printf("shell: no se pudo crear %s\n", outfile);
        exit(1);
      }
    }

    exec(argv[0], argv);
    printf("shell: comando no encontrado: %s\n", argv[0]);
    exit(1);
  } else {
    // Estamos en el PADRE
    wait(0);
  }
}

void
run_pipeline(char *commands[MAXCMDS][MAXARGS], int ncommands)
{
  int i;
  int in_fd = 0;        // de donde el comando actual debe LEER (empieza en stdin real)
  int p[2];

  for (i = 0; i < ncommands; i++) {
    int is_last = (i == ncommands - 1);

    if (!is_last)
      pipe(p);  // creamos el pipe que conecta este comando con el siguiente

    int pid = fork();

    if (pid < 0) {
      printf("shell: fork fallo\n");
      return;
    }

    if (pid == 0) {
      // Estamos en el HIJO que va a correr commands[i]

      // Si no somos el primer comando, nuestra entrada viene del pipe anterior
      if (in_fd != 0) {
        close(0);
        dup(in_fd);
        close(in_fd);
      }

      // Si no somos el ultimo comando, nuestra salida va al pipe actual
      if (!is_last) {
        close(1);
        dup(p[1]);
      }

      // Cerramos los extremos del pipe actual que ya no necesitamos
      if (!is_last) {
        close(p[0]);
        close(p[1]);
      }

      exec(commands[i][0], commands[i]);
      printf("shell: comando no encontrado: %s\n", commands[i][0]);
      exit(1);
    }

    // Estamos en el PADRE

    // Ya no necesitamos el extremo de lectura del pipe ANTERIOR
    if (in_fd != 0)
      close(in_fd);
    if (!is_last) {
      close(p[1]);       // el padre no escribe en este pipe
      in_fd = p[0];       // el siguiente comando va a leer de aqui
    }
  }

  // Esperar a los N hijos
  for (i = 0; i < ncommands; i++)
    wait(0);
}