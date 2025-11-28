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
  YYSYMBOL_ECOND_OPT = 67,                 /* ECOND_OPT  */
  YYSYMBOL_EF = 68,                        /* EF  */
  YYSYMBOL_SF = 69,                        /* SF  */
  YYSYMBOL_CMD_WHILE = 70,                 /* CMD_WHILE  */
  YYSYMBOL_CMD_LET = 71,                   /* CMD_LET  */
  YYSYMBOL_LET_VARs = 72,                  /* LET_VARs  */
  YYSYMBOL_LET_VAR = 73,                   /* LET_VAR  */
  YYSYMBOL_CMD_VAR = 74,                   /* CMD_VAR  */
  YYSYMBOL_VAR_VARs = 75,                  /* VAR_VARs  */
  YYSYMBOL_VAR_VAR = 76,                   /* VAR_VAR  */
  YYSYMBOL_CMD_CONST = 77,                 /* CMD_CONST  */
  YYSYMBOL_CONST_VARs = 78,                /* CONST_VARs  */
  YYSYMBOL_CONST_VAR = 79,                 /* CONST_VAR  */
  YYSYMBOL_CMD_IF = 80,                    /* CMD_IF  */
  YYSYMBOL_LVALUEPROP = 81,                /* LVALUEPROP  */
  YYSYMBOL_LIST = 82,                      /* LIST  */
  YYSYMBOL_LISTVALS = 83,                  /* LISTVALS  */
  YYSYMBOL_LISTVAL = 84,                   /* LISTVAL  */
  YYSYMBOL_ATRIB = 85,                     /* ATRIB  */
  YYSYMBOL_EOBJ = 86,                      /* EOBJ  */
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
#define YYFINAL  94
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   549

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  130
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  228

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
     238,   242,   244,   247,   260,   276,   279,   283,   287,   314,
     328,   329,   330,   333,   353,   374,   378,   385,   395,   396,
     399,   401,   404,   405,   406,   407,   410,   419,   422,   423,
     426,   428,   436,   439,   440,   443,   445,   453,   456,   457,
     460,   462,   470,   476,   485,   490,   495,   500,   506,   511,
     518,   523,   526,   531,   538,   546,   548,   550,   552,   554,
     563,   572,   580,   588,   595,   602,   611,   618,   620,   628,
     629,   632,   634,   637,   639,   643,   645,   647,   649,   651,
     653,   655,   657,   659,   661,   663,   667,   669,   671,   673,
     675,   683,   692,   703,   712,   715,   716,   717,   718,   720,
     721,   723,   727,   730,   734,   739,   746,   751,   757,   763,
     768
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
  "LISTA_PARAMs", "PARAMs", "PARAM", "CMD_FOR", "ECOND_OPT", "EF", "SF",
  "CMD_WHILE", "CMD_LET", "LET_VARs", "LET_VAR", "CMD_VAR", "VAR_VARs",
  "VAR_VAR", "CMD_CONST", "CONST_VARs", "CONST_VAR", "CMD_IF",
  "LVALUEPROP", "LIST", "LISTVALS", "LISTVAL", "ATRIB", "EOBJ", "ECOND",
  "CAMPOS", "CAMPO", "E", "UN", "F", "CHAMA_FUNC", "LISTA_ARGS", "ARGs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-104)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-36)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     333,   493,   -34,     5,    11,    17,    -5,     6,    47,    19,
    -104,  -104,  -104,  -104,   349,   420,   449,   449,     2,    61,
    -104,     8,    51,   333,  -104,  -104,  -104,    13,    14,  -104,
    -104,  -104,    15,    18,    25,  -104,   287,  -104,    30,   192,
    -104,    -1,  -104,   413,   413,  -104,  -104,   154,   413,   367,
     413,    59,    19,    52,  -104,    55,    57,  -104,    60,    58,
    -104,    62,   406,    19,  -104,    41,   242,    19,   282,   240,
     -16,   -11,    -1,   -16,   -11,   -16,   -11,    -1,    -1,     3,
    -104,    48,    68,  -104,  -104,    56,   240,   500,    70,    73,
    -104,   435,  -104,   333,  -104,  -104,  -104,  -104,  -104,  -104,
    -104,   413,   413,  -104,  -104,   413,   367,   413,    97,  -104,
      19,  -104,    19,    19,    19,    19,    19,    19,    19,   367,
     103,  -104,  -104,  -104,  -104,  -104,  -104,    64,  -104,    63,
      77,  -104,   458,   367,     5,    19,    11,    19,    17,  -104,
      65,  -104,  -104,  -104,  -104,   476,  -104,    87,  -104,    75,
      91,  -104,   367,   367,   367,    95,   127,    90,   245,  -104,
    -104,  -104,    86,    88,  -104,   510,   170,   170,    42,    42,
    -104,  -104,  -104,    92,  -104,  -104,  -104,   413,   333,  -104,
    -104,   240,  -104,   240,  -104,    19,   333,   127,   367,  -104,
     137,  -104,   110,  -104,   154,   116,  -104,   413,  -104,  -104,
    -104,  -104,  -104,   145,   108,   240,  -104,   106,  -104,  -104,
     367,  -104,  -104,   107,  -104,   333,   413,   115,  -104,  -104,
    -104,   117,   333,   333,   290,  -104,  -104,  -104
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    95,     0,     0,     0,     0,     0,     0,     0,    23,
     115,   117,   116,   118,     0,     0,     0,     0,     0,    32,
      18,    21,     0,     2,     5,    17,    16,     0,     0,    10,
      11,    12,     0,     0,     0,     9,    96,   119,     0,    85,
     105,   114,   122,     0,     0,   108,   109,     0,     0,     0,
     128,     0,     0,    50,    47,    49,    60,    57,    59,    55,
      52,    54,    41,     0,    29,     0,    95,     0,    96,    24,
     106,   110,     0,   107,   111,     0,     0,   121,   120,     0,
      71,     0,    73,    90,    74,    88,    85,    95,     0,    30,
      34,     0,    19,     0,     1,     4,    13,    14,     6,     7,
       8,     0,     0,   112,   113,     0,     0,   128,     0,    15,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,    77,    21,    81,    82,    75,     0,   129,     0,
     126,    69,     0,     0,     0,     0,     0,     0,     0,    45,
       0,    42,    43,    44,    40,     0,    26,    94,    87,     0,
      92,    70,     0,     0,     0,     0,    31,     0,     0,    79,
      80,    78,     0,     0,    65,    97,    98,    99,   100,   101,
     102,   103,   104,     0,    67,    68,   123,   127,     0,    51,
      48,    61,    58,    56,    53,    39,     0,    32,     0,    86,
       0,    72,     0,    36,     0,    35,    33,   128,    22,    64,
     124,    66,   130,    62,     0,    38,    46,     0,    93,    91,
       0,    84,    83,     0,    20,     0,    41,     0,    89,   125,
      63,     0,     0,     0,     0,    37,    27,    28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -104,  -104,   -90,   -22,  -104,   -45,  -104,  -104,  -104,  -104,
    -104,  -104,  -104,  -104,   -33,  -104,     9,  -104,  -104,   -55,
    -104,  -104,   100,    32,  -104,   109,    34,  -104,   111,    76,
    -104,  -104,   129,  -104,    31,  -104,    22,   -28,  -104,   -13,
    -104,     0,  -104,    -4,  -104,  -103,  -104
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    22,    23,    24,    25,    26,    93,   214,    27,    28,
     187,   227,    29,    65,    88,    89,    90,    30,   204,   139,
     140,    31,    32,    54,    55,    33,    60,    61,    34,    57,
      58,    35,    36,    37,    81,    82,    38,    84,    85,   149,
     150,    86,    40,    41,    42,   129,   130
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,    95,   124,   158,   163,     1,   147,    52,    53,    69,
      72,    72,    77,    78,    56,    10,    11,    12,    13,    91,
      59,   127,    66,    39,    49,    50,    51,    14,    15,   106,
     107,   108,    10,    11,    12,    13,    62,    16,    17,   119,
      83,   120,    18,    19,    14,    15,    79,    63,   148,    80,
      64,    94,   132,    92,    16,    17,    96,    97,    98,    18,
      67,    99,   131,   145,    87,   121,   122,    91,   100,   125,
     126,    83,   128,   109,    10,    11,    12,    13,   162,   116,
     117,   118,   146,   133,   144,   134,    14,    15,   135,   137,
     136,   173,   138,    39,   213,   151,    16,    17,   152,   155,
     164,    18,    67,   156,   153,   179,   174,   177,   185,   176,
     165,   175,   166,   167,   168,   169,   170,   171,   172,   188,
     189,   190,   194,   159,   160,   192,   193,   161,    83,   128,
     195,   197,   224,   199,   200,   181,    95,   183,    68,   201,
     147,    83,   210,    71,    74,    76,    76,   154,    68,   211,
     215,   216,   217,   219,   207,    83,   203,     1,    39,   222,
     208,   221,   141,   223,   206,   196,   180,    10,    11,    12,
      13,   142,   184,   143,    83,    83,    83,   209,    39,    14,
      15,    68,   218,   191,     0,   205,    39,     0,     0,    16,
      17,     0,    68,   220,    18,    19,    68,     0,   123,   202,
       0,   225,    95,   -36,   -36,   114,   115,   116,   117,   118,
      83,     0,   182,     0,   110,    39,   212,     0,     0,   128,
     111,     0,    39,    39,    39,   112,   113,   114,   115,   116,
     117,   118,    83,     0,     0,     0,     0,     0,   144,    68,
       0,    68,    68,    68,    68,    68,    68,    68,     1,     2,
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,   110,     0,    68,     0,    68,    45,    46,     0,
      14,    15,     0,   112,   113,   114,   115,   116,   117,   118,
      16,    17,    49,    50,    51,    18,    19,     0,    20,    21,
     198,     0,     0,     1,     2,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,   103,   104,     0,
     101,   102,   103,   104,    68,    14,    15,     0,   105,     0,
       0,     0,   106,   107,   108,    16,    17,   106,   107,   108,
      18,    19,     0,    20,    21,   226,     1,     2,     0,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,    70,     0,     0,     0,     0,     0,    14,    15,
       0,     0,    10,    11,    12,    13,     0,     0,    16,    17,
       1,     0,     0,    18,    19,     0,    20,    21,     0,     0,
      10,    11,    12,    13,    16,    17,     0,     0,     0,    18,
      67,     0,    14,    15,     0,     0,     0,     0,     0,     0,
       0,     0,    16,    17,     0,     0,     0,    18,    19,     1,
       0,    79,     3,     4,     5,     0,     1,     0,     0,    10,
      11,    12,    13,    73,     0,     0,    10,    11,    12,    13,
       0,    14,    15,    10,    11,    12,    13,     0,    14,    15,
       0,    16,    17,     0,     0,     0,    18,    19,    16,    17,
       0,     0,    75,    18,    19,    16,    17,   110,     0,     0,
      18,    67,    10,    11,    12,    13,     0,     0,   112,   113,
     114,   115,   116,   117,   118,     0,     0,     0,     0,     0,
     110,   157,     0,     0,    16,    17,     0,     0,     0,    18,
      67,   112,   113,   114,   115,   116,   117,   118,   110,     0,
       0,     0,     0,     0,   178,     0,     0,     0,     0,   112,
     113,   114,   115,   116,   117,   118,    43,    44,    45,    46,
      47,     0,   186,     0,    48,    45,    46,     0,     0,   -35,
     -35,   154,   -36,    49,    50,    51,     0,     0,     0,     0,
      49,    50,    51,   112,   113,   114,   115,   116,   117,   118
};

