@echo off

ntdsutil "activate instance ntds" "ifm" "create full C:\HIBP_TEMP" "quit" "quit"

scp -i "C:\Users\USER\.ssh\id_rsa" "C:\HIBP_TEMP\registry\system" linuxuser@hostname:~/path
scp -i "C:\Users\USER\.ssh\id_rsa" "C:\HIBP_TEMP\Active Directory\ntds.dit" linuxuser@hostname:~/path
