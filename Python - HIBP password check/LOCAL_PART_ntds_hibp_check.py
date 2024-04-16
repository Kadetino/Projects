#!/usr/bin/env python3
import subprocess
import requests
import argparse
from datetime import date

########################################################################################################
# Скрипт для проверки NTLM-хэшей из NTDS.dit через API HaveIBeenPwned для обнаружения утёкших паролей. #
########################################################################################################

# Запуск скрипта с аргументами из CLI
parser = argparse.ArgumentParser(description='Example usage: ./ntds_hibp_check.py -o output.txt -sep ";" --disable_stdout')

parser.add_argument('-ntds', '--ntds_path', type=str, default='./ntds.dit', help='Path to NTDS.dit file. Assumed default path: \"./ntds.dit\".')
parser.add_argument('-system', '--system_path', type=str, default='./system', help='Path to HKLM/SYSTEM file. Assumed default path: \"./system\".')
parser.add_argument('-o', '--output_path', type=str, default='./results.txt', help='Path for the file with results. Assumed default path: \"./results.txt\".')
parser.add_argument('-sep', '--separator', type=str, default='\t', help='Separator for the output.')
parser.add_argument('--print_non_breached', action='store_true', help='Print non-breached users.')
parser.add_argument('--disable_stdout', action='store_true', help='Disable printing results information to stdout.')

args = parser.parse_args()

# Инициализация переменных
temp_foldername = './output_temp'
output_filename = f"{temp_foldername}/output"

def_sep = args.separator
print_safe_userdata = args.print_non_breached
stdout_disabled = args.disable_stdout

output_path = args.output_path
ntds_path = args.ntds_path
system_path = args.system_path

#####################
hibp_ntlm_hashes_name = f"./ntlm.txt"
PARSED_HASHES_FOLDER = "./parsed_ntlm_folder"
# # Скачать ntlm-хэши через утилиту создателя HIBP https://github.com/HaveIBeenPwned/PwnedPasswordsDownloader
# # 05/04/2024 - ~32GB, 931'856'448 строк
# subprocess.run(f"haveibeenpwned-downloader -n {hibp_ntlm_hashes_name}", shell=True, capture_output=True, text=True);
# 
# 
# HIBP_NTLM_FILE = './ntlm.txt.txt'
# PARSED_HASHES_FOLDER = "./parsed_ntlm_folder"
# subprocess.run(f"mkdir {PARSED_HASHES_FOLDER}", shell=True, capture_output=True, text=True);
# 
# counter = 0
# output_files = {} # Dictionary to hold open file objects
# 
# # Разделить полученный огромный файл на более маленькие (~1'048'576 файлов)
# with open(HIBP_NTLM_FILE, 'r') as file:
#     for line in file:
#         current_line = line.rstrip()
#         prefix = current_line[:5]
#       
#         # Open the output file if it's not already open
#         if prefix not in output_files:
#             output_files[prefix] = open(f'{PARSED_HASHES_FOLDER}/{prefix}.txt', 'a')
#       
#         # Write to the appropriate file
#         output_files[prefix].write(f'{current_line}\n')
# 
#         # [Errno 24] Too many open files - dodge
#         counter += 1
#         if counter >=1000:
#             for output_file in output_files.values():
#                 output_file.close()
#             output_files = {}
#             counter = 0 
#             
# 
# # Close all open files
# for output_file in output_files.values():
#     output_file.close()
# 
# 
# 
# 
# 

# 
# 
# #######################
# Временная папка для результатов Impacket
subprocess.run(f"mkdir {temp_foldername}", shell=True, capture_output=True, text=True);
 
 
# Имея ntds.dit и HKLM/SYSTEM, достать хэши с помощью Impacket secretsdump.py
print(f"{date.today()} [*] Extracting data from NTDS.dit ...")
subprocess.run(f"secretsdump.py -ntds {ntds_path} -system {system_path} -hashes lmhash:nthash LOCAL -outputfile {output_filename}",
               shell=True,
               capture_output=True,
               text=True)

# Открыть файл с извлечёнными хэшами
print(f"{date.today()} [*] Reading extracted data ...")
with open (f"{output_filename}.ntds", "r") as file:
    # Формат прочтённых строк: domain\uid:rid:lmhash:nthash:::
    data = file.readlines()
    data = list(temp.split(sep=":") for temp in data)
    data = list(temp[:-3:] for temp in data)

# Удалить временную папку    
subprocess.run(f"rm -r {temp_foldername}", shell=True, capture_output=True, text=True)  
    
# Открыть файл для записи результата
print(f"{date.today()} [*] Checking hashes with HaveIBeenPwned locally downloaded data ...")
if not stdout_disabled:
    print(f"Results are:\nUser{def_sep}Pwned{def_sep}Times breached")
with open (output_path, "w") as file:
    file.write(f"user{def_sep}passwd_is_pwned{def_sep}times")
    for item in data:
        hash_string = item[-1].upper()
        username = item[0]
        prefix = hash_string[0:5]
        
        # # Запрос к API с хэшами паролей HIBP 
        #req = requests.get(f"https://api.pwnedpasswords.com/range/{prefix}?mode=ntlm").content.decode('utf-8')
        
        hashes_ntlm = []
        with open(f'{PARSED_HASHES_FOLDER}/{prefix}.txt', 'r') as file_hashes:
            hashes_ntlm = file_hashes.readlines()
            hashes_ntlm = {entry.split(':')[0]: int(entry.split(':')[1]) for entry in hashes_ntlm}
            
        # Преобразовать ответ HIBP в dict с парами (хэш: количество_раз)
        #hashes = dict(t.split(":") for t in req.split('\r\n'))
        #hashes = dict((prefix + key, value) for (key, value) in hashes.items()) # Добавить первые 5 символов хэша к остальным
        hashes = hashes_ntlm
        
        # Проверить есть ли этот хэш у HIBP
        if hash_string in hashes_ntlm.keys():
            # Breached password
            file.write(f"\n{username}{def_sep}yes{def_sep}{hashes[hash_string]}")
            
            if not stdout_disabled:
                print(f"{username}{def_sep}yes{def_sep}{hashes[hash_string]}")
        else:
            # Non-breached password
            if print_safe_userdata:
                file.write(f"\n{username}{def_sep}no{def_sep}0")
                
                if not stdout_disabled:
                    print(f"{username}{def_sep}no{def_sep}0")

# Уведомить об окончании проверки
print(f"{date.today()} [*] Done.")
