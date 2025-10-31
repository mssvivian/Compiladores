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
#include <sstream>

using namespace std;

int linha = 1, coluna = 0; 

struct Atributos {
  vector<string> c; // Código
  string endereco_funcao;
  int linha = 0, coluna = 0;

  // Só para argumentos e parâmetros
  int n_args = 0;     

  int contador = 0; //contar parametros e argumentos
  
  // Só para valor default de argumento        
  vector<string> valor_default; 

  void clear() {
    c.clear();
    valor_default.clear();
    linha = 0;
    coluna = 0;
    n_args = 0;
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


// Divide uma string em vetor de strings usando espaço em branco como separador
vector<string> tokeniza(const string& s) {
  vector<string> tokens;
  string token;
  istringstream iss(s);
  while (iss >> token) tokens.push_back(token);
  return tokens;
}

// Remove todos os caracteres de 'chars' do início e fim da string 's'
string trim(const string& s, const string& chars) {
  size_t start = 0;
  size_t end = s.size();
  // Remove do início
  while (start < end && chars.find(s[start]) != string::npos) ++start;
  // Remove do fim
  while (end > start && chars.find(s[end-1]) != string::npos) --end;
  return s.substr(start, end - start);
}

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

void empilha_escopo_novo();
void desempilha_escopo();

#line 219 "y.tab.c"

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
    ASM = 268,                     /* ASM  */
    CDOUBLE = 269,                 /* CDOUBLE  */
    CSTRING = 270,                 /* CSTRING  */
    CINT = 271,                    /* CINT  */
    CBOOL = 272,                   /* CBOOL  */
    AND = 273,                     /* AND  */
    OR = 274,                      /* OR  */
    ME_IG = 275,                   /* ME_IG  */
    MA_IG = 276,                   /* MA_IG  */
    DIF = 277,                     /* DIF  */
    IGUAL = 278,                   /* IGUAL  */
    MAIS_IGUAL = 279,              /* MAIS_IGUAL  */
    MAIS_MAIS = 280,               /* MAIS_MAIS  */
    MENOS_IGUAL = 281,             /* MENOS_IGUAL  */
    MENOS_MENOS = 282              /* MENOS_MENOS  */
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
#define ASM 268
#define CDOUBLE 269
#define CSTRING 270
#define CINT 271
#define CBOOL 272
#define AND 273
#define OR 274
#define ME_IG 275
#define MA_IG 276
#define DIF 277
#define IGUAL 278
#define MAIS_IGUAL 279
#define MAIS_MAIS 280
#define MENOS_IGUAL 281
#define MENOS_MENOS 282

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
  YYSYMBOL_ASM = 13,                       /* ASM  */
  YYSYMBOL_CDOUBLE = 14,                   /* CDOUBLE  */
  YYSYMBOL_CSTRING = 15,                   /* CSTRING  */
  YYSYMBOL_CINT = 16,                      /* CINT  */
  YYSYMBOL_CBOOL = 17,                     /* CBOOL  */
  YYSYMBOL_AND = 18,                       /* AND  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_ME_IG = 20,                     /* ME_IG  */
  YYSYMBOL_MA_IG = 21,                     /* MA_IG  */
  YYSYMBOL_DIF = 22,                       /* DIF  */
  YYSYMBOL_IGUAL = 23,                     /* IGUAL  */
  YYSYMBOL_MAIS_IGUAL = 24,                /* MAIS_IGUAL  */
  YYSYMBOL_MAIS_MAIS = 25,                 /* MAIS_MAIS  */
  YYSYMBOL_MENOS_IGUAL = 26,               /* MENOS_IGUAL  */
  YYSYMBOL_MENOS_MENOS = 27,               /* MENOS_MENOS  */
  YYSYMBOL_28_ = 28,                       /* '='  */
  YYSYMBOL_29_ = 29,                       /* '<'  */
  YYSYMBOL_30_ = 30,                       /* '>'  */
  YYSYMBOL_31_ = 31,                       /* '+'  */
  YYSYMBOL_32_ = 32,                       /* '-'  */
  YYSYMBOL_33_ = 33,                       /* '*'  */
  YYSYMBOL_34_ = 34,                       /* '/'  */
  YYSYMBOL_35_ = 35,                       /* '%'  */
  YYSYMBOL_36_ = 36,                       /* '['  */
  YYSYMBOL_37_ = 37,                       /* '('  */
  YYSYMBOL_38_ = 38,                       /* '.'  */
  YYSYMBOL_39_ = 39,                       /* ';'  */
  YYSYMBOL_40_ = 40,                       /* '{'  */
  YYSYMBOL_41_ = 41,                       /* '}'  */
  YYSYMBOL_42_ = 42,                       /* ')'  */
  YYSYMBOL_43_ = 43,                       /* ','  */
  YYSYMBOL_44_ = 44,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_S = 46,                         /* S  */
  YYSYMBOL_CMDs = 47,                      /* CMDs  */
  YYSYMBOL_CMD = 48,                       /* CMD  */
  YYSYMBOL_BLOCO = 49,                     /* BLOCO  */
  YYSYMBOL_START_BLOCO = 50,               /* START_BLOCO  */
  YYSYMBOL_END_BLOCO = 51,                 /* END_BLOCO  */
  YYSYMBOL_CMD_RET = 52,                   /* CMD_RET  */
  YYSYMBOL_CMD_ASM = 53,                   /* CMD_ASM  */
  YYSYMBOL_EMPILHA_TS_FUNC = 54,           /* EMPILHA_TS_FUNC  */
  YYSYMBOL_DESEMPILHA_TS_FUNC = 55,        /* DESEMPILHA_TS_FUNC  */
  YYSYMBOL_CMD_FUNC = 56,                  /* CMD_FUNC  */
  YYSYMBOL_NOME_FUNCAO = 57,               /* NOME_FUNCAO  */
  YYSYMBOL_LISTA_PARAMs = 58,              /* LISTA_PARAMs  */
  YYSYMBOL_PARAMs = 59,                    /* PARAMs  */
  YYSYMBOL_PARAM = 60,                     /* PARAM  */
  YYSYMBOL_CMD_FOR = 61,                   /* CMD_FOR  */
  YYSYMBOL_EF = 62,                        /* EF  */
  YYSYMBOL_SF = 63,                        /* SF  */
  YYSYMBOL_CMD_WHILE = 64,                 /* CMD_WHILE  */
  YYSYMBOL_CMD_LET = 65,                   /* CMD_LET  */
  YYSYMBOL_LET_VARs = 66,                  /* LET_VARs  */
  YYSYMBOL_LET_VAR = 67,                   /* LET_VAR  */
  YYSYMBOL_CMD_VAR = 68,                   /* CMD_VAR  */
  YYSYMBOL_VAR_VARs = 69,                  /* VAR_VARs  */
  YYSYMBOL_VAR_VAR = 70,                   /* VAR_VAR  */
  YYSYMBOL_CMD_CONST = 71,                 /* CMD_CONST  */
  YYSYMBOL_CONST_VARs = 72,                /* CONST_VARs  */
  YYSYMBOL_CONST_VAR = 73,                 /* CONST_VAR  */
  YYSYMBOL_CMD_IF = 74,                    /* CMD_IF  */
  YYSYMBOL_LVALUE = 75,                    /* LVALUE  */
  YYSYMBOL_LVALUEPROP = 76,                /* LVALUEPROP  */
  YYSYMBOL_LIST = 77,                      /* LIST  */
  YYSYMBOL_LISTVALS = 78,                  /* LISTVALS  */
  YYSYMBOL_LISTVAL = 79,                   /* LISTVAL  */
  YYSYMBOL_ATRIB = 80,                     /* ATRIB  */
  YYSYMBOL_E = 81,                         /* E  */
  YYSYMBOL_UN = 82,                        /* UN  */
  YYSYMBOL_F = 83,                         /* F  */
  YYSYMBOL_CHAMA_FUNC = 84,                /* CHAMA_FUNC  */
  YYSYMBOL_LISTA_ARGS = 85,                /* LISTA_ARGS  */
  YYSYMBOL_ARGs = 86                       /* ARGs  */
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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   644

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  120
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  208

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   282


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
       2,     2,     2,     2,     2,     2,     2,    35,     2,     2,
      37,    42,    33,    31,    43,    32,    38,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
      29,    28,    30,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,    44,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
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
      25,    26,    27
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   168,   168,   171,   172,   176,   177,   178,   179,   180,
     183,   184,   185,   186,   187,   189,   190,   194,   202,   204,
     207,   216,   228,   231,   235,   239,   266,   280,   281,   282,
     285,   305,   306,   327,   331,   338,   348,   350,   353,   354,
     355,   356,   359,   368,   371,   372,   375,   377,   381,   387,
     390,   391,   394,   396,   399,   405,   408,   409,   412,   414,
     417,   423,   429,   437,   440,   442,   444,   446,   448,   450,
     454,   455,   458,   459,   462,   466,   468,   470,   472,   474,
     476,   478,   480,   483,   485,   488,   490,   492,   494,   496,
     498,   500,   502,   504,   506,   508,   512,   514,   516,   518,
     520,   522,   524,   526,   528,   531,   532,   533,   534,   535,
     537,   538,   540,   543,   551,   560,   572,   577,   583,   589,
     594
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
  "LET", "CONST", "VAR", "FOR", "WHILE", "FUNCTION", "RETURN", "ASM",
  "CDOUBLE", "CSTRING", "CINT", "CBOOL", "AND", "OR", "ME_IG", "MA_IG",
  "DIF", "IGUAL", "MAIS_IGUAL", "MAIS_MAIS", "MENOS_IGUAL", "MENOS_MENOS",
  "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'['", "'('",
  "'.'", "';'", "'{'", "'}'", "')'", "','", "']'", "$accept", "S", "CMDs",
  "CMD", "BLOCO", "START_BLOCO", "END_BLOCO", "CMD_RET", "CMD_ASM",
  "EMPILHA_TS_FUNC", "DESEMPILHA_TS_FUNC", "CMD_FUNC", "NOME_FUNCAO",
  "LISTA_PARAMs", "PARAMs", "PARAM", "CMD_FOR", "EF", "SF", "CMD_WHILE",
  "CMD_LET", "LET_VARs", "LET_VAR", "CMD_VAR", "VAR_VARs", "VAR_VAR",
  "CMD_CONST", "CONST_VARs", "CONST_VAR", "CMD_IF", "LVALUE", "LVALUEPROP",
  "LIST", "LISTVALS", "LISTVAL", "ATRIB", "E", "UN", "F", "CHAMA_FUNC",
  "LISTA_ARGS", "ARGs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-153)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-32)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -153,     5,   342,  -153,   -28,   -21,    16,    16,    16,   -16,
      -8,    29,   556,  -153,  -153,  -153,  -153,    45,    45,   556,
     556,   212,   369,  -153,  -153,  -153,  -153,     2,    11,  -153,
    -153,  -153,    17,    18,    24,  -153,   226,   606,  -153,    26,
     147,  -153,   -30,  -153,   556,   556,  -153,  -153,    15,    39,
    -153,    27,    41,  -153,    30,    47,   531,   556,  -153,    40,
       6,   -14,   466,   -24,   -10,   -30,   -24,   -10,    19,    19,
    -153,    34,    37,  -153,   466,    42,   566,  -153,  -153,  -153,
    -153,  -153,  -153,   556,  -153,   556,  -153,   396,   556,    82,
     556,  -153,  -153,   423,   556,   556,    84,  -153,  -153,   556,
     556,   556,   556,   556,   556,   556,   556,   556,    85,  -153,
      48,    46,   580,    16,   450,    16,   477,    16,   504,  -153,
      52,  -153,  -153,  -153,  -153,   594,  -153,  -153,   556,    51,
      57,   262,  -153,  -153,    54,  -153,   174,  -153,  -153,    55,
    -153,   385,    64,  -153,   493,   331,   331,    19,    19,  -153,
    -153,  -153,   412,  -153,  -153,   556,   342,  -153,    66,   466,
    -153,    67,   466,  -153,    69,   466,   556,   342,   114,  -153,
    -153,   556,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
     115,  -153,  -153,  -153,   439,  -153,    91,    80,    81,  -153,
      86,  -153,   342,   556,   556,    87,     4,  -153,  -153,    88,
     466,  -153,  -153,   342,   303,  -153,  -153,  -153
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,    63,     0,     0,     0,     0,     0,
       0,     0,    20,   105,   107,   106,   108,     0,     0,     0,
       0,     0,     0,    16,    18,     3,    15,     0,     0,     9,
      10,    11,     0,     0,     0,     8,    84,    83,   110,     0,
      82,    95,   104,   112,   118,     0,    63,    43,    45,    46,
      55,    57,    58,    49,    51,    52,    37,     0,    26,     0,
      84,    83,    21,    96,   100,     0,    97,   101,    94,    93,
      71,     0,    73,    74,    82,     0,     0,     4,    12,    13,
       5,     6,     7,     0,    98,     0,    99,     0,     0,     0,
       0,   102,   103,     0,     0,   118,     0,    14,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   119,
       0,   116,     0,     0,     0,     0,     0,     0,     0,    41,
       0,    38,    39,    40,    36,     0,    23,    70,     0,     0,
     109,     0,    76,    77,     0,    75,     0,    69,    79,     0,
      78,     0,     0,    65,    85,    86,    87,    88,    89,    90,
      91,    92,     0,    67,   113,   117,     0,    44,     0,    47,
      56,     0,    59,    50,     0,    53,     0,     0,    29,    72,
     111,   118,    19,    80,    68,    81,    64,   114,    66,   120,
      61,    48,    60,    54,     0,    42,    33,     0,    27,    32,
       0,    17,     0,    37,     0,     0,    28,   115,    62,     0,
      34,     4,    30,     0,     0,    35,    24,    25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -153,  -153,   -76,  -152,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,   -70,  -153,   -61,  -153,  -153,
      77,    22,  -153,    78,    20,  -153,    83,    23,  -153,  -153,
      28,   129,  -153,    12,  -153,   -19,    -2,  -153,    21,  -153,
     -92,  -153
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    25,    26,    77,   191,    27,    28,   168,
     207,    29,    59,   187,   188,   189,    30,   119,   120,    31,
      32,    47,    48,    33,    53,    54,    34,    50,    51,    35,
      60,    61,    38,    71,    72,    39,    74,    41,    42,    43,
     110,   111
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,   131,    73,   142,   180,     3,   107,   186,   108,    44,
      62,    91,    88,    92,    89,   185,    45,    68,    69,    46,
      76,    56,    94,    95,    96,   109,    94,    95,    96,    57,
      36,    84,    58,    86,    49,    52,    55,   124,    65,    65,
     198,    78,    88,   112,    89,    63,    66,   -31,     4,    36,
      79,   205,   104,   105,   106,   125,    80,    81,   113,    13,
      14,    15,    16,    82,   132,    97,   133,   114,   135,   116,
     115,   138,    36,   117,   140,   118,   109,   126,   127,   190,
     128,    21,    22,   129,    36,   137,   136,   143,   153,   155,
     154,   166,   141,   170,   171,   173,   175,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   177,   181,   182,    73,
     183,    36,   159,    36,   162,    36,   165,   186,    36,   194,
     192,    36,   195,    36,   196,   204,   202,   201,   197,    40,
     203,    37,   199,   121,   122,   157,   179,   163,   160,   123,
     169,    49,     0,    52,     0,    55,    64,    67,     0,     0,
      37,     0,   109,     0,    40,     0,    36,     0,     0,    36,
      98,     0,     0,     0,   184,    40,     0,     0,     0,     0,
      99,     0,     0,    37,   124,     0,   100,   101,   102,   103,
     104,   105,   106,    36,    36,    37,     0,     0,     0,     0,
      40,     0,   200,     0,     0,    36,     0,    99,     0,    36,
       0,    40,    40,   100,   101,   102,   103,   104,   105,   106,
       0,     0,    37,     0,    37,     4,    37,     0,   174,    37,
      36,    36,    37,     0,    37,     0,    13,    14,    15,    16,
       0,    36,    36,     0,     0,     0,     0,    17,     0,    18,
       0,     0,     0,    19,    20,     0,     0,     0,    21,    22,
      83,    84,    85,    86,    87,     0,    70,    37,     0,     0,
      37,     0,    88,     0,    89,     4,     5,     0,     6,     7,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
       0,     0,     0,     0,    37,    37,     0,    17,     0,    18,
       0,     0,     0,    19,    20,     0,    37,     0,    21,    22,
      37,    23,    24,   172,     0,     0,     4,     5,     0,     6,
       7,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    37,    37,     0,     0,     0,     0,     0,    17,     0,
      18,     0,    37,    37,    19,    20,     0,     0,     0,    21,
      22,     0,    23,    24,   206,     4,     5,     0,     6,     7,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
     -32,   -32,   102,   103,   104,   105,   106,    17,     0,    18,
       0,     0,     4,    19,    20,     0,     0,     0,    21,    22,
       0,    23,    24,    13,    14,    15,    16,     0,     0,     0,
       0,     0,     0,     0,    17,     0,    18,     0,     0,     4,
      19,    20,     0,     0,     0,    21,    22,     0,    99,    75,
      13,    14,    15,    16,   100,   101,   102,   103,   104,   105,
     106,    17,     0,    18,     0,     0,     4,    19,    20,   176,
       0,     0,    21,    22,     0,    99,   134,    13,    14,    15,
      16,   100,   101,   102,   103,   104,   105,   106,    17,     0,
      18,     0,     0,     4,    19,    20,   178,     0,     0,    21,
      22,     0,    99,   139,    13,    14,    15,    16,   100,   101,
     102,   103,   104,   105,   106,    17,     0,    18,   193,     0,
       4,    19,    20,     0,     0,     0,    21,    22,     0,    99,
     158,    13,    14,    15,    16,   100,   101,   102,   103,   104,
     105,   106,    17,     0,    18,     0,     0,     4,    19,    20,
       0,     0,     0,    21,    22,     0,   -32,   161,    13,    14,
      15,    16,   100,   101,   102,   103,   104,   105,   106,    17,
       0,    18,     0,     0,     4,    19,    20,     6,     7,     8,
      21,    22,     0,     0,   164,    13,    14,    15,    16,     0,
       0,     0,     0,     0,     0,     0,    17,     0,    18,     4,
       0,     0,    19,    20,     0,     0,     0,    21,    22,     0,
      13,    14,    15,    16,     0,     0,     0,     0,     0,     0,
       0,    17,     0,    18,     0,     0,     0,    19,    20,    99,
       0,     0,    21,    22,     0,   100,   101,   102,   103,   104,
     105,   106,     0,    99,     0,     0,     0,     0,   130,   100,
     101,   102,   103,   104,   105,   106,     0,    99,     0,     0,
       0,     0,   156,   100,   101,   102,   103,   104,   105,   106,
      90,    91,     0,    92,    93,     0,   167,     0,     0,     0,
       0,     0,    94,    95,    96
};

static const yytype_int16 yycheck[] =
{
       2,    77,    21,    95,   156,     0,    36,     3,    38,    37,
      12,    25,    36,    27,    38,   167,    37,    19,    20,     3,
      22,    37,    36,    37,    38,    44,    36,    37,    38,    37,
       2,    25,     3,    27,     6,     7,     8,    56,    17,    18,
     192,    39,    36,    45,    38,    17,    18,    43,     3,    21,
      39,   203,    33,    34,    35,    57,    39,    39,    43,    14,
      15,    16,    17,    39,    83,    39,    85,    28,    87,    28,
      43,    90,    44,    43,    93,    28,    95,    37,    44,   171,
      43,    36,    37,    41,    56,     3,    88,     3,     3,    43,
      42,    39,    94,    42,    37,    41,    41,    99,   100,   101,
     102,   103,   104,   105,   106,   107,    42,    41,    41,   128,
      41,    83,   114,    85,   116,    87,   118,     3,    90,    28,
       5,    93,    42,    95,    43,   201,   196,    40,    42,   131,
      42,     2,   193,    56,    56,   113,   155,   117,   115,    56,
     128,   113,    -1,   115,    -1,   117,    17,    18,    -1,    -1,
      21,    -1,   171,    -1,   156,    -1,   128,    -1,    -1,   131,
      13,    -1,    -1,    -1,   166,   167,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    44,   193,    -1,    29,    30,    31,    32,
      33,    34,    35,   155,   156,    56,    -1,    -1,    -1,    -1,
     192,    -1,   194,    -1,    -1,   167,    -1,    23,    -1,   171,
      -1,   203,   204,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    83,    -1,    85,     3,    87,    -1,    44,    90,
     192,   193,    93,    -1,    95,    -1,    14,    15,    16,    17,
      -1,   203,   204,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    36,    37,
      24,    25,    26,    27,    28,    -1,    44,   128,    -1,    -1,
     131,    -1,    36,    -1,    38,     3,     4,    -1,     6,     7,
       8,     9,    10,    11,    12,    -1,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,   155,   156,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    32,    -1,   167,    -1,    36,    37,
     171,    39,    40,    41,    -1,    -1,     3,     4,    -1,     6,
       7,     8,     9,    10,    11,    12,    -1,    14,    15,    16,
      17,   192,   193,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,   203,   204,    31,    32,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    41,     3,     4,    -1,     6,     7,
       8,     9,    10,    11,    12,    -1,    14,    15,    16,    17,
      29,    30,    31,    32,    33,    34,    35,    25,    -1,    27,
      -1,    -1,     3,    31,    32,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,     3,
      31,    32,    -1,    -1,    -1,    36,    37,    -1,    23,    40,
      14,    15,    16,    17,    29,    30,    31,    32,    33,    34,
      35,    25,    -1,    27,    -1,    -1,     3,    31,    32,    44,
      -1,    -1,    36,    37,    -1,    23,    40,    14,    15,    16,
      17,    29,    30,    31,    32,    33,    34,    35,    25,    -1,
      27,    -1,    -1,     3,    31,    32,    44,    -1,    -1,    36,
      37,    -1,    23,    40,    14,    15,    16,    17,    29,    30,
      31,    32,    33,    34,    35,    25,    -1,    27,    39,    -1,
       3,    31,    32,    -1,    -1,    -1,    36,    37,    -1,    23,
      40,    14,    15,    16,    17,    29,    30,    31,    32,    33,
      34,    35,    25,    -1,    27,    -1,    -1,     3,    31,    32,
      -1,    -1,    -1,    36,    37,    -1,    23,    40,    14,    15,
      16,    17,    29,    30,    31,    32,    33,    34,    35,    25,
      -1,    27,    -1,    -1,     3,    31,    32,     6,     7,     8,
      36,    37,    -1,    -1,    40,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,     3,
      -1,    -1,    31,    32,    -1,    -1,    -1,    36,    37,    -1,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    32,    23,
      -1,    -1,    36,    37,    -1,    29,    30,    31,    32,    33,
      34,    35,    -1,    23,    -1,    -1,    -1,    -1,    42,    29,
      30,    31,    32,    33,    34,    35,    -1,    23,    -1,    -1,
      -1,    -1,    42,    29,    30,    31,    32,    33,    34,    35,
      24,    25,    -1,    27,    28,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,    47,     0,     3,     4,     6,     7,     8,     9,
      10,    11,    12,    14,    15,    16,    17,    25,    27,    31,
      32,    36,    37,    39,    40,    48,    49,    52,    53,    56,
      61,    64,    65,    68,    71,    74,    75,    76,    77,    80,
      81,    82,    83,    84,    37,    37,     3,    66,    67,    75,
      72,    73,    75,    69,    70,    75,    37,    37,     3,    57,
      75,    76,    81,    75,    76,    83,    75,    76,    81,    81,
      44,    78,    79,    80,    81,    40,    81,    50,    39,    39,
      39,    39,    39,    24,    25,    26,    27,    28,    36,    38,
      24,    25,    27,    28,    36,    37,    38,    39,    13,    23,
      29,    30,    31,    32,    33,    34,    35,    36,    38,    80,
      85,    86,    81,    43,    28,    43,    28,    43,    28,    62,
      63,    65,    68,    71,    80,    81,    37,    44,    43,    41,
      42,    47,    80,    80,    40,    80,    81,     3,    80,    40,
      80,    81,    85,     3,    81,    81,    81,    81,    81,    81,
      81,    81,    81,     3,    42,    43,    42,    66,    40,    81,
      72,    40,    81,    69,    40,    81,    39,    42,    54,    78,
      42,    37,    41,    41,    44,    41,    44,    42,    44,    80,
      48,    41,    41,    41,    81,    48,     3,    58,    59,    60,
      85,    51,     5,    39,    28,    42,    43,    42,    48,    62,
      81,    40,    60,    42,    47,    48,    41,    55
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    49,    50,    51,
      52,    52,    53,    54,    55,    56,    57,    58,    58,    58,
      59,    59,    59,    60,    60,    61,    62,    62,    63,    63,
      63,    63,    64,    65,    66,    66,    67,    67,    67,    68,
      69,    69,    70,    70,    70,    71,    72,    72,    73,    73,
      73,    74,    74,    75,    76,    76,    76,    76,    76,    76,
      77,    77,    78,    78,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    83,    83,    83,    83,    83,
      83,    83,    83,    84,    84,    84,    85,    85,    85,    86,
      86
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     2,     2,     1,     1,
       1,     1,     2,     2,     2,     1,     1,     5,     0,     0,
       1,     2,     2,     0,     0,    10,     1,     1,     2,     0,
       3,     2,     1,     1,     3,     9,     1,     0,     1,     1,
       1,     1,     5,     2,     3,     1,     1,     3,     4,     2,
       3,     1,     1,     3,     4,     2,     3,     1,     1,     3,
       4,     5,     7,     1,     4,     3,     4,     3,     4,     3,
       3,     2,     3,     1,     1,     3,     3,     3,     3,     3,
       4,     4,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     3,
       1,     4,     1,     4,     4,     6,     1,     2,     0,     1,
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
#line 168 "mini_js.y"
         { print( resolve_enderecos( yyvsp[0].c + "." + funcoes ) ); }
#line 1615 "y.tab.c"
    break;

  case 3: /* CMDs: CMDs CMD  */
#line 171 "mini_js.y"
                 { yyval.c = yyvsp[-1].c + yyvsp[0].c; }
#line 1621 "y.tab.c"
    break;

  case 4: /* CMDs: %empty  */
#line 172 "mini_js.y"
                 { yyval.clear(); }
#line 1627 "y.tab.c"
    break;

  case 14: /* CMD: ATRIB ';'  */
#line 188 "mini_js.y"
      { yyval.c = yyvsp[-1].c + "^"; }
#line 1633 "y.tab.c"
    break;

  case 16: /* CMD: ';'  */
#line 191 "mini_js.y"
      { yyval.clear(); }
#line 1639 "y.tab.c"
    break;

  case 17: /* BLOCO: '{' START_BLOCO CMDs '}' END_BLOCO  */
#line 195 "mini_js.y"
        { //if ($3.c.size() > 0)
            yyval.c = "<{" + yyvsp[-2].c + "}>" ; 
          //else
            //$$.c = $3.c;
        }
#line 1649 "y.tab.c"
    break;

  case 18: /* START_BLOCO: %empty  */
#line 202 "mini_js.y"
              {empilha_escopo_novo();}
#line 1655 "y.tab.c"
    break;

  case 19: /* END_BLOCO: %empty  */
#line 204 "mini_js.y"
            {desempilha_escopo();}
#line 1661 "y.tab.c"
    break;

  case 20: /* CMD_RET: RETURN  */
#line 208 "mini_js.y"
          { 
            yyval.c = vector<string>{"undefined"}; /* 1. Empilha undefined */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            yyval.c =  yyval.c + vector<string>{ "'&retorno'", "@", "~" }; /* 3. Instruções de retorno */
          }
#line 1674 "y.tab.c"
    break;

  case 21: /* CMD_RET: RETURN E  */
#line 217 "mini_js.y"
          { 
            yyval.c = yyvsp[0].c; /* 1. Empilha o valor da expressão E */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            /* 3. Instruções de retorno (NÃO adicione $2.c de novo) */
            yyval.c = yyval.c + vector<string>{ "'&retorno'", "@", "~" }; 
          }
#line 1688 "y.tab.c"
    break;

  case 22: /* CMD_ASM: E ASM  */
#line 228 "mini_js.y"
                { yyval.c = yyvsp[-1].c + yyvsp[0].c + "^"; }
#line 1694 "y.tab.c"
    break;

  case 23: /* EMPILHA_TS_FUNC: %empty  */
#line 231 "mini_js.y"
                  { ts.push_back( map< string, Simbolo >{} ); 
                blocos_alinhados_em_funcao.push_back(0);}
#line 1701 "y.tab.c"
    break;

  case 24: /* DESEMPILHA_TS_FUNC: %empty  */
#line 235 "mini_js.y"
                     { ts.pop_back(); 
                      blocos_alinhados_em_funcao.pop_back();}
#line 1708 "y.tab.c"
    break;

  case 25: /* CMD_FUNC: FUNCTION NOME_FUNCAO '(' EMPILHA_TS_FUNC LISTA_PARAMs ')' '{' CMDs '}' DESEMPILHA_TS_FUNC  */
#line 240 "mini_js.y"
           { 
             /* Note que os índices mudaram: NOME_FUNCAO é $2, LISTA_PARAMs é $5, CMDs é $8 */
             string definicao_lbl_endereco_funcao = ":" + yyvsp[-8].endereco_funcao;
             
             yyval.c = yyvsp[-8].c ; /* Código de NOME_FUNCAO (declara var, atribui obj função) */

             funcoes = funcoes + definicao_lbl_endereco_funcao + yyvsp[-5].c + yyvsp[-2].c +
                         "undefined" + "@" + "'&retorno'" + "@"+ "~";
             
           }
#line 1723 "y.tab.c"
    break;

  case 26: /* NOME_FUNCAO: ID  */
#line 267 "mini_js.y"
            { 
              string endereco_funcao = gera_label( "func_" + yyvsp[0].c[0] );
              yyval.endereco_funcao = endereco_funcao; /* 1. Salva o label para CMD_FUNC usar */
              
              Atributos decl = declara_var( Var, yyvsp[0] ); // 2. Declara a variável
              
              /* 3. Gera código usando decl.c (que pode ou não ter '&') e a var local 'endereco_funcao' */
              yyval.c = decl.c +                                    /* 'foo &' ou nada se for redeclaração */
                     yyvsp[0].c + "{}" + "="  + "'&funcao'" + endereco_funcao +  /* 'foo '&funcao' func_foo_1' ... */
                     "[=]" + "^";                              /* ... '[=] ^' (atribui prop) */
            }
#line 1739 "y.tab.c"
    break;

  case 29: /* LISTA_PARAMs: %empty  */
#line 282 "mini_js.y"
             { yyval.clear(); }
#line 1745 "y.tab.c"
    break;

  case 30: /* PARAMs: PARAMs ',' PARAM  */
#line 286 "mini_js.y"
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
#line 1769 "y.tab.c"
    break;

  case 32: /* PARAMs: PARAM  */
#line 307 "mini_js.y"
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
#line 1792 "y.tab.c"
    break;

  case 33: /* PARAM: ID  */
#line 328 "mini_js.y"
      { yyval = yyvsp[0]; 
        yyval.n_args = 1;
        yyval.valor_default.clear(); }
#line 1800 "y.tab.c"
    break;

  case 34: /* PARAM: ID '=' E  */
#line 332 "mini_js.y"
      { 
        yyval = yyvsp[-2];
        yyval.n_args = 1;
        yyval.valor_default = yyvsp[0].c; }
#line 1809 "y.tab.c"
    break;

  case 35: /* CMD_FOR: FOR '(' SF ';' E ';' EF ')' CMD  */
#line 339 "mini_js.y"
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          yyval.c = yyvsp[-6].c + define_label (teste_for) +
                 yyvsp[-4].c + "!" + fim_for + "?" + yyvsp[0].c + 
                 yyvsp[-2].c + teste_for + "#" + define_label(fim_for);
        }
#line 1821 "y.tab.c"
    break;

