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

map<TipoDecl, string> nomeTipoDecl = { 
  { Let, "let" }, 
  { Const, "const" }, 
  { Var, "var" }
};

struct Simbolo {
  TipoDecl tipo;
  int linha;
  int coluna;
};

int in_func = 0;

// Tabela de símbolos - agora é uma pilha
vector< map< string, Simbolo > > ts = { map< string, Simbolo >{} }; 
vector<string> funcoes;


Atributos declara_var( TipoDecl tipo, Atributos atrib );
void checa_simbolo( string nome, bool modificavel );
// --- CORREÇÃO 2: Adicionada função 'busca_simbolo' para escopo aninhado ---
Simbolo* busca_simbolo( string nome );


// Armazena o código (instruções) de todas as funções encontradas.
// vector<string> funcoes;

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

#line 211 "y.tab.c"

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
    PRINT = 264,                   /* PRINT  */
    FOR = 265,                     /* FOR  */
    WHILE = 266,                   /* WHILE  */
    FUNCTION = 267,                /* FUNCTION  */
    RETURN = 268,                  /* RETURN  */
    ASM = 269,                     /* ASM  */
    CDOUBLE = 270,                 /* CDOUBLE  */
    CSTRING = 271,                 /* CSTRING  */
    CINT = 272,                    /* CINT  */
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
#define PRINT 264
#define FOR 265
#define WHILE 266
#define FUNCTION 267
#define RETURN 268
#define ASM 269
#define CDOUBLE 270
#define CSTRING 271
#define CINT 272
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
  YYSYMBOL_PRINT = 9,                      /* PRINT  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_FUNCTION = 12,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 13,                    /* RETURN  */
  YYSYMBOL_ASM = 14,                       /* ASM  */
  YYSYMBOL_CDOUBLE = 15,                   /* CDOUBLE  */
  YYSYMBOL_CSTRING = 16,                   /* CSTRING  */
  YYSYMBOL_CINT = 17,                      /* CINT  */
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
  YYSYMBOL_EMPILHA_TS = 49,                /* EMPILHA_TS  */
  YYSYMBOL_CMD_FUNC = 50,                  /* CMD_FUNC  */
  YYSYMBOL_51_1 = 51,                      /* $@1  */
  YYSYMBOL_NOME_FUNCAO = 52,               /* NOME_FUNCAO  */
  YYSYMBOL_LISTA_PARAMs = 53,              /* LISTA_PARAMs  */
  YYSYMBOL_PARAMs = 54,                    /* PARAMs  */
  YYSYMBOL_PARAM = 55,                     /* PARAM  */
  YYSYMBOL_CMD_FOR = 56,                   /* CMD_FOR  */
  YYSYMBOL_EF = 57,                        /* EF  */
  YYSYMBOL_SF = 58,                        /* SF  */
  YYSYMBOL_CMD_WHILE = 59,                 /* CMD_WHILE  */
  YYSYMBOL_CMD_LET = 60,                   /* CMD_LET  */
  YYSYMBOL_LET_VARs = 61,                  /* LET_VARs  */
  YYSYMBOL_LET_VAR = 62,                   /* LET_VAR  */
  YYSYMBOL_CMD_VAR = 63,                   /* CMD_VAR  */
  YYSYMBOL_VAR_VARs = 64,                  /* VAR_VARs  */
  YYSYMBOL_VAR_VAR = 65,                   /* VAR_VAR  */
  YYSYMBOL_CMD_CONST = 66,                 /* CMD_CONST  */
  YYSYMBOL_CONST_VARs = 67,                /* CONST_VARs  */
  YYSYMBOL_CONST_VAR = 68,                 /* CONST_VAR  */
  YYSYMBOL_CMD_IF = 69,                    /* CMD_IF  */
  YYSYMBOL_LVALUE = 70,                    /* LVALUE  */
  YYSYMBOL_LVALUEPROP = 71,                /* LVALUEPROP  */
  YYSYMBOL_LIST = 72,                      /* LIST  */
  YYSYMBOL_LISTVALS = 73,                  /* LISTVALS  */
  YYSYMBOL_LISTVAL = 74,                   /* LISTVAL  */
  YYSYMBOL_ATRIB = 75,                     /* ATRIB  */
  YYSYMBOL_E = 76,                         /* E  */
  YYSYMBOL_UN = 77,                        /* UN  */
  YYSYMBOL_F = 78,                         /* F  */
  YYSYMBOL_CHAMA_FUNC = 79,                /* CHAMA_FUNC  */
  YYSYMBOL_LISTA_ARGS = 80,                /* LISTA_ARGS  */
  YYSYMBOL_ARGs = 81                       /* ARGs  */
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
#define YYLAST   658

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  112
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  207

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
       0,   161,   161,   164,   165,   169,   170,   171,   172,   173,
     174,   175,   179,   181,   183,   184,   185,   188,   190,   194,
     198,   198,   212,   217,   218,   222,   234,   248,   252,   259,
     269,   271,   274,   275,   276,   277,   280,   289,   292,   293,
     296,   298,   302,   308,   311,   312,   315,   317,   320,   326,
     329,   330,   333,   335,   338,   344,   350,   358,   363,   365,
     367,   369,   371,   374,   379,   380,   383,   384,   387,   391,
     393,   395,   397,   399,   401,   404,   406,   409,   411,   414,
     416,   418,   420,   422,   424,   426,   428,   430,   432,   434,
     438,   441,   444,   447,   450,   452,   454,   457,   459,   462,
     463,   464,   465,   467,   468,   470,   475,   488,   501,   517,
     518,   521,   523
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
  "LET", "CONST", "VAR", "PRINT", "FOR", "WHILE", "FUNCTION", "RETURN",
  "ASM", "CDOUBLE", "CSTRING", "CINT", "AND", "OR", "ME_IG", "MA_IG",
  "DIF", "IGUAL", "MAIS_IGUAL", "MAIS_MAIS", "MENOS_IGUAL", "MENOS_MENOS",
  "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'['", "'('",
  "'.'", "';'", "'{'", "'}'", "')'", "','", "']'", "$accept", "S", "CMDs",
  "CMD", "EMPILHA_TS", "CMD_FUNC", "$@1", "NOME_FUNCAO", "LISTA_PARAMs",
  "PARAMs", "PARAM", "CMD_FOR", "EF", "SF", "CMD_WHILE", "CMD_LET",
  "LET_VARs", "LET_VAR", "CMD_VAR", "VAR_VARs", "VAR_VAR", "CMD_CONST",
  "CONST_VARs", "CONST_VAR", "CMD_IF", "LVALUE", "LVALUEPROP", "LIST",
  "LISTVALS", "LISTVAL", "ATRIB", "E", "UN", "F", "CHAMA_FUNC",
  "LISTA_ARGS", "ARGs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-150)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -150,     1,   333,  -150,   -27,   -14,    29,    29,    29,    89,
       3,    15,    58,   256,  -150,  -150,  -150,     5,     5,    89,
      89,    28,   124,  -150,  -150,  -150,  -150,  -150,  -150,    34,
      35,    36,  -150,    42,    10,  -150,    37,   468,  -150,   -19,
    -150,    89,    89,  -150,  -150,    19,    43,  -150,    39,    55,
    -150,    46,    57,    72,   132,   571,   458,    89,  -150,  -150,
    -150,   584,    -9,   -24,   -19,    -9,   -24,    23,    23,  -150,
      50,    52,   610,    59,   529,  -150,  -150,  -150,  -150,    89,
    -150,    89,  -150,   351,    89,    95,    89,  -150,  -150,   377,
      89,    89,    98,  -150,    63,    89,    89,    89,    89,    89,
      89,    89,    89,    89,   100,   610,    70,    74,   543,    29,
     395,    29,   413,    29,   431,  -150,  -150,    76,  -150,  -150,
    -150,  -150,   610,   557,    81,  -150,  -150,    89,    71,    82,
     238,  -150,  -150,    83,  -150,   481,  -150,  -150,    90,  -150,
     497,    80,  -150,  -150,   623,   159,   159,    23,    23,  -150,
    -150,  -150,   513,  -150,  -150,    89,   333,  -150,    91,   610,
    -150,    92,   610,  -150,    94,   610,    89,   333,  -150,  -150,
    -150,    89,  -150,  -150,  -150,  -150,  -150,  -150,  -150,   610,
     118,  -150,  -150,  -150,   597,  -150,   127,   101,   333,    89,
     114,   106,   107,  -150,  -150,  -150,   110,    89,   113,   127,
     333,   610,  -150,  -150,  -150,   294,  -150
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,   101,   100,     0,     0,     0,
       0,     0,     0,    18,    19,     3,    10,    14,    15,     0,
       0,     0,     9,    78,    77,   103,     0,    76,    89,    98,
     105,   110,     0,    57,    37,    39,    40,    49,    51,    52,
      43,    45,    46,    78,    77,     0,    31,     0,    22,    20,
      12,     0,    90,    94,     0,    91,    95,    88,    87,    65,
       0,    67,    68,     0,     0,     4,     6,     7,     8,     0,
      92,     0,    93,     0,     0,     0,     0,    96,    97,     0,
       0,   110,     0,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,   109,     0,     0,
       0,     0,     0,     0,     0,    11,    35,     0,    32,    33,
      34,    30,    76,     0,     0,    13,    64,     0,     0,   102,
       0,    70,    71,     0,    69,     0,    63,    73,     0,    72,
       0,     0,    59,     5,    79,    80,    81,    82,    83,    84,
      85,    86,     0,    61,   106,     0,     0,    38,     0,    41,
      50,     0,    53,    44,     0,    47,     0,     0,    19,    66,
     104,   110,    17,    74,    62,    75,    58,   107,    60,   112,
      55,    42,    54,    48,     0,    36,    24,     0,     0,    31,
      27,     0,    23,    26,   108,    56,     0,     0,     0,     0,
       0,    28,     4,    25,    29,     0,    21
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -150,  -150,   -73,  -149,   -10,  -150,  -150,  -150,  -150,  -150,
     -37,  -150,   -18,  -150,  -150,   116,    64,  -150,   119,    65,
    -150,   120,    66,  -150,  -150,    -2,     7,  -150,    53,  -150,
     -53,   125,  -150,    32,  -150,   -80,  -150
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    25,    75,    26,   124,    59,   191,   192,
     193,    27,   116,   117,    28,    29,    44,    45,    30,    50,
      51,    31,    47,    48,    32,    53,    54,    35,    70,    71,
      36,    37,    38,    39,    40,   106,   107
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,     3,   130,   121,    46,    49,    52,   180,     4,    34,
      41,   141,    90,    91,    92,    62,    65,   103,   185,   104,
      14,    15,    16,    42,    63,    66,   131,    84,   132,    85,
     134,     4,    43,   137,    86,    87,   139,    88,    89,   195,
      56,    21,    22,    14,    15,    16,    90,    91,    92,    64,
      64,   204,    57,    17,    33,    18,   100,   101,   102,    19,
      20,    58,   109,    34,    21,    22,    79,    80,    81,    82,
      83,   110,    69,    76,    77,    78,    93,    33,    84,    33,
      85,    33,   111,   112,    33,   114,    34,    33,    34,   113,
      34,   187,     4,    34,   126,   127,    34,    80,   136,    82,
     128,   142,   143,   153,    14,    15,    16,    46,    84,    49,
      85,    52,   154,   170,    17,   166,    18,   155,   168,   171,
      19,    20,   177,   188,   173,    21,    22,     4,    33,   205,
     190,   175,   181,   182,    55,   183,   121,    34,    61,    14,
      15,    16,   197,   194,    67,    68,    72,    74,   198,    17,
     199,    18,   200,   202,    33,    19,    20,    87,   186,    88,
      21,    22,   203,    34,    73,    33,   105,   108,    90,    91,
      92,   196,   118,   157,    34,   119,   120,   160,   163,     0,
     169,   122,   123,     0,     0,     0,    33,    33,    -1,    -1,
      98,    99,   100,   101,   102,    34,    34,     0,    33,     0,
       0,     0,     0,    33,   122,     0,   122,    34,   122,   135,
       0,   122,    34,     0,   122,   140,   105,     0,     0,     0,
     144,   145,   146,   147,   148,   149,   150,   151,   152,     0,
       0,     0,     0,     0,     0,   159,     0,   162,     0,   165,
       0,     4,     5,     0,     6,     7,     8,     9,    10,    11,
      12,    13,    72,    14,    15,    16,     0,     0,     0,     4,
       0,     0,     0,    17,     0,    18,     0,     0,     0,    19,
      20,    14,    15,    16,    21,    22,     0,    23,    24,   172,
     179,    17,     0,    18,     0,     0,     0,    19,    20,     0,
       0,   184,    21,    22,     0,    60,   105,     4,     5,     0,
       6,     7,     8,     9,    10,    11,    12,    13,     0,    14,
      15,    16,     0,     0,   122,     0,     0,     0,     0,    17,
       0,    18,   201,     0,     0,    19,    20,     0,     0,     0,
      21,    22,     0,    23,    24,   206,     4,     5,     0,     6,
       7,     8,     9,    10,    11,    12,    13,     0,    14,    15,
      16,     0,     0,     0,     4,     0,     0,     0,    17,     0,
      18,     0,     0,     0,    19,    20,    14,    15,    16,    21,
      22,     0,    23,    24,     0,     0,    17,     0,    18,     0,
       4,     0,    19,    20,     0,     0,     0,    21,    22,     0,
       0,   133,    14,    15,    16,     0,     0,     0,     4,     0,
       0,     0,    17,     0,    18,     0,     0,     0,    19,    20,
      14,    15,    16,    21,    22,     0,     4,   138,     0,     0,
      17,     0,    18,     0,     0,     0,    19,    20,    14,    15,
      16,    21,    22,     0,     4,   158,     0,     0,    17,     0,
      18,     0,     0,     0,    19,    20,    14,    15,    16,    21,
      22,     0,     0,   161,     0,     0,    17,     0,    18,     0,
       0,     4,    19,    20,     6,     7,     8,    21,    22,     0,
       0,   164,     0,    14,    15,    16,     0,     0,     0,     0,
       0,     0,    94,    17,     0,    18,     0,     0,     0,    19,
      20,    95,     0,     0,    21,    22,     0,    96,    97,    98,
      99,   100,   101,   102,    95,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,   101,   102,     0,     0,     0,
      95,     0,     0,     0,     0,   174,    96,    97,    98,    99,
     100,   101,   102,     0,     0,     0,    95,     0,     0,     0,
       0,   176,    96,    97,    98,    99,   100,   101,   102,     0,
       0,     0,    95,     0,     0,     0,     0,   178,    96,    97,
      98,    99,   100,   101,   102,     0,    95,     0,     0,     0,
       0,   129,    96,    97,    98,    99,   100,   101,   102,     0,
      95,     0,     0,     0,     0,   156,    96,    97,    98,    99,
     100,   101,   102,     0,    95,     0,     0,     0,     0,   167,
      96,    97,    98,    99,   100,   101,   102,    95,     0,     0,
     115,     0,     0,    96,    97,    98,    99,   100,   101,   102,
      95,     0,     0,   125,     0,     0,    96,    97,    98,    99,
     100,   101,   102,    95,     0,     0,   189,     0,     0,    96,
      97,    98,    99,   100,   101,   102,    -1,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,   101,   102
};

