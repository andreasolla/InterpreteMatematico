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
    #include "auxiliares/tabla_simbolos.h"
    #include "auxiliares/errores.h"

    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    


#line 60 "include/auxiliares/definiciones.h"

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
    OPERADOR_SUMA = 263,
    OPERADOR_RESTA = 264,
    OPERADOR_MULT = 265,
    OPERADOR_DIV = 266,
    SEPARADOR_PAR_IZQ = 267,
    SEPARADOR_PAR_DER = 268,
    NEG = 269,
    OPERADOR_EXP = 270,
    OPERADOR_IGUAL = 271
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 16 "src/analizadores/analizador_sintactico.y"
 numero num; char *ptr; 

#line 91 "include/auxiliares/definiciones.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_AUXILIARES_DEFINICIONES_H_INCLUDED  */
