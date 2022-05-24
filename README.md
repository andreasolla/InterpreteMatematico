# Intérprete matemático

Este programa es un intérprete de expresiones de cálculo matemático, que ejecuta las instrucciones introducidas y muestra el resultado. Acepta números decimales tanto de tipo entero como punto flotante. Las instrucciones admitidas son operaciones básicas y funciones simples contenidas de librerías (es necesario importar las librerías que se quieran utilizar). 

La implementación está realizada en C, con el uso de la herramienta Flex para llevar a cabo el proceso de análisis léxico y Bison para el sintáctico-semántico.

## Guía de uso

### Funciones
Las funciones propias del intérprete son las siguientes:
- ``help``:
    imprime la ayuda.
    
- ``workspace``:
    muestra las variables almacenadas en el intérprete.

- ``clear``:
    elimina todas las variables, si se añade (<nombre_variable>), elimina la variable indicada.

- ``quit``:
    sale del programa.

- ``load("<archivo>")``:
    carga el archivo indicado y lo ejecuta línea a línea.

- ``import("<nombre>")``:
    importa la libreria indicada para tener disponibles sus funciones.


### Operaciones básicas
- ``+`` suma.
- ``-`` resta.
- ``*`` multiplicación.
- ``/`` división.
- ``^`` potencia.
- ``%`` módulo.

### Constantes almacenadas
- ``e`` 2.718282
- ``pi`` 3.141593

No se eliminan al vaciar el workspace

### Consideraciones
- Tras cada instrucción se muestra el resultado de la misma. Para evitar que se muestre el resultado, se debe añadir ``;`` al final de la línea.

- Las operaciones básicas con números enteros realizan el cálculo entero (para obtener el resultado flotante, se debe introducir alguno de los números flotante)
    por ejemplo:
  ```
    (╮°-°)╮┳━━┳ >> 4/3
    1

    (╮°-°)╮┳━━┳ >> 4/3.0 
    1.333333

    (╮°-°)╮┳━━┳ >> 4.0/3
    1.333333
  ```

### Ejemplo de uso 

Para importar las funciones de la librería matemática:

  ```
    (╮°-°)╮┳━━┳ >> import("libm.so.6")   

    (╮°-°)╮┳━━┳ >> sin(pi/2)
    1.000000
  ```

## Utilización
Para empezar, habrá que descargar o clonar el repositorio. Además, será necesario tener el siguiente software:

- Compilador de C (como gcc)
- Make
- CMake
- Flex
- Bison

gcc y make suelen estar preinstalados en cualquier distribución de linux común, pero es probable que sea necesario instalar cmake, flex o bison. Por ejemplo, en una distribución como Ubuntu: 

```
sudo apt install cmake
sudo apt install flex
sudo apt install bison
```

### Compilar y ejecutar
  
Se puede ejecutar el script compilar_y_ejecutar.sh, que se encarga de crear el directorio necesario, compilar y ejecutar. Cmake se encarga de compilar también los archivos .lex y .y.

Esto solo se debe hacer la primera vez que se quiera ejecutar, a partir de ahí llegará con situarse en la carpeta del proyecto, e introducir el comando:

``./build/ComInt``

