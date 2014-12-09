# D/OMG(4086): Account decoded secret: 20
# 12-08 18:19:42.572: D/OMG(4086): Decoded secret: e739ce739ce739ce739ce739ce739ce739ce739c
# 12-08 18:19:42.572: D/OMG(4086): Time: 47268639
# 12-08 18:19:42.573: D/OMG(4086): Time as byte array: 0000000002d1431f
# 12-08 18:19:42.584: D/OMG(4086): HMAC: ec553081a7f618364b797ed4db66c8017a03445e
# 12-08 18:19:42.584: D/OMG(4086): Offset: 14
# 12-08 18:19:42.584: D/OMG(4086): Chunk: c8017a03
# 12-08 18:19:42.584: D/OMG(4086): Code: 056323

use Modern::Perl;
use Digest::SHA qw(hmac_sha1_hex);

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
  my ($secret) = @_;

  $secret =~ s/ //g;
  return uc $secret;
}

sub  hmac_sha1_hex_string {
  my ($data, $key) = map pack('H*', $_), @_;
  hmac_sha1_hex($data, $key);
}

sub get_code {
  my $secret = shift;

  my $key = unpack("H*", decode_rfc3548(usable_secret($secret)));
  my $time = sprintf("%016x", int(time()/30));
  my $hmac = hmac_sha1_hex_string($time, $key);

  my $offset = sprintf("%d", hex(substr($hmac, -1)));

  my $p1 = 0 + sprintf("%d", hex(substr($hmac, $offset  *2, 8)));
  my $p2 = 0 + sprintf("%d", hex("7fffffff"));

  my $token = substr("".($p1 & $p2), -6);
  return $token;
}

my $secret = "4444 4444 4444 4444 4444 4444 4444 4444";
my $code = get_code($secret);
say $code;
