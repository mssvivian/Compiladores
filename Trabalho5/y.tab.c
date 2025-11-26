/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "mini_js.y"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>

using namespace std;

int linha = 1, coluna = 0; 

struct Atributos {
  vector<string> c; // Código - vazio para LVALUEPROP
  string endereco_funcao;
  int linha = 0, coluna = 0;

  vector<string> esq ; //usado para LVALUEPROP para separar E[E]
  vector<string> dir ; //usado para LVALUEPROP sempre variaveis temporarias
  // Só para argumentos e parâmetros
  int n_args = 0;     

  int contador = 0; //contar parametros e argumentos
  
  // Só para valor default de argumento        
  vector<string> valor_default;
  
  // Para funções anônimas e arrow functions
  bool eh_lambda = false;  // true se for arrow function com expressão (sem bloco)
  vector<string> captura; // variáveis capturadas (nomes)
  set<string> parametros_funcao; // parâmetros da função para detecção de captura
  set<string> variaveis_locais; // variáveis locais da função (não precisam captura)

  void clear() {
    c.clear();
    valor_default.clear();
    linha = 0;
    coluna = 0;
    n_args = 0;
    eh_lambda = false;
    captura.clear();
    parametros_funcao.clear();
    variaveis_locais.clear();
  }
};

enum TipoDecl { Let = 1, Const, Var };

struct Simbolo {
  TipoDecl tipo;
  int linha;
  int coluna;
};


// Tabela de símbolos - agora é uma pilha
vector< map< string, Simbolo > > ts = { map< string, Simbolo >{} }; 
vector<string> funcoes;

vector<int> blocos_alinhados_em_funcao = {0};


Atributos declara_var( TipoDecl tipo, Atributos atrib );
void checa_simbolo( string nome, bool modificavel );

Simbolo* busca_simbolo( string nome );

#define YYSTYPE Atributos

extern "C" int yylex();
int yyparse();
void yyerror(const char *);

vector<string> concatena( vector<string> a, vector<string> b ) {
  a.insert( a.end(), b.begin(), b.end() );
  return a;
}

vector<string> operator+( vector<string> a, vector<string> b ) {
  return concatena( a, b );
}

vector<string> operator+( vector<string> a, string b ) {
  a.push_back( b );
  return a;
}

vector<string> operator+( string a, vector<string> b ) {
  return vector<string>{ a } + b;
}

// Concatena dois vetores
vector<string>& operator+=(vector<string>& a, const vector<string>& b) {
  a.insert(a.end(), b.begin(), b.end());
  return a;
}

// Adiciona uma string ao vetor
vector<string>& operator+=(vector<string>& a, const string& b) {
  a.push_back(b);
  return a;
}


vector<string> resolve_enderecos( vector<string> entrada ) {
  map<string,int> label;
  vector<string> saida;
  for( int i = 0; i < entrada.size(); i++ ) 
    if( entrada[i][0] == ':' ) 
        label[entrada[i].substr(1)] = saida.size();
    else
      saida.push_back( entrada[i] );
  
  for( int i = 0; i < saida.size(); i++ ) 
    if( label.count( saida[i] ) > 0 )
        saida[i] = to_string(label[saida[i]]);
    
  return saida;
}

string gera_label( string prefixo ) {
  static int n = 0;
  return prefixo + "_" + to_string( ++n ) + ":";
}

string define_label( string label) {
  return ":" + label;
}

void print( vector<string> codigo ) {
  for( string s : codigo )
    cout << s << " ";
    
  cout << endl;  
}

string gera_temp(string prefixo) {
  static int n = 0;
  return "temp_" + prefixo + to_string( ++n );
}

vector<string> to_inst(string st){
    return vector<string>{st};
}

// Detecta variáveis capturadas (usadas mas não são parâmetros nem variáveis locais)
set<string> detecta_capturas(set<string>& ids_usados, set<string>& parametros, set<string>& locais) {
  set<string> capturas;
  for(const string& id : ids_usados) {
    // Se não é parâmetro e não é variável local, deve ser capturada
    if(parametros.find(id) == parametros.end() && locais.find(id) == locais.end()) {
      capturas.insert(id);
    }
  }
  return capturas;
}

// Extrai nomes de variáveis de um vetor de código
set<string> extrai_ids_de_codigo(vector<string>& codigo) {
  set<string> ids;
  for(const string& s : codigo) {
    // Simples heurística: se começa com letra ou _, é um possível ID
    if(!s.empty() && (isalpha(s[0]) || s[0] == '_') && s[0] != '\'' && !isdigit(s[0])) {
      // Ignora keywords conhecidas
      if(s != "@" && s != "=" && s != "@" && s != "^" && s != "$" && s != "[=]" && s != "[<=]" && 
         s != "[@]" && s != "&" && s != "~" && s != "#" && s != "?" && s != "!" && 
         s != "+" && s != "-" && s != "*" && s != "/" && s != "%" && s != "==" && s != "!=" &&
         s != "<" && s != ">" && s != "<=" && s != ">=" && s != "<{" && s != "}>" && s != "{}") {
        ids.insert(s);
      }
    }
  }
  return ids;
}

void empilha_escopo_novo();
void desempilha_escopo();