  case 36: /* EF: ATRIB  */
#line 349 "mini_js.y"
      { yyval.c = yyvsp[0].c + "^"; }
#line 1827 "y.tab.c"
    break;

  case 37: /* EF: %empty  */
#line 350 "mini_js.y"
      { yyval.clear(); }
#line 1833 "y.tab.c"
    break;

  case 42: /* CMD_WHILE: WHILE '(' E ')' CMD  */
#line 360 "mini_js.y"
        { string inicio_while = gera_label( "inicio_while" );
          string fim_while = gera_label( "fim_while" );
          yyval.c = define_label(inicio_while) + 
                 yyvsp[-2].c + "!" + fim_while + "?" + yyvsp[0].c + 
                 inicio_while + "#" + define_label(fim_while);
        }
#line 1844 "y.tab.c"
    break;

  case 43: /* CMD_LET: LET LET_VARs  */
#line 368 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1850 "y.tab.c"
    break;

  case 44: /* LET_VARs: LET_VAR ',' LET_VARs  */
#line 371 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1856 "y.tab.c"
    break;

  case 46: /* LET_VAR: LVALUE  */
#line 376 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[0] ).c; }
#line 1862 "y.tab.c"
    break;

  case 47: /* LET_VAR: LVALUE '=' E  */
#line 378 "mini_js.y"
          { 
            yyval.c = declara_var( Let, yyvsp[-2] ).c + // ex: a &
                   yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1870 "y.tab.c"
    break;

  case 48: /* LET_VAR: LVALUE '=' '{' '}'  */
#line 382 "mini_js.y"
          {
            yyval.c = declara_var( Let, yyvsp[-3] ).c +
            yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1878 "y.tab.c"
    break;

  case 49: /* CMD_VAR: VAR VAR_VARs  */
#line 387 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1884 "y.tab.c"
    break;

  case 50: /* VAR_VARs: VAR_VAR ',' VAR_VARs  */
#line 390 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1890 "y.tab.c"
    break;

  case 52: /* VAR_VAR: LVALUE  */
#line 395 "mini_js.y"
          { yyval.c = declara_var( Var, yyvsp[0] ).c; }
#line 1896 "y.tab.c"
    break;

  case 53: /* VAR_VAR: LVALUE '=' E  */
#line 397 "mini_js.y"
          {  yyval.c = declara_var( Var, yyvsp[-2] ).c + 
                    yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1903 "y.tab.c"
    break;

  case 54: /* VAR_VAR: LVALUE '=' '{' '}'  */
#line 400 "mini_js.y"
          {
            yyval.c = declara_var( Var, yyvsp[-3] ).c +
            yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1911 "y.tab.c"
    break;

  case 55: /* CMD_CONST: CONST CONST_VARs  */
#line 405 "mini_js.y"
                            { yyval.c = yyvsp[0].c; }
#line 1917 "y.tab.c"
    break;

  case 56: /* CONST_VARs: CONST_VAR ',' CONST_VARs  */
#line 408 "mini_js.y"
                                      { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1923 "y.tab.c"
    break;

  case 58: /* CONST_VAR: LVALUE  */
#line 413 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[0] ).c; }
#line 1929 "y.tab.c"
    break;

  case 59: /* CONST_VAR: LVALUE '=' E  */
#line 415 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[-2] ).c + 
                     yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1936 "y.tab.c"
    break;

  case 60: /* CONST_VAR: LVALUE '=' '{' '}'  */
#line 418 "mini_js.y"
            {
              yyval.c = declara_var( Const, yyvsp[-3] ).c +
              yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1944 "y.tab.c"
    break;

  case 61: /* CMD_IF: IF '(' E ')' CMD  */
#line 424 "mini_js.y"
          {
            string fim_if = gera_label( "fim_if" );
            yyval.c = yyvsp[-2].c + "!" + fim_if + "?" + yyvsp[0].c + define_label(fim_if);
          }
#line 1953 "y.tab.c"
    break;

  case 62: /* CMD_IF: IF '(' E ')' CMD ELSE CMD  */
#line 430 "mini_js.y"
          {  string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            yyval.c = yyvsp[-4].c + "!" + else_if + "?" + yyvsp[-2].c + fim_if + 
            "#" + define_label(else_if) + yyvsp[0].c +  define_label(fim_if);
          }
#line 1963 "y.tab.c"
    break;

  case 64: /* LVALUEPROP: LVALUEPROP '[' E ']'  */
#line 441 "mini_js.y"
            { yyval.c = yyvsp[-3].c + "[@]" + yyvsp[-1].c; }
#line 1969 "y.tab.c"
    break;

  case 65: /* LVALUEPROP: LVALUEPROP '.' ID  */
#line 443 "mini_js.y"
            { yyval.c = yyvsp[-2].c + "[@]"  + yyvsp[0].c[0] ; }
#line 1975 "y.tab.c"
    break;

  case 66: /* LVALUEPROP: F '[' E ']'  */
#line 445 "mini_js.y"
            { yyval.c = yyvsp[-3].c + yyvsp[-1].c; }
#line 1981 "y.tab.c"
    break;

  case 67: /* LVALUEPROP: F '.' ID  */
#line 447 "mini_js.y"
            { yyval.c = yyvsp[-2].c  + yyvsp[0].c[0] ; }
#line 1987 "y.tab.c"
    break;

  case 68: /* LVALUEPROP: LVALUE '[' E ']'  */
#line 449 "mini_js.y"
            { yyval.c = yyvsp[-3].c + "@" + yyvsp[-1].c; }
#line 1993 "y.tab.c"
    break;

  case 69: /* LVALUEPROP: LVALUE '.' ID  */
#line 451 "mini_js.y"
            { yyval.c = yyvsp[-2].c + "@"  + yyvsp[0].c[0] ; }
#line 1999 "y.tab.c"
    break;

  case 70: /* LIST: '[' LISTVALS ']'  */
#line 454 "mini_js.y"
                         { yyval.c = yyvsp[-2].c + yyvsp[-1].c + yyvsp[0].c; }
#line 2005 "y.tab.c"
    break;

  case 71: /* LIST: '[' ']'  */
#line 455 "mini_js.y"
                { yyval.c = vector<string>{"[]"}; }
#line 2011 "y.tab.c"
    break;

  case 72: /* LISTVALS: LISTVAL ',' LISTVALS  */
#line 458 "mini_js.y"
                                  { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 2017 "y.tab.c"
    break;

  case 75: /* ATRIB: LVALUE '=' ATRIB  */
#line 467 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[0].c + "="; }
#line 2023 "y.tab.c"
    break;

  case 76: /* ATRIB: LVALUE MAIS_IGUAL ATRIB  */
#line 469 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "+" + "="; }
#line 2029 "y.tab.c"
    break;

  case 77: /* ATRIB: LVALUE MENOS_IGUAL ATRIB  */
#line 471 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "-" + "="; }
#line 2035 "y.tab.c"
    break;

  case 78: /* ATRIB: LVALUEPROP '=' ATRIB  */
#line 473 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + "[=]"; }
#line 2041 "y.tab.c"
    break;

  case 79: /* ATRIB: LVALUEPROP MAIS_IGUAL ATRIB  */
#line 475 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; }
#line 2047 "y.tab.c"
    break;

  case 80: /* ATRIB: LVALUE '=' '{' '}'  */
#line 477 "mini_js.y"
      { checa_simbolo( yyvsp[-3].c[0], true ); yyval.c = yyvsp[-3].c + "{}" + "="; }
#line 2053 "y.tab.c"
    break;

  case 81: /* ATRIB: LVALUEPROP '=' '{' '}'  */
#line 479 "mini_js.y"
      { yyval.c = yyvsp[-3].c + "{}" + "[=]"; }
#line 2059 "y.tab.c"
    break;

  case 83: /* E: LVALUEPROP  */
#line 484 "mini_js.y"
    { yyval.c = yyvsp[0].c + "[@]"; }
#line 2065 "y.tab.c"
    break;

  case 84: /* E: LVALUE  */
#line 486 "mini_js.y"
      { //checa_simbolo( $1.c[0], false ); 
        yyval.c = yyvsp[0].c + "@"; }
#line 2072 "y.tab.c"
    break;

  case 85: /* E: E IGUAL E  */
#line 489 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2078 "y.tab.c"
    break;

  case 86: /* E: E '<' E  */
#line 491 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2084 "y.tab.c"
    break;

  case 87: /* E: E '>' E  */
#line 493 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2090 "y.tab.c"
    break;

  case 88: /* E: E '+' E  */
#line 495 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2096 "y.tab.c"
    break;

  case 89: /* E: E '-' E  */
#line 497 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2102 "y.tab.c"
    break;

  case 90: /* E: E '*' E  */
#line 499 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2108 "y.tab.c"
    break;

  case 91: /* E: E '/' E  */
#line 501 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2114 "y.tab.c"
    break;

  case 92: /* E: E '%' E  */
#line 503 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2120 "y.tab.c"
    break;

  case 93: /* E: '-' E  */
#line 505 "mini_js.y"
    { yyval.c = "0" + yyvsp[0].c + "-" ; }
#line 2126 "y.tab.c"
    break;

  case 94: /* E: '+' E  */
#line 507 "mini_js.y"
    { yyval.c = "0" + yyvsp[0].c + "+" ; }
#line 2132 "y.tab.c"
    break;

  case 95: /* E: UN  */
#line 509 "mini_js.y"
    { yyval.c = yyvsp[0].c; }
#line 2138 "y.tab.c"
    break;

  case 96: /* UN: MAIS_MAIS LVALUE  */
#line 513 "mini_js.y"
    {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "+" + "=";}
#line 2144 "y.tab.c"
    break;

  case 97: /* UN: MENOS_MENOS LVALUE  */
#line 515 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "-" + "=";}
#line 2150 "y.tab.c"
    break;

  case 98: /* UN: LVALUE MAIS_MAIS  */
#line 517 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "+" + "=" + "^";}
#line 2156 "y.tab.c"
    break;

  case 99: /* UN: LVALUE MENOS_MENOS  */
#line 519 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "-" + "=" + "^";}
#line 2162 "y.tab.c"
    break;

  case 100: /* UN: MAIS_MAIS LVALUEPROP  */
#line 521 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "[@]" + "1" + "+" + "[=]";}
#line 2168 "y.tab.c"
    break;

  case 101: /* UN: MENOS_MENOS LVALUEPROP  */
#line 523 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "[@]" + "1" + "-" + "[=]";}
#line 2174 "y.tab.c"
    break;

  case 102: /* UN: LVALUEPROP MAIS_MAIS  */
#line 525 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "[@]" + yyvsp[-1].c + yyvsp[-1].c + "[@]" + "1" + "+" + "[=]" + "^";}
#line 2180 "y.tab.c"
    break;

  case 103: /* UN: LVALUEPROP MENOS_MENOS  */
#line 527 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "[@]" + yyvsp[-1].c + yyvsp[-1].c + "[@]" + "1" + "-" + "[=]" + "^";}
#line 2186 "y.tab.c"
    break;

  case 109: /* F: '(' E ')'  */
#line 536 "mini_js.y"
      { yyval.c = yyvsp[-1].c; }
#line 2192 "y.tab.c"
    break;

  case 111: /* F: '(' '{' '}' ')'  */
#line 539 "mini_js.y"
      { yyval.c = vector<string>{"{}"}; }
#line 2198 "y.tab.c"
    break;

  case 113: /* CHAMA_FUNC: ID '(' LISTA_ARGS ')'  */
#line 544 "mini_js.y"
              { 
                yyval.c = yyvsp[-1].c + // 1. Empilha args e contagem
                       yyvsp[-3].c[0] + "@" + // 2. Empilha objeto da função (lendo da var)
                  
                       "$"  // 6. CHAMA (a instrução '$')
                       ;
              }
#line 2210 "y.tab.c"
    break;

  case 114: /* CHAMA_FUNC: LVALUEPROP '(' LISTA_ARGS ')'  */
#line 552 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + // 1. Empilha args e contagem
                       yyvsp[-3].c + "[@]" + // 2. Empilha objeto da função (LVALUEPROP + [@])
                        "$"  // 6. CHAMA (a instrução '$')
                        ;
              }
#line 2222 "y.tab.c"
    break;

  case 115: /* CHAMA_FUNC: '(' E ')' '(' LISTA_ARGS ')'  */
#line 561 "mini_js.y"
            { 
              string lbl_retorno = gera_label( "retorno_funcao" );
              yyval.c = yyvsp[-1].c + 
                   yyvsp[-4].c + 
                    //"@" +
                   "$"  // 6. CHAMA (a instrução '$') + "^";
                    ;

           }
#line 2236 "y.tab.c"
    break;

  case 116: /* LISTA_ARGS: ARGs  */
#line 573 "mini_js.y"
           { 
             yyval.c = yyvsp[0].c + to_string( yyvsp[0].n_args ); 
             yyval.n_args = yyvsp[0].n_args;
           }
#line 2245 "y.tab.c"
    break;

  case 117: /* LISTA_ARGS: ARGs ','  */
#line 578 "mini_js.y"
           {
             yyval.c = yyvsp[-1].c + to_string( yyvsp[-1].n_args ); 
             yyval.n_args = yyvsp[-1].n_args;
           }
#line 2254 "y.tab.c"
    break;

  case 118: /* LISTA_ARGS: %empty  */
#line 583 "mini_js.y"
           { 
              yyval.c = vector<string>{"0"}; 
              yyval.n_args = 0; 
           }
#line 2263 "y.tab.c"
    break;

  case 119: /* ARGs: ATRIB  */
#line 590 "mini_js.y"
      { 
        yyval.c = yyvsp[0].c;     
        yyval.n_args = 1;   
      }
#line 2272 "y.tab.c"
    break;

  case 120: /* ARGs: ARGs ',' ATRIB  */
#line 595 "mini_js.y"
      { 
        yyval.c = yyvsp[-2].c + yyvsp[0].c; 
        yyval.n_args = yyvsp[-2].n_args + 1; 
      }
#line 2281 "y.tab.c"
    break;


#line 2285 "y.tab.c"

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

#line 605 "mini_js.y"


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



