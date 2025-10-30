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

vector<int> pilha_funcoes;


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

#line 203 "y.tab.c"

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
  YYSYMBOL_CMD_RET = 50,                   /* CMD_RET  */
  YYSYMBOL_CMD_ASM = 51,                   /* CMD_ASM  */
  YYSYMBOL_EMPILHA_TS = 52,                /* EMPILHA_TS  */
  YYSYMBOL_CMD_FUNC = 53,                  /* CMD_FUNC  */
  YYSYMBOL_NOME_FUNCAO = 54,               /* NOME_FUNCAO  */
  YYSYMBOL_LISTA_PARAMs = 55,              /* LISTA_PARAMs  */
  YYSYMBOL_PARAMs = 56,                    /* PARAMs  */
  YYSYMBOL_PARAM = 57,                     /* PARAM  */
  YYSYMBOL_CMD_FOR = 58,                   /* CMD_FOR  */
  YYSYMBOL_EF = 59,                        /* EF  */
  YYSYMBOL_SF = 60,                        /* SF  */
  YYSYMBOL_CMD_WHILE = 61,                 /* CMD_WHILE  */
  YYSYMBOL_CMD_LET = 62,                   /* CMD_LET  */
  YYSYMBOL_LET_VARs = 63,                  /* LET_VARs  */
  YYSYMBOL_LET_VAR = 64,                   /* LET_VAR  */
  YYSYMBOL_CMD_VAR = 65,                   /* CMD_VAR  */
  YYSYMBOL_VAR_VARs = 66,                  /* VAR_VARs  */
  YYSYMBOL_VAR_VAR = 67,                   /* VAR_VAR  */
  YYSYMBOL_CMD_CONST = 68,                 /* CMD_CONST  */
  YYSYMBOL_CONST_VARs = 69,                /* CONST_VARs  */
  YYSYMBOL_CONST_VAR = 70,                 /* CONST_VAR  */
  YYSYMBOL_CMD_IF = 71,                    /* CMD_IF  */
  YYSYMBOL_LVALUE = 72,                    /* LVALUE  */
  YYSYMBOL_LVALUEPROP = 73,                /* LVALUEPROP  */
  YYSYMBOL_LIST = 74,                      /* LIST  */
  YYSYMBOL_LISTVALS = 75,                  /* LISTVALS  */
  YYSYMBOL_LISTVAL = 76,                   /* LISTVAL  */
  YYSYMBOL_ATRIB = 77,                     /* ATRIB  */
  YYSYMBOL_E = 78,                         /* E  */
  YYSYMBOL_UN = 79,                        /* UN  */
  YYSYMBOL_F = 80,                         /* F  */
  YYSYMBOL_CHAMA_FUNC = 81,                /* CHAMA_FUNC  */
  YYSYMBOL_LISTA_ARGS = 82,                /* LISTA_ARGS  */
  YYSYMBOL_ARGs = 83                       /* ARGs  */
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
#define YYLAST   563

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  115
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  206

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
       0,   152,   152,   155,   156,   160,   161,   162,   163,   164,
     167,   168,   169,   170,   171,   173,   174,   178,   184,   195,
     198,   201,   215,   221,   222,   223,   226,   238,   251,   255,
     262,   272,   274,   277,   278,   279,   280,   283,   292,   295,
     296,   299,   301,   305,   311,   314,   315,   318,   320,   323,
     329,   332,   333,   336,   338,   341,   347,   353,   361,   364,
     366,   368,   370,   372,   374,   378,   379,   382,   383,   386,
     390,   392,   394,   396,   398,   400,   402,   404,   407,   409,
     411,   413,   415,   417,   419,   421,   423,   425,   427,   429,
     431,   435,   437,   439,   441,   443,   445,   447,   449,   451,
     454,   455,   456,   457,   458,   460,   461,   463,   466,   474,
     483,   495,   500,   506,   512,   517
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
  "CMD", "BLOCO", "CMD_RET", "CMD_ASM", "EMPILHA_TS", "CMD_FUNC",
  "NOME_FUNCAO", "LISTA_PARAMs", "PARAMs", "PARAM", "CMD_FOR", "EF", "SF",
  "CMD_WHILE", "CMD_LET", "LET_VARs", "LET_VAR", "CMD_VAR", "VAR_VARs",
  "VAR_VAR", "CMD_CONST", "CONST_VARs", "CONST_VAR", "CMD_IF", "LVALUE",
  "LVALUEPROP", "LIST", "LISTVALS", "LISTVAL", "ATRIB", "E", "UN", "F",
  "CHAMA_FUNC", "LISTA_ARGS", "ARGs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-154)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -154,    13,   342,  -154,   -30,   -11,    33,    33,    33,    14,
      16,    52,   475,  -154,  -154,  -154,  -154,    29,    29,   475,
     475,    25,    91,  -154,  -154,  -154,  -154,    32,    47,  -154,
    -154,  -154,    53,    56,    58,  -154,   511,   518,  -154,    59,
     173,  -154,   -26,  -154,   475,   475,  -154,  -154,    21,    40,
    -154,    60,    76,  -154,    66,    83,   450,   475,  -154,    78,
      45,   300,   439,    -3,    22,   -26,    -3,    22,    67,    67,
    -154,    69,    74,   439,    79,   -12,  -154,  -154,  -154,  -154,
    -154,  -154,   475,  -154,   475,  -154,   130,   475,   116,   475,
    -154,  -154,   224,   475,   475,   118,  -154,  -154,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   121,   439,    88,
      82,   485,    33,   369,    33,   396,    33,   423,  -154,    93,
    -154,  -154,  -154,  -154,   439,   499,  -154,  -154,   475,    92,
      98,   264,  -154,  -154,    95,  -154,    44,  -154,  -154,    99,
    -154,   148,    97,  -154,   528,   331,   331,    67,    67,  -154,
    -154,  -154,   385,  -154,  -154,   475,   342,  -154,   100,   439,
    -154,   101,   439,  -154,   107,   439,   475,   342,   146,  -154,
    -154,   475,  -154,  -154,  -154,  -154,  -154,  -154,  -154,   439,
     151,  -154,  -154,  -154,   412,  -154,   132,   117,   115,  -154,
     122,   342,   475,   475,   128,   146,  -154,  -154,   127,   439,
    -154,  -154,   342,   303,  -154,  -154
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,    58,     0,     0,     0,     0,     0,
       0,     0,     0,   100,   102,   101,   103,     0,     0,     0,
       0,     0,     0,    16,    20,     3,    15,     0,     0,     9,
      10,    11,     0,     0,     0,     8,    79,    78,   105,     0,
      77,    90,    99,   107,   113,     0,    58,    38,    40,    41,
      50,    52,    53,    44,    46,    47,    32,     0,    22,     0,
      79,    78,    18,    91,    95,     0,    92,    96,    89,    88,
      66,     0,    68,    69,     0,     0,     4,    12,    13,     5,
       6,     7,     0,    93,     0,    94,     0,     0,     0,     0,
      97,    98,     0,     0,   113,     0,    14,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
     111,     0,     0,     0,     0,     0,     0,     0,    36,     0,
      33,    34,    35,    31,    77,     0,    20,    65,     0,     0,
     104,     0,    71,    72,     0,    70,     0,    64,    74,     0,
      73,     0,     0,    60,    80,    81,    82,    83,    84,    85,
      86,    87,     0,    62,   108,   112,     0,    39,     0,    42,
      51,     0,    54,    45,     0,    48,     0,     0,    25,    67,
     106,   113,    17,    75,    63,    76,    59,   109,    61,   115,
      56,    43,    55,    49,     0,    37,    28,     0,    23,    27,
       0,     0,    32,     0,     0,    24,   110,    57,     0,    29,
       4,    26,     0,     0,    30,    21
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -154,  -154,   -74,  -153,  -154,  -154,  -154,    46,  -154,  -154,
    -154,  -154,   -22,  -154,    -8,  -154,  -154,   129,    81,  -154,
     135,    96,  -154,   138,   102,  -154,  -154,    -2,     7,  -154,
      86,  -154,   -55,   131,  -154,    30,  -154,   -86,  -154
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    25,    26,    27,    28,    76,    29,    59,
     187,   188,   189,    30,   118,   119,    31,    32,    47,    48,
      33,    53,    54,    34,    50,    51,    35,    60,    61,    38,
      71,    72,    39,    40,    41,    42,    43,   109,   110
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   123,   131,   180,    49,    52,    55,    44,   142,    37,
     106,    98,   107,     3,   185,    63,    66,    99,   100,   101,
     102,   103,   104,   105,    64,    67,    45,   132,     4,   133,
     130,   135,     4,    87,   138,    88,    46,   140,   197,    13,
      14,    15,    16,    13,    14,    15,    16,    65,    65,   204,
      17,    56,    18,    57,    36,    58,    19,    20,    93,    94,
      95,    21,    22,    37,   112,    21,    22,    98,   113,    70,
      83,    77,    85,    99,   100,   101,   102,   103,   104,   105,
      36,    87,    36,    88,    36,   190,    78,    36,   174,    37,
      36,    37,    79,    37,     4,    80,    37,    81,    96,    37,
     103,   104,   105,   114,   115,    13,    14,    15,    16,   116,
      49,   117,    52,   127,    55,   126,    17,   128,    18,   137,
     129,   143,    19,    20,   153,   155,   203,    21,    22,    36,
     154,    74,   166,     4,   170,   171,   173,   123,    37,   177,
     175,   181,   182,    62,    13,    14,    15,    16,   183,   186,
      68,    69,    73,    75,    36,    17,   191,    18,   195,   194,
     193,    19,    20,    37,   196,    36,    21,    22,   200,   202,
     134,    98,   168,   201,    37,   108,   111,    99,   100,   101,
     102,   103,   104,   105,   198,   120,    97,   124,   125,    36,
      36,   121,   176,   157,   122,     0,    98,     0,    37,    37,
      36,    36,    99,   100,   101,   102,   103,   104,   105,    37,
      37,     0,   163,   124,   169,   124,   160,   124,   136,     0,
     124,     0,     0,   124,   141,   108,     0,     4,     0,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    13,    14,
      15,    16,     0,     0,   159,     0,   162,     0,   165,    17,
       0,    18,     0,     0,     0,    19,    20,     0,     0,    73,
      21,    22,     0,     0,   139,     0,     0,     4,     5,     0,
       6,     7,     8,     9,    10,    11,    12,     0,    13,    14,
      15,    16,     0,     0,     0,     0,   179,     0,     0,    17,
       0,    18,     0,     0,     0,    19,    20,   184,     0,     0,
      21,    22,   108,    23,    24,   172,     4,     5,     0,     6,
       7,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,     0,     0,   124,   199,    90,     0,    91,    17,     0,
      18,     0,     0,     0,    19,    20,    93,    94,    95,    21,
      22,     0,    23,    24,   205,     4,     5,     0,     6,     7,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      -1,    -1,   101,   102,   103,   104,   105,    17,     0,    18,
       0,     0,     4,    19,    20,     0,     0,     0,    21,    22,
       0,    23,    24,    13,    14,    15,    16,     0,     0,     0,
       0,     0,     0,     0,    17,     0,    18,     0,     0,     4,
      19,    20,     0,     0,     0,    21,    22,     0,    98,   158,
      13,    14,    15,    16,    99,   100,   101,   102,   103,   104,
     105,    17,     0,    18,     0,     0,     4,    19,    20,   178,
       0,     0,    21,    22,     0,    98,   161,    13,    14,    15,
      16,    99,   100,   101,   102,   103,   104,   105,    17,     0,
      18,   192,     0,     4,    19,    20,     6,     7,     8,    21,
      22,     0,    98,   164,    13,    14,    15,    16,    99,   100,
     101,   102,   103,   104,   105,    17,     0,    18,     4,     0,
       0,    19,    20,     0,     0,     0,    21,    22,     0,    13,
      14,    15,    16,     0,     0,     0,     0,     0,     0,     0,
      17,     0,    18,     0,     0,     0,    19,    20,    98,     0,
       0,    21,    22,     0,    99,   100,   101,   102,   103,   104,
     105,     0,    98,     0,     0,     0,     0,   156,    99,   100,
     101,   102,   103,   104,   105,    82,    83,    84,    85,    86,
       0,   167,    89,    90,     0,    91,    92,    87,     0,    88,
       0,    -1,     0,     0,    93,    94,    95,    99,   100,   101,
     102,   103,   104,   105
};