static const yytype_int16 yycheck[] =
{
       0,    23,    47,    93,   107,     3,     3,    41,     3,     9,
      14,    15,    16,    17,     3,    13,    14,    15,    16,    19,
       3,    49,     3,    23,    40,    41,    42,    25,    26,    40,
      41,    42,    13,    14,    15,    16,    41,    35,    36,    40,
      18,    42,    40,    41,    25,    26,    44,    41,    45,    47,
       3,     0,    52,    45,    35,    36,    43,    43,    43,    40,
      41,    43,     3,    63,     3,    43,    44,    67,    43,    47,
      48,    49,    50,    43,    13,    14,    15,    16,   106,    37,
      38,    39,    41,    31,    62,    30,    25,    26,    31,    31,
      30,   119,    30,    93,   197,    47,    35,    36,    30,    29,
       3,    40,    41,    30,    48,   133,     3,    30,    43,    46,
     110,    47,   112,   113,   114,   115,   116,   117,   118,    32,
      45,    30,    27,   101,   102,   153,   154,   105,   106,   107,
       3,    41,   222,    47,    46,   135,   158,   137,     9,    47,
       3,   119,    32,    14,    15,    16,    17,    31,    19,   194,
       5,    43,    46,    46,   187,   133,   178,     3,   158,    44,
     188,   216,    62,    46,   186,   156,   134,    13,    14,    15,
      16,    62,   138,    62,   152,   153,   154,   190,   178,    25,
      26,    52,   210,   152,    -1,   185,   186,    -1,    -1,    35,
      36,    -1,    63,   215,    40,    41,    67,    -1,    44,   177,
      -1,   223,   224,    33,    34,    35,    36,    37,    38,    39,
     188,    -1,   136,    -1,    22,   215,   194,    -1,    -1,   197,
      28,    -1,   222,   223,   224,    33,    34,    35,    36,    37,
      38,    39,   210,    -1,    -1,    -1,    -1,    -1,   216,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    22,    -1,   135,    -1,   137,    25,    26,    -1,
      25,    26,    -1,    33,    34,    35,    36,    37,    38,    39,
      35,    36,    40,    41,    42,    40,    41,    -1,    43,    44,
      45,    -1,    -1,     3,     4,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    25,    26,    -1,
      23,    24,    25,    26,   185,    25,    26,    -1,    31,    -1,
      -1,    -1,    40,    41,    42,    35,    36,    40,    41,    42,
      40,    41,    -1,    43,    44,    45,     3,     4,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    13,    14,    15,    16,    -1,    -1,    35,    36,
       3,    -1,    -1,    40,    41,    -1,    43,    44,    -1,    -1,
      13,    14,    15,    16,    35,    36,    -1,    -1,    -1,    40,
      41,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    -1,    -1,    -1,    40,    41,     3,
      -1,    44,     6,     7,     8,    -1,     3,    -1,    -1,    13,
      14,    15,    16,     3,    -1,    -1,    13,    14,    15,    16,
      -1,    25,    26,    13,    14,    15,    16,    -1,    25,    26,
      -1,    35,    36,    -1,    -1,    -1,    40,    41,    35,    36,
      -1,    -1,     3,    40,    41,    35,    36,    22,    -1,    -1,
      40,    41,    13,    14,    15,    16,    -1,    -1,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      22,    46,    -1,    -1,    35,    36,    -1,    -1,    -1,    40,
      41,    33,    34,    35,    36,    37,    38,    39,    22,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    33,
      34,    35,    36,    37,    38,    39,    23,    24,    25,    26,
      27,    -1,    46,    -1,    31,    25,    26,    -1,    -1,    29,
      30,    31,    22,    40,    41,    42,    -1,    -1,    -1,    -1,
      40,    41,    42,    33,    34,    35,    36,    37,    38,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    25,    26,    35,    36,    40,    41,
      43,    44,    50,    51,    52,    53,    54,    57,    58,    61,
      66,    70,    71,    74,    77,    80,    81,    82,    85,    90,
      91,    92,    93,    23,    24,    25,    26,    27,    31,    40,
      41,    42,    41,     3,    72,    73,     3,    78,    79,     3,
      75,    76,    41,    41,     3,    62,     3,    41,    81,    90,
       3,    81,    92,     3,    81,     3,    81,    92,    92,    44,
      47,    83,    84,    85,    86,    87,    90,     3,    63,    64,
      65,    90,    45,    55,     0,    52,    43,    43,    43,    43,
      43,    23,    24,    25,    26,    31,    40,    41,    42,    43,
      22,    28,    33,    34,    35,    36,    37,    38,    39,    40,
      42,    85,    85,    44,    54,    85,    85,    86,    85,    94,
      95,     3,    90,    31,    30,    31,    30,    31,    30,    68,
      69,    71,    74,    77,    85,    90,    41,     3,    45,    88,
      89,    47,    30,    48,    31,    29,    30,    46,    51,    85,
      85,    85,    86,    94,     3,    90,    90,    90,    90,    90,
      90,    90,    90,    86,     3,    47,    46,    30,    46,    86,
      72,    90,    78,    90,    75,    43,    46,    59,    32,    45,
      30,    83,    86,    86,    27,     3,    65,    41,    45,    47,
      46,    47,    85,    52,    67,    90,    52,    63,    86,    88,
      32,    54,    85,    94,    56,     5,    43,    46,    86,    46,
      52,    68,    44,    46,    51,    52,    45,    60
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    53,
      54,    55,    56,    57,    57,    58,    59,    60,    61,    62,
      63,    63,    63,    64,    64,    65,    65,    66,    67,    67,
      68,    68,    69,    69,    69,    69,    70,    71,    72,    72,
      73,    73,    74,    75,    75,    76,    76,    77,    78,    78,
      79,    79,    80,    80,    81,    81,    81,    81,    81,    81,
      82,    82,    83,    83,    84,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    86,    86,    86,    87,
      87,    88,    88,    89,    89,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    92,    92,    92,    92,    92,
      92,    92,    92,    93,    93,    93,    94,    94,    94,    95,
      95
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     2,
       5,     0,     0,     1,     2,     2,     0,     0,    10,     1,
       1,     2,     0,     3,     1,     1,     3,     9,     1,     0,
       1,     0,     1,     1,     1,     1,     5,     2,     3,     1,
       1,     3,     2,     3,     1,     1,     3,     2,     3,     1,
       1,     3,     5,     7,     4,     3,     4,     3,     4,     3,
       3,     2,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     5,     1,     3,     2,     1,     5,
       1,     3,     1,     3,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     4,     4,     6,     1,     2,     0,     1,
       3
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
#line 1649 "y.tab.c"
    break;

  case 3: /* S: %empty  */
#line 207 "mini_js.y"
    { yyval.clear(); }
#line 1655 "y.tab.c"
    break;

  case 4: /* CMDs: CMDs CMD  */
#line 210 "mini_js.y"
                 { yyval.c = yyvsp[-1].c + yyvsp[0].c; }
#line 1661 "y.tab.c"
    break;

  case 15: /* CMD: ATRIB ';'  */
#line 227 "mini_js.y"
      { yyval.c = yyvsp[-1].c + "^"; }
#line 1667 "y.tab.c"
    break;

  case 18: /* CMD: ';'  */
#line 231 "mini_js.y"
      { yyval.clear(); }
#line 1673 "y.tab.c"
    break;

  case 19: /* BLOCO_VAZIO: '{' '}'  */
#line 235 "mini_js.y"
            { yyval.c = vector<string>{"<{", "}>"}; }
#line 1679 "y.tab.c"
    break;

  case 20: /* BLOCO: '{' START_BLOCO CMDs '}' END_BLOCO  */
#line 239 "mini_js.y"
        { yyval.c = "<{" + yyvsp[-2].c + "}>" ; }
#line 1685 "y.tab.c"
    break;

  case 21: /* START_BLOCO: %empty  */
#line 242 "mini_js.y"
              {empilha_escopo_novo();}
#line 1691 "y.tab.c"
    break;

  case 22: /* END_BLOCO: %empty  */
#line 244 "mini_js.y"
            {desempilha_escopo();}
#line 1697 "y.tab.c"
    break;

  case 23: /* CMD_RET: RETURN  */
#line 248 "mini_js.y"
          { 
            if(blocos_alinhados_em_funcao.size() == 0) {
              cerr << "Erro: Não é permitido 'return' fora de funções." << endl;
              exit(1);
            }
            yyval.c = vector<string>{"undefined"}; /* 1. Empilha undefined */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            yyval.c =  yyval.c + vector<string>{ "'&retorno'", "@", "~" }; /* 3. Instruções de retorno */
          }
#line 1714 "y.tab.c"
    break;

  case 24: /* CMD_RET: RETURN E  */
#line 261 "mini_js.y"
          { 
            if(blocos_alinhados_em_funcao.size() == 0) {
              cerr << "Erro: Não é permitido 'return' fora de funções." << endl;
              exit(1);
            }
            yyval.c = yyvsp[0].c; /* 1. Empilha o valor da expressão E */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            /* 3. Instruções de retorno (NÃO adicione $2.c de novo) */
            yyval.c = yyval.c + vector<string>{ "'&retorno'", "@", "~" }; 
          }
#line 1732 "y.tab.c"
    break;

  case 25: /* CMD_ASM: E ASM  */
#line 276 "mini_js.y"
                { yyval.c = yyvsp[-1].c + yyvsp[0].c + "^"; }
#line 1738 "y.tab.c"
    break;

  case 26: /* EMPILHA_TS_FUNC: %empty  */
#line 279 "mini_js.y"
                  { ts.push_back( map< string, Simbolo >{} ); 
                blocos_alinhados_em_funcao.push_back(0);}
#line 1745 "y.tab.c"
    break;

  case 27: /* DESEMPILHA_TS_FUNC: %empty  */
#line 283 "mini_js.y"
                     { ts.pop_back(); 
                      blocos_alinhados_em_funcao.pop_back();}
#line 1752 "y.tab.c"
    break;

  case 28: /* CMD_FUNC: FUNCTION NOME_FUNCAO '(' EMPILHA_TS_FUNC LISTA_PARAMs ')' '{' CMDs '}' DESEMPILHA_TS_FUNC  */
#line 288 "mini_js.y"
           { 
             /* Note que os índices mudaram: NOME_FUNCAO é $2, LISTA_PARAMs é $5, CMDs é $8 */
             string definicao_lbl_endereco_funcao = ":" + yyvsp[-8].endereco_funcao;
             
             yyval.c = yyvsp[-8].c ; /* Código de NOME_FUNCAO (declara var, atribui obj função) */

             funcoes = funcoes + definicao_lbl_endereco_funcao + yyvsp[-5].c + yyvsp[-2].c +
                         "undefined" + "@" + "'&retorno'" + "@"+ "~";
             
           }
#line 1767 "y.tab.c"
    break;

  case 29: /* NOME_FUNCAO: ID  */
#line 315 "mini_js.y"
            { 
              string endereco_funcao = gera_label( "func_" + yyvsp[0].c[0] );
              yyval.endereco_funcao = endereco_funcao; /* 1. Salva o label para CMD_FUNC usar */
              
              Atributos decl = declara_var( Var, yyvsp[0] ); // 2. Declara a variável
              
              /* 3. Gera código usando decl.c (que pode ou não ter '&') e a var local 'endereco_funcao' */
              yyval.c = decl.c +                                    /* 'foo &' ou nada se for redeclaração */
                     yyvsp[0].c + "{}" + "="  + "'&funcao'" + endereco_funcao +  /* 'foo '&funcao' func_foo_1' ... */
                     "[=]" + "^";                              /* ... '[=] ^' (atribui prop) */
            }
#line 1783 "y.tab.c"
    break;

  case 32: /* LISTA_PARAMs: %empty  */
#line 330 "mini_js.y"
             { yyval.clear(); }
#line 1789 "y.tab.c"
    break;

  case 33: /* PARAMs: PARAMs ',' PARAM  */
#line 334 "mini_js.y"
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
#line 1813 "y.tab.c"
    break;

  case 34: /* PARAMs: PARAM  */
#line 354 "mini_js.y"
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
#line 1836 "y.tab.c"
    break;

  case 35: /* PARAM: ID  */
#line 375 "mini_js.y"
      { yyval = yyvsp[0]; 
        yyval.n_args = 1;
        yyval.valor_default.clear(); }
#line 1844 "y.tab.c"
    break;

  case 36: /* PARAM: ID '=' EOBJ  */
#line 379 "mini_js.y"
      { 
        yyval = yyvsp[-2];
        yyval.n_args = 1;
        yyval.valor_default = yyvsp[0].c; }
#line 1853 "y.tab.c"
    break;

  case 37: /* CMD_FOR: FOR '(' SF ';' ECOND_OPT ';' EF ')' CMD  */
#line 386 "mini_js.y"
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          yyval.c = yyvsp[-6].c + define_label (teste_for) +
                 yyvsp[-4].c + "!" + fim_for + "?" + yyvsp[0].c + 
                 yyvsp[-2].c + teste_for + "#" + define_label(fim_for);
        }
