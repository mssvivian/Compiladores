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
  
  string nome_var; // Para identificar ID em listas mistas
  vector<Atributos> lista_exprs; // Para lista unificada de expressões/parâmetros
  bool eh_atribuicao = false; // Identifica se é uma atribuição (para checagem tardia)

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
    nome_var.clear();
    lista_exprs.clear();
    eh_atribuicao = false;
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
    // Se é parâmetro ou variável local da própria arrow function, ignora
    if(parametros.find(id) != parametros.end() || locais.find(id) != locais.end()) {
        continue;
    }

    // Verifica se é global e não sombreada
    bool is_global = (ts[0].count(id) > 0);
    bool is_shadowed = false;
    
    // Escopos intermediários: do 1 até o penúltimo (o último é o da arrow function)
    // ts.size() deve ser pelo menos 2 (Global + Arrow)
    if (ts.size() > 2) {
        for(size_t k = 1; k < ts.size() - 1; ++k) {
            if(ts[k].count(id) > 0) {
                is_shadowed = true;
                break;
            }
        }
    }

    if (is_global && !is_shadowed) {
        continue; // Não captura global pura
    }

    capturas.insert(id);
  }
  return capturas;
}

// Extrai nomes de variáveis de um vetor de código
set<string> extrai_ids_de_codigo(vector<string>& codigo) {
  set<string> ids;
  for(const string& s : codigo) {
    // Simples heurística: se começa com letra ou _, é um possível ID
    if(!s.empty() && (isalpha(s[0]) || s[0] == '_') && s[0] != '\'' && !isdigit(s[0])) {
      if(s != "@" && s != "=" && s != "@" && s != "^" && s != "$" && s != "[=]" && s != "[<=]" && 
         s != "[@]" && s != "&" && s != "~" && s != "#" && s != "?" && s != "!" && 
         s != "+" && s != "-" && s != "*" && s != "/" && s != "%" && s != "==" && s != "!=" &&
         s != "<" && s != ">" && s != "<=" && s != ">=" && s != "<{" && s != "}>" && s != "{}" &&
         s != "println" && s != "to_string" && s != "undefined" && s.substr(0, 5) != "temp_" && s.substr(0, 5) != "func_") {
        ids.insert(s);
      }
    }
  }
  // cerr << "IDs extraidos: "; for(auto s : ids) cerr << s << " "; cerr << endl;
  return ids;
}

void empilha_escopo_novo();
void desempilha_escopo();

