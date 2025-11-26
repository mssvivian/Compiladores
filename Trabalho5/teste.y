%{
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream> 

using namespace std;

int linha = 1, coluna = 0; 

struct Atributos {
  vector<string> c;
  int linha = 0, coluna = 0;
  int n_args = 0;     
  vector<string> valor_default;
  int profundidade_blocos = 0;
  void clear() {
    c.clear();
    valor_default.clear();
    linha = 0;
    coluna = 0;
    n_args = 0;
    profundidade_blocos = 0;
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

vector< map<string, Simbolo> > ts = { map<string, Simbolo>{} }; 
vector<string> funcoes;

int nivel_funcao = 0;           
vector<int> pilha_blocos_funcao;
int profundidade_blocos_atual = 0;

vector<string> declara_var(TipoDecl tipo, string nome, int linha, int coluna);
void checa_simbolo(string nome, bool modificavel);

#define YYSTYPE Atributos

extern "C" int yylex();
int yyparse();
void yyerror(const char *);

extern char* yytext;

vector<string> concatena(vector<string> a, vector<string> b) {
  a.insert(a.end(), b.begin(), b.end());
  return a;
}
vector<string> operator+(vector<string> a, vector<string> b) { return concatena(a,b); }
vector<string> operator+(vector<string> a, string b) { a.push_back(b); return a; }
vector<string> operator+(string a, vector<string> b) { return vector<string>{a} + b; }

vector<string> resolve_enderecos(vector<string> entrada) {
  map<string,int> label;
  vector<string> saida;
  for (int i = 0; i < (int)entrada.size(); i++) {
    if (!entrada[i].empty() && entrada[i][0] == ':') {
      label[entrada[i].substr(1)] = (int)saida.size();
    } else {
      saida.push_back(entrada[i]);
    }
  }
  for (int i = 0; i < (int)saida.size(); i++) {
    auto it = label.find(saida[i]);
    if (it != label.end()) saida[i] = to_string(it->second);
  }
  return saida;
}

string gera_label(string prefixo) {
  static int n = 0;
  return prefixo + "_" + to_string(++n);
}

void print(vector<string> codigo) {
  for (string s : codigo) cout << s << " ";
  cout << endl;
}

vector<string> fecha_blocos(int n) {
  vector<string> r;
  for (int i = 0; i < n; i++) r.push_back("}>");
  return r;
}
%}

%token ID IF ELSE LET CONST VAR FOR WHILE FUNCTION RETURN ASM
%token CDOUBLE CSTRING CINT TRUE FALSE
%token AND OR ME_IG MA_IG DIF IGUAL
%token MAIS_IGUAL MAIS_MAIS MENOS_IGUAL MENOS_MENOS

%right '=' MAIS_IGUAL MENOS_IGUAL
%nonassoc IGUAL DIF
%nonassoc '<' '>' ME_IG MA_IG
%left '+' '-'
%left '*' '/' '%'
%left MAIS_MAIS MENOS_MENOS
%precedence UNARIO
%left '.'
%right '[' '(' 

%%

S :
  CMDs {
    vector<string> codigo = $1.c + "." + funcoes;
    print( resolve_enderecos( codigo ) );
  }
| { /* vazio */ }
;

CMDs : CMDs CMD { $$.c = $1.c + $2.c; }
     |          { $$.clear(); }
     ;

CMD :
    CMD_LET ';'
  | CMD_VAR ';'
  | CMD_CONST ';'
  | CMD_IF
  | CMD_WHILE
  | CMD_FOR
  | CMD_FUNC
  | RETURN E ';' {
      int n = (nivel_funcao>0 && !pilha_blocos_funcao.empty()) ? pilha_blocos_funcao.back() : profundidade_blocos_atual;
      $$.c = $2.c + fecha_blocos(n) + "\"&retorno\"" + "@" + "~";
    }
  | RETURN ';' {
      int n = (nivel_funcao>0 && !pilha_blocos_funcao.empty()) ? pilha_blocos_funcao.back() : profundidade_blocos_atual;
      $$.c = vector<string>{"undefined"} + fecha_blocos(n) + "\"&retorno\"" + "@" + "~";
    }
  | CMD_ASM ';' { $$.c = $1.c + "^"; }
  | E ';' { $$.c = $1.c + "^"; }
  | '{' {
      if (nivel_funcao>0) {
        if (pilha_blocos_funcao.empty()) pilha_blocos_funcao.push_back(0);
        pilha_blocos_funcao.back()++;
      } else {
        profundidade_blocos_atual++;
      }
    } EMPILHA_TS CMDs '}' {
      if (nivel_funcao>0) pilha_blocos_funcao.back()--;
      else profundidade_blocos_atual--;
      ts.pop_back();
      $$.c = "<{" + $4.c + "}>";
    }
  | ';' { $$.clear(); }
  ;

EMPILHA_TS : { ts.push_back( map<string,Simbolo>{} ); } ;

CMD_FUNC :
  FUNCTION ID { declara_var(Var, $2.c[0], $2.linha, $2.coluna); } 
  '(' EMPILHA_TS { 
        nivel_funcao++; 
        pilha_blocos_funcao.push_back(0); 
      } LISTA_PARAMs ')'
  '{' CMDs '}' {
    nivel_funcao--;
    if (!pilha_blocos_funcao.empty()) pilha_blocos_funcao.pop_back();

    string lbl = gera_label("func_" + $2.c[0]);
    string def = ":" + lbl;

    $$.c = $2.c + "&" + $2.c + "{}" + "=" + "\"&funcao\"" + lbl + "[=]" + "^";

    funcoes = funcoes + def + $7.c + $10.c + "undefined" + "\"&retorno\"" + "@" + "~";

    ts.pop_back();
  }
;

LISTA_PARAMs : PARAMs
             | PARAMs ','   { $$ = $1; }
             | { $$.clear(); }
             ;

PARAMs :
    PARAMs ',' PARAM {
      int i = $1.n_args;
      $$.c = $1.c + $3.c
           + "&" + $3.c + "arguments" + "@" + to_string(i) + "[@]" + "=" + "^";
      if ($3.valor_default.size() > 0) {
        string lbl_fim_if = gera_label("fim_if_default");
        string def_lbl    = ":" + lbl_fim_if;
        $$.c = $$.c
             + $3.c + "@" + "undefined" + "@" + "!="
             + lbl_fim_if + "?"
             + $3.c + $3.valor_default + "=" + "^"
             + def_lbl;
      }
      $$.n_args = $1.n_args + $3.n_args;
    }
  | PARAM {
      $$.c = $1.c
           + "&" + $1.c + "arguments" + "@" + "0" + "[@]" + "=" + "^";
      if ($1.valor_default.size() > 0) {
        string lbl_fim_if = gera_label("fim_if_default");
        string def_lbl    = ":" + lbl_fim_if;
        $$.c = $$.c
             + $1.c + "@" + "undefined" + "@" + "!="
             + lbl_fim_if + "?"
             + $1.c + $1.valor_default + "=" + "^"
             + def_lbl;
      }
      $$.n_args = $1.n_args;
    }
  ;

PARAM :
    ID {
      $$.c = $1.c;
      $$.n_args = 1;
      $$.valor_default.clear();
      declara_var(Let, $1.c[0], $1.linha, $1.coluna);
    }
  | ID '=' E {
      $$.c = $1.c;
      $$.n_args = 1;
      $$.valor_default = $3.c;
      declara_var(Let, $1.c[0], $1.linha, $1.coluna);
    }
  ;

CMD_ASM : E ASM { $$.c = $1.c + $2.c; }
        | ASM   { $$.c = $1.c; }
        ;

CMD_LET : LET LET_VARs { $$.c = $2.c; } ;
LET_VARs : LET_VAR ',' LET_VARs { $$.c = $1.c + $3.c; } | LET_VAR ;
LET_VAR :
    ID { $$.c = declara_var(Let, $1.c[0], $1.linha, $1.coluna); }
  | ID '=' E {
      $$.c = declara_var(Let, $1.c[0], $1.linha, $1.coluna) + $1.c + $3.c + "=" + "^";
    }
  ;

CMD_VAR : VAR VAR_VARs { $$.c = $2.c; } ;
VAR_VARs : VAR_VAR ',' VAR_VARs { $$.c = $1.c + $3.c; } | VAR_VAR ;
VAR_VAR :
    ID { $$.c = declara_var(Var, $1.c[0], $1.linha, $1.coluna); }
  | ID '=' E {
      $$.c = declara_var(Var, $1.c[0], $1.linha, $1.coluna) + $1.c + $3.c + "=" + "^";
    }
  ;

CMD_CONST : CONST CONST_VARs { $$.c = $2.c; } ;
CONST_VARs : CONST_VAR ',' CONST_VARs { $$.c = $1.c + $3.c; } | CONST_VAR ;
CONST_VAR :
    ID '=' E {
      $$.c = declara_var(Const, $1.c[0], $1.linha, $1.coluna) + $1.c + $3.c + "=" + "^";
    }
  ;

OBJ :
    ID            { checa_simbolo($1.c[0], false); $$.c = $1.c + "@"; }
  | OBJ '[' E ']' { $$.c = $1.c + $3.c + "[@]"; }
  | OBJ '.' ID    { $$.c = $1.c + ("\"" + $3.c[0] + "\"") + "[@]"; }
  | '(' E ')'     { $$.c = $2.c; }
  ;

LVALUE : ID ;
LVALUEPROP :
    OBJ '[' E ']' { $$.c = $1.c + $3.c; }
  | OBJ '.' ID    { $$.c = $1.c + ("\"" + $3.c[0] + "\""); }
  ;

CMD_IF :
    IF '(' E ')' CMD {
      string lbl_fim = gera_label("if_fim");
      string def_lbl_fim = ":" + lbl_fim;
      $$.c = $3.c + "!" + lbl_fim + "?" + $5.c + def_lbl_fim;
    }
  | IF '(' E ')' CMD ELSE CMD {
      string lbl_else = gera_label("if_else");
      string def_lbl_else = ":" + lbl_else;
      string lbl_fim_if = gera_label("if_end");
      string def_lbl_fim_if = ":" + lbl_fim_if;
      $$.c = $3.c + "!" + lbl_else + "?" + 
             $5.c + lbl_fim_if + "#" + 
             def_lbl_else + $7.c + 
             def_lbl_fim_if;
    }
  ;

CMD_WHILE :
  WHILE '(' E ')' CMD {
    string lbl_ini = gera_label("while_ini");
    string def_lbl_ini = ":" + lbl_ini;
    string lbl_fim = gera_label("while_fim");
    string def_lbl_fim = ":" + lbl_fim;
    $$.c = def_lbl_ini + $3.c + "!" + lbl_fim + "?" + $5.c + lbl_ini + "#" + def_lbl_fim;
  }
;

CMD_FOR :
  FOR '(' PRIM_E ';' E_opt ';' E_opt ')' CMD {
    string lbl_fim_for = gera_label("for_fim");
    string def_lbl_fim_for = ":" + lbl_fim_for;
    string lbl_condicao_for = gera_label("for_cond");
    string def_lbl_condicao_for = ":" + lbl_condicao_for;
    $$.c = $3.c + def_lbl_condicao_for + $5.c + "!" + lbl_fim_for + "?" + $9.c + $7.c + "^" +
           lbl_condicao_for + "#" + def_lbl_fim_for;
  }
;

PRIM_E :
    CMD_LET { $$ = $1; }
  | CMD_VAR { $$ = $1; }
  | E       { $$.c = $1.c + "^"; }
  |         { $$.clear(); }
  ;

E_opt : E | { $$.c.clear(); };

E :
    LVALUE '=' E            { checa_simbolo($1.c[0], true);  $$.c = $1.c + $3.c + "="; }
  | LVALUE MAIS_IGUAL E     { checa_simbolo($1.c[0], true);  $$.c = $1.c + $1.c + "@" + $3.c + "+" + "="; }

  | LVALUEPROP '=' E        { $$.c = $1.c + $3.c + "[=]"; }
  | LVALUEPROP MAIS_IGUAL E { $$.c = $1.c + $1.c + "[@]" + $3.c + "+" + "[=]"; }

  | E '(' LISTA_ARGs ')'    { $$.c = $3.c + to_string($3.n_args) + $1.c + "$"; }

  | E '<' E    { $$.c = $1.c + $3.c + "<"; }
  | E '>' E    { $$.c = $1.c + $3.c + ">"; }
  | E IGUAL E  { $$.c = $1.c + $3.c + "=="; }
  | E DIF E    { $$.c = $1.c + $3.c + "!="; }
  | E ME_IG E  { $$.c = $1.c + $3.c + "<="; }
  | E MA_IG E  { $$.c = $1.c + $3.c + ">="; }
  | E '+' E    { $$.c = $1.c + $3.c + "+"; }
  | E '-' E    { $$.c = $1.c + $3.c + "-"; }
  | E '' E    { $$.c = $1.c + $3.c + ""; }
  | E '/' E    { $$.c = $1.c + $3.c + "/"; }
  | E '%' E    { $$.c = $1.c + $3.c + "%"; }

  | '-' E %prec UNARIO            { $$.c = "0" + $2.c + "-"; }
  | MAIS_MAIS LVALUE %prec UNARIO { checa_simbolo($2.c[0], true); $$.c = $2.c + $2.c + "@" + "1" + "+" + "="; }
  | MENOS_MENOS LVALUE %prec UNARIO { checa_simbolo($2.c[0], true); $$.c = $2.c + $2.c + "@" + "1" + "-" + "="; }

  | LVALUE       { checa_simbolo($1.c[0], false); $$.c = $1.c + "@"; }
  | LVALUEPROP   { $$.c = $1.c + "[@]"; }

  | '(' E ')'  { $$.c = $2.c; }
  | CINT
  | CDOUBLE
  | CSTRING { string s = $1.c[0]; s = s.substr(1, s.length()-2); $$.c = vector<string>{"\"" + s + "\""}; }
  | TRUE    { $$.c = vector<string>{"true"}; }
  | FALSE   { $$.c = vector<string>{"false"}; }
  | '{' '}' { $$.c = vector<string>{"{}"}; }
  | '[' ']' { $$.c = vector<string>{"[]"}; }

  | LVALUE MAIS_MAIS   { checa_simbolo($1.c[0], true); $$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "+" + "=" + "^"; }
  | LVALUE MENOS_MENOS { checa_simbolo($1.c[0], true); $$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "-" + "=" + "^"; }

  | OBJ '[' E ']' MENOS_MENOS {
      string tO = string("tO") + gera_label("p");
      string tI = string("tI") + gera_label("p");
      string tV = string("tV") + gera_label("p");
      vector<string> dec = declara_var(Let,tO,$1.linha,$1.coluna)
                         + declara_var(Let,tI,$1.linha,$1.coluna)
                         + declara_var(Let,tV,$1.linha,$1.coluna);
      $$.c = "<{" + dec
        + tO + $1.c + "=" + "^"
        + tI + $3.c + "=" + "^"
        + tV + tO + "@" + tI + "@" + "[@]" + "=" + "^"
        + tO + "@" + tI + "@" + tV + "@" + "1" + "-" + "[=]" + "^"
        + tV + "@"
        + "}>";
    }
  | MAIS_MAIS OBJ '[' E ']' %prec UNARIO {
      string tO = string("tO") + gera_label("p");
      string tI = string("tI") + gera_label("p");
      string tN = string("tN") + gera_label("p");
      vector<string> dec = declara_var(Let,tO,$2.linha,$2.coluna)
                         + declara_var(Let,tI,$2.linha,$2.coluna)
                         + declara_var(Let,tN,$2.linha,$2.coluna);
      $$.c = "<{" + dec
        + tO + $2.c + "=" + "^"
        + tI + $4.c + "=" + "^"
        + tN + tO + "@" + tI + "@" + "[@]" + "1" + "+" + "=" + "^"
        + tO + "@" + tI + "@" + tN + "@" + "[=]" + "^"
        + tN + "@"
        + "}>";
    }
  | OBJ '[' E ']' MAIS_MAIS {
      string tO = string("tO") + gera_label("p");
      string tI = string("tI") + gera_label("p");
      string tV = string("tV") + gera_label("p");
      vector<string> dec = declara_var(Let,tO,$1.linha,$1.coluna)
                         + declara_var(Let,tI,$1.linha,$1.coluna)
                         + declara_var(Let,tV,$1.linha,$1.coluna);
      $$.c = "<{" + dec
        + tO + $1.c + "=" + "^"
        + tI + $3.c + "=" + "^"
        + tV + tO + "@" + tI + "@" + "[@]" + "=" + "^"
        + tO + "@" + tI + "@" + tV + "@" + "1" + "+" + "[=]" + "^"
        + tV + "@"
        + "}>";
    }
  | MENOS_MENOS OBJ '[' E ']' %prec UNARIO {
      string tO = string("tO") + gera_label("p");
      string tI = string("tI") + gera_label("p");
      string tN = string("tN") + gera_label("p");
      vector<string> dec = declara_var(Let,tO,$2.linha,$2.coluna)
                         + declara_var(Let,tI,$2.linha,$2.coluna)
                         + declara_var(Let,tN,$2.linha,$2.coluna);
      $$.c = "<{" + dec
        + tO + $2.c + "=" + "^"
        + tI + $4.c + "=" + "^"
        + tN + tO + "@" + tI + "@" + "[@]" + "1" + "-" + "=" + "^"
        + tO + "@" + tI + "@" + tN + "@" + "[=]" + "^"
        + tN + "@"
        + "}>";
    }
  | OBJ '.' ID MENOS_MENOS {
      string key = "\"" + $3.c[0] + "\"";
      string tO = string("tO") + gera_label("p");
      string tV = string("tV") + gera_label("p");
      vector<string> dec = declara_var(Let,tO,$1.linha,$1.coluna)
                         + declara_var(Let,tV,$1.linha,$1.coluna);
      $$.c = "<{" + dec
        + tO + $1.c + "=" + "^"
        + tV + tO + "@" + key + "[@]" + "=" + "^"
        + tO + "@" + key + tV + "@" + "1" + "-" + "[=]" + "^"
        + tV + "@"
        + "}>";
    }
  | OBJ '.' ID MAIS_MAIS {
      string key = "\"" + $3.c[0] + "\"";
      string tO = string("tO") + gera_label("p");
      string tV = string("tV") + gera_label("p");
      vector<string> dec = declara_var(Let,tO,$1.linha,$1.coluna)
                         + declara_var(Let,tV,$1.linha,$1.coluna);
      $$.c = "<{" + dec
        + tO + $1.c + "=" + "^"
        + tV + tO + "@" + key + "[@]" + "=" + "^"
        + tO + "@" + key + tV + "@" + "1" + "+" + "[=]" + "^"
        + tV + "@"
        + "}>";
    }
  ;

LISTA_ARGs : ARGs | ARGs ',' { $$ = $1; } | { $$.clear(); };
ARGs :
    ARGs ',' E { $$.c = $1.c + $3.c; $$.n_args = $1.n_args + 1; }
  | E          { $$.c = $1.c; $$.n_args = 1; }
  ;

%%

#include "lex.yy.c"

vector<string> declara_var(TipoDecl tipo, string nome, int linha_atual, int coluna_atual) {
  auto& topo = ts.back();
  if (topo.count(nome) == 0) {
    topo[nome] = Simbolo{ tipo, linha_atual, coluna_atual };
    return vector<string>{ nome, "&" };
  } else if (tipo == Var && topo[nome].tipo == Var) {
    topo[nome] = Simbolo{ tipo, linha_atual, coluna_atual };
    return vector<string>{};
  } else {
    cerr << "Erro: a variável '" << nome 
         << "' já foi declarada na linha " << topo[nome].linha << "."
         << endl;
    exit(1);
  }
}

void checa_simbolo(string nome, bool modificavel) {
  if (nivel_funcao > 0) return;
  for (int i = (int)ts.size()-1; i >= 0; i--) {
    auto& atual = ts[i];
    if (atual.count(nome) > 0) {
      if (modificavel && atual[nome].tipo == Const) {
        cerr << "Erro: a variável '" << nome << "' não pode ser modificada." << endl;
        exit(1);
      }
      return;
    }
  }
  cerr << "Erro: a variável '" << nome << "' não foi declarada." << endl;
  exit(1);
}

void yyerror(const char* st) {
  fprintf(stderr, "Erro de sintaxe próximo a: %s (linha %d)\n", yytext, linha);
  exit(1);
}

int main(int argc, char* argv[]) {
  yyparse();
  return 0;
}