```
Usage: [-?fMSTrAV] [-?|--help] [--usage] [-B|--broadcast=BROADCAST-ADDRESS] [-f|--flags] [-U|--unicast=STRING] [-M|--master-browser] [--recursion]
        [-S|--status] [-T|--translate] [-r|--root-port] [-A|--lookup-by-ip] [-d|--debuglevel=DEBUGLEVEL] [--debug-stdout] [-s|--configfile=CONFIGFILE]
        [--option=name=value] [-l|--log-basename=LOGFILEBASE] [--leak-report] [--leak-report-full] [-R|--name-resolve=NAME-RESOLVE-ORDER]
        [-O|--socket-options=SOCKETOPTIONS] [-m|--max-protocol=MAXPROTOCOL] [-n|--netbiosname=NETBIOSNAME] [--netbios-scope=SCOPE] [-W|--workgroup=WORKGROUP]
        [--realm=REALM] [-V|--version] <NODE> ..
```

## -Aオプション
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
