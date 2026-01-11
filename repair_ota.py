import os
import glob
import re

TARGET_DIR = "/srv/esphome"

def repair_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    modified = False
    new_lines = []
    
    i = 0
    while i < len(lines):
        line = lines[i]
        
        # Check for the pattern created by the previous bug:
        # ota:
        #   platform: esphome
        #   - platform: esphome (or similar list item)
        
        if re.match(r'^\s*platform: esphome\s*$', line):
            # Check next line
            if i + 1 < len(lines):
                next_line = lines[i+1]
                # If next line starts with a list item containing platform
                if re.match(r'^\s*-\s+platform:', next_line):
                    print(f"  [Repair] Removing redundant 'platform: esphome' before list in {os.path.basename(filepath)}")
                    modified = True
                    i += 1 # Skip this line
                    continue
        
        new_lines.append(line)
        i += 1

    if modified:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(new_lines)
        return True
    return False

def main():
    print("Starting OTA Config Repair...")
    yaml_files = glob.glob(os.path.join(TARGET_DIR, "*.yaml"))
    
    count = 0
    for file in yaml_files:
        try:
            if repair_file(file):
                 count += 1
        except Exception as e:
            print(f"Error checking {file}: {e}")
            
    print(f"Repaired {count} files.")

if __name__ == "__main__":
    main()