static const yytype_int16 yycheck[] =
{
       2,     0,    75,    56,     6,     7,     8,   156,     3,     2,
      37,    91,    36,    37,    38,    17,    18,    36,   167,    38,
      15,    16,    17,    37,    17,    18,    79,    36,    81,    38,
      83,     3,     3,    86,    24,    25,    89,    27,    28,   188,
      37,    36,    37,    15,    16,    17,    36,    37,    38,    17,
      18,   200,    37,    25,    56,    27,    33,    34,    35,    31,
      32,     3,    43,    56,    36,    37,    24,    25,    26,    27,
      28,    28,    44,    39,    39,    39,    39,    79,    36,    81,
      38,    83,    43,    28,    86,    28,    79,    89,    81,    43,
      83,   171,     3,    86,    44,    43,    89,    25,     3,    27,
      41,     3,    39,     3,    15,    16,    17,   109,    36,   111,
      38,   113,    42,    42,    25,    39,    27,    43,    37,    37,
      31,    32,    42,     5,    41,    36,    37,     3,   130,   202,
       3,    41,    41,    41,     9,    41,   189,   130,    13,    15,
      16,    17,    28,    42,    19,    20,    21,    22,    42,    25,
      43,    27,    42,    40,   156,    31,    32,    25,   168,    27,
      36,    37,   199,   156,    40,   167,    41,    42,    36,    37,
      38,   189,    56,   109,   167,    56,    56,   111,   113,    -1,
     127,    56,    57,    -1,    -1,    -1,   188,   189,    29,    30,
      31,    32,    33,    34,    35,   188,   189,    -1,   200,    -1,
      -1,    -1,    -1,   205,    79,    -1,    81,   200,    83,    84,
      -1,    86,   205,    -1,    89,    90,    91,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,
      -1,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      12,    13,   127,    15,    16,    17,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      32,    15,    16,    17,    36,    37,    -1,    39,    40,    41,
     155,    25,    -1,    27,    -1,    -1,    -1,    31,    32,    -1,
      -1,   166,    36,    37,    -1,    39,   171,     3,     4,    -1,
       6,     7,     8,     9,    10,    11,    12,    13,    -1,    15,
      16,    17,    -1,    -1,   189,    -1,    -1,    -1,    -1,    25,
      -1,    27,   197,    -1,    -1,    31,    32,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    41,     3,     4,    -1,     6,
       7,     8,     9,    10,    11,    12,    13,    -1,    15,    16,
      17,    -1,    -1,    -1,     3,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    32,    15,    16,    17,    36,
      37,    -1,    39,    40,    -1,    -1,    25,    -1,    27,    -1,
       3,    -1,    31,    32,    -1,    -1,    -1,    36,    37,    -1,
      -1,    40,    15,    16,    17,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    32,
      15,    16,    17,    36,    37,    -1,     3,    40,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    32,    15,    16,
      17,    36,    37,    -1,     3,    40,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    32,    15,    16,    17,    36,
      37,    -1,    -1,    40,    -1,    -1,    25,    -1,    27,    -1,
      -1,     3,    31,    32,     6,     7,     8,    36,    37,    -1,
      -1,    40,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    25,    -1,    27,    -1,    -1,    -1,    31,
      32,    23,    -1,    -1,    36,    37,    -1,    29,    30,    31,
      32,    33,    34,    35,    23,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    44,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    44,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    44,    29,    30,
      31,    32,    33,    34,    35,    -1,    23,    -1,    -1,    -1,
      -1,    42,    29,    30,    31,    32,    33,    34,    35,    -1,
      23,    -1,    -1,    -1,    -1,    42,    29,    30,    31,    32,
      33,    34,    35,    -1,    23,    -1,    -1,    -1,    -1,    42,
      29,    30,    31,    32,    33,    34,    35,    23,    -1,    -1,
      39,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      23,    -1,    -1,    39,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    23,    -1,    -1,    39,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    23,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,    47,     0,     3,     4,     6,     7,     8,     9,
      10,    11,    12,    13,    15,    16,    17,    25,    27,    31,
      32,    36,    37,    39,    40,    48,    50,    56,    59,    60,
      63,    66,    69,    70,    71,    72,    75,    76,    77,    78,
      79,    37,    37,     3,    61,    62,    70,    67,    68,    70,
      64,    65,    70,    70,    71,    76,    37,    37,     3,    52,
      39,    76,    70,    71,    78,    70,    71,    76,    76,    44,
      73,    74,    76,    40,    76,    49,    39,    39,    39,    24,
      25,    26,    27,    28,    36,    38,    24,    25,    27,    28,
      36,    37,    38,    39,    14,    23,    29,    30,    31,    32,
      33,    34,    35,    36,    38,    76,    80,    81,    76,    43,
      28,    43,    28,    43,    28,    39,    57,    58,    60,    63,
      66,    75,    76,    76,    51,    39,    44,    43,    41,    42,
      47,    75,    75,    40,    75,    76,     3,    75,    40,    75,
      76,    80,     3,    39,    76,    76,    76,    76,    76,    76,
      76,    76,    76,     3,    42,    43,    42,    61,    40,    76,
      67,    40,    76,    64,    40,    76,    39,    42,    37,    73,
      42,    37,    41,    41,    44,    41,    44,    42,    44,    76,
      48,    41,    41,    41,    76,    48,    49,    80,     5,    39,
       3,    53,    54,    55,    42,    48,    57,    28,    42,    43,
      42,    76,    40,    55,    48,    47,    41
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    49,
      51,    50,    52,    53,    53,    54,    54,    55,    55,    56,
      57,    57,    58,    58,    58,    58,    59,    60,    61,    61,
      62,    62,    62,    63,    64,    64,    65,    65,    65,    66,
      67,    67,    68,    68,    68,    69,    69,    70,    71,    71,
      71,    71,    71,    71,    72,    72,    73,    73,    74,    75,
      75,    75,    75,    75,    75,    75,    75,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    78,
      78,    78,    78,    78,    78,    78,    79,    79,    79,    80,
      80,    81,    81
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     2,     2,     1,
       1,     3,     2,     3,     1,     1,     2,     4,     1,     0,
       0,    10,     1,     1,     0,     3,     1,     1,     3,     9,
       1,     0,     1,     1,     1,     1,     5,     2,     3,     1,
       1,     3,     4,     2,     3,     1,     1,     3,     4,     2,
       3,     1,     1,     3,     4,     5,     7,     1,     4,     3,
       4,     3,     4,     3,     3,     2,     3,     1,     1,     3,
       3,     3,     3,     3,     4,     4,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     3,     1,     4,     1,     4,     4,     6,     1,
       0,     1,     3
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
#line 161 "mini_js.y"
         { print( resolve_enderecos( yyvsp[0].c + "." + funcoes ) ); }
#line 1598 "y.tab.c"
    break;

  case 3: /* CMDs: CMDs CMD  */
#line 164 "mini_js.y"
                 { yyval.c = yyvsp[-1].c + yyvsp[0].c; }
#line 1604 "y.tab.c"
    break;

  case 4: /* CMDs: %empty  */
#line 165 "mini_js.y"
                 { yyval.clear(); }
#line 1610 "y.tab.c"
    break;

  case 5: /* CMD: E ASM ';'  */
#line 169 "mini_js.y"
                 { yyval.c = yyvsp[-2].c + yyvsp[-1].c; }
#line 1616 "y.tab.c"
    break;

  case 11: /* CMD: PRINT E ';'  */
#line 176 "mini_js.y"
      { yyval.c = yyvsp[-1].c + "println" + "#"; }
#line 1622 "y.tab.c"
    break;

  case 12: /* CMD: RETURN ';'  */
#line 180 "mini_js.y"
      { yyval.c = vector<string>{ "undefined", "&retorno", "=", "~" }; }
#line 1628 "y.tab.c"
    break;

  case 13: /* CMD: RETURN E ';'  */
#line 182 "mini_js.y"
      { yyval.c = yyvsp[-1].c + vector<string>{ "&retorno", "=", "~" }; }
#line 1634 "y.tab.c"
    break;

  case 16: /* CMD: ATRIB ';'  */
#line 186 "mini_js.y"
      { yyval.c = yyvsp[-1].c + "^"; }
#line 1640 "y.tab.c"
    break;

  case 17: /* CMD: '{' EMPILHA_TS CMDs '}'  */
#line 189 "mini_js.y"
      { yyval.c = yyvsp[-1].c; ts.pop_back(); }
#line 1646 "y.tab.c"
    break;

  case 18: /* CMD: ';'  */
#line 191 "mini_js.y"
      { yyval.clear(); }
#line 1652 "y.tab.c"
    break;

  case 19: /* EMPILHA_TS: %empty  */
#line 194 "mini_js.y"
             { ts.push_back( map< string, Simbolo >{} ); }
#line 1658 "y.tab.c"
    break;

  case 20: /* $@1: %empty  */
#line 198 "mini_js.y"
                                { /*declara_var( Let, $2 );*/ in_func++; }
#line 1664 "y.tab.c"
    break;

  case 21: /* CMD_FUNC: FUNCTION NOME_FUNCAO $@1 '(' EMPILHA_TS LISTA_PARAMs ')' '{' CMDs '}'  */
#line 200 "mini_js.y"
           { 
             string definicao_lbl_endereco_funcao = ":" + yyvsp[-8].endereco_funcao;
             
             yyval.c = yyvsp[-8].c ;

             funcoes = funcoes + definicao_lbl_endereco_funcao + yyvsp[-4].c + yyvsp[-1].c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
             in_func--;
           }
#line 1679 "y.tab.c"
    break;

  case 22: /* NOME_FUNCAO: ID  */
#line 213 "mini_js.y"
            { string endereco_funcao = gera_label( "func_" + yyvsp[0].c[0] );
              yyval.c = yyvsp[0].c + "&" + yyvsp[0].c + "{}" + "=" + "'&funcao'" + yyval.endereco_funcao + "[=]" + "^"; }
#line 1686 "y.tab.c"
    break;

  case 24: /* LISTA_PARAMs: %empty  */
#line 218 "mini_js.y"
             { yyval.clear(); }
#line 1692 "y.tab.c"
    break;

  case 25: /* PARAMs: PARAMs ',' PARAM  */
#line 223 "mini_js.y"
       { 
         Atributos decl = declara_var( Let, yyvsp[0] ); // Declara o parâmetro no escopo da função
         yyval.c = yyvsp[-2].c + decl.c + // Código de alocação (ex: msg &)
                yyvsp[0].c[0] + "arguments" + "@" + to_string( yyvsp[-2].n_args ) // Nome da var + ...
                + "[@]" + "=" + "^"; 
                
         if( yyvsp[0].valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         yyval.n_args = yyvsp[-2].n_args + yyvsp[0].n_args; 
       }
#line 1708 "y.tab.c"
    break;

  case 26: /* PARAMs: PARAM  */
#line 235 "mini_js.y"
       { 
         Atributos decl = declara_var( Let, yyvsp[0] ); // Declara o parâmetro no escopo da função
         yyval.c = decl.c + // Código de alocação (ex: msg &)
                yyvsp[0].c[0] + "arguments" + "@" + "0" + "[@]" + "=" + "^"; // Nome da var + ...
                
         if( yyvsp[0].valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         yyval.n_args = yyvsp[0].n_args; 
       }
#line 1723 "y.tab.c"
    break;

  case 27: /* PARAM: ID  */
#line 249 "mini_js.y"
      { yyval = yyvsp[0]; // Passa os atributos do ID (linha, coluna, nome)
        yyval.n_args = 1;
        yyval.valor_default.clear(); }
#line 1731 "y.tab.c"
    break;

  case 28: /* PARAM: ID '=' E  */
#line 253 "mini_js.y"
      { 
        yyval = yyvsp[-2];
        yyval.n_args = 1;
        yyval.valor_default = yyvsp[0].c; }
#line 1740 "y.tab.c"
    break;

  case 29: /* CMD_FOR: FOR '(' SF ';' E ';' EF ')' CMD  */
#line 260 "mini_js.y"
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          yyval.c = yyvsp[-6].c + define_label (teste_for) +
                 yyvsp[-4].c + "!" + fim_for + "?" + yyvsp[0].c + 
                 yyvsp[-2].c + teste_for + "#" + define_label(fim_for);
        }
#line 1752 "y.tab.c"
    break;

  case 30: /* EF: ATRIB  */
#line 270 "mini_js.y"
      { yyval.c = yyvsp[0].c + "^"; }
#line 1758 "y.tab.c"
    break;

  case 31: /* EF: %empty  */
#line 271 "mini_js.y"
      { yyval.clear(); }
#line 1764 "y.tab.c"
    break;

  case 36: /* CMD_WHILE: WHILE '(' E ')' CMD  */
#line 281 "mini_js.y"
        { string inicio_while = gera_label( "inicio_while" );
          string fim_while = gera_label( "fim_while" );
          yyval.c = define_label(inicio_while) + 
                 yyvsp[-2].c + "!" + fim_while + "?" + yyvsp[0].c + 
                 inicio_while + "#" + define_label(fim_while);
        }
#line 1775 "y.tab.c"
    break;

  case 37: /* CMD_LET: LET LET_VARs  */
#line 289 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1781 "y.tab.c"
    break;

  case 38: /* LET_VARs: LET_VAR ',' LET_VARs  */
#line 292 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1787 "y.tab.c"
    break;

  case 40: /* LET_VAR: LVALUE  */
#line 297 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[0] ).c; }
#line 1793 "y.tab.c"
    break;

  case 41: /* LET_VAR: LVALUE '=' E  */
#line 299 "mini_js.y"
          { 
            yyval.c = declara_var( Let, yyvsp[-2] ).c + // ex: a &
                   yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1801 "y.tab.c"
    break;

  case 42: /* LET_VAR: LVALUE '=' '{' '}'  */
#line 303 "mini_js.y"
          {
            yyval.c = declara_var( Let, yyvsp[-3] ).c +
            yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1809 "y.tab.c"
    break;

  case 43: /* CMD_VAR: VAR VAR_VARs  */
#line 308 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1815 "y.tab.c"
    break;

  case 44: /* VAR_VARs: VAR_VAR ',' VAR_VARs  */
#line 311 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1821 "y.tab.c"
    break;

  case 46: /* VAR_VAR: LVALUE  */
#line 316 "mini_js.y"
          { yyval.c = declara_var( Var, yyvsp[0] ).c; }
#line 1827 "y.tab.c"
    break;

  case 47: /* VAR_VAR: LVALUE '=' E  */
#line 318 "mini_js.y"
          {  yyval.c = declara_var( Var, yyvsp[-2] ).c + 
                    yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1834 "y.tab.c"
    break;

  case 48: /* VAR_VAR: LVALUE '=' '{' '}'  */
#line 321 "mini_js.y"
          {
            yyval.c = declara_var( Var, yyvsp[-3] ).c +
            yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1842 "y.tab.c"
    break;

  case 49: /* CMD_CONST: CONST CONST_VARs  */
#line 326 "mini_js.y"
                            { yyval.c = yyvsp[0].c; }
#line 1848 "y.tab.c"
    break;

  case 50: /* CONST_VARs: CONST_VAR ',' CONST_VARs  */
#line 329 "mini_js.y"
                                      { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1854 "y.tab.c"
    break;

  case 52: /* CONST_VAR: LVALUE  */
#line 334 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[0] ).c; }
#line 1860 "y.tab.c"
    break;

  case 53: /* CONST_VAR: LVALUE '=' E  */
#line 336 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[-2] ).c + 
                     yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1867 "y.tab.c"
    break;

  case 54: /* CONST_VAR: LVALUE '=' '{' '}'  */
#line 339 "mini_js.y"
            {
              yyval.c = declara_var( Const, yyvsp[-3] ).c +
              yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1875 "y.tab.c"
    break;

  case 55: /* CMD_IF: IF '(' E ')' CMD  */
#line 345 "mini_js.y"
          {
            string fim_if = gera_label( "fim_if" );
            yyval.c = yyvsp[-2].c + "!" + fim_if + "?" + yyvsp[0].c + define_label(fim_if);
          }
#line 1884 "y.tab.c"
    break;

  case 56: /* CMD_IF: IF '(' E ')' CMD ELSE CMD  */
#line 351 "mini_js.y"
          {  string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            yyval.c = yyvsp[-4].c + "!" + else_if + "?" + yyvsp[-2].c + fim_if + 
            "#" + define_label(else_if) + yyvsp[0].c +  define_label(fim_if);
          }
#line 1894 "y.tab.c"
    break;

  case 58: /* LVALUEPROP: LVALUEPROP '[' E ']'  */
#line 364 "mini_js.y"
            { yyval.c = yyvsp[-3].c + "[@]" + yyvsp[-1].c; }
#line 1900 "y.tab.c"
    break;

  case 59: /* LVALUEPROP: LVALUEPROP '.' ID  */
#line 366 "mini_js.y"
            { yyval.c = yyvsp[-2].c + "[@]"  + yyvsp[0].c[0] ; }
#line 1906 "y.tab.c"
    break;

  case 60: /* LVALUEPROP: F '[' E ']'  */
#line 368 "mini_js.y"
            { yyval.c = yyvsp[-3].c + yyvsp[-1].c; }
#line 1912 "y.tab.c"
    break;

  case 61: /* LVALUEPROP: F '.' ID  */
#line 370 "mini_js.y"
            { yyval.c = yyvsp[-2].c  + yyvsp[0].c[0] ; }
#line 1918 "y.tab.c"
    break;

  case 62: /* LVALUEPROP: LVALUE '[' E ']'  */
#line 373 "mini_js.y"
            { yyval.c = yyvsp[-3].c + "@" + yyvsp[-1].c; }
#line 1924 "y.tab.c"
    break;

  case 63: /* LVALUEPROP: LVALUE '.' ID  */
#line 376 "mini_js.y"
            { yyval.c = yyvsp[-2].c + "@"  + yyvsp[0].c[0] ; }
#line 1930 "y.tab.c"
    break;

  case 64: /* LIST: '[' LISTVALS ']'  */
#line 379 "mini_js.y"
                         { yyval.c = yyvsp[-2].c + yyvsp[-1].c + yyvsp[0].c; }
#line 1936 "y.tab.c"
    break;

  case 65: /* LIST: '[' ']'  */
#line 380 "mini_js.y"
                { yyval.c = vector<string>{"[]"}; }
#line 1942 "y.tab.c"
    break;

  case 66: /* LISTVALS: LISTVAL ',' LISTVALS  */
#line 383 "mini_js.y"
                                  { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1948 "y.tab.c"
    break;

  case 69: /* ATRIB: LVALUE '=' ATRIB  */
#line 392 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c[0] + yyvsp[0].c + "="; }
#line 1954 "y.tab.c"
    break;

  case 70: /* ATRIB: LVALUE MAIS_IGUAL ATRIB  */
#line 394 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c[0] + yyvsp[-2].c[0] + "@" + yyvsp[0].c + "+" + "="; }
#line 1960 "y.tab.c"
    break;

  case 71: /* ATRIB: LVALUE MENOS_IGUAL ATRIB  */
#line 396 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c[0] + yyvsp[-2].c[0] + "@" + yyvsp[0].c + "-" + "="; }
#line 1966 "y.tab.c"
    break;

  case 72: /* ATRIB: LVALUEPROP '=' ATRIB  */
#line 398 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + "[=]"; }
#line 1972 "y.tab.c"
    break;

  case 73: /* ATRIB: LVALUEPROP MAIS_IGUAL ATRIB  */
#line 400 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; }
#line 1978 "y.tab.c"
    break;

  case 74: /* ATRIB: LVALUE '=' '{' '}'  */
#line 403 "mini_js.y"
      { checa_simbolo( yyvsp[-3].c[0], true ); yyval.c = yyvsp[-3].c + "{}" + "="; }
#line 1984 "y.tab.c"
    break;

  case 75: /* ATRIB: LVALUEPROP '=' '{' '}'  */
#line 405 "mini_js.y"
      { yyval.c = yyvsp[-3].c + "{}" + "[=]"; }
#line 1990 "y.tab.c"
    break;

  case 77: /* E: LVALUEPROP  */
#line 410 "mini_js.y"
    { yyval.c = yyvsp[0].c + "[@]"; }
#line 1996 "y.tab.c"
    break;

  case 78: /* E: LVALUE  */
#line 413 "mini_js.y"
      { checa_simbolo( yyvsp[0].c[0], false ); yyval.c = yyvsp[0].c + "@"; }
#line 2002 "y.tab.c"
    break;

  case 79: /* E: E IGUAL E  */
#line 415 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2008 "y.tab.c"
    break;

  case 80: /* E: E '<' E  */
#line 417 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2014 "y.tab.c"
    break;

  case 81: /* E: E '>' E  */
#line 419 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2020 "y.tab.c"
    break;

  case 82: /* E: E '+' E  */
#line 421 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2026 "y.tab.c"
    break;

  case 83: /* E: E '-' E  */
#line 423 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2032 "y.tab.c"
    break;

  case 84: /* E: E '*' E  */
#line 425 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2038 "y.tab.c"
    break;

  case 85: /* E: E '/' E  */
#line 427 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2044 "y.tab.c"
    break;

  case 86: /* E: E '%' E  */
#line 429 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2050 "y.tab.c"
    break;

  case 87: /* E: '-' E  */
#line 431 "mini_js.y"
    { yyval.c = "0" + yyvsp[0].c + "-" ; }
#line 2056 "y.tab.c"
    break;

  case 88: /* E: '+' E  */
#line 433 "mini_js.y"
    { yyval.c = "0" + yyvsp[0].c + "+" ; }
#line 2062 "y.tab.c"
    break;

  case 89: /* E: UN  */
#line 435 "mini_js.y"
    { yyval.c = yyvsp[0].c; }
#line 2068 "y.tab.c"
    break;

  case 90: /* UN: MAIS_MAIS LVALUE  */
#line 440 "mini_js.y"
    {yyval.c = yyvsp[0].c + yyvsp[0].c[0] + "@" + "1" + "+" + "=";}
#line 2074 "y.tab.c"
    break;

  case 91: /* UN: MENOS_MENOS LVALUE  */
#line 443 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c[0] + "@" + "1" + "-" + "=";}
#line 2080 "y.tab.c"
    break;

  case 92: /* UN: LVALUE MAIS_MAIS  */
#line 446 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c[0] + yyvsp[-1].c[0] + "@" + "1" + "+" + "=" + "^";}
#line 2086 "y.tab.c"
    break;

  case 93: /* UN: LVALUE MENOS_MENOS  */
#line 449 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c[0] + yyvsp[-1].c[0] + "@" + "1" + "-" + "=" + "^";}
#line 2092 "y.tab.c"
    break;

  case 94: /* UN: MAIS_MAIS LVALUEPROP  */
#line 451 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "[@]" + "1" + "+" + "[=]";}
#line 2098 "y.tab.c"
    break;

  case 95: /* UN: MENOS_MENOS LVALUEPROP  */
#line 453 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "[@]" + "1" + "-" + "[=]";}
#line 2104 "y.tab.c"
    break;

  case 96: /* UN: LVALUEPROP MAIS_MAIS  */
#line 455 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "[@]" + yyvsp[-1].c + yyvsp[-1].c + "[@]" + "1" + "+" + "[=]" + "^";}
#line 2110 "y.tab.c"
    break;

  case 97: /* UN: LVALUEPROP MENOS_MENOS  */
#line 458 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "[@]" + yyvsp[-1].c + yyvsp[-1].c + "[@]" + "1" + "-" + "[=]" + "^";}
#line 2116 "y.tab.c"
    break;

  case 102: /* F: '(' E ')'  */
#line 466 "mini_js.y"
      { yyval.c = yyvsp[-1].c; }
#line 2122 "y.tab.c"
    break;

  case 104: /* F: '(' '{' '}' ')'  */
#line 469 "mini_js.y"
      { yyval.c = vector<string>{"{}"}; }
#line 2128 "y.tab.c"
    break;

  case 106: /* CHAMA_FUNC: ID '(' LISTA_ARGS ')'  */
#line 476 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + // 1. Empilha array de argumentos
                       "arguments" + "&" + "=" + "^" + // 2. Salva em 'arguments'
                       yyvsp[-3].c[0] + "@" + // 3. Empilha objeto da função (lendo da var)
                       "dup" + "this" + "&" + "=" + "^" + // 4. Salva em 'this'
                       "&funcao" + "[@]" + // 5. Pega o label do código
                       lbl_retorno + "[&]" + // 6. Empilha endereço de retorno
                       "~" + // 7. CHAMA
                       define_label( lbl_retorno ) + // 8. Ponto de retorno
                       "&retorno" + "@"; // 9. Empilha valor de retorno
              }
#line 2145 "y.tab.c"
    break;

  case 107: /* CHAMA_FUNC: LVALUEPROP '(' LISTA_ARGS ')'  */
#line 489 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + // 1. Empilha array de argumentos
                       "arguments" + "&" + "=" + "^" + // 2. Salva em 'arguments'
                       yyvsp[-3].c + "[@]" + // 3. Empilha objeto da função (LVALUEPROP + [@])
                       "dup" + "this" + "&" + "=" + "^" + // 4. Salva em 'this'
                      	"&funcao" + "[@]" + // 5. Pega o label do código
                      	lbl_retorno + "[&]" + // 6. Empilha endereço de retorno
                      	"~" + // 7. CHAMA
                      	define_label( lbl_retorno ) + // 8. Ponto de retorno
                      	"&retorno" + "@"; // 9. Empilha valor de retorno
              }
#line 2162 "y.tab.c"
    break;

  case 108: /* CHAMA_FUNC: '(' E ')' '(' LISTA_ARGS ')'  */
#line 502 "mini_js.y"
            { 
      	      string lbl_retorno = gera_label( "retorno_funcao" );
      	      yyval.c = yyvsp[-1].c + // 1. Empilha array de argumentos
      	       	 	 "arguments" + "&" + "=" + "^" + // 2. Salva em 'arguments'
      	       	 	 yyvsp[-4].c + // 3. Empilha objeto da função (resultado de E)
      	       	 	 "dup" + "this" + "&" + "=" + "^" + // 4. Salva em 'this'
      	       	 	 "&funcao" + "[@]" + // 5. Pega o label do código
      	       	 	 lbl_retorno + "[&]" + // 6. Empilha endereço de retorno
      	       	 	 "~" + // 7. CHAMA
      	       	 	 define_label( lbl_retorno ) + // 8. Ponto de retorno
  	       	 	 	 "&retorno" + "@"; // 9. Empilha valor de retorno
      	    }
#line 2179 "y.tab.c"
    break;

  case 110: /* LISTA_ARGS: %empty  */
#line 518 "mini_js.y"
             { yyval.c = vector<string>{"[]"}; }
#line 2185 "y.tab.c"
    break;

  case 111: /* ARGs: E  */
#line 522 "mini_js.y"
      { yyval.c = vector<string>{"[]"} + yyvsp[0].c + "append"; }
#line 2191 "y.tab.c"
    break;

  case 112: /* ARGs: ARGs ',' E  */
#line 524 "mini_js.y"
          { yyval.c = yyvsp[-2].c + yyvsp[0].c + "append"; }
#line 2197 "y.tab.c"
    break;


#line 2201 "y.tab.c"

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

#line 530 "mini_js.y"


#include "lex.yy.c"

Simbolo* busca_simbolo( string nome ) {
    // Procura do escopo mais interno (fim do vetor) para o mais externo (início)
    for( int i = ts.size() - 1; i >= 0; i-- ) {
        if( ts[i].count( nome ) > 0 ) {
            // Encontrou o símbolo
            return &ts[i][nome];
        }
    }
    // Não encontrou em nenhum escopo
    return nullptr;
}


Atributos declara_var( TipoDecl tipo, Atributos atrib ) {
  
  string nome = atrib.c[0];
  int linha = atrib.linha;
  int coluna = atrib.coluna;
       
  auto& topo = ts.back();    
       
  // Esta lógica foi corrigida para:
  // 1. Sempre retornar um 'Atributos'
  // 2. Tratar a semântica de 'var' (não gera código de alocação)
  //    diferente de 'let' e 'const' (geram código).

  if( topo.count( nome ) == 0 ) {
    topo[nome] = Simbolo{ tipo, linha, coluna };
    
    if (tipo == Let || tipo == Const) {
        // 'let' e 'const' geram código de alocação (ex: 'a &')
        atrib.c = { nome, "&" }; 
    } else {
        // 'var' é registrada, mas não gera código de alocação
        atrib.clear();
    }
    return atrib; // --- CORRIGIDO: Retorna o struct 'atrib'
  }
  else if( tipo == Var && topo[nome].tipo == Var ) {
    // Permite redeclaração de 'var' no mesmo escopo
    topo[nome] = Simbolo{ tipo, linha, coluna };
    
    atrib.clear(); // --- CORRIGIDO: Chama a função void
    return atrib;  // --- CORRIGIDO: Retorna o struct 'atrib'
  } 
  else {
    // Erro de redeclaração
    cerr << "Redeclaração de '" << nomeTipoDecl[topo[nome].tipo] << " " << nome 
         << "' na linha: " << topo[nome].linha 
         << ", coluna: " << topo[nome].coluna << endl;
    exit( 1 );     
  }
  // Adicionado para satisfazer o compilador (evitar warning)
  return atrib;
}

void checa_simbolo( string nome, bool modificavel ) {
  
  // --- CORREÇÃO CRÍTICA ADICIONADA ---
  // Variáveis especiais ('built-in') não estão na tabela de símbolos
  // e devem ser permitidas.
  if (nome == "this" || nome == "arguments" || nome == "&retorno" || nome == "&funcao") {
      return;
  }

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
