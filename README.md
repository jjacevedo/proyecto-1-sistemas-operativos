# Proyecto de Sistemas Operativos
## Implementación de un Shell para xv6

### Integrantes

- Juan José Acevedo Otálvaro
- Miguel Anegel Marin Mejia
- 

### Curso

Sistemas Operativos

### Semestre

2026-2

---

# Descripción

Este proyecto implementa un shell para el sistema operativo xv6. El programa
lee comandos desde la entrada estándar, los interpreta y ejecuta los programas
correspondientes usando los mecanismos de procesos de xv6 (`fork`, `exec`,
`wait`), sin depender de la implementación original del shell de xv6.

La implementación soporta:

- Ejecución de comandos simples (`ls`, `echo`).
- Comandos con argumentos (`echo hola mundo`).
- Redirección de entrada (`<`).
- Redirección de salida (`>`), con truncado del archivo si ya existía.
- Tuberías simples (`A | B`) y tuberías múltiples encadenadas (`A | B | C | ...`).
- Comando interno `exit` para finalizar el shell de forma controlada.

---

# Estructura de archivos

```text
user/
├── sh.c
├── parser.c
├── parser.h
├── commands.c
└── commands.h
```

## Descripción de los archivos

### sh.c

Loop principal del shell. Se encarga de mostrar el prompt, leer la línea
escrita por el usuario, decidir si se trata de un comando simple o de una
tubería, y delegar el parsing y la ejecución a `parser.c` y `commands.c`
respectivamente. También detecta el comando interno `exit`.

### parser.c / parser.h

Convierte el texto que escribe el usuario en estructuras que `commands.c`
puede ejecutar:

- `parse_line`: separa una línea en argumentos (`argv[]`), detectando además
  los operadores `<` y `>` y extrayendo el nombre del archivo asociado sin
  incluirlo en `argv`.
- `parse_pipeline`: separa una línea por el operador `|` en varios comandos
  independientes, y tokeniza cada uno.
- `tokenize` (interna, no expuesta en el `.h`): función compartida que separa
  un string en palabras por espacios. La usan tanto `parse_pipeline` como,
  parcialmente, la lógica de `parse_line`.

### commands.c / commands.h

Ejecuta lo que el parser preparó:

- `run_command`: ejecuta un comando simple. Hace `fork`, y en el hijo aplica
  las redirecciones de entrada/salida (si las hay) antes de llamar a `exec`.
  El padre espera con `wait`.
- `run_pipeline`: ejecuta una cadena de N comandos conectados por pipes.
  Crea un proceso por cada comando, conecta la salida de cada uno con la
  entrada del siguiente mediante pipes, y espera a que todos terminen.

---

# Integración con xv6

## Paso 1. Obtener xv6

```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
```

## Paso 2. Copiar archivos

Copiar `sh.c`, `parser.c`, `parser.h`, `commands.c` y `commands.h` dentro del
directorio `user/` de xv6.

## Paso 3. Modificar el Makefile

Agregar una regla específica para `$U/_sh` que enlace los tres archivos
fuente (`sh.o`, `parser.o`, `commands.o`) junto con las librerías comunes de
usuario, en lugar de depender de la regla genérica `_%` (que solo enlaza el
primer archivo `.o`).

## Paso 4. Compilar y ejecutar

```bash
make qemu
```

Dentro de xv6, el shell arranca automáticamente (`init: starting sh`).

---

# Casos de prueba

| Comando | Resultado esperado | Verificado |
|---|---|---|
| `ls` | Lista los archivos de xv6 | Sí |
| `echo hola` | Imprime `hola` | Sí |
| `echo hola mundo` | Imprime `hola mundo` | Sí |
| `comandoquenoexiste` | Imprime `shell: comando no encontrado: ...` sin cerrar el shell | Sí |
| `cat < README` | Muestra el contenido de README, leído por redirección de entrada | Sí |
| `cat < noexiste.txt` | Imprime `shell: no se pudo abrir noexiste.txt` | Sí |
| `echo hola > salida.txt` seguido de `cat salida.txt` | El archivo contiene `hola` | Sí |
| `echo primero > salida.txt` seguido de `echo segundo > salida.txt` y `cat salida.txt` | Solo muestra `segundo` (el archivo se trunca en cada escritura) | Sí |
| `echo hola \| wc` | Cuenta correctamente líneas, palabras y caracteres | Sí |
| `ls \| grep sh` | Filtra solo la línea correspondiente al binario `sh` | Sí |
| `cat README \| grep the \| grep xv6 \| wc` | Tubería de 4 comandos, filtra en cascada y cuenta el resultado | Sí |
| `exit` | Cierra el shell (`init` lanza uno nuevo) | Sí |

