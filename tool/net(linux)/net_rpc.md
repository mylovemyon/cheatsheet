## help
```
Usage:
net rpc audit           Modify global audit settings
net rpc info            Show basic info about a domain
net rpc join            Join a domain
net rpc oldjoin         Join a domain created in server manager
net rpc testjoin        Test that a join is valid
net rpc user            List/modify users
net rpc password        Change a user password
net rpc group           List/modify groups
net rpc share           List/modify shares
net rpc file            List open files
net rpc printer         List/modify printers
net rpc changetrustpw   Change trust account password
net rpc trustdom        Modify domain trusts
net rpc abortshutdown   Abort a remote shutdown
net rpc shutdown        Shutdown a remote server
net rpc vampire         Sync a remote NT PDC's data into local passdb
net rpc getsid          Fetch the domain sid into local secrets.tdb
net rpc rights          Manage privileges assigned to SID
net rpc service         Start/stop/query remote services
net rpc registry        Manage registry hives
net rpc shell           Open interactive shell on remote server
net rpc trust           Manage trusts
net rpc conf            Configure a remote samba server
```
```
net [<method>] group [misc. options] [targets]
        List user groups

net rpc group LIST [global|local|builtin]* [misc. options]
        List specific user groups

net [<method>] group DELETE <name> [misc. options] [targets]
        Delete specified group

net [<method>] group ADD <name> [-C comment] [-c container] [misc. options] [targets]
        Create specified group

net rpc group MEMBERS <name>
        List Group Members


net rpc group ADDMEM <group> <member>
        Add Group Members


net rpc group DELMEM <group> <member>
        Delete Group Members

Valid methods: (auto-detected if not specified)
        ads                             Active Directory (LDAP/Kerberos)
        rpc                             DCE-RPC
        rap                             RAP (older systems)

Valid targets: choose one (none defaults to localhost)
        -S|--server=<server>                    server name
        -I|--ipaddress=<ipaddr>                 address of target server
        -w|--target-workgroup=<wg>              target workgroup or domain

Valid misc options are:
        -p|--port=<port>                        connection port on target
        --myname=<name>                         client name
        --long                                  Display full information

Valid common options are:
        -d|--debuglevel=<level>                 debug level (0-10)
        --debug-stdout                          Send debug output to standard output
        --configfile=<path>                     pathname of smb.conf file
        --option=name=value                     Set smb.conf option from command line
        -l|--log-basename=LOGFILEBASE           Basename for log/debug files
        --leak-report                           enable talloc leak reporting on exit
        --leak-report-full                      enable full talloc leak reporting on exit
        -V|--version                            Print samba version information

Valid connection options are:
        -R|--name-resolve=NAME-RESOLVE-ORDER    Use these name resolution services only
        -O|--socket-options=SOCKETOPTIONS       socket options to use
        -m|--max-protocol=MAXPROTOCOL           Set max protocol level
        -n|--netbiosname=NETBIOSNAME            Primary netbios name
        --netbios-scope=SCOPE                   Use this Netbios scope
        -W|--workgroup=WORKGROUP                Set the workgroup name
        --realm=REALM                           Set the realm name

Valid credential options are:
        -U|--user=[DOMAIN/]USERNAME[%PASSWORD]  Set the network username
        -N|--no-pass                            Don't ask for a password
        --password=STRING                       Set a password
        --pw-nt-hash                            The supplied password is the NT hash
        -A|--authentication-file=FILE           Get the credentials from a file
        -P|--machine-pass                       Use stored machine account password
        --simple-bind-dn=DN                     DN to use for a simple bind
        --use-kerberos=desired|required|off     Use kerberos authentication
        --use-krb5-ccache=CCACHE                Credentials cache location for Kerberos
        --use-winbind-ccache                    Use the winbind ccache for authentication
        --client-protection=sign|encrypt|off    Configure used protection for client connections
        -C or --comment=<comment>       descriptive comment (for add only)
        -c or --container=<container>   LDAP container, defaults to cn=Users (for add in ADS only)
        -L or --localgroup              When adding groups, create a local group (alias)
```
## audit
TryHackmeの[Attacktive Directory](https://tryhackme.com/r/room/attacktivedirectory)でお試し
backupユーザはドメインユーザ権限のみをもつ（Domain Admin権限はなし）
```
Usage:
net rpc audit get             View configured auditing settings
net rpc audit set             Set auditing policies
net rpc audit enable          Enable auditing
net rpc audit disable         Disable auditing
net rpc audit list            List configured auditing settings
```
audit listはユーザ権限ではアクセス拒否された
```zsh
└─$ net rpc audit list -U thm-ad/administrator%0e0363213e37b94221497260b0bcb4fc --pw-nt-hash -I 10.10.131.84
Auditing:               Enabled
Auditing categories:    9
Auditing settings:
        System Events                 None
        Logon events                  None
        Object Access                 None
        Privilege Use                 None
        Process Tracking              None
        Policy Change                 None
        Account Management            None
        Directory service access      None
        Account logon events          None
```
