#!/usr/bin/perl

sub min
{
    my ($a, $b) = @_;

    return ($a > $b ? $b : $a)
}

sub explore 
{
    my $filea = $ARGV[0];

    open (OPF, "< $filea");

    while (<OPF>)
    {
	while (/\#(..)(..)(..)/)
	{
	    $a = sprintf "%02x", min((hex($1) - @ARGV[4]) * $ARGV[1] + @ARGV[4], 255);
	    $b = sprintf "%02x", min((hex($2) - @ARGV[5]) * $ARGV[2] + @ARGV[5], 255);
	    $c = sprintf "%02x", min((hex($3) - @ARGV[6]) * $ARGV[3] + @ARGV[6], 255);

	    s/\#$1$2$3/$a$b$c/;
	}
	s/([0-9a-f]{6})/\#$1/g;
	print $_;
    }

    close (OPF);
}

explore();