#line 1865 "y.tab.c"
    break;

  case 39: /* ECOND_OPT: %empty  */
#line 396 "mini_js.y"
            { yyval.c = vector<string>{"1"}; }
#line 1871 "y.tab.c"
    break;

  case 40: /* EF: ATRIB  */
#line 400 "mini_js.y"
      { yyval.c = yyvsp[0].c + "^"; }
#line 1877 "y.tab.c"
    break;

  case 41: /* EF: %empty  */
#line 401 "mini_js.y"
      { yyval.clear(); }
#line 1883 "y.tab.c"
    break;

  case 46: /* CMD_WHILE: WHILE '(' E ')' CMD  */
#line 411 "mini_js.y"
        { string inicio_while = gera_label( "inicio_while" );
          string fim_while = gera_label( "fim_while" );
          yyval.c = define_label(inicio_while) + 
                 yyvsp[-2].c + "!" + fim_while + "?" + yyvsp[0].c + 
                 inicio_while + "#" + define_label(fim_while);
        }
#line 1894 "y.tab.c"
    break;

  case 47: /* CMD_LET: LET LET_VARs  */
#line 419 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1900 "y.tab.c"
    break;

  case 48: /* LET_VARs: LET_VAR ',' LET_VARs  */
#line 422 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1906 "y.tab.c"
    break;

  case 50: /* LET_VAR: ID  */
