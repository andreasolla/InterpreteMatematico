# ComInt Practica 1 - analizador léxico

Será necesario tener el siguiente software:

- Compilador de C (como gcc)
- Make
- CMake
- Flex

gcc y make suelen estar preinstalados en cualquier distribución de linux común, pero es probable que sea necesario instalar cmake o flex. Por ejemplo, en una distribución como Ubuntu: 

`sudo apt install cmake`
`sudo apt install flex`

Una vez hecho esto, se puede ejecutar el script compilar_y_ejecutar.sh, que se encarga de crear el directorio necesario, compilar y ejecutar. Cmake se encarga de compilar también el archivo .lex.

Esto solo se debe hacer la primera vez que se quiera ejecutar, a partir de ahí llegará con situarse en la carpeta del proyecto, e introducir el comando:

`./build/ComInt`

