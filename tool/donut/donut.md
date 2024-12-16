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


## makefile
windowsの場合は  
`nmake -f makefile.msvc`（「nmake」コマンドでターゲットを指定しない場合、一番最初に書かれたターゲット（この場合は「donut」ターゲット）が実行される）
