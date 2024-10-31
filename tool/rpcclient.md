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
IPC$についての参考  
WindowsXP以前まではIPC$へのNullセッションができたので、脆弱な部分があったらしい
以降は、Null接続はできるが権限はもたないようになっている

[IPC$:プログラム間の通信に必要な名前付きパイプを共有するリソースで削除できない](https://learn.microsoft.com/ja-jp/troubleshoot/windows-server/networking/remove-administrative-shares)  
[Windows での IPC$ 共有と null セッション動作](https://learn.microsoft.com/ja-jp/troubleshoot/windows-server/networking/inter-process-communication-share-null-session)  
[What is the IPC$ Share and Why Should You Care?](https://dec-solutions.com/what-is-the-ipc-share-and-why-should-you-care/)  
[Description of the IPC$ share](https://smallvoid.com/article/winnt-ipc-share.html)  
[Windows IPC$ share behaviour](https://community.spiceworks.com/t/windows-ipc-share-behaviour/741818)  
[The Hacker Recipes](https://www.thehacker.recipes/ad/recon/ms-rpc)  
[Listing remote named pipes](https://www.outflank.nl/blog/2023/10/19/listing-remote-named-pipes/)