#line 279 "y.tab.c"

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
  YYSYMBOL_EMPILHA_TS_FUNC = 58,           /* EMPILHA_TS_FUNC  */
  YYSYMBOL_DESEMPILHA_TS_FUNC = 59,        /* DESEMPILHA_TS_FUNC  */
  YYSYMBOL_CMD_FUNC = 60,                  /* CMD_FUNC  */
  YYSYMBOL_61_1 = 61,                      /* @1  */
  YYSYMBOL_NOME_FUNCAO = 62,               /* NOME_FUNCAO  */
  YYSYMBOL_CMD_FOR = 63,                   /* CMD_FOR  */
  YYSYMBOL_ECOND_OPT = 64,                 /* ECOND_OPT  */
  YYSYMBOL_EF = 65,                        /* EF  */
  YYSYMBOL_SF = 66,                        /* SF  */
  YYSYMBOL_CMD_WHILE = 67,                 /* CMD_WHILE  */
  YYSYMBOL_CMD_LET = 68,                   /* CMD_LET  */
  YYSYMBOL_LET_VARs = 69,                  /* LET_VARs  */
  YYSYMBOL_LET_VAR = 70,                   /* LET_VAR  */
  YYSYMBOL_CMD_VAR = 71,                   /* CMD_VAR  */
  YYSYMBOL_VAR_VARs = 72,                  /* VAR_VARs  */
  YYSYMBOL_VAR_VAR = 73,                   /* VAR_VAR  */
  YYSYMBOL_CMD_CONST = 74,                 /* CMD_CONST  */
  YYSYMBOL_CONST_VARs = 75,                /* CONST_VARs  */
  YYSYMBOL_CONST_VAR = 76,                 /* CONST_VAR  */
  YYSYMBOL_CMD_IF = 77,                    /* CMD_IF  */
  YYSYMBOL_LVALUEPROP = 78,                /* LVALUEPROP  */
  YYSYMBOL_LIST = 79,                      /* LIST  */
  YYSYMBOL_LISTVALS = 80,                  /* LISTVALS  */
  YYSYMBOL_LISTVAL = 81,                   /* LISTVAL  */
  YYSYMBOL_ATRIB = 82,                     /* ATRIB  */
  YYSYMBOL_ATRIB_NO_OBJ = 83,              /* ATRIB_NO_OBJ  */
  YYSYMBOL_84_2 = 84,                      /* @2  */
  YYSYMBOL_85_3 = 85,                      /* @3  */
  YYSYMBOL_86_4 = 86,                      /* @4  */
  YYSYMBOL_87_5 = 87,                      /* @5  */
  YYSYMBOL_EOBJ = 88,                      /* EOBJ  */
  YYSYMBOL_OBJ_LITERAL = 89,               /* OBJ_LITERAL  */
  YYSYMBOL_PARAM = 90,                     /* PARAM  */
  YYSYMBOL_LISTA_PARAMS = 91,              /* LISTA_PARAMS  */
  YYSYMBOL_LISTA_PARAMS_OPT = 92,          /* LISTA_PARAMS_OPT  */
  YYSYMBOL_LISTA_MISTA = 93,               /* LISTA_MISTA  */
  YYSYMBOL_ECOND = 94,                     /* ECOND  */
  YYSYMBOL_CAMPOS = 95,                    /* CAMPOS  */
  YYSYMBOL_CAMPO = 96,                     /* CAMPO  */
  YYSYMBOL_E = 97,                         /* E  */
  YYSYMBOL_UN = 98,                        /* UN  */
  YYSYMBOL_F = 99,                         /* F  */
  YYSYMBOL_100_6 = 100,                    /* @6  */
  YYSYMBOL_CHAMA_FUNC = 101,               /* CHAMA_FUNC  */
  YYSYMBOL_LISTA_ARGS = 102,               /* LISTA_ARGS  */
  YYSYMBOL_ARGs = 103                      /* ARGs  */
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
#define YYFINAL  92
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   620

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  140
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  243

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
       0,   235,   235,   236,   239,   240,   244,   245,   246,   247,
     248,   251,   252,   253,   255,   257,   258,   259,   263,   267,
     271,   273,   276,   289,   307,   311,   316,   315,   369,   383,
     393,   394,   397,   399,   402,   403,   404,   405,   408,   417,
     420,   421,   424,   426,   430,   433,   434,   437,   439,   443,
     446,   447,   450,   452,   456,   462,   471,   476,   481,   486,
     493,   498,   501,   506,   513,   520,   521,   525,   528,   530,
     532,   534,   543,   553,   552,   610,   609,   667,   666,   744,
     743,   820,   830,   833,   840,   844,   846,   850,   852,   857,
     858,   861,   863,   872,   878,   881,   883,   886,   888,   892,
     894,   896,   898,   900,   902,   904,   906,   908,   910,   912,
     914,   916,   918,   922,   924,   926,   928,   930,   938,   947,
     958,   967,   972,   974,   975,   976,   977,   979,   980,   982,
     986,   988,   987,  1041,  1055,  1065,  1071,  1076,  1082,  1088,
    1093
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
  "BLOCO", "START_BLOCO", "END_BLOCO", "CMD_RET", "EMPILHA_TS_FUNC",
  "DESEMPILHA_TS_FUNC", "CMD_FUNC", "@1", "NOME_FUNCAO", "CMD_FOR",
  "ECOND_OPT", "EF", "SF", "CMD_WHILE", "CMD_LET", "LET_VARs", "LET_VAR",
  "CMD_VAR", "VAR_VARs", "VAR_VAR", "CMD_CONST", "CONST_VARs", "CONST_VAR",
  "CMD_IF", "LVALUEPROP", "LIST", "LISTVALS", "LISTVAL", "ATRIB",
  "ATRIB_NO_OBJ", "@2", "@3", "@4", "@5", "EOBJ", "OBJ_LITERAL", "PARAM",
  "LISTA_PARAMS", "LISTA_PARAMS_OPT", "LISTA_MISTA", "ECOND", "CAMPOS",
  "CAMPO", "E", "UN", "F", "@6", "CHAMA_FUNC", "LISTA_ARGS", "ARGs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-115)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-123)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     374,   -10,   -35,     5,    16,    22,    -3,    -1,     6,   420,
    -115,  -115,  -115,  -115,    54,   228,   504,   504,    19,   455,
    -115,     8,    31,   374,  -115,  -115,  -115,    13,  -115,  -115,
    -115,    15,    18,    21,  -115,   285,  -115,    28,   548,  -115,
     123,  -115,   420,   420,  -115,  -115,     7,   420,   462,    41,
    -115,    32,    50,  -115,    71,    72,  -115,    84,   413,   462,
    -115,   116,    79,    82,     1,  -115,  -115,  -115,   138,   420,
     141,   123,   138,   141,  -115,   141,   123,   123,  -115,   -23,
    -115,  -115,  -115,    76,    86,   100,   101,  -115,   104,    88,
    -115,   374,  -115,  -115,  -115,  -115,  -115,  -115,   420,   420,
    -115,  -115,   420,   420,   420,   134,  -115,   462,   462,   462,
     462,  -115,   462,   462,   462,   462,   462,   462,   462,   420,
     420,   145,  -115,  -115,   106,   497,  -115,    17,   -14,   107,
     420,     5,   462,    16,   462,    22,  -115,   108,  -115,  -115,
    -115,  -115,   527,   125,   111,  -115,   127,  -115,   115,   155,
     420,  -115,   420,   420,   420,   116,   159,  -115,   245,  -115,
    -115,  -115,   140,  -115,   142,   160,  -115,   581,   581,   569,
     569,   581,   581,    11,    11,   161,   161,   161,   147,   150,
    -115,  -115,  -115,  -115,   374,  -115,  -115,   548,  -115,   548,
    -115,   462,   374,   420,  -115,   116,   420,  -115,   194,  -115,
     167,   171,  -115,  -115,  -115,   157,  -115,  -115,  -115,   420,
    -115,  -115,   197,   162,   548,  -115,   163,   158,  -115,  -115,
     420,   497,   106,  -115,  -115,   374,   420,   374,  -115,  -115,
    -115,  -115,  -115,   164,   288,   165,   374,  -115,   374,  -115,
     331,  -115,  -115
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,   122,     0,     0,     0,     0,     0,     0,     0,    22,
     123,   125,   124,   126,     0,     0,     0,     0,     0,    90,
      17,    20,     0,     2,     5,    16,    15,     0,    10,    11,
      12,     0,     0,     0,     9,    99,   127,     0,    81,   111,
     121,   130,     0,     0,   115,   116,    75,     0,     0,    42,
      39,    41,    52,    49,    51,    47,    44,    46,    33,     0,
      28,    90,     0,     0,     0,    23,    65,    66,   113,     0,
     117,     0,   114,   118,   122,     0,   129,   128,    61,     0,
      63,    94,    64,    82,   122,    91,    87,    89,     0,     0,
      18,     0,     1,     4,    13,     6,     7,     8,     0,     0,
     119,   120,     0,     0,   138,     0,    14,     0,     0,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
     138,     0,    68,    69,     0,     0,    67,   122,    99,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    34,    35,
      36,    32,     0,    85,     0,    24,    98,    84,     0,    96,
       0,    60,     0,     0,     0,     0,     0,   133,     0,    71,
      72,    70,     0,   139,     0,   136,    57,   102,   103,   101,
     100,   104,   105,   106,   107,   108,   109,   110,     0,     0,
      59,    20,    74,    76,     0,    43,    40,    53,    50,    48,
      45,    31,     0,     0,   131,    90,     0,    83,     0,    62,
       0,    67,    86,    92,    88,    77,    21,    56,   135,   137,
      58,   134,    54,     0,    30,    38,     0,     0,    97,    95,
       0,     0,     0,    19,   140,     0,    33,     0,    26,    93,
      78,    80,    55,     0,     0,     0,     0,   132,     0,    29,
       0,    25,    27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -115,  -115,   -89,   -22,  -115,  -114,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,   -20,  -115,  -115,   166,
      77,  -115,   168,    78,  -115,   170,    90,  -115,  -115,    59,
    -115,  -115,    65,    -6,     0,  -115,  -115,  -115,  -115,     2,
    -115,  -115,    61,   -56,    63,  -115,    34,  -115,   -30,  -115,
      83,  -115,  -115,    99,  -115
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    22,    23,    24,    25,    26,    91,   223,    27,   195,
     242,    28,   235,    62,    29,   213,   136,   137,    30,    31,
      50,    51,    32,    56,    57,    33,    53,    54,    34,    35,
      36,    79,    80,    81,    66,   124,   125,   221,   222,    85,
      67,    86,    87,    88,    89,    83,   148,   149,    38,    39,
      40,   216,    41,   164,   165
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,    93,   158,    65,   146,   144,    48,   150,    49,    60,
     182,   100,   101,    42,    43,    44,    45,    46,   129,    52,
      82,    47,     1,    37,   151,    55,   103,   104,   105,   142,
      63,    92,    10,    11,    12,    13,   122,   123,    58,   111,
      59,   126,    44,    45,    14,    15,   147,    61,   116,   117,
     118,   -73,   141,    90,    16,    17,    94,    68,    95,    18,
      19,    96,   131,    64,    97,    63,    78,    10,    11,    12,
      13,   106,   130,    70,    73,    75,    75,   167,   168,   169,
     170,   132,   171,   172,   173,   174,   175,   176,   177,    16,
      17,    37,   159,   160,    18,    69,   161,    71,    71,    76,
      77,   133,   187,   134,   189,   162,   163,   128,   231,    42,
      43,    44,    45,    46,   135,   -85,   -85,   153,   128,   143,
     145,   178,   163,    61,   152,   183,   107,   108,   109,   110,
     154,   155,   185,   156,   157,   111,    93,   166,   234,   217,
     112,   113,   114,   115,   116,   117,   118,   201,   180,   240,
     181,   191,    82,   184,   200,   202,   193,   194,    37,   196,
     197,   214,   212,   119,   120,   121,   128,   128,   128,   128,
     215,   128,   128,   128,   128,   128,   128,   128,  -122,  -122,
    -122,   103,   104,   105,    37,   198,   205,   207,   208,   111,
     209,   128,    37,   128,   210,   202,   211,   146,   218,   220,
     -94,   -79,   225,   232,   228,   226,   233,   227,   186,   238,
     236,   224,    93,   190,   239,   199,   204,   203,    93,   179,
     141,   230,   229,   188,   138,    37,   139,    37,   140,     0,
       0,    72,   219,     0,    37,     0,    37,     0,    37,    63,
      37,    10,    11,    12,    13,     0,     0,     0,     1,     2,
     128,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,    16,    17,     0,     0,     0,    18,    69,
      14,    15,     0,     0,     0,     0,     0,     0,     0,     0,
      16,    17,     0,     0,     0,    18,    19,     0,    20,    21,
     206,     1,     2,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    98,    99,
     100,   101,     0,    14,    15,     0,   102,     0,     0,     0,
       0,     0,     0,    16,    17,   103,   104,   105,    18,    19,
       0,    20,    21,   237,     1,     2,     0,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,     0,     0,
       0,     0,     0,     0,     0,     0,    16,    17,     0,     0,
       0,    18,    19,     0,    20,    21,   241,     1,     2,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,     0,     0,     0,     0,     0,     0,     0,    14,
      15,     0,     0,     0,     0,     0,     0,     0,     0,    16,
      17,     0,     0,     0,    18,    19,     1,    20,    21,     3,
       4,     5,     0,     1,    63,     0,    10,    11,    12,    13,
       0,    63,     0,    10,    11,    12,    13,     0,    14,    15,
       0,     0,     0,     0,     0,    14,    15,     0,    16,    17,
       0,     0,     0,    18,    19,    16,    17,    64,    84,     0,
      18,    19,     0,     0,    64,   127,    63,     0,    10,    11,
      12,    13,     0,    63,     0,    10,    11,    12,    13,     0,
      14,    15,     0,     0,     0,     0,     0,    14,    15,     0,
      16,    17,     0,     0,     0,    18,    19,    16,    17,    64,
       1,     0,    18,    69,     0,     0,     0,    74,    63,     0,
      10,    11,    12,    13,     0,    63,     0,    10,    11,    12,
      13,     0,    14,    15,     0,     0,     0,     0,     0,     0,
       0,     0,    16,    17,     0,     0,     0,    18,    19,    16,
      17,     0,     0,     0,    18,    69,   107,   108,   109,   110,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
     112,   113,   114,   115,   116,   117,   118,   107,   108,   109,
     110,     0,     0,   192,     0,     0,   111,     0,     0,     0,
       0,   112,   113,   114,   115,   116,   117,   118,   107,   108,
    -123,  -123,     0,     0,     0,     0,     0,   111,     0,     0,
    -123,  -123,   112,   113,   114,   115,   116,   117,   118,   111,
       0,     0,     0,     0,  -123,  -123,   114,   115,   116,   117,
     118
};