#line 250 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    IF = 259,                      /* IF  */
    ELSE = 260,                    /* ELSE  */
    LET = 261,                     /* LET  */
    CONST = 262,                   /* CONST  */
    VAR = 263,                     /* VAR  */
    FOR = 264,                     /* FOR  */
    WHILE = 265,                   /* WHILE  */
    FUNCTION = 266,                /* FUNCTION  */
    RETURN = 267,                  /* RETURN  */
    CDOUBLE = 268,                 /* CDOUBLE  */
    CSTRING = 269,                 /* CSTRING  */
    CINT = 270,                    /* CINT  */
    CBOOL = 271,                   /* CBOOL  */
    AND = 272,                     /* AND  */
    OR = 273,                      /* OR  */
    ME_IG = 274,                   /* ME_IG  */
    MA_IG = 275,                   /* MA_IG  */
    DIF = 276,                     /* DIF  */
    IGUAL = 277,                   /* IGUAL  */
    MAIS_IGUAL = 278,              /* MAIS_IGUAL  */
    MENOS_IGUAL = 279,             /* MENOS_IGUAL  */
    MAIS_MAIS = 280,               /* MAIS_MAIS  */
    MENOS_MENOS = 281,             /* MENOS_MENOS  */
    SETA = 282,                    /* SETA  */
    ASM = 283,                     /* ASM  */
    FECHA_PARENTESES_LAMBDA = 284  /* FECHA_PARENTESES_LAMBDA  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ID 258
#define IF 259
#define ELSE 260
#define LET 261
#define CONST 262
#define VAR 263
#define FOR 264
#define WHILE 265
#define FUNCTION 266
#define RETURN 267
#define CDOUBLE 268
#define CSTRING 269
#define CINT 270
#define CBOOL 271
#define AND 272
#define OR 273
#define ME_IG 274
#define MA_IG 275
#define DIF 276
#define IGUAL 277
#define MAIS_IGUAL 278
#define MENOS_IGUAL 279
#define MAIS_MAIS 280
#define MENOS_MENOS 281
#define SETA 282
#define ASM 283
#define FECHA_PARENTESES_LAMBDA 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_IF = 4,                         /* IF  */
  YYSYMBOL_ELSE = 5,                       /* ELSE  */
  YYSYMBOL_LET = 6,                        /* LET  */
  YYSYMBOL_CONST = 7,                      /* CONST  */
  YYSYMBOL_VAR = 8,                        /* VAR  */
  YYSYMBOL_FOR = 9,                        /* FOR  */
  YYSYMBOL_WHILE = 10,                     /* WHILE  */
  YYSYMBOL_FUNCTION = 11,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_CDOUBLE = 13,                   /* CDOUBLE  */
  YYSYMBOL_CSTRING = 14,                   /* CSTRING  */
  YYSYMBOL_CINT = 15,                      /* CINT  */
  YYSYMBOL_CBOOL = 16,                     /* CBOOL  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_ME_IG = 19,                     /* ME_IG  */
  YYSYMBOL_MA_IG = 20,                     /* MA_IG  */
  YYSYMBOL_DIF = 21,                       /* DIF  */
  YYSYMBOL_IGUAL = 22,                     /* IGUAL  */
  YYSYMBOL_MAIS_IGUAL = 23,                /* MAIS_IGUAL  */
  YYSYMBOL_MENOS_IGUAL = 24,               /* MENOS_IGUAL  */
  YYSYMBOL_MAIS_MAIS = 25,                 /* MAIS_MAIS  */
  YYSYMBOL_MENOS_MENOS = 26,               /* MENOS_MENOS  */
  YYSYMBOL_SETA = 27,                      /* SETA  */
  YYSYMBOL_ASM = 28,                       /* ASM  */
  YYSYMBOL_FECHA_PARENTESES_LAMBDA = 29,   /* FECHA_PARENTESES_LAMBDA  */
  YYSYMBOL_30_ = 30,                       /* ','  */
  YYSYMBOL_31_ = 31,                       /* '='  */
  YYSYMBOL_32_ = 32,                       /* ':'  */
  YYSYMBOL_33_ = 33,                       /* '<'  */
  YYSYMBOL_34_ = 34,                       /* '>'  */
  YYSYMBOL_35_ = 35,                       /* '+'  */
  YYSYMBOL_36_ = 36,                       /* '-'  */
  YYSYMBOL_37_ = 37,                       /* '*'  */
  YYSYMBOL_38_ = 38,                       /* '/'  */
  YYSYMBOL_39_ = 39,                       /* '%'  */
  YYSYMBOL_40_ = 40,                       /* '['  */
  YYSYMBOL_41_ = 41,                       /* '('  */
  YYSYMBOL_42_ = 42,                       /* '.'  */
  YYSYMBOL_43_ = 43,                       /* ';'  */
  YYSYMBOL_44_ = 44,                       /* '{'  */
  YYSYMBOL_45_ = 45,                       /* '}'  */
  YYSYMBOL_46_ = 46,                       /* ')'  */
  YYSYMBOL_47_ = 47,                       /* ']'  */
  YYSYMBOL_48_ = 48,                       /* '?'  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_S = 50,                         /* S  */
  YYSYMBOL_CMDs = 51,                      /* CMDs  */
  YYSYMBOL_CMD = 52,                       /* CMD  */
  YYSYMBOL_BLOCO_VAZIO = 53,               /* BLOCO_VAZIO  */
  YYSYMBOL_BLOCO = 54,                     /* BLOCO  */
  YYSYMBOL_START_BLOCO = 55,               /* START_BLOCO  */
  YYSYMBOL_END_BLOCO = 56,                 /* END_BLOCO  */
  YYSYMBOL_CMD_RET = 57,                   /* CMD_RET  */
  YYSYMBOL_CMD_ASM = 58,                   /* CMD_ASM  */
  YYSYMBOL_EMPILHA_TS_FUNC = 59,           /* EMPILHA_TS_FUNC  */
  YYSYMBOL_DESEMPILHA_TS_FUNC = 60,        /* DESEMPILHA_TS_FUNC  */
  YYSYMBOL_CMD_FUNC = 61,                  /* CMD_FUNC  */
  YYSYMBOL_NOME_FUNCAO = 62,               /* NOME_FUNCAO  */
  YYSYMBOL_LISTA_PARAMs = 63,              /* LISTA_PARAMs  */
  YYSYMBOL_PARAMs = 64,                    /* PARAMs  */
  YYSYMBOL_PARAM = 65,                     /* PARAM  */
  YYSYMBOL_CMD_FOR = 66,                   /* CMD_FOR  */
  YYSYMBOL_EF = 67,                        /* EF  */
  YYSYMBOL_SF = 68,                        /* SF  */
  YYSYMBOL_CMD_WHILE = 69,                 /* CMD_WHILE  */
  YYSYMBOL_CMD_LET = 70,                   /* CMD_LET  */
  YYSYMBOL_LET_VARs = 71,                  /* LET_VARs  */
  YYSYMBOL_LET_VAR = 72,                   /* LET_VAR  */
  YYSYMBOL_CMD_VAR = 73,                   /* CMD_VAR  */
  YYSYMBOL_VAR_VARs = 74,                  /* VAR_VARs  */
  YYSYMBOL_VAR_VAR = 75,                   /* VAR_VAR  */
  YYSYMBOL_CMD_CONST = 76,                 /* CMD_CONST  */
  YYSYMBOL_CONST_VARs = 77,                /* CONST_VARs  */
  YYSYMBOL_CONST_VAR = 78,                 /* CONST_VAR  */
  YYSYMBOL_CMD_IF = 79,                    /* CMD_IF  */
  YYSYMBOL_LVALUEPROP = 80,                /* LVALUEPROP  */
  YYSYMBOL_LIST = 81,                      /* LIST  */
  YYSYMBOL_LISTVALS = 82,                  /* LISTVALS  */
  YYSYMBOL_LISTVAL = 83,                   /* LISTVAL  */
  YYSYMBOL_ATRIB = 84,                     /* ATRIB  */
  YYSYMBOL_EOBJ = 85,                      /* EOBJ  */
  YYSYMBOL_EVG = 86,                       /* EVG  */
  YYSYMBOL_ECOND = 87,                     /* ECOND  */
  YYSYMBOL_CAMPOS = 88,                    /* CAMPOS  */
  YYSYMBOL_CAMPO = 89,                     /* CAMPO  */
  YYSYMBOL_E = 90,                         /* E  */
  YYSYMBOL_UN = 91,                        /* UN  */
  YYSYMBOL_F = 92,                         /* F  */
  YYSYMBOL_CHAMA_FUNC = 93,                /* CHAMA_FUNC  */
  YYSYMBOL_LISTA_ARGS = 94,                /* LISTA_ARGS  */
  YYSYMBOL_ARGs = 95                       /* ARGs  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  97
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   531

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  233

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    39,     2,     2,
      41,    46,    37,    35,    30,    36,    42,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    32,    43,
      33,    31,    34,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    47,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   206,   206,   207,   210,   211,   215,   216,   217,   218,
     219,   222,   223,   224,   225,   226,   228,   229,   230,   234,
     238,   242,   244,   247,   256,   268,   271,   275,   279,   306,
     320,   321,   322,   325,   345,   366,   370,   377,   387,   389,
     392,   393,   394,   395,   398,   407,   410,   411,   414,   416,
     424,   427,   428,   431,   433,   441,   444,   445,   448,   450,
     458,   464,   473,   478,   483,   488,   494,   499,   506,   507,
     510,   511,   514,   518,   520,   522,   524,   526,   535,   544,
     545,   546,   547,   548,   557,   559,   562,   563,   566,   567,
     570,   571,   572,   575,   576,   579,   581,   583,   585,   587,
     589,   591,   593,   595,   597,   599,   603,   605,   607,   609,
     611,   619,   628,   639,   648,   651,   652,   653,   654,   655,
     656,   657,   659,   663,   666,   670,   675,   682,   687,   693,
     699,   704
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "IF", "ELSE",
  "LET", "CONST", "VAR", "FOR", "WHILE", "FUNCTION", "RETURN", "CDOUBLE",
  "CSTRING", "CINT", "CBOOL", "AND", "OR", "ME_IG", "MA_IG", "DIF",
  "IGUAL", "MAIS_IGUAL", "MENOS_IGUAL", "MAIS_MAIS", "MENOS_MENOS", "SETA",
  "ASM", "FECHA_PARENTESES_LAMBDA", "','", "'='", "':'", "'<'", "'>'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'['", "'('", "'.'", "';'", "'{'",
  "'}'", "')'", "']'", "'?'", "$accept", "S", "CMDs", "CMD", "BLOCO_VAZIO",
  "BLOCO", "START_BLOCO", "END_BLOCO", "CMD_RET", "CMD_ASM",
  "EMPILHA_TS_FUNC", "DESEMPILHA_TS_FUNC", "CMD_FUNC", "NOME_FUNCAO",
  "LISTA_PARAMs", "PARAMs", "PARAM", "CMD_FOR", "EF", "SF", "CMD_WHILE",
  "CMD_LET", "LET_VARs", "LET_VAR", "CMD_VAR", "VAR_VARs", "VAR_VAR",
  "CMD_CONST", "CONST_VARs", "CONST_VAR", "CMD_IF", "LVALUEPROP", "LIST",
  "LISTVALS", "LISTVAL", "ATRIB", "EOBJ", "EVG", "ECOND", "CAMPOS",
  "CAMPO", "E", "UN", "F", "CHAMA_FUNC", "LISTA_ARGS", "ARGs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-108)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-90)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     292,   246,   -16,    33,    46,    59,    22,    23,    68,   372,
    -108,  -108,  -108,  -108,   308,   415,   444,   444,   161,   401,
    -108,     3,    72,   292,  -108,  -108,  -108,    30,    31,  -108,
    -108,  -108,    34,    38,    39,  -108,   475,  -108,    40,    57,
      50,   474,  -108,    15,  -108,   408,   408,  -108,  -108,    53,
     408,   326,   408,    96,   372,    70,  -108,    74,    75,  -108,
      90,    76,  -108,    93,   365,   372,  -108,    83,    19,   408,
      77,   474,   -27,    12,    15,   -27,    12,   -27,    12,    15,
      15,  -108,    78,   105,   474,   463,   112,   115,  -108,   116,
     102,     4,   110,  -108,   292,   104,   120,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,   408,   408,  -108,  -108,   408,   326,
     408,   148,  -108,  -108,   326,   372,   372,   372,   372,   372,
     372,   372,   372,   326,   160,  -108,  -108,  -108,  -108,  -108,
    -108,   164,  -108,   124,  -108,   126,   143,  -108,   122,   326,
      33,   372,    46,   372,    59,  -108,   136,  -108,  -108,  -108,
    -108,   430,  -108,  -108,   372,   326,   154,   179,   408,  -108,
     142,   326,   206,  -108,   164,  -108,  -108,  -108,   141,   145,
    -108,   157,   492,   191,   191,   -20,   -20,  -108,  -108,  -108,
     146,  -108,  -108,  -108,   408,   292,  -108,  -108,   474,  -108,
     474,  -108,   372,   292,   179,  -108,   -13,  -108,    53,   163,
    -108,  -108,   408,  -108,  -108,  -108,  -108,  -108,   326,  -108,
    -108,   187,   201,  -108,   149,  -108,  -108,   326,   165,  -108,
    -108,   292,   408,   156,  -108,  -108,   197,   292,   292,   249,
    -108,  -108,  -108
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    95,     0,     0,     0,     0,     0,     0,     0,    23,
     115,   117,   116,   118,     0,     0,     0,     0,     0,    32,
      18,    21,     0,     2,     5,    17,    16,     0,     0,    10,
      11,    12,     0,     0,     0,     9,    96,   120,    89,     0,
      85,    83,   105,   114,   123,     0,     0,   108,   109,     0,
       0,     0,   129,     0,     0,    48,    45,    47,    58,    55,
      57,    53,    50,    52,    39,     0,    29,     0,    95,     0,
      96,    24,   106,   110,     0,   107,   111,     0,     0,   122,
     121,    69,     0,    71,    72,    95,     0,    30,    34,    87,
       0,    83,    94,    19,     0,     0,    91,     1,     4,    13,
      14,     6,     7,     8,     0,     0,   112,   113,     0,     0,
     129,     0,    15,    25,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    21,    79,    80,
      73,    92,    89,     0,   130,     0,   127,    67,     0,     0,
       0,     0,     0,     0,     0,    43,     0,    40,    41,    42,
      38,     0,    26,    68,     0,     0,     0,    31,     0,   119,
       0,     0,     0,    84,    92,    77,    78,    76,     0,     0,
      63,     0,    97,    98,    99,   100,   101,   102,   103,   104,
       0,    65,    66,   124,   128,     0,    49,    46,    59,    56,
      54,    51,     0,     0,    32,    70,    73,    36,     0,    35,
      33,    86,   129,    93,    22,    90,    62,   125,     0,    64,
     131,    60,     0,    44,     0,    82,    81,     0,     0,    20,
      88,     0,    39,     0,   126,    61,     0,     0,     0,     0,
      37,    27,    28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -108,  -108,   -89,   -23,  -108,   -45,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,    13,  -108,    88,  -108,    26,  -108,
    -108,   169,   114,  -108,   202,   123,  -108,   204,   134,  -108,
    -108,    -7,  -108,   125,  -108,   -18,   -39,   133,  -108,   117,
    -108,    11,  -108,     7,  -108,  -107,  -108
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    22,    23,    24,    25,    26,    94,   219,    27,    28,
     194,   232,    29,    67,    86,    87,    88,    30,   145,   146,
      31,    32,    56,    57,    33,    62,    63,    34,    59,    60,
      35,    36,    37,    82,    83,    38,    39,    90,    40,    95,
      96,    41,    42,    43,    44,   135,   136
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      98,    89,    70,   169,   128,   162,    92,    73,    76,    78,
      78,    70,   133,    51,    52,    53,   -89,   120,   121,   122,
      71,    74,    74,    79,    80,    54,   115,   125,   126,    84,
      91,   129,   130,   132,   134,   -89,    55,   116,   117,   118,
     119,   120,   121,   122,    47,    48,   150,    70,    93,    58,
     160,    89,   109,   110,   111,   123,     1,   124,    70,    51,
      52,    53,    61,    64,    65,   138,    10,    11,    12,    13,
     168,    66,    97,    99,   100,   171,   151,   101,    14,    15,
      91,   102,   103,   112,   180,   113,   165,   166,    16,    17,
     167,   132,   134,    18,    19,   218,   132,   127,   114,   137,
     186,   139,   106,   107,   140,   132,   141,   143,    70,    70,
      70,    70,    70,    70,    70,    70,   197,   109,   110,   111,
     142,   132,   203,   144,   152,   153,   172,   173,   174,   175,
     176,   177,   178,   179,    70,   154,    70,   196,   229,    98,
      89,   156,   161,   132,   115,   157,   158,    70,   159,   163,
     164,   170,   188,   215,   190,   116,   117,   118,   119,   120,
     121,   122,   211,   181,    68,    84,   210,    92,   185,   220,
     213,   182,   183,   184,    10,    11,    12,    13,   197,   192,
     216,   198,   199,   202,   134,    70,    14,    15,   206,   208,
     132,   207,   221,   209,   217,   223,    16,    17,   225,   132,
     227,    18,    69,   212,   150,   230,    98,   214,    81,     1,
       2,   224,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,   115,   -90,   -90,   118,   119,   120,   121,
     122,    14,    15,   147,   116,   117,   118,   119,   120,   121,
     122,    16,    17,   228,   222,   200,    18,    19,   226,    20,
      21,   204,     1,     2,   187,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,   148,   191,   149,    45,
      46,    47,    48,    49,    14,    15,   189,    50,     0,   195,
       0,   205,     0,     0,    16,    17,    51,    52,    53,    18,
      19,   201,    20,    21,   231,     1,     2,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,    72,     0,     0,     0,     0,     0,    14,    15,     0,
       0,    10,    11,    12,    13,     0,     0,    16,    17,     1,
       0,     0,    18,    19,     0,    20,    21,     0,     0,    10,
      11,    12,    13,    16,    17,     0,     0,     0,    18,    69,
       0,    14,    15,     0,     0,     0,     0,     0,     0,     0,
       0,    16,    17,     0,     0,     0,    18,    19,     1,     0,
     131,     3,     4,     5,     0,    68,     0,     0,    10,    11,
      12,    13,     0,     0,     0,    10,    11,    12,    13,     0,
      14,    15,     0,     0,     0,     0,     0,    14,    15,     0,
      16,    17,     0,     0,    85,    18,    19,    16,    17,     0,
       0,     1,    18,    69,    10,    11,    12,    13,    75,     0,
       0,    10,    11,    12,    13,     0,    14,    15,    10,    11,
      12,    13,     0,    14,    15,     0,    16,    17,     0,     0,
       0,    18,    19,    16,    17,     0,     0,    77,    18,    19,
      16,    17,   115,     0,     0,    18,    69,    10,    11,    12,
      13,     0,     0,   116,   117,   118,   119,   120,   121,   122,
       0,     0,     0,     0,     0,     0,   193,     0,     0,    16,
      17,     0,     0,     0,    18,    69,    45,    46,    47,    48,
      49,     0,   -35,   -35,   155,     0,   115,     0,   104,   105,
     106,   107,     0,    51,    52,    53,   108,   116,   117,   118,
     119,   120,   121,   122,   -90,   109,   110,   111,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,   119,   120,
     121,   122
};

static const yytype_int16 yycheck[] =
{
      23,    19,     9,   110,    49,    94,     3,    14,    15,    16,
      17,    18,    51,    40,    41,    42,    29,    37,    38,    39,
       9,    14,    15,    16,    17,    41,    22,    45,    46,    18,
      19,    49,    50,    51,    52,    48,     3,    33,    34,    35,
      36,    37,    38,    39,    25,    26,    64,    54,    45,     3,
      46,    69,    40,    41,    42,    40,     3,    42,    65,    40,
      41,    42,     3,    41,    41,    54,    13,    14,    15,    16,
     109,     3,     0,    43,    43,   114,    65,    43,    25,    26,
      69,    43,    43,    43,   123,    28,   104,   105,    35,    36,
     108,   109,   110,    40,    41,   202,   114,    44,    48,     3,
     139,    31,    25,    26,    30,   123,    31,    31,   115,   116,
     117,   118,   119,   120,   121,   122,   155,    40,    41,    42,
      30,   139,   161,    30,    41,    47,   115,   116,   117,   118,
     119,   120,   121,   122,   141,    30,   143,   155,   227,   162,
     158,    29,    32,   161,    22,    30,    30,   154,    46,    45,
      30,     3,   141,   198,   143,    33,    34,    35,    36,    37,
      38,    39,   185,     3,     3,   154,   184,     3,    46,   208,
     193,    47,    46,    30,    13,    14,    15,    16,   217,    43,
     198,    27,     3,    41,   202,   192,    25,    26,    47,    32,
     208,    46,     5,    47,    31,    46,    35,    36,   221,   217,
      44,    40,    41,   192,   222,   228,   229,   194,    47,     3,
       4,    46,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    22,    33,    34,    35,    36,    37,    38,
      39,    25,    26,    64,    33,    34,    35,    36,    37,    38,
      39,    35,    36,    46,    43,   157,    40,    41,   222,    43,
      44,    45,     3,     4,   140,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    64,   144,    64,    23,
      24,    25,    26,    27,    25,    26,   142,    31,    -1,   154,
      -1,   164,    -1,    -1,    35,    36,    40,    41,    42,    40,
      41,   158,    43,    44,    45,     3,     4,    -1,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,    35,    36,     3,
      -1,    -1,    40,    41,    -1,    43,    44,    -1,    -1,    13,
      14,    15,    16,    35,    36,    -1,    -1,    -1,    40,    41,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    -1,    -1,    40,    41,     3,    -1,
      44,     6,     7,     8,    -1,     3,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,    -1,    13,    14,    15,    16,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      35,    36,    -1,    -1,     3,    40,    41,    35,    36,    -1,
      -1,     3,    40,    41,    13,    14,    15,    16,     3,    -1,
      -1,    13,    14,    15,    16,    -1,    25,    26,    13,    14,
      15,    16,    -1,    25,    26,    -1,    35,    36,    -1,    -1,
      -1,    40,    41,    35,    36,    -1,    -1,     3,    40,    41,
      35,    36,    22,    -1,    -1,    40,    41,    13,    14,    15,
      16,    -1,    -1,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    35,
      36,    -1,    -1,    -1,    40,    41,    23,    24,    25,    26,
      27,    -1,    29,    30,    31,    -1,    22,    -1,    23,    24,
      25,    26,    -1,    40,    41,    42,    31,    33,    34,    35,
      36,    37,    38,    39,    22,    40,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,
      38,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    25,    26,    35,    36,    40,    41,
      43,    44,    50,    51,    52,    53,    54,    57,    58,    61,
      66,    69,    70,    73,    76,    79,    80,    81,    84,    85,
      87,    90,    91,    92,    93,    23,    24,    25,    26,    27,
      31,    40,    41,    42,    41,     3,    71,    72,     3,    77,
      78,     3,    74,    75,    41,    41,     3,    62,     3,    41,
      80,    90,     3,    80,    92,     3,    80,     3,    80,    92,
      92,    47,    82,    83,    90,     3,    63,    64,    65,    84,
      86,    90,     3,    45,    55,    88,    89,     0,    52,    43,
      43,    43,    43,    43,    23,    24,    25,    26,    31,    40,
      41,    42,    43,    28,    48,    22,    33,    34,    35,    36,
      37,    38,    39,    40,    42,    84,    84,    44,    54,    84,
      84,    44,    84,    85,    84,    94,    95,     3,    90,    31,
      30,    31,    30,    31,    30,    67,    68,    70,    73,    76,
      84,    90,    41,    47,    30,    31,    29,    30,    30,    46,
      46,    32,    51,    45,    30,    84,    84,    84,    85,    94,
       3,    85,    90,    90,    90,    90,    90,    90,    90,    90,
      85,     3,    47,    46,    30,    46,    85,    71,    90,    77,
      90,    74,    43,    46,    59,    82,    84,    85,    27,     3,
      65,    86,    41,    85,    45,    88,    47,    46,    32,    47,
      84,    52,    90,    52,    63,    54,    84,    31,    94,    56,
      85,     5,    43,    46,    46,    52,    67,    44,    46,    51,
      52,    45,    60
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    53,
      54,    55,    56,    57,    57,    58,    59,    60,    61,    62,
      63,    63,    63,    64,    64,    65,    65,    66,    67,    67,
      68,    68,    68,    68,    69,    70,    71,    71,    72,    72,
      73,    74,    74,    75,    75,    76,    77,    77,    78,    78,
      79,    79,    80,    80,    80,    80,    80,    80,    81,    81,
      82,    82,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    85,    85,    86,    86,    87,    87,
      88,    88,    88,    89,    89,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    93,    93,    93,    94,    94,    94,
      95,    95
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     2,
       5,     0,     0,     1,     2,     2,     0,     0,    10,     1,
       1,     2,     0,     3,     1,     1,     3,     9,     1,     0,
       1,     1,     1,     1,     5,     2,     3,     1,     1,     3,
       2,     3,     1,     1,     3,     2,     3,     1,     1,     3,
       5,     7,     4,     3,     4,     3,     4,     3,     3,     2,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     5,     1,     3,     1,     3,     1,     5,     1,
       3,     1,     0,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     3,
       1,     2,     2,     1,     4,     4,     6,     1,     2,     0,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* S: CMDs  */
