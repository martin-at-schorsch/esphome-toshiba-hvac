import os
import glob
import re

TARGET_DIR = "/srv/esphome"

def update_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    modified = False
    new_lines = []
    
    in_api_block = False
    api_indent = -1
    
    for line in lines:
        stripped = line.strip()
        current_indent = len(line) - len(line.lstrip())
        
        # Detect api: block
        if re.match(r'^api:\s*(#.*)?$', stripped):
            in_api_block = True
            api_indent = current_indent
            new_lines.append(line)
            continue
            
        if in_api_block:
            # Check if we are still in block (indentation must be deeper)
            if not stripped: # Empty line
                new_lines.append(line)
                continue
            
            if stripped.startswith('#'): # Comment
                new_lines.append(line)
                continue
                
            if current_indent <= api_indent:
                # We exited the block
                in_api_block = False
                api_indent = -1
                # Fall through to process this line normally
            else:
                # We are inside api block
                # Check for password:
                pass_match = re.match(r'^password:\s*(.+)$', stripped)
                if pass_match:
                    password_val = pass_match.group(1).strip()
                    # Ensure quotes if not present (optional but good for keys)
                    if not (password_val.startswith('"') and password_val.endswith('"')) and \
                       not (password_val.startswith("'") and password_val.endswith("'")):
                        password_val = f'"{password_val}"'
                        
                    print(f"  [Update] Converting password to encryption key in {os.path.basename(filepath)}")
                    
                    # Construct new lines
                    # Preserving indentation of the password line for 'encryption'
                    # checking indentation delta
                    indent_str = line[:current_indent]
                    
                    new_lines.append(f"{indent_str}encryption:\n")
                    new_lines.append(f"{indent_str}  key: {password_val}\n")
                    modified = True
                    continue # Skip adding the original password line
        
        new_lines.append(line)

    if modified:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(new_lines)
        return True
    return False

def main():
    print("Starting API Config Update...")
    yaml_files = glob.glob(os.path.join(TARGET_DIR, "*.yaml"))
    
    count = 0
    for file in yaml_files:
        try:
            if update_file(file):
                 count += 1
        except Exception as e:
            print(f"Error checking {file}: {e}")
            
    print(f"Updated {count} files.")

if __name__ == "__main__":
    main()
