#!/usr/bin/env python3
# Script para modificar mini_js.y adicionando funcionalidades faltantes

import re

def modify_mini_js():
    with open('mini_js.y', 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 1. Adicionar operadores faltantes na regra E
    # Procurar a regra E e adicionar os operadores após E IGUAL E
    e_rule_pattern = r'(E : ID\s+\{ \$\$\.c = \$1\.c \+ "@"; \}\s+\| LVALUEPROP\s+\{ \$\$\.c = \$1\.esq \+ \$1\.dir \+ "\[@\]"; \}\s+\| E IGUAL E\s+\{ \$\$\.c = \$1\.c \+ \$3\.c \+ \$2\.c;\})'
    
    new_operators = r'''\1
  | E DIF E
    { $$.c = $1.c + $3.c + $2.c;}
  | E ME_IG E
    { $$.c = $1.c + $3.c + $2.c; }
  | E MA_IG E
    { $$.c = $1.c + $3.c + $2.c; }
  | E AND E
    { $$.c = $1.c + $3.c + $2.c; }
  | E OR E
    { $$.c = $1.c + $3.c + $2.c; }'''
    
    content = re.sub(e_rule_pattern, new_operators, content, flags=re.DOTALL)
    
    # 2. Implementar operador ternário na regra ECOND
    econd_pattern = r'ECOND : ECOND \'\?\' EOBJ \':\' EOBJ\s+\| ATRIB\s+;'
    
    ternary_impl = r'''ECOND : ECOND '?' EOBJ ':' EOBJ
        {
          string lbl_false = gera_label("ternary_false");
          string lbl_end = gera_label("ternary_end");
          $$.c = $1.c + "!" + lbl_false + "?" + 
                 $3.c + lbl_end + "#" + 
                 define_label(lbl_false) + $5.c + 
                 define_label(lbl_end);
        }
      | ATRIB
      ;'''
    
    content = re.sub(econd_pattern, ternary_impl, content, flags=re.DOTALL)
    
    # 3. Adicionar parênteses em expressões na regra F
    # Descomentar a linha que já existe
    content = content.replace('   // | \'(\' EXPR_VIRGULA \')\' { $$.c = $2.c; }', 
                             '    | \'(\' EXPR_VIRGULA \')\' \n      { $$.c = $2.c; }')
    
    # 4. Corrigir EXPR_VIRGULA para descartar valores intermediários
    expr_virgula_pattern = r'EXPR_VIRGULA : ATRIB\s+\| ATRIB \',\' EXPR_VIRGULA\s+\{ \$\$\.c = \$1\.c \+ \$3\.c; \}\s+// \$1 é descartado, retorna \$3\s+;'
    
    expr_virgula_impl = r'''EXPR_VIRGULA : ATRIB
              { $$.c = $1.c; }
              | ATRIB ',' EXPR_VIRGULA
                { $$.c = $1.c + "^" + $3.c; }  // $1 é descartado, retorna $3
              ;'''
    
    content = re.sub(expr_virgula_pattern, expr_virgula_impl, content, flags=re.DOTALL)
    
    # 5. Adicionar funções anônimas na regra ATRIB (antes de | E)
    atrib_e_pattern = r'(\s+\| E\s+;)'
    
    anon_func = r'''  | FUNCTION '(' EMPILHA_TS_FUNC LISTA_PARAMs ')' '{' CMDs '}' DESEMPILHA_TS_FUNC
    {
      string endereco_funcao = gera_label("func_anon");
      $$.c = vector<string>{"{}"}  + "'&funcao'" + endereco_funcao + "[<=]";
      funcoes = funcoes + define_label(endereco_funcao) + $4.c + $7.c +
                "undefined" + "'&retorno'" + "@" + "~";
    }
\1'''
    
    content = re.sub(atrib_e_pattern, anon_func, content)
    
    # Salvar o arquivo modificado
    with open('mini_js.y', 'w', encoding='utf-8') as f:
        f.write(content)
    
    print("Arquivo mini_js.y modificado com sucesso!")

if __name__ == '__main__':
    modify_mini_js()
