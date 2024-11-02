```
Usage: rpcclient [OPTION...] BINDING-STRING|HOST
Options:
  -c, --command=COMMANDS                       Execute semicolon separated cmds
  -I, --dest-ip=IP                             Specify destination IP address
  -p, --port=PORT                              Specify port number

Help options:
  -?, --help                                   Show this help message
      --usage                                  Display brief usage message

Common Samba options:
  -d, --debuglevel=DEBUGLEVEL                  Set debug level
      --debug-stdout                           Send debug output to standard output
  -s, --configfile=CONFIGFILE                  Use alternative configuration file
      --option=name=value                      Set smb.conf option from command line
  -l, --log-basename=LOGFILEBASE               Basename for log/debug files
      --leak-report                            enable talloc leak reporting on exit
      --leak-report-full                       enable full talloc leak reporting on exit

Connection options:
  -R, --name-resolve=NAME-RESOLVE-ORDER        Use these name resolution services only
  -O, --socket-options=SOCKETOPTIONS           socket options to use
  -m, --max-protocol=MAXPROTOCOL               Set max protocol level
  -n, --netbiosname=NETBIOSNAME                Primary netbios name
      --netbios-scope=SCOPE                    Use this Netbios scope
  -W, --workgroup=WORKGROUP                    Set the workgroup name
      --realm=REALM                            Set the realm name

Credential options:
  -U, --user=[DOMAIN/]USERNAME[%PASSWORD]      Set the network username
  -N, --no-pass                                Don't ask for a password
      --password=STRING                        Password
      --pw-nt-hash                             The supplied password is the NT hash
  -A, --authentication-file=FILE               Get the credentials from a file
  -P, --machine-pass                           Use stored machine account password
      --simple-bind-dn=DN                      DN to use for a simple bind
      --use-kerberos=desired|required|off      Use Kerberos authentication
      --use-krb5-ccache=CCACHE                 Credentials cache location for Kerberos
      --use-winbind-ccache                     Use the winbind ccache for authentication
      --client-protection=sign|encrypt|off     Configure used protection for client connections

Deprecated legacy options:
  -k, --kerberos                               DEPRECATED: Migrate to --use-kerberos

Version options:
  -V, --version                                Print version
```
TryHackmeのAttacktive Directoryで実施
```
└─$ rpcclient -W "spookysec.local" -U "backup"%"backup2517860" -c "lsaquery" 10.10.165.140
Domain Name: THM-AD
Domain Sid: S-1-5-21-3591857110-2884097990-301047963
```
TryHackmeのAttacktive Directoryで実施
```
└─$ rpcclient -W "spookysec.local" -U "backup"%"backup2517860" -c "querydispinfo" 10.10.165.140 
index: 0xfc1 RID: 0x641 acb: 0x00000210 Account: a-spooks       Name: Admin Spooks      Desc: (null)
index: 0xeda RID: 0x1f4 acb: 0x00000210 Account: Administrator  Name: (null)    Desc: Built-in account for administering the computer/domain
index: 0xfc0 RID: 0x45e acb: 0x00000210 Account: backup Name: backup    Desc: (null)
index: 0xfae RID: 0x450 acb: 0x00000210 Account: breakerofthings        Name: Ashu BreakerOfThings      Desc: (null)
index: 0xfb2 RID: 0x454 acb: 0x00000210 Account: darkstar       Name: Jon Darkstar      Desc: (null)
index: 0xedb RID: 0x1f5 acb: 0x00000215 Account: Guest  Name: (null)    Desc: Built-in account for guest access to the computer/domain
index: 0xfb7 RID: 0x459 acb: 0x00000210 Account: horshark       Name: horshark  Desc: (null)
index: 0xfaf RID: 0x451 acb: 0x00000210 Account: james  Name: James Ninja       Desc: (null)
index: 0xf0f RID: 0x1f6 acb: 0x00020011 Account: krbtgt Name: (null)    Desc: Key Distribution Center Service Account
index: 0xfb6 RID: 0x458 acb: 0x00000210 Account: Muirland       Name: Muirland Oracle   Desc: (null)
index: 0xfb0 RID: 0x452 acb: 0x00000210 Account: optional       Name: root optional     Desc: (null)
index: 0xfb3 RID: 0x455 acb: 0x00000210 Account: Ori    Name: Ori       Desc: (null)
index: 0xfb5 RID: 0x457 acb: 0x00000210 Account: paradox        Name: Par Adox  Desc: (null)
index: 0xfb4 RID: 0x456 acb: 0x00000210 Account: robin  Name: Robin     Desc: (null)
index: 0xfb1 RID: 0x453 acb: 0x00000210 Account: sherlocksec    Name: Dan SherlockSec   Desc: (null)
index: 0xfad RID: 0x44f acb: 0x00000210 Account: skidy  Name: Ben Skidy Desc: (null)
index: 0xfb8 RID: 0x45a acb: 0x00010210 Account: svc-admin      Name: svc admin Desc: (null)
```

IPC$について  
WindowsXP以前まではIPC$すなわちNullセッションから特定の操作ができた。  
以降は、Nullセッションはできるが権限はもたないようにデフォルトで設定されている（ポリシーやレジストリで）。  
プログラム間通信の名前付きパイプのために使用され、恒久的にIPC$を削除することはできない。  
rpcでもIPC$を使用する。詳細は[こちら](https://sensepost.com/blog/2018/a-new-look-at-null-sessions-and-user-enumeration/)

[IPC$:プログラム間の通信に必要な名前付きパイプを共有するリソースで削除できない](https://learn.microsoft.com/ja-jp/troubleshoot/windows-server/networking/remove-administrative-shares)  
[Windows での IPC$ 共有と null セッション動作](https://learn.microsoft.com/ja-jp/troubleshoot/windows-server/networking/inter-process-communication-share-null-session)  
[What is the IPC$ Share and Why Should You Care?](https://dec-solutions.com/what-is-the-ipc-share-and-why-should-you-care/)  
[Description of the IPC$ share](https://smallvoid.com/article/winnt-ipc-share.html)  
[Listing remote named pipes](https://www.outflank.nl/blog/2023/10/19/listing-remote-named-pipes/)  
[MS-RPC とそのセキュリティメカニズムの概要](https://www.akamai.com/ja/blog/security-research/msrpc-security-mechanisms)  
[MS-RPC](https://www.thehacker.recipes/ad/recon/ms-rpc)  
