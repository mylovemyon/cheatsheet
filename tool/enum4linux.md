https://github.com/CiscoCXSecurity/enum4linux  

## help
```
enum4linux v0.9.1 (http://labs.portcullis.co.uk/application/enum4linux/)
Copyright (C) 2011 Mark Lowe (mrl@portcullis-security.com)

Simple wrapper around the tools in the samba package to provide similar 
functionality to enum.exe (formerly from www.bindview.com).  Some additional 
features such as RID cycling have also been added for convenience.

Usage: ./enum4linux.pl [options] ip

Options are (like "enum"):
    -U        get userlist
    -M        get machine list*
    -S        get sharelist
    -P        get password policy information
    -G        get group and member list
    -d        be detailed, applies to -U and -S
    -u user   specify username to use (default "")  
    -p pass   specify password to use (default "")   

The following options from enum.exe aren't implemented: -L, -N, -D, -f

Additional options:
    -a        Do all simple enumeration (-U -S -G -P -r -o -n -i).
              This option is enabled if you don't provide any other options.
    -h        Display this help message and exit
    -r        enumerate users via RID cycling
    -R range  RID ranges to enumerate (default: 500-550,1000-1050, implies -r)
    -K n      Keep searching RIDs until n consective RIDs don't correspond to
              a username.  Impies RID range ends at 999999. Useful 
              against DCs.
    -l        Get some (limited) info via LDAP 389/TCP (for DCs only)
    -s file   brute force guessing for share names
    -k user   User(s) that exists on remote system (default: administrator,guest,krbtgt,domain admins,root,bin,none)
              Used to get sid with "lookupsid known_username"
              Use commas to try several users: "-k admin,user1,user2"
    -o        Get OS information
    -i        Get printer information
    -w wrkg   Specify workgroup manually (usually found automatically)
    -n        Do an nmblookup (similar to nbtstat)
    -v        Verbose.  Shows full commands being run (net, rpcclient, etc.)
    -A        Aggressive. Do write checks on shares etc

RID cycling should extract a list of users from Windows (or Samba) hosts 
which have RestrictAnonymous set to 1 (Windows NT and 2000), or "Network 
access: Allow anonymous SID/Name translation" enabled (XP, 2003).

NB: Samba servers often seem to have RIDs in the range 3000-3050.

Dependancy info: You will need to have the samba package installed as this 
script is basically just a wrapper around rpcclient, net, nmblookup and 
smbclient.  Polenum from http://labs.portcullis.co.uk/application/polenum/ 
is required to get Password Policy info.
```
v、u、p、w以外のオプションがないと、aオプションが設定される
```perl
# Enable -a if no other options (apart from -v, -u, -p or -w) are given
unless (scalar( grep { $_ ne 'v' && $_ ne 'u' && $_ ne 'p' && $_ ne 'w' } keys %opts)) {
	$opts{'a'} = 1;
}
```

## デフォルト 
オプション指定しなくても実行される関数がある。
```perl
# Basic enumeration, check session
get_workgroup();
get_nbtstat()          if $opts{'n'};
make_session();
get_ldapinfo()         if $opts{'l'};
get_domain_sid();
get_os_info()          if $opts{'o'};
```
### [get_workgroup()](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L384)
```perl
$global_workgroup = `nmblookup -A '$global_target'`; # Global var.  Erg!
```
`-w`オプションで手動でドメインが指定されていない場合は、`nmblookup`でドメインを取得する。  
nmblookupで取得できない場合は、`dig`で取得するがそれでも取得できない場合はドメイン名に「WORKGROUP」を設定する。
### [make_session()](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L452)
```perl
my $command = "smbclient -W '$global_workgroup' //'$global_target'/ipc\$ -U'$global_username'\%'$global_password' -c 'help' 2>&1";
```
`smbclient`でNullセッションか指定したクレデンシャルでIPC$に接続できるか確認している。  
NullセッションはWindowsXP以降ではデフォルトで無効にされている（詳細はrpcclient.md）。
### [get_domain_sid()](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L366)
```perl
my $command = "rpcclient -W '$global_workgroup' -U'$global_username'\%'$global_password' $global_target -c 'lsaquery' 2>&1";
```
`rpcclient`で「lsaquery」を実行しドメインのSIDを取得。取得したSIDからドメインに加入しているかどうか判別。


## [-U](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L1014)
rpcclientの「querydispinfo」「enumdomusers」でユーザ列挙
```perl
my $command = "rpcclient -W '$global_workgroup' -c querydispinfo -U'$global_username'\%'$global_password' -d 10 '$global_target' 2>&1";
~~~
$command = "rpcclient -W '$global_workgroup' -c enumdomusers -U'$global_username'\%'$global_password' -d 10 '$global_target' 2>&1";
```
`-d`も引数である場合、先程のコマンドで取得したRIDを用いて「queryuser」で各ユーザの詳細を調査  
「acb_info」を整形して人に分かりやすい形式で出力するのでわかりやすい
```perl
foreach my $rid (keys %rids) {
	get_user_details_from_rid($rid);
```
```perl
sub get_user_details_from_rid {
~~~
	return unless $global_detailed;
	my $command = "rpcclient -W '$global_workgroup' -U'$global_username'\%'$global_password' -c 'queryuser $rid' '$global_target' 2>&1";
~~~
	if ($acb_int & 0x00000001) {
	printf $pad . "%-25.25s: %s\n", "Account Disabled", "True";
	} else {
	printf $pad . "%-25.25s: %s\n", "Account Disabled", "False";
	}
```


