# Proyecto de Sistemas Operativos

## Implementación de un Shell para xv6

### Integrantes

- Juan José Acevedo
- Miguel Angel Marín

### Curso

Sistemas Operativos

### Semestre

2026-2

---

# Descripción

Este proyecto implementará un shell para el sistema operativo educativo xv6. El programa recibirá comandos desde la entrada estándar, analizará cada línea y ejecutará los programas ya existentes en xv6 mediante la creación de procesos.

La implementación final incluirá:

- Ejecución de comandos simples.
- Ejecución de comandos con argumentos.
- Redirección de entrada (`<`).
- Redirección de salida (`>`).
- Tuberías simples y múltiples (`|`).
- Comando interno `exit` para finalizar el shell de forma controlada.

La solución se desarrollará en C y se integrará al flujo de compilación estándar de xv6 ejecutado sobre QEMU. En esta primera entrega, `user/sh.c` es únicamente un programa mínimo de validación; el shell aún no ha sido implementado.

---

# Estructura de archivos

```text
README.md
user/
└── sh.c
```

## Descripción de los archivos

### sh.c

Programa mínimo para comprobar la integración inicial con xv6. TODO: reemplazarlo por la implementación del shell en las entregas posteriores.

---

# Integración con xv6

## Paso 1. Crear el repositorio en GitHub

En GitHub, seleccionar **New repository**, asignar un nombre al repositorio y crearlo vacío, sin añadir un README, `.gitignore` ni licencia desde GitHub. El repositorio debe ser accesible para el mecanismo de entrega y permitir su clonación automática. No se debe subir el repositorio completo de xv6.

Desde esta carpeta local, asociar el repositorio recién creado:

```bash
git remote add origin https://github.com/TODO_USUARIO/TODO_REPOSITORIO.git
```

TODO: reemplazar la URL de ejemplo por la URL HTTPS real del repositorio de GitHub.

## Paso 2. Subir los archivos de esta entrega

Subir únicamente `README.md`, `user/sh.c` y `.gitignore`. Ejecutar:

```bash
git add README.md user/sh.c .gitignore
git commit -m "Preparar estructura de la entrega 1"
git push -u origin main
```

TODO: confirmar que la rama principal del repositorio remoto se llama `main`; de lo contrario, usar el nombre de rama configurado.

## Paso 3. Obtener xv6

```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
```

## Paso 4. Copiar archivos

Copiar los archivos desarrollados por el grupo al directorio `user/` de una copia local de xv6. Para esta entrega, copiar `user/sh.c`.

## Paso 5. Modificar el Makefile

Agregar el programa `sh` al listado de aplicaciones de usuario de xv6. TODO: documentar la línea exacta según la versión de xv6 utilizada por el grupo.

## Paso 6. Compilar xv6

```bash
make qemu
```

## Paso 7. Ejecutar el programa

```bash
sh
```

En la Entrega 1, el programa mostrará un mensaje indicando que el shell aún no está implementado.

---

# Casos de prueba

En la Entrega 1 se verificará que xv6 compile con `user/sh.c` integrado y que, al ejecutar `sh`, aparezca el mensaje de programa pendiente de implementación.

Para la entrega final se probarán:

- Ejecución simple.
- Argumentos.
- Redirección de entrada.
- Redirección de salida.
- Tuberías simples.
- Tuberías múltiples.
- Comando `exit`.

---

# Decisiones de diseño

- El repositorio contiene únicamente archivos desarrollados por el grupo; xv6 se obtiene y mantiene por separado.
- La Entrega 1 conserva un único archivo fuente mínimo para validar la integración sin adelantar la implementación del shell.
- TODO: documentar la separación entre análisis sintáctico, ejecución, tuberías y redirecciones cuando se implemente el shell.

---

# Limitaciones conocidas

- El shell aún no está implementado en la Entrega 1.
- Por ahora, `sh` no interpreta ni ejecuta comandos.
- TODO: registrar las limitaciones definitivas de la implementación final, incluidas las funcionalidades explícitamente excluidas por el enunciado.

---

# Declaración de uso de IA

Durante la preparación de esta entrega se utilizaron herramientas de inteligencia artificial generativa como apoyo para revisar la estructura del repositorio y redactar la documentación inicial. Los integrantes asumen plena responsabilidad académica sobre el contenido entregado y deberán comprender, verificar y poder justificar cualquier cambio incorporado al proyecto.
