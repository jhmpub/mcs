# raas is a generic utility to Run an application As A Service
#
# usage: raas [<executable file>]
#
# Optionally create a "raas.dat" file with the following parameters:
#
# Application=<executable file>
# Description=<summary of service function>
# DisplayName=<long service name with spaces>
# ServiceName=<short service name with no spaces>
#
# If Application is not specified in raas.dat the exe name must be specified
# as a command line argument.  If ServiceName is not specified it will be
# set to the Application value without an extension.  Description and DisplayName
# are optional values.
#
# Note: An installed service can be removed using the system controller utility:
#       sc delete <ServiceName>
#
# 2015 Dec 13 jhm original creation

use Win32::TieRegistry;
use Cwd;

$appDir=cwd();
$appDir =~ s/\//\\/g;
$utilsDir = $ENV{SYSTEMDRIVE}."\\utils\\win32\\";
$srvany = $utilsDir."srvany.exe";
$instsrv = $utilsDir."instsrv.exe";

open RAAS, "<raas.dat" if (-f "raas.dat");
while (<RAAS>) {
   /([\.\w]+)=([-\.\w ]+)/;
   $raas{$1}=$2;
}

die "\nUsage: raas [<executable file>]\n\n" unless $raas{'Application'} || $ARGV[0];
$raas{'Application'} = $ARGV[0] if $ARGV[0];
unless ($raas{'ServiceName'}) {
   $raas{'ServiceName'} = $raas{'Application'};
   $raas{'ServiceName'} =~ s/\.[^\.]+$//;
}   

print "\nInstalling ".$raas{'ServiceName'}." as a service ";

open RESULT, $instsrv." ".$raas{'ServiceName'}." ".$srvany." | ";

while (<RESULT>) {
   $success=1 if /SUCCESS/;
   push @result, $_;
}

if ($success) {
   # add a service summary and long service name information to the registry for the services panel
   $services = new Win32::TieRegistry "LMachine\\SYSTEM\\CurrentControlSet\\Services",
      { Access=>KEY_ALL_ACCESS, Delimiter=>"\\" };
   $services->CreateKey($raas{'ServiceName'});
   $service = new Win32::TieRegistry "LMachine\\SYSTEM\\CurrentControlSet\\Services\\".$raas{'ServiceName'},
      { Access=>KEY_ALL_ACCESS, Delimiter=>"\\" };
   $service->SetValue("Description", $raas{'Description'}, "REG_SZ") if $raas{'Description'}; 
   $service->SetValue("DisplayName", $raas{'DisplayName'}, "REG_SZ") if $raas{'DisplayName'}; 

   # set the startup directory to the current working directory and fully qualified path to the application
   $service->CreateKey("Parameters");
   $parameters = new Win32::TieRegistry 
      "LMachine\\SYSTEM\\CurrentControlSet\\Services\\".$raas{'ServiceName'}."\\Parameters",
      { Access=>KEY_ALL_ACCESS, Delimiter=>"\\" };
   $parameters->SetValue("AppDirectory", $appDir); 
   $parameters->SetValue("Application", $appDir."\\".$raas{'Application'});

   print "succeeded\n\n".
         "Notes: 1. go to the services panel and change the ".$raas{'ServiceName'}." Log On\n".
         "          to Local System account\n".
         "       2. the \"".$raas{'DisplayName'}."\" display name will not appear in the\n".
         "          services panel until after reboot\n";
} else {
   print "failed\n";
   foreach (@result) { print; }
}
             





