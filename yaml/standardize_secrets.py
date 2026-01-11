import os
import glob
import re

TARGET_DIR = "/srv/esphome"

def update_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    modified = False
    new_lines = []
    
    current_block = None # 'wifi' or 'mqtt'
    block_indent = -1
    first_prop_indent = -1
    
    for line in lines:
        stripped = line.strip()
        current_indent = len(line) - len(line.lstrip())
        
        # Check for block entry
        # We look for top-level or specific keys
        if re.match(r'^wifi:\s*(#.*)?$', stripped):
            current_block = 'wifi'
            block_indent = current_indent
            first_prop_indent = -1
            new_lines.append(line)
            continue
        elif re.match(r'^mqtt:\s*(#.*)?$', stripped):
            current_block = 'mqtt'
            block_indent = current_indent
            first_prop_indent = -1
            new_lines.append(line)
            continue
            
        if current_block:
            # Check if we exited the block
            if stripped and not stripped.startswith('#'):
                if current_indent <= block_indent:
                    current_block = None
                    block_indent = -1
                    first_prop_indent = -1
            
        if current_block:
            # We are inside a block
            if stripped and not stripped.startswith('#'):
                # Determine property indentation level from the first key we see
                if first_prop_indent == -1:
                    first_prop_indent = current_indent
                
                # Only process keys at the immediate child level
                if current_indent == first_prop_indent:
                    indent_str = line[:current_indent]
                    
                    if current_block == 'wifi':
                        if re.match(r'^ssid:', stripped):
                            new_val = "!secret wifi_ssid"
                            if new_val not in line:
                                print(f"  [{os.path.basename(filepath)}] Updating wifi ssid")
                                new_lines.append(f"{indent_str}ssid: {new_val}\n")
                                modified = True
                                continue
                        elif re.match(r'^password:', stripped):
                            new_val = "!secret wifi_password"
                            if new_val not in line:
                                print(f"  [{os.path.basename(filepath)}] Updating wifi password")
                                new_lines.append(f"{indent_str}password: {new_val}\n")
                                modified = True
                                continue
                                
                    elif current_block == 'mqtt':
                        if re.match(r'^broker:', stripped):
                            new_val = "!secret mqtt_host"
                            if new_val not in line:
                                print(f"  [{os.path.basename(filepath)}] Updating mqtt broker")
                                new_lines.append(f"{indent_str}broker: {new_val}\n")
                                modified = True
                                continue
                        elif re.match(r'^username:', stripped):
                            new_val = "!secret mqtt_username"
                            if new_val not in line:
                                print(f"  [{os.path.basename(filepath)}] Updating mqtt username")
                                new_lines.append(f"{indent_str}username: {new_val}\n")
                                modified = True
                                continue
                        elif re.match(r'^password:', stripped):
                            new_val = "!secret mqtt_password"
                            if new_val not in line:
                                print(f"  [{os.path.basename(filepath)}] Updating mqtt password")
                                new_lines.append(f"{indent_str}password: {new_val}\n")
                                modified = True
                                continue

        new_lines.append(line)

    if modified:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(new_lines)
        return True
    return False

def main():
    print("Starting Secret Standardization...")
    # Skip secrets.yaml itself obviously
    yaml_files = [f for f in glob.glob(os.path.join(TARGET_DIR, "*.yaml")) if "secrets.yaml" not in f]
    
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