static const yytype_int16 yycheck[] =
{
       0,    23,    91,     9,     3,    61,    41,    30,     3,     3,
     124,    25,    26,    23,    24,    25,    26,    27,    48,     3,
      18,    31,     3,    23,    47,     3,    40,    41,    42,    59,
      11,     0,    13,    14,    15,    16,    42,    43,    41,    28,
      41,    47,    25,    26,    25,    26,    45,    41,    37,    38,
      39,    44,    58,    45,    35,    36,    43,     3,    43,    40,
      41,    43,    30,    44,    43,    11,    47,    13,    14,    15,
      16,    43,    31,    14,    15,    16,    17,   107,   108,   109,
     110,    31,   112,   113,   114,   115,   116,   117,   118,    35,
      36,    91,    98,    99,    40,    41,   102,    14,    15,    16,
      17,    30,   132,    31,   134,   103,   104,    48,   222,    23,
      24,    25,    26,    27,    30,    29,    30,    31,    59,     3,
      41,   119,   120,    41,    48,   125,    19,    20,    21,    22,
      30,    30,   130,    29,    46,    28,   158,     3,   227,   195,
      33,    34,    35,    36,    37,    38,    39,   153,     3,   238,
      44,    43,   150,    46,   152,   153,    31,    46,   158,    32,
      45,   191,   184,    40,    41,    42,   107,   108,   109,   110,
     192,   112,   113,   114,   115,   116,   117,   118,    40,    41,
      42,    40,    41,    42,   184,    30,    27,    47,    46,    28,
      30,   132,   192,   134,    47,   193,    46,     3,   196,    32,
      29,    44,     5,   225,    46,    43,   226,    44,   131,    44,
      46,   209,   234,   135,   236,   150,   155,   154,   240,   120,
     226,   221,   220,   133,    58,   225,    58,   227,    58,    -1,
      -1,     3,   198,    -1,   234,    -1,   236,    -1,   238,    11,
     240,    13,    14,    15,    16,    -1,    -1,    -1,     3,     4,
     191,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    35,    36,    -1,    -1,    -1,    40,    41,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    -1,    -1,    -1,    40,    41,    -1,    43,    44,
      45,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    23,    24,
      25,    26,    -1,    25,    26,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    40,    41,    42,    40,    41,
      -1,    43,    44,    45,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    -1,
      -1,    40,    41,    -1,    43,    44,    45,     3,     4,    -1,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    -1,    -1,    40,    41,     3,    43,    44,     6,
       7,     8,    -1,     3,    11,    -1,    13,    14,    15,    16,
      -1,    11,    -1,    13,    14,    15,    16,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    35,    36,
      -1,    -1,    -1,    40,    41,    35,    36,    44,     3,    -1,
      40,    41,    -1,    -1,    44,     3,    11,    -1,    13,    14,
      15,    16,    -1,    11,    -1,    13,    14,    15,    16,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      35,    36,    -1,    -1,    -1,    40,    41,    35,    36,    44,
       3,    -1,    40,    41,    -1,    -1,    -1,     3,    11,    -1,
      13,    14,    15,    16,    -1,    11,    -1,    13,    14,    15,
      16,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    -1,    -1,    -1,    40,    41,    35,
      36,    -1,    -1,    -1,    40,    41,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    19,    20,    21,
      22,    -1,    -1,    46,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    33,    34,    35,    36,    37,    38,    39,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      19,    20,    33,    34,    35,    36,    37,    38,    39,    28,
      -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,    38,
      39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    25,    26,    35,    36,    40,    41,
      43,    44,    50,    51,    52,    53,    54,    57,    60,    63,
      67,    68,    71,    74,    77,    78,    79,    83,    97,    98,
      99,   101,    23,    24,    25,    26,    27,    31,    41,     3,
      69,    70,     3,    75,    76,     3,    72,    73,    41,    41,
       3,    41,    62,    11,    44,    82,    83,    89,     3,    41,
      78,    99,     3,    78,     3,    78,    99,    99,    47,    80,
      81,    82,    88,    94,     3,    88,    90,    91,    92,    93,
      45,    55,     0,    52,    43,    43,    43,    43,    23,    24,
      25,    26,    31,    40,    41,    42,    43,    19,    20,    21,
      22,    28,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    82,    82,    84,    85,    82,     3,    78,    97,
      31,    30,    31,    30,    31,    30,    65,    66,    68,    71,
      74,    82,    97,     3,    92,    41,     3,    45,    95,    96,
      30,    47,    48,    31,    30,    30,    29,    46,    51,    82,
      82,    82,    88,    88,   102,   103,     3,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    88,   102,
       3,    44,    54,    83,    46,    88,    69,    97,    75,    97,
      72,    43,    46,    31,    46,    58,    32,    45,    30,    81,
      88,    82,    88,    93,    91,    27,    45,    47,    46,    30,
      47,    46,    52,    64,    97,    52,   100,    92,    88,    95,
      32,    86,    87,    56,    88,     5,    43,    44,    46,    88,
      83,    54,    52,    65,    51,    61,    46,    45,    44,    52,
      51,    45,    59
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    53,    54,
      55,    56,    57,    57,    58,    59,    61,    60,    62,    63,
      64,    64,    65,    65,    66,    66,    66,    66,    67,    68,
      69,    69,    70,    70,    71,    72,    72,    73,    73,    74,
      75,    75,    76,    76,    77,    77,    78,    78,    78,    78,
      79,    79,    80,    80,    81,    82,    82,    83,    83,    83,
      83,    83,    83,    84,    83,    85,    83,    86,    83,    87,
      83,    83,    88,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    94,    94,    95,    95,    96,    96,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    99,    99,    99,    99,    99,    99,    99,    99,
      99,   100,    99,    99,   101,   101,   102,   102,   102,   103,
     103
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     2,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     2,     5,
       0,     0,     1,     2,     0,     0,     0,    11,     1,     9,
       1,     0,     1,     0,     1,     1,     1,     1,     5,     2,
       3,     1,     1,     3,     2,     3,     1,     1,     3,     2,
       3,     1,     1,     3,     5,     7,     4,     3,     4,     3,
       3,     2,     3,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     0,     4,     0,     4,     0,     6,     0,
       6,     1,     1,     3,     2,     1,     3,     1,     3,     1,
       0,     1,     3,     5,     1,     3,     1,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     0,     8,     3,     4,     4,     1,     2,     0,     1,
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
#line 235 "mini_js.y"
         { print( resolve_enderecos( yyvsp[0].c + "." + funcoes ) ); }
#line 1714 "y.tab.c"
    break;

  case 3: /* S: %empty  */
#line 236 "mini_js.y"
    { yyval.clear(); }
#line 1720 "y.tab.c"
    break;

  case 4: /* CMDs: CMDs CMD  */
#line 239 "mini_js.y"
                 { yyval.c = yyvsp[-1].c + yyvsp[0].c; }
#line 1726 "y.tab.c"
    break;

  case 14: /* CMD: ATRIB_NO_OBJ ';'  */
#line 256 "mini_js.y"
      { yyval.c = yyvsp[-1].c + "^"; }
#line 1732 "y.tab.c"
    break;

  case 17: /* CMD: ';'  */
#line 260 "mini_js.y"
      { yyval.clear(); }
#line 1738 "y.tab.c"
    break;

  case 18: /* BLOCO_VAZIO: '{' '}'  */
#line 264 "mini_js.y"
            { yyval.c = vector<string>{"<{", "}>"}; }
#line 1744 "y.tab.c"
    break;

  case 19: /* BLOCO: '{' START_BLOCO CMDs '}' END_BLOCO  */
#line 268 "mini_js.y"
        { yyval.c = "<{" + yyvsp[-2].c + "}>" ; }
#line 1750 "y.tab.c"
    break;

  case 20: /* START_BLOCO: %empty  */
#line 271 "mini_js.y"
              {empilha_escopo_novo();}
#line 1756 "y.tab.c"
    break;

  case 21: /* END_BLOCO: %empty  */
#line 273 "mini_js.y"
            {desempilha_escopo();}
#line 1762 "y.tab.c"
    break;

  case 22: /* CMD_RET: RETURN  */
#line 277 "mini_js.y"
          { 
            if(blocos_alinhados_em_funcao.size() <= 1 ) {
              cerr << "Erro: Não é permitido 'return' fora de funções." << endl;
              exit(1);
            }
            yyval.c = vector<string>{"undefined"}; /* 1. Empilha undefined */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            yyval.c =  yyval.c + vector<string>{ "'&retorno'", "@", "~" }; /* 3. Instruções de retorno */
          }
#line 1779 "y.tab.c"
    break;

  case 23: /* CMD_RET: RETURN ATRIB  */
#line 290 "mini_js.y"
          { 
            if(blocos_alinhados_em_funcao.size() <= 1) {
              cerr << "Erro: Não é permitido 'return' fora de funções." << endl;
              exit(1);
            }
            yyval.c = yyvsp[0].c; /* 1. Empilha o valor da expressão E */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                yyval.c = yyval.c + "}>"; /* 2. Fecha blocos aninhados */
            
            /* 3. Instruções de retorno (NÃO adicione $2.c de novo) */
            yyval.c = yyval.c + vector<string>{ "'&retorno'", "@", "~" }; 
          }
#line 1797 "y.tab.c"
    break;

  case 24: /* EMPILHA_TS_FUNC: %empty  */
#line 307 "mini_js.y"
                  { ts.push_back( map< string, Simbolo >{} ); 
                blocos_alinhados_em_funcao.push_back(0);}
#line 1804 "y.tab.c"
    break;

  case 25: /* DESEMPILHA_TS_FUNC: %empty  */
#line 311 "mini_js.y"
                     { ts.pop_back(); 
                      blocos_alinhados_em_funcao.pop_back();}
#line 1811 "y.tab.c"
    break;

  case 26: /* @1: %empty  */
#line 316 "mini_js.y"
           {
              // Mid-rule action: Declare parameters
              vector<string> codigo_params;
              for(size_t i=0; i<yyvsp[-1].lista_exprs.size(); i++) {
                 Atributos& p = yyvsp[-1].lista_exprs[i];
                 if (p.nome_var.empty()) {
                    cerr << "Erro: Parametro invalido na funcao " << yyvsp[-4].c[0] << endl;
                    exit(1);
                 }
                 Atributos decl = declara_var(Let, p); 
                 codigo_params += decl.c;
                 codigo_params += p.nome_var;
                 codigo_params += "arguments";
                 codigo_params += "@";
                 codigo_params += to_string(i);
                 codigo_params += "[@]";
                 codigo_params += "=";
                 codigo_params += "^";
                 
                 if (p.valor_default.size() > 0) {
                    string lbl = gera_label("fim_default");
                    codigo_params += p.nome_var;
                    codigo_params += "@";
                    codigo_params += "undefined";
                    codigo_params += "@";
                    codigo_params += "==";
                    codigo_params += "!";
                    codigo_params += lbl;
                    codigo_params += "?";
                    codigo_params += p.nome_var;
                    codigo_params += p.valor_default;
                    codigo_params += "=";
                    codigo_params += "^";
                    codigo_params += define_label(lbl);
                 }
              }
              yyval.c = codigo_params; // Pass generated code to next part
           }
#line 1854 "y.tab.c"
    break;

  case 27: /* CMD_FUNC: FUNCTION NOME_FUNCAO '(' EMPILHA_TS_FUNC LISTA_PARAMS_OPT ')' @1 '{' CMDs '}' DESEMPILHA_TS_FUNC  */
#line 355 "mini_js.y"
           { 
             string definicao_lbl_endereco_funcao = ":" + yyvsp[-9].endereco_funcao;
             
             // $7 contains the code generated by the mid-rule action
             vector<string> codigo_params = yyvsp[-4].c; 

             yyval.c = yyvsp[-9].c ; 

             funcoes = funcoes + definicao_lbl_endereco_funcao + codigo_params + yyvsp[-2].c +
                         "undefined" + "@" + "'&retorno'" + "@"+ "~";
             
           }
#line 1871 "y.tab.c"
    break;

  case 28: /* NOME_FUNCAO: ID  */
#line 370 "mini_js.y"
            { 
              string endereco_funcao = gera_label( "func_" + yyvsp[0].c[0] );
              yyval.endereco_funcao = endereco_funcao; /* 1. Salva o label para CMD_FUNC usar */
              
              Atributos decl = declara_var( Var, yyvsp[0] ); // 2. Declara a variável
              
              /* 3. Gera código usando decl.c (que pode ou não ter '&') e a var local 'endereco_funcao' */
              yyval.c = decl.c +                                    /* 'foo &' ou nada se for redeclaração */
                     yyvsp[0].c + "{}" + "="  + "'&funcao'" + endereco_funcao +  /* 'foo '&funcao' func_foo_1' ... */
                     "[=]" + "^";                              /* ... '[=] ^' (atribui prop) */
            }
#line 1887 "y.tab.c"
    break;

  case 29: /* CMD_FOR: FOR '(' SF ';' ECOND_OPT ';' EF ')' CMD  */
#line 384 "mini_js.y"
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          yyval.c = yyvsp[-6].c + define_label (teste_for) +
                 yyvsp[-4].c + "!" + fim_for + "?" + yyvsp[0].c + 
                 yyvsp[-2].c + teste_for + "#" + define_label(fim_for);
        }
