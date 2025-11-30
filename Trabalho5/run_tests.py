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
    try:
        # We need to run inside wsl. 
        # ./mini_js < entrada.txt > codigo
        # But redirection happens in the shell.
        cmd1 = './mini_js < entrada.txt > codigo'
        subprocess.run(cmd1, shell=True, check=True)
        
        # Run interpretador
        # ./mdp/interpretador < codigo
        cmd2 = './mdp/interpretador < codigo'
        result = subprocess.run(cmd2, shell=True, capture_output=True, text=True)
        
        output = result.stdout.strip()
        
        # Normalize line endings and whitespace
        output_lines = [l.strip() for l in output.splitlines() if l.strip()]
        expected_lines = [l.strip() for l in test['expected'].splitlines() if l.strip()]
        
        if output_lines == expected_lines:
            print("PASS")
            return True
        else:
            print("FAIL")
            print("Expected:")
            print('\n'.join(expected_lines))
            print("Got:")
            print('\n'.join(output_lines))
            return False
            
    except subprocess.CalledProcessError as e:
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
