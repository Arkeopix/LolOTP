use Modern::Perl;
use Digest::SHA qw(hmac_sha1_hex);
use IO::File;
use IO::All;

use constant PATH_FILE => $^O eq 'MSWin32' ? 'C:\Users\\' . getlogin . '\.LolOTP'
                                           : '/home/' . getpwuid($<) . '/.LolOTP';

sub decode_rfc3548 {
  $_ = shift;
  my( $l );

  tr|A-Z2-7|\0-\37|;
  $_=unpack('B*', $_);
  s/000(.....)/$1/g;
  $l=length;

  $_=substr($_, 0, $l & ~7) if $l & 7;

  $_=pack('B*', $_);
}

sub usable_secret {
  $_ = shift;

  $_ =~ s/ //g;
  return uc $_;
}

sub  hmac_sha1_hex_string {
  my ($data, $key) = map pack('H*', $_), @_;
  hmac_sha1_hex($data, $key);
}

sub get_code {
  my $fh = IO::File->new('<' . PATH_FILE)
    || die "Could not open file: $!";

  while ($_ = $fh->getline) {
    $_ =~ /(?<account_name>[a-zA-Z0-9]+)\s(?<key>[A-Z2-9\s]{39})/xms;
    my $key = unpack("H*", decode_rfc3548(usable_secret($+{key})));
    my $time = sprintf("%016x", int(time()/30));
    my $hmac = hmac_sha1_hex_string($time, $key);

    my $offset = sprintf("%d", hex(substr($hmac, -1)));

    my $p1 = 0 + sprintf("%d", hex(substr($hmac, $offset  *2, 8)));
    my $p2 = 0 + sprintf("%d", hex("7fffffff"));

    my $token = substr("".($p1 & $p2), -6);
    print "account: $+{account_name}\t code = $token\n";
  }
}

sub add_code {
  my ($account, $key) = @_;

  my $save_file = io->file(PATH_FILE)->all;
  if ($save_file !~ /\Q$account\E\s\Q$key\E/xms) {
    my $fh = IO::File->new('+>>' . PATH_FILE)
      || die "could not open file: $!";
    $fh->print($account . ' ' . $key . "\n");
    $fh->close;
  } else {
    print "account / key already exists\n";
  }
}

sub print_usage {
  print <<"EOT"
Usage: LolOTP command [args]
where command is one of the folowing:
  code:                    just print a list of codes suitable for authentication with google services
  add [account_name key]:  Add a new account and associate a key to it
EOT
}

my $args = $#ARGV + 1;
if ($args < 1) {
  print_usage;
  exit;
}
my $arg_string = join(' ', @ARGV);

get_code if $arg_string eq 'code';
add_code($+{account_name}, $+{key})
  if $arg_string =~ /add\s(?<account_name>[a-zA-Z0-9]+)\s(?<key>[A-Z2-9\s]{39})/xms;
