import subprocess


HIBP_NTLM_FILE = './ntlm.txt.txt'
PARSED_HASHES_FOLDER = "./parsed_ntlm_folder"
subprocess.run(f"mkdir {PARSED_HASHES_FOLDER}", shell=True, capture_output=True, text=True);
counter = 0
output_files = {} # Dictionary to hold open file objects

with open(HIBP_NTLM_FILE, 'r') as file:
    for line in file:
        current_line = line.rstrip()
        prefix = current_line[:5]
        
        # Open the output file if it's not already open
        if prefix not in output_files:
            output_files[prefix] = open(f'{PARSED_HASHES_FOLDER}/{prefix}.txt', 'a')
        
        # Write to the appropriate file
        output_files[prefix].write(f'{current_line}\n')

        # [Errno 24] Too many open files - dodge
        counter += 1
        if counter >=1000:
            for output_file in output_files.values():
                output_file.close()
            output_files = {}
            counter = 0 
            

# Close all open files
for output_file in output_files.values():
    output_file.close()