#line 1899 "y.tab.c"
    break;

  case 31: /* ECOND_OPT: %empty  */
#line 394 "mini_js.y"
            { yyval.c = vector<string>{"1"}; }
#line 1905 "y.tab.c"
    break;

  case 32: /* EF: ATRIB  */
#line 398 "mini_js.y"
      { yyval.c = yyvsp[0].c + "^"; }
#line 1911 "y.tab.c"
    break;

  case 33: /* EF: %empty  */
#line 399 "mini_js.y"
      { yyval.clear(); }
#line 1917 "y.tab.c"
    break;

  case 38: /* CMD_WHILE: WHILE '(' E ')' CMD  */
#line 409 "mini_js.y"
        { string inicio_while = gera_label( "inicio_while" );
          string fim_while = gera_label( "fim_while" );
          yyval.c = define_label(inicio_while) + 
                 yyvsp[-2].c + "!" + fim_while + "?" + yyvsp[0].c + 
                 inicio_while + "#" + define_label(fim_while);
        }
#line 1928 "y.tab.c"
    break;

  case 39: /* CMD_LET: LET LET_VARs  */
#line 417 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1934 "y.tab.c"
    break;

  case 40: /* LET_VARs: LET_VAR ',' LET_VARs  */
#line 420 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1940 "y.tab.c"
    break;

  case 42: /* LET_VAR: ID  */
