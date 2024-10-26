```
Usage: [-?fMSTrAV] [-?|--help] [--usage] [-B|--broadcast=BROADCAST-ADDRESS] [-f|--flags] [-U|--unicast=STRING] [-M|--master-browser] [--recursion]
        [-S|--status] [-T|--translate] [-r|--root-port] [-A|--lookup-by-ip] [-d|--debuglevel=DEBUGLEVEL] [--debug-stdout] [-s|--configfile=CONFIGFILE]
        [--option=name=value] [-l|--log-basename=LOGFILEBASE] [--leak-report] [--leak-report-full] [-R|--name-resolve=NAME-RESOLVE-ORDER]
        [-O|--socket-options=SOCKETOPTIONS] [-m|--max-protocol=MAXPROTOCOL] [-n|--netbiosname=NETBIOSNAME] [--netbios-scope=SCOPE] [-W|--workgroup=WORKGROUP]
        [--realm=REALM] [-V|--version] <NODE> ..
```

## -Aオプション
UDP137番ポートに対して、ノードステータスの問い合わせを実行。
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
ノードタイプの種類はこちら（enum4linuxから抜粋）
```
["__MSBROWSE__", "01", 0, "Master Browser"],
["INet~Services", "1C", 0, "IIS"],
["IS~", "00", 1, "IIS"],
["", "00", 1, "Workstation Service"],
["", "01", 1, "Messenger Service"],
["", "03", 1, "Messenger Service"],
["", "06", 1, "RAS Server Service"],
["", "1F", 1, "NetDDE Service"],
["", "20", 1, "File Server Service"],
["", "21", 1, "RAS Client Service"],
["", "22", 1, "Microsoft Exchange Interchange(MSMail Connector)"],
["", "23", 1, "Microsoft Exchange Store"],
["", "24", 1, "Microsoft Exchange Directory"],
["", "30", 1, "Modem Sharing Server Service"],
["", "31", 1, "Modem Sharing Client Service"],
["", "43", 1, "SMS Clients Remote Control"],
["", "44", 1, "SMS Administrators Remote Control Tool"],
["", "45", 1, "SMS Clients Remote Chat"],
["", "46", 1, "SMS Clients Remote Transfer"],
["", "4C", 1, "DEC Pathworks TCPIP service on Windows NT"],
["", "52", 1, "DEC Pathworks TCPIP service on Windows NT"],
["", "87", 1, "Microsoft Exchange MTA"],
["", "6A", 1, "Microsoft Exchange IMC"],
["", "BE", 1, "Network Monitor Agent"],
["", "BF", 1, "Network Monitor Application"],
["", "03", 1, "Messenger Service"],
["", "00", 0, "Domain/Workgroup Name"],
["", "1B", 1, "Domain Master Browser"],
["", "1C", 0, "Domain Controllers"],
["", "1D", 1, "Master Browser"],
["", "1E", 0, "Browser Service Elections"],
["", "2B", 1, "Lotus Notes Server Service"],
["IRISMULTICAST", "2F", 0, "Lotus Notes"],
["IRISNAMESERVER", "33", 0, "Lotus Notes"],
['Forte_$ND800ZA', "20", 1, "DCA IrmaLan Gateway Server Service"]
```

NetBIOS over TCP/IPについて  
[Windowsに潜むデフォルト設定の落とし穴（中）](https://xtech.nikkei.com/it/members/NBY/techsquare/20021129/2/)  
[Vistaの目指すネットワークの姿（2）－－NetBIOSなしでWindowsネットを実現](https://xtech.nikkei.com/it/article/COLUMN/20080916/314893/)  
[Windowsのファイル共有に用いられるNetBIOSとは？](https://atmarkit.itmedia.co.jp/fpc/experiments/008bbrouter_sec/block_share_02.html)  
[NetBIOSネームサービスでネットワーク内の端末をリアルタイムに列挙する](https://codezine.jp/article/detail/192)
