%{
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int linha = 1, coluna = 0; 

struct Atributos {
  vector<string> c; // Código

  int linha = 0, coluna = 0;

  void clear() {
    c.clear();
    linha = 0;
    coluna = 0;
  }
};

enum TipoDecl { Let = 1, Const, Var };

struct Simbolo {
  TipoDecl tipo;
  int linha;
  int coluna;
};

map< string, Simbolo > ts; // Tabela de símbolos

Atributos declara_var( TipoDecl tipo, Atributos atrib );

void checa_simbolo( string nome, bool modificavel );

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
%}

%token ID IF ELSE LET CONST VAR PRINT FOR WHILE
%token CDOUBLE CSTRING CINT
%token AND OR ME_IG MA_IG DIF IGUAL
%token MAIS_IGUAL MAIS_MAIS

%right '=' MAIS_IGUAL
%nonassoc AND OR
%nonassoc IGUAL DIF
%nonassoc '<' '>' ME_IG MA_IG
%left '+' '-'
%left '*' '/' '%'
%right MAIS_MAIS
%nonassoc '[' '('
%left '.'


%%

S : CMDs { print( resolve_enderecos( $1.c + "." ) ); }
  |      { /* Programa Vazio */ }
  ;

CMDs: CMDs CMD { $$.c = $1.c + $2.c; }
    | CMD
    ;
     
CMD : CMD_LET ';'
    | CMD_VAR ';'
    | CMD_CONST ';'
    | CMD_IF
    | PRINT E ';' 
      { $$.c = $2.c + "println" + "#"; }
    | CMD_FOR
    | E ';'
      { $$.c = $1.c + "^"; };
    | '{' CMDs '}'  
      { $$.c = $2.c; }
    ;
 
CMD_FOR : FOR '(' SF ';' E ';' EF ')' CMD 
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          $$.c = $3.c + define_label (teste_for) +
                 $5.c + "!" + fim_for + "?" + $9.c + 
                 $7.c + teste_for + "#";
        }
        //| FOR '(' SF '; ; )' CMD {\\aqui tem que gerar um loop infinito}
        ;
EF : E
      { $$.c = $1.c + "^"; };
    | { $$.clear(); }
    ;

SF : CMD_LET 
       | CMD_VAR
       | CMD_CONST
       | EF
       ;
  

CMD_LET : LET LET_VARs { $$.c = $2.c; }
        ;

LET_VARs : LET_VAR ',' LET_VARs { $$.c = $1.c + $3.c; } 
         | LET_VAR
         ;

LET_VAR : ID  
          { $$.c = declara_var( Let, $1 ).c; }
        | ID '=' E
          { 
            $$.c = declara_var( Let, $1 ).c + 
                   $1.c + $3.c + "=" + "^"; }
        ;
  
CMD_VAR : VAR VAR_VARs { $$.c = $2.c; }
        ;
        
VAR_VARs : VAR_VAR ',' VAR_VARs { $$.c = $1.c + $3.c; } 
         | VAR_VAR
         ;

VAR_VAR : ID  
          { $$.c = declara_var( Var, $1 ).c; }
        | ID '=' E
          {  $$.c = declara_var( Var, $1 ).c + 
                    $1.c + $3.c + "=" + "^"; }
        ;
  
CMD_CONST: CONST CONST_VARs { $$.c = $2.c; }
         ;
  
CONST_VARs : CONST_VAR ',' CONST_VARs { $$.c = $1.c + $3.c; } 
           | CONST_VAR
           ;

CONST_VAR : ID '=' E
            { $$.c = declara_var( Const, $1 ).c + 
                     $1.c + $3.c + "=" + "^"; }
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
        
LVALUE : ID 
       ;
       
LVALUEPROP : E '[' E ']'
            { $$.c = $1.c + $3.c; }
            | E '.' ID  
            { $$.c = $1.c + $3.c; }
           ;

E : LVALUE '=' '{' '}'
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + "{}" + "="; }
  | LVALUE '=' E 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $3.c + "="; }
  | LVALUEPROP '=' E 	
    { $$.c = $1.c + $3.c + "[=]"; }
  
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
  | F 
  ;

F : CDOUBLE
    | CINT
    | CSTRING
    | LVALUE
    { checa_simbolo( $1.c[0], false ); $$.c = $1.c + "@"; }
    | LVALUEPROP
    { $$.c = $1.c + "[@]"; }
    | '(' E ')'
      { $$.c = $2.c; }
    | '(' '{' '}' ')'
      { $$.c = vector<string>{"{}"}; }
    | LVALUE MAIS_MAIS {$$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "+" + "=" + "^";}

  
%%

#include "lex.yy.c"

Atributos declara_var( TipoDecl tipo, Atributos atrib ) {
      
  string nome_var = atrib.c[0];

  if (tipo == Var){
    if( ts.count(nome_var) > 0 && ts[nome_var].tipo != Var){
      yyerror("Variavel já declarada com var ou let");
    }
    atrib.c.clear();
  } 
  else if (ts.count(nome_var) > 0){
    yyerror("Variavel já declarada");
  }
  else{
    ts[nome_var].linha = atrib.linha;
    ts[nome_var].coluna = atrib.coluna;
    ts[nome_var].tipo = tipo;
    atrib.c = atrib.c + "&";
  }
  return atrib;
}

void checa_simbolo( string nome, bool modificavel ) {
  if( ts.count( nome ) > 0 ) {
    if( modificavel && ts[nome].tipo == Const ) {
      cerr << "Variavel '" << nome << "' não pode ser modificada." << endl;
      exit( 1 );     
    }
  }
  else {
    cerr << "Variavel '" << nome << "' não declarada." << endl;
    exit( 1 );     
  }
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