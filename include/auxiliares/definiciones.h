/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_INCLUDE_AUXILIARES_DEFINICIONES_H_INCLUDED
# define YY_YY_INCLUDE_AUXILIARES_DEFINICIONES_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 4 "src/analizadores/analizador_sintactico.y"

    #include "analizadores/analizador_sintactico.h"
    #include "analizadores/analizador_lexico.h"
    #include "auxiliares/tabla_simbolos.h"
    #include "auxiliares/errores.h"
    #include "auxiliares/funciones.h"

    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    
    #define COLOR_VERDE   "\x1b[32m"
    #define COLOR_RESET   "\x1b[0m"

#line 63 "include/auxiliares/definiciones.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BLANCO = 258,
    INTEGER = 259,
    FLOAT = 260,
    ID = 261,
    FUNC = 262,
    ARCHIVO = 263,
    LIMITE_EOF = 264,
    OPERADOR_SUMA = 265,
    OPERADOR_RESTA = 266,
    OPERADOR_MULT = 267,
    OPERADOR_DIV = 268,
    SEPARADOR_PAR_IZQ = 269,
    SEPARADOR_PAR_DER = 270,
    NEG = 271,
    OPERADOR_EXP = 272,
    OPERADOR_IGUAL = 273,
    KW_HELP = 274,
    KW_QUIT = 275,
    KW_LOAD = 276,
    KW_WORKSPACE = 277,
    KW_CLEAR = 278,
    KW_IMPORT = 279
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "src/analizadores/analizador_sintactico.y"
 numero num; char *ptr; 

#line 102 "include/auxiliares/definiciones.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_AUXILIARES_DEFINICIONES_H_INCLUDED  */