#line 206 "mini_js.y"
         { print( resolve_enderecos( yyvsp[0].c + "." + funcoes ) ); }
#line 1650 "y.tab.c"
    break;

  case 3: /* S: %empty  */
#line 207 "mini_js.y"
    { yyval.clear(); }
#line 1656 "y.tab.c"
    break;

  case 4: /* CMDs: CMDs CMD  */
#line 210 "mini_js.y"
                 { yyval.c = yyvsp[-1].c + yyvsp[0].c; }
#line 1662 "y.tab.c"
    break;

  case 15: /* CMD: ATRIB ';'  */
#line 227 "mini_js.y"
      { yyval.c = yyvsp[-1].c + "^"; }
#line 1668 "y.tab.c"
    break;

  case 18: /* CMD: ';'  */
#line 231 "mini_js.y"
      { yyval.clear(); }
#line 1674 "y.tab.c"
    break;

  case 19: /* BLOCO_VAZIO: '{' '}'  */
#line 235 "mini_js.y"
            { yyval.c = vector<string>{"<{", "}>"}; }
#line 1680 "y.tab.c"
    break;

  case 20: /* BLOCO: '{' START_BLOCO CMDs '}' END_BLOCO  */
#line 239 "mini_js.y"
        { yyval.c = "<{" + yyvsp[-2].c + "}>" ; }
