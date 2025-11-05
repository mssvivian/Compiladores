%{
#include <iostream>
#include <string>
#include <vector>
#include <map>
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


%token ID IF ELSE LET CONST VAR  FOR WHILE FUNCTION RETURN ASM //PRINT
%token CDOUBLE CSTRING CINT CBOOL
%token AND OR ME_IG MA_IG DIF IGUAL
%token MAIS_IGUAL MAIS_MAIS MENOS_IGUAL MENOS_MENOS

%right '=' MAIS_IGUAL
%nonassoc AND OR
%nonassoc IGUAL DIF
%nonassoc '<' '>' ME_IG MA_IG
%left '+' '-'
%left '*' '/' '%'

%right '[' '('
%left '.' 
%left MAIS_MAIS MENOS_MENOS


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
    //| PRINT E ';'
    //  { $$.c = $2.c + "println" + "#"; }
    | CMD_FOR
    | CMD_WHILE
    | CMD_RET ';'
    | CMD_ASM ';'
    | ATRIB ';'
      { $$.c = $1.c + "^"; } 
    | BLOCO
    
    | ';'
      { $$.clear(); }
    ;

BLOCO : '{' START_BLOCO CMDs '}' END_BLOCO
        { //if ($3.c.size() > 0)
            $$.c = "<{" + $3.c + "}>" ; 
          //else
            //$$.c = $3.c;
        }
       ;
       
START_BLOCO : {empilha_escopo_novo();}

END_BLOCO : {desempilha_escopo();}
       ;

CMD_RET : RETURN 
          { 
            $$.c = vector<string>{"undefined"}; /* 1. Empilha undefined */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                $$.c = $$.c + "}>"; /* 2. Fecha blocos aninhados */
            
            $$.c =  $$.c + vector<string>{ "'&retorno'", "@", "~" }; /* 3. Instruções de retorno */
          }
      | RETURN E 
          { 
            $$.c = $2.c; /* 1. Empilha o valor da expressão E */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                $$.c = $$.c + "}>"; /* 2. Fecha blocos aninhados */
            
            /* 3. Instruções de retorno (NÃO adicione $2.c de novo) */
            $$.c = $$.c + vector<string>{ "'&retorno'", "@", "~" }; 
          }
        ;

CMD_ASM : E ASM { $$.c = $1.c + $2.c + "^"; }
        ;

EMPILHA_TS_FUNC : { ts.push_back( map< string, Simbolo >{} ); 
                blocos_alinhados_em_funcao.push_back(0);} 
           ;

DESEMPILHA_TS_FUNC : { ts.pop_back(); 
                      blocos_alinhados_em_funcao.pop_back();} 
                      ;

CMD_FUNC : FUNCTION NOME_FUNCAO '(' EMPILHA_TS_FUNC LISTA_PARAMs ')' '{' CMDs '}' DESEMPILHA_TS_FUNC 
           { 
             /* Note que os índices mudaram: NOME_FUNCAO é $2, LISTA_PARAMs é $5, CMDs é $8 */
             string definicao_lbl_endereco_funcao = ":" + $2.endereco_funcao;
             
             $$.c = $2.c ; /* Código de NOME_FUNCAO (declara var, atribui obj função) */

             funcoes = funcoes + definicao_lbl_endereco_funcao + $5.c + $8.c +
                         "undefined" + "@" + "'&retorno'" + "@"+ "~";
             
           }
         ;
/*    
CMD_FUNC : FUNCTION NOME_FUNCAO '(' EMPILHA_TS LISTA_PARAMs ')' '{' CMDs '}'
           { 
             blocos_alinhados_em_funcao.push_back(0);
             string definicao_lbl_endereco_funcao = ":" + $2.endereco_funcao;
             
             $$.c = $2.c ;

             funcoes = funcoes + definicao_lbl_endereco_funcao + $5.c + $8.c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
             
           }
         ;*/
         
NOME_FUNCAO : ID 
            { 
              string endereco_funcao = gera_label( "func_" + $1.c[0] );
              $$.endereco_funcao = endereco_funcao; /* 1. Salva o label para CMD_FUNC usar */
              
              Atributos decl = declara_var( Var, $1 ); // 2. Declara a variável
              
              /* 3. Gera código usando decl.c (que pode ou não ter '&') e a var local 'endereco_funcao' */
              $$.c = decl.c +                                    /* 'foo &' ou nada se for redeclaração */
                     $1.c + "{}" + "="  + "'&funcao'" + endereco_funcao +  /* 'foo '&funcao' func_foo_1' ... */
                     "[=]" + "^";                              /* ... '[=] ^' (atribui prop) */
            }
          ;

LISTA_PARAMs : PARAMs
           | PARAMs ','   
           | { $$.clear(); } 
           ;
           
PARAMs : PARAMs ',' PARAM
       { 
         Atributos decl = declara_var(Let, $3); // Declara o parâmetro no escopo da função
         
         string idx = to_string($1.n_args); // índice do parâmetro
         string nome = $3.c[0]; // nome da variável

         // Código base: associar argumento à variável
         $$.c = $1.c + $3.c + "&" + $3.c + "arguments" + "@" + idx + "[@]" + "=" + "^" ;

         // Se tiver valor default, gerar código adicional
         if ($3.valor_default.size() > 0) {
            string lbl_fim_default = gera_label( "fim_default_if" );
            $$.c += $3.c + "@" + "undefined" + "@" + "==" + "!" + lbl_fim_default + "?" + $3.c +
                     $3.valor_default + "=" + "^" +
                     define_label(lbl_fim_default);
         }

         $$.n_args = $1.n_args + 1; 
       }
     | PARAM
       { 
         Atributos decl = declara_var(Let, $1); // Declara o parâmetro
         
         string nome = $1.c[0];

         // Código base para o primeiro parâmetro
         $$.c = decl.c + 
                $1.c + "arguments" +  "@" + "0" + "[@]" + "=" + "^";

         if ($1.valor_default.size() > 0) {
          string lbl_fim_default = gera_label( "fim_default_if" );
           $$.c += $1.c + "@" + "undefined" + "@" + "==" + "!" + lbl_fim_default + "?" + $1.c +
                     $1.valor_default + "=" + "^" +
                     define_label(lbl_fim_default);
         }

         $$.n_args = 1; 
       }
     ;

PARAM : ID 
      { $$ = $1; 
        $$.n_args = 1;
        $$.valor_default.clear(); }
    | ID '=' E
      { 
        $$ = $1;
        $$.n_args = 1;
        $$.valor_default = $3.c; }
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
        | ID '=' E
          { $$.c = declara_var( Let, $1 ).c + $1.c[0] + $3.c + "=" + "^"; } // ex: a 1 = ^
     /*   | ID '=' '{' '}'
          {
            $$.c = declara_var( Let, $1 ).c +
            $1.c[0] + "{}" + "=" + "^"; } */
        ;
  
CMD_VAR : VAR VAR_VARs { $$.c = $2.c; }
        ;
        
VAR_VARs : VAR_VAR ',' VAR_VARs { $$.c = $1.c + $3.c; } 
         | VAR_VAR
         ;

VAR_VAR : ID  
          { $$.c = declara_var( Var, $1 ).c; }
        | ID '=' E
          {  $$.c = declara_var( Var, $1 ).c + $1.c[0] + $3.c + "=" + "^"; }
     /*   | ID '=' '{' '}'
          {
            $$.c = declara_var( Var, $1 ).c +
            $1.c[0] + "{}" + "=" + "^"; }*/
        ;
  
CMD_CONST: CONST CONST_VARs { $$.c = $2.c; }
         ;
  
CONST_VARs : CONST_VAR ',' CONST_VARs { $$.c = $1.c + $3.c; } 
           | CONST_VAR
           ;

CONST_VAR : ID  
            { $$.c = declara_var( Const, $1 ).c; }
          | ID '=' E
            { $$.c = declara_var( Const, $1 ).c + $1.c[0] + $3.c + "=" + "^"; }
        /*  | ID '=' '{' '}'
            {
              $$.c = declara_var( Const, $1 ).c +
              $1.c[0] + "{}" + "=" + "^"; } */
          ;
  
CMD_IF : IF '(' E ')' CMD
          {
            string fim_if = gera_label( "fim_if" );
            $$.c = $3.c + "!" + fim_if + "?" + $5.c + define_label(fim_if);
          }

        | IF '(' E ')' CMD ELSE CMD
          {  string fim_if = gera_label( "fim_if" );
            string else_if = gera_label( "else_if" );
            $$.c = $3.c + "!" + else_if + "?" + $5.c + fim_if + 
            "#" + define_label(else_if) + $7.c +  define_label(fim_if);
          }
       ;
        
       
LVALUEPROP : LVALUEPROP '[' E ']'
            { $$.c.clear(); 
              $$.esq = $1.esq + $1.dir + "[@]";
              $$.dir = $3.c;
               } // obj.prop[idx]
           | LVALUEPROP '.' ID  
            { $$.c.clear(); 
              $$.esq = $1.esq + $1.dir + "[@]";
              $$.dir = $3.c;
               } // obj.prop.prop
           | F '[' E ']'
             { $$.c.clear(); 
              $$.esq = $1.c;
              $$.dir = $3.c;
               } // foo()[idx]
           | F '.' ID  
            { $$.c.clear(); 
              $$.esq = $1.c;
              $$.dir = $3.c;
              } // criar temp e atribuir na variavel 
              // foo().prop
           | ID '[' E ']'
            { $$.c.clear(); 
              $$.esq = $1.c + "@";
              $$.dir = $3.c;
              } // var[idx]
           | ID '.' ID  
            { $$.c.clear(); 
              $$.esq = $1.c + "@";
              $$.dir = $3.c;
               } // var.prop
           ;

LIST  : '[' LISTVALS ']' { $$.c = $1.c + $2.c + $3.c; }
      | '[' ']' { $$.c = vector<string>{"[]"}; }
      ;

LISTVALS : LISTVAL ',' LISTVALS   { $$.c = $1.c + $3.c; }
         | LISTVAL
         ;

LISTVAL : E
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
    { $$.c = $1.c + $1.c + "[@]" + $3.c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          $$.c =  to_inst(esq) + "&" + esq + $1.esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + $1.dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  $3.c + "+" + "[=]" ;}
  | LVALUEPROP MENOS_IGUAL ATRIB 
    { $$.c = $1.c + $1.c + "[@]" + $3.c + "+" + "[=]"; 
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          $$.c =  to_inst(esq) + "&" + esq + $1.esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + $1.dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  $3.c + "-" + "[=]" ;}
  | E
  ;
  /*| ID '=' '{' '}'
      { checa_simbolo( $1.c[0], true ); $$.c = $1.c + "{}" + "="; }
  | LVALUEPROP '=' '{' '}'
      { $$.c = $1.c + "{}" + "[=]"; } 
  | E 
  ; */

E : ID
    { $$.c = $1.c + "@"; }
  | LVALUEPROP
    { $$.c = $1.esq + $1.dir + "[@]"; }
  | E IGUAL E
    { $$.c = $1.c + $3.c + $2.c;}
  | E '<' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '>' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '+' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '-' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '*' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '/' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '%' E
    { $$.c = $1.c + $3.c + $2.c; }
  | '-' E 
    { $$.c = "0" + $2.c + "-" ; }
  | '+' E
    { $$.c = "0" + $2.c + "+" ; }
  | UN
    { $$.c = $1.c; }
  ;

UN  : MAIS_MAIS ID 
        {$$.c = $2.c + $2.c + "@" + "1" + "+" + "=";}
    | MENOS_MENOS ID 
        {$$.c = $2.c + $2.c + "@" + "1" + "-" + "=";}
    | ID MAIS_MAIS 
        {$$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "+" + "=" + "^";}
    | ID MENOS_MENOS 
        {$$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "-" + "=" + "^";}
    | MAIS_MAIS LVALUEPROP 
        { string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          $$.c =  to_inst(esq) + "&" + esq + $2.esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + $2.dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "+" + "[=]" ;} 
    | MENOS_MENOS LVALUEPROP 
        {
          string esq = gera_temp("esq");
          string dir = gera_temp("dir");
          $$.c =  to_inst(esq) + "&" + esq + $2.esq + "=" + "^" +
                  to_inst(dir) + "&" + dir + $2.dir + "=" + "^" +
                  esq + "@" + dir + "@" +
                  esq + "@" + dir + "@" + "[@]" +
                  "1" + "-" + "[=]" ;} 
    | LVALUEPROP MAIS_MAIS 
        {
         string esq = gera_temp("esq");
         string dir = gera_temp("dir");
         $$.c = to_inst(esq) + "&" + esq + $1.esq + "=" + "^" +
                to_inst(dir) + "&" + dir + $1.dir + "=" + "^" +
                esq + "@" + dir + "@" + "[@]" +
                esq + "@" + dir + "@" +
                esq + "@" + dir + "@" + "[@]" +
                "1" + "+" + "[=]" + "^";
        }
    | LVALUEPROP MENOS_MENOS 
        {string esq = gera_temp("esq");
         string dir = gera_temp("dir");
         $$.c = to_inst(esq) + "&" + esq + $1.esq + "=" + "^" +
                to_inst(dir) + "&" + dir + $1.dir + "=" + "^" +
                esq + "@" + dir + "@" + "[@]" +
                esq + "@" + dir + "@" +
                esq + "@" + dir + "@" + "[@]" +
                "1" + "-" + "[=]" + "^";}
    | F
    ;

F :   CDOUBLE
    | CINT
    | CSTRING
    | CBOOL
    | '(' E ')'
      { $$.c = $2.c; }
    | LIST
    | '{' '}' 
      { $$.c = vector<string>{"{}"}; }
    | CHAMA_FUNC
  ;

CHAMA_FUNC : ID '(' LISTA_ARGS ')' 
              { 
                $$.c = $3.c + $1.c[0] + "@" + "$" ;
              }
            | LVALUEPROP '(' LISTA_ARGS ')' 
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                $$.c = $3.c + $1.esq + $1.dir + "[@]" + "$" ;
              }
            | '(' E ')' '(' LISTA_ARGS ')' 
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                $$.c = $5.c + $2.c + "$" ;
              }
            ;

LISTA_ARGS : ARGs
           { 
             $$.c = $1.c + to_string( $1.n_args ); 
             $$.n_args = $1.n_args;
           }
         | ARGs ',' 
           {
             $$.c = $1.c + to_string( $1.n_args ); 
             $$.n_args = $1.n_args;
           }
           | 
           { 
              $$.c = vector<string>{"0"}; 
              $$.n_args = 0; 
           }
           ;

ARGs : ATRIB
      { 
        $$.c = $1.c;     
        $$.n_args = 1;   
      } 
    | ARGs ',' ATRIB  
      { 
        $$.c = $1.c + $3.c; 
        $$.n_args = $1.n_args + 1; 
      }
    ;

  
%%

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



