%{
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>

using namespace std;

int linha = 1, coluna = 0; 

struct Atributos {
  vector<string> c; // Código
  string endereco_funcao;
  int linha = 0, coluna = 0;

  vector<string> esq; // usado para LVALUEPROP
  vector<string> dir; // usado para LVALUEPROP
  
  int n_args = 0;     
  int contador = 0;
  
  vector<string> valor_default; 
  
  // Para captura de variáveis
  set<string> variaveis_usadas;
  set<string> variaveis_declaradas;

  void clear() {
    c.clear();
    valor_default.clear();
    linha = 0;
    coluna = 0;
    n_args = 0;
    variaveis_usadas.clear();
    variaveis_declaradas.clear();
  }
};

enum TipoDecl { Let = 1, Const, Var };

struct Simbolo {
  TipoDecl tipo;
  int linha;
  int coluna;
};

vector< map< string, Simbolo > > ts = { map< string, Simbolo >{} }; 
vector<string> funcoes;
vector<int> blocos_alinhados_em_funcao = {0};

Atributos declara_var( TipoDecl tipo, Atributos atrib );
void checa_simbolo( string nome, bool modificavel );
Simbolo* busca_simbolo( string nome );
set<string> calcula_captura(const set<string>& usadas, const set<string>& declaradas);

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

vector<string>& operator+=(vector<string>& a, const vector<string>& b) {
  a.insert(a.end(), b.begin(), b.end());
  return a;
}

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

void empilha_escopo_novo();
void desempilha_escopo();
%}

%token ID IF ELSE LET CONST VAR FOR WHILE FUNCTION RETURN 
%token CDOUBLE CSTRING CINT CBOOL
%token AND OR ME_IG MA_IG DIF IGUAL
%token MAIS_IGUAL MENOS_IGUAL MAIS_MAIS MENOS_MENOS SETA
%token ASM

%left ','
%right '?' ':'
%right '=' SETA MAIS_IGUAL MENOS_IGUAL
%left OR
%left AND
%nonassoc IGUAL DIF
%nonassoc '<' '>' ME_IG MA_IG
%left '+' '-'
%left '*' '/' '%'
%right MAIS_MAIS MENOS_MENOS
%left '[' '(' '.'

%%

S : CMDs { print( resolve_enderecos( $1.c + "." + funcoes ) ); }
  | { $$.clear(); }
  ;

CMDs : CMDs CMD  { $$.c = $1.c + $2.c; }
     | CMD
     ;

CMD : CMD_LET ';'
    | CMD_VAR ';'
    | CMD_CONST ';'
    | CMD_IF
    | CMD_FUNC
    | CMD_FOR
    | CMD_WHILE
    | CMD_RET ';'
    | CMD_ASM ';'
    | ATRIB ';'
      { $$.c = $1.c + "^"; } 
    | BLOCO
    | BLOCO_VAZIO
    | ';'
      { $$.clear(); }
    ;

BLOCO_VAZIO : '{' '}'
            { $$.c = vector<string>{"<{", "}>"}; }
           ;

BLOCO : '{' START_BLOCO CMDs '}' END_BLOCO
        { $$.c = "<{" + $3.c + "}>" ; }
       ;
       
START_BLOCO : {empilha_escopo_novo();}

END_BLOCO : {desempilha_escopo();}
       ;

CMD_RET : RETURN 
          { 
            $$.c = vector<string>{"undefined"};
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                $$.c = $$.c + "}>";
            $$.c =  $$.c + vector<string>{ "'&retorno'", "@", "~" };
          }
      | RETURN E 
          { 
            $$.c = $2.c;
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                $$.c = $$.c + "}>";
            $$.c = $$.c + vector<string>{ "'&retorno'", "@", "~" }; 
          }
        ;

CMD_ASM : EOBJ ASM { $$.c = $1.c + $2.c + "^"; }
        ;

EMPILHA_TS_FUNC : { ts.push_back( map< string, Simbolo >{} ); 
                blocos_alinhados_em_funcao.push_back(0);} 
           ;