#line 1686 "y.tab.c"
    break;

  case 21: /* START_BLOCO: %empty  */
#line 242 "mini_js.y"
              {empilha_escopo_novo();}
#line 1692 "y.tab.c"
    break;

  case 22: /* END_BLOCO: %empty  */
#line 244 "mini_js.y"
            {desempilha_escopo();}
#line 1698 "y.tab.c"
    break;

  case 23: /* CMD_RET: RETURN  */
#line 248 "mini_js.y"
          { 
            yyval.c = vector<string>{"undefined"}; /* 1. Empilha undefined */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            yyval.c =  yyval.c + vector<string>{ "'&retorno'", "@", "~" }; /* 3. Instruções de retorno */
          }
#line 1711 "y.tab.c"
    break;

  case 24: /* CMD_RET: RETURN E  */
#line 257 "mini_js.y"
          { 
            yyval.c = yyvsp[0].c; /* 1. Empilha o valor da expressão E */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            /* 3. Instruções de retorno (NÃO adicione $2.c de novo) */
            yyval.c = yyval.c + vector<string>{ "'&retorno'", "@", "~" }; 
          }
#line 1725 "y.tab.c"
    break;

  case 25: /* CMD_ASM: EOBJ ASM  */
#line 268 "mini_js.y"
                   { yyval.c = yyvsp[-1].c + yyvsp[0].c + "^"; }
