```
Usage: [-?fMSTrAV] [-?|--help] [--usage] [-B|--broadcast=BROADCAST-ADDRESS] [-f|--flags] [-U|--unicast=STRING] [-M|--master-browser] [--recursion]
        [-S|--status] [-T|--translate] [-r|--root-port] [-A|--lookup-by-ip] [-d|--debuglevel=DEBUGLEVEL] [--debug-stdout] [-s|--configfile=CONFIGFILE]
        [--option=name=value] [-l|--log-basename=LOGFILEBASE] [--leak-report] [--leak-report-full] [-R|--name-resolve=NAME-RESOLVE-ORDER]
        [-O|--socket-options=SOCKETOPTIONS] [-m|--max-protocol=MAXPROTOCOL] [-n|--netbiosname=NETBIOSNAME] [--netbios-scope=SCOPE] [-W|--workgroup=WORKGROUP]
        [--realm=REALM] [-V|--version] <NODE> ..
```

## -Aオプション
ノードステータスの問い合わせを実行。
```
└─$ nmblookup -A 10.10.82.228
Looking up status of 10.10.82.228
        JON-PC          <00> -         B <ACTIVE> 
        WORKGROUP       <00> - <GROUP> B <ACTIVE> 
        JON-PC          <20> -         B <ACTIVE> 
        WORKGROUP       <1e> - <GROUP> B <ACTIVE> 
        WORKGROUP       <1d> -         B <ACTIVE> 
        ..__MSBROWSE__. <01> - <GROUP> B <ACTIVE> 

        MAC Address = 02-A4-17-25-12-B9
```
<>内の数値が、ノードタイプを表す。

NetBIOS over TCP/IPについて  
[Windowsに潜むデフォルト設定の落とし穴（中）](https://xtech.nikkei.com/it/members/NBY/techsquare/20021129/2/)  
[Vistaの目指すネットワークの姿（2）－－NetBIOSなしでWindowsネットを実現](https://xtech.nikkei.com/it/article/COLUMN/20080916/314893/)  
[Windowsのファイル共有に用いられるNetBIOSとは？](https://atmarkit.itmedia.co.jp/fpc/experiments/008bbrouter_sec/block_share_02.html)