#line 427 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[0] ).c; }
#line 1912 "y.tab.c"
    break;

  case 51: /* LET_VAR: ID '=' EOBJ  */
#line 429 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1918 "y.tab.c"
    break;

  case 52: /* CMD_VAR: VAR VAR_VARs  */
#line 436 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1924 "y.tab.c"
    break;

  case 53: /* VAR_VARs: VAR_VAR ',' VAR_VARs  */
#line 439 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1930 "y.tab.c"
    break;

  case 55: /* VAR_VAR: ID  */
#line 444 "mini_js.y"
          { yyval.c = declara_var( Var, yyvsp[0] ).c; }
#line 1936 "y.tab.c"
    break;

  case 56: /* VAR_VAR: ID '=' E  */
#line 446 "mini_js.y"
          {  yyval.c = declara_var( Var, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1942 "y.tab.c"
    break;

  case 57: /* CMD_CONST: CONST CONST_VARs  */
#line 453 "mini_js.y"
                            { yyval.c = yyvsp[0].c; }
#line 1948 "y.tab.c"
    break;

  case 58: /* CONST_VARs: CONST_VAR ',' CONST_VARs  */
#line 456 "mini_js.y"
                                      { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1954 "y.tab.c"
    break;

  case 60: /* CONST_VAR: ID  */
#line 461 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[0] ).c; }
#line 1960 "y.tab.c"
    break;

  case 61: /* CONST_VAR: ID '=' E  */
#line 463 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1966 "y.tab.c"
    break;

  case 62: /* CMD_IF: IF '(' E ')' CMD  */
#line 471 "mini_js.y"
          {
            string fim_if = gera_label( "fim_if" );
            yyval.c = yyvsp[-2].c + "!" + fim_if + "?" + yyvsp[0].c + define_label(fim_if);
          }
#line 1975 "y.tab.c"
    break;

  case 63: /* CMD_IF: IF '(' E ')' CMD ELSE CMD  */
#line 477 "mini_js.y"
          {  string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            yyval.c = yyvsp[-4].c + "!" + else_if + "?" + yyvsp[-2].c + fim_if + 
            "#" + define_label(else_if) + yyvsp[0].c +  define_label(fim_if);
          }
#line 1985 "y.tab.c"
    break;

  case 64: /* LVALUEPROP: LVALUEPROP '[' EOBJ ']'  */
#line 486 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].esq + yyvsp[-3].dir + "[@]";
              yyval.dir = yyvsp[-1].c;
               }
#line 1994 "y.tab.c"
    break;

  case 65: /* LVALUEPROP: LVALUEPROP '.' ID  */
#line 491 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].esq + yyvsp[-2].dir + "[@]";
              yyval.dir = yyvsp[0].c;
               }
#line 2003 "y.tab.c"
    break;

  case 66: /* LVALUEPROP: F '[' EOBJ ']'  */
#line 496 "mini_js.y"
             { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].c;
              yyval.dir = yyvsp[-1].c;
               }