#line 1731 "y.tab.c"
    break;

  case 26: /* EMPILHA_TS_FUNC: %empty  */
#line 271 "mini_js.y"
                  { ts.push_back( map< string, Simbolo >{} ); 
                blocos_alinhados_em_funcao.push_back(0);}
#line 1738 "y.tab.c"
    break;

  case 27: /* DESEMPILHA_TS_FUNC: %empty  */
#line 275 "mini_js.y"
                     { ts.pop_back(); 
                      blocos_alinhados_em_funcao.pop_back();}
#line 1745 "y.tab.c"
    break;

  case 28: /* CMD_FUNC: FUNCTION NOME_FUNCAO '(' EMPILHA_TS_FUNC LISTA_PARAMs ')' '{' CMDs '}' DESEMPILHA_TS_FUNC  */
#line 280 "mini_js.y"
           { 
             /* Note que os índices mudaram: NOME_FUNCAO é $2, LISTA_PARAMs é $5, CMDs é $8 */
             string definicao_lbl_endereco_funcao = ":" + yyvsp[-8].endereco_funcao;
             
             yyval.c = yyvsp[-8].c ; /* Código de NOME_FUNCAO (declara var, atribui obj função) */

             funcoes = funcoes + definicao_lbl_endereco_funcao + yyvsp[-5].c + yyvsp[-2].c +
                         "undefined" + "@" + "'&retorno'" + "@"+ "~";
             
           }
