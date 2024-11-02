## help
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

## -c
TryHackmeの[Attacktive Directory](https://tryhackme.com/r/room/attacktivedirectory)でお試し  
backupユーザはドメインユーザ権限のみをもつ（Domain Admin権限はなし）

### lsarpc
#### lsaquery
ドメイン名やドメインSIDを取得
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "lsaquery" 10.10.165.140
Domain Name: THM-AD
Domain Sid: S-1-5-21-3591857110-2884097990-301047963
```
#### lookupsids
SID からユーザー名を解決
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "lookupsids S-1-5-21-3591857110-2884097990-301047963-500" 10.10.165.140 
S-1-5-21-3591857110-2884097990-301047963-500 THM-AD\Administrator (1)
```
#### lookupsids3
lookupsidsコマンドと同じ機能らしいが、Administrator権限でもアクセス拒否され実行されなかった。
```zsh
└─$ rpcclient -U "thm-ad/administrator"%"0e0363213e37b94221497260b0bcb4fc" --pw-nt-hash -c "lookupsids3 S-1-5-21-3591857110-2884097990-301047963-500" 10.10.165.140 
result was NT_STATUS_ACCESS_DENIED
```
#### lookupnames
ユーザー名から SID を解決
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "lookupnames administrator" 10.10.165.140 
administrator S-1-5-21-3591857110-2884097990-301047963-500 (User: 1)
```
#### lookupnames4
lookupnamesコマンドと同じ機能らしいが、Administrator権限でもアクセス拒否され実行されなかった。
```zsh
└─$ rpcclient -U "thm-ad/administrator"%"0e0363213e37b94221497260b0bcb4fc" --pw-nt-hash  -c "lookupnames4 administrator" 10.10.165.140 
result was NT_STATUS_ACCESS_DENIED
```
#### enumtrust
信頼するドメインの一覧を表示
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "enumtrust" 10.10.165.140 

```
#### enumprivs
ユーザー権利の一覧を表示
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "enumprivs" 10.10.165.140 
found 35 privileges

SeCreateTokenPrivilege          0:2 (0x0:0x2)
SeAssignPrimaryTokenPrivilege           0:3 (0x0:0x3)
SeLockMemoryPrivilege           0:4 (0x0:0x4)
SeIncreaseQuotaPrivilege                0:5 (0x0:0x5)
SeMachineAccountPrivilege               0:6 (0x0:0x6)
SeTcbPrivilege          0:7 (0x0:0x7)
SeSecurityPrivilege             0:8 (0x0:0x8)
SeTakeOwnershipPrivilege                0:9 (0x0:0x9)
SeLoadDriverPrivilege           0:10 (0x0:0xa)
SeSystemProfilePrivilege                0:11 (0x0:0xb)
SeSystemtimePrivilege           0:12 (0x0:0xc)
SeProfileSingleProcessPrivilege                 0:13 (0x0:0xd)
SeIncreaseBasePriorityPrivilege                 0:14 (0x0:0xe)
SeCreatePagefilePrivilege               0:15 (0x0:0xf)
SeCreatePermanentPrivilege              0:16 (0x0:0x10)
SeBackupPrivilege               0:17 (0x0:0x11)
SeRestorePrivilege              0:18 (0x0:0x12)
SeShutdownPrivilege             0:19 (0x0:0x13)
SeDebugPrivilege                0:20 (0x0:0x14)
SeAuditPrivilege                0:21 (0x0:0x15)
SeSystemEnvironmentPrivilege            0:22 (0x0:0x16)
SeChangeNotifyPrivilege                 0:23 (0x0:0x17)
SeRemoteShutdownPrivilege               0:24 (0x0:0x18)
SeUndockPrivilege               0:25 (0x0:0x19)
SeSyncAgentPrivilege            0:26 (0x0:0x1a)
SeEnableDelegationPrivilege             0:27 (0x0:0x1b)
SeManageVolumePrivilege                 0:28 (0x0:0x1c)
SeImpersonatePrivilege          0:29 (0x0:0x1d)
SeCreateGlobalPrivilege                 0:30 (0x0:0x1e)
SeTrustedCredManAccessPrivilege                 0:31 (0x0:0x1f)
SeRelabelPrivilege              0:32 (0x0:0x20)
SeIncreaseWorkingSetPrivilege           0:33 (0x0:0x21)
SeTimeZonePrivilege             0:34 (0x0:0x22)
SeCreateSymbolicLinkPrivilege           0:35 (0x0:0x23)
SeDelegateSessionUserImpersonatePrivilege               0:36 (0x0:0x24)
```
#### lsaenumsid
LSA の SID の一覧を表示
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "lsaenumsid" 10.10.165.140 
found 20 SIDs

S-1-5-90-0
S-1-5-9
S-1-5-82-3006700770-424185619-1745488364-794895919-4004696415
S-1-5-80-3139157870-2983391045-3678747466-658725712-1809340420
S-1-5-80-0
S-1-5-6
S-1-5-32-568
S-1-5-32-559
S-1-5-32-554
S-1-5-32-551
S-1-5-32-550
S-1-5-32-549
S-1-5-32-545
S-1-5-32-544
S-1-5-21-3591857110-2884097990-301047963-513
S-1-5-21-3591857110-2884097990-301047963-512
S-1-5-20
S-1-5-19
S-1-5-11
S-1-1-0
```

### samr
#### queryuser
ユーザー情報を問い合わせる。コマンドではユーザ名でもRIDでも問い合わせる。
```
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "queryuser administrator" 10.10.165.140 
        User Name   :   Administrator
        Full Name   :
        Home Drive  :
        Dir Drive   :
        Profile Path:
        Logon Script:
        Description :   Built-in account for administering the computer/domain
        Workstations:
        Comment     :
        Remote Dial :
        Logon Time               :      Sat, 02 Nov 2024 03:26:28 EDT
        Logoff Time              :      Wed, 31 Dec 1969 19:00:00 EST
        Kickoff Time             :      Wed, 31 Dec 1969 19:00:00 EST
        Password last set Time   :      Thu, 17 Sep 2020 18:53:29 EDT
        Password can change Time :      Fri, 18 Sep 2020 18:53:29 EDT
        Password must change Time:      Wed, 13 Sep 30828 22:48:05 EDT
        unknown_2[0..31]...
        user_rid :      0x1f4
        group_rid:      0x201
        acb_info :      0x00000210
        fields_present: 0x00ffffff
        logon_divs:     168
        bad_password_count:     0x00000000
        logon_count:    0x0000000f
        padding1[0..7]...
        logon_hrs[0..21]...
```
#### querygroup
グループ情報を問い合わせる
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "querygroup 0x201" 10.10.165.140 
        Group Name:     Domain Users
        Description:    All domain users
        Group Attribute:7
        Num Members:16
```
#### queryusergroups
ユーザーが所属するグループを問い合わせる
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "queryusergroups 0x1f4" 10.10.165.140 
        group rid:[0x207] attr:[0x7]
        group rid:[0x201] attr:[0x7]
        group rid:[0x208] attr:[0x7]
        group rid:[0x200] attr:[0x7]
        group rid:[0x206] attr:[0x7]
```
#### querydominfo
ドメイン情報を問い合わせる  
ユーザ数が52のと分かる
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "querydominfo" 10.10.165.140                      
Domain:         THM-AD
Server:
Comment:
Total Users:    52
Total Groups:   0
Total Aliases:  16
Sequence No:    1
Force Logoff:   -1
Domain Server State:    0x1
Server Role:    ROLE_DOMAIN_PDC
Unknown 3:      0x1
```
#### enumdomusers
ドメインユーザーを一覧表示
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "enumdomusers" 10.10.165.140 
user:[Administrator] rid:[0x1f4]
user:[Guest] rid:[0x1f5]
user:[krbtgt] rid:[0x1f6]
user:[skidy] rid:[0x44f]
user:[breakerofthings] rid:[0x450]
user:[james] rid:[0x451]
user:[optional] rid:[0x452]
user:[sherlocksec] rid:[0x453]
user:[darkstar] rid:[0x454]
user:[Ori] rid:[0x455]
user:[robin] rid:[0x456]
user:[paradox] rid:[0x457]
user:[Muirland] rid:[0x458]
user:[horshark] rid:[0x459]
user:[svc-admin] rid:[0x45a]
user:[backup] rid:[0x45e]
user:[a-spooks] rid:[0x641]
```
#### enumdomgroups
ドメイングループを一覧表示
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "enumdomgroups" 10.10.165.140 
group:[Enterprise Read-only Domain Controllers] rid:[0x1f2]
group:[Domain Admins] rid:[0x200]
group:[Domain Users] rid:[0x201]
group:[Domain Guests] rid:[0x202]
group:[Domain Computers] rid:[0x203]
group:[Domain Controllers] rid:[0x204]
group:[Schema Admins] rid:[0x206]
group:[Enterprise Admins] rid:[0x207]
group:[Group Policy Creator Owners] rid:[0x208]
group:[Read-only Domain Controllers] rid:[0x209]
group:[Cloneable Domain Controllers] rid:[0x20a]
group:[Protected Users] rid:[0x20d]
group:[Key Admins] rid:[0x20e]
group:[Enterprise Key Admins] rid:[0x20f]
group:[DnsUpdateProxy] rid:[0x44e]
group:[dc] rid:[0x45d]
```

### a
```zsh
└─$ rpcclient -U "thm-ad/backup"%"backup2517860" -c "querydispinfo" 10.10.165.140 
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

https://www.hackingarticles.in/active-directory-enumeration-rpcclient/
### IPC$について  
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