static const yytype_int16 yycheck[] =
{
       2,    56,    76,   156,     6,     7,     8,    37,    94,     2,
      36,    23,    38,     0,   167,    17,    18,    29,    30,    31,
      32,    33,    34,    35,    17,    18,    37,    82,     3,    84,
      42,    86,     3,    36,    89,    38,     3,    92,   191,    14,
      15,    16,    17,    14,    15,    16,    17,    17,    18,   202,
      25,    37,    27,    37,    56,     3,    31,    32,    36,    37,
      38,    36,    37,    56,    43,    36,    37,    23,    28,    44,
      25,    39,    27,    29,    30,    31,    32,    33,    34,    35,
      82,    36,    84,    38,    86,   171,    39,    89,    44,    82,
      92,    84,    39,    86,     3,    39,    89,    39,    39,    92,
      33,    34,    35,    43,    28,    14,    15,    16,    17,    43,
     112,    28,   114,    44,   116,    37,    25,    43,    27,     3,
      41,     3,    31,    32,     3,    43,   200,    36,    37,   131,
      42,    40,    39,     3,    42,    37,    41,   192,   131,    42,
      41,    41,    41,    12,    14,    15,    16,    17,    41,     3,
      19,    20,    21,    22,   156,    25,     5,    27,    43,    42,
      28,    31,    32,   156,    42,   167,    36,    37,    40,    42,
      40,    23,   126,   195,   167,    44,    45,    29,    30,    31,
      32,    33,    34,    35,   192,    56,    13,    56,    57,   191,
     192,    56,    44,   112,    56,    -1,    23,    -1,   191,   192,
     202,   203,    29,    30,    31,    32,    33,    34,    35,   202,
     203,    -1,   116,    82,   128,    84,   114,    86,    87,    -1,
      89,    -1,    -1,    92,    93,    94,    -1,     3,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,   106,    14,    15,
      16,    17,    -1,    -1,   113,    -1,   115,    -1,   117,    25,
      -1,    27,    -1,    -1,    -1,    31,    32,    -1,    -1,   128,
      36,    37,    -1,    -1,    40,    -1,    -1,     3,     4,    -1,
       6,     7,     8,     9,    10,    11,    12,    -1,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,   155,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    32,   166,    -1,    -1,
      36,    37,   171,    39,    40,    41,     3,     4,    -1,     6,
       7,     8,     9,    10,    11,    12,    -1,    14,    15,    16,
      17,    -1,    -1,   192,   193,    25,    -1,    27,    25,    -1,
      27,    -1,    -1,    -1,    31,    32,    36,    37,    38,    36,
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
      27,    39,    -1,     3,    31,    32,     6,     7,     8,    36,
      37,    -1,    23,    40,    14,    15,    16,    17,    29,    30,
      31,    32,    33,    34,    35,    25,    -1,    27,     3,    -1,
      -1,    31,    32,    -1,    -1,    -1,    36,    37,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    32,    23,    -1,
      -1,    36,    37,    -1,    29,    30,    31,    32,    33,    34,
      35,    -1,    23,    -1,    -1,    -1,    -1,    42,    29,    30,
      31,    32,    33,    34,    35,    24,    25,    26,    27,    28,
      -1,    42,    24,    25,    -1,    27,    28,    36,    -1,    38,
      -1,    23,    -1,    -1,    36,    37,    38,    29,    30,    31,
      32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,    47,     0,     3,     4,     6,     7,     8,     9,
      10,    11,    12,    14,    15,    16,    17,    25,    27,    31,
      32,    36,    37,    39,    40,    48,    49,    50,    51,    53,
      58,    61,    62,    65,    68,    71,    72,    73,    74,    77,
      78,    79,    80,    81,    37,    37,     3,    63,    64,    72,
      69,    70,    72,    66,    67,    72,    37,    37,     3,    54,
      72,    73,    78,    72,    73,    80,    72,    73,    78,    78,
      44,    75,    76,    78,    40,    78,    52,    39,    39,    39,
      39,    39,    24,    25,    26,    27,    28,    36,    38,    24,
      25,    27,    28,    36,    37,    38,    39,    13,    23,    29,
      30,    31,    32,    33,    34,    35,    36,    38,    78,    82,
      83,    78,    43,    28,    43,    28,    43,    28,    59,    60,
      62,    65,    68,    77,    78,    78,    37,    44,    43,    41,
      42,    47,    77,    77,    40,    77,    78,     3,    77,    40,
      77,    78,    82,     3,    78,    78,    78,    78,    78,    78,
      78,    78,    78,     3,    42,    43,    42,    63,    40,    78,
      69,    40,    78,    66,    40,    78,    39,    42,    52,    75,
      42,    37,    41,    41,    44,    41,    44,    42,    44,    78,
      48,    41,    41,    41,    78,    48,     3,    55,    56,    57,
      82,     5,    39,    28,    42,    43,    42,    48,    59,    78,
      40,    57,    42,    47,    48,    41
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    49,    50,    51,
      52,    53,    54,    55,    55,    55,    56,    56,    57,    57,
      58,    59,    59,    60,    60,    60,    60,    61,    62,    63,
      63,    64,    64,    64,    65,    66,    66,    67,    67,    67,
      68,    69,    69,    70,    70,    70,    71,    71,    72,    73,
      73,    73,    73,    73,    73,    74,    74,    75,    75,    76,
      77,    77,    77,    77,    77,    77,    77,    77,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      80,    80,    80,    80,    80,    80,    80,    80,    81,    81,
      81,    82,    82,    82,    83,    83
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     2,     2,     1,     1,
       1,     1,     2,     2,     2,     1,     1,     4,     2,     2,
       0,     9,     1,     1,     2,     0,     3,     1,     1,     3,
       9,     1,     0,     1,     1,     1,     1,     5,     2,     3,
       1,     1,     3,     4,     2,     3,     1,     1,     3,     4,
       2,     3,     1,     1,     3,     4,     5,     7,     1,     4,
       3,     4,     3,     4,     3,     3,     2,     3,     1,     1,
       3,     3,     3,     3,     3,     4,     4,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     3,     1,     4,     1,     4,     4,
       6,     1,     2,     0,     1,     3
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
#line 152 "mini_js.y"
         { print( resolve_enderecos( yyvsp[0].c + "." + funcoes ) ); }
#line 1574 "y.tab.c"
    break;

  case 3: /* CMDs: CMDs CMD  */
#line 155 "mini_js.y"
                 { yyval.c = yyvsp[-1].c + yyvsp[0].c; }
#line 1580 "y.tab.c"
    break;

  case 4: /* CMDs: %empty  */
#line 156 "mini_js.y"
                 { yyval.clear(); }
#line 1586 "y.tab.c"
    break;

  case 14: /* CMD: ATRIB ';'  */
#line 172 "mini_js.y"
      { yyval.c = yyvsp[-1].c + "^"; }
#line 1592 "y.tab.c"
    break;

  case 16: /* CMD: ';'  */
#line 175 "mini_js.y"
      { yyval.clear(); }
#line 1598 "y.tab.c"
    break;

  case 17: /* BLOCO: '{' EMPILHA_TS CMDs '}'  */
#line 179 "mini_js.y"
        { yyval.c = "<{" + yyvsp[-1].c + "}>" ; ts.pop_back(); 
          //pilha_funcoes.back()++;
        }
#line 1606 "y.tab.c"
    break;

  case 18: /* CMD_RET: RETURN E  */
#line 185 "mini_js.y"
          { 
            /*int quant_ret = pilha_funcoes.back();
            pilha_funcoes.pop_back();
            vector<string> retornos;
            for( int i = 0; i < quant_ret; i++ )
                retornos.push_back( "}>" ); erro de seg nao é aqui*/
            yyval.c = yyvsp[0].c +  vector<string>{ "'&retorno'", "@", "~" }; 
            
            }
#line 1620 "y.tab.c"
    break;

  case 19: /* CMD_ASM: E ASM  */
#line 195 "mini_js.y"
                { yyval.c = yyvsp[-1].c + yyvsp[0].c + "^"; }
#line 1626 "y.tab.c"
    break;

  case 20: /* EMPILHA_TS: %empty  */
#line 198 "mini_js.y"
             { ts.push_back( map< string, Simbolo >{} ); }
#line 1632 "y.tab.c"
    break;

  case 21: /* CMD_FUNC: FUNCTION NOME_FUNCAO '(' EMPILHA_TS LISTA_PARAMs ')' '{' CMDs '}'  */
#line 202 "mini_js.y"
           { 
             //pilha_funcoes.push_back(1);
             string definicao_lbl_endereco_funcao = ":" + yyvsp[-7].endereco_funcao;
             
             yyval.c = yyvsp[-7].c ;

             funcoes = funcoes + definicao_lbl_endereco_funcao + yyvsp[-4].c + yyvsp[-1].c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
             
           }
#line 1648 "y.tab.c"
    break;

  case 22: /* NOME_FUNCAO: ID  */
#line 216 "mini_js.y"
            { string endereco_funcao = gera_label( "func_" + yyvsp[0].c[0] );
              Atributos decl = declara_var( Var, yyvsp[0] ); // Declara a função no escopo atual
              yyval.c = yyvsp[0].c + "&" + yyvsp[0].c + "{}" + "=" + "'&funcao'" + yyval.endereco_funcao + "[=]" + "^"; }
#line 1656 "y.tab.c"
    break;

  case 25: /* LISTA_PARAMs: %empty  */
#line 223 "mini_js.y"
             { yyval.clear(); }
#line 1662 "y.tab.c"
    break;

  case 26: /* PARAMs: PARAMs ',' PARAM  */
#line 227 "mini_js.y"
       { 
         Atributos decl = declara_var( Let, yyvsp[0] ); // Declara o parâmetro no escopo da função
         yyval.c = yyvsp[-2].c + decl.c + 
                yyvsp[0].c[0] + "arguments" + "@" + to_string( yyvsp[-2].n_args ) 
                + "[@]" + "=" + "^"; 
                
         if( yyvsp[0].valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         yyval.n_args = yyvsp[-2].n_args + yyvsp[0].n_args; 
       }
#line 1678 "y.tab.c"
    break;

  case 27: /* PARAMs: PARAM  */
#line 239 "mini_js.y"
       { 
         Atributos decl = declara_var( Let, yyvsp[0] ); // Declara o parâmetro no escopo da função
         yyval.c = decl.c + 
                yyvsp[0].c[0] + "arguments" + "@" + "0" + "[@]" + "=" + "^"; 
                
         if( yyvsp[0].valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         yyval.n_args = yyvsp[0].n_args; 
       }
#line 1693 "y.tab.c"
    break;

  case 28: /* PARAM: ID  */
#line 252 "mini_js.y"
      { yyval = yyvsp[0]; 
        yyval.n_args = 1;
        yyval.valor_default.clear(); }
#line 1701 "y.tab.c"
    break;

  case 29: /* PARAM: ID '=' E  */
#line 256 "mini_js.y"
      { 
        yyval = yyvsp[-2];
        yyval.n_args = 1;
        yyval.valor_default = yyvsp[0].c; }
#line 1710 "y.tab.c"
    break;

  case 30: /* CMD_FOR: FOR '(' SF ';' E ';' EF ')' CMD  */
#line 263 "mini_js.y"
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          yyval.c = yyvsp[-6].c + define_label (teste_for) +
                 yyvsp[-4].c + "!" + fim_for + "?" + yyvsp[0].c + 
                 yyvsp[-2].c + teste_for + "#" + define_label(fim_for);
        }
#line 1722 "y.tab.c"
    break;

  case 31: /* EF: ATRIB  */
#line 273 "mini_js.y"
      { yyval.c = yyvsp[0].c + "^"; }
#line 1728 "y.tab.c"
    break;

  case 32: /* EF: %empty  */
#line 274 "mini_js.y"
      { yyval.clear(); }
#line 1734 "y.tab.c"
    break;

  case 37: /* CMD_WHILE: WHILE '(' E ')' CMD  */
#line 284 "mini_js.y"
        { string inicio_while = gera_label( "inicio_while" );
          string fim_while = gera_label( "fim_while" );
          yyval.c = define_label(inicio_while) + 
                 yyvsp[-2].c + "!" + fim_while + "?" + yyvsp[0].c + 
                 inicio_while + "#" + define_label(fim_while);
        }
#line 1745 "y.tab.c"
    break;

  case 38: /* CMD_LET: LET LET_VARs  */
#line 292 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1751 "y.tab.c"
    break;

  case 39: /* LET_VARs: LET_VAR ',' LET_VARs  */
#line 295 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1757 "y.tab.c"
    break;

  case 41: /* LET_VAR: LVALUE  */
#line 300 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[0] ).c; }
#line 1763 "y.tab.c"
    break;

  case 42: /* LET_VAR: LVALUE '=' E  */
#line 302 "mini_js.y"
          { 
            yyval.c = declara_var( Let, yyvsp[-2] ).c + // ex: a &
                   yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1771 "y.tab.c"
    break;

  case 43: /* LET_VAR: LVALUE '=' '{' '}'  */
#line 306 "mini_js.y"
          {
            yyval.c = declara_var( Let, yyvsp[-3] ).c +
            yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1779 "y.tab.c"
    break;

  case 44: /* CMD_VAR: VAR VAR_VARs  */
#line 311 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1785 "y.tab.c"
    break;

  case 45: /* VAR_VARs: VAR_VAR ',' VAR_VARs  */
#line 314 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1791 "y.tab.c"
    break;

  case 47: /* VAR_VAR: LVALUE  */
#line 319 "mini_js.y"
          { yyval.c = declara_var( Var, yyvsp[0] ).c; }
#line 1797 "y.tab.c"
    break;

  case 48: /* VAR_VAR: LVALUE '=' E  */
#line 321 "mini_js.y"
          {  yyval.c = declara_var( Var, yyvsp[-2] ).c + 
                    yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1804 "y.tab.c"
    break;

  case 49: /* VAR_VAR: LVALUE '=' '{' '}'  */
#line 324 "mini_js.y"
          {
            yyval.c = declara_var( Var, yyvsp[-3] ).c +
            yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1812 "y.tab.c"
    break;

  case 50: /* CMD_CONST: CONST CONST_VARs  */
#line 329 "mini_js.y"
                            { yyval.c = yyvsp[0].c; }
#line 1818 "y.tab.c"
    break;

  case 51: /* CONST_VARs: CONST_VAR ',' CONST_VARs  */
#line 332 "mini_js.y"
                                      { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1824 "y.tab.c"
    break;

  case 53: /* CONST_VAR: LVALUE  */
#line 337 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[0] ).c; }
#line 1830 "y.tab.c"
    break;

  case 54: /* CONST_VAR: LVALUE '=' E  */
#line 339 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[-2] ).c + 
                     yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1837 "y.tab.c"
    break;

  case 55: /* CONST_VAR: LVALUE '=' '{' '}'  */
#line 342 "mini_js.y"
            {
              yyval.c = declara_var( Const, yyvsp[-3] ).c +
              yyvsp[-3].c[0] + "{}" + "=" + "^"; }
#line 1845 "y.tab.c"
    break;

  case 56: /* CMD_IF: IF '(' E ')' CMD  */
#line 348 "mini_js.y"
          {
            string fim_if = gera_label( "fim_if" );
            yyval.c = yyvsp[-2].c + "!" + fim_if + "?" + yyvsp[0].c + define_label(fim_if);
          }
#line 1854 "y.tab.c"
    break;

  case 57: /* CMD_IF: IF '(' E ')' CMD ELSE CMD  */
#line 354 "mini_js.y"
          {  string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            yyval.c = yyvsp[-4].c + "!" + else_if + "?" + yyvsp[-2].c + fim_if + 
            "#" + define_label(else_if) + yyvsp[0].c +  define_label(fim_if);
          }
#line 1864 "y.tab.c"
    break;

  case 59: /* LVALUEPROP: LVALUEPROP '[' E ']'  */
#line 365 "mini_js.y"
            { yyval.c = yyvsp[-3].c + "[@]" + yyvsp[-1].c; }
#line 1870 "y.tab.c"
    break;

  case 60: /* LVALUEPROP: LVALUEPROP '.' ID  */
#line 367 "mini_js.y"
            { yyval.c = yyvsp[-2].c + "[@]"  + yyvsp[0].c[0] ; }
#line 1876 "y.tab.c"
    break;

  case 61: /* LVALUEPROP: F '[' E ']'  */
#line 369 "mini_js.y"
            { yyval.c = yyvsp[-3].c + yyvsp[-1].c; }
#line 1882 "y.tab.c"
    break;

  case 62: /* LVALUEPROP: F '.' ID  */
#line 371 "mini_js.y"
            { yyval.c = yyvsp[-2].c  + yyvsp[0].c[0] ; }
#line 1888 "y.tab.c"
    break;

  case 63: /* LVALUEPROP: LVALUE '[' E ']'  */
#line 373 "mini_js.y"
            { yyval.c = yyvsp[-3].c + "@" + yyvsp[-1].c; }
#line 1894 "y.tab.c"
    break;

  case 64: /* LVALUEPROP: LVALUE '.' ID  */
#line 375 "mini_js.y"
            { yyval.c = yyvsp[-2].c + "@"  + yyvsp[0].c[0] ; }
#line 1900 "y.tab.c"
    break;

  case 65: /* LIST: '[' LISTVALS ']'  */
#line 378 "mini_js.y"
                         { yyval.c = yyvsp[-2].c + yyvsp[-1].c + yyvsp[0].c; }
#line 1906 "y.tab.c"
    break;

  case 66: /* LIST: '[' ']'  */
#line 379 "mini_js.y"
                { yyval.c = vector<string>{"[]"}; }
#line 1912 "y.tab.c"
    break;

  case 67: /* LISTVALS: LISTVAL ',' LISTVALS  */
#line 382 "mini_js.y"
                                  { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1918 "y.tab.c"
    break;

  case 70: /* ATRIB: LVALUE '=' ATRIB  */
#line 391 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[0].c + "="; }
#line 1924 "y.tab.c"
    break;

  case 71: /* ATRIB: LVALUE MAIS_IGUAL ATRIB  */
#line 393 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "+" + "="; }
#line 1930 "y.tab.c"
    break;

  case 72: /* ATRIB: LVALUE MENOS_IGUAL ATRIB  */
#line 395 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "-" + "="; }
#line 1936 "y.tab.c"
    break;

  case 73: /* ATRIB: LVALUEPROP '=' ATRIB  */
#line 397 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + "[=]"; }
#line 1942 "y.tab.c"
    break;

  case 74: /* ATRIB: LVALUEPROP MAIS_IGUAL ATRIB  */
#line 399 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; }
#line 1948 "y.tab.c"
    break;

  case 75: /* ATRIB: LVALUE '=' '{' '}'  */
#line 401 "mini_js.y"
      { checa_simbolo( yyvsp[-3].c[0], true ); yyval.c = yyvsp[-3].c + "{}" + "="; }
#line 1954 "y.tab.c"
    break;

  case 76: /* ATRIB: LVALUEPROP '=' '{' '}'  */
#line 403 "mini_js.y"
      { yyval.c = yyvsp[-3].c + "{}" + "[=]"; }
#line 1960 "y.tab.c"
    break;

  case 78: /* E: LVALUEPROP  */
#line 408 "mini_js.y"
    { yyval.c = yyvsp[0].c + "[@]"; }
#line 1966 "y.tab.c"
    break;

  case 79: /* E: LVALUE  */
#line 410 "mini_js.y"
      { checa_simbolo( yyvsp[0].c[0], false ); yyval.c = yyvsp[0].c + "@"; }
#line 1972 "y.tab.c"
    break;

  case 80: /* E: E IGUAL E  */
#line 412 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 1978 "y.tab.c"
    break;

  case 81: /* E: E '<' E  */
#line 414 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 1984 "y.tab.c"
    break;

  case 82: /* E: E '>' E  */
#line 416 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 1990 "y.tab.c"
    break;

  case 83: /* E: E '+' E  */
#line 418 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 1996 "y.tab.c"
    break;

  case 84: /* E: E '-' E  */
#line 420 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2002 "y.tab.c"
    break;

  case 85: /* E: E '*' E  */
#line 422 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2008 "y.tab.c"
    break;

  case 86: /* E: E '/' E  */
#line 424 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2014 "y.tab.c"
    break;

  case 87: /* E: E '%' E  */
#line 426 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2020 "y.tab.c"
    break;

  case 88: /* E: '-' E  */
#line 428 "mini_js.y"
    { yyval.c = "0" + yyvsp[0].c + "-" ; }
#line 2026 "y.tab.c"
    break;

  case 89: /* E: '+' E  */
#line 430 "mini_js.y"
    { yyval.c = "0" + yyvsp[0].c + "+" ; }
#line 2032 "y.tab.c"
    break;

  case 90: /* E: UN  */
#line 432 "mini_js.y"
    { yyval.c = yyvsp[0].c; }
#line 2038 "y.tab.c"
    break;

  case 91: /* UN: MAIS_MAIS LVALUE  */
#line 436 "mini_js.y"
    {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "+" + "=";}
#line 2044 "y.tab.c"
    break;

  case 92: /* UN: MENOS_MENOS LVALUE  */
#line 438 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "-" + "=";}
#line 2050 "y.tab.c"
    break;

  case 93: /* UN: LVALUE MAIS_MAIS  */
#line 440 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "+" + "=" + "^";}
#line 2056 "y.tab.c"
    break;

  case 94: /* UN: LVALUE MENOS_MENOS  */
#line 442 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "-" + "=" + "^";}
#line 2062 "y.tab.c"
    break;

  case 95: /* UN: MAIS_MAIS LVALUEPROP  */
#line 444 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "[@]" + "1" + "+" + "[=]";}
#line 2068 "y.tab.c"
    break;

  case 96: /* UN: MENOS_MENOS LVALUEPROP  */
#line 446 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "[@]" + "1" + "-" + "[=]";}
#line 2074 "y.tab.c"
    break;

  case 97: /* UN: LVALUEPROP MAIS_MAIS  */
#line 448 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "[@]" + yyvsp[-1].c + yyvsp[-1].c + "[@]" + "1" + "+" + "[=]" + "^";}
#line 2080 "y.tab.c"
    break;

  case 98: /* UN: LVALUEPROP MENOS_MENOS  */
#line 450 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "[@]" + yyvsp[-1].c + yyvsp[-1].c + "[@]" + "1" + "-" + "[=]" + "^";}
#line 2086 "y.tab.c"
    break;

  case 104: /* F: '(' E ')'  */
#line 459 "mini_js.y"
      { yyval.c = yyvsp[-1].c; }
#line 2092 "y.tab.c"
    break;

  case 106: /* F: '(' '{' '}' ')'  */
#line 462 "mini_js.y"
      { yyval.c = vector<string>{"{}"}; }
#line 2098 "y.tab.c"
    break;

  case 108: /* CHAMA_FUNC: ID '(' LISTA_ARGS ')'  */
#line 467 "mini_js.y"
              { 
                yyval.c = yyvsp[-1].c + // 1. Empilha args e contagem
                       yyvsp[-3].c[0] + "@" + // 2. Empilha objeto da função (lendo da var)
                  
                       "$"  // 6. CHAMA (a instrução '$')
                       ;
              }
#line 2110 "y.tab.c"
    break;

  case 109: /* CHAMA_FUNC: LVALUEPROP '(' LISTA_ARGS ')'  */
#line 475 "mini_js.y"
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                yyval.c = yyvsp[-1].c + // 1. Empilha args e contagem
                       yyvsp[-3].c + "[@]" + // 2. Empilha objeto da função (LVALUEPROP + [@])
                        "$"  // 6. CHAMA (a instrução '$')
                        ;
              }
#line 2122 "y.tab.c"
    break;

  case 110: /* CHAMA_FUNC: '(' E ')' '(' LISTA_ARGS ')'  */
#line 484 "mini_js.y"
            { 
              string lbl_retorno = gera_label( "retorno_funcao" );
              yyval.c = yyvsp[-1].c + 
                   yyvsp[-4].c + 
                    //"@" +
                   "$"  // 6. CHAMA (a instrução '$') + "^";
                    ;

           }
#line 2136 "y.tab.c"
    break;

  case 111: /* LISTA_ARGS: ARGs  */
#line 496 "mini_js.y"
           { 
             yyval.c = yyvsp[0].c + to_string( yyvsp[0].n_args ); 
             yyval.n_args = yyvsp[0].n_args;
           }
#line 2145 "y.tab.c"
    break;

  case 112: /* LISTA_ARGS: ARGs ','  */
#line 501 "mini_js.y"
           {
             yyval.c = yyvsp[-1].c + to_string( yyvsp[-1].n_args ); 
             yyval.n_args = yyvsp[-1].n_args;
           }
#line 2154 "y.tab.c"
    break;

  case 113: /* LISTA_ARGS: %empty  */
#line 506 "mini_js.y"
           { 
              yyval.c = vector<string>{"0"}; 
              yyval.n_args = 0; 
           }
#line 2163 "y.tab.c"
    break;

  case 114: /* ARGs: E  */
#line 513 "mini_js.y"
      { 
        yyval.c = yyvsp[0].c;     
        yyval.n_args = 1;   
      }
#line 2172 "y.tab.c"
    break;

  case 115: /* ARGs: ARGs ',' E  */
#line 518 "mini_js.y"
      { 
        yyval.c = yyvsp[-2].c + yyvsp[0].c; 
        yyval.n_args = yyvsp[-2].n_args + 1; 
      }
#line 2181 "y.tab.c"
    break;


#line 2185 "y.tab.c"

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

#line 528 "mini_js.y"


#include "lex.yy.c"

Simbolo* busca_simbolo( string nome ) {
    for( int i = ts.size() - 1; i >= 0; i-- ) {
        if( ts[i].count( nome ) > 0 ) {
            return &ts[i][nome];
        }
    }
    return nullptr;
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



