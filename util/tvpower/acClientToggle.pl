# Abstract: if a window titled "Audio Control" OR "Surround Control" are found,
#   a "tv power off" message is sent to the audio receiver agent
#   instead of launching the audio control utility.  The audio receiver
#   agent broadcasts the "tv power off" message to all registered
#   audio control clients.  Exiting audio control clients will power off an
#   attached LG TV and deregister from the audio receiver agent.  
#
# 2026 May 7 jhm original creation

require 5.026;
$|=1;
use Getopt::Long;
use Win32::GUI;

# begin site specific variables
my $acClientDir = $ENV{DATA_DRIVE}."\\home\\jhm\\src\\mcs\\util\\acclient";
my $audioControlMessage = $ENV{DATA_DRIVE}."\\home\\jhm\\src\\mcs\\client\\msg\\msg.exe";
my $javaw = $ENV{PROGRAM_DRIVE}."\\pkg\\win32\\java\\jdk\\".$ENV{JAVA_VERSION}."\\bin\\javaw.exe";
my $acAgentHost = "jimson";
# end site specific variables

chdir $acClientDir;

$usage = "\nusage: acClientToggle.pl [-d]\n".
   "-d   debug - print commands without executing\n";
my ($debug, $info, $surround);
die $usage unless GetOptions (
  'debug' => \$debug
);

(isRunning("Audio Control") || isRunning("Surround Control")) ? 
   sendAudioControlMessage("tv power off") : launchAudioControlClient();


sub launchAudioControlClient {
  my $acClient = $acClientDir."\\acclient.jar";

  foreach $file ($javaw, $acClient) {
    unless (-f $file) {
      Win32::GUI::MessageBox(undef, "file not found\n".$file, "Error", MB_OK) unless -f $file;
      exit;
    }
  }

  my @cmd = (
    $javaw, 
    "-jar", 
    $acClient, 
    ($surround ? "-s":""), 
    "-t");    # -t informs acClient to run minimized
  print "\n" if $debug;  
  foreach (@cmd) {print $_." " if $debug};
  print "\n" if $debug;
  exec (@cmd) unless $debug;
}


sub isRunning {
  my $title = shift;
  my $isRunning = "isRunning.exe";
  unless (-f $isRunning) { 
     Win32::GUI::MessageBox(undef, "file not found\n".$acClientDir."\\".$file, "Error", MB_OK) unless -f $isRunning;
     exit;
  }
  my @cmd = ($isRunning, ("\"".$title."\""));
  foreach (@cmd) {print $_." " if $debug};
  return $debug ? 1 : system (@cmd);
}


sub sendAudioControlMessage {
  my $msg = shift;
  my $acAgentPort = "2200";
  my @cmd = ($audioControlMessage, $acAgentHost, $acAgentPort, "\"".$msg."\"");
  foreach (@cmd) {print $_." " if $debug};
  exec (@cmd) unless $debug;
}



