lines = open('mini_js.y', 'r').readlines()

new_block = """       if (capturas.size() > 0) {
           $$.c += "'captura'";
           $$.c += "{}";
           for(const string& cap : capturas) {
               if(cap == "print" && busca_simbolo("print") == nullptr) continue;
               $$.c += "'" + cap + "'";
               $$.c += cap;
               $$.c += "@";
               $$.c += "[<=]";
           }
           $$.c += "[<=]";
       }
"""

# Rule 2: 625-631 (0-indexed: 624-631)
# Check if lines match expected old block
if "for(const string& cap : capturas)" in lines[624]:
    print("Patching Rule 2")
    # Replace 7 lines with new block (which is 12 lines)
    del lines[624:631]
    lines.insert(624, new_block)
else:
    print(f"Rule 2 mismatch: {lines[624]}")

# Rule 1: 572-578 (0-indexed: 571-578)
if "for(const string& cap : capturas)" in lines[571]:
    print("Patching Rule 1")
    del lines[571:578]
    lines.insert(571, new_block)
else:
    print(f"Rule 1 mismatch: {lines[571]}")

with open('mini_js.y', 'w') as f:
    f.writelines(lines)
