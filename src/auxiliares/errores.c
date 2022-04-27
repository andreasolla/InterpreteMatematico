#include "auxiliares/errores.h"

#include <stdio.h>

#define COLOR_ROJO     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

void lanzar_error(char *msg){
    printf(COLOR_ROJO "\n  ╭────────────────────────────────╮\n");
    printf("╭─┤ERROR (╮°-°)╮┳━━┳ ( ╯°□°)╯ ┻━━┻ ├────────────\n");
    printf("│ ╰────────────────────────────────╯\n");
    //printf("│  Línea: %d Columna: %d\n");
    printf("│  %s\n", msg);
    printf("╰───────────────────────────────────────────────\n\n" COLOR_RESET);
}