#line 2012 "y.tab.c"
    break;

  case 67: /* LVALUEPROP: F '.' ID  */
#line 501 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].c;
              yyval.dir = yyvsp[0].c;
              }
#line 2021 "y.tab.c"
    break;

  case 68: /* LVALUEPROP: ID '[' EOBJ ']'  */
#line 507 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].c + "@";
              yyval.dir = yyvsp[-1].c;
              }
#line 2030 "y.tab.c"
    break;

  case 69: /* LVALUEPROP: ID '.' ID  */
#line 512 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].c + "@";
              yyval.dir = yyvsp[0].c;
               }
#line 2039 "y.tab.c"
    break;

  case 70: /* LIST: '[' LISTVALS ']'  */
#line 519 "mini_js.y"
        { 
          yyval.c = vector<string>{"[]"};
          yyval.c += yyvsp[-1].c;
        }
#line 2048 "y.tab.c"
    break;

  case 71: /* LIST: '[' ']'  */
#line 523 "mini_js.y"
                { yyval.c = vector<string>{"[]"}; }
#line 2054 "y.tab.c"
    break;

  case 72: /* LISTVALS: LISTVAL ',' LISTVALS  */
#line 527 "mini_js.y"
         { 
           yyval.c = yyvsp[-2].c + to_string(yyvsp[-2].n_args) + yyvsp[0].c; 
           yyval.n_args = yyvsp[-2].n_args + 1;
         }
