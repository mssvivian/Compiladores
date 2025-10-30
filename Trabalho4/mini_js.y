%{
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
%right MAIS_MAIS 
%left '[' '(' 
%left '.'


%%

S : CMDs { print( resolve_enderecos( $1.c + "." + funcoes ) ); }
  ;

CMDs : CMDs CMD  { $$.c = $1.c + $2.c; };
     |           { $$.clear(); }
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

BLOCO : '{' EMPILHA_TS CMDs '}' 
        { $$.c = "<{" + $3.c + "}>" ; ts.pop_back(); 
          //pilha_funcoes.back()++;
        }
       ;
       
CMD_RET : RETURN E
          { 
            /*int quant_ret = pilha_funcoes.back();
            pilha_funcoes.pop_back();
            vector<string> retornos;
            for( int i = 0; i < quant_ret; i++ )
                retornos.push_back( "}>" ); erro de seg nao é aqui*/
            $$.c = $2.c +  vector<string>{ "'&retorno'", "@", "~" }; 
            
            }
         ;
CMD_ASM : E ASM { $$.c = $1.c + $2.c + "^"; }
        ;

EMPILHA_TS : { ts.push_back( map< string, Simbolo >{} ); } 
           ;
    
CMD_FUNC : FUNCTION NOME_FUNCAO '(' EMPILHA_TS LISTA_PARAMs ')' '{' CMDs '}'
           { 
             //pilha_funcoes.push_back(1);
             string definicao_lbl_endereco_funcao = ":" + $2.endereco_funcao;
             
             $$.c = $2.c ;

             funcoes = funcoes + definicao_lbl_endereco_funcao + $5.c + $8.c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
             
           }
         ;
         
NOME_FUNCAO : ID 
            { string endereco_funcao = gera_label( "func_" + $1.c[0] );
              Atributos decl = declara_var( Var, $1 ); // Declara a função no escopo atual
              $$.c = $1.c + "&" + $1.c + "{}" + "=" + "'&funcao'" + $$.endereco_funcao + "[=]" + "^"; }
          ;

LISTA_PARAMs : PARAMs
           | PARAMs ','   
           | { $$.clear(); } 
           ;
           
PARAMs : PARAMs ',' PARAM  
       { 
         Atributos decl = declara_var( Let, $3 ); // Declara o parâmetro no escopo da função
         $$.c = $1.c + decl.c + 
                $3.c[0] + "arguments" + "@" + to_string( $1.n_args ) 
                + "[@]" + "=" + "^"; 
                
         if( $3.valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         $$.n_args = $1.n_args + $3.n_args; 
       }
     | PARAM 
       { 
         Atributos decl = declara_var( Let, $1 ); // Declara o parâmetro no escopo da função
         $$.c = decl.c + 
                $1.c[0] + "arguments" + "@" + "0" + "[@]" + "=" + "^"; 
                
         if( $1.valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         $$.n_args = $1.n_args; 
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

LET_VAR : LVALUE  
          { $$.c = declara_var( Let, $1 ).c; }
        | LVALUE '=' E
          { 
            $$.c = declara_var( Let, $1 ).c + // ex: a &
                   $1.c[0] + $3.c + "=" + "^"; } // ex: a 1 = ^
        | LVALUE '=' '{' '}'
          {
            $$.c = declara_var( Let, $1 ).c +
            $1.c[0] + "{}" + "=" + "^"; }
        ;
  
CMD_VAR : VAR VAR_VARs { $$.c = $2.c; }
        ;
        
VAR_VARs : VAR_VAR ',' VAR_VARs { $$.c = $1.c + $3.c; } 
         | VAR_VAR
         ;

VAR_VAR : LVALUE  
          { $$.c = declara_var( Var, $1 ).c; }
        | LVALUE '=' E
          {  $$.c = declara_var( Var, $1 ).c + 
                    $1.c[0] + $3.c + "=" + "^"; }
        | LVALUE '=' '{' '}'
          {
            $$.c = declara_var( Var, $1 ).c +
            $1.c[0] + "{}" + "=" + "^"; }
        ;
  
CMD_CONST: CONST CONST_VARs { $$.c = $2.c; }
         ;
  
CONST_VARs : CONST_VAR ',' CONST_VARs { $$.c = $1.c + $3.c; } 
           | CONST_VAR
           ;

CONST_VAR : LVALUE  
            { $$.c = declara_var( Const, $1 ).c; }
          | LVALUE '=' E
            { $$.c = declara_var( Const, $1 ).c + 
                     $1.c[0] + $3.c + "=" + "^"; }
          | LVALUE '=' '{' '}'
            {
              $$.c = declara_var( Const, $1 ).c +
              $1.c[0] + "{}" + "=" + "^"; }
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
       
LVALUEPROP : LVALUEPROP '[' E ']'
            { $$.c = $1.c + "[@]" + $3.c; } // obj.prop[idx]
           | LVALUEPROP '.' ID  
            { $$.c = $1.c + "[@]"  + $3.c[0] ; } // obj.prop.prop
           | F '[' E ']'
            { $$.c = $1.c + $3.c; } // foo()[idx]
           | F '.' ID  
            { $$.c = $1.c  + $3.c[0] ; } // foo().prop
           | LVALUE '[' E ']'
            { $$.c = $1.c + "@" + $3.c; } // var[idx]
           | LVALUE '.' ID  
            { $$.c = $1.c + "@"  + $3.c[0] ; } // var.prop
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
  : LVALUE '=' ATRIB
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $3.c + "="; }
  | LVALUE MAIS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $1.c + "@" + $3.c + "+" + "="; }
  | LVALUE MENOS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $1.c + "@" + $3.c + "-" + "="; }
  | LVALUEPROP '=' ATRIB
    { $$.c = $1.c + $3.c + "[=]"; }
  | LVALUEPROP MAIS_IGUAL ATRIB 
    { $$.c = $1.c + $1.c + "[@]" + $3.c + "+" + "[=]"; }
  | LVALUE '=' '{' '}'
      { checa_simbolo( $1.c[0], true ); $$.c = $1.c + "{}" + "="; }
  | LVALUEPROP '=' '{' '}'
      { $$.c = $1.c + "{}" + "[=]"; } 
  | E 
  ;

E : LVALUEPROP
    { $$.c = $1.c + "[@]"; }
  | LVALUE
      { checa_simbolo( $1.c[0], false ); $$.c = $1.c + "@"; }
  | E IGUAL E
    { $$.c = $1.c + $3.c + $2.c; }
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

UN :  MAIS_MAIS LVALUE 
    {$$.c = $2.c + $2.c + "@" + "1" + "+" + "=";}
    | MENOS_MENOS LVALUE 
        {$$.c = $2.c + $2.c + "@" + "1" + "-" + "=";}
    | LVALUE MAIS_MAIS 
        {$$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "+" + "=" + "^";}
    | LVALUE MENOS_MENOS 
        {$$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "-" + "=" + "^";}
    | MAIS_MAIS LVALUEPROP 
        {$$.c = $2.c + $2.c + "[@]" + "1" + "+" + "[=]";}
    | MENOS_MENOS LVALUEPROP 
        {$$.c = $2.c + $2.c + "[@]" + "1" + "-" + "[=]";} 
    | LVALUEPROP MAIS_MAIS 
        {$$.c = $1.c + "[@]" + $1.c + $1.c + "[@]" + "1" + "+" + "[=]" + "^";}
    | LVALUEPROP MENOS_MENOS 
        {$$.c = $1.c + "[@]" + $1.c + $1.c + "[@]" + "1" + "-" + "[=]" + "^";}
    | F
    ;

F :   CDOUBLE
    | CINT
    | CSTRING
    | CBOOL
    | '(' E ')'
      { $$.c = $2.c; }
    | LIST
    | '(' '{' '}' ')'
      { $$.c = vector<string>{"{}"}; }
    | CHAMA_FUNC
  ;

CHAMA_FUNC : ID '(' LISTA_ARGS ')' 
              { 
                $$.c = $3.c + // 1. Empilha args e contagem
                       $1.c[0] + "@" + // 2. Empilha objeto da função (lendo da var)
                  
                       "$"  // 6. CHAMA (a instrução '$')
                       ;
              }
            | LVALUEPROP '(' LISTA_ARGS ')' 
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                $$.c = $3.c + // 1. Empilha args e contagem
                       $1.c + "[@]" + // 2. Empilha objeto da função (LVALUEPROP + [@])
                        "$"  // 6. CHAMA (a instrução '$')
                        ;
              }
    
    | '(' E ')' '(' LISTA_ARGS ')' 
            { 
              string lbl_retorno = gera_label( "retorno_funcao" );
              $$.c = $5.c + 
                   $2.c + 
                    //"@" +
                   "$"  // 6. CHAMA (a instrução '$') + "^";
                    ;

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

ARGs : E
      { 
        $$.c = $1.c;     
        $$.n_args = 1;   
      } 
    | ARGs ',' E  
      { 
        $$.c = $1.c + $3.c; 
        $$.n_args = $1.n_args + 1; 
      }
    ;

/* Esta regra não será mais usada */
ARG : E
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