DESEMPILHA_TS_FUNC : { ts.pop_back(); 
                      blocos_alinhados_em_funcao.pop_back();} 
                      ;

CMD_FUNC : FUNCTION NOME_FUNCAO '(' EMPILHA_TS_FUNC LISTA_PARAMs ')' '{' CMDs '}' DESEMPILHA_TS_FUNC 
           { 
             string definicao_lbl_endereco_funcao = ":" + $2.endereco_funcao;
             $$.c = $2.c;
             funcoes = funcoes + definicao_lbl_endereco_funcao + $5.c + $8.c +
                         "undefined" + "@" + "'&retorno'" + "@"+ "~";
           }
         ;

NOME_FUNCAO : ID 
            { 
              string endereco_funcao = gera_label( "func_" + $1.c[0] );
              $$.endereco_funcao = endereco_funcao;
              Atributos decl = declara_var( Var, $1 );
              $$.c = decl.c + $1.c + "{}" + "="  + "'&funcao'" + endereco_funcao + "[=]" + "^";
            }
          ;

LISTA_PARAMs : PARAMs
           | PARAMs ','   
           | { $$.clear(); } 
           ;
           
PARAMs : PARAMs ',' PARAM
       { 
         Atributos decl = declara_var(Let, $3);
         string idx = to_string($1.n_args);
         string nome = $3.c[0];

         $$.c = $1.c + $3.c + "&" + $3.c + "arguments" + "@" + idx + "[@]" + "=" + "^" ;

         if ($3.valor_default.size() > 0) {
            string lbl_fim_default = gera_label( "fim_default_if" );
            $$.c += $3.c + "@" + "undefined" + "@" + "==" + "!" + lbl_fim_default + "?" + $3.c +
                     $3.valor_default + "=" + "^" + define_label(lbl_fim_default);
         }

         $$.n_args = $1.n_args + 1; 
       }
     | PARAM
       { 
         Atributos decl = declara_var(Let, $1);
         string nome = $1.c[0];

         $$.c = decl.c + $1.c + "arguments" +  "@" + "0" + "[@]" + "=" + "^";

         if ($1.valor_default.size() > 0) {
          string lbl_fim_default = gera_label( "fim_default_if" );
           $$.c += $1.c + "@" + "undefined" + "@" + "==" + "!" + lbl_fim_default + "?" + $1.c +
                     $1.valor_default + "=" + "^" + define_label(lbl_fim_default);
         }

         $$.n_args = 1; 
       }
     ;

PARAM : ID 
      { $$ = $1; 
        $$.n_args = 1;
        $$.valor_default.clear(); 
        $$.variaveis_declaradas.insert($1.c[0]);
      }
    | ID '=' EOBJ
      { 
        $$ = $1;
        $$.n_args = 1;
        $$.valor_default = $3.c;
        $$.variaveis_declaradas.insert($1.c[0]);
      }
    ;

CMD_FOR : FOR '(' SF ';' E ';' EF ')' CMD 
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          $$.c = $3.c + define_label (teste_for) +
                 $5.c + "!" + fim_for + "?" + $9.c + 
                 $7.c + teste_for + "#" + define_label(fim_for);
        }
        ;

EF : ATRIB
      { $$.c = $1.c + "^"; };
    | { $$.clear(); }
    ;

SF : CMD_LET 
    | CMD_VAR
    | CMD_CONST
    | EF
    ;

CMD_WHILE : WHILE '(' E ')' CMD
        { string inicio_while = gera_label( "inicio_while" );
          string fim_while = gera_label( "fim_while" );
          $$.c = define_label(inicio_while) + 
                 $3.c + "!" + fim_while + "?" + $5.c + 
                 inicio_while + "#" + define_label(fim_while);
        }
        ;

CMD_LET : LET LET_VARs { $$.c = $2.c; }
        ;

LET_VARs : LET_VAR ',' LET_VARs { $$.c = $1.c + $3.c; } 
         | LET_VAR
         ;