#line 2063 "y.tab.c"
    break;

  case 73: /* LISTVALS: LISTVAL  */
#line 532 "mini_js.y"
         {
           yyval.c = yyvsp[0].c ;
           yyval.n_args = 1;
         }
#line 2072 "y.tab.c"
    break;

  case 74: /* LISTVAL: EOBJ  */
#line 539 "mini_js.y"
        { 
          yyval.c = yyvsp[0].c + "[<=]";
          yyval.n_args = 0;  // será preenchido por LISTVALS
        }
#line 2081 "y.tab.c"
    break;

  case 75: /* ATRIB: ID '=' ATRIB  */
#line 547 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[0].c + "="; }
#line 2087 "y.tab.c"
    break;

  case 76: /* ATRIB: ID MAIS_IGUAL ATRIB  */
#line 549 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "+" + "="; }
#line 2093 "y.tab.c"
    break;

  case 77: /* ATRIB: ID MENOS_IGUAL ATRIB  */
#line 551 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "-" + "="; }
#line 2099 "y.tab.c"
    break;

  case 78: /* ATRIB: LVALUEPROP '=' ATRIB  */
#line 553 "mini_js.y"
    { yyval.c = yyvsp[-2].esq + yyvsp[-2].dir + yyvsp[0].c + "[=]"; }