#line 1760 "y.tab.c"
    break;

  case 29: /* NOME_FUNCAO: ID  */
#line 307 "mini_js.y"
            { 
              string endereco_funcao = gera_label( "func_" + yyvsp[0].c[0] );
              yyval.endereco_funcao = endereco_funcao; /* 1. Salva o label para CMD_FUNC usar */
              
              Atributos decl = declara_var( Var, yyvsp[0] ); // 2. Declara a variável
              
              /* 3. Gera código usando decl.c (que pode ou não ter '&') e a var local 'endereco_funcao' */
              yyval.c = decl.c +                                    /* 'foo &' ou nada se for redeclaração */
                     yyvsp[0].c + "{}" + "="  + "'&funcao'" + endereco_funcao +  /* 'foo '&funcao' func_foo_1' ... */
                     "[=]" + "^";                              /* ... '[=] ^' (atribui prop) */
            }
#line 1776 "y.tab.c"
    break;

  case 32: /* LISTA_PARAMs: %empty  */
#line 322 "mini_js.y"
             { yyval.clear(); }
#line 1782 "y.tab.c"
    break;

  case 33: /* PARAMs: PARAMs ',' PARAM  */
#line 326 "mini_js.y"
       { 
         Atributos decl = declara_var(Let, yyvsp[0]); // Declara o parâmetro no escopo da função
         
         string idx = to_string(yyvsp[-2].n_args); // índice do parâmetro
         string nome = yyvsp[0].c[0]; // nome da variável

         // Código base: associar argumento à variável
         yyval.c = yyvsp[-2].c + yyvsp[0].c + "&" + yyvsp[0].c + "arguments" + "@" + idx + "[@]" + "=" + "^" ;

         // Se tiver valor default, gerar código adicional
         if (yyvsp[0].valor_default.size() > 0) {
            string lbl_fim_default = gera_label( "fim_default_if" );
            yyval.c += yyvsp[0].c + "@" + "undefined" + "@" + "==" + "!" + lbl_fim_default + "?" + yyvsp[0].c +
                     yyvsp[0].valor_default + "=" + "^" +
                     define_label(lbl_fim_default);
         }

         yyval.n_args = yyvsp[-2].n_args + 1; 
       }
#line 1806 "y.tab.c"
    break;

  case 34: /* PARAMs: PARAM  */
#line 346 "mini_js.y"
       { 
         Atributos decl = declara_var(Let, yyvsp[0]); // Declara o parâmetro
         
         string nome = yyvsp[0].c[0];

         // Código base para o primeiro parâmetro
         yyval.c = decl.c + 
                yyvsp[0].c + "arguments" +  "@" + "0" + "[@]" + "=" + "^";

         if (yyvsp[0].valor_default.size() > 0) {
          string lbl_fim_default = gera_label( "fim_default_if" );
           yyval.c += yyvsp[0].c + "@" + "undefined" + "@" + "==" + "!" + lbl_fim_default + "?" + yyvsp[0].c +
                     yyvsp[0].valor_default + "=" + "^" +
                     define_label(lbl_fim_default);
         }

         yyval.n_args = 1; 
       }
#line 1829 "y.tab.c"
    break;

  case 35: /* PARAM: ID  */
#line 367 "mini_js.y"
      { yyval = yyvsp[0]; 
        yyval.n_args = 1;
        yyval.valor_default.clear(); }
#line 1837 "y.tab.c"
    break;

  case 36: /* PARAM: ID '=' EOBJ  */
#line 371 "mini_js.y"
      { 
        yyval = yyvsp[-2];
        yyval.n_args = 1;
        yyval.valor_default = yyvsp[0].c; }
#line 1846 "y.tab.c"
    break;

  case 37: /* CMD_FOR: FOR '(' SF ';' E ';' EF ')' CMD  */
#line 378 "mini_js.y"
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          yyval.c = yyvsp[-6].c + define_label (teste_for) +
                 yyvsp[-4].c + "!" + fim_for + "?" + yyvsp[0].c + 
                 yyvsp[-2].c + teste_for + "#" + define_label(fim_for);
        }
#line 1858 "y.tab.c"
    break;

  case 38: /* EF: ATRIB  */
#line 388 "mini_js.y"
      { yyval.c = yyvsp[0].c + "^"; }
#line 1864 "y.tab.c"
    break;

  case 39: /* EF: %empty  */
#line 389 "mini_js.y"
      { yyval.clear(); }
#line 1870 "y.tab.c"
    break;

  case 44: /* CMD_WHILE: WHILE '(' E ')' CMD  */
#line 399 "mini_js.y"
        { string inicio_while = gera_label( "inicio_while" );
          string fim_while = gera_label( "fim_while" );
          yyval.c = define_label(inicio_while) + 
                 yyvsp[-2].c + "!" + fim_while + "?" + yyvsp[0].c + 
                 inicio_while + "#" + define_label(fim_while);
        }
#line 1881 "y.tab.c"
    break;

  case 45: /* CMD_LET: LET LET_VARs  */
#line 407 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1887 "y.tab.c"
    break;

  case 46: /* LET_VARs: LET_VAR ',' LET_VARs  */
#line 410 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1893 "y.tab.c"
    break;

  case 48: /* LET_VAR: ID  */
#line 415 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[0] ).c; }
#line 1899 "y.tab.c"
    break;

  case 49: /* LET_VAR: ID '=' EOBJ  */
#line 417 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1905 "y.tab.c"
    break;

  case 50: /* CMD_VAR: VAR VAR_VARs  */
#line 424 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1911 "y.tab.c"
    break;

  case 51: /* VAR_VARs: VAR_VAR ',' VAR_VARs  */
#line 427 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1917 "y.tab.c"
    break;

  case 53: /* VAR_VAR: ID  */
#line 432 "mini_js.y"
          { yyval.c = declara_var( Var, yyvsp[0] ).c; }
#line 1923 "y.tab.c"
    break;

  case 54: /* VAR_VAR: ID '=' E  */