---

# Decisiones de diseño

- **Separación en tres archivos** (`sh.c`, `parser.c`, `commands.c`): cada uno
  tiene una única responsabilidad. `sh.c` orquesta el ciclo de vida del
  shell, `parser.c` solo transforma texto en datos, `commands.c` es el único
  archivo que interactúa con las llamadas al sistema de procesos.

- **`infile`/`outfile` como parámetros separados del `argv`**: se descartó
  dejar `<`/`>` y el nombre del archivo dentro del `argv[]`, porque el
  programa ejecutado (por ejemplo `cat`) no debe recibir esos símbolos como
  argumentos propios.

- **`in_fd` como "cable" entre iteraciones en `run_pipeline`**: para soportar
  tuberías de longitud arbitraria, se usa una sola variable que guarda el
  descriptor de lectura heredado de la iteración anterior. Esto evita tener
  que tratar el primer y el último comando de la tubería como casos
  completamente distintos del resto.

- **`exit` como comando interno, no como programa externo**: xv6 no incluye
  un binario `exit`, y aunque lo incluyera, un proceso hijo terminándose no
  cerraría el shell (proceso padre). Por eso `exit` se detecta directamente
  en `sh.c` y se resuelve llamando a `exit()` sobre el propio shell, sin
  hacer `fork`.

- **Se descartó una refactorización mayor de `parse_line`**: se evaluó
  extraer una función común para el manejo de `<` y `>` (ambos bloques son
  casi idénticos), pero el primer intento de refactorización introducía un
  buffer intermedio adicional y mayor riesgo de desbordamiento sin una
  ganancia real de claridad. Se mantuvo la duplicación puntual por ser más
  simple de verificar y más segura. Sí se extrajo `tokenize` como función
  compartida entre `parse_pipeline` y la lógica de tokenización, donde la
  duplicación era directa y la extracción no añadía complejidad.

---

# Limitaciones conocidas

- No soporta comillas.
- No soporta variables de entorno.
- No soporta ejecución en segundo plano (`&`).
- No soporta combinar redirección y tuberías en el mismo comando compuesto
  (por ejemplo `cat < a.txt | grep x > b.txt`), ya que el enunciado no lo
  exige y `parse_pipeline` no procesa `<`/`>` dentro de cada tramo.
- No implementa `cd`, `history`, `jobs`, `fg`, `bg` ni `kill`, por quedar
  explícitamente fuera del alcance del proyecto.

---

# Declaración de uso de IA

Durante el desarrollo del proyecto se utilizó Claude (Anthropic) como
herramienta de apoyo para comprensión de conceptos de sistemas operativos,
guía en el diseño e implementación, revisión de código y
resolución de errores de compilación y de lógica (incluyendo un bug de
sincronización en la actividad de pipes con procesos padre-hijo).

Los integrantes comprenden el funcionamiento completo del shell implementado
y son capaces de explicar y justificar cada decisión de diseño tomada,
incluyendo el manejo de descriptores de archivo en las redirecciones, el
mecanismo de doble fork y `dup` en las tuberías, y el motivo por el cual
`exit` se implementó como comando interno.

Los integrantes asumen plena responsabilidad académica sobre el contenido
entregado.

# Nota sobre el historial de commits

El desarrollo del shell se realizó de forma incremental (comandos simples,
redirección de entrada, redirección de salida, pipes, comando `exit`,
refactorización), con un commit por cada etapa, dentro del entorno de trabajo
local usado para compilar y probar contra xv6 completo. Ese entorno no
corresponde a este repositorio de entrega (que, según lo solicitado, no debe
contener el código fuente completo de xv6), por lo que el código final se
integró a este repositorio en un único commit consolidado.