#line 425 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[0] ).c; }
#line 1946 "y.tab.c"
    break;

  case 43: /* LET_VAR: ID '=' EOBJ  */
#line 427 "mini_js.y"
          { yyval.c = declara_var( Let, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1952 "y.tab.c"
    break;

  case 44: /* CMD_VAR: VAR VAR_VARs  */
#line 430 "mini_js.y"
                       { yyval.c = yyvsp[0].c; }
#line 1958 "y.tab.c"
    break;

  case 45: /* VAR_VARs: VAR_VAR ',' VAR_VARs  */
#line 433 "mini_js.y"
                                { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1964 "y.tab.c"
    break;

  case 47: /* VAR_VAR: ID  */
#line 438 "mini_js.y"
          { yyval.c = declara_var( Var, yyvsp[0] ).c; }
#line 1970 "y.tab.c"
    break;

  case 48: /* VAR_VAR: ID '=' E  */
#line 440 "mini_js.y"
          {  yyval.c = declara_var( Var, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 1976 "y.tab.c"
    break;

  case 49: /* CMD_CONST: CONST CONST_VARs  */
#line 443 "mini_js.y"
                            { yyval.c = yyvsp[0].c; }
#line 1982 "y.tab.c"
    break;

  case 50: /* CONST_VARs: CONST_VAR ',' CONST_VARs  */
#line 446 "mini_js.y"
                                      { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 1988 "y.tab.c"
    break;

  case 52: /* CONST_VAR: ID  */
#line 451 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[0] ).c; }
#line 1994 "y.tab.c"
    break;

  case 53: /* CONST_VAR: ID '=' E  */
#line 453 "mini_js.y"
            { yyval.c = declara_var( Const, yyvsp[-2] ).c + yyvsp[-2].c[0] + yyvsp[0].c + "=" + "^"; }
#line 2000 "y.tab.c"
    break;

  case 54: /* CMD_IF: IF '(' E ')' CMD  */
#line 457 "mini_js.y"
          {
            string fim_if = gera_label( "fim_if" );
            yyval.c = yyvsp[-2].c + "!" + fim_if + "?" + yyvsp[0].c + define_label(fim_if);
          }
#line 2009 "y.tab.c"
    break;

  case 55: /* CMD_IF: IF '(' E ')' CMD ELSE CMD  */
#line 463 "mini_js.y"
          {  string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            yyval.c = yyvsp[-4].c + "!" + else_if + "?" + yyvsp[-2].c + fim_if + 
            "#" + define_label(else_if) + yyvsp[0].c +  define_label(fim_if);
          }
#line 2019 "y.tab.c"
    break;

  case 56: /* LVALUEPROP: LVALUEPROP '[' EOBJ ']'  */
#line 472 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].esq + yyvsp[-3].dir + "[@]";
              yyval.dir = yyvsp[-1].c;
               }
#line 2028 "y.tab.c"
    break;

  case 57: /* LVALUEPROP: LVALUEPROP '.' ID  */
#line 477 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].esq + yyvsp[-2].dir + "[@]";
              yyval.dir = vector<string>{"'" + yyvsp[0].c[0] + "'"};
               }
#line 2037 "y.tab.c"
    break;

  case 58: /* LVALUEPROP: F '[' EOBJ ']'  */
#line 482 "mini_js.y"
             { yyval.c.clear(); 
              yyval.esq = yyvsp[-3].c;
              yyval.dir = yyvsp[-1].c;
               }
#line 2046 "y.tab.c"
    break;

  case 59: /* LVALUEPROP: F '.' ID  */
#line 487 "mini_js.y"
            { yyval.c.clear(); 
              yyval.esq = yyvsp[-2].c;
              yyval.dir = vector<string>{"'" + yyvsp[0].c[0] + "'"};
              }
#line 2055 "y.tab.c"
    break;

  case 60: /* LIST: '[' LISTVALS ']'  */
#line 494 "mini_js.y"
        { 
          yyval.c = vector<string>{"[]"};
          yyval.c += yyvsp[-1].c;
        }
#line 2064 "y.tab.c"
    break;

  case 61: /* LIST: '[' ']'  */
#line 498 "mini_js.y"
                { yyval.c = vector<string>{"[]"}; }
#line 2070 "y.tab.c"
    break;

  case 62: /* LISTVALS: LISTVALS ',' LISTVAL  */
#line 502 "mini_js.y"
         { 
           yyval.c = yyvsp[-2].c + to_string(yyvsp[-2].n_args) + yyvsp[0].c + "[<=]"; 
           yyval.n_args = yyvsp[-2].n_args + 1;
         }
#line 2079 "y.tab.c"
    break;

  case 63: /* LISTVALS: LISTVAL  */
#line 507 "mini_js.y"
         {
           yyval.c = vector<string>{"0"} + yyvsp[0].c + "[<=]";
           yyval.n_args = 1;
         }
#line 2088 "y.tab.c"
    break;

  case 64: /* LISTVAL: EOBJ  */
#line 514 "mini_js.y"
        { 
          yyval.c = yyvsp[0].c;
          yyval.n_args = 0; 
        }
#line 2097 "y.tab.c"
    break;

  case 67: /* ATRIB_NO_OBJ: ID '=' ATRIB  */
#line 526 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[0].c + "="; 
      yyval.nome_var = yyvsp[-2].c[0]; yyval.valor_default = yyvsp[0].c; yyval.eh_atribuicao = true; }
#line 2104 "y.tab.c"
    break;

  case 68: /* ATRIB_NO_OBJ: ID MAIS_IGUAL ATRIB  */
#line 529 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "+" + "="; yyval.eh_atribuicao = true; yyval.nome_var = yyvsp[-2].c[0]; }
#line 2110 "y.tab.c"
    break;

  case 69: /* ATRIB_NO_OBJ: ID MENOS_IGUAL ATRIB  */
#line 531 "mini_js.y"
    { checa_simbolo( yyvsp[-2].c[0], true ); yyval.c = yyvsp[-2].c + yyvsp[-2].c + "@" + yyvsp[0].c + "-" + "="; yyval.eh_atribuicao = true; yyval.nome_var = yyvsp[-2].c[0]; }
#line 2116 "y.tab.c"
    break;

  case 70: /* ATRIB_NO_OBJ: LVALUEPROP '=' ATRIB  */
#line 533 "mini_js.y"
    { yyval.c = yyvsp[-2].esq + yyvsp[-2].dir + yyvsp[0].c + "[=]"; }
#line 2122 "y.tab.c"
    break;

  case 71: /* ATRIB_NO_OBJ: LVALUEPROP MAIS_IGUAL ATRIB  */
#line 535 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[-2].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[-2].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  yyvsp[0].c + "+" + "[=]" ;}
#line 2135 "y.tab.c"
    break;

  case 72: /* ATRIB_NO_OBJ: LVALUEPROP MENOS_IGUAL ATRIB  */
#line 544 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[-2].c + "[@]" + yyvsp[0].c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[-2].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[-2].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  yyvsp[0].c + "-" + "[=]" ;}
#line 2148 "y.tab.c"
    break;

  case 73: /* @2: %empty  */
#line 553 "mini_js.y"
    {
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      Atributos decl = declara_var(Let, yyvsp[-1]);
      
      string endereco_funcao = gera_label( "func_arrow_" + yyvsp[-1].c[0] );
      
      // Code to init param
      vector<string> codigo_params;
      codigo_params += decl.c;
      codigo_params += yyvsp[-1].c;
      codigo_params += "arguments";
      codigo_params += "@";
      codigo_params += "0";
      codigo_params += "[@]";
      codigo_params += "=";
      codigo_params += "^";
      
      yyval.c = codigo_params;
      yyval.endereco_funcao = endereco_funcao;
    }
#line 2175 "y.tab.c"
    break;

  case 74: /* ATRIB_NO_OBJ: ID SETA @2 BLOCO  */
#line 576 "mini_js.y"
    {
      string endereco_funcao = yyvsp[-1].endereco_funcao;
      vector<string> codigo_params = yyvsp[-1].c;
      
      yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      
      set<string> ids_usados = extrai_ids_de_codigo(yyvsp[0].c);
      set<string> parametros;
      parametros.insert(yyvsp[-3].c[0]);
      
      set<string> locais;
      for(auto& p : ts.back()) locais.insert(p.first);
      
      set<string> capturas = detecta_capturas(ids_usados, parametros, locais);
      
       if (capturas.size() > 0) {
           yyval.c += "'captura'";
           yyval.c += "{}";
           for(const string& cap : capturas) {
               yyval.c += "'" + cap + "'";
               yyval.c += cap;
               yyval.c += "@";
               yyval.c += "[<=]";
           }
           yyval.c += "[<=]";
       }

      funcoes = funcoes + define_label(endereco_funcao) + codigo_params + yyvsp[0].c +
                "undefined" + "'&retorno'" + "@" + "~";
      
      ts.pop_back(); 
      blocos_alinhados_em_funcao.pop_back();
    }
#line 2213 "y.tab.c"
    break;

  case 75: /* @3: %empty  */
#line 610 "mini_js.y"
    {
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      Atributos decl = declara_var(Let, yyvsp[-1]);
      
      string endereco_funcao = gera_label( "func_arrow_" + yyvsp[-1].c[0] );
      
      // Code to init param
      vector<string> codigo_params;
      codigo_params += decl.c;
      codigo_params += yyvsp[-1].c;
      codigo_params += "arguments";
      codigo_params += "@";
      codigo_params += "0";
      codigo_params += "[@]";
      codigo_params += "=";
      codigo_params += "^";
      
      yyval.c = codigo_params;
      yyval.endereco_funcao = endereco_funcao;
    }
#line 2240 "y.tab.c"
    break;

  case 76: /* ATRIB_NO_OBJ: ID SETA @3 ATRIB_NO_OBJ  */
#line 633 "mini_js.y"
    {
      string endereco_funcao = yyvsp[-1].endereco_funcao;
      vector<string> codigo_params = yyvsp[-1].c;
      
      yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      
      set<string> ids_usados = extrai_ids_de_codigo(yyvsp[0].c);
      set<string> parametros;
      parametros.insert(yyvsp[-3].c[0]);
      
      set<string> locais;
      for(auto& p : ts.back()) locais.insert(p.first);
      
      set<string> capturas = detecta_capturas(ids_usados, parametros, locais);
      
       if (capturas.size() > 0) {
           yyval.c += "'captura'";
           yyval.c += "{}";
           for(const string& cap : capturas) {
               yyval.c += "'" + cap + "'";
               yyval.c += cap;
               yyval.c += "@";
               yyval.c += "[<=]";
           }
           yyval.c += "[<=]";
       }

      funcoes = funcoes + define_label(endereco_funcao) + codigo_params + yyvsp[0].c +
                "'&retorno'" + "@" + "~";
      
      ts.pop_back(); 
      blocos_alinhados_em_funcao.pop_back();
    }
#line 2278 "y.tab.c"
    break;

  case 77: /* @4: %empty  */
#line 667 "mini_js.y"
    {
      string endereco_funcao = gera_label( "func_arrow" );
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      vector<string> codigo_params;
      for(size_t i=0; i<yyvsp[-2].lista_exprs.size(); i++) {
         Atributos& p = yyvsp[-2].lista_exprs[i];
         if (p.nome_var.empty()) {
            cerr << "Erro: Parametro invalido na posicao " << i+1 << endl;
            exit(1);
         }
         Atributos decl = declara_var(Let, p); 
         codigo_params += decl.c;
         codigo_params += p.nome_var;
         codigo_params += "arguments";
         codigo_params += "@";
         codigo_params += to_string(i);
         codigo_params += "[@]";
         codigo_params += "=";
         codigo_params += "^";
         
         if (p.valor_default.size() > 0) {
            string lbl = gera_label("fim_default");
            codigo_params += p.nome_var;
            codigo_params += "@";
            codigo_params += "undefined";
            codigo_params += "@";
            codigo_params += "==";
            codigo_params += "!";
            codigo_params += lbl;
            codigo_params += "?";
            codigo_params += p.nome_var;
            codigo_params += p.valor_default;
            codigo_params += "=";
            codigo_params += "^";
            codigo_params += define_label(lbl);
         }
      }
      yyval.c = codigo_params;
      yyval.endereco_funcao = endereco_funcao;
    }
#line 2325 "y.tab.c"
    break;

  case 78: /* ATRIB_NO_OBJ: '(' LISTA_PARAMS_OPT FECHA_PARENTESES_LAMBDA SETA @4 ATRIB_NO_OBJ  */
#line 710 "mini_js.y"
    {
       string endereco_funcao = yyvsp[-1].endereco_funcao;
       vector<string> codigo_params = yyvsp[-1].c;

       yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
       
       set<string> ids_usados = extrai_ids_de_codigo(yyvsp[0].c);
       set<string> parametros;
       for(auto& p : yyvsp[-4].lista_exprs) parametros.insert(p.nome_var);
       
       set<string> locais;
       for(auto& p : ts.back()) locais.insert(p.first);
       
       set<string> capturas = detecta_capturas(ids_usados, parametros, locais);
       
       if (capturas.size() > 0) {
           yyval.c += "'captura'";
           yyval.c += "{}";
           for(const string& cap : capturas) {
               if(cap == "print" && busca_simbolo("print") == nullptr) continue;
               yyval.c += "'" + cap + "'";
               yyval.c += cap;
               yyval.c += "@";
               yyval.c += "[<=]";
           }
           yyval.c += "[<=]";
       }

       funcoes = funcoes + define_label(endereco_funcao) + codigo_params + yyvsp[0].c +
                 "'&retorno'" + "@" + "~";
       ts.pop_back(); 
       blocos_alinhados_em_funcao.pop_back();
    }
#line 2363 "y.tab.c"
    break;

  case 79: /* @5: %empty  */
#line 744 "mini_js.y"
    {
      string endereco_funcao = gera_label( "func_arrow" );
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      vector<string> codigo_params;
      for(size_t i=0; i<yyvsp[-2].lista_exprs.size(); i++) {
         Atributos& p = yyvsp[-2].lista_exprs[i];
         if (p.nome_var.empty()) {
            cerr << "Erro: Parametro invalido na posicao " << i+1 << endl;
            exit(1);
         }
         Atributos decl = declara_var(Let, p); 
         codigo_params += decl.c;
         codigo_params += p.nome_var;
         codigo_params += "arguments";
         codigo_params += "@";
         codigo_params += to_string(i);
         codigo_params += "[@]";
         codigo_params += "=";
         codigo_params += "^";
         
         if (p.valor_default.size() > 0) {
            string lbl = gera_label("fim_default");
            codigo_params += p.nome_var;
            codigo_params += "@";
            codigo_params += "undefined";
            codigo_params += "@";
            codigo_params += "==";
            codigo_params += "!";
            codigo_params += lbl;
            codigo_params += "?";
            codigo_params += p.nome_var;
            codigo_params += p.valor_default;
            codigo_params += "=";
            codigo_params += "^";
            codigo_params += define_label(lbl);
         }
      }
      yyval.c = codigo_params;
      yyval.endereco_funcao = endereco_funcao;
    }
#line 2410 "y.tab.c"
    break;

  case 80: /* ATRIB_NO_OBJ: '(' LISTA_PARAMS_OPT FECHA_PARENTESES_LAMBDA SETA @5 BLOCO  */
#line 787 "mini_js.y"
    {
       string endereco_funcao = yyvsp[-1].endereco_funcao;
       vector<string> codigo_params = yyvsp[-1].c;

       yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
       
       set<string> ids_usados = extrai_ids_de_codigo(yyvsp[0].c);
       set<string> parametros;
       for(auto& p : yyvsp[-4].lista_exprs) parametros.insert(p.nome_var);
       
       set<string> locais;
       for(auto& p : ts.back()) locais.insert(p.first);
       
       set<string> capturas = detecta_capturas(ids_usados, parametros, locais);
       
       if (capturas.size() > 0) {
           yyval.c += "'captura'";
           yyval.c += "{}";
           for(const string& cap : capturas) {
               if(cap == "print" && busca_simbolo("print") == nullptr) continue;
               yyval.c += "'" + cap + "'";
               yyval.c += cap;
               yyval.c += "@";
               yyval.c += "[<=]";
           }
           yyval.c += "[<=]";
       }

       funcoes = funcoes + define_label(endereco_funcao) + codigo_params + yyvsp[0].c +
                 "undefined" + "'&retorno'" + "@" + "~";
       ts.pop_back(); 
       blocos_alinhados_em_funcao.pop_back();
    }
#line 2448 "y.tab.c"
    break;

  case 81: /* ATRIB_NO_OBJ: E  */
#line 821 "mini_js.y"
    { yyval.c = yyvsp[0].c; yyval.nome_var = yyvsp[0].nome_var; }
#line 2454 "y.tab.c"
    break;

  case 83: /* OBJ_LITERAL: '{' CAMPOS '}'  */
#line 834 "mini_js.y"
    { 
      yyval.c = vector<string>{"{}"};
      if(yyvsp[-1].c.size() > 0) {
        yyval.c += yyvsp[-1].c;
      }
    }
#line 2465 "y.tab.c"
    break;

  case 84: /* OBJ_LITERAL: '{' '}'  */
#line 841 "mini_js.y"
    { yyval.c = vector<string>{"{}"}; }
#line 2471 "y.tab.c"
    break;

  case 85: /* PARAM: ID  */
#line 845 "mini_js.y"
      { yyval.nome_var = yyvsp[0].c[0]; yyval.c = yyvsp[0].c; }
#line 2477 "y.tab.c"
    break;

  case 86: /* PARAM: ID '=' EOBJ  */
#line 847 "mini_js.y"
      { yyval.nome_var = yyvsp[-2].c[0]; yyval.valor_default = yyvsp[0].c; yyval.c = yyvsp[-2].c; }
#line 2483 "y.tab.c"
    break;

  case 87: /* LISTA_PARAMS: PARAM  */
#line 851 "mini_js.y"
             { yyval.lista_exprs = {yyvsp[0]}; }
#line 2489 "y.tab.c"
    break;

  case 88: /* LISTA_PARAMS: PARAM ',' LISTA_PARAMS  */
#line 853 "mini_js.y"
             { yyval.lista_exprs = {yyvsp[-2]}; 
               yyval.lista_exprs.insert(yyval.lista_exprs.end(), yyvsp[0].lista_exprs.begin(), yyvsp[0].lista_exprs.end()); }
#line 2496 "y.tab.c"
    break;

  case 90: /* LISTA_PARAMS_OPT: %empty  */
#line 858 "mini_js.y"
                   { yyval.clear(); }
#line 2502 "y.tab.c"
    break;

  case 91: /* LISTA_MISTA: EOBJ  */
#line 862 "mini_js.y"
            { yyval.lista_exprs = {yyvsp[0]}; }
#line 2508 "y.tab.c"
    break;

  case 92: /* LISTA_MISTA: EOBJ ',' LISTA_MISTA  */
#line 864 "mini_js.y"
            { yyval.lista_exprs = {yyvsp[-2]}; 
              yyval.lista_exprs.insert(yyval.lista_exprs.end(), yyvsp[0].lista_exprs.begin(), yyvsp[0].lista_exprs.end()); }
#line 2515 "y.tab.c"
    break;

  case 93: /* ECOND: ECOND '?' EOBJ ':' EOBJ  */
#line 873 "mini_js.y"
      {
        string lbl_false = gera_label("ternary_false");
        string lbl_end = gera_label("ternary_end");
        yyval.c = yyvsp[-4].c + "!" + lbl_false + "?" + yyvsp[-2].c + lbl_end + "#" + define_label(lbl_false) + yyvsp[0].c + define_label(lbl_end);
      }
#line 2525 "y.tab.c"
    break;

  case 95: /* CAMPOS: CAMPO ',' CAMPOS  */
#line 882 "mini_js.y"
       { yyval.c = yyvsp[-2].c + yyvsp[0].c; }
#line 2531 "y.tab.c"
    break;

  case 97: /* CAMPO: ID ':' EOBJ  */
#line 887 "mini_js.y"
      { yyval.c = "'" + yyvsp[-2].c[0] + "'" + yyvsp[0].c + "[<=]"; }
#line 2537 "y.tab.c"
    break;

  case 98: /* CAMPO: ID  */
#line 889 "mini_js.y"
      { yyval.c = "'" + yyvsp[0].c[0] + "'" + yyvsp[0].c + "@" + "[<=]"; }
#line 2543 "y.tab.c"
    break;

  case 99: /* E: LVALUEPROP  */
#line 893 "mini_js.y"
    { yyval.c = yyvsp[0].esq + yyvsp[0].dir + "[@]"; }
#line 2549 "y.tab.c"
    break;

  case 100: /* E: E IGUAL E  */
#line 895 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c;}
#line 2555 "y.tab.c"
    break;

  case 101: /* E: E DIF E  */
#line 897 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c;}
#line 2561 "y.tab.c"
    break;

  case 102: /* E: E ME_IG E  */
#line 899 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c;}
#line 2567 "y.tab.c"
    break;

  case 103: /* E: E MA_IG E  */
#line 901 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c;}
#line 2573 "y.tab.c"
    break;

  case 104: /* E: E '<' E  */
#line 903 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2579 "y.tab.c"
    break;

  case 105: /* E: E '>' E  */
#line 905 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2585 "y.tab.c"
    break;

  case 106: /* E: E '+' E  */
#line 907 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2591 "y.tab.c"
    break;

  case 107: /* E: E '-' E  */
#line 909 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2597 "y.tab.c"
    break;

  case 108: /* E: E '*' E  */
#line 911 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2603 "y.tab.c"
    break;

  case 109: /* E: E '/' E  */
#line 913 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2609 "y.tab.c"
    break;

  case 110: /* E: E '%' E  */
#line 915 "mini_js.y"
    { yyval.c = yyvsp[-2].c + yyvsp[0].c + yyvsp[-1].c; }
#line 2615 "y.tab.c"
    break;

  case 111: /* E: UN  */
#line 917 "mini_js.y"
    { yyval.c = yyvsp[0].c; yyval.nome_var = yyvsp[0].nome_var; }
#line 2621 "y.tab.c"
    break;

  case 112: /* E: E ASM  */
#line 919 "mini_js.y"
    { yyval.c = yyvsp[-1].c + yyvsp[0].c; }
#line 2627 "y.tab.c"
    break;

  case 113: /* UN: MAIS_MAIS ID  */
#line 923 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "+" + "=";}
#line 2633 "y.tab.c"
    break;

  case 114: /* UN: MENOS_MENOS ID  */
#line 925 "mini_js.y"
        {yyval.c = yyvsp[0].c + yyvsp[0].c + "@" + "1" + "-" + "=";}
#line 2639 "y.tab.c"
    break;

  case 115: /* UN: ID MAIS_MAIS  */
#line 927 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "+" + "=" + "^";}
#line 2645 "y.tab.c"
    break;

  case 116: /* UN: ID MENOS_MENOS  */
#line 929 "mini_js.y"
        {yyval.c = yyvsp[-1].c + "@" + yyvsp[-1].c + yyvsp[-1].c + "@" + "1" + "-" + "=" + "^";}
#line 2651 "y.tab.c"
    break;

  case 117: /* UN: MAIS_MAIS LVALUEPROP  */
#line 931 "mini_js.y"
        { string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[0].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[0].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "+" + "[=]" ;}
#line 2663 "y.tab.c"
    break;

  case 118: /* UN: MENOS_MENOS LVALUEPROP  */
#line 939 "mini_js.y"
        {
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          yyval.c =  to_inst(esq) + "&" + esq + yyvsp[0].esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + yyvsp[0].dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "-" + "[=]" ;}
#line 2676 "y.tab.c"
    break;

  case 119: /* UN: LVALUEPROP MAIS_MAIS  */
#line 948 "mini_js.y"
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
#line 2691 "y.tab.c"
    break;

  case 120: /* UN: LVALUEPROP MENOS_MENOS  */
#line 959 "mini_js.y"
        {string esq = gera_temp("esq");
         string dir = gera_temp("dir");
         yyval.c = to_inst(esq) + "&" + esq + yyvsp[-1].esq + "=" + "^" +
                to_inst(dir) + "&" + dir + yyvsp[-1].dir + "=" + "^" +
                esq + "@" + dir + "@" + "[@]" +
                esq + "@" + dir + "@" +
                esq + "@" + dir + "@" + "[@]" +
                "1" + "-" + "[=]" + "^";}
#line 2704 "y.tab.c"
    break;

  case 122: /* F: ID  */
#line 973 "mini_js.y"
    { yyval.c = yyvsp[0].c + "@"; yyval.nome_var = yyvsp[0].c[0]; }
#line 2710 "y.tab.c"
    break;

  case 128: /* F: '-' F  */
#line 981 "mini_js.y"
      { yyval.c = "0" + yyvsp[0].c + "-" ; }
#line 2716 "y.tab.c"
    break;

  case 129: /* F: '+' F  */
#line 983 "mini_js.y"
      { yyval.c = "0" + yyvsp[0].c + "+" ; }
#line 2722 "y.tab.c"
    break;

  case 131: /* @6: %empty  */
#line 988 "mini_js.y"
      {
         string endereco_funcao = gera_label( "func_anon" );
         ts.push_back( map< string, Simbolo >{} ); 
         blocos_alinhados_em_funcao.push_back(0);
         
         vector<string> codigo_params;
         for(size_t i=0; i<yyvsp[-1].lista_exprs.size(); i++) {
            Atributos& p = yyvsp[-1].lista_exprs[i];
            if (p.nome_var.empty()) {
               cerr << "Erro: Parametro invalido na funcao anonima" << endl;
               exit(1);
            }
            Atributos decl = declara_var(Let, p); 
            codigo_params += decl.c;
            codigo_params += p.nome_var;
            codigo_params += "arguments";
            codigo_params += "@";
            codigo_params += to_string(i);
            codigo_params += "[@]";
            codigo_params += "=";
            codigo_params += "^";
            
            if (p.valor_default.size() > 0) {
               string lbl = gera_label("fim_default");
               codigo_params += p.nome_var;
               codigo_params += "@";
               codigo_params += "undefined";
               codigo_params += "@";
               codigo_params += "==";
               codigo_params += "!";
               codigo_params += lbl;
               codigo_params += "?";
               codigo_params += p.nome_var;
               codigo_params += p.valor_default;
               codigo_params += "=";
               codigo_params += "^";
               codigo_params += define_label(lbl);
            }
         }
         yyval.c = codigo_params;
         yyval.endereco_funcao = endereco_funcao;
      }
#line 2769 "y.tab.c"
    break;

  case 132: /* F: FUNCTION '(' LISTA_PARAMS_OPT ')' @6 '{' CMDs '}'  */
#line 1031 "mini_js.y"
      {
         string endereco_funcao = yyvsp[-3].endereco_funcao;
         vector<string> codigo_params = yyvsp[-3].c;

         yyval.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
         funcoes = funcoes + define_label(endereco_funcao) + codigo_params + yyvsp[-1].c +
                   "undefined" + "'&retorno'" + "@" + "~";
         ts.pop_back(); 
         blocos_alinhados_em_funcao.pop_back();
      }
#line 2784 "y.tab.c"
    break;

  case 133: /* F: '(' LISTA_MISTA ')'  */
#line 1042 "mini_js.y"
      { 
         yyval.c.clear();
         for(size_t i=0; i<yyvsp[-1].lista_exprs.size(); i++) {
            if (i > 0) yyval.c.push_back("^");
            yyval.c += yyvsp[-1].lista_exprs[i].c;
            
            if (yyvsp[-1].lista_exprs[i].eh_atribuicao) {
               checa_simbolo(yyvsp[-1].lista_exprs[i].nome_var, true);
            }
         }
      }
#line 2800 "y.tab.c"
    break;

  case 134: /* CHAMA_FUNC: F '(' LISTA_ARGS ')'  */
#line 1056 "mini_js.y"
              { 
                if(yyvsp[-3].nome_var == "print" && busca_simbolo("print") == nullptr) {
                   vector<string> args_code = yyvsp[-1].c;
                   if(!args_code.empty()) args_code.pop_back();
                   yyval.c = args_code + "print" + "#" + "undefined";
                } else {
                   yyval.c = yyvsp[-1].c + yyvsp[-3].c + "$" ;
                }
              }
#line 2814 "y.tab.c"
    break;

  case 135: /* CHAMA_FUNC: LVALUEPROP '(' LISTA_ARGS ')'  */
#line 1066 "mini_js.y"
              { 
                yyval.c = yyvsp[-1].c + yyvsp[-3].esq + yyvsp[-3].dir + "[@]" + "$" ;
              }
#line 2822 "y.tab.c"
    break;

  case 136: /* LISTA_ARGS: ARGs  */
#line 1072 "mini_js.y"
           { 
             yyval.c = yyvsp[0].c + to_string( yyvsp[0].n_args ); 
             yyval.n_args = yyvsp[0].n_args;
           }
#line 2831 "y.tab.c"
    break;

  case 137: /* LISTA_ARGS: ARGs ','  */
#line 1077 "mini_js.y"
           {
             yyval.c = yyvsp[-1].c + to_string( yyvsp[-1].n_args ); 
             yyval.n_args = yyvsp[-1].n_args;
           }
#line 2840 "y.tab.c"
    break;

  case 138: /* LISTA_ARGS: %empty  */
#line 1082 "mini_js.y"
           { 
              yyval.c = vector<string>{"0"}; 
              yyval.n_args = 0; 
           }
#line 2849 "y.tab.c"
    break;

  case 139: /* ARGs: EOBJ  */
#line 1089 "mini_js.y"
      { 
        yyval.c = yyvsp[0].c;     
        yyval.n_args = 1;   
      }
#line 2858 "y.tab.c"
    break;

  case 140: /* ARGs: ARGs ',' EOBJ  */
#line 1094 "mini_js.y"
      { 
        yyval.c = yyvsp[-2].c + yyvsp[0].c; 
        yyval.n_args = yyvsp[-2].n_args + 1; 
      }
#line 2867 "y.tab.c"
    break;


#line 2871 "y.tab.c"

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

#line 1100 "mini_js.y"


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

  // cerr << "Variavel '" << nome << "' não declarada." << endl;
  // exit( 1 );     
}

void yyerror( const char* st ) {
  cerr << st << endl; 
  cerr << "Linha: " << linha << " Coluna: " << coluna << endl;
   exit( 1 );
}

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}
