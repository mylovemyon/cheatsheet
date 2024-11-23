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


## [menu](https://github.com/Hackplayers/evil-winrm/blob/ffe958c841da655ba3c44740ca22aa0eee9fc5ed/evil-winrm.rb#L836)
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