#line 434 "mini_js.y"
          {  yyval.c = declara_var( Var, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1929 "y.tab.c"
    break;

  case 55: /* CMD_CONST: CONST CONST_VARs  */
#line 441 "mini_js.y"
                            { yyval.c = yyvsp[0].c; }
#line 1935 "y.tab.c"
    break;

  case 56: /* CONST_VARs: CONST_VAR ',' CONST_VARs  */
#line 444 "mini_js.y"
                                      { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1941 "y.tab.c"
    break;

  case 58: /* CONST_VAR: ID  */
#line 449 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[0] ).c; }
#line 1947 "y.tab.c"
    break;

  case 59: /* CONST_VAR: ID '=' E  */
#line 451 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1953 "y.tab.c"
    break;

  case 60: /* CMD_IF: IF '(' E ')' CMD  */
#line 459 "mini_js.y"
          {
            string fim_if = gera_label( "fim_if" );
            yyval.c = yyvsp[-2].c + "!" + fim_if + "?" + yyvsp[0].c + define_label(fim_if);
          }
#line 1962 "y.tab.c"
    break;

  case 61: /* CMD_IF: IF '(' E ')' CMD ELSE CMD  */
#line 465 "mini_js.y"
          {  string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            yyval.c = yyvsp[-4].c + "!" + else_if + "?" + yyvsp[-2].c + fim_if + 
            "#" + define_label(else_if) + yyvsp[0].c +  define_label(fim_if);
          }
#line 1972 "y.tab.c"
    break;

  case 62: /* LVALUEPROP: LVALUEPROP '[' EOBJ ']'  */
#line 474 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].esq + yyvsp[-3].dir + "[@]";
              yyval.dir = yyvsp[-1].c;
               }
#line 1981 "y.tab.c"
    break;

  case 63: /* LVALUEPROP: LVALUEPROP '.' ID  */
#line 479 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].esq + yyvsp[-2].dir + "[@]";
              yyval.dir = yyvsp[0].c;
               }
#line 1990 "y.tab.c"
    break;

  case 64: /* LVALUEPROP: F '[' EOBJ ']'  */
#line 484 "mini_js.y"
             { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].c;
              yyval.dir = yyvsp[-1].c;
               }
#line 1999 "y.tab.c"
    break;

  case 65: /* LVALUEPROP: F '.' ID  */
#line 489 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].c;
              yyval.dir = yyvsp[0].c;
              }
#line 2008 "y.tab.c"
    break;

  case 66: /* LVALUEPROP: ID '[' EOBJ ']'  */
#line 495 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].c + "@";
              yyval.dir = yyvsp[-1].c;
              }
#line 2017 "y.tab.c"
    break;

  case 67: /* LVALUEPROP: ID '.' ID  */
#line 500 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].c + "@";
              yyval.dir = yyvsp[0].c;
               }
#line 2026 "y.tab.c"
    break;

  case 68: /* LIST: '[' LISTVALS ']'  */
#line 506 "mini_js.y"
                         { yyval.c = yyvsp[-2].c + yyvsp[-1].c + yyvsp[0].c; }
#line 2032 "y.tab.c"
    break;

  case 69: /* LIST: '[' ']'  */
#line 507 "mini_js.y"
                { yyval.c = vector<string>{"[]"}; }
#line 2038 "y.tab.c"
    break;

  case 70: /* LISTVALS: LISTVAL ',' LISTVALS  */
#line 510 "mini_js.y"
                                  { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 2044 "y.tab.c"
    break;

  case 73: /* ATRIB: ID '=' ATRIB  */
#line 519 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[0].c + "="; }
#line 2050 "y.tab.c"
    break;

  case 74: /* ATRIB: ID MAIS_IGUAL ATRIB  */
#line 521 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "+" + "="; }
#line 2056 "y.tab.c"
    break;

  case 75: /* ATRIB: ID MENOS_IGUAL ATRIB  */
#line 523 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "-" + "="; }
#line 2062 "y.tab.c"
    break;

  case 76: /* ATRIB: LVALUEPROP '=' ATRIB  */
#line 525 "mini_js.y"
    { yyval.c = yyvsp[-2].esq + yyvsp[-2].dir + yyvsp[0].c + "[=]"; }
#line 2068 "y.tab.c"
    break;

  case 77: /* ATRIB: LVALUEPROP MAIS_IGUAL ATRIB  */
#line 527 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[-2].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[-2].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  yyvsp[0].c + "+" + "[=]" ;}
#line 2081 "y.tab.c"
    break;

  case 78: /* ATRIB: LVALUEPROP MENOS_IGUAL ATRIB  */
#line 536 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[-2].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[-2].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  yyvsp[0].c + "-" + "[=]" ;}
#line 2094 "y.tab.c"
    break;

  case 84: /* EOBJ: '{' CAMPOS '}'  */
#line 558 "mini_js.y"
    { yyval.c = vector<string>{"{}"}; }
#line 2100 "y.tab.c"
    break;

  case 95: /* E: ID  */
#line 580 "mini_js.y"
    { yyval.c = yyvsp[0].c + "@"; }
#line 2106 "y.tab.c"
    break;

  case 96: /* E: LVALUEPROP  */
#line 582 "mini_js.y"
    { yyval.c = yyvsp[0].esq + yyvsp[0].dir + "[@]"; }
#line 2112 "y.tab.c"
    break;

  case 97: /* E: E IGUAL E  */
#line 584 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c;}
#line 2118 "y.tab.c"
    break;

  case 98: /* E: E '<' E  */
#line 586 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2124 "y.tab.c"
    break;

  case 99: /* E: E '>' E  */
#line 588 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2130 "y.tab.c"
    break;

  case 100: /* E: E '+' E  */
#line 590 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2136 "y.tab.c"
    break;

  case 101: /* E: E '-' E  */
#line 592 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2142 "y.tab.c"
    break;

  case 102: /* E: E '*' E  */
#line 594 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2148 "y.tab.c"
    break;

  case 103: /* E: E '/' E  */
#line 596 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2154 "y.tab.c"
    break;

  case 104: /* E: E '%' E  */
#line 598 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2160 "y.tab.c"
    break;

  case 105: /* E: UN  */
#line 600 "mini_js.y"
    { yyval.c = yyvsp[0].c; }
#line 2166 "y.tab.c"
    break;

  case 106: /* UN: MAIS_MAIS ID  */
#line 604 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "+" + "=";}
#line 2172 "y.tab.c"
    break;

  case 107: /* UN: MENOS_MENOS ID  */
#line 606 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "-" + "=";}
#line 2178 "y.tab.c"
    break;

  case 108: /* UN: ID MAIS_MAIS  */
#line 608 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "+" + "=" + "^";}
#line 2184 "y.tab.c"
    break;

  case 109: /* UN: ID MENOS_MENOS  */
#line 610 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "-" + "=" + "^";}
#line 2190 "y.tab.c"
    break;

  case 110: /* UN: MAIS_MAIS LVALUEPROP  */
#line 612 "mini_js.y"
        { string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[0].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[0].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "+" + "[=]" ;}
#line 2202 "y.tab.c"
    break;

  case 111: /* UN: MENOS_MENOS LVALUEPROP  */
#line 620 "mini_js.y"
        {
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[0].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[0].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "-" + "[=]" ;}
#line 2215 "y.tab.c"
    break;

  case 112: /* UN: LVALUEPROP MAIS_MAIS  */
#line 629 "mini_js.y"
        {
         string esq = gera_temp("esq");
         string dir = gera_temp("dir");
         yyval.c = to_inst(esq) + "&" + esq + yyvsp[-1].esq + "=" + "^" +
                to_inst(dir) + "&" + dir + yyvsp[-1].dir + "=" + "^" +
                esq + "@" + dir + "@" + "[@]" +
                esq + "@" + dir + "@" +
                esq + "@" + dir + "@" + "[@]" +
                "1" + "+" + "[=]" + "^";
        }
#line 2230 "y.tab.c"
    break;

  case 113: /* UN: LVALUEPROP MENOS_MENOS  */
#line 640 "mini_js.y"
        {string esq = gera_temp("esq");
         string dir = gera_temp("dir");
         yyval.c = to_inst(esq) + "&" + esq + yyvsp[-1].esq + "=" + "^" +
                to_inst(dir) + "&" + dir + yyvsp[-1].dir + "=" + "^" +
                esq + "@" + dir + "@" + "[@]" +
                esq + "@" + dir + "@" +
                esq + "@" + dir + "@" + "[@]" +
                "1" + "-" + "[=]" + "^";}
#line 2243 "y.tab.c"
    break;

  case 119: /* F: '(' EVG ')'  */
#line 655 "mini_js.y"
                  { yyval.c = yyvsp[-1].c; }
#line 2249 "y.tab.c"
    break;

  case 121: /* F: '-' F  */
#line 658 "mini_js.y"
      { yyval.c = "0" + yyvsp[0].c + "-" ; }
#line 2255 "y.tab.c"
    break;

  case 122: /* F: '+' F  */
#line 660 "mini_js.y"
      { yyval.c = "0" + yyvsp[0].c + "+" ; }
#line 2261 "y.tab.c"
    break;

  case 124: /* CHAMA_FUNC: ID '(' LISTA_ARGS ')'  */
#line 667 "mini_js.y"
              { 
                yyval.c = yyvsp[-1].c + yyvsp[-3].c[0] + "@" + "$" ;
              }
#line 2269 "y.tab.c"
    break;

  case 125: /* CHAMA_FUNC: LVALUEPROP '(' LISTA_ARGS ')'  */
#line 671 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + yyvsp[-3].esq + yyvsp[-3].dir + "[@]" + "$" ;
              }
