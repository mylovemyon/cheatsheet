```
Usage:
./polenum.py -u <username> -p <password> -d <domain/ip> --protocols <protocols>

    Available protocols: ['445/SMB', '139/SMB']

example: polenum aaa:bbb@127.0.0.1
```
TryHackMeの[AttacktiveDirectory](https://tryhackme.com/r/room/attacktivedirectory)でお試し
```zsh
└─$ polenum backup:backup2517860@10.10.167.255


[+] Attaching to 10.10.167.255 using backup:backup2517860

[+] Trying protocol 139/SMB...

        [!] Protocol failed: Cannot request session (Called Name:10.10.167.255)

[+] Trying protocol 445/SMB...

[+] Found domain(s):

        [+] THM-AD
        [+] Builtin

[+] Password Info for Domain: THM-AD

        [+] Minimum password length: None
        [+] Password history length: None
        [+] Maximum password age: 41 days 23 hours 53 minutes 
        [+] Password Complexity Flags: 000000

                [+] Domain Refuse Password Change: 0
                [+] Domain Password Store Cleartext: 0
                [+] Domain Password Lockout Admins: 0
                [+] Domain Password No Clear Change: 0
                [+] Domain Password No Anon Change: 0
                [+] Domain Password Complex: 0

        [+] Minimum password age: 1 day 4 minutes 
        [+] Reset Account Lockout Counter: 30 minutes 
        [+] Locked Account Duration: 30 minutes 
        [+] Account Lockout Threshold: None
        [+] Forced Log off Time: Not Set
```