## [-M](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L557)
Kaliに入っているenum4linuxは、対応してなさそう
```perl
sub enum_machines {
        print_heading("Machine Enumeration on $global_target");
        print_error("Not implemented in this version of enum4linux.\n");
}
```


## [-S](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L711)
smbclientの「-L」で共有を列挙（smbclientでは、NTハッシュでのログインができるがこれは対応していない）
```perl
my $command = "smbclient -W '$global_workgroup' -L //'$global_target' -U'$global_username'\%'$global_password' 2>&1";
```
列挙した共有にアクセス・コマンド実行しログインユーザのマッピング・読み取り権限を確認する
```perl
my $command = "smbclient -W '$global_workgroup' //'$global_target'/'$share' -U'$global_username'\%'$global_password' -c dir 2>&1";
```
`-A`も引数である場合、ディレクトリを作成し書き込み権限も確認する
ディレクトリが作成されれば、削除も行う
```perl
$command = "smbclient -W '$global_workgroup' //'$global_target'/'$share' -U'$global_username'\%'$global_password' -c 'mkdir $random_string' 2>&1";
```


## [-P](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L508)
polenumとrpcclientの「getdompwinfo」でパスワードポリシーの取得（polenumのほうが、取得できる情報量が多い）
```perl
my $command = "polenum '$global_username':'$global_password'\@'$global_target' 2>&1";
~~~
$command = "rpcclient -W '$global_workgroup' -U'$global_username'\%'$global_password' '$global_target' -c \"getdompwinfo\" 2>&1";
```


## [-G](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L612)
rpcclientの「enumalsgroups」でローカル・グローバルグループの列挙
```perl
foreach my $grouptype ("builtin", "domain") {
	# Get list of groups
	my $command = "rpcclient -W '$global_workgroup' -U'$global_username'\%'$global_password' '$global_target' -c 'enumalsgroups $grouptype' 2>&1";
```
net rpcコマンドで先程取得した各グループに所属するユーザを問い合わせる
```perl
$command = "net rpc group members '$groupname' -W '$global_workgroup' -I '$global_target' -U'$global_username'\%'$global_password' 2>&1\n";
```


## [-r](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L832)
rpcclientの「lookupnames」で指定ユーザのSIDを問い合わせる  
`-k`でユーザ名が指定されいなければ、デフォルトで`administrator,guest,krbtgt,domain admins,root,bin,non`の８ユーザ  
また、取得したSIDからRIDを除いたものを出力している  
カウントしたSIDが２回以上にならないとプロンプトに出力されないので、１回しかカウントしていないSIDは確認できない  
なぜenum4linuxがこの仕様なのかわからないがあんま使い勝手は良くなさそう
```perl
foreach my $known_username (@global_known_usernames) {
	my $command = "rpcclient -W '$global_workgroup' -U'$global_username'\%'$global_password' '$global_target' -c 'lookupnames $known_username' 2>&1";
```
さらにrpcclientの「lsaenumsid」でLSAのSIDを問い合わせる  
SIDのカウントは先程の「lookupnames」と合計して２回以上にならないとプロンプトに出力されないので、１回しかカウントしていないSIDは確認できない  
```perl
my $command = "rpcclient -W '$global_workgroup' -U'$global_username'\%'$global_password' '$global_target' -c lsaenumsid 2>&1";
```


## [-n](https://github.com/CiscoCXSecurity/enum4linux/blob/ee106b71ffda52c070057e10a9ee3f28e14db8df/enum4linux.pl#L359)
`nmblookup`コマンドを使用してノードステータスの問い合わせを実行。
```perl
my $output = `nmblookup -A '$global_target' 2>&1`;
```
実行結果。nmblookupの結果をノードの種類が分かるよう整形している。
```
 ================================( Nbtstat Information for 10.10.49.235 )================================

Looking up status of 10.10.49.235
        JON-PC          <00> -         B <ACTIVE>  Workstation Service
        WORKGROUP       <00> - <GROUP> B <ACTIVE>  Domain/Workgroup Name
        JON-PC          <20> -         B <ACTIVE>  File Server Service
        WORKGROUP       <1e> - <GROUP> B <ACTIVE>  Browser Service Elections
        WORKGROUP       <1d> -         B <ACTIVE>  Master Browser
        ..__MSBROWSE__. <01> - <GROUP> B <ACTIVE>  Master Browser

        MAC Address = 02-BC-B7-A3-F7-17
```