LET_VAR : ID  
          { $$.c = declara_var( Let, $1 ).c; }
        | ID '=' EOBJ
          { $$.c = declara_var( Let, $1 ).c + $1.c[0] + $3.c + "=" + "^"; }
        ;
  
CMD_VAR : VAR VAR_VARs { $$.c = $2.c; }
        ;
        
VAR_VARs : VAR_VAR ',' VAR_VARs { $$.c = $1.c + $3.c; } 
         | VAR_VAR
         ;

VAR_VAR : ID  
          { $$.c = declara_var( Var, $1 ).c; }
        | ID '=' EOBJ
          {  $$.c = declara_var( Var, $1 ).c + $1.c[0] + $3.c + "=" + "^"; }
        ;
  
CMD_CONST: CONST CONST_VARs { $$.c = $2.c; }
         ;
  
CONST_VARs : CONST_VAR ',' CONST_VARs { $$.c = $1.c + $3.c; } 
           | CONST_VAR
           ;

CONST_VAR : ID  
            { $$.c = declara_var( Const, $1 ).c; }
          | ID '=' EOBJ
            { $$.c = declara_var( Const, $1 ).c + $1.c[0] + $3.c + "=" + "^"; }
          ;
  
CMD_IF : IF '(' E ')' CMD
          {
            string fim_if = gera_label( "fim_if" );
            $$.c = $3.c + "!" + fim_if + "?" + $5.c + define_label(fim_if);
          }
        | IF '(' E ')' CMD ELSE CMD
          {  
            string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            $$.c = $3.c + "!" + else_if + "?" + $5.c + fim_if + 
            "#" + define_label(else_if) + $7.c +  define_label(fim_if);
          }
       ;

LVALUEPROP : LVALUEPROP '[' EOBJ ']'
            { $$.c.clear(); 
              $$.esq = $1.esq + $1.dir + "[@]";
              $$.dir = $3.c;
            }
           | LVALUEPROP '.' ID  
            { $$.c.clear(); 
              $$.esq = $1.esq + $1.dir + "[@]";
              $$.dir = $3.c;
            }
           | F '[' EOBJ ']'
             { $$.c.clear(); 
              $$.esq = $1.c;
              $$.dir = $3.c;
            }
           | F '.' ID  
            { $$.c.clear(); 
              $$.esq = $1.c;
              $$.dir = $3.c;
            }
           | ID '[' EOBJ ']'
            { $$.c.clear(); 
              $$.esq = $1.c + "@";
              $$.dir = $3.c;
              $$.variaveis_usadas.insert($1.c[0]);
            }
           | ID '.' ID  
            { $$.c.clear(); 
              $$.esq = $1.c + "@";
              $$.dir = $3.c;
              $$.variaveis_usadas.insert($1.c[0]);
            }
           ;

LIST  : '[' LISTVALS ']' 
        { 
          $$.c = vector<string>{"[]"};
          for(int i = 0; i < $2.n_args; i++) {
            $$.c += to_string(i) + $2.c + "[=]";
          }
          $$.c.erase($$.c.end() - 1); // Remove último [=]
        }
      | '[' ']' { $$.c = vector<string>{"[]"}; }
      ;

LISTVALS : LISTVAL ',' LISTVALS   
         { 
           $$.c = $1.c + $3.c; 
           $$.n_args = 1 + $3.n_args;
         }
         | LISTVAL
         {
           $$.c = $1.c;
           $$.n_args = 1;
         }
         ;

LISTVAL : EOBJ
        ;

