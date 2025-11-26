#!/usr/bin/env python3

import subprocess
import re
import os
import sys

# --- Configuração ---
# Altere estes caminhos se seus executáveis estiverem em outro lugar
COMPILER_PATH = './mini_js'
INTERPRETER_PATH = './mdp/interpretador'
# --------------------

# Estrutura de dados para todos os testes
TESTS = [
    {
        "id": 1,
        "input": """
function log( msg ) {
  msg asm{println # undefined};
}

let r = log( 'Hello, world!' );
""",
        "expected": """
=== Console ===
Hello, world!
=== Vars ===
|{ log: { &funcao: ##; }; r: undefined; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 2,
        "input": """
const a = 9;
function log( msg ) { 
  msg asm{println # undefined};
}

var c = [ 
];
(log)( c );
{ let a = 3.1; }
{ const a = 8;
  let b = 7;
  c = b; }
var c = c+ c++;
""",
        "expected": """
=== Console ===
[ ]
=== Vars ===
|{ a: 9; c: 14; log: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 3,
        "input": """
let b, c;
{ const a = 0;
  var c;
}
let a = 8;
if( a ) {[];}
else {
  let b, d = { };
}
let d;
let a;
""",
        "expected": """
Erro: a variável 'a' já foi declarada na linha 5.
=== Console ===
=== Vars ===
|{ undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 4,
        "input": """
function print(x) { x asm{println # undefined};
} 
var a, b = [];
const c = -1;
print( c + c );
(b)[0] = true;
if( b[0] ) {
  let b = { };
  (b).m = 9;
  print( b );
  a = b.m;
}
""",
        "expected": """
=== Console ===
-2
{ m: 9; }
=== Vars ===
|{ a: 9; b: [ 0: true; ]; c: -1; print: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 5,
        "input": """
var a, b = [];
const c = -1;
b[0] = true;
function print(x) { x asm{println # undefined};
  0 asm{undefined .};
  return 0;
} 
if( b[0] ) { let a = -1; {
    let b = { };
     b.m = 9;
     print( b ); 
   }
   a = b;
}
""",
        "expected": """
=== Console ===
{ m: 9; }
=== Vars ===
|{ a: undefined; b: [ 0: true; ]; c: -1; print: { &funcao: ##; }; undefined: undefined; }|
|{ a: -1; }|
|{ b: { m: 9; }; }|
|{ &retorno: ##; arguments: [ 0: { m: 9; }; ]; x: { m: 9; }; }|
=== Pilha ===
|0|
|undefined|
"""
    },
    {
        "id": 6,
        "input": """
let str = "hello";
function valor() { return str + ", world"; }
var a = valor();
str = "HELLO";;;
var b = valor
(
)
;
""",
        "expected": """
=== Console ===
=== Vars ===
|{ a: hello, world; b: HELLO, world; str: HELLO; undefined: undefined; valor: { &funcao: ##; }; }|
=== Pilha ===
"""
    },
    {
        "id": 7,
        "input": """
function sqr(x) { return x*x + y; }
let y = 0.01, a = sqr( 5.2, 7 );
""",
        "expected": """
=== Console ===
=== Vars ===
|{ a: 27.05; sqr: { &funcao: ##; }; undefined: undefined; y: 0.01; }|
=== Pilha ===
"""
    },
    {
        "id": 8,
        "input": """
function sqr(x,) { return x*x; }
let y = 0.01, a = sqr( 5.2, 7, );
""",
        "expected": """
=== Console ===
=== Vars ===
|{ a: 27.05; sqr: { &funcao: ##; }; undefined: undefined; y: 0.01; }|
=== Pilha ===
"""
    },
    {
        "id": 9,
        "input": """
function f(x) { 
  if( x % 2 == 0 ) { 
    let x = 1;

    function g() { 
      {
        return 3.1 + x++; 
        1; 
      }  
    }

    for( let i = 0; i < 5; i++ ) {     
      g();
    }
    return g() + x;
    let v;
  }
  else 
    return x; 
}

let a = f(4);
let b = f(3);
""",
        "expected": """
=== Console ===
=== Vars ===
|{ a: 16.1; b: 3; f: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 10,
        "input": """
let x = 0;

function f( a, b ) {
  let y = a++ * --b;
  x += ++a*10 + b---a++;

  return y;
}

let y = [], i = 2;
y[2] = 2;
y[3] = 4;
let c;
{ c = f( 7, y[++y[i++]]-- ); }
""",
        "expected": """
=== Console ===
=== Vars ===
|{ c: 21; f: { &funcao: ##; }; i: 3; undefined: undefined; x: 84; y: [ 0: undefined; 1: undefined; 2: 3; 3: 3; ]; }|
=== Pilha ===
"""
    },
    {
        "id": 11,
        "input": """
function f( x, y, z ) { 
  let z;
  return x*x + y;
}
let f( 5.2, 7 );
""",
        "expected": """
Erro: a variável 'z' já foi declarada na linha 1.
=== Console ===
=== Vars ===
|{ undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 12,
        "input": """
function mdc( a, b ) { 
  if( b == 0 ) 
    return a;
  else
    return mdc( b, a % b );
}

let a = mdc( 24, 33, 8  );
""",
        "expected": """
=== Console ===
=== Vars ===
|{ a: 3; mdc: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 13,
        "input": """
function teste( a, b ) { 
  if( a > b ) 
    return a;
}

let um = teste( 3, 4 ),
    dois = teste( 4, 3 );
""",
        "expected": """
=== Console ===
=== Vars ===
|{ dois: 4; teste: { &funcao: ##; }; um: undefined; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 14,
        "input": """
let console = { };
let Number =  { 
};

function log( msg ) {
  msg asm{println # undefined};
}

function number_to_string( msg ) {
  msg asm{to_string # '&retorno' @ ~};
};

console.log = log;
Number.toString = number_to_string;

let a = "Saida: ";
let b = 3.14;

console.log( a + Number.toString( b ) );
""",
        "expected": """
=== Console ===
Saida: 3.14
=== Vars ===
|{ Number: { toString: { &funcao: ##; }; }; a: Saida: ; b: 3.14; console: { log: { &funcao: ##; }; }; log: { &funcao: ##; }; number_to_string: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 15,
        "input": """
let console = {};

function exit( n ) {
  'Codigo de erro: ' asm{print # undefined};
  n asm{println # undefined};
  0 asm{.};
}

function teste( a, b, c ) {
  exit( b );
}

let a = "Saida: ";
let b = 3.14;

console.teste = {};
console.teste.log = [
] ;
console.teste.log[1] = teste;

console.teste.log[1]( a, b, "5" );
""",
        "expected": """
=== Console ===
Codigo de erro: 3.14
=== Vars ===
|{ a: Saida: ; b: 3.14; console: { teste: { log: [ 0: undefined; 1: { &funcao: ##; }; ]; }; }; exit: { &funcao: ##; }; teste: { &funcao: ##; }; undefined: undefined; }|
|{ &retorno: ##; a: Saida: ; arguments: [ 0: Saida: ; 1: 3.14; 2: 5; ]; b: 3.14; c: 5; }|
|{ &retorno: ##; arguments: [ 0: 3.14; ]; n: 3.14; }|
=== Pilha ===
|0|
"""
    },
    {
        "id": 16,
        "input": """
function f( x ) {
  let b = 5 * x;
  let c = { };

  c.num = b;
  (c).arr = [];
  (c.arr)[1] = 0;
  return c ;
}

let res = f( 11 );
""",
        "expected": """
=== Console ===
=== Vars ===
|{ f: { &funcao: ##; }; res: { arr: [ 0: undefined; 1: 0; ]; num: 55; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 17,
        "input": """
function f( x ) {
  let b = f;
  const h = [ ];
  f = x;
  return b ;
}

let a;
let g = f( a = [ ], { 
} );
""",
        "expected": """
=== Console ===
=== Vars ===
|{ a: [ ]; f: [ ]; g: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 18,
        "input": """
function f( x = 9, y = 8.1 ) {
  log( 'Parametros: ' ); log( x ); log( ' ' ); log( y, true ); 
  return x + y;
}

function log( msg = 'Default!', pular_linha = false ) {
  if( pular_linha )
    msg asm{println # undefined};
  else
    msg asm{print # undefined};
}

log();
log( '-', true );
log( f(), true );
log( 'F = 'M + f(), true );
let x = f(),
    a = f( 1 ),
    b = f( false, 8 ),
    c = f( 1, 2 );
""",
        "expected": """
=== Console ===
Default!-
Parametros: 9 8.1
17.1
Parametros: 9 8.1
Default!
Parametros: 9 8.1
Parametros: 1 8.1
Parametros: false 8
Parametros: 1 2
=== Vars ===
|{ a: 9.1; b: undefined; c: 3; f: { &funcao: ##; }; log: { &funcao: ##; }; undefined: undefined; x: 17.1; }|
=== Pilha ===
"""
    },
    {
        "id": 19,
        "input": """
function log( msg = 'Default!', pular_linha = f() ) {
  if( pular_linha )
    msg asm{println # undefined};
  else
    msg asm{print # undefined};
}

function f( a = f( 1 ) ) {
  log( 'Parametro a: ', false ); log( ' ', false ); log( a, true ); 
  if( a == true )
    return false;
  else
    return a == 1;
}

log( f() );
log( ' ', true );
log( f( 2 ), true );
let a = f();
""",
        "expected": """
=== Console ===
Parametro a:  1
Parametro a:  true
Parametro a:  1
Parametro a:  true
false 
Parametro a:  2
false
Parametro a:  1
Parametro a:  true
=== Vars ===
|{ a: false; f: { &funcao: ##; }; log: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    },
    {
        "id": 20,
        "input": """
function getNome( obj ) { return obj.nome; }

function getClass( obj ) { return obj.class; }

function criaAluno( nome, celular, email ) {
  let aluno = {};

  aluno.nome = nome;
  aluno.celular = celular;
  aluno.email = email;
  aluno.super = prototipoAluno;
  aluno.getNome = getNome;

  return aluno;
}

function log( msg ) {
  msg asm{println # undefined};
}

function invoke( obj, metodo ) {
  if( toString( obj[metodo] ) == 'undefined' )
    return obj.super[metodo]( obj.super );
  else
    return obj[metodo]( obj );
}

function toString( msg ) {
  msg asm{to_string # '&retorno' @ ~};
}

let prototipoAluno = { };

prototipoAluno.class = 'Classe Aluno';
prototipoAluno.getClass = getClass;

let joao = criaAluno( 'Joao', '123456', 'eu@aqui.com' );
let maria = criaAluno( 'Maria', '123457', 'voce@la.com' );

log( invoke( joao, 'getNome' ) );
log( invoke( joao, 'getClass' ) );
log( invoke( maria, 'getNome' ) );
log( invoke( maria, 'getClass' ) );
""",
        "expected": """
=== Console ===
Joao
Classe Aluno
Maria
Classe Aluno
=== Vars ===
|{ criaAluno: { &funcao: ##; }; getClass: { &funcao: ##; }; getNome: { &funcao: ##; }; invoke: { &funcao: ##; }; joao: { celular: 123456; email: eu@aqui.com; getNome: { &funcao: ##; }; nome: Joao; super: { class: Classe Aluno; getClass: { &funcao: ##; }; }; }; log: { &funcao: ##; }; maria: { celular: 123457; email: voce@la.com; getNome: { &funcao: ##; }; nome: Maria; super: { class: Classe Aluno; getClass: { &funcao: ##; }; }; }; prototipoAluno: { class: Classe Aluno; getClass: { &funcao: ##; }; }; toString: { &funcao: ##; }; undefined: undefined; }|
=== Pilha ===
"""
    }
]

# Cores para o terminal
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def normalize_output(text):
    """
    Normaliza a string de saída para permitir comparações
    - Remove espaços em branco no início/fim de cada linha
    - Substitui endereços de função/retorno por '##'
    """
    if text is None:
        return ""
    
    # Substitui endereços de função: `&funcao: 123` -> `&funcao: ##`
    text = re.sub(r'(&funcao: )\d+', r'\1##', text)
    # Substitui endereços de retorno: `&retorno: 456` -> `&retorno: ##`
    text = re.sub(r'(&retorno: )\d+', r'\1##', text)
    
    # Remove espaços em branco de cada linha e junta
    lines = [line.strip() for line in text.strip().split('\n')]
    return '\n'.join(lines).strip()

def run_test(test):
    """
    Executa um único teste
    """
    test_id = test['id']
    test_input = test['input'].strip()
    expected_output_norm = normalize_output(test['expected'])

    # 1. Compilar o código
    try:
        compile_proc = subprocess.run(
            [COMPILER_PATH],
            input=test_input,
            capture_output=True,
            text=True,
            encoding='utf-8',
            timeout=5
        )
    except FileNotFoundError:
        print(f"{bcolors.FAIL}Erro: Compilador '{COMPILER_PATH}' não encontrado.{bcolors.ENDC}")
        return False
    except Exception as e:
        print(f"{bcolors.FAIL}Erro ao executar o compilador no Teste {test_id}: {e}{bcolors.ENDC}")
        return False

    # 2. Executar o interpretador
    # O interpretador sempre roda, lendo o stdout do compilador
    # Isso simula seu Makefile (rodando com código vazio em caso de erro de compilação)
    try:
        interpreter_proc = subprocess.run(
            [INTERPRETER_PATH],
            input=compile_proc.stdout, # Passa o código compilado como stdin
            capture_output=True,
            text=True,
            encoding='utf-8',
            timeout=5
        )
    except FileNotFoundError:
        print(f"{bcolors.FAIL}Erro: Interpretador '{INTERPRETER_PATH}' não encontrado.{bcolors.ENDC}")
        return False
    except Exception as e:
        print(f"{bcolors.FAIL}Erro ao executar o interpretador no Teste {test_id}: {e}{bcolors.ENDC}")
        return False

    # 3. Combinar saídas para formar a "Saída Real"
    # A saída real é o stderr do compilador (para erros de sintaxe)
    # + o stdout do interpretador (a execução real)
    # + o stderr do interpretador (para erros de runtime, se houver)
    
    actual_output_parts = []
    if compile_proc.stderr:
        actual_output_parts.append(compile_proc.stderr.strip())
        
    if interpreter_proc.stdout:
        # Processa a saída do interpretador para remover o código compilado
        interpreter_stdout = interpreter_proc.stdout.strip()
        console_marker = '=== Console ==='
        
        if console_marker in interpreter_stdout:
            # Pega tudo a partir do marcador '=== Console ==='
            marker_index = interpreter_stdout.find(console_marker)
            cleaned_stdout = interpreter_stdout[marker_index:]
            actual_output_parts.append(cleaned_stdout)
        else:
            # Se não houver marcador, adicione tudo (pode ser um erro)
            actual_output_parts.append(interpreter_stdout)
            
    if interpreter_proc.stderr:
        actual_output_parts.append(interpreter_proc.stderr.strip())

    actual_output_combined = '\n'.join(actual_output_parts).strip()
    actual_output_norm = normalize_output(actual_output_combined)

    # 4. Comparar e imprimir o resultado
    if actual_output_norm == expected_output_norm:
        print(f"{bcolors.OKGREEN}✅ PASS: Teste {test_id}{bcolors.ENDC}")
        return True
    else:
        print(f"{bcolors.FAIL}❌ FAIL: Teste {test_id}{bcolors.ENDC}")
        print(f"{bcolors.BOLD}--- ESPERADO ---{bcolors.ENDC}")
        print(expected_output_norm)
        print(f"{bcolors.BOLD}--- RECEBIDO ---{bcolors.ENDC}")
        print(actual_output_norm)
        print(f"{bcolors.BOLD}------------------{bcolors.ENDC}")
        return False

def main():
    # Verifica se os executáveis existem
    if not os.path.exists(COMPILER_PATH):
        print(f"{bcolors.FAIL}Erro: Compilador '{COMPILER_PATH}' não encontrado.{bcolors.ENDC}")
        print("Certifique-se de que ele está compilado e no diretório correto.")
        sys.exit(1)
        
    if not os.path.exists(INTERPRETER_PATH):
        print(f"{bcolors.FAIL}Erro: Interpretador '{INTERPRETER_PATH}' não encontrado.{bcolors.ENDC}")
        sys.exit(1)
        
    print(f"{bcolors.HEADER}Iniciando bateria de testes...{bcolors.ENDC}")
    print(f"Compilador: {COMPILER_PATH}")
    print(f"Interpretador: {INTERPRETER_PATH}\n")
    
    passed_count = 0
    failed_count = 0
    
    # Pega argumentos da linha de comando para rodar testes específicos
    test_ids_to_run = []
    if len(sys.argv) > 1:
        try:
            test_ids_to_run = [int(arg) for arg in sys.argv[1:]]
            print(f"Rodando testes específicos: {test_ids_to_run}\n")
        except ValueError:
            print(f"{bcolors.FAIL}Argumento inválido. Forneça números de teste (ex: ./run_tests.py 1 3 5){bcolors.ENDC}")
            sys.exit(1)
            
    tests_to_run = TESTS
    if test_ids_to_run:
        tests_to_run = [t for t in TESTS if t['id'] in test_ids_to_run]
        if len(tests_to_run) != len(test_ids_to_run):
            print(f"{bcolors.WARNING}Atenção: Alguns IDs de teste não foram encontrados.{bcolors.ENDC}")

    if not tests_to_run:
        print(f"{bcolors.WARNING}Nenhum teste para rodar.{bcolors.ENDC}")
        sys.exit(0)

    for test in tests_to_run:
        if run_test(test):
            passed_count += 1
        else:
            failed_count += 1
            
    # Relatório Final
    print("\n" + "="*30)
    print(f"{bcolors.BOLD}RELATÓRIO FINAL{bcolors.ENDC}")
    print(f"{bcolors.OKGREEN}Passaram: {passed_count}{bcolors.ENDC}")
    print(f"{bcolors.FAIL}Falharam: {failed_count}{bcolors.ENDC}")
    print("="*30)
    
    # Retorna um código de erro se algum teste falhar
    if failed_count > 0:
        sys.exit(1)

if __name__ == "__main__":
    main()

