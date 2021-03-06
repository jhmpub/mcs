# redirect output to ..\..\agent\remote\itach.h

$0 =~ /(\w+)(\.?\w+$)/;
$filename = $1.$2;
$id=1;

@mode = qw(tv dvd vcr);

open CAPTURE, ("<itach_codes.cmd") or die "error: unable to open itach_codes.cmd\n";
open RENUM, (">itach_codes.renum") or die "error: unable to open itach_codes.renum\n";
while (<CAPTURE>) {
   if (/rem\s+(\w+)/) {
      $label = $enum = $1;
      $enum =~ s/([A-Z])/_\1/g;
      $enum =~ tr/a-z/A-Z/;
      $enum = "ITACH".$enum;
      $enum{$label} = $enum;
      $mode=0;
   }
   if (/^sendir/ || /NULL/) {
      if (/^sendir/) {
         s/1:3,\d+/1:3,$id/;  # give each sendir a unique numeric identifier
         $id++;
      }   
      if ($mode==0) {
         $tv{$label} = $_;
      } 
      elsif ($mode==1) {
         $dvd{$label} = $_;
      }
      else {
         $vcr{$label} = $_;
      }   
      $mode++;
   }
   print RENUM $_;
}
close CAPTURE;
close RENUM;
rename "itach_codes.renum", "itach_codes.cmd" or die "error: ".$!."\n";

print 
   "////\n".
   "//// itach wifi to ir codes\n".
   "////\n".
   "//// This file is automatically generated by mcs/util/itach/$filename\n".
   "////\n".
   "\n".
   "enum remote {\n".
   "   TV_MODE,\n".
   "   DVD_MODE,\n".
   "   VCR_MODE,\n".
   "   REMOTE_MODES,\n".
   "   UNDEFINED_MODE\n".
   "};\n".
   "\n".   
   "struct irTx {\n".
   "   const char * code;   // itach ir codes, NULL if n/a\n".
   "   const char * sz;     // command message and debug description\n".
   "};\n\n".
   "struct irTxCmd {\n".
   "   int id;                             // itachId enum index\n".
   "   struct irTx remote[REMOTE_MODES];   // remote enum index\n".
   "};\n".
   "\n".
   "enum itachId {\n";

foreach $label (sort keys %tv) {
   print "   ".$enum{$label}.",\n";
}

print "   ITACH_IDS\n};\n\n\n".
      "// irTxGroup structures indexed by itachId enum\n".
      "// irTx member structures indexed by remote enum\n".
      "static struct irTxCmd irTxCmd[ITACH_IDS] = {\n";

$id=1;  # insure a unique ID for each command
foreach $label (sort keys %tv) {
   $szCmd = $label;
   $szCmd =~ s/([A-Z])/ \1/g;
   $szCmd =~ tr/A-Z/a-z/;
   $szCmd =~ s/^ //;
   $szCmd[0] = $szCmd[1] = $szCmd[2] = $szCmd;
   $cmds[0] = $tv{$label}; $cmds[1]=$dvd{$label}; $cmds[2]=$vcr{$label};
   next unless $cmds[0] && $cmds[1] && $cmds[2];
   print "   {\n".
         "      ".$enum{$label}.",\n".
         "      {\n";
   for ($k=0; $k<3; $k++) {
      $nl = $k==2 ? "\n" : ",\n\n";
      $cr = $k==2 ? "\\r\"\n" : "\\r\",\n\n";
      chomp $cmds[$k];
      print "         {\n";
      if ($cmds[$k] eq "NULL") {
         print "            NULL,\n\n";
         $szCmd[$k] .= " (not defined)";
      } else {
         $cmds[$k] =~ s/1:3,\d+,/1:3,$id,/; 
         $id++;
         for ($l=0; $l < length($cmds[$k]);$l+=62) {
            $line = substr($cmds[$k], $l, 62);
            print "            \"".$line;
            print $l+62>=length($cmds[$k]) ? "\\r\",\n\n" : "\"\n";
         }
      }
      
      # prepend mode if command is mode specific 
      $mode = ($szCmd[$k] eq "mute" || 
               $szCmd[$k] eq "volume down" || 
               $szCmd[$k] eq "volume up" ||
               $szCmd[$k] =~ /hdmi source/) ? "" : $mode[$k]." ";

      # ch+ ch- does a chapter next/back in dvd mode
      $szCmd[$k] =~ s/channel up/chapter next/ if $mode[$k] eq "dvd";
      $szCmd[$k] =~ s/channel down/chapter back/ if $mode[$k] eq "dvd";

      print "            \"".$mode.$szCmd[$k]."\"\n";
      print "         }".$nl;
   }
   print "      }\n";
   print "   },\n";
}      
print "};\n\n";
      
   
    
   