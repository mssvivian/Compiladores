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
%}


%token ID IF ELSE LET CONST VAR FOR WHILE FUNCTION RETURN 
%token CDOUBLE CSTRING CINT CBOOL
%token AND OR ME_IG MA_IG DIF IGUAL
%token MAIS_IGUAL MENOS_IGUAL MAIS_MAIS MENOS_MENOS SETA
%token ASM
%token FECHA_PARENTESES_LAMBDA


%left ','
%nonassoc MAIS_IGUAL MENOS_IGUAL

%right '='  SETA':'
%left OR
%left AND
%nonassoc IGUAL DIF
%nonassoc '<' '>' ME_IG MA_IG
%left '+' '-'
%left '*' '/' '%'
%right MAIS_MAIS MENOS_MENOS
%right '[' '('
%left '.'


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
            if(blocos_alinhados_em_funcao.size() == 0) {
              cerr << "Erro: Não é permitido 'return' fora de funções." << endl;
              exit(1);
            }
            $$.c = vector<string>{"undefined"}; /* 1. Empilha undefined */
            
            for( int i = 0; i <  blocos_alinhados_em_funcao.back(); i++ )
                $$.c = $$.c + "}>"; /* 2. Fecha blocos aninhados */
            
            $$.c =  $$.c + vector<string>{ "'&retorno'", "@", "~" }; /* 3. Instruções de retorno */
          }
      | RETURN E 
          { 
            if(blocos_alinhados_em_funcao.size() == 0) {
              cerr << "Erro: Não é permitido 'return' fora de funções." << endl;
              exit(1);
            }
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

CMD_FUNC : FUNCTION NOME_FUNCAO '(' EMPILHA_TS_FUNC LISTA_PARAMS_OPT ')' 
           {
              // Mid-rule action: Declare parameters
              vector<string> codigo_params;
              for(size_t i=0; i<$5.lista_exprs.size(); i++) {
                 Atributos& p = $5.lista_exprs[i];
                 if (p.nome_var.empty()) {
                    cerr << "Erro: Parametro invalido na funcao " << $2.c[0] << endl;
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
              $$.c = codigo_params; // Pass generated code to next part
           }
           '{' CMDs '}' DESEMPILHA_TS_FUNC 
           { 
             string definicao_lbl_endereco_funcao = ":" + $2.endereco_funcao;
             
             // $7 contains the code generated by the mid-rule action
             vector<string> codigo_params = $7.c; 

             $$.c = $2.c ; 

             funcoes = funcoes + definicao_lbl_endereco_funcao + codigo_params + $9.c +
                         "undefined" + "@" + "'&retorno'" + "@"+ "~";
             
           }
         ;

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

CMD_FOR : FOR '(' SF ';' ECOND_OPT ';' EF ')' CMD 
        { string teste_for = gera_label( "teste_for" );
          string fim_for = gera_label( "fim_for" );
          
          $$.c = $3.c + define_label (teste_for) +
                 $5.c + "!" + fim_for + "?" + $9.c + 
                 $7.c + teste_for + "#" + define_label(fim_for);
        }
        ;

ECOND_OPT : E
          | { $$.c = vector<string>{"1"}; }
          ;

EF : ATRIB
      { $$.c = $1.c + "^"; }
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
        | ID '=' E
          {  $$.c = declara_var( Var, $1 ).c + $1.c[0] + $3.c + "=" + "^"; }
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
        
       
LVALUEPROP : LVALUEPROP '[' EOBJ ']'
            { $$.c.clear(); 
              $$.esq = $1.esq + $1.dir + "[@]";
              $$.dir = $3.c;
               } // obj.prop[idx]
           | LVALUEPROP '.' ID  
            { $$.c.clear(); 
              $$.esq = $1.esq + $1.dir + "[@]";
              $$.dir = $3.c;
               } // obj.prop.prop
           | F '[' EOBJ ']'
             { $$.c.clear(); 
              $$.esq = $1.c;
              $$.dir = $3.c;
               } // foo()[idx]
           | F '.' ID  //F1 . ID
            { $$.c.clear(); 
              $$.esq = $1.c;
              $$.dir = $3.c;
              } 
           ;

LIST  : '[' LISTVALS ']' 
        { 
          $$.c = vector<string>{"[]"};
          $$.c += $2.c;
        }
      | '[' ']' { $$.c = vector<string>{"[]"}; }
      ;

LISTVALS : LISTVALS ',' LISTVAL
         { 
           $$.c = $1.c + to_string($1.n_args) + $3.c + "[<=]"; 
           $$.n_args = $1.n_args + 1;
         }
         | LISTVAL
         {
           $$.c = vector<string>{"0"} + $1.c + "[<=]";
           $$.n_args = 1;
         }
         ;

LISTVAL : EOBJ
        { 
          $$.c = $1.c;
          $$.n_args = 0; 
        }
        ;

ATRIB : ATRIB_NO_OBJ
      | OBJ_LITERAL
      ;

ATRIB_NO_OBJ
  : ID '=' ATRIB
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $3.c + "="; 
      $$.nome_var = $1.c[0]; $$.valor_default = $3.c; $$.eh_atribuicao = true; }
  | ID MAIS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $1.c + "@" + $3.c + "+" + "="; $$.eh_atribuicao = true; $$.nome_var = $1.c[0]; }
  | ID MENOS_IGUAL ATRIB 
    { checa_simbolo( $1.c[0], true ); $$.c = $1.c + $1.c + "@" + $3.c + "-" + "="; $$.eh_atribuicao = true; $$.nome_var = $1.c[0]; }
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
  | ID SETA 
    {
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      Atributos decl = declara_var(Let, $1);
      
      string endereco_funcao = gera_label( "func_arrow_" + $1.c[0] );
      
      // Code to init param
      vector<string> codigo_params;
      codigo_params += decl.c;
      codigo_params += $1.c;
      codigo_params += "arguments";
      codigo_params += "@";
      codigo_params += "0";
      codigo_params += "[@]";
      codigo_params += "=";
      codigo_params += "^";
      
      $$.c = codigo_params;
      $$.endereco_funcao = endereco_funcao;
    }
    BLOCO
    {
      string endereco_funcao = $3.endereco_funcao;
      vector<string> codigo_params = $3.c;
      
      $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      funcoes = funcoes + define_label(endereco_funcao) + codigo_params + $4.c +
                "undefined" + "'&retorno'" + "@" + "~";
      
      ts.pop_back(); 
      blocos_alinhados_em_funcao.pop_back();
    }
  | ID SETA 
    {
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      Atributos decl = declara_var(Let, $1);
      
      string endereco_funcao = gera_label( "func_arrow_" + $1.c[0] );
      
      // Code to init param
      vector<string> codigo_params;
      codigo_params += decl.c;
      codigo_params += $1.c;
      codigo_params += "arguments";
      codigo_params += "@";
      codigo_params += "0";
      codigo_params += "[@]";
      codigo_params += "=";
      codigo_params += "^";
      
      $$.c = codigo_params;
      $$.endereco_funcao = endereco_funcao;
    }
    ATRIB_NO_OBJ
    {
      string endereco_funcao = $3.endereco_funcao;
      vector<string> codigo_params = $3.c;
      
      $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
      funcoes = funcoes + define_label(endereco_funcao) + codigo_params + $4.c +
                "'&retorno'" + "@" + "~";
      
      ts.pop_back(); 
      blocos_alinhados_em_funcao.pop_back();
    }
  | '(' LISTA_PARAMS_OPT FECHA_PARENTESES_LAMBDA SETA 
    {
      string endereco_funcao = gera_label( "func_arrow" );
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      vector<string> codigo_params;
      for(size_t i=0; i<$2.lista_exprs.size(); i++) {
         Atributos& p = $2.lista_exprs[i];
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
      $$.c = codigo_params;
      $$.endereco_funcao = endereco_funcao;
    }
    ATRIB_NO_OBJ
    {
       string endereco_funcao = $5.endereco_funcao;
       vector<string> codigo_params = $5.c;

       $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
       funcoes = funcoes + define_label(endereco_funcao) + codigo_params + $6.c +
                 "'&retorno'" + "@" + "~";
       ts.pop_back(); 
       blocos_alinhados_em_funcao.pop_back();
    }
  | '(' LISTA_PARAMS_OPT FECHA_PARENTESES_LAMBDA SETA 
    {
      string endereco_funcao = gera_label( "func_arrow" );
      ts.push_back( map< string, Simbolo >{} ); 
      blocos_alinhados_em_funcao.push_back(0);
      
      vector<string> codigo_params;
      for(size_t i=0; i<$2.lista_exprs.size(); i++) {
         Atributos& p = $2.lista_exprs[i];
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
      $$.c = codigo_params;
      $$.endereco_funcao = endereco_funcao;
    }
    BLOCO
    {
       string endereco_funcao = $5.endereco_funcao;
       vector<string> codigo_params = $5.c;

       $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
       funcoes = funcoes + define_label(endereco_funcao) + codigo_params + $6.c +
                 "undefined" + "'&retorno'" + "@" + "~";
       ts.pop_back(); 
       blocos_alinhados_em_funcao.pop_back();
    }
  | E
    { $$.c = $1.c; $$.nome_var = $1.nome_var; }
  ;
  /*| ID '=' '{' '}'
      { checa_simbolo( $1.c[0], true ); $$.c = $1.c + "{}" + "="; }
  | LVALUEPROP '=' '{' '}'
      { $$.c = $1.c + "{}" + "[=]"; } 
  | E 
  ; */

EOBJ : ECOND
     ;

OBJ_LITERAL : '{' CAMPOS '}'
    { 
      $$.c = vector<string>{"{}"};
      if($2.c.size() > 0) {
        $$.c += $2.c;
      }
    }
     | '{'  '}'
    { $$.c = vector<string>{"{}"}; }
     ;

PARAM : ID 
      { $$.nome_var = $1.c[0]; $$.c = $1.c; }
      | ID '=' EOBJ 
      { $$.nome_var = $1.c[0]; $$.valor_default = $3.c; $$.c = $1.c; }
      ;

LISTA_PARAMS : PARAM
             { $$.lista_exprs = {$1}; }
             | PARAM ',' LISTA_PARAMS
             { $$.lista_exprs = {$1}; 
               $$.lista_exprs.insert($$.lista_exprs.end(), $3.lista_exprs.begin(), $3.lista_exprs.end()); }
             ;

LISTA_PARAMS_OPT : LISTA_PARAMS
                 | { $$.clear(); }
                 ;

LISTA_MISTA : ATRIB
            { $$.lista_exprs = {$1}; }
            | ATRIB ',' LISTA_MISTA
            { $$.lista_exprs = {$1}; 
              $$.lista_exprs.insert($$.lista_exprs.end(), $3.lista_exprs.begin(), $3.lista_exprs.end()); }
            ;

LISTA_MISTA_OPT : LISTA_MISTA
                | { $$.clear(); }
                ;

ECOND : ECOND '?' EOBJ ':' EOBJ
      | ATRIB
      ;

CAMPOS : CAMPO ',' CAMPOS
       { $$.c = $1.c + $3.c; }
       | CAMPO
       ;

CAMPO : ID ':' EOBJ
      { $$.c = "'" + $1.c[0] + "'" + $3.c + "[<=]"; }
      | ID
      { $$.c = "'" + $1.c[0] + "'" + $1.c + "@" + "[<=]"; }
      ; 

E :  LVALUEPROP
    { $$.c = $1.esq + $1.dir + "[@]"; }
  | E IGUAL E
    { $$.c = $1.c + $3.c + $2.c;}
  | E DIF E
    { $$.c = $1.c + $3.c + $2.c;}
  | E ME_IG E
    { $$.c = $1.c + $3.c + $2.c;}
  | E MA_IG E
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
  | UN
    { $$.c = $1.c; $$.nome_var = $1.nome_var; }
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



F: ID
    { $$.c = $1.c + "@"; $$.nome_var = $1.c[0]; }
    | CDOUBLE
    | CINT
    | CSTRING
    | CBOOL
   // | '(' EXPR_VIRGULA ')' { $$.c = $2.c; }
    | LIST
    | '-' F 
      { $$.c = "0" + $2.c + "-" ; }
    | '+' F
      { $$.c = "0" + $2.c + "+" ; }
   // | '{' '}' 
     // { $$.c = vector<string>{"{}"}; }
    | CHAMA_FUNC
    | FUNCTION '(' LISTA_PARAMS_OPT ')' 
      {
         string endereco_funcao = gera_label( "func_anon" );
         ts.push_back( map< string, Simbolo >{} ); 
         blocos_alinhados_em_funcao.push_back(0);
         
         vector<string> codigo_params;
         for(size_t i=0; i<$3.lista_exprs.size(); i++) {
            Atributos& p = $3.lista_exprs[i];
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
         $$.c = codigo_params;
         $$.endereco_funcao = endereco_funcao;
      }
      '{' CMDs '}'
      {
         string endereco_funcao = $5.endereco_funcao;
         vector<string> codigo_params = $5.c;

         $$.c = vector<string>{"{}"} + "'&funcao'" + endereco_funcao + "[<=]";
         funcoes = funcoes + define_label(endereco_funcao) + codigo_params + $7.c +
                   "undefined" + "'&retorno'" + "@" + "~";
         ts.pop_back(); 
         blocos_alinhados_em_funcao.pop_back();
      }
    | '(' LISTA_MISTA ')' 
      { 
         $$.c.clear();
         for(size_t i=0; i<$2.lista_exprs.size(); i++) {
            if (i > 0) $$.c.push_back("^");
            $$.c += $2.lista_exprs[i].c;
            
            if ($2.lista_exprs[i].eh_atribuicao) {
               checa_simbolo($2.lista_exprs[i].nome_var, true);
            }
         }
      }
  ;

CHAMA_FUNC : F '(' LISTA_ARGS ')' 
              { 
                $$.c = $3.c + $1.c + "$" ;
              }
            | LVALUEPROP '(' LISTA_ARGS ')' 
              { 
                $$.c = $3.c + $1.esq + $1.dir + "[@]" + "$" ;
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