ATRIB
  : ID '=' ATRIB
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $3.c + "="; }
  | ID MAIS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $1.c + "@" + $3.c + "+" + "="; }
  | ID MENOS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $1.c + "@" + $3.c + "-" + "="; }
  | LVALUEPROP '=' ATRIB
    { $$.c = $1.esq + $1.dir + $3.c + "[=]"; }
  | LVALUEPROP MAIS_IGUAL ATRIB 
    { 
      string esq = gera_temp("esq");
      string dir = gera_temp("dir");
      $$.c =  to_inst(esq) + "&" + esq + $1.esq + "=" + "^" +
              to_inst(dir) + "&" + dir + $1.dir + "=" + "^" +
              esq + "@" + dir + "@" +
              esq + "@" + dir + "@" + "[@]" +
              $3.c + "+" + "[=]";
    }
  | LVALUEPROP MENOS_IGUAL ATRIB 
    { 
      string esq = gera_temp("esq");
      string dir = gera_temp("dir");
      $$.c =  to_inst(esq) + "&" + esq + $1.esq + "=" + "^" +
              to_inst(dir) + "&" + dir + $1.dir + "=" + "^" +
              esq + "@" + dir + "@" +
              esq + "@" + dir + "@" + "[@]" +
              $3.c + "-" + "[=]";
    }
  | FUNCAO_ANONIMA
  | ARROW_FUNCTION
  | E
  ;

FUNCAO_ANONIMA : FUNCTION '(' EMPILHA_TS_FUNC LISTA_PARAMs ')' '{' CMDs '}' DESEMPILHA_TS_FUNC
               {
                 string endereco_funcao = gera_label("func_anonima");
                 
                 // Calcular variáveis capturadas
                 set<string> captura = calcula_captura($7.variaveis_usadas, $4.variaveis_declaradas);
                 
                 // Gerar código de captura
                 vector<string> codigo_captura;
                 if(!captura.empty()) {
                   codigo_captura += "{}" + "'captura'" + "{}";
                   for(const string& var : captura) {
                     codigo_captura += "'" + var + "'" + var + "@" + "[<=]";
                   }
                   codigo_captura += "[<=]";
                 }
                 
                 $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
                 if(!captura.empty()) {
                   $$.c += codigo_captura;
                 }
                 
                 funcoes = funcoes + ":" + endereco_funcao + $4.c + $7.c +
                           "undefined" + "@" + "'&retorno'" + "@" + "~";
               }
               ;

ARROW_FUNCTION : ID SETA EOBJ
               {
                 string endereco_funcao = gera_label("arrow_func");
                 
                 // Calcular captura
                 set<string> usadas = $3.variaveis_usadas;
                 set<string> declaradas;
                 declaradas.insert($1.c[0]);
                 set<string> captura = calcula_captura(usadas, declaradas);
                 
                 // Código de captura
                 vector<string> codigo_captura;
                 if(!captura.empty()) {
                   codigo_captura += "{}" + "'captura'" + "{}";
                   for(const string& var : captura) {
                     codigo_captura += "'" + var + "'" + var + "@" + "[<=]";
                   }
                   codigo_captura += "[<=]";
                 }
                 
                 $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
                 if(!captura.empty()) {
                   $$.c += codigo_captura;
                 }
                 
                 // Corpo da função (com return implícito)
                 funcoes = funcoes + ":" + endereco_funcao + 
                           $1.c + "&" + $1.c + "arguments" + "@" + "0" + "[@]" + "=" + "^" +
                           $3.c + "'&retorno'" + "@" + "~";
               }
             | ID SETA BLOCO
               {
                 string endereco_funcao = gera_label("arrow_func");
                 
                 set<string> usadas = $3.variaveis_usadas;
                 set<string> declaradas;
                 declaradas.insert($1.c[0]);
                 set<string> captura = calcula_captura(usadas, declaradas);
                 
                 vector<string> codigo_captura;
                 if(!captura.empty()) {
                   codigo_captura += "{}" + "'captura'" + "{}";
                   for(const string& var : captura) {
                     codigo_captura += "'" + var + "'" + var + "@" + "[<=]";
                   }
                   codigo_captura += "[<=]";
                 }
                 
                 $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
                 if(!captura.empty()) {
                   $$.c += codigo_captura;
                 }
                 
                 funcoes = funcoes + ":" + endereco_funcao + 
                           $1.c + "&" + $1.c + "arguments" + "@" + "0" + "[@]" + "=" + "^" +
                           $3.c + "undefined" + "@" + "'&retorno'" + "@" + "~";
               }
             | '(' LISTA_PARAMs ')' SETA EOBJ
               {
                 string endereco_funcao = gera_label("arrow_func");
                 
                 set<string> captura = calcula_captura($5.variaveis_usadas, $2.variaveis_declaradas);
                 
                 vector<string> codigo_captura;
                 if(!captura.empty()) {
                   codigo_captura += "{}" + "'captura'" + "{}";
                   for(const string& var : captura) {
                     codigo_captura += "'" + var + "'" + var + "@" + "[<=]";
                   }
                   codigo_captura += "[<=]";
                 }
                 
                 $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
                 if(!captura.empty()) {
                   $$.c += codigo_captura;
                 }
                 
                 funcoes = funcoes + ":" + endereco_funcao + $2.c + 
                           $5.c + "'&retorno'" + "@" + "~";
               }
             | '(' LISTA_PARAMs ')' SETA BLOCO
               {
                 string endereco_funcao = gera_label("arrow_func");
                 
                 set<string> captura = calcula_captura($5.variaveis_usadas, $2.variaveis_declaradas);
                 
                 vector<string> codigo_captura;
                 if(!captura.empty()) {
                   codigo_captura += "{}" + "'captura'" + "{}";
                   for(const string& var : captura) {
                     codigo_captura += "'" + var + "'" + var + "@" + "[<=]";
                   }
                   codigo_captura += "[<=]";
                 }
                 
                 $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
                 if(!captura.empty()) {
                   $$.c += codigo_captura;
                 }
                 
                 funcoes = funcoes + ":" + endereco_funcao + $2.c + 
                           $5.c + "undefined" + "@" + "'&retorno'" + "@" + "~";
               }
             ;

