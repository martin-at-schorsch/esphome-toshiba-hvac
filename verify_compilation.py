import os
import subprocess
import glob
from concurrent.futures import ThreadPoolExecutor
import time
import sys

FILES_DIR = "/srv/esphome"
REPORT_FILE = os.path.join(FILES_DIR, "compilation_report.md")
MAX_WORKERS = 4

def check_file(filepath):
    filename = os.path.basename(filepath)
    if filename == "secrets.yaml":
        return None
        
    print(f"Checking {filename}...")
    cmd = [
        "docker", "run", "--rm",
        "-v", "/srv/esphome/:/config",
        "esphome/esphome", "compile",
        f"/config/{filename}"
    ]
    
    start_time = time.time()
    try:
        # Run with timeout
        result = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            timeout=600 # 10 minutes max per file
        )
        duration = time.time() - start_time
        return {
            "file": filename,
            "success": result.returncode == 0,
            "output": result.stdout,
            "duration": duration
        }
    except subprocess.TimeoutExpired:
        return {
            "file": filename,
            "success": False,
            "output": "TIMEOUT",
            "duration": time.time() - start_time
        }
    except Exception as e:
        return {
            "file": filename,
            "success": False,
            "output": str(e),
            "duration": time.time() - start_time
        }

def main():
    yaml_files = glob.glob(os.path.join(FILES_DIR, "*.yaml"))
    
    results = []
    # Create thread pool
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        futures = []
        for f in yaml_files:
            futures.append(executor.submit(check_file, f))
        
        for future in futures:
            res = future.result()
            if res:
                results.append(res)
            
    # Generate Report
    with open(REPORT_FILE, "w") as f:
        f.write("# ESPHome Compilation Report\n\n")
        f.write(f"Date: {time.ctime()}\n\n")
        
        success_count = sum(1 for r in results if r['success'])
        fail_count = len(results) - success_count
        
        f.write(f"## Summary\n")
        f.write(f"- Total Files Checked: {len(results)}\n")
        f.write(f"- Success: {success_count}\n")
        f.write(f"- Failure: {fail_count}\n\n")
        
        f.write("## Details\n")
        # Write failures first
        results.sort(key=lambda x: x['success'])
        
        for r in results:
            status = "✅ PASS" if r['success'] else "❌ FAIL"
            f.write(f"### {status} - {r['file']} ({r['duration']:.1f}s)\n")
            if not r['success']:
                f.write("```\n")
                # Limit output size to keep report manageable, capture end of log
                output = r['output']
                if len(output) > 3000:
                    output = output[-3000:] 
                f.write(output)
                f.write("\n```\n")
    
    print(f"Report written to {REPORT_FILE}")

if __name__ == "__main__":
    main()
