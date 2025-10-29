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
%}

/* %define removed for POSIX yacc compatibility; using #define YYSTYPE Atributos */

%token ID IF ELSE LET CONST VAR PRINT FOR WHILE FUNCTION RETURN ASM
%token CDOUBLE CSTRING CINT
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
           

CMD : E ASM ';'  { $$.c = $1.c + $2.c; }
    | CMD_LET ';'
    | CMD_VAR ';'
    | CMD_CONST ';'
    | CMD_IF
    | CMD_FUNC
    | PRINT E ';'
      { $$.c = $2.c + "println" + "#"; }
    // --- CORREÇÃO 5: 'RETURN' deve ATRIBUIR para '&retorno' com '=' ---
    // --- CORREÇÃO 7: Removidas aspas de '&retorno' ---
    | RETURN ';'
      { $$.c = vector<string>{ "undefined", "&retorno", "=", "~" }; }
    | RETURN E ';'
      { $$.c = $2.c + vector<string>{ "&retorno", "=", "~" }; }
    | CMD_FOR
    | CMD_WHILE
    | ATRIB ';'
      { $$.c = $1.c + "^"; } 
    // --- CORREÇÃO 1: Blocos '{...}' agora criam/destroem escopos ---
    | '{' EMPILHA_TS CMDs '}' 
      { $$.c = $3.c; ts.pop_back(); }
    | ';'
      { $$.clear(); }
    ;

EMPILHA_TS : { ts.push_back( map< string, Simbolo >{} ); } 
           ;
    
// --- CORREÇÃO 1: CMD_FUNC agora usa CMDs para permitir corpo vazio ---
CMD_FUNC : FUNCTION NOME_FUNCAO { /*declara_var( Let, $2 );*/ in_func++; } 
             '(' EMPILHA_TS LISTA_PARAMs ')' '{' CMDs '}'
           { 
             string definicao_lbl_endereco_funcao = ":" + $2.endereco_funcao;
             
             $$.c = $2.c ;

             funcoes = funcoes + definicao_lbl_endereco_funcao + $6.c + $9.c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
             in_func--;
           }
         ;
         
NOME_FUNCAO : ID 
            { string endereco_funcao = gera_label( "func_" + $1.c[0] );
              $$.c = $1.c + "&" + $1.c + "{}" + "=" + "'&funcao'" + $$.endereco_funcao + "[=]" + "^"; }
          ;

LISTA_PARAMs : PARAMs
           | { $$.clear(); }
           ;
           
// --- CORREÇÃO 3: 'PARAMs' agora chama 'declara_var' para cada parâmetro ---
PARAMs : PARAMs ',' PARAM  
       { 
         Atributos decl = declara_var( Let, $3 ); // Declara o parâmetro no escopo da função
         $$.c = $1.c + decl.c + // Código de alocação (ex: msg &)
                $3.c[0] + "arguments" + "@" + to_string( $1.n_args ) // Nome da var + ...
                + "[@]" + "=" + "^"; 
                
         if( $3.valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         $$.n_args = $1.n_args + $3.n_args; 
       }
     | PARAM 
       { 
         Atributos decl = declara_var( Let, $1 ); // Declara o parâmetro no escopo da função
         $$.c = decl.c + // Código de alocação (ex: msg &)
                $1.c[0] + "arguments" + "@" + "0" + "[@]" + "=" + "^"; // Nome da var + ...
                
         if( $1.valor_default.size() > 0 ) {
           // Gerar código para testar valor default.
         }
         $$.n_args = $1.n_args; 
       }
     ;
     
// --- CORREÇÃO 3: 'PARAM' agora só passa atributos, 'PARAMs' faz a declaração ---
PARAM : ID 
      { $$ = $1; // Passa os atributos do ID (linha, coluna, nome)
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
       
// --- CORREÇÃO 6: Usando a regra LVALUEPROP exata que você pediu ---
// --- E corrigindo para usar .c[0] para nomes de ID ---
LVALUEPROP : LVALUEPROP '[' E ']'
            { $$.c = $1.c + "[@]" + $3.c; } // obj.prop[idx]
           | LVALUEPROP '.' ID  
            { $$.c = $1.c + "[@]"  + $3.c[0] ; } // obj.prop.prop
           | F '[' E ']'
            { $$.c = $1.c + $3.c; } // foo()[idx]
           | F '.' ID  
            { $$.c = $1.c  + $3.c[0] ; } // foo().prop
           | LVALUE '[' E ']'
            // --- CORREÇÃO C++: Mudado $1.c[0] para $1.c para iniciar com vector ---
            { $$.c = $1.c + "@" + $3.c; } // var[idx]
           | LVALUE '.' ID  
            // --- CORREÇÃO C++: Mudado $1.c[0] para $1.c para iniciar com vector ---
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
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c[0] + $3.c + "="; }
  | LVALUE MAIS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c[0] + $1.c[0] + "@" + $3.c + "+" + "="; }
  | LVALUE MENOS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c[0] + $1.c[0] + "@" + $3.c + "-" + "="; }
  | LVALUEPROP '=' ATRIB
    { $$.c = $1.c + $3.c + "[=]"; }
  | LVALUEPROP MAIS_IGUAL ATRIB 
    { $$.c = $1.c + $1.c + "[@]" + $3.c + "+" + "[=]"; }
  | LVALUE '=' '{' '}'
      // --- CORREÇÃO C++: Mudado $1.c[0] para $1.c para iniciar com vector ---
      { checa_simbolo( $1.c[0], true ); $$.c = $1.c + "{}" + "="; }
  | LVALUEPROP '=' '{' '}'
      { $$.c = $1.c + "{}" + "[=]"; } // Não precisa checar LVALUEPROP
  | E 
  ;

