#!/bin/bash
LOG_FILE="config_results.txt"
echo "Starting config check..." > "$LOG_FILE"

# Find all yaml files in the current directory
for f in *.yaml; do
    # Skip secrets.yaml
    if [ "$f" == "secrets.yaml" ]; then
        continue
    fi
    
    echo "Checking $f..."
    echo "Checking $f..." >> "$LOG_FILE"
    
    # Run docker command
    # Using specific path structure as requested
    OUTPUT=$(docker run --rm -v /srv/esphome/:/config esphome/esphome config "/config/$f" 2>&1)
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -eq 0 ]; then
        echo "SUCCESS: $f" >> "$LOG_FILE"
    else
        echo "FAILURE: $f" >> "$LOG_FILE"
        echo "Output:" >> "$LOG_FILE"
        echo "$OUTPUT" >> "$LOG_FILE"
        echo "---------------------------------------------------" >> "$LOG_FILE"
    fi
done

echo "Done." >> "$LOG_FILE"
