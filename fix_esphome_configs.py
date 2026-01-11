import os
import glob
import re

TARGET_DIR = "/srv/esphome"

def fix_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    modified = False
    new_lines = []
    
    # State tracking
    in_esphome_block = False
    esphome_indent = 0
    
    extracted_platform = None
    extracted_board = None
    
    # Pass 1: Handle esphome platform/board migration
    # We will reconstruct the file, removing platform/board from esphome block
    # and saving them to append a new block later.
    
    # Need to find where the esphome block ends to insert the new block there
    esphome_block_end_index = -1
    
    temp_lines = []
    
    for i, line in enumerate(lines):
        line_content = line.strip()
        
        # Check for esphome block start
        if re.match(r'^esphome:\s*(#.*)?$', line):
            in_esphome_block = True
            temp_lines.append(line)
            continue
            
        if in_esphome_block:
            # Check indentation to see if we are still in block
            current_indent = len(line) - len(line.lstrip())
            
            # If line is not empty and has 0 indentation (and not a comment), we exited the block
            if line_content and not line_content.startswith('#') and current_indent == 0:
                in_esphome_block = False
                esphome_block_end_index = len(temp_lines)
            
            # If inside block, look for legacy keys
            # indent > 0 or empty line
            if in_esphome_block:
                platform_match = re.match(r'^\s+platform:\s*(ESP32|ESP8266)\s*(#.*)?$', line, re.IGNORECASE)
                board_match = re.match(r'^\s+board:\s*(.+?)\s*(#.*)?$', line, re.IGNORECASE)
                
                if platform_match:
                    extracted_platform = platform_match.group(1).upper()
                    print(f"  [Fix] Found legacy platform: {extracted_platform}")
                    modified = True
                    continue # Skip adding this line
                elif board_match:
                    extracted_board = board_match.group(1)
                    print(f"  [Fix] Found legacy board: {extracted_board}")
                    modified = True
                    continue # Skip adding this line
        
        temp_lines.append(line)
        
    # If we reached EOF and were still in esphome block
    if in_esphome_block:
        esphome_block_end_index = len(temp_lines)

    # Insert new platform block if found
    if extracted_platform and extracted_board:
        new_block = []
        new_block.append(f"\n{extracted_platform.lower()}:\n")
        new_block.append(f"  board: {extracted_board}\n")
        if extracted_platform == "ESP32":
             new_block.append("  framework:\n")
             new_block.append("    type: arduino\n")
        
        # Insert after esphome block
        if esphome_block_end_index != -1:
            temp_lines[esphome_block_end_index:esphome_block_end_index] = new_block
        else:
            temp_lines.extend(new_block)
            
    lines = temp_lines
    temp_lines = []
    
    # Pass 2: Handle OTA
    # This is trickier line-by-line. simpler to scan for "ota:" then check next line.
    
    i = 0
    while i < len(lines):
        line = lines[i]
        temp_lines.append(line)
        
        # Detect ota:
        if re.match(r'^ota:\s*(#.*)?$', line):
            # Check if next lines contain platform: esphome
            # We look ahead strictly at indented lines
            has_platform = False
            j = i + 1
            is_empty_ota = True 
            
            while j < len(lines):
                next_line = lines[j]
                stripped = next_line.strip()
                indent = len(next_line) - len(next_line.lstrip())
                
                if not stripped or next_line.strip().startswith("#"):
                    j += 1
                    continue
                
                if indent == 0:
                    # End of ota block
                    break
                
                # Check for platform key
                if re.match(r'^\s*platform:', next_line):
                    has_platform = True
                
                is_empty_ota = False
                j += 1
            
            if not has_platform:
                print("  [Fix] OTA missing platform. Injecting 'platform: esphome'")
                # Inject it right after ota:
                temp_lines.append("  platform: esphome\n")
                modified = True
                
        i += 1

    if modified:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(temp_lines)
        return True
    return False

def main():
    print("Starting ESPHome Config Fixer...")
    yaml_files = glob.glob(os.path.join(TARGET_DIR, "*.yaml"))
    
    fixed_count = 0
    for file in yaml_files:
        filename = os.path.basename(file)
        # Optional: Filter based on compilation_report? No, safe to check all.
        try:
            # print(f"Checking {filename}...")
            if fix_file(file):
                print(f"✅ Fixed {filename}")
                fixed_count += 1
        except Exception as e:
            print(f"❌ Error processing {filename}: {e}")
            
    print(f"\nFinished! Total files modified: {fixed_count}")

if __name__ == "__main__":
    main()