E : LVALUEPROP
    { $$.c = $1.c + "[@]"; }
  | LVALUE
      // --- CORREÇÃO C++: Mudado $1.c[0] para $1.c para iniciar com vector ---
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
    // --- CORREÇÃO C++: Mudado $2.c[0] para $2.c para iniciar com vector ---
    {$$.c = $2.c + $2.c[0] + "@" + "1" + "+" + "=";}
    | MENOS_MENOS LVALUE 
        // --- CORREÇÃO C++: Mudado $2.c[0] para $2.c para iniciar com vector ---
        {$$.c = $2.c + $2.c[0] + "@" + "1" + "-" + "=";}
    | LVALUE MAIS_MAIS 
        // --- CORREÇÃO C++: Mudado $1.c[0] para $1.c para iniciar com vector ---
        {$$.c = $1.c + "@" + $1.c[0] + $1.c[0] + "@" + "1" + "+" + "=" + "^";}
    | LVALUE MENOS_MENOS 
        // --- CORREÇÃO C++: Mudado $1.c[0] para $1.c para iniciar com vector ---
        {$$.c = $1.c + "@" + $1.c[0] + $1.c[0] + "@" + "1" + "-" + "=" + "^";}
    | MAIS_MAIS LVALUEPROP 
        {$$.c = $2.c + $2.c + "[@]" + "1" + "+" + "[=]";}
    | MENOS_MENOS LVALUEPROP 
        {$$.c = $2.c + $2.c + "[@]" + "1" + "-" + "[=]";} // Corrigido de '+' para '-'
    | LVALUEPROP MAIS_MAIS 
        {$$.c = $1.c + "[@]" + $1.c + $1.c + "[@]" + "1" + "+" + "[=]" + "^";}
    // --- CORREÇÃO 8: Removido o 's' solto daqui ---
    | LVALUEPROP MENOS_MENOS 
        {$$.c = $1.c + "[@]" + $1.c + $1.c + "[@]" + "1" + "-" + "[=]" + "^";}
    | F
    ;

F :   CDOUBLE
    | CINT
    | CSTRING
    | '(' E ')'
      { $$.c = $2.c; }
    | LIST
    | '(' '{' '}' ')'
      { $$.c = vector<string>{"{}"}; }
    | CHAMA_FUNC
  ;

