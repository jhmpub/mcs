# Abstract: use ssh to run a cmd on a remote host
#
# https://metacpan.org/pod/Net::SSH2
#
# Asymmetric Key Authentication Process
# 1. Generate the Key Pair: The client generates a private/public key pair 
#    (typically RSA or Ed25519) on their local machine using ssh-keygen.
# 2. Save Keys: The private key (id_rsa) is saved in the local ~/.ssh/ 
#    directory and should never be shared. The public key (id_rsa.pub) is 
#    stored in the same directory.
# 3. Copy Public Key to Server: The client copies the public key to the 
#    remote server, adding it to the ~/.ssh/authorized_keys file for the 
#    desired user. This is typically done with 
#    ssh-copy-id -i ~/.ssh/id_rsa.pub user@server.
# 4. Initiate Connection: The client initiates an SSH connection 
#    (ssh user@server).
# 5. Challenge Generation: The server finds the client's public key in the 
#    authorized_keys file. It generates a random challenge (number) and 
#    encrypts it using the client's public key.
# 6. Challenge Transmission: The encrypted challenge is sent to the client.  
# 7. Decryption and Verification: The client receives the encrypted challenge
#    and decrypts it using their private key.
# 8. Signing/Hash Response: The client combines the decrypted challenge with 
#    the active shared session key and calculates an MD5 hash of this value. 
#    This hash is sent back to the server.
# 9. Authentication Success: The server performs the same hash calculation. 
#    If the hashes match, the server verifies the client's identity and allows 
#    access.
#
# Key Considerations
#   Passphrase: During key generation, it is highly recommended to protect 
#     private keys with a passphrase in case the client device is stolen.
#   Security: Asymmetric key authentication is more secure than passwords 
#     because a private key never leaves the client machine.
#   Automation: strict key file security is important because private key
#     files typically don't have a passphrase  
#
# n.b. OpenSSH Client and OpenSSH Server are not preinstalled on Windows
#      Settings > System > Optional features > View features > 
#      See available features > Available features: ssh
#
# 2026 May 09 jhm original creation
#

use Getopt::Long;
my $usage = "\nusage: sshCmd [-q] <remote host> <cmd>\n";
my ($help, $quiet) = (undef, undef);
die $usage unless GetOptions (
  'help|?' => \$help,
  'quiet' => \$quiet
);
die "\nusage: sshCmd [-q] <remote host> <cmd>\n" unless @ARGV==2;
my ($remoteHost, $cmd) = ($ARGV[0], $ARGV[1]);
sshCmd($remoteHost, $cmd);

sub sshCmd {
  my ($remoteHost, $cmd) = @_;

  use Net::SSH2;
  my $ssh2 = Net::SSH2->new();

  # establish connection
  $ssh2->connect($remoteHost) 
    or $ssh2->die_with_error;
  
  # verify the remote host's public key
  $ssh2->check_hostkey('tofu', $ENV{HOME}."/.ssh/known_hosts")
    or $ssh2->die_with_error;
  
  # authenticate
  $ssh2->auth_publickey($ENV{USERNAME}, $ENV{HOME}."/.ssh/id_rsa.pub", $ENV{HOME}."/.ssh/id_rsa")
    or $ssh2->die_with_error;
  
  # create a bidirectional channel
  my $chan = $ssh2->channel()
    or $ssh2->die_with_error;
  
  # exec cmd 
  $chan->exec($cmd)
    or $ssh2->die_with_error;
  
  # report results
  unless ($quiet) {
    print while <$chan>;
    print "EXIT CODE: ", $chan->exit_status, "\n";
  }

  $chan->close;
}

