cmake_minimum_required(VERSION 3.0.0)

project(ComInt VERSION 0.0.1)
set(CMAKE_BUILD_TYPE Release)


#COMPILACIÓN DE FLEX
find_package(FLEX)
FLEX_TARGET(
    analizadorLexico src/analizadores/analizador_lexico.lex ${CMAKE_CURRENT_BINARY_DIR}/lexico.c
)

#COMPILACIÓN DE BISON
find_package(BISON)
BISON_TARGET(
    analizadorSintactico src/analizadores/analizador_sintactico.y ${CMAKE_CURRENT_BINARY_DIR}/sintactico.c
    COMPILE_FLAGS
        "--defines=include/auxiliares/definiciones.h --debug -Wall --report=solved"
)

ADD_FLEX_BISON_DEPENDENCY(analizadorLexico analizadorSintactico)


#OPCIONES COMPILACIÓN
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c11 -O3 -Wno-unused-parameter")


#FICHEROS QUE HACEN FALTA
set(
    HEADERS 
    include/analizadores/analizador_lexico.h
    include/analizadores/analizador_sintactico.h
    include/auxiliares/definiciones.h
    include/auxiliares/errores.h
    include/auxiliares/tabla_simbolos.h
    include/auxiliares/funciones.h
    include/auxiliares/uthash.h
)

set(
    SOURCES
    ${FLEX_analizadorLexico_OUTPUTS}
    ${BISON_analizadorSintactico_OUTPUTS}
    src/auxiliares/errores.c
    src/auxiliares/tabla_simbolos.c
    src/auxiliares/funciones.c
    src/main.c
)


#EJECUTABLE
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} m dl)

add_library(dl_radians SHARED src/dl_radians.c)