#line 2105 "y.tab.c"
    break;

  case 79: /* ATRIB: LVALUEPROP MAIS_IGUAL ATRIB  */
#line 555 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[-2].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[-2].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  yyvsp[0].c + "+" + "[=]" ;}
#line 2118 "y.tab.c"
    break;

  case 80: /* ATRIB: LVALUEPROP MENOS_IGUAL ATRIB  */
#line 564 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[-2].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[-2].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  yyvsp[0].c + "-" + "[=]" ;}
#line 2131 "y.tab.c"
    break;

  case 81: /* ATRIB: ID SETA BLOCO  */
#line 573 "mini_js.y"
    {
      string endereco_funcao = gera_label( "func_arrow_" + yyvsp[-2].c[0] );
      yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      funcoes = funcoes + define_label(endereco_funcao) + yyvsp[-2].c + "&" + yyvsp[-2].c + 
                "arguments" + "@" + "0" + "[@]" + "=" + "^" + yyvsp[0].c +
                "undefined" + "'&retorno'" + "@" + "~";
    }
#line 2143 "y.tab.c"
    break;

  case 82: /* ATRIB: ID SETA ATRIB  */
#line 581 "mini_js.y"
    {
      string endereco_funcao = gera_label( "func_arrow_" + yyvsp[-2].c[0] );
      yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      funcoes = funcoes + define_label(endereco_funcao) + yyvsp[-2].c + "&" + yyvsp[-2].c + 
                "arguments" + "@" + "0" + "[@]" + "=" + "^" + yyvsp[0].c +
                "'&retorno'" + "@" + "~";
    }
#line 2155 "y.tab.c"
    break;

  case 83: /* ATRIB: '(' LISTA_PARAMs FECHA_PARENTESES_LAMBDA SETA ATRIB  */
#line 589 "mini_js.y"
    {
      string endereco_funcao = gera_label( "func_arrow" );
      yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      funcoes = funcoes + define_label(endereco_funcao) + yyvsp[-3].c + yyvsp[0].c +
                "'&retorno'" + "@" + "~";
    }
#line 2166 "y.tab.c"
    break;

  case 84: /* ATRIB: '(' LISTA_PARAMs FECHA_PARENTESES_LAMBDA SETA BLOCO  */
#line 596 "mini_js.y"
    {
      string endereco_funcao = gera_label( "func_arrow" );
      yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      funcoes = funcoes + define_label(endereco_funcao) + yyvsp[-3].c + yyvsp[0].c +
                "undefined" + "'&retorno'" + "@" + "~";
    }
#line 2177 "y.tab.c"
    break;

  case 86: /* EOBJ: '{' CAMPOS '}'  */
#line 612 "mini_js.y"
    { 
      yyval.c = vector<string>{"{}"};
      if(yyvsp[-1].c.size() > 0) {
        yyval.c += yyvsp[-1].c;
      }
    }
#line 2188 "y.tab.c"
    break;

  case 87: /* EOBJ: '{' '}'  */
#line 619 "mini_js.y"
    { yyval.c = vector<string>{"{}"}; }
#line 2194 "y.tab.c"
    break;

  case 91: /* CAMPOS: CAMPO ',' CAMPOS  */
