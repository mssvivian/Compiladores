%{
#include <string>
#include <vector>
#include <sstream>
using namespace std;
int token( int tk );
vector<string> tokeniza(const string& textoFonte);
string trim(const std::string& s, const char* t);
%}

D  	        [0-9]
L   	    [A-Za-z_]
INT	        {D}+
DOUBLE      {D}+"."{D}+([Ee][+\-]?{D}+)?
ID          {L}({L}|{D})*
STRING      (["](\\\"|\\\'|\"\"|\"\'|\'\'|[^"])*["]|['](\\\"|\\\'|\"\"|\"\'|\'\'|[^'])*['])

WS          [ \n\t\r]*
ARGS        {WS}{ID}{WS}(","{WS}{ID}{WS})*")""{WS}""=>"
ASM         "asm{"[^}]*"}"

%%

"\t"       { coluna += 4; }
" "        { coluna++; }
"\n"	   { linha++; coluna = 0; }
"\r"	   {  }

{DOUBLE}   { return token( CDOUBLE ); }
{INT}	   { return token( CINT ); } 

"&&"	   { return token( AND ); }
"||"	   { return token( OR ); }
"<="	   { return token( ME_IG ); }
">="	   { return token( MA_IG ); }
"!="	   { return token( DIF ); }
"=="	   { return token( IGUAL ); }
"=>"	   { return token( SETA ); }
","{WS}")"  { return token( ')' ); }

"("{WS}","{WS}")" { return token( 1 ); }

")"/{WS}"=>" { return token( FECHA_PARENTESES_LAMBDA ); }

","{WS}")"/{WS}"=>" { return token( FECHA_PARENTESES_LAMBDA ); }

"+="       { return token( MAIS_IGUAL ); }
"--"       { return token( MENOS_MENOS ); }
"++"       { return token( MAIS_MAIS ); }

{ASM}      { string lexema = trim( yytext + 3, " \t\n\r\f\v{}" ); 
              yylval.c = tokeniza( lexema );
              coluna += strlen( yytext ); 
              return ASM; 
            }

"let"      { return token( LET ); }
"const"    { return token( CONST ); }
"var"      { return token( VAR ); }
"if"       { return token( IF ); }
"else"     { return token( ELSE ); }
"for"      { return token( FOR ); }
"while"    { return token( WHILE ); }
"function" { return token( FUNCTION ); }
"return"   { return token( RETURN ); }
"true"     { return token( CBOOL) ; }
"false"    { return token( CBOOL ); }

{ID}       { return token( ID ); }

{STRING}   { return token( CSTRING ); }

.          { return token( *yytext ); }

%%

vector<string> tokeniza(const string& textoFonte) {

    vector<string> listaDePalavras;
    

    stringstream fluxo(textoFonte);

    string palavraAtual;


    while (fluxo >> palavraAtual) {
        listaDePalavras.push_back(palavraAtual);
    }

    return listaDePalavras;
}

string trim(const std::string& s, const char* t) {
    size_t start = s.find_first_not_of(t);

    if (start == std::string::npos) {
        return ""; 
    }

    size_t end = s.find_last_not_of(t);

    size_t length = end - start + 1;

    return s.substr(start, length);
}

int token( int tk ) {  
  yylval.c = vector<string>{ yytext };
  coluna += strlen( yytext ); 
  
  yylval.linha = linha;
  yylval.coluna = coluna;

  return tk;

}