// --- CORREÇÃO 4: Lógica de 'CHAMA_FUNC' totalmente refeita ---
// --- CORREÇÃO 7: Removidas aspas de 'arguments', 'this', '&funcao', '&retorno' ---
CHAMA_FUNC : ID '(' LISTA_ARGS ')' 
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                $$.c = $3.c + // 1. Empilha array de argumentos
                       "arguments" + "&" + "=" + "^" + // 2. Salva em 'arguments'
                       $1.c[0] + "@" + // 3. Empilha objeto da função (lendo da var)
                       "dup" + "this" + "&" + "=" + "^" + // 4. Salva em 'this'
                       "&funcao" + "[@]" + // 5. Pega o label do código
                       lbl_retorno + "[&]" + // 6. Empilha endereço de retorno
                       "~" + // 7. CHAMA
                       define_label( lbl_retorno ) + // 8. Ponto de retorno
                       "&retorno" + "@"; // 9. Empilha valor de retorno
              }
            | LVALUEPROP '(' LISTA_ARGS ')' 
              { 
                string lbl_retorno = gera_label( "retorno_funcao" );
                $$.c = $3.c + // 1. Empilha array de argumentos
                       "arguments" + "&" + "=" + "^" + // 2. Salva em 'arguments'
                       $1.c + "[@]" + // 3. Empilha objeto da função (LVALUEPROP + [@])
                       "dup" + "this" + "&" + "=" + "^" + // 4. Salva em 'this'
                      	"&funcao" + "[@]" + // 5. Pega o label do código
                      	lbl_retorno + "[&]" + // 6. Empilha endereço de retorno
                      	"~" + // 7. CHAMA
                      	define_label( lbl_retorno ) + // 8. Ponto de retorno
                      	"&retorno" + "@"; // 9. Empilha valor de retorno
              }
      	  | '(' E ')' '(' LISTA_ARGS ')' 
      	    { 
      	      string lbl_retorno = gera_label( "retorno_funcao" );
      	      $$.c = $5.c + // 1. Empilha array de argumentos
      	       	 	 "arguments" + "&" + "=" + "^" + // 2. Salva em 'arguments'
      	       	 	 $2.c + // 3. Empilha objeto da função (resultado de E)
      	       	 	 "dup" + "this" + "&" + "=" + "^" + // 4. Salva em 'this'
      	       	 	 "&funcao" + "[@]" + // 5. Pega o label do código
      	       	 	 lbl_retorno + "[&]" + // 6. Empilha endereço de retorno
      	       	 	 "~" + // 7. CHAMA
      	       	 	 define_label( lbl_retorno ) + // 8. Ponto de retorno
  	       	 	 	 "&retorno" + "@"; // 9. Empilha valor de retorno
      	    }
      	  ;

// --- CORREÇÃO 4: 'LISTA_ARGS' e 'ARGs' agora constroem um array ---
LISTA_ARGS : ARGs
           | { $$.c = vector<string>{"[]"}; } // Lista de args vazia -> array vazio
           ;

ARGs : E
      { $$.c = vector<string>{"[]"} + $1.c + "append"; } // Primeiro arg
  	| ARGs ',' E  
  	  { $$.c = $1.c + $3.c + "append"; } // Args subsequentes
  	;

ARG : E // Esta regra não é mais usada por LISTA_ARGS
    ;
  
%%

#include "lex.yy.c"

// --- CORREÇÃO 2: Implementação de 'busca_simbolo' ---
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
      
  string nome_var = atrib.c[0];
  string mensagem_erro;

  if (tipo == Var){
    if( ts.back().count(nome_var) > 0 && ts.back()[nome_var].tipo != Var){
      yyerror("Variavel já declarada com var ou let");
    }
    // 'var' não gera código de alocação por si só, mas deve ser
    // registrada na tabela de símbolos (escopo atual) para que
    // futuras referências encontrem a declaração.
    ts.back()[nome_var].linha = atrib.linha;
    ts.back()[nome_var].coluna = atrib.coluna;
    ts.back()[nome_var].tipo = tipo;
    atrib.c.clear(); // mantém comportamento: não emite '&' para var
  } 
  else if (ts.back().count(nome_var) > 0){ // let/const checa só escopo atual
    cerr << "Erro: a variável '" << nome_var << "' ja foi declarada na linha " << to_string(ts.back()[nome_var].linha) << "." << endl;
    exit(1);
  }
  else{
    ts.back()[nome_var].linha = atrib.linha;
    ts.back()[nome_var].coluna = atrib.coluna;
    ts.back()[nome_var].tipo = tipo;
    atrib.c = atrib.c + "&"; // Gera código de alocação (ex: 'a &')
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
  // --- CORREÇÃO C++: Removido o 'A' que causava erro de compilação ---
  cerr << st << endl; 
   cerr << "Proximo a: " << yytext << endl;
   exit( 1 );
}

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}