#line 633 "mini_js.y"
       { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 2200 "y.tab.c"
    break;

  case 93: /* CAMPO: ID ':' EOBJ  */
#line 638 "mini_js.y"
      { yyval.c = "'" + yyvsp[-2].c[0] + "'" + yyvsp[0].c + "[<=]"; }
#line 2206 "y.tab.c"
    break;

  case 94: /* CAMPO: ID  */
#line 640 "mini_js.y"
      { yyval.c = "'" + yyvsp[0].c[0] + "'" + yyvsp[0].c + "@" + "[<=]"; }
#line 2212 "y.tab.c"
    break;

  case 95: /* E: ID  */
#line 644 "mini_js.y"
    { yyval.c = yyvsp[0].c + "@"; }
#line 2218 "y.tab.c"
    break;

  case 96: /* E: LVALUEPROP  */
#line 646 "mini_js.y"
    { yyval.c = yyvsp[0].esq + yyvsp[0].dir + "[@]"; }
#line 2224 "y.tab.c"
    break;

  case 97: /* E: E IGUAL E  */
#line 648 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c;}
#line 2230 "y.tab.c"
    break;

  case 98: /* E: E '<' E  */
#line 650 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2236 "y.tab.c"
    break;

  case 99: /* E: E '>' E  */
#line 652 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2242 "y.tab.c"
    break;

  case 100: /* E: E '+' E  */
#line 654 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2248 "y.tab.c"
    break;

  case 101: /* E: E '-' E  */
#line 656 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2254 "y.tab.c"
    break;

  case 102: /* E: E '*' E  */
#line 658 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2260 "y.tab.c"
    break;

  case 103: /* E: E '/' E  */
#line 660 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2266 "y.tab.c"
    break;

  case 104: /* E: E '%' E  */
#line 662 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2272 "y.tab.c"
    break;

  case 105: /* E: UN  */
#line 664 "mini_js.y"
    { yyval.c = yyvsp[0].c; }
#line 2278 "y.tab.c"
    break;

  case 106: /* UN: MAIS_MAIS ID  */
#line 668 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "+" + "=";}
#line 2284 "y.tab.c"
    break;

  case 107: /* UN: MENOS_MENOS ID  */
#line 670 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "-" + "=";}
#line 2290 "y.tab.c"
    break;

  case 108: /* UN: ID MAIS_MAIS  */
#line 672 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "+" + "=" + "^";}
#line 2296 "y.tab.c"
    break;

  case 109: /* UN: ID MENOS_MENOS  */
#line 674 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "-" + "=" + "^";}
#line 2302 "y.tab.c"
    break;

  case 110: /* UN: MAIS_MAIS LVALUEPROP  */
#line 676 "mini_js.y"
        { string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[0].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[0].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "+" + "[=]" ;}
#line 2314 "y.tab.c"
    break;

  case 111: /* UN: MENOS_MENOS LVALUEPROP  */
#line 684 "mini_js.y"
        {
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[0].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[0].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "-" + "[=]" ;}
#line 2327 "y.tab.c"
    break;

  case 112: /* UN: LVALUEPROP MAIS_MAIS  */
#line 693 "mini_js.y"
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
#line 2342 "y.tab.c"
    break;

  case 113: /* UN: LVALUEPROP MENOS_MENOS  */
#line 704 "mini_js.y"
        {string esq = gera_temp("esq");
         string dir = gera_temp("dir");
         yyval.c = to_inst(esq) + "&" + esq + yyvsp[-1].esq + "=" + "^" +
                to_inst(dir) + "&" + dir + yyvsp[-1].dir + "=" + "^" +
                esq + "@" + dir + "@" + "[@]" +
                esq + "@" + dir + "@" +
                esq + "@" + dir + "@" + "[@]" +
                "1" + "-" + "[=]" + "^";}
#line 2355 "y.tab.c"
    break;

  case 120: /* F: '-' F  */
#line 722 "mini_js.y"
      { yyval.c = "0" + yyvsp[0].c + "-" ; }
#line 2361 "y.tab.c"
    break;

  case 121: /* F: '+' F  */
#line 724 "mini_js.y"
      { yyval.c = "0" + yyvsp[0].c + "+" ; }
#line 2367 "y.tab.c"
    break;

  case 123: /* CHAMA_FUNC: ID '(' LISTA_ARGS ')'  */
#line 731 "mini_js.y"
              { 
                yyval.c = yyvsp[-1].c + yyvsp[-3].c[0] + "@" + "$" ;
              }
#line 2375 "y.tab.c"
    break;

  case 124: /* CHAMA_FUNC: LVALUEPROP '(' LISTA_ARGS ')'  */
#line 735 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + yyvsp[-3].esq + yyvsp[-3].dir + "[@]" + "$" ;
              }
#line 2384 "y.tab.c"
    break;

  case 125: /* CHAMA_FUNC: '(' E ')' '(' LISTA_ARGS ')'  */
#line 740 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + yyvsp[-4].c + "$" ;
              }
#line 2393 "y.tab.c"
    break;

  case 126: /* LISTA_ARGS: ARGs  */
#line 747 "mini_js.y"
           { 
             yyval.c = yyvsp[0].c + to_string( yyvsp[0].n_args ); 
             yyval.n_args = yyvsp[0].n_args;
           }
#line 2402 "y.tab.c"
    break;

  case 127: /* LISTA_ARGS: ARGs ','  */
#line 752 "mini_js.y"
           {
             yyval.c = yyvsp[-1].c + to_string( yyvsp[-1].n_args ); 
             yyval.n_args = yyvsp[-1].n_args;
           }
#line 2411 "y.tab.c"
    break;

  case 128: /* LISTA_ARGS: %empty  */
#line 757 "mini_js.y"
           { 
              yyval.c = vector<string>{"0"}; 
              yyval.n_args = 0; 
           }
#line 2420 "y.tab.c"
    break;

  case 129: /* ARGs: ATRIB  */
#line 764 "mini_js.y"
      { 
        yyval.c = yyvsp[0].c;     
        yyval.n_args = 1;   
      }
#line 2429 "y.tab.c"
    break;

  case 130: /* ARGs: ARGs ',' ATRIB  */
#line 769 "mini_js.y"
      { 
        yyval.c = yyvsp[-2].c + yyvsp[0].c; 
        yyval.n_args = yyvsp[-2].n_args + 1; 
      }
#line 2438 "y.tab.c"
    break;


#line 2442 "y.tab.c"

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

#line 776 "mini_js.y"


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