EOBJ : '{' CAMPOS '}'
     { 
       $$.c = vector<string>{"{}"};
       if($2.c.size() > 0) {
         $$.c += $2.c;
       }
     }
     | ECOND
     ;

EVG : ATRIB ',' EVG 
    { $$.c = $1.c + "^" + $3.c; }
    | ATRIB
    { $$.c = $1.c; }
    ;

ECOND : ECOND '?' EOBJ ':' EOBJ
      {
        string lbl_falso = gera_label("ternario_falso");
        string lbl_fim = gera_label("ternario_fim");
        $$.c = $1.c + "!" + lbl_falso + "?" + $3.c + lbl_fim + "#" + 
               ":" + lbl_falso + $5.c + ":" + lbl_fim;
      }
      | E
      ;

CAMPOS : CAMPO ',' CAMPOS
       { $$.c = $1.c + $3.c; }
       | CAMPO
       |
       { $$.clear(); }
       ;

CAMPO : ID ':' EOBJ
      { $$.c = "'" + $1.c[0] + "'" + $3.c + "[<=]"; }
      | ID
      { $$.c = "'" + $1.c[0] + "'" + $1.c + "@" + "[<=]"; }
      ; 

E : ID
    { 
      $$.c = $1.c + "@";
      $$.variaveis_usadas.insert($1.c[0]);
    }
  | LVALUEPROP
    { 
      $$.c = $1.esq + $1.dir + "[@]"; 
      $$.variaveis_usadas = $1.variaveis_usadas;
    }
  | E IGUAL E
    { 
      $$.c = $1.c + $3.c + $2.c;
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | E '<' E
    { 
      $$.c = $1.c + $3.c + $2.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | E '>' E
    { 
      $$.c = $1.c + $3.c + $2.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | E '+' E
    { 
      $$.c = $1.c + $3.c + $2.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | E '-' E
    { 
      $$.c = $1.c + $3.c + $2.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | E '*' E
    { 
      $$.c = $1.c + $3.c + $2.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | E '/' E
    { 
      $$.c = $1.c + $3.c + $2.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | E '%' E
    { 
      $$.c = $1.c + $3.c + $2.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
      $$.variaveis_usadas.insert($3.variaveis_usadas.begin(), $3.variaveis_usadas.end());
    }
  | UN
    { 
      $$.c = $1.c; 
      $$.variaveis_usadas = $1.variaveis_usadas;
    }
  ;

