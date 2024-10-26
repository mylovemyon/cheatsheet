https://github.com/CiscoCXSecurity/enum4linux  
Perlで実装  
https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/manage/managing-rid-issuance
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


## デフォルトオプション 
コード一部を確認してみると、オプション指定しなくても実行される関数がある。
```perl
# Basic enumeration, check session
get_workgroup();
get_nbtstat()          if $opts{'n'};
make_session();
get_ldapinfo()         if $opts{'l'};
get_domain_sid();
get_os_info()          if $opts{'o'};
```
### get_workgroup()
```
# Get workgroup from nbstat info - we need this for lots of rpcclient calls
sub get_workgroup {
	print_heading("Enumerating Workgroup/Domain on $global_target");
	print_verbose("Attempting to get domain name with command: nmblookup -A '$global_target'\n") if $verbose;

	# Workgroup might already be known - e.g. from command line or from get_os_info()
	unless ($global_workgroup) {
		print "target is tainted\n" if tainted($global_target); # DEBUG
		$global_workgroup = `nmblookup -A '$global_target'`; # Global var.  Erg!
		($global_workgroup) = $global_workgroup =~ /\s+(\S+)\s+<00> - <GROUP>/s;
		unless (defined($global_workgroup)) {
			# dc.example.org. hostmaster.example.org. 1 900 600 86400 3600
			$global_workgroup = `dig +short 0.in-addr.arpa`;
			($global_workgroup) = $global_workgroup =~ /.*\. hostmaster\.(.*?)\. .*/s;
			if (defined($global_workgroup)) {
				print "[+] Domain guessed: $global_workgroup\n";
			} else {
				$global_workgroup = "WORKGROUP";
				print_error("Can\'t find workgroup/domain\n");
				print "\n";
				return;
			}
		}
		unless (defined($global_workgroup) and $global_workgroup =~ /^[A-Za-z0-9_\.\-]+$/) {
			print_error("Workgroup \"$global_workgroup\"contains some illegal characters\n");
			exit 1;
		}
	}
	print_plus("Got domain/workgroup name: $global_workgroup\n");
}
```

## -nオプション  
nmblookupコマンドを使用してノードステータスの問い合わせを実行。
```
sub get_nbtstat {
	print_heading("Nbtstat Information for $global_target");
	my $output = `nmblookup -A '$global_target' 2>&1`;
	$output = nbt_to_human($output);
	print "$output\n";
}
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