#line 2278 "y.tab.c"
    break;

  case 126: /* CHAMA_FUNC: '(' E ')' '(' LISTA_ARGS ')'  */
#line 676 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + yyvsp[-4].c + "$" ;
              }
#line 2287 "y.tab.c"
    break;

  case 127: /* LISTA_ARGS: ARGs  */
#line 683 "mini_js.y"
           { 
             yyval.c = yyvsp[0].c + to_string( yyvsp[0].n_args ); 
             yyval.n_args = yyvsp[0].n_args;
           }
#line 2296 "y.tab.c"
    break;

  case 128: /* LISTA_ARGS: ARGs ','  */
#line 688 "mini_js.y"
           {
             yyval.c = yyvsp[-1].c + to_string( yyvsp[-1].n_args ); 
             yyval.n_args = yyvsp[-1].n_args;
           }
#line 2305 "y.tab.c"
    break;

  case 129: /* LISTA_ARGS: %empty  */
#line 693 "mini_js.y"
           { 
              yyval.c = vector<string>{"0"}; 
              yyval.n_args = 0; 
           }
#line 2314 "y.tab.c"
    break;

  case 130: /* ARGs: ATRIB  */
#line 700 "mini_js.y"
      { 
        yyval.c = yyvsp[0].c;     
        yyval.n_args = 1;   
      }
#line 2323 "y.tab.c"
    break;

  case 131: /* ARGs: ARGs ',' ATRIB  */
#line 705 "mini_js.y"
      { 
        yyval.c = yyvsp[-2].c + yyvsp[0].c; 
        yyval.n_args = yyvsp[-2].n_args + 1; 
      }
#line 2332 "y.tab.c"
    break;


#line 2336 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 712 "mini_js.y"


#include "lex.yy.c"

Simbolo* busca_simbolo( string nome ) {
    for( int i = ts.size() - 1; i >= 0; i-- ) {
        if( ts[i].count( nome ) > 0 ) {
            return &ts[i][nome];
        }
    }
    return nullptr;
}

void empilha_escopo_novo(){
    ts.push_back( map< string, Simbolo >{} ); 
    if (!blocos_alinhados_em_funcao.empty())
      blocos_alinhados_em_funcao.back()++;
}

void desempilha_escopo(){
    if (!ts.empty()){
      ts.pop_back();
      if (!blocos_alinhados_em_funcao.empty())
        blocos_alinhados_em_funcao.back()--;
    } else {
      cerr << "Erro: Não é permitido desempilhar o escopo global" << endl;
      exit(1);
    }
}


Atributos declara_var( TipoDecl tipo, Atributos atrib ) {
      
  string nome_var = atrib.c[0];
  string mensagem_erro;

  if (tipo == Var){
    if( ts.back().count(nome_var) > 0){
      if (ts.back()[nome_var].tipo != Var){
        yyerror("Variavel já declarada com const ou let");
      }
      else{//redefine var
        ts.back()[nome_var].linha = atrib.linha;
        ts.back()[nome_var].coluna = atrib.coluna;
        ts.back()[nome_var].tipo = tipo;
        atrib.c.pop_back();
      }
    }
    else{//declara var
    ts.back()[nome_var].linha = atrib.linha;
    ts.back()[nome_var].coluna = atrib.coluna;
    ts.back()[nome_var].tipo = tipo;
    atrib.c = atrib.c + "&"; // Gera código de alocação (ex: 'a &')
    }
    
  } 
  else if (ts.back().count(nome_var) > 0){ // let/const checa só escopo atual
    cerr << "Erro: a variável '" << nome_var << "' já foi declarada na linha " << to_string(ts.back()[nome_var].linha) << "." << endl;
    exit(1);
  }
  else{ //declara nova variável
    ts.back()[nome_var].linha = atrib.linha;
    ts.back()[nome_var].coluna = atrib.coluna;
    ts.back()[nome_var].tipo = tipo;
    atrib.c = atrib.c + "&"; 
  }
  return atrib;
}

void checa_simbolo( string nome, bool modificavel ) {
  for( int i = ts.size() - 1; i >= 0; i-- ) {  
    auto& atual = ts[i];
    
    if( atual.count( nome ) > 0 ) {
      if( modificavel && atual[nome].tipo == Const ) {
        cerr << "Variavel '" << nome << "' não pode ser modificada." << endl;
        exit( 1 );     
      }
      else 
        return;
    }
  }

  cerr << "Variavel '" << nome << "' não declarada." << endl;
  exit( 1 );     
}

void yyerror( const char* st ) {
  cerr << st << endl; 
   cerr << "Proximo a: " << yytext << endl;
   exit( 1 );
}

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}



