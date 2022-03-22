cmake_minimum_required(VERSION 3.0.0)

project(ComInt VERSION 0.0.1)
set(CMAKE_BUILD_TYPE Release)


#OPCIONES COMPILACIÓN
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3")


#FICHEROS QUE HACEN FALTA
set(
    HEADERS 
    analizadores/analizador_lexico.h
    analizadores/analizador_sintactico.h
    auxiliares/definiciones.h
    auxiliares/tabla_simbolos.h
    auxiliares/errores.h
    sistema_entrada.h
)

set(
    SOURCES 
    analizadores/analizador_lexico.c
    analizadores/analizador_sintactico.c
    auxiliares/definiciones.c
    auxiliares/tabla_simbolos.c
    auxiliares/errores.c
    sistema_entrada.c
    main.c
)


#EJECUTABLE
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
