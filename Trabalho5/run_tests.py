import re
import subprocess
import os

def parse_tests(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        content = f.read()

    tests = []
    # Split by "Test " but keep the delimiter
    parts = re.split(r'(^Test \d+:)', content, flags=re.MULTILINE)
    
    current_test = {}
    
    for part in parts:
        if not part.strip():
            continue
            
        if part.startswith("Test "):
            if current_test:
                tests.append(current_test)
            current_test = {'name': part.strip()}
        else:
            # Parse the body
            input_start = part.find('--- Input ---')
            if input_start != -1:
                input_content_start = input_start + len('--- Input ---')
            else:
                input_content_start = 0
                
            expected_start = part.find('--- Expected output')
            if expected_start != -1:
                raw_input = part[input_content_start:expected_start].strip()
                # If we guessed the start, we might have included the test description (starting with #)
                # Filter it out
                lines = raw_input.splitlines()
                if lines and lines[0].strip().startswith('#'):
                    raw_input = '\n'.join(lines[1:]).strip()
                current_test['input'] = raw_input
                
                # Handle variations of expected output header
                rest = part[expected_start + len('--- Expected output'):]
                if rest.startswith(' (exact text)---'):
                    current_test['expected'] = rest[16:].strip()
                elif rest.startswith(' (text)---'):
                    current_test['expected'] = rest[10:].strip()
                elif rest.startswith('---'):
                    current_test['expected'] = rest[3:].strip()
                else:
                    current_test['expected'] = rest.strip()
            else:
                # Missing expected header (Test 11 case)
                # Look for start of expected output: === Console === or Erro:
                match = re.search(r'\n(Erro:|=== Console ===)', part[input_content_start:])
                if match:
                    split_idx = input_content_start + match.start()
                    raw_input = part[input_content_start:split_idx].strip()
                    # Filter #
                    lines = raw_input.splitlines()
                    if lines and lines[0].strip().startswith('#'):
                        raw_input = '\n'.join(lines[1:]).strip()
                    current_test['input'] = raw_input
                    
                    current_test['expected'] = part[split_idx:].strip()
                
    if current_test:
        tests.append(current_test)
        
    return tests

def run_test(test):
    print(f"Running {test['name']}...")
    
    # Write input to entrada.txt
    with open('entrada.txt', 'w', encoding='utf-8') as f:
        f.write(test['input'])
        
    # Run mini_js
    # Run mini_js
    try:
        # Check if we expect a compilation error
        expect_compilation_error = test['expected'].startswith('Erro:') or '=== Erro:' in test['expected']
        
        cmd1 = './mini_js < entrada.txt > codigo'
        # Capture stderr to check for compilation errors
        result_js = subprocess.run(cmd1, shell=True, capture_output=True, text=True)
        
        if result_js.returncode != 0:
            if expect_compilation_error:
                # If we expect an error, check if the stderr matches the expected error
                # The expected error might be in test['expected']
                # But test['expected'] might also contain interpreter output?
                # For Test 11: 
                # Erro: Não é permitido 'return' fora de funções.
                # === Console === ...
                
                # We just check if the error message is present in stderr
                # Or if the expected output STARTS with the error
                
                # Let's just treat it as a pass if we expected an error and got one, 
                # AND the error message is in the expected output.
                
                # Normalize stderr
                stderr_lines = [l.strip() for l in result_js.stderr.splitlines() if l.strip()]
                expected_lines = [l.strip() for l in test['expected'].splitlines() if l.strip()]
                
                # Check if any line in stderr matches the first line of expected (which is usually the error)
                if any(l in expected_lines for l in stderr_lines):
                     print("PASS (Compilation Error as Expected)")
                     return True
                else:
                     print("FAIL (Expected Error Mismatch)")
                     print("Expected one of:")
                     print('\n'.join(expected_lines))
                     print("Got stderr:")
                     print('\n'.join(stderr_lines))
                     return False
            else:
                print(f"Error running mini_js (Exit code {result_js.returncode})")
                print("Stderr:")
                print(result_js.stderr)
                return False
        
        # If mini_js succeeded but we expected an error (and the error was supposed to be from compiler)
        if expect_compilation_error and "=== Erro:" not in test['expected']: 
             # "=== Erro:" usually comes from interpreter (runtime). "Erro:" comes from compiler.
             # Test 11 has "Erro: ..." at start.
             if test['expected'].startswith("Erro:"):
                 print("FAIL (Expected Compilation Error, but mini_js succeeded)")
                 return False

        # Run interpretador
        cmd2 = './mdp/interpretador < codigo'
        result = subprocess.run(cmd2, shell=True, capture_output=True, text=True)
        
        output = result.stdout.strip()
        
        # Normalize line endings and whitespace
        output_lines = [l.strip() for l in output.splitlines() if l.strip()]
        expected_lines = [l.strip() for l in test['expected'].splitlines() if l.strip()]
        
        # Filter output to start from '=== Console ===' or 'Erro:' or '=== Erro:'
        # Find the first line that matches one of these
        start_idx = -1
        for i, line in enumerate(output_lines):
            if line.startswith('=== Console ===') or line.startswith('Erro:') or line.startswith('=== Erro:'):
                start_idx = i
                break
        
        if start_idx != -1:
            filtered_output = output_lines[start_idx:]
        else:
            # If header not found, maybe it's empty output or just code?
            # If expected output is empty, then it's fine.
            if not expected_lines:
                filtered_output = []
            else:
                # If expected output has content but we didn't find header, use all output (likely mismatch)
                filtered_output = output_lines

        if filtered_output == expected_lines:
            print("PASS")
            return True
        else:
            print("FAIL")
            print("Expected:")
            print('\n'.join(expected_lines))
            print("Got:")
            print('\n'.join(filtered_output))
            return False
            
    except Exception as e:
        print(f"Error running test: {e}")
        return False

def main():
    tests = parse_tests('testes.txt')
    passed = 0
    for test in tests:
        if run_test(test):
            passed += 1
            
    print(f"\nPassed {passed}/{len(tests)} tests.")

if __name__ == '__main__':
    main